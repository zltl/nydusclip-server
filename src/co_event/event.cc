#include "co_event/event.h"

#include <event2/event.h>

#include <functional>

#include "co_event/event_base.h"
#include "co_event/event_type.h"

namespace nydus {
namespace co_event {

extern "C" {

void EventCallback(evutil_socket_t fd, short events, void* arg) {
  Event* ev = reinterpret_cast<Event*>(arg);
  ev->Callback(fd, events);
}
}

Event::~Event() {
  event_free(event_);
  event_ = nullptr;

  if (!have_called_) {
    // call cb(EventType=0) to notify that we want to delete this event
    callback_(0);
  }
}

struct event* Event::GetCEvent() { return event_; }

void Event::Callback(SocketFd fd, EventType events) {
  (void)fd;
  have_called_ = true;
  callback_(events);
}

Result<Void> Event::Add() {
  int r = event_add(event_, nullptr);
  if (r == 0) {
    return {};
  }
  return std::unexpected(
      Error{Code::kLibeventError, "event_add() - an error occurred"});
}

Result<Void> Event::Add(std::chrono::nanoseconds dura) {
  struct timeval tv;
  std::chrono::seconds const sec =
      std::chrono::duration_cast<std::chrono::seconds>(dura);

  tv.tv_sec = sec.count();
  tv.tv_usec =
      std::chrono::duration_cast<std::chrono::microseconds>(dura - sec).count();

  int r = event_add(event_, &tv);
  if (r == 0) {
    return {};
  }
  return std::unexpected(
      Error{Code::kLibeventError, "event_add() - an error occurred"});
}

Result<Void> Event::Del() {
  int r = event_del_noblock(event_);
  if (r == 0) {
    return {};
  }
  return unexpected(
      Error{Code::kLibeventError, "event_del() - an error occurred"});
}

void Event::Activate(EventType events) { event_active(event_, events, 0); }

SocketFd Event::GetFd() { return event_get_fd(event_); }

}  // namespace co_event
}  // namespace nydus
