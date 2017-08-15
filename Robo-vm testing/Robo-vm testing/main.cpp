//
//  main.cpp
//  Robo-vm testing
//
//  Created by Ben Wyngaard on 8/2/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include "VM.h"

std::vector<i16> loadDiskFromFile(const char* fileName){
    std::vector<i16> out;
    std::ifstream inFile(fileName);

    char singleCharacter;

    while (inFile.get(singleCharacter))
    {
        out.push_back(((i16) singleCharacter)&0x00ff);
    }

    if (inFile.eof()){
        return out;
    }else{
        std::cout << "ERROR" << std::endl;
    }
}

int main(int argc, const char * argv[]) {
    if (argc != 2){
        std::cout << "USAGE: robo-boot <disk_name>" << std::endl;
        return 0;
    }
    VM vm;
    vm.loadDisk(loadDiskFromFile(argv[1]));
    vm.run();
    vm.close();
}
