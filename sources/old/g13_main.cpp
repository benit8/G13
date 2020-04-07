/*
** EPITECH PROJECT, 2019
** G13
** File description:
** g13_main.cpp
*/

#include "g13.hpp"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char *argv[])
{
	G13::Manager manager;
	manager.setLogLevel("info");

	// Declare the supported options.
	po::options_description desc("Allowed options");
	desc.add_options()("help", "produce help message");
	std::vector<std::string> sopt_names;
	auto add_string_option = [&sopt_names, &desc](const char *name, const char *description) {
		desc.add_options()(name, po::value<std::string>(), description);
		sopt_names.push_back(name);
	};

	add_string_option("logo", "set logo from file");
	add_string_option("config", "load config commands from file");
	add_string_option("pipe_in", "specify name for input pipe");
	add_string_option("pipe_out", "specify name for output pipe");
	add_string_option("log_level", "logging level");
	// add_string_option("logfile", "write log to logfile");

	po::positional_options_description p;
	p.add("logo", -1);
	po::variables_map vm;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
	po::notify(vm);

	if (vm.count("help")) {
		cout << argv[0] << " : user space G13 driver" << endl;
	    cout << desc << "\n";
	    return 1;
	}

	BOOST_FOREACH(const std::string &tag, sopt_names) {
		if (vm.count(tag)) {
			manager.set_string_config_value(tag,vm[tag].as<std::string>());
		}
	}

	if (vm.count("logo")) {
		manager.set_logo(vm["logo"].as<std::string>());
	}

	if (vm.count("log_level")) {
		manager.setLogLevel(manager.string_config_value("log_level"));
	}

	manager.run();
	return 0;
}