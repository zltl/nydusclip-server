#include "initial.h"

#include <event2/event.h>
#include <event2/thread.h>

#include <csignal>

namespace nydus {
namespace co_event {

bool Initial::is_initialized_ = false;

void Initial::Initialize() {
#ifdef WIN32
  evthread_use_windows_threads();
#else
  evthread_use_pthreads();

  // Ignore SIGPIPE and allow errors to propagate through error codes.
  signal(SIGPIPE, SIG_IGN);
#endif
  is_initialized_ = true;
}

}  // namespace co_event
}  // namespace nydus
