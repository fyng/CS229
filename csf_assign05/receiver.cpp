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
  int server_port = std::stoi(argv[2]);
  std::string username = argv[3];
  std::string room_name = argv[4];

  // TODO: connect to server
  Connection conn;
  conn.connect(server_hostname, server_port); 
  if (!conn.is_open()) {
    std::cerr << "Failed to connect to server" << std::endl;
    return 1;
  }

  
  // TODO: send rlogin and join messages (expect a response from
  //       the server for each one)
  Send(&conn, TAG_RLOGIN, username);
  if (Receive(&conn)){
    return 1;
  }
  Send(&conn, TAG_JOIN, room_name);
  if (Receive(&conn)){
    return 1;
  }

  // // Receive returned properly, check answer
  // std::vector<std::string> received_message;
  // received_message = username_check.split_payload();
  // // Error, something went wrong server side or OK, else unknown behavior
  // if (received_message[0].compare(TAG_ERR) == 0) {
  //   std::cerr << received_message[1] << std::endl;
  //   std::cout << received_message[1] << std::endl;
  //   return 1;
  // } else if (received_message[0].compare(TAG_OK) == 0) {
  //   std::cout << received_message[1] << std::endl;
  // } else {
  //   // Should have received OK, if not, unknown behavior
  //   std::cerr << "Unknown/Invalid Tag received" << std::endl;
  //   return 2;
  // }

  // // Receive returned properly, received reply message
  // received_message = rec_room_mess.split_payload();
  // // Error, something went wrong server side or OK, else unknown behavior
  // if (received_message[0].compare(TAG_ERR) == 0) {
  //   std::cerr << received_message[1] << std::endl;
  //   std::cout << received_message[1] << std::endl;
  //   return 1;
  // } else if (received_message[0].compare(TAG_OK) == 0) {
  //   std::cout << received_message[1] << std::endl;
  // } else {
  //   // Should have received OK, if not, unknown behavior
  //   std::cerr << "Uknown/Invalid Tag received" << std::endl;
  //   return 2;
  // }
  
  // TODO: loop waiting for messages from server
  //       (which should be tagged with TAG_DELIVERY)

  // Set while loop to receive and print sent info
  Message server_message;
  std::vector<std::string> received_message;
  while (1) {
    if (!conn.receive(server_message)) {
      std::cerr << "Failed to receive any message" << std::endl;
      return 1;
    }
    
    // If receive did not fail, then print sent message
    received_message = server_message.split_payload();
    if (received_message[0].compare(TAG_DELIVERY) == 0) {
      std::cout << received_message[2]	<< ": "	<< received_message[3];
    } else if (received_message[0].compare(TAG_ERR) == 0) {
      std::cout << received_message[1];
    }
  }

  return 0;
}
