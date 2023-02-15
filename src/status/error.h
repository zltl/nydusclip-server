#pragma once

#include <expected>
#include <optional>
#include <string>
#include <string_view>

namespace nydus {
namespace status {

/**
   All error codes.
 */
enum class Code {
  kOK = 0,

  kAsioError = 7001,
};

/**
   Wrapper of error code and its description.
 */
struct Error {
  // Error(int code): code{code} {}
  // Error(int code, std::string_view desc): code{code}, desc{desc} {}

  std::string to_string() const;

  Code code{};
  std::string desc{};
};

bool operator==(const Error& lhs, const Error& rhs);

/**
   Result<Void> need this.
 */
struct Void {};

template <class T, class E = Error>
using Result = ::std::expected<T, E>;

}  // namespace status

using namespace nydus::status;
using nydus::status::Code;
using nydus::status::Error;
using nydus::status::Void;

}  // namespace nydus
