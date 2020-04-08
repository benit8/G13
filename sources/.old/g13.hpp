/*
** EPITECH PROJECT, 2019
** G13
** File description:
** g13.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

#include "helper.hpp"

#include <boost/log/trivial.hpp>

#include <libusb-1.0/libusb.h>

#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <linux/uinput.h>
#include <fcntl.h>

////////////////////////////////////////////////////////////////////////////////

namespace G13
{

////////////////////////////////////////////////////////////////////////////////

#define LOG(level, message) BOOST_LOG_TRIVIAL(level) << message
#define OUT(message) BOOST_LOG_TRIVIAL(info) << message

enum {
	INTERFACE = 0,
	KEY_ENDPOINT = 1,
	LCD_ENDPOINT = 2,
	REPORT_SIZE = 8,
	NUM_KEYS = 40,
	LCD_BUFFER_SIZE = 960,
	VENDOR_ID = 0x046D, // 1133
	PRODUCT_ID = 0xC21C, // 49692
}


const size_t LCD_COLUMNS = 160;
const size_t LCD_ROWS = 48;
const size_t LCD_BYTES_PER_ROW = LCD_COLUMNS / 8;
const size_t LCD_BUF_SIZE = LCD_ROWS * LCD_BYTES_PER_ROW;
const size_t LCD_TEXT_CHEIGHT = 8;
const size_t LCD_TEXT_ROWS = LCD_COLUMNS / LCD_TEXT_CHEIGHT;

enum stick_mode_t
{
	STICK_ABSOLUTE,
	STICK_RELATIVE,
	STICK_KEYS,
	STICK_CALCENTER,
	STICK_CALBOUNDS,
	STICK_CALNORTH
};

typedef int LinuxKeyValue_t;
const LinuxKeyValue_t BAD_KEY_VALUE = -1;

typedef int KeyIndex;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

using Helper::repr;
using Helper::find_or_throw;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class G13_Action;
class G13_Stick;
class G13_LCD;
class G13_Profile;
class G13_Device;
class G13_Manager;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class G13_CommandException : public std::exception
{
public:
	G13_CommandException(const std::string &reason)
	: m_reason(reason)
	{}

	virtual ~G13_CommandException() throw ()
	{}

	virtual const char *what() const throw () {
		return m_reason.c_str();
	}

private:
	std::string m_reason;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*
 * ! holds potential actions which can be bound to G13 activity
 */
class G13_Action
{
public:
	G13_Action(G13_Device &keypad)
	: m_keypad(keypad)
	{}

	virtual ~G13_Action();

	virtual void act(G13_Device &, bool is_down) = 0;
	virtual void dump(std::ostream &) const = 0;

	void act(bool is_down) {
		act(keypad(), is_down);
	}

	G13_Device &keypad() { return m_keypad; }
	const G13_Device & keypad() const { return m_keypad; }

	G13_Manager &manager();
	const G13_Manager &manager() const;

private:
	G13_Device &m_keypad;
};


/*
 * ! action to send one or more keystrokes
 */
class G13_Action_Keys : public G13_Action
{
public:
	G13_Action_Keys(G13_Device & keypad, const std::string &keys);
	virtual ~G13_Action_Keys();

	virtual void act(G13_Device &, bool is_down);
	virtual void dump(std::ostream &) const;

private:
	std::vector<LinuxKeyValue_t> m_keys;
};

/*!
 * action to send a string to the output pipe
 */
class G13_Action_PipeOut : public G13_Action
{
public:
	G13_Action_PipeOut(G13_Device & keypad, const std::string &out);
	virtual ~G13_Action_PipeOut();

	virtual void act(G13_Device &, bool is_down);
	virtual void dump(std::ostream &) const;

private:
	std::string m_out;
};

/*!
 * action to send a command to the g13
 */
class G13_Action_Command : public G13_Action
{
public:
	G13_Action_Command(G13_Device & keypad, const std::string &cmd);
	virtual ~G13_Action_Command();

