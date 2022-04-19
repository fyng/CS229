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
  conn = Connection(receiver_fd); 
  
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Message username_message = Message(TAG_RLOGIN, username);
  rio_writen(receiver_fd, username_message, 255);
  Message rec_user_mess = rio_readnb();
  if (!conn.receive(rec_user_mess)) {
    cout << "Error reason" << endl;
    return 1;
  }
  Message room_message = Message(TAG_JOIN, room_name);
  rio_writen(receiver_fd, room_message, 255);
  Message rec_room_mess = rio_readnb();
  if (!conn.receive(rec_room_mess)) {
    cout << "Error reason" << endl;
    return 1;
  }
  
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)
  while (1) {
    
    cout << "[insert text]" << endl;
  }

  return 0;
}
