#pragma once
#include <chrono>
#include <iostream>
#include <windows.h>
#include <vector> 
#include "ServerGame.h"
#include <stdlib.h> 
#include <Windows.h>
#include <stdint.h> // portable: uint64_t   MSVC: __int64 
struct PacketTimeCompare
{
    int id;
    int packetType;
    int timestamp;
    int idClient;
};




void addPacketToVect( std::vector<PacketTimeCompare> &Vect, int packetType, int timestamp, int idClient);
void addFoundPacketToVect(int packetType, int timestamp, int idClient);
void PacketUpdateFoundGame(/*std::vector<PacketTimeCompare> &Vect,*/ ServerGame *server);
int getTime();
void timeNow();
void clearQueEndVect();
int gettimeofday();