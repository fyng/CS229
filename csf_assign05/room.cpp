#include "guard.h"
#include "message.h"
#include "message_queue.h"
#include "user.h"
#include "room.h"
#include "client_util.h"

Room::Room(const std::string &room_name)
  : room_name(room_name) {
  // TODO: initialize the mutex
  pthread_mutex_init(&lock, NULL);
}

Room::~Room() {
  // TODO: destroy the mutex
  pthread_mutex_destroy(&lock);
}

void Room::add_member(User *user) {
  // TODO: add User to the room
  std::set<User *>::iterator it = members.find(user);
  if (it == members.end()) {
    members.insert(user);
  }
}

void Room::remove_member(User *user) {
  // TODO: remove User from the room
  std::set<User *>::iterator it = members.find(user);
  if (it != members.end()) {
    members.erase(user);
  }
}

void Room::broadcast_message(const std::string &sender_username, const std::string &message_text) {
  // TODO: send a message to every (receiver) User in the room
  Guard g(lock);
  for (std::set<User *>::iterator it=members.begin(); it!=members.end(); ++it) {
    Message* msg = new Message(TAG_DELIVERY, room_name + ":" + sender_username + ":" + message_text);
    (*it)->mqueue.enqueue(msg);
  }
}
