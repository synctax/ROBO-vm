//
//  VM.h
//  Robo-vm testing
//
//  Created by Ben Wyngaard on 8/6/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

#include <iostream>
#include <vector>
#include "Monitor.h"

#ifndef VM_h
#define VM_h

typedef uint16_t i16;

class VM{
    i16 running = 1;

    i16 pc = 0;
    i16 sp = 0;
    i16 ax = 0;
    i16 bx = 0;
    i16 cx = 0;
    i16 dx = 0;
    i16 flags = 0; //zero
    const i16 memSize = 64000;
    const i16 vRamStart = memSize - 2048;
    const i16 keyAddr = vRamStart-2;
    std::vector<i16> memory;

    i16* registers[6] = {&ax,&bx,&cx,&dx,&pc,&sp};

    Monitor screen;

    void executeNext();
    void updateZeroFlag();
    void startMonitor();
    void updateMonitor();
    void getKey();
    void closeMonitor();

public:
    VM();
    void loadDisk(std::vector<i16> disk);
    void attachMonitor(Monitor m);
    void run();
    void close();
};

#endif /* VM_h */
