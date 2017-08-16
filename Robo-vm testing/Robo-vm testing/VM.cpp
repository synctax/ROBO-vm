//
//  VM.cpp
//  Robo-vm testing
//
//  Created by Ben Wyngaard on 8/6/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

/*

    instructions
        MOV reg, reg
        MOV reg, address
        MOV reg, regaddress
        MOV reg, const
        MOV address, reg
        MOV address, regaddress
        MOV regadress, reg
        ADD reg, reg
        ADD reg, address
        ADD reg, regaddress
        SUB reg, reg
        SUB reg, address
        SUB reg, regaddress
        INC reg
        DEC reg
        MUL reg
        MUL address
        MUL regaddress
        DIV reg
        DIV address
        DIV regaddress
        MOD reg
        MOD address
        MOD regadress
        AND reg, reg
        AND reg, address
        AND reg, regaddress
        OR reg, reg
        OR reg, address
        OR reg, regaddress
        XOR reg, reg
        XOR reg, address
        XOR reg, regaddress
        NOT reg
        SHL reg, reg
        SHL reg, address
        SHL reg, regaddress
        SHR reg, reg
        SHR reg, address
        SHR reg, regaddress
        CMP reg, reg
        CMP reg, address
        CMP reg, regaddress
        JMP address
        CALL address
        RET
        PUSH reg
        PUSH address
        PUSH regaddress
        POP reg
        HLT
        JE	=	Z = TRUE
        JNE	!=	Z = FALSE
 */
#include <stdio.h>
#include <math.h>
#include "VM.h"

VM::VM(){
    memory.resize(memSize);
    //std::replace (memory.begin()+vRamStart, memory.end(), 0, 32);
}

void VM::loadDisk(std::vector<i16> disk){
    for (int i = 0; i < disk.size(); i+=2){ 
        memory[pc+floor(i/2)] = (i16)(disk[i] << 8)+(i16)(disk[i+1]);
    }
}

void VM::attachMonitor(Monitor m){
    screen = m;
}

void VM::startMonitor(){
    screen.start();
}

void VM::updateMonitor(){
    if (memory[vRamStart-1] == 1){
        screen.update(memory, vRamStart);
        memory[vRamStart - 1] = 0;
    }
}

void VM::closeMonitor(){
    screen.close();
}

void VM::getKey(){
    memory[keyAddr] = screen.getKey();
}

void VM::run(){
    running = 1;
    pc -= 1;
    startMonitor();
    while (running == 1){
        /*std::cout << "PC:" << pc << std::endl;
        std::cout << "SP:" << sp << std::endl;
        std::cout << "AX:" << ax << std::endl;
        std::cout << "BX:" << bx << std::endl;
        std::cout << "CX:" << cx << std::endl;
        std::cout << "DX:" << dx << std::endl;
        std::cout << "FLAGS:" << flags << std::endl;
        std::cout << "-------" << std::endl;
        //for(int i = 1000; i < 1400; i++){
           // std::cout << memory[i]; 
        //}
        //std::cout << std::endl;
        //std::cout << "-------" << std::endl;
        //std::cout << memory[keyAddr] << std::endl;*/
        executeNext();
        updateMonitor();
        getKey(); 
    }
    std::cout << "halting" << std::endl;
}

void VM::close(){
    closeMonitor();
}

void VM::updateZeroFlag(){
    if (*registers[memory[pc+1]] == 0){
        flags = 1;
    }else{
        flags = 0;
    }
}

