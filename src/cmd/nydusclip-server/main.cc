#include "log/log.h"
#include "option/option.h"
#include "server/server.h"

using namespace nydus;
using Conf = nydus::option::Conf;

int main(int argc, const char** argv) {
  option::parse(Conf::conf(), argc, argv);

  nydus::log::init();

  TRACE("hello");
  DEBUG("hello");
  INFO("hello {}", 123);
  WARN("hello");
  ERROR("ERROR");
  CRITICAL("HELLO");

  // try server/http
  nydus::server::Tx();

  return 0;
}
