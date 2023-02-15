#pragma once

#include <boost/beast.hpp>

namespace beast = boost::beast;                 // from <boost/beast.hpp>
namespace http = beast::http;                   // from <boost/beast/http.hpp>
// namespace websocket = beast::websocket;  // from <boost/beast/websocket.hpp>

using BodyType = http::string_body;
using RequestType = http::request<BodyType>;
using ResponseType = http::response<BodyType>;
