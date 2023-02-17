#pragma once
#include <format>
#include <string>

namespace nydus {
namespace option {

struct Server {
  std::string addr;
  int port;
  int io_threads;
  std::string string() {
    return std::format("[server]\n") + std::format("addr = {}\n", addr) +
           std::format("port = {}\n", port) +
           std::format("io_threads={}\n", io_threads);
  }
};

struct Data {
  std::string dir;

  std::string string() {
    return std::format("[data]\n") + std::format("dir = {}\n", dir);
  }
};

struct Db {
  std::string host;
  int port;
  std::string username;
  std::string password;
  std::string dbname;
  int max_connections;
  int max_idle_connections;

  std::string string() {
    return std::format(R"([db]
host = {}
port = {}
username = {}
password = {}
dbname = {}
max_connections = {}
max_idle_connections = {}
)",
                       host, port, username, password, dbname, max_connections,
                       max_idle_connections);
  }
};

struct Conf {
  Server server;

  Data data;

  Db db;

  /**
     Get the global instalce of Conf.
   */
  static Conf &conf() { return conf_; }

  std::string string() { return server.string() + data.string() + db.string(); }

 private:
  static Conf conf_;
};

void parse(Conf &conf, int argc, const char **argv);

}  // namespace option
}  // namespace nydus
