#pragma once

#include <string>
#include <string_view>

namespace nydus {
namespace server {

std::string_view mime_type(std::string_view path);

}
}  // namespace nydus
