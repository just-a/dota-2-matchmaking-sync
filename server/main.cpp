#pragma once
// may need #include "stdafx.h" in visual studio
#include "stdafx.h"
#include "ServerGame.h"
#include "PacketTimeCompare.h"

// used for multi-threading
#include <process.h>
#include <iostream>
#include <unistd.h>


void serverLoop(void *);
void packetCheckLoop(void* );
void initConsole();
ServerGame * server;

// states:
// 0 = off (turn auto re-que off on ACTION_STOP_QUE)
// 1 = on (turn auto re-que on on ACTION_STOP_QUE)

int main()
{
// console config
initConsole();

// initialize the server
server = new ServerGame();

    // create thread with arbitrary argument for the run function
    _beginthread( serverLoop, 0, (void*)12);
    _beginthread( packetCheckLoop, 0, (void*)10 );

    while(true)
    {
        std::cout<<">";
        std::string input;
        std::cin>>input;

        if(input=="clear") { system("cls"); }
        if(input=="start") { server->sendPacket(ACTION_START_COORDINATOR); }
        if(input=="stop") { server->sendPacket(ACTION_STOP_COORDINATOR); }
        if(input=="reque") { server->sendPacket(ACTION_REQUE); }
        if(input=="exit") { server->shutdown(); return 0; }
    }
}

void initConsole()
{
    HWND console = GetConsoleWindow();

    SetWindowPos(console, HWND_TOPMOST, 1205, 93, 702, 253,  SWP_DRAWFRAME | SWP_SHOWWINDOW);
    //ShowWindow(console, SW_NORMAL);
    //RECT r;
    //GetWindowRect(console, &r); //stores the console's current dimensions
    //MoveWindow(window_handle, x, y, width, height, redraw_window);
    
    //ShowScrollBar(console, SB_VERT, FALSE);
}

void serverLoop(void * arg) 
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
        server->update();
    }
}

void packetCheckLoop(void * arg)
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
        PacketUpdateFoundGame(server);
    }
}