	virtual void act(G13_Device &, bool is_down);
	virtual void dump(std::ostream &) const;

private:
	std::string m_cmd;
};


typedef boost::shared_ptr<G13_Action> G13_ActionPtr;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

template <class PARENT_T>
class G13_Actionable
{
public:
	G13_Actionable(PARENT_T &parent_arg, const std::string &name)
	: m_parent_ptr(&parent_arg)
	, m_name(name)
	{}

	virtual ~G13_Actionable()
	{
		m_parent_ptr = 0;
	}

	G13_ActionPtr action() const { return m_action; }
	const std::string &name() const { return m_name; }
	      PARENT_T &parent()       { return *m_parent_ptr; }
	const PARENT_T &parent() const { return *m_parent_ptr; }
	      G13_Manager &manager()       { return m_parent_ptr->manager(); }
	const G13_Manager &manager() const { return m_parent_ptr->manager(); }

	virtual void set_action(const G13_ActionPtr &action) {
		m_action = action;
	}

protected:
	std::string m_name;
	G13_ActionPtr m_action;

private:
	PARENT_T *m_parent_ptr;
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/*! manages the bindings for a G13 key
 *
 */
class G13_Key : public G13_Actionable<G13_Profile>
{
public:
	void dump(std::ostream &o) const;
	KeyIndex index() const { return m_index.index; }

	void parse_key(unsigned char *byte, G13_Device *g13);

protected:
	struct KeyIndex
	{
		KeyIndex(int key)
		: index(key)
		, offset(key / 8)
		, mask(1 << (key % 8))
		{}

		int index;
		unsigned char offset;
		unsigned char mask;
	};

	// G13_Profile is the only class able to instantiate G13_Keys
	friend class G13_Profile;

	G13_Key(G13_Profile &mode, const std::string &name, int index)
	: G13_Actionable<G13_Profile>(mode, name)
	, m_index(index)
	, m_should_parse(true)
	{
	}

	G13_Key(G13_Profile &mode, const G13_Key &key)
	: G13_Actionable<G13_Profile>(mode, key.name())
	, m_index(key._index)
	, m_should_parse(key._should_parse)
	{
		set_action(key.action());
	}


	KeyIndex m_index;
	bool m_should_parse;
};

/*!
 * Represents a set of configured key mappings
 *
 * This allows a keypad to have multiple configured
 * profiles and switch between them easily
 */
class G13_Profile
{
public:
	G13_Profile(G13_Device &keypad, const std::string &name_arg)
	: m_keypad(keypad)
	, m_name(name_arg)
	{
		_init_keys();
	}

	G13_Profile(const G13_Profile &other, const std::string &name_arg)
	: m_keypad(other.m_keypad)
	, m_name(name_arg)
	, m_keys(other.m_keys)
	{
	}

	// search key by G13 keyname
	G13_Key *find_key(const std::string &keyname);

	void dump(std::ostream &o) const;

	void parse_keys(unsigned char *buf);
	const std::string &name() const { return m_name; }

	const G13_Manager &manager() const;

protected:
	G13_Device &m_keypad;
	std::vector<G13_Key> m_keys;
	std::string m_name;

	void _init_keys();
};

typedef boost::shared_ptr<G13_Profile> ProfilePtr;

class G13_FontChar
{
public:
	enum FONT_FLAGS
	{
		FF_ROTATE = 0x01
	};

	static const int CHAR_BUF_SIZE = 8;

public:
	G13_FontChar()
	{
		memset(bits_regular, 0, CHAR_BUF_SIZE);
		memset(bits_inverted, 0, CHAR_BUF_SIZE);
	}

	void set_character(unsigned char *data, int width, unsigned flags);

