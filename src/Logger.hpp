/*
** EPITECH PROJECT, 2019
** G13
** File description:
** Logger.hpp
*/

#pragma once

////////////////////////////////////////////////////////////////////////////////

class Logger;

////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "extern/tinyformat.hpp"

////////////////////////////////////////////////////////////////////////////////

class Logger
{
public:
	enum Level
	{
		Trace,
		Debug,
		Info,
		Warn,
		Error,
		Fatal
	};

public:
	static Level level() { return s_level; }
	static void setLevel(Level level) { s_level = level; }
	static void useColors(bool useColors) { s_useColors = useColors; }

	static std::ostream &log(Level level);
	static std::ostream &trace() { return log(Trace); }
	static std::ostream &debug() { return log(Debug); }
	static std::ostream &info() { return log(Info); }
	static std::ostream &warn() { return log(Warn); }
	static std::ostream &error() { return log(Error); }
	static std::ostream &fatal() { return log(Fatal); }

	template <typename... Args>
	static void log(Level level, const char *fmt, const Args&... args) {
		log(level) << tfm::format(fmt, args...) << std::endl;
	}

	template <typename... Args>
	static void trace(const char *fmt, const Args&... args) { return log(Trace, fmt, args...); }
	template <typename... Args>
	static void debug(const char *fmt, const Args&... args) { return log(Debug, fmt, args...); }
	template <typename... Args>
	static void info(const char *fmt, const Args&... args) { return log(Info, fmt, args...); }
	template <typename... Args>
	static void warn(const char *fmt, const Args&... args) { return log(Warn, fmt, args...); }
	template <typename... Args>
	static void error(const char *fmt, const Args&... args) { return log(Error, fmt, args...); }
	template <typename... Args>
	static void fatal(const char *fmt, const Args&... args) { return log(Fatal, fmt, args...); }

private:
	static const char *levelString(Level level);

private:
	static Level s_level;
	static bool s_useColors;
};