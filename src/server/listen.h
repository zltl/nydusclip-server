#pragma once

#include "server/net.h"

namespace nydus {
namespace server {

// Accepts incoming connections and launches the sessions
net::awaitable<void> do_listen(tcp::endpoint endpoint);

}  // namespace server
}  // namespace nydus