	unsigned char bits_regular[CHAR_BUF_SIZE];
	unsigned char bits_inverted[CHAR_BUF_SIZE];
};

class G13_Font
{
public:
	G13_Font();
	G13_Font(const std::string &name, unsigned int width = 8);

	void set_character(unsigned int c, unsigned char *data);

	template <class ARRAY_T, class FLAGST>
	void install_font(ARRAY_T &data, FLAGST flags, int first = 0);

	const std::string &name() const { return m_name; }
	unsigned int width() const { return m_width; }

	const G13_FontChar &char_data(unsigned int x) { return m_chars[x]; }

protected:
	std::string m_name;
	unsigned int m_width;

	G13_FontChar m_chars[256];

	// unsigned char font_basic[256][8];
	// unsigned char font_inverted[256][8];
};

typedef boost::shared_ptr<G13_Font> FontPtr;

class G13_LCD
{
public:
	G13_LCD(G13_Device &keypad);

	G13_Device &_keypad;
	unsigned char image_buf[G13_LCD_BUF_SIZE + 8];
	unsigned cursor_row;
	unsigned cursor_col;
	int text_mode;

	void image(unsigned char *data, int size);
	void image_send() {
		image(image_buf, G13_LCD_BUF_SIZE);
	}

	void image_test(int x, int y);
	void image_clear() {
		memset(image_buf, 0, G13_LCD_BUF_SIZE);
	}

	unsigned image_byte_offset(unsigned row, unsigned col) {
		return col + (row / 8) * LCD_BYTES_PER_ROW * 8;
	}

	void image_setpixel(unsigned row, unsigned col);
	void image_clearpixel(unsigned row, unsigned col);

	void write_char(char c, int row = -1, int col = -1);
	void write_string(const char *str);
	void write_pos(int row, int col);
};

using Helper::repr;

typedef Helper::Coord<int> G13_StickCoord;
typedef Helper::Bounds<int> G13_StickBounds;
typedef Helper::Coord<double> G13_ZoneCoord;
typedef Helper::Bounds<double> G13_ZoneBounds;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class G13_StickZone : public G13_Actionable<G13_Stick>
{
public:
	G13_StickZone(G13_Stick &, const std::string &name, const G13_ZoneBounds &, G13_ActionPtr = 0);

	bool operator ==(const G13_StickZone &other) const { return m_name == other.m_name; }

	void dump(std::ostream &) const;

	void parse_key(unsigned char *byte, G13_Device *g13);
	void test(const G13_ZoneCoord &loc);
	void set_bounds(const G13_ZoneBounds &bounds) { m_bounds = bounds; }

protected:
	bool m_active;

	G13_ZoneBounds m_bounds;
};

typedef boost::shared_ptr< G13_StickZone> G13_StickZonePtr;

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class G13_Stick
{
public:
	G13_Stick(G13_Device &keypad);

	void parse_joystick(unsigned char *buf);
	void set_mode(stick_mode_t);
	G13_StickZone * zone(const std::string &, bool create = false);
	void remove_zone(const G13_StickZone &zone);

	const std::vector<G13_StickZone> &zones() const { return m_zones; }

	void dump(std::ostream &) const;

protected:
	void _recalc_calibrated();

	G13_Device &m_keypad;
	std::vector<G13_StickZone> m_zones;

	G13_StickBounds m_bounds;
	G13_StickCoord m_center_pos;
	G13_StickCoord m_north_pos;

	G13_StickCoord m_current_pos;

	stick_mode_t m_stick_mode;

};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

class Device
{
public:
	Device(Manager &manager, libusb_device_handle *handle, int id);


	Manager &manager() { return m_manager; }
	const Manager &manager() const { return m_manager; }

	LCD &lcd() { return m_lcd; }
	const LCD &lcd() const { return m_lcd; }

	Stick &stick() { return m_stick; }
	const Stick &stick() const { return m_stick; }

	FontPtr switch_to_font(const std::string &name);
	void switch_to_profile(const std::string &name);
	ProfilePtr profile(const std::string &name);