void VM::executeNext(){
    pc++;
    switch (memory[pc]) {
        case 0:                                                     //MOV reg, reg
            *registers[memory[pc+1]] = *registers[memory[pc+2]];
            pc+=2;
            break;

        case 1:                                                     //MOV reg, address
            *registers[memory[pc+1]] = memory[memory[pc+2]];
            pc+=2;
            break;

        case 2:                                                     //MOV reg, regaddress
            *registers[memory[pc+1]] = memory[*registers[memory[pc+2]]];
            pc+=2;
            break;

        case 3:                                                     //MOV reg, const
            *registers[memory[pc+1]] = memory[pc+2];
            pc+=2;
            break;

        case 4:                                                     //MOV address, reg
            memory[memory[pc+1]] = *registers[memory[pc+2]];
            pc+=2;
            break;

        case 5:                                                     //MOV address, regaddress
            memory[memory[pc+1]] = memory[*registers[memory[pc+2]]];
            pc+=2;
            break;

        case 6:                                                     //MOV regaddress, reg
            memory[*registers[memory[pc+1]]] = *registers[memory[pc+2]];
            pc+=2;
            break;

        case 7:                                                     //ADD reg, reg
            *registers[memory[pc+1]] += *registers[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 8:                                                     //ADD reg, address
            *registers[memory[pc+1]] += memory[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 9:                                                     //ADD reg, regaddress
            *registers[memory[pc+1]] += memory[*registers[memory[pc+2]]];
            updateZeroFlag();
            pc+=2;
            break;

        case 10:                                                     //SUB reg, reg
            *registers[memory[pc+1]] -= *registers[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 11:                                                     //SUB reg, address
            *registers[memory[pc+1]] -= memory[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 12:                                                    //SUB reg, regaddress
            *registers[memory[pc+1]] -= memory[*registers[memory[pc+2]]];
            updateZeroFlag();
            pc+=2;
            break;

        case 13:                                                    //INC reg
            ++*registers[memory[pc+1]];
            updateZeroFlag();
            pc++;
            break;

        case 14:                                                    //DEC reg
            --*registers[memory[pc+1]];
            updateZeroFlag();
            pc++;
            break;

        case 15:                                                    //MUL reg
            *registers[0] *= *registers[memory[pc+1]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 16:                                                    //MUL address
            *registers[0] *= memory[memory[pc+1]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 17:                                                    //MUL regaddress
            *registers[0] *= memory[*registers[memory[pc+1]]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 18:                                                    //DIV reg
            *registers[0] /= *registers[memory[pc+1]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 19:                                                    //DIV address
            *registers[0] /= memory[memory[pc+1]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 20:                                                    //DIV regaddress
            *registers[0] /= memory[*registers[memory[pc+1]]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 21:                                                    //AND reg, reg
            *registers[memory[pc+1]] &= *registers[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 22:                                                    //AND reg, address
            *registers[memory[pc+1]] &= memory[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 23:                                                    //AND reg, regaddress
            *registers[memory[pc+1]] &= memory[*registers[memory[pc+2]]];
            updateZeroFlag();
            pc+=2;
            break;

        case 24:                                                    //OR reg, reg
            *registers[memory[pc+1]] |= *registers[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 25:                                                    //OR reg, address
            *registers[memory[pc+1]] |= memory[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 26:                                                    //OR reg, regaddress
            *registers[memory[pc+1]] |= memory[*registers[memory[pc+2]]];
            updateZeroFlag();
            pc+=2;
            break;

        case 27:                                                    //XOR reg, reg
            *registers[memory[pc+1]] ^= *registers[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 28:                                                    //XOR reg, address
            *registers[memory[pc+1]] ^= memory[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 29:                                                    //XOR reg, regaddress
            *registers[memory[pc+1]] ^= memory[*registers[memory[pc+2]]];
            updateZeroFlag();
            pc+=2;
            break;

        case 30:                                                    //NOT reg
            *registers[memory[pc+1]] = ~*registers[memory[pc+1]];
            updateZeroFlag();
            pc++;
            break;

        case 31:                                                    //SHL reg, reg
            *registers[memory[pc+1]] <<= *registers[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 32:                                                    //SHL reg, address
            *registers[memory[pc+1]] <<= memory[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 33:                                                    //SHL reg, regaddress
            *registers[memory[pc+1]] <<= memory[*registers[memory[pc+2]]];
            updateZeroFlag();
            pc+=2;
            break;

        case 34:                                                    //SHR reg, reg
            *registers[memory[pc+1]] >>= *registers[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 35:                                                    //SHR reg, address
            *registers[memory[pc+1]] >>= memory[memory[pc+2]];
            updateZeroFlag();
            pc+=2;
            break;

        case 36:                                                    //SHR reg, regaddress
            *registers[memory[pc+1]] >>=memory[*registers[memory[pc+2]]];
            updateZeroFlag();
            pc+=2;
            break;

        case 37:                                                    //CMP reg, reg
            if (*registers[memory[pc+1]] - *registers[memory[pc+2]] == 0){
                flags = 1;
            }else{
                flags = 0;
            }
            pc+=2;
            break;

        case 38:                                                    //CMP reg, address
            if (*registers[memory[pc+1]] - memory[memory[pc+2]] == 0){
                flags = 1;
            }else{
                flags = 0;
            }
            pc+=2;
            break;

        case 39:                                                    //CMP reg, regaddress
            if (*registers[memory[pc+1]] - memory[*registers[memory[pc+2]]] == 0){
                flags = 1;
            }else{
                flags = 0;
            }
            pc+=2;
            break;

        case 40:                                                    //JMP address
            pc = memory[pc+1]-1;
            break;

        case 41:                                                    //CALL reg
            sp++;
            memory[sp] = pc+1;
            pc = *registers[memory[pc+1]]-1;
            break;

        case 42:                                                    //CALL address
            sp++;
            memory[sp] = pc+1;
            pc = memory[pc+1]-1;
            break;

        case 43:                                                    //CALL regaddress
            sp++;
            memory[sp] = pc+1;
            pc = memory[*registers[memory[pc+1]]]-1;
            break;

        case 44:                                                    //RET
            pc = memory[sp];
            sp--;
            break;

        case 45:                                                    //PUSH reg
            sp++;
            memory[sp] = *registers[memory[pc+1]];
            pc++;
            break;

        case 46:                                                    //PUSH address
            sp++;
            memory[sp] -= memory[memory[pc+1]];
            pc++;
            break;

        case 47:                                                    //PUSH regaddress
            sp++;
            memory[sp] = memory[*registers[memory[pc+1]]];
            pc++;
            break;

        case 48:                                                    //POP reg
            *registers[memory[pc+1]] = memory[sp];
            sp--;
            pc++;
            break;

        case 49:                                                    //HLT
            running = 0;
            break;

        case 50:                                                    //JE reg
            if (flags == 1){
                pc = *registers[memory[pc+1]]-1;
            }else{
                pc++;
            }
            break;


        case 51:                                                    //JE address
            if (flags == 1){
                pc = memory[pc+1]-1;
            }else{
                pc++;
            }
            break;

         case 52:                                                    //JE regaddress
            if (flags == 1){
                pc = memory[*registers[memory[pc+1]]]-1;
            }else{
                pc++;
            }
            break;


        case 53:                                                    //JNE reg
            if (flags == 0){
                pc = *registers[memory[pc+1]]-1;
            }else{
                pc++;
            }
            break;

        case 54:                                                    //JNE address
            if (flags == 0){
                pc = memory[pc+1]-1;
            }else{
                pc++;
            }
            break;

         case 55:                                                    //JNE regaddress
            if (flags == 0){
                pc = memory[*registers[memory[pc+1]]]-1;
            }else{
                pc++;
            }
            break;

        case 56:                                                    //MOD reg
            *registers[0] %= *registers[memory[pc+1]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 57:                                                    //MOD address
            *registers[0] %= memory[memory[pc+1]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        case 58:                                                    //MOD regaddress
            *registers[0] %= memory[*registers[memory[pc+1]]];
            flags = (*registers[0] == 0);
            pc++;
            break;

        default:
            break;
    }
}
