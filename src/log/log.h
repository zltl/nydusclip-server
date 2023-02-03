#pragma once

#include <spdlog/spdlog.h>

#define TRACE SPDLOG_TRACE
#define DEBUG SPDLOG_DEBUG
#define INFO SPDLOG_INFO
#define WARN SPDLOG_WARN
#define ERROR SPDLOG_ERROR
#define CRITICAL SPDLOG_CRITICAL

namespace nydus {
namespace log {
void init();
}
}  // namespace nydus
