#include <boost/program_options.hpp>
#include <iostream>
#include <string>

#include "log/log.h"

namespace nydus {
namespace option {

namespace po = boost::program_options;

void parse(int argc, const char **argv) {
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
  int opt;
  po::options_description config("Configuration");
  config.add_options()  //
      ("optimization", po::value<int>(&opt)->default_value(10),
       "optimization level")  //
      ("include-path,I", po::value<std::vector<std::string> >()->composing(),
       "include path");

  // Hidden options, will be allowed both on command line and
  // in config file, but will not be shown to the user.
  po::options_description hidden("Hidden options");
  hidden.add_options()  //
      ("input-file", po::value<std::vector<std::string> >(), "input file");

  po::options_description cmdline_options;
  cmdline_options.add(generic).add(config).add(hidden);

  po::options_description config_file_options;
  config_file_options.add(config).add(hidden);

  po::options_description visible("Allowed options");
  visible.add(generic).add(config);

  po::variables_map vm;
  po::store(po::parse_command_line(argc, argv, cmdline_options), vm);
  po::notify(vm);

  if (vm.count("help")) {
    std::cout << visible << "\n";
    exit(1);
  }

  if (vm.count("conf")) {
    std::string confile{vm["conf"].as<std::string>()};
    std::cout << "try read options from file " << confile << std::endl;
    po::store(po::parse_config_file(confile.c_str(), config_file_options), vm);
    po::notify(vm);
  }

  std::cout << "adfafds" << vm["optimization"].as<int>() << std::endl;
}

}  // namespace option
}  // namespace nydus
