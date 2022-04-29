#include <iostream>
#include <string>
#include "connection.h"
#include "message.h"
#include "client_util.h"

// string trim functions shamelessly stolen from
// https://www.techiedelight.com/trim-string-cpp-remove-leading-trailing-spaces/

const std::string WHITESPACE = " \n\r\t\f\v";

std::string ltrim(const std::string &s) {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}
 
std::string rtrim(const std::string &s) {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}
 
std::string trim(const std::string &s) {
  return rtrim(ltrim(s));
}

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
  Message msg = Message(tag, data);
  if ((msg.tag.length() + msg.data.length() + 1) > msg.MAX_LEN){
    std::cerr << "Message exceeds max length" << std::endl;
    return 1;
  }
  link->send(msg);
  return 0;
} 