	void dump(std::ostream &, int detail = 0);
	void command(char const *str);

	void read_commands();
	void read_config_file(const std::string &filename);

	int read_keys();
	void parse_joystick(unsigned char *buf);

	ActionPtr make_action(const std::string &);

	void set_key_color(int red, int green, int blue);
	void set_mode_leds(int leds);


	void send_event(int type, int code, int val);
	void write_output_pipe(const std::string &out);

	void write_lcd(unsigned char *data, size_t size);

	bool is_set(int key);
	bool update(int key, bool v);

	// used by Manager
	void cleanup();
	void register_context(libusb_context *ctx);
	void write_lcd_file(const std::string &filename);

	Font &current_font() { return *m_current_font; }
	Profile &current_profile() { return *m_current_profile; }

	int id_within_manager() const { return m_id_within_manager; }

	typedef boost::function<void (const char *)> COMMAND_FUNCTION;
	typedef std::map<std::string, COMMAND_FUNCTION> CommandFunctionTable;

protected:
	void _init_fonts();
	void init_lcd();
	void _init_commands();

	// typedef void (COMMAND_FUNCTION)(G13_Device*, const char *, const char *);
	CommandFunctionTable m_command_table;

	struct timeval m_event_time;
	struct input_event m_event;

	int m_id_within_manager;
	libusb_device_handle *m_handle;
	libusb_context *m_ctx;

	int m_uinput_fid;

	int m_input_pipe_fid;
	std::string m_input_pipe_name;
	int m_output_pipe_fid;
	std::string m_output_pipe_name;

	std::map<std::string,FontPtr> m_fonts;
	FontPtr m_current_font;
	std::map<std::string,ProfilePtr> m_profiles;
	ProfilePtr m_current_profile;

	Manager &m_manager;
	LCD m_lcd;
	Stick m_stick;


	bool keys[G13::NUM_KEYS];
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


/*!
 * top level class, holds what would otherwise be in global variables
 */

class Manager
{
public:
	Manager();

	KeyIndex find_g13_key_value(const std::string &keyname) const;
	std::string find_g13_key_name(KeyIndex) const;

	LinuxKeyValue_t find_input_key_value(const std::string &keyname) const;
	std::string find_input_key_name(LinuxKeyValue_t) const;

	void set_logo(const std::string &fn) { logo_filename = fn; }
	int run();

	std::string string_config_value(const std::string &name) const;
	void set_string_config_value(const std::string &name, const std::string &val);

	std::string make_pipe_name(G13_Device *d, bool is_input);

	void setLogLevel(::boost::log::trivial::severity_level lvl);
	void setLogLevel(const std::string &);

protected:
	void init_keynames();
	void display_keys();
	void discover_g13s(libusb_device **devs, ssize_t count, std::vector<G13::Device*> &g13s);
	void cleanup();

protected:
	std::string m_logo_filename;
	libusb_device **m_devs;

	libusb_context *m_ctx;
	std::vector<Device *> m_g13s;

	std::map<KeyIndex, std::string> m_g13_key_to_name;
	std::map<std::string, KeyIndex> m_g13_name_to_key;
	std::map<LinuxKeyValue_t, std::string> m_input_key_to_name;
	std::map<std::string, LinuxKeyValue_t> m_input_name_to_key;

	std::map<std::string, std::string> m_string_config_values;

	static bool m_running;
	static void set_stop(int);
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// inlines

inline Manager &Action::manager()
{
	return m_keypad.manager();
}

inline const Manager &Action::manager() const
{
	return m_keypad.manager();
}

inline bool Device::is_set(int key)
{
	return keys[key];
}

inline bool Device::update(int key, bool v)
{
	bool old = keys[key];
	keys[key] = v;
	return old != v;
}

inline const Manager &Profile::manager() const
{
	return m_keypad.manager();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

} // namespace G13