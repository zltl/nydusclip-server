#include "log/log.h"
#include "option/option.h"

int main(int argc, const char** argv) {

  nydus::option::parse(argc, argv);

  nydus::log::init();

  TRACE("hello");
  DEBUG("hello");
  INFO("hello {}", 123);
  WARN("hello");
  ERROR("ERROR");
  CRITICAL("HELLO");


  return 0;
}
