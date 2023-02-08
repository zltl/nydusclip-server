#pragma once

#include <event2/event.h>
#include <event2/util.h>

#include <boost/core/noncopyable.hpp>
#include <boost/noncopyable.hpp>
#include <chrono>
#include <coroutine>
#include <exception>
#include <functional>
#include <memory>
#include <tuple>
#include <utility>

#include "co_event/event_base.h"
#include "co_event/event_type.h"
#include "co_event/socket_fd.h"
#include "log/log.h"
#include "status/error.h"

namespace nydus {
namespace co_event {
class EventBase;

using nydus::status::Code;
using nydus::status::Error;
using nydus::status::Result;
using std::unexpected;
using namespace std::placeholders;

extern event_base* GetCBase(EventBase& b);

extern "C" void EventCallback(evutil_socket_t fd, short events, void* arg);

class Event : public boost::noncopyable {
 public:
  template <class F, class... Args>
  Event(EventBase& base, SocketFd fd, EventType events, F&& cb,
               Args&&... cb_args)
      : event_{event_new(GetCBase(base), fd, events, EventCallback, this)},
        callback_{
            std::bind(std::forward<F>(cb), _1, std::forward<Args>(cb_args)...)},
        base_(&base){}

  template <class F, class... Args>
  static Event* CreateTimer(EventBase& base, F&& cb, Args&&... cb_args) {
    return new Event(base, -1, 0, std::forward<F>(cb),
                     std::forward<Args>(cb_args)...);
  }

  template <class F, class... Args>
  static Event* CreateSignal(EventBase& base, SocketFd x, F&& cb,
                             Args&&... cb_args) {
    return new Event(base, x, EV_SIGNAL | EV_PERSIST, std::forward<F>(cb),
                     std::forward<Args>(cb_args)...);
  }

  ~Event();

  struct event* GetCEvent();

  void Callback(SocketFd fd, EventType events);

  Result<void> Add();

  Result<void> Add(std::chrono::nanoseconds dura);

  Result<void> Del();

  void Activate(EventType events);

  SocketFd GetFd();

 private:
  bool have_called_{};
  struct event* event_{};
  std::function<void(EventType)> callback_;
  EventBase* base_{};  // not own
};

}  // namespace co_event
}  // namespace nydus
