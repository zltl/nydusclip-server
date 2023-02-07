#pragma once

namespace nydus {
namespace co_event {

class Initial {
 public:
  static bool IsInitialized() { return is_initialized_; }

  static void Initialize();

 private:
  static bool is_initialized_;
};

}  // namespace co_event
}  // namespace nydus
