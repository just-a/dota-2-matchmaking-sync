// may need #include "stdafx.h" in visual studio
#include "stdafx.h"

#include "ClientGame.h"
#include "QueExploit.h"
// used for multi-threading
#include <process.h>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <windows.h>
#include <unistd.h>
#include <array>
#include <chrono>


#define LOG(x) std::cout<< x <<std::endl

void clientLoop(void*);
void queUpdateLoop(void*);
void initConsole();

ClientGame * client;

int main()
{

    initConsole();
 
    if (!Dota2) { printf("DOTA 2 CLIENT NOT FOUND!"); getch(); return 0; }
    printf("FOUND DOTA CLIENT\n");

    std::string IPv4;
    std::cout << "Enter server IP:" << std::endl;
    std::cout<<">";
    std::cin >> IPv4;


    // initialize the client
    client = new ClientGame(IPv4.c_str());
    _beginthread(clientLoop, 0, (void*)12);
   
    std::cout << "Connected!" <<std::endl;

    _beginthread(queUpdateLoop, 0, (void*)0);

    while(true)
    {
        std::cout<<">";
        std::string input;
        std::cin>>input;

        if(input=="clear") { system("cls"); }
        if(input=="stop") { QueOFF(); }
        if(input=="start") { QueON(); }
        if(input=="exit") { return 0; }
        if(input=="fakestop") { client->sendPacket(ACTION_STOP_QUE); }
    }
}

void clientLoop(void * arg) //receiving packets !!
{
/////////////////////limiting cycles////////////////////
int now;
int lastFrame = getTime();
        while (true) {
/////////////////////limiting cycles////////////////////
now = getTime();
int delta = now - lastFrame;
lastFrame = now;
    if(delta < 33) // 1000/30
    {
        Sleep(33 - delta);
    }
/////////////////////limiting cycles////////////////////
       client->update(QueON, QueOFF, &QueState, &shouldBeQueueing);
    }
}

void queUpdateLoop(void * arg)
{
/////////////////////limiting cycles////////////////////
        int now;
        int lastFrame = getTime();
        while (true) {
/////////////////////limiting cycles////////////////////
        now = getTime();
        int delta = now - lastFrame;
        lastFrame = now;
            if(delta < 33) // 1000/30
            {
                Sleep(33 - delta);
            }
/////////////////////limiting cycles////////////////////
       
        bool changed = set(&QueStatus, isQueueing());
        if(changed){
           
            switch(QueStatus){
                case 1:
                {
                    LOG("QueStatus: "<<QueStatus);
                    //InvalidateRect( statusWindowHandle, NULL, FALSE );
                    if ( (QueState == 1) && (shouldBeQueueing == false) ) { client->sendPacket( ACTION_START_QUE ); LOG("SENT START ACTION"); }
                }
                break; 
                case 0:
                {
                    LOG("QueStatus: "<<QueStatus);
                    //InvalidateRect( statusWindowHandle, NULL, FALSE );
                    if ( (QueState == 1) && (shouldBeQueueing == true) ) { client->sendPacket( ACTION_STOP_QUE ); LOG("SENT STOP ACTION"); }
                }
                break;
                default:
                    LOG("Incorrect value");
                break;
            }
        }


    }
}
