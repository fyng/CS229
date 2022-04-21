#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include "client_util.h"

int Receive(Connection * link){
  Message incoming_msg;
  link->receive(incoming_msg);

  if (incoming_msg.tag == TAG_ERR){
    std::cerr << incoming_msg.data << std::endl;
    return 1;
  }
  // if (incoming_msg->tag == TAG_OK){
  //   return 0;
  // }
  return 0;
}

int main(int argc, char **argv) {
  if (argc != 4) {
    std::cerr << "Usage: ./sender [server_address] [port] [username]\n";
    return 1;
  }

  std::string server_hostname;
  int server_port;
  std::string username;

  server_hostname = argv[1];
  server_port = std::stoi(argv[2]);
  username = argv[3];

  // TODO: connect to server
  Connection link;
  link.connect(server_hostname, server_port);

  // TODO: send slogin message
  Message outgoing_msg = Message(TAG_SLOGIN, username);
  bool sent = false;
  while (!sent) {
    sent = link.send(outgoing_msg);
  }
  if (Receive(&link)){
    return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string input;
  while (std::getline(std::cin, input)){
    std::stringstream ss(input);
    std::string cmd;
    ss >> cmd;
    if (!(cmd.compare("/quit"))){
      outgoing_msg = Message(TAG_QUIT, "");
      link.send(outgoing_msg);
      if (!Receive(&link)){
        return 0;
      }
    } else if (!(cmd.compare("/join"))) {
      outgoing_msg = Message(TAG_JOIN, ss.str());
      Receive(&link);
    } else if (!(cmd.compare("/leave"))){
      outgoing_msg = Message(TAG_LEAVE, "");
      link.send(outgoing_msg);
      Receive(&link);
    } else if (strtok(&*cmd.begin(), "/") != NULL){
      std::cerr << "Invalid command" << std::endl;
    } else {
      outgoing_msg = Message(TAG_SENDALL, ss.str());
      link.send(outgoing_msg);
      Receive(&link);
    }

  }

  return 0;
}
