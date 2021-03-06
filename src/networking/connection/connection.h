#ifndef CONNECTION_H
#define CONNECTION_H

#include "stats.h"
#include "flowControl.h"
#include "socket.h" /// has address in it.
#include "../packet/packet.h"

extern char message[];
#define myprintf(a, ...) 
//sprintf(message, a,  __VA_ARGS__);
//OutputDebugString(message);

///friendly user-data-group encapsulator

///the init packet
///{
///short protocolId;    // this is just a security measure
///short sendKey;       // this is the key you send, so as they can directly access you in their mail List
///short ReceiveKey;    // this is the key to access them in your mailList
///}

namespace net
{
  struct GameHeader
  {
    int16_t protocol_id;
    int16_t sender_id;
    int32_t current_id;
    int32_t known_id;
    int32_t ack_bits;
  };
  typedef NewPacket<256, GameHeader> GamePacket;

class connection
{
public:

    connection(unsigned short _protocolID, float _timeout, unsigned int _maxSequence = 0xFFFFFFFF );
    virtual ~connection();
    bool start(int _port);
    void stop();
    void connect(const address &_address);

    void update(float _deltaTime);
    bool sendPacket(packet* _packet, unsigned short _key, float _deltaTime);
    packet* receivePacket(unsigned int _size);
    int getHeaderSize() const;
    unsigned short getMailListSize(void){return m_mailList.size();}
    unsigned int getBytesRead(void){return m_bytesRead;}

protected:

    unsigned int m_port;

private:

    enum state
    {
        e_disconnected = 0,
        e_connecting,
        e_connected
    };

    struct sender
    {
        sender(unsigned int max):
			m_state(e_disconnected),
			m_timeoutAccumulator(0),
			m_sendAccumulator(0),
			m_stats(max){};

        state m_state;
        float m_timeoutAccumulator;
        float m_sendAccumulator;
        address m_address;
        stats m_stats;
        flowControl m_flow;
    };

    unsigned int m_bytesRead;
    unsigned short m_protocolID;                                  /// the protocal id
    float m_timeout;                                              /// how long before someone will time-out
    unsigned int m_maxSequence;                                  /// max sequence for reliablesystem
    bool m_running;                                               /// is this connection running

    socket m_socket;                                              /// this is a socket
    std::vector<std::pair<sender*, unsigned short> > m_mailList;  /// these are all the connections, and their send keys.
    std::vector<unsigned short> m_keyPool;                        /// these are keys that can be reused
    std::vector<unsigned short> m_newConnKeys;                     /// this stores all the new connection's Keys

    float m_sendAccumulator;                                      /// this is so as the rate at which initialisation packets are sent
                                                                /// can be maintained
};

}

#endif//CONNECTION_H
