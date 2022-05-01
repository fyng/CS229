// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104

# Contributions #
*AS5MS1:*
We split the work between sender.cpp and receiver.cpp. Feiyang worked on sender.cpp while Tae Wan worked on receiver.cpp and the split_payload(). Both edited connection.cpp together.


*AS5MS2:*
The ares of code we mainly focused for concurrency and synchronization include a short block of code in our MessageQueue object's dequeue, the broadcast_message function in the created room, and the bulk of find_or_create_room method in the server.cpp. The following snippets of code are the exact pieces we wrote to synchonize our program. For the first case, we need to make sure we dequeue objects properly at the same time. To prevent discrepencies between each of the receivers in a room, it is needed that each individual MessageQueue dequeues the same message together, so each receiver is on the same page. Thus, we need to guard the the process of getting the same message and then popping it from out Queue. The next part we needed to focus on in concurrency is handling the case when we broadcast the message out to everyone. In this case we have the other side of dequeueing our message to the MessageQueue. We need to make sure that whatever message the senders give to the room goes to each present receiver. Thus, we have to protect the iterator for loop as well since each message must be made individual for each message Queue and given to each receivor sepeately. Finally, when handling the process of opening or creating a new room, we need to be careful that multiple clients don't create a room with the same name at the same time. It is possible that two clients try to make the same room, thus we need to make the rooms one at a time thus the clients don't end up apart from each other when they are supposed to be together in the same room. 

Guard g(m_lock);
msg = m_messages.front();
m_messages.pop_front();

Guard g(lock);
for (std::set<User *>::iterator it = members.begin(); it!=members.ed) {
    Message* msg = new Message(TAG_DELIVERY, room_name + ":" + sender_username + ":" + message_text;
    (*it)->mqueue.equeue(msg);
}

{
  Guard g(m_lock);
  auto i = m_rooms.find(room_name);
  if (i == m_rooms.end()) {
    room = new Room(room_name);
    m_rooms[room_name] = room
  } else {
    room = 1->seconds;
  }
}
