#pragma once
#include <string.h>

#define MAX_PACKET_SIZE 1000000

enum PacketTypes {

    INIT_CONNECTION = 0,
    ACTION_EVENT = 1,
    ACTION_STOP_QUE = 2,
    ACTION_START_QUE = 3,
    ACTION_START_COORDINATOR = 4,
    ACTION_STOP_COORDINATOR = 5,
    ACTION_REQUE = 6

}; 

struct Packet {

    unsigned int packet_type;

    void serialize(char * data) {
        memcpy(data, this, sizeof(Packet));
    }

    void deserialize(char * data) {
        memcpy(this, data, sizeof(Packet));
    }
};