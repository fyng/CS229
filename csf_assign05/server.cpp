#include <pthread.h>
#include <iostream>
#include <sstream>
#include <memory>
#include <set>
#include <vector>
#include <cctype>
#include <cassert>
#include "message.h"
#include "connection.h"
#include "user.h"
#include "room.h"
#include "guard.h"
#include "server.h"

////////////////////////////////////////////////////////////////////////
// Server implementation data types
////////////////////////////////////////////////////////////////////////

// TODO: add any additional data types that might be helpful
//       for implementing the Server member functions
struct ConnInfo {
  Server *server;
  Connection* conn; 

  ConnInfo(Connection *conn, Server *server) : conn(conn), server(server) { }
  ~ConnInfo() {
    // destroy connection when ConnInfo object is destroyed
    delete conn;
  }
};


////////////////////////////////////////////////////////////////////////
// Client thread functions
////////////////////////////////////////////////////////////////////////

namespace {

void *worker(void *arg) {
  pthread_detach(pthread_self());

  // TODO: use a static cast to convert arg from a void* to
  //       whatever pointer type describes the object(s) needed
  //       to communicate with a client (sender or receiver)
  struct ConnInfo *info_ = static_cast<ConnInfo *>(arg);
  // use a std::unique_ptr to automatically destroy the ConnInfo object
  // when the worker function finishes; this will automatically ensure
  // that the Connection object is destroyed
  std::unique_ptr<ConnInfo> info(info_);

  // TODO: read login message (should be tagged either with
  //       TAG_SLOGIN or TAG_RLOGIN), send response
  Message msg;

  if (!info->conn->receive(msg)) {
    if (info->conn->get_last_result() == Connection::INVALID_MSG) {
      info->conn->send(Message(TAG_ERR, "invalid message"));
    }
    return nullptr;
  }

  if (msg.tag != TAG_SLOGIN && msg.tag != TAG_RLOGIN) {
    info->conn->send(Message(TAG_ERR, "first message should be slogin or rlogin"));
    return nullptr;
  }

  std::string username = msg.data;
  if (!info->conn->send(Message(TAG_OK, "welcome " + username))) {
    return nullptr;
  }

  // TODO: depending on whether the client logged in as a sender or
  //       receiver, communicate with the client (implementing
  //       separate helper functions for each of these possibilities
  //       is a good idea)
  if (msg.tag == TAG_SLOGIN) info->server->chat_with_sender(info, username);
  else if (msg.tag == TAG_RLOGIN) info->server->chat_with_receiver(info, username);

  return nullptr;
}
}

////////////////////////////////////////////////////////////////////////
// Server member function implementation
////////////////////////////////////////////////////////////////////////

Server::Server(int port)
  : m_port(port)
  , m_ssock(-1) {
  // TODO: initialize mutex
  pthread_mutex_init(&m_lock, nullptr);
}

Server::~Server() {
  // TODO: destroy mutex
  pthread_mutex_destroy(&m_lock);
}

bool Server::listen() {
  // TODO: use open_listenfd to create the server socket, return true
  //       if successful, false if not
  std::string port = std::to_string(m_port);
  m_ssock = open_listenfd(port.c_str());
  return m_ssock >= 0;
}

void Server::handle_client_requests() {
  // TODO: infinite loop calling accept or Accept, starting a new
  //       pthread for each connected client
  // Create a thread for each accepted client connection, may use a struct pass connection object and other data to create the thread
  // Use worker() as the entry point for the thread
  // Create a User object in each client thread, perhaps make a room object to keep track of who is which server room
  // Need a chat_with_sender() loop function and char_with_receiver() loop too
  // Always send back with err or ok tag, do not send back to a receiver with the same login as the receiver
  // In client receiver loop, if quit or transmission fail, break loop and delete thread, don't forget memory leaks
  
  while (1) {
    //wait_for_avail_proc();
    int clientfd = accept(m_ssock, NULL, NULL);
    if (clientfd < 0) {
      std::cerr << "Error accepting connection\n";
      return;
    }

    ConnInfo *info = new ConnInfo(new Connection(clientfd), this);
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, worker, static_cast<void *>(info)) != 0) {
      std::cerr << "could not create thread\n";
      return;
    }


  }
}

Room *Server::find_or_create_room(const std::string &room_name) {
  // TODO: return a pointer to the unique Room object representing
  //       the named chat room, creating a new one if necessary

  // this function can be called from multiple threads, so
  // make sure the mutex is held while accessing the shared
  // data (the map of room names to room objects)
  Guard g(m_lock);

  Room *room;

  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    // room does not exist yet, so create it and add it to the map
    room = new Room(room_name);
    m_rooms[room_name] = room;
  } else {
    room = i->second;
  }

  return room;
}

void Server::chat_with_sender(std::unique_ptr<ConnInfo> &info, const std::string &username){
  Message msg;
  Room * room = NULL;

  while (true) {
    if (!info->conn->receive(msg)) {
      if (info->conn->get_last_result() == Connection::INVALID_MSG) {
        info->conn->send(Message(TAG_ERR, "invalid message"));
      }
      break;
    }

    if (msg.tag == TAG_QUIT) {
      info->conn->send(Message(TAG_OK, "bye!"));
      break;
    }
    else if (msg.tag == TAG_JOIN) {
      room = find_or_create_room(msg.data);
      info->conn->send(Message(TAG_OK, "Joined" + room->get_room_name()));
    } else if (msg.tag == TAG_SENDALL) {
      room->broadcast_message(username, msg.data);
      info->conn->send(Message(TAG_OK, "message send"));
    } else if (msg.tag == TAG_LEAVE) {
      room = NULL;
      info->conn->send(Message(TAG_OK, "left" + room->get_room_name()));
    }
  }
}


