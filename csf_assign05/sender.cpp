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
    std::cerr << incoming_msg.data;
    return 1;
  }
  return 0;
}

int Send(Connection * link, std::string tag, std::string data){
  data += "\n";
  Message msg = Message(tag, data);
  if ((msg.tag.length() + msg.data.length() + 1) > msg.MAX_LEN){
    std::cerr << "Message exceeds max length" << std::endl;
    return 1;
  }
  link->send(msg);
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
  if (Send(&link, TAG_SLOGIN, username)){
    return 1;
  }
  if (Receive(&link)){
    // std::cerr << "Sender failed to login to server" << std::endl;
    return 1;
  }

  // TODO: loop reading commands from user, sending messages to
  //       server as appropriate
  std::string input;
  while (std::getline(std::cin, input)){
    std::stringstream ss(input);
    std::string cmd, payload;
    ss >> cmd;
    if (input.find("/") == std::string::npos){
      Send(&link, TAG_SENDALL, input);
      Receive(&link);
    } else if (!(cmd.compare("/quit"))){
      Send(&link, TAG_QUIT, "bye");
      if (!Receive(&link)){
        return 0;
      }
    } else if (!(cmd.compare("/join"))) {
      ss >> payload;
      Send(&link, TAG_JOIN, payload);
      // if (Receive(&link)) return 1;
    } else if (!(cmd.compare("/leave"))){
      Send(&link, TAG_LEAVE, "yeet");
      Receive(&link);
    } else {
      std::cerr << "Invalid command" << std::endl;
    } 
  }

  return 0;
}
