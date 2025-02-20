#ifndef MESSAGE_H
#define MESSAGE_H

#include <vector>
#include <string>

struct Message {
  // An encoded message may have at most this many characters,
  // including the trailing newline ('\n'). Note that this does
  // *not* include a NUL terminator (if one is needed to
  // temporarily store the encoded message.)
  static const unsigned MAX_LEN = 255;

  std::string tag;
  std::string data;

  Message() { }

  Message(const std::string &tag, const std::string &data)
    : tag(tag), data(data) { }

  // returns the payload (data) as a vector of strings,
  // split using ':' as the separator
  std::vector<std::string> split_payload() const {
    std::vector<std::string> result;
    // TODO: split the message data into fields separated by ':', add them
    //       to result vector
    // Read the first tag to set the message vector[0] as the tag
    result.push_back(tag);

    // Set read the data messages in between
    std::size_t data_segment_start = 0;
    std::size_t found = data.find(":");
    while (found!=std::string::npos) {
      result.push_back(data.substr(data_segment_start, found - data_segment_start));
      data_segment_start = found + 1;
      found = data.find(":", data_segment_start);
    }
    
    // Read last data message that does not end with :
    result.push_back(data.substr(data_segment_start, found - data_segment_start));
    return result;
  }
};

// standard message tags (note that you don't need to worry about
// "senduser" or "empty" messages)
#define TAG_ERR       "err"       // protocol error
#define TAG_OK        "ok"        // success response
#define TAG_SLOGIN    "slogin"    // register as specific user for sending
#define TAG_RLOGIN    "rlogin"    // register as specific user for receiving
#define TAG_JOIN      "join"      // join a chat room
#define TAG_LEAVE     "leave"     // leave a chat room
#define TAG_SENDALL   "sendall"   // send message to all users in chat room
#define TAG_SENDUSER  "senduser"  // send message to specific user in chat room
#define TAG_QUIT      "quit"      // quit
#define TAG_DELIVERY  "delivery"  // message delivered by server to receiving client
#define TAG_EMPTY     "empty"     // sent by server to receiving client to indicate no msgs available

#endif // MESSAGE_H
