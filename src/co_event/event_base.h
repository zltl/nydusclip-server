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

/**
   Class to hold information and state for Libevent dispatch loop.
 */
class EventBase : boost::noncopyable {
 public:
  EventBase() : base_{event_base_new()} {}
  explicit EventBase(event_base* base) : base_{base} {}
  ~EventBase() {
    if (base_) event_base_free(base_);
  }

  /**
     Reinitialize the event base after a fork
   */
  Result<void> Reinit() {
    int r = event_reinit(base_);
    if (r != 0) {
      return unexpected(
          Error{Code::kLibEventError,
                "event_reinit() - some events could not be re-added"});
    }

    return {};
  }

  /**
     Event dispatching loop.

     @return 0 if successful, or 1 if we exited because
     no events were pending or active.
   */
  Result<int> Dispatch() {
    int r = event_base_dispatch(base_);
    switch (r) {
      case 0:
        return 0;
      case 1:
        return 1;
      default:
        return unexpected(Error{Code::kLibEventError,
                                "event_base_dispatch() - an error occurred"});
    }
  }

  Result<std::chrono::steady_clock::time_point> GetTimeMonotonic() {
    struct timeval tv;
    int r = event_gettime_monotonic(base_, &tv);
    if (r == -1) {
      return unexpected(Error{Code::kLibEventError,
                              "event_gettime_monotonic() - an error occured"});
    }
    using namespace std::chrono;
    return steady_clock::time_point{seconds{tv.tv_sec} +
                                    microseconds{tv.tv_usec}};
  }

  int GetActiveEventNum() {
    return event_base_get_num_events(base_, EVENT_BASE_COUNT_ACTIVE);
  }
  int GetVirtualEventNum() {
    return event_base_get_num_events(base_, EVENT_BASE_COUNT_VIRTUAL);
  }
  int GetAddedEventNum() {
    return event_base_get_num_events(base_, EVENT_BASE_COUNT_ADDED);
  }

 private:
  event_base* base_{};
};

}  // namespace co_event
}  // namespace nydus
