#include <gtest/gtest.h>

#include <expected>

#include "co_event/event.h"
#include "co_event/event_base.h"
#include "co_event/event_type.h"

using namespace nydus::co_event;
TEST(LibEvent, Timer1ns) {
  using namespace std::chrono_literals;

  auto base = std::unique_ptr<EventBase>(new EventBase);
  auto a = "hello";
  auto b = 123;
  bool triggered = false;

  std::chrono::duration c = 5min + 1ms + 60ns;
  auto timer_event = std::unique_ptr<Event>(Event::CreateTimer(
      *base,
      [&triggered](EventType events, const char* a, int b, decltype(c) mc) {
        DEBUG("timer triggered: {:x} {} {} {}", events, a, b, mc.count());
        triggered = true;
      },
      a, b, c));

  using RT = Result<EventBase::R>;

  auto x = timer_event->Add(1ns);
  if (!x.has_value()) {
    FAIL() << "Add event failed" << x.error().to_string();
  }

  auto y = base->Dispatch().and_then([&](auto r) -> RT {
    DEBUG("base loop return success: {}", static_cast<int>(r));
    return r;
  });

  ASSERT_TRUE(triggered);
  if (!y.has_value()) {
    FAIL() << "EventBase loop exit failed: " << x.error().to_string();
  }
}
