#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int main(int argc, char **argv) {
  if (argc != 5) {
    std::cerr << "Usage: ./receiver [server_address] [port] [username] [room]\n";
    return 1;
  }

  std::string server_hostname = argv[1];
  std::string server_port = argv[2];
  std::string username = argv[3];
  std::string room_name = argv[4];

  Connection conn;

  // TODO: connect to server
  int receiver_fd = open_clientfd(server_hostname, server_port);

  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
 rlogin:username;
 join:room;

  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  while () {
    cout << "[insert text]" << endl;
  }

  return 0;
}
