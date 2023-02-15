#pragma once
#include <string>

namespace nydus {
namespace option {

struct Server {
  std::string addr;
  int port;
  int io_threads;
};

struct Conf {
  Server server;


  /**
     Get the global instalce of Conf.
   */
  static Conf &conf() { return conf_; }

 private:
  static Conf conf_;
};

void parse(Conf &conf, int argc, const char **argv);

}  // namespace option
}  // namespace nydus
