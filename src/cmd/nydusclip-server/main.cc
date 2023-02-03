#include "log/log.h"

int main(int argc, const char** argv) {
  (void)argc;
  (void)argv;

  nydus::log::init();
  TRACE("hello");
  DEBUG("hello");
  INFO("hello {}", 123);
  WARN("hello");
  ERROR("ERROR");
  CRITICAL("HELLO");
  return 0;
}
