#include "status/error.h"
#include <format>
#include <string>

namespace nydus {
namespace status {

std::string Error::to_string() const {
  return std::to_string(static_cast<int>(code)) + ": " + desc;
}

bool operator==(const Error& lhs, const Error& rhs) {
  return lhs.code == rhs.code;
}


}  // namespace status
}  // namespace nydus


// The wrapper Box<T> can be formatted using the format specification of the wrapped value
template<class CharT>
struct std::formatter<nydus::status::Error, CharT> : std::formatter<std::string, CharT> {
    // parse() is inherited from the base class

    // Define format() by calling the base class implementation with the wrapped value
    template<class FormatContext>
    auto format(nydus::status::Error e, FormatContext& fc) const {
      return std::formatter<std::string, CharT>::format(e.to_string(), fc);
    }
};


