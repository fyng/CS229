#include <sstream>
#include <cctype>
#include <cassert>
#include "csapp.h"
#include "message.h"
#include "connection.h"
#include <cstring>

Connection::Connection()
  : m_fd(-1)
  , m_last_result(SUCCESS) {
}

Connection::Connection(int fd)
  : m_fd(fd)
  , m_last_result(SUCCESS) {
  // TODO: call rio_readinitb to initialize the rio_t object
  Rio_readinitb(&m_fdbuf, m_fd);
}

void Connection::connect(const std::string &hostname, int port) {
  // TODO: call open_clientfd to connect to the server
  // TODO: call rio_readinitb to initialize the rio_t object
  char hostnm[hostname.length() + 1];
  strcpy(hostnm, hostname.c_str());
  std::stringstream p;
  p << port;

  m_fd = open_clientfd(hostnm, p.str().c_str());
  if (m_fd < 0){
    return;
  } 
  
  // assume successful open
  Rio_readinitb(&m_fdbuf, m_fd);
}

Connection::~Connection() {
  // TODO: close the socket if it is open
  close();
}

bool Connection::is_open() const {
  // TODO: return true if the connection is open
  return !(m_fd < 0);
}

void Connection::close() {
  // TODO: close the connection if it is open
  if (is_open()){
    Close(m_fd);
  }
}

bool Connection::send(const Message &msg) {
  // TODO: send a message
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately
  size_t msg_len = msg.tag.length() + msg.data.length() + 1;
  std::string send_msg;
  send_msg = msg.tag + ":" + msg.data;
  // send_msg += ":";
  // send_msg += msg.data;
  
  ssize_t write_len = rio_writen(m_fd, &send_msg[0], msg_len);

  if (write_len == -1){
    m_last_result = EOF_OR_ERROR;
    return false;
  }
  m_last_result = SUCCESS;
  return true;
}

bool Connection::receive(Message &msg) {
  // TODO: receive a message, storing its tag and data in msg
  // return true if successful, false if not
  // make sure that m_last_result is set appropriately

  // First set read_buf as void* to receive message
  char read_buf[msg.MAX_LEN];
  ssize_t read_len = rio_readlineb(&m_fdbuf, read_buf, msg.MAX_LEN);
  if (read_len == 0){
     m_last_result = EOF_OR_ERROR;
    return false;   
  }
  std::string buf = std::string(read_buf);
  size_t delim = buf.find(":");
  msg.tag = buf.substr(0, delim);
  msg.data = buf.substr(delim+1, std::string::npos);

  m_last_result = SUCCESS;
  return true;
}
