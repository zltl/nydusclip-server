#include "log/log.h"

#include <spdlog/spdlog.h>


namespace nydus {
  namespace log {
    void init() {
      spdlog::set_pattern("[%Y-%m-%dT%H:%M:%S.%F%z] [%^%l%$] %v [%P/%t] [%g:%# %!]");
      spdlog::set_level(spdlog::level::trace);
    }
  }
}
