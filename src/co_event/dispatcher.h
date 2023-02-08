#pragma once
#include <thread>

#include "boost/noncopyable.hpp"
#include "event2/event.h"

namespace nydus {
namespace co_event {

/**
 * Abstract event dispatching loop.
 */
class Dispatcher : boost::noncopyable {
 public:
  Dispatcher() {

  }

 private:
  std::thread::id id_;
};

}  // namespace co_event
}  // namespace nydus
