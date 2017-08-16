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
    start_color();
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
    char symbol;
    int FGcolor, BGcolor;
    for (int y = 0; y < 32; y++){
        for(int x = 0; x < 64; x++){
            int c = m[(y*64)+x+i];
            symbol = c&0x00ff;
            std::cout << symbol;
            if (symbol != '\0'){
                FGcolor = (c&0xf000) >> 12;
                BGcolor = (c&0x0f00) >> 8;
                init_pair(1, FGcolor, BGcolor);
                //init_pair(1,COLOR_WHITE, COLOR_RED);
                attron(COLOR_PAIR(1));
                mvaddch(y,x,symbol);
                attroff(COLOR_PAIR(1));
            }
        }
    }
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
