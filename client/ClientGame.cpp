#include "stdafx.h"
#include "ClientGame.h" 

ClientGame::ClientGame(PCSTR IPv4)
{
    network = new ClientNetwork(IPv4);

    // send init packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = INIT_CONNECTION;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
    
}

void ClientGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::sendPacket(int packetType)
{
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = packetType;

    packet.serialize(packet_data);

    NetworkServices::sendMessage(network->ConnectSocket, packet_data, packet_size);
}

void ClientGame::update(void (*QueON)(), void (*QueOFF)(), int *QueState, bool *shouldBeQueueing)
{
    Packet packet;
    int data_length = network->receivePackets(network_data);

    if (data_length <= 0) 
    {
        //no data recieved
        return;
    }

    int i = 0;
    while (i < (unsigned int)data_length) 
    {
        packet.deserialize(&(network_data[i]));
        i += sizeof(Packet);

        switch (packet.packet_type) {
            case ACTION_EVENT:
                printf("client received action event packet from server\n");
                break;

            case ACTION_START_QUE:
                *shouldBeQueueing = true;
                QueON();
                printf("server->client = ACTION_START_QUE\n");
                break;

            case ACTION_STOP_QUE:
                *shouldBeQueueing = false;
                QueOFF();
                printf("server->client = ACTION_STOP_QUE\n");
                break;

            case ACTION_START_COORDINATOR:
                *QueState = 1;
                *shouldBeQueueing = true;
                printf("server->client = ACTION_START_COORDINATOR\n");
                QueON();
                break;

            case ACTION_STOP_COORDINATOR:
                *shouldBeQueueing = false;
                QueOFF();
                printf("server->client = ACTION_STOP_COORDINATOR\n");
                *QueState = 0;
                break;

            case ACTION_REQUE:
                printf("RECEIVED REQUE ACTION\n");
                *shouldBeQueueing = false;
                QueOFF();
                Sleep(1000);
                QueON();
                *shouldBeQueueing = true;
                break;

            default:
                printf("error in packet types\n");
                break;
        }
    }
}

