#include "co_event/event_base.h"

#include <exception>

#include "co_event/event.h"

namespace nydus {
namespace co_event {
EventBase::EventBase() : base_{event_base_new()} {}
EventBase::EventBase(event_base* base) : base_{base} {}
EventBase::EventBase(EventBase&& other) : base_(other.base_) {
  other.base_ = nullptr;
}
EventBase& EventBase::operator=(EventBase&& other) {
  base_ = other.base_;
  other.base_ = nullptr;
  return *this;
}
EventBase::~EventBase() {
  if (base_) event_base_free(base_);
}

event_base* EventBase::GetCBase() { return base_; }

Result<Void> EventBase::Reinit() {
  int r = event_reinit(base_);
  if (r != 0) {
    return unexpected(
        Error{Code::kLibeventError,
              "event_reinit() - some events could not be re-added"});
  }

  return {};
}

Result<EventBase::R> EventBase::Dispatch() {
  int r = event_base_dispatch(base_);
  switch (r) {
    case 0:
      return R::kOK;
    case 1:
      return R::kNoEvent;
    default:
      return unexpected(Error{Code::kLibeventError,
                              "event_base_dispatch() - an error occurred"});
  }
}

Result<std::chrono::steady_clock::time_point> EventBase::GetTimeMonotonic() {
  struct timeval tv;
  int r = event_gettime_monotonic(base_, &tv);
  if (r == -1) {
    return unexpected(Error{Code::kLibeventError,
                            "event_gettime_monotonic() - an error occured"});
  }
  using namespace std::chrono;
  return steady_clock::time_point{seconds{tv.tv_sec} +
                                  microseconds{tv.tv_usec}};
}

int EventBase::GetActiveEventNum() {
  return event_base_get_num_events(base_, EVENT_BASE_COUNT_ACTIVE);
}
int EventBase::GetVirtualEventNum() {
  return event_base_get_num_events(base_, EVENT_BASE_COUNT_VIRTUAL);
}
int EventBase::GetAddedEventNum() {
  return event_base_get_num_events(base_, EVENT_BASE_COUNT_ADDED);
}

Result<EventBase::R> EventBase::Loop(int flags) {
  int r = event_base_loop(base_, flags);
  switch (r) {
    case 0:
      return R::kOK;
    case 1:
      return R::kNoEvent;
    default:
      return std::unexpected(
          Error{Code::kLibeventError, "event_base_loop() - an error occurred"});
  }
}

Result<Void> EventBase::LoopExit(std::chrono::nanoseconds dura) {
  struct timeval tv;
  std::chrono::seconds const sec =
      std::chrono::duration_cast<std::chrono::seconds>(dura);

  tv.tv_sec = sec.count();
  tv.tv_usec =
      std::chrono::duration_cast<std::chrono::microseconds>(dura - sec).count();
  int r = event_base_loopexit(base_, &tv);
  if (r == 0) {
    return {};
  }
  return std::unexpected(
      Error{Code::kLibeventError, "event_base_loopexit() - an error occurred"});
}

Result<Void> EventBase::LoopBreak() {
  int r = event_base_loopbreak(base_);
  if (r == 0) {
    return {};
  }
  return std::unexpected(Error{Code::kLibeventError,
                               "event_base_loopbreak() - an error occurred"});
}

Result<Void> EventBase::SetEvent(Event& e) {
  int r = event_base_set(base_, e.GetCEvent());
  if (r == -1) {
    return std::unexpected(
        Error{Code::kLibeventError, "event_base_set() - failure"});
  }
  return {};
}

event_base* GetCBase(EventBase& b) { return b.GetCBase(); }
}  // namespace co_event
}  // namespace nydus
