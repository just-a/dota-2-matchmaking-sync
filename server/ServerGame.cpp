#pragma once
#include "stdafx.h" 
#include "ServerGame.h"
#include "PacketTimeCompare.h"
unsigned int ServerGame::client_id; 

ServerGame::ServerGame(void)
{
    // id's to assign clients for our table
    client_id = 0;

    // set up the server network to listen 
    network = new ServerNetwork(); 
}

void ServerGame::receiveFromClients()
{

    Packet packet;

    // go through all clients
    std::map<unsigned int, SOCKET>::iterator iter;

    for(iter = network->sessions.begin(); iter != network->sessions.end(); iter++)
    {
        int data_length = network->receiveData(iter->first, network_data);

        if (data_length <= 0) 
        {
            //no data recieved
            continue;
        }

        int i = 0;
        while (i < (unsigned int)data_length) 
        {
            packet.deserialize(&(network_data[i]));
            i += sizeof(Packet);

            switch (packet.packet_type) {

                case INIT_CONNECTION:

                    printf("server received init packet from client\n");
                    break;

                case ACTION_EVENT:

                    printf("server received action event packet from client\n");
                    break;

                case ACTION_START_QUE:
                {
                    auto time =  std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::cout<< std::ctime(&time) << ": server received START_QUE action packet from client" <<std::endl;
                    clearQueEndVect();
                    break;
                }

                case ACTION_STOP_QUE:
                {
                    auto time =  std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                    std::cout<< std::ctime(&time) << ": server received STOP_QUE action packet from client" <<std::endl;
                    addFoundPacketToVect(ACTION_STOP_QUE,  getTime(), iter->first );
                    break;
                }

                default:

                    printf("error in packet types\n");
                    std::cout<< "Packet type: " << packet.packet_type <<std::endl;
                    break;
            }

        }
    }
}


void ServerGame::update() 
{

    // get new clients
    if(network->acceptNewClient(client_id))
    {
        printf("client %d has been connected to the server\n",client_id); 

        client_id++;
    }

    receiveFromClients();
}

void ServerGame::sendPacket(int packetType)
{
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];
    
    Packet packet;
    packet.packet_type = packetType;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}

void ServerGame::sendActionPackets()
{
    // send action packet
    const unsigned int packet_size = sizeof(Packet);
    char packet_data[packet_size];

    Packet packet;
    packet.packet_type = ACTION_EVENT;

    packet.serialize(packet_data);

    network->sendToAll(packet_data,packet_size);
}

void ServerGame::shutdown()
{
    network->closeConnections();
}