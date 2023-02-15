#include <boost/beast/http/dynamic_body.hpp>
#include <boost/beast/http/message_generator.hpp>
#include <boost/beast/http/string_body.hpp>
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <regex>
#include <string_view>
#include <utility>

#include "log/log.h"
#include "server/beast.h"
#include "server/mime.h"
#include "server/net.h"
#include "status/error.h"

namespace nydus {
namespace server {

class Mux {
 public:
  using HandleFuncType =
      std::function<http::message_generator(RequestType&)>;
  struct Handle {
    std::string re;
    HandleFuncType fn;
  };

  Mux(std::initializer_list<Handle>);

  http::message_generator Serve(RequestType &&req);

 private:
  struct HandlePair {
    std::regex re;
    HandleFuncType fn;
  };
  std::vector<HandlePair> handles_;
};

http::message_generator handle_request(beast::string_view doc_root,
                                       http::request<http::string_body>&& req);

}  // namespace server

}  // namespace nydus
