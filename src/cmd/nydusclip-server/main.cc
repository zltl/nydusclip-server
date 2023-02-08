#include "co_event/co_event.h"
#include "co_event/initial.h"
#include "log/log.h"
#include "option/option.h"
#include "co_event/event_base.h"
#include "co_event/event.h"

int main(int argc, const char** argv) {

  nydus::option::parse(argc, argv);

  nydus::log::init();

  TRACE("hello");
  DEBUG("hello");
  INFO("hello {}", 123);
  WARN("hello");
  ERROR("ERROR");
  CRITICAL("HELLO");

  nydus::co_event::Initial::Initialize();
  nydus::co_event::Initial::LibeventEnableDebugLogging();

  nydus::co_event::Initial::Uninitilize();
  return 0;
}
