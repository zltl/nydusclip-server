#include "option/option.h"

#include <boost/program_options.hpp>
#include <fstream>
#include <iostream>
#include <string>

#include "boost/program_options/parsers.hpp"
#include "boost/program_options/positional_options.hpp"
#include "boost/program_options/variables_map.hpp"
#include "log/log.h"

namespace nydus {
namespace option {

namespace po = boost::program_options;

Conf Conf::conf_;

void parse(Conf& conf, int argc, const char** argv) {
  // Declare a group of options that will be
  // allowed only on command line
  po::options_description generic("Generic options");
  generic.add_options()                      //
      ("version,v", "print version string")  //
      ("help", "produce help message")       //
      ("conf,c", po::value<std::string>()->composing(), "configuratiojn file");

  // Declare a group of options that will be
  // allowed both on command line and in
  // config file
  po::options_description config("Configuration");
  config.add_options()  //
      ("server.addr",
       po::value<std::string>(&conf.server.addr)->default_value("0.0.0.0"))  //
      ("server.port",
       po::value<int>(&conf.server.port)->default_value(8080))  //
      ("server.io_threads",
       po::value<int>(&conf.server.io_threads)->default_value(4))  //
      ("data.dir",
       po::value<std::string>(&conf.data.dir)->default_value("./data"))  //
      ("db.host",
       po::value<std::string>(&conf.db.host)->default_value("127.0.0.1"))  //
      ("db.port", po::value<int>(&conf.db.port)->default_value(5432))      //
      ("db.username",
       po::value<std::string>(&conf.db.username)->default_value(""))  //
      ("db.password",
       po::value<std::string>(&conf.db.password)->default_value(""))  //
      ("db.dbname",
       po::value<std::string>(&conf.db.dbname)->default_value("nydus"))  //
      ("db.max_connections",
       po::value<int>(&conf.db.max_connections)->default_value(0))  //
      ("db.max_idle_connections",
       po::value<int>(&conf.db.max_idle_connections)->default_value(0))  //
      ;

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(config);

  po::options_description config_file_options;
  config_file_options.add(config);

  po::options_description visible("Allowed options");
  visible.add(generic).add(config);

  po::variables_map vm;
  po::store(po::command_line_parser(argc, argv).options(cmdline_options).run(),
            vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << visible << "\n";
    exit(1);
  }

  if (vm.count("version")) {
    std::cout << "nydusclip-server version 0.0.1\n";
    exit(0);
  }

  if (vm.count("conf")) {
    std::string confile{vm["conf"].as<std::string>()};
    std::cout << "reading options from file " << confile << std::endl;

    std::ifstream ifs(confile.c_str());
    if (!ifs) {
      std::cout << "Cannot open config file: " << confile << "\n";
      exit(1);
    } else {
      po::store(po::parse_config_file(ifs, config_file_options), vm);
      po::notify(vm);
    }
  }

  std::cout << "Config:\n" << conf.string();
}

}  // namespace option
}  // namespace nydus
