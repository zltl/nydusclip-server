#include "initial.h"

#include <event2/event.h>
#include <event2/thread.h>

#include <csignal>

#include "log/log.h"

namespace nydus {
namespace co_event {

static void EventLogCallback(int severity, const char* msg) {
  switch (severity) {
    case EVENT_LOG_DEBUG:
      DEBUG("libevent: {}", msg);
      return;
    case EVENT_LOG_MSG:
      INFO("libevent: {}", msg);
      return;
    case EVENT_LOG_WARN:
      WARN("libevent: {}", msg);
      return;
    case EVENT_LOG_ERR:
      ERROR("libevent: {}", msg);
      return;
    default:
      break;
  }
}

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

  event_set_log_callback(EventLogCallback);
}

void Initial::LibeventEnableDebugLogging() {
  event_enable_debug_logging(EVENT_DBG_ALL);
}

void Initial::Uninitilize() { libevent_global_shutdown(); }

}  // namespace co_event
}  // namespace nydus
