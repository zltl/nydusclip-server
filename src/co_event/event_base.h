#pragma once

/**
   @mainpage

   @section intro Introduction

   @section usage Standard usage

   @section setup Library setup

   ...

   TODO:

 */

#include <event2/event.h>

#include <boost/core/noncopyable.hpp>
#include <chrono>
#include <exception>
#include <memory>
#include <tuple>
#include <utility>

#include "co_event/event.h"
#include "co_event/event_type.h"
#include "co_event/socket_fd.h"
#include "status/error.h"

namespace nydus {
namespace co_event {

using nydus::status::Code;
using nydus::status::Error;
using nydus::status::Result;
using std::unexpected;

class Event;

/**
   Class to hold information and state for Libevent dispatch loop.
 */
class EventBase : public boost::noncopyable {
 public:
  EventBase();
  explicit EventBase(event_base* base);
  EventBase(EventBase&& other);
  EventBase& operator=(EventBase&& other);
  ~EventBase();

  event_base* GetCBase();

  /**
     Reinitialize the event base after a fork
   */
  Result<void> Reinit();

  enum class R {
    kOK,
    kNoEvent,
  };

  /**
     Event dispatching loop.

     @return EventBase::Result::kOK if successful, or
     EventBase::Result::kNoEvent if we exited because no events were pending or
     active.
   */
  Result<R> Dispatch();

  Result<std::chrono::steady_clock::time_point> GetTimeMonotonic();

  int GetActiveEventNum();
  int GetVirtualEventNum();
  int GetAddedEventNum();

  // Associate a different event base with an event
  Result<void> SetEvent(Event& e);  // TODO

  enum LoopType {
    kOnce = EVLOOP_ONCE,
    kNonBlock = EVLOOP_NONBLOCK,
    kNoExitOnEmpty = EVLOOP_NO_EXIT_ON_EMPTY,
  };

  Result<R> Loop(int flags);

  Result<void> LoopExit(std::chrono::nanoseconds dura);

  Result<void> LoopBreak();

 private:
  event_base* base_{};
};



}  // namespace co_event
}  // namespace nydus
