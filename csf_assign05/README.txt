// Feiyang Huang; fhuang15
// Tae Wan Kim; tkim104

# Contributions #
*AS5MS1:*
We split the work between sender.cpp and receiver.cpp. Feiyang worked on sender.cpp while Tae Wan worked on receiver.cpp and the split_payload(). Both edited connection.cpp together.


*AS5MS2:*
In general, a critical section is a data structure or object that are potentially read/accessed/modified by multiple sender or receiver threads concurrently. Multiple threads accessing and modifying the same data could lead to data corruption and data loss. If multiple threads reading the same data which are potentially modified, the program cannot guarantee consistent read output.

Critical sections are protected by mutex. A mutex is initiated (pthread_mutex_init) in the constructor of the critical section object and destroyed in the destructor (pthread_mutex_destroy). This is done in the following:
1. In the Room class, when calling the constructor and destructor
2. In message_queue constructor and destructor. 

In the case, we also use semiphore to track the resource in the queue. This puts a dequeue request to sleep if the queue is empty and wakes it up again when a message is enqueued by another thread, thus preventing an invalid dequeue operation on an empty queue.

The following instance requires synchronization to protect the critical section during concurrent running. This is done by calling the Guard object, which ensures the mutex is locked in its constructor and release the mutex in its destructor:
1. In server.cpp, find_or_create_room()
2. In server.cpp, calling Room::broadcast_message() in Server::chat_with_sender
3. In Room::broadcast_message(), when iterating through Room::UserSet to add to each user's MessageQueue
4. In message_queue::enqueue() and message_queue::dequeue(), when we modify the deque.

Race condition refers to when 2 threads concurrently modify the same resource and the order of the modification cannot be guaranteed, so the program cannot guarantee the output. This occurs if critical sections are not protected to ensure synchronicity. A deadlock refers to when we have circular resource references. Each waits on its dependency to unlock the mutex before processing, thus no process is able to proceed and release its own mutex. Both are prevented by correctly calling on the Guard object to lock the mutex before a thread is allowed to modify resources. The Guard object automates the release of the mutex when it is destroyed, which helps to prevent deadlock.

The following snippets of code are the exact pieces we wrote to synchonize our program:
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

For the first case, we need to make sure we dequeue objects properly at the same time. To prevent discrepencies between each of the receivers in a room, it is needed that each individual MessageQueue dequeues the same message together, so each receiver is on the same page. Thus, we need to guard the the process of getting the same message and then popping it from out Queue. 

The next part we needed to focus on in concurrency is handling the case when we broadcast the message out to everyone. In this case we have the other side of dequeueing our message to the MessageQueue. We need to make sure that whatever message the senders give to the room goes to each present receiver. Thus, we have to protect the iterator for loop as well since each message must be made individual for each message Queue and given to each receivor sepeately. 

Finally, when handling the process of opening or creating a new room, we need to be careful that multiple clients don't create a room with the same name at the same time. It is possible that two clients try to make the same room, thus we need to make the rooms one at a time thus the clients don't end up apart from each other when they are supposed to be together in the same room. 


