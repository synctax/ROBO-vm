//
//  Monitor.h
//  Robo-vm testing
//
//  Created by Ben Wyngaard on 8/6/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

#include <iostream>
#include <ncurses.h>
#include <sstream>
#include <string>
#include <vector>
#include <thread>

#ifndef Monitor_h
#define Monitor_h

typedef uint16_t i16;

class Monitor{
public:
    Monitor();
    void start();
    void update(std::vector<i16> m, i16 i);
    void close();
};

#endif
