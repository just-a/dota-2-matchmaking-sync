#include <winsock2.h>
#include <Windows.h>
#include "ClientNetwork.h"
#include "NetworkData.h"

class ClientGame
{
    public:
        ClientGame(PCSTR IPv4);
        ~ClientGame(void);
        ClientNetwork* network; 
        void sendActionPackets();
        void sendPacket(int packetType);
        char network_data[MAX_PACKET_SIZE];
        void update(void (*QueON)(), void (*QueOFF)(), int *QueState, bool *shouldBeQueueing);
};