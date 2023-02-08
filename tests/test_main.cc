#include <gtest/gtest.h>

#include "co_event/co_event.h"
#include "co_event/event.h"
#include "co_event/event_base.h"
#include "co_event/initial.h"

struct InitialStaticClass {
  InitialStaticClass() {
    nydus::log::init();

    nydus::co_event::Initial::Initialize();
    nydus::co_event::Initial::LibeventEnableDebugLogging();
  }
  ~InitialStaticClass() { nydus::co_event::Initial::Uninitilize(); }
};

static InitialStaticClass my_init_object{};

