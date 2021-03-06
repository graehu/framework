#ifndef NET_H
#define NET_H

/// Include


#include "connection/connection.h"
#include "connection/stats.h"
#include "../entities/networked/netEntity.h"




using namespace std;

#if PLATFORM == PLATFORM_WINDOWS

inline void waitsecs(float seconds)
{
    Sleep((int)(seconds * 1000.0f));
}

#else

inline void waitsecs(float seconds)
{
    usleep((int)(seconds * 1000000.0f));
}

#endif

namespace net
{

    /*enum entityState
    {
        e_uninitialised = 0,
        e_initialised,
        e_updating
    };

    enum packets
    {
    	e_initPacket = 0,
    	e_playerUpdatePacket,
    	e_totalPackets, //e_entityUpdate after this on purpose
    	e_entityUpdatePacket //This is a special event, it assumes the rest of the packet is entity updates
    };*/

    class network : protected connection
    {
        public:

        network(unsigned short _protocolID, float _timeout , unsigned int _maxSequence = 0xFFFFFFFF);
        ~network();
        bool init(bool _host, int _port);
		bool update(float _deltaTime);

		packet* recievePacket(void);
		void sendPacket(packet* _packet, float _deltaTime);

        //void addEntity(netEntity* _newEntity);
        //netEntity* getEntity(unsigned int _element);
        //bool getType(void){return m_host;}

        protected:
        private:

        //void initEntity(unsigned short _packetSender, unsigned short _accessKey);


	bool m_host;
        /// this will have to be something like this eventually
        /// vector<pair<entity*,vector<unsigned short(sendKeys)> > >
        /*vector<netEntity*> m_entities;
        struct enInfo
        {
            enInfo(unsigned short _enKey = 0, entityState _state = e_uninitialised) :
            m_enKey(_enKey),
            m_state(_state){};

            unsigned short m_enKey;
            entityState m_state;
        };
        ///state list PER connection...
        ///this might want to be a vector<pair<there, >> mEntityConnKeys
        vector<vector<enInfo> >m_enUpdate; ///these are all the keys a client knows about.*/
        unsigned int m_packetSize; /// this is the maximum size of a packet expect by the network
                                  /// implemented in net.cpp

    };

}



#endif
