#pragma once

#include "PacketTimeCompare.h"

std::vector<PacketTimeCompare> queEndVect;
int incrementQueEnd; // packet counter

//std::vector<PacketTimeCompare> queStartArr; // QUE START PACKETS
 // FOUND GAME PACKETS (QUE_END)


int getTime()
{
 return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch()).count();
}

void addPacketToVect(std::vector<PacketTimeCompare> &Vect, int id, int packetType, int timestamp, int idClient)
{
    PacketTimeCompare newVal = {id: id, packetType: packetType, timestamp: timestamp, idClient: idClient};
    Vect.push_back(newVal);
}

void addFoundPacketToVect(int packetType, int timestamp, int idClient)
{
    addPacketToVect(queEndVect, incrementQueEnd, packetType, timestamp, idClient);
}



int gettimeofday(struct timeval * tp, struct timezone * tzp)
{
    // Note: some broken versions only have 8 trailing zero's, the correct epoch has 9 trailing zero's
    // This magic number is the number of 100 nanosecond intervals since January 1, 1601 (UTC)
    // until 00:00:00 January 1, 1970 
    static const uint64_t EPOCH = ((uint64_t) 116444736000000000ULL);

    SYSTEMTIME  system_time;
    FILETIME    file_time;
    uint64_t    time;

    GetSystemTime( &system_time );
    SystemTimeToFileTime( &system_time, &file_time );
    time =  ((uint64_t)file_time.dwLowDateTime )      ;
    time += ((uint64_t)file_time.dwHighDateTime) << 32;

    tp->tv_sec  = (long) ((time - EPOCH) / 10000000L);
    tp->tv_usec = (long) (system_time.wMilliseconds * 1000);
    return 0;
}

void TimeNow()
{
timeval curTime;
gettimeofday(&curTime, NULL);
int milli = curTime.tv_usec / 1000;

char buffer [80];
strftime(buffer, 80, "%Y-%m-%d %H:%M:%S", localtime(&curTime.tv_sec));

char currentTime[84] = "";
sprintf(currentTime, "%s:%d", buffer, milli);
printf("current time: %s:03d \n", currentTime);
}

void clearQueEndVect()
{
    queEndVect.clear();
}

void PacketUpdateFoundGame(/*std::vector<PacketTimeCompare> &Vect,*/ ServerGame *server) {
    int timeNow;
    int timeDiff;
    bool foundMatchingPacket;
    for ( auto &i : queEndVect ) {

        for ( auto &j : queEndVect){
            if ( !(i.idClient == j.idClient) & (i.packetType == ACTION_STOP_QUE) ) {
                timeDiff = abs(i.timestamp - j.timestamp); // absolute value
                switch (timeDiff >= 200)
                {
                    case 1:
                        // STOP MATCHMAKING
                        //std::cout<< i.idClient<<std::endl;
                        std::cout<<"Stopped que >200ms difference and no flicker from client 2"<<std::endl;
                        server->sendPacket(ACTION_STOP_QUE);
                        
                        break;
                
                    case 0:
                        // SLEEP (ALLOW GAME POP)
                        clearQueEndVect();
                        // Global
                        incrementQueEnd = 0;
                        std::cout<<"Both clients found game GL HF :)"<<std::endl;
                        Sleep(5000);
                        clearQueEndVect();
                        server->sendPacket(ACTION_STOP_COORDINATOR);
                        goto end;
                        break;
                }
            } 
        }

        timeDiff = abs(getTime() - i.timestamp);
        
        std::cout<< "timestamp: " << i.timestamp << "  ClientID:  " << i.idClient << " packetType: " << i.packetType << std::endl;
        std::cout<< timeDiff <<std::endl;

        if( timeDiff > 200 ) {
            // STOP MATCHMAKING or REQUE
            //server->sendPacket(ACTION_STOP_QUE);
            server->sendPacket(ACTION_REQUE);
            std::cout<<"Stopped que since only client: "<< i.idClient <<" found game <200ms"<<std::endl;
            // REQUE
            std::cout<<"REQUE: "; TimeNow();
            clearQueEndVect();
            //Sleep(1000);
           /* queEndVect->clear();*/
        }
        //std::cout << i << std::endl;
    }


end:;

}