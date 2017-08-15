//
//  Monitor.h
//  Robo-vm testing
//
//  Created by Ben Wyngaard on 8/6/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

#include "Monitor.h"

std::unique_ptr<std::thread> keyThread;

Monitor::Monitor(){}

void Monitor::start(){
    std::cout << 'a';
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr,true);
    keypad(stdscr, true);
    //keyThread = std::unique_ptr<std::thread>(new std::thread(keyThreadFunc,&keyPressed,&keyThreadShouldQuit));
}

void keyThreadFunc(int* key, bool* quit){
    //std::cout << "checking keys" << std::endl;
    while(!*quit){
        int c = getch();
        if (c != ERR){
            *key = c;
        }else{
            *key = 0;
        }
    }
}

void Monitor::update(std::vector<i16> m, i16 i){
    erase();
    std::string str;
    for (int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            char c = m[(y*64)+x+i];
            if (c == '\0') c = ' '; 
            str.push_back(c);
        }
        str.push_back('\n');
    }
    //std::cout << str.c_str();
    printw(str.c_str());
    refresh();
}

int Monitor::getKey(){
    return keyPressed;
}

void Monitor::close(){
    while (getch() != 'q'){
    }
    keyThreadShouldQuit = true;
    keyThread->join();
    endwin(); 
}
