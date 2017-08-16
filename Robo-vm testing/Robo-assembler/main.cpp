//
//  main.cpp
//  Robo-assembler
//
//  Created by Ben Wyngaard on 8/6/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <map>
#include <math.h>
#include "Lexer.h"

typedef uint16_t i16;

int lexFile(const char* fileName, strings* s);
std::vector<i16> compile(strings s);
int getInstructionCode(strings s, i16 i); //returns -1 if not an instruction
i16 detectType1(strings s, i16 i);
i16 detectType2(strings s, i16 i);
i16 getInt(std::string s);
std::string outFileName = "out.bin";
std::map<std::string, i16> labels;
std::map<i16, std::string> labelRefs;
Lexer lexer;

int main(int argc, char* argv[]){
    if (argc < 2){
        std::cout << "USAGE: " << argv[0] << " <file>" << std::endl;
        return 0;
    }else if (argc == 3){
        outFileName = argv[2];
    }

    strings lexemes;
    if (lexFile(argv[1],&lexemes) == 0) return 0;


    for (i16 i = 0; i < lexemes.size(); i++){
        std::cout << lexemes[i] << std::endl;
    }

   std::vector<i16> instructions = compile(lexemes);
   for (int i = 0; i < instructions.size(); i++){
       //std::cout << i << ": " <<  instructions[i] << std::endl;
   }

   std::ofstream ofile;
   ofile.open(outFileName);
   for (i16 i = 0; i < instructions.size(); i++){
       ofile << (char)((instructions[i]&0xff00) >> 8);
       ofile << (char)(instructions[i]&0x00ff);   
   }
   ofile.close();
   return 0;
}

int lexFile(const char* fileName, strings* s){
    std::cout << "trying to open: " << fileName << std::endl;
    std::ifstream inFile;
    inFile.open(fileName);
    if (!inFile.is_open()){
        std::cout << "ERROR: Failed to open file " << fileName << std::endl;
        return -1;
    }
    std::string line;
    std::string contents;
    while(std::getline(inFile, line)){
        contents += line + '\n';
    }
    inFile.close();

    *s =  lexer.lex(contents);
    return 1;
}

std::vector<i16> compile(strings s){
    std::vector<i16> prog;
    int iCode;
    int expecting;
    for (i16 i = 0; i < s.size(); i++){iCode = getInstructionCode(s,i);
        if (iCode != -1){
            prog.push_back(iCode);
        }else if(s[i] == "("){
            i++;
            if (s[i] == "PASTE"){
                i++;
                strings toPaste;
                if (lexFile(s[i].substr(1,s[i].length()-2).c_str(),&toPaste) == 0){
                    std::cout << "could not link file " << s[i-1] << std::endl;
                    exit(0);
                }
                s.insert(s.end()-1,toPaste.begin(),toPaste.end());               
            } 
        }else{
            if (s[i][0] == 'e'){
                switch (s[i][1]) {
                    case 'a':
                        prog.push_back(0);
                        break;
                    case 'b':
                        prog.push_back(1);
                        break;
                    case 'c':
                        prog.push_back(2);
                        break;
                    case 'd':
                        prog.push_back(3);
                        break;
                    case 'p':
                        prog.push_back(4);
                        break;
                    case 's':
                        prog.push_back(5);
                        break;
                }
            }else if(s[i][0] == '#'){
                prog.push_back(getInt(s[i].substr(1)));
            }else if(s[i] == "["){
                i++;
                if (s[i][0] != '.'){
                    int t = 0;
                    int arithmetic = 1;
                    while (s[i] != "]"){
                        if (s[i][0] == 'e'){
                            switch (s[i][1]) {
                                case 'b':
                                    t = 1;
                                   break;
                                case 'c':
                                    t =2;
                                    break;
                                case 'd':
                                    t =3;
                                    break;
                                case 'p':
                                    t =4;
                                    break;
                                case 's':
                                    t =5;
                                    break;
                             }
                         }else if(s[i] == "+"){
                             printf("plus\n");
                             arithmetic = 1;
                         }else if(s[i] == "-"){
                             printf("minus\n");
                             arithmetic = -1;
                         }else{
                             t += getInt(s[i])*arithmetic;
                         }
                         i++;
                    }
                    prog.push_back(t);
                }else{
                    labelRefs[prog.size()] = s[i];
                    prog.push_back(0);
                }
            }else if(s[i] == ":"){
                labels[s[i-1]] = prog.size();
            }else if(s[i] == "DB"){
                i++;
                if (s[i][0] == '"'){
                    for (int j = 1; j < s[i].length()-1; j++){
                        prog.push_back((i16) s[i][j]);
                    }
                    prog.push_back(0);
                }else if (s[i] == "{"){
                    i++;
                    std::vector<i16> values;
                    while (s[i] != "}"){
                        if(s[i] != "," && s[i] != " ") values.push_back(getInt(s[i]));
                        i++;
                    }
                    prog.push_back(values.size());                    
                    prog.insert(prog.end(),values.begin(),values.end());
                }else if(s[i] == "["){
                    i++;
                    int size = getInt(s[i]);
                    prog.push_back(size);
                    for(int j = 0; j < size; j++){
                        prog.push_back(0);
                    }
                    i++;
                }else if(s[i][0] == '.'){
                    if (s[i+1] != ":"){
                        labelRefs[prog.size()] = s[i];
                        prog.push_back(0);
                    }
                }else{
                    prog.push_back(getInt(s[i]));   
                }
            }else if(s[i] == " "){
                continue;
            }else if(s[i][0] == '.'){
                if (s[i+1] != ":"){
                    labelRefs[prog.size()] = s[i];
                    prog.push_back(0);
                }
            }
        }
    }
    labels[".end"] = prog.size();
    for ( const auto &pair : labelRefs ) {
        std::cout << "replacing " << pair.second << " with " << labels[pair.second] << std::endl;
        prog[pair.first] = labels[pair.second];
    }

    for (int i = 0; i < s.size(); i++){
        //std::cout << s[i] << std::endl;
    }

    return prog;
}

int getInstructionCode(strings s, i16 i){
    int base;
    if(s[i] == "MOV"){
        base = 0;
        base += detectType2(s,i);
    }else if (s[i] == "ADD"){
        base = 7;
        base += detectType2(s,i);
    }else if (s[i] == "SUB"){
        base = 10;
        base += detectType2(s,i);
    }else if (s[i] == "INC"){
        base = 13;
    }else if (s[i] == "DEC"){
        base = 14;
    }else if (s[i] == "MUL"){
        base = 15;
        base += detectType1(s,i);
    }else if (s[i] == "DIV"){
        base = 18;
        base += detectType1(s,i);
    }else if (s[i] == "AND"){
        base = 21;
        base += detectType2(s,i);
    }else if (s[i] == "OR"){
        base = 24;
        base += detectType2(s,i);
    }else if (s[i] == "XOR"){
        base = 27;
    }else if (s[i] == "NOT"){
        base = 30;
    }else if (s[i] == "SHL"){
        base = 31;
        base += detectType2(s,i);
    }else if (s[i] == "SHR"){
        base = 34;
        base += detectType2(s,i);
    }else if (s[i] == "CMP"){
        base = 37;
        base += detectType2(s,i);
    }else if (s[i] == "JMP"){
        base = 40;
    }else if (s[i] == "CALL"){
        base = 41;
        base += detectType1(s,i);
    }else if (s[i] == "RET"){
        base = 44;
    }else if (s[i] == "PUSH"){
        base = 45;
        base += detectType2(s,i);
        std::cout << std::endl; // for some reason it doesnt work without this
    }else if (s[i] == "POP"){
        base = 48;
    }else if (s[i] == "HLT"){
        base = 49;
    }else if (s[i] == "JE"){
        base = 50;
        base += detectType1(s,i);
    }else if (s[i] == "JNE"){
        base = 53;
        base += detectType1(s,i);
    }else if(s[i] == "MOD"){
        base = 56;
        base += detectType1(s,i);
    }else{
        return -1;
    }
    return base;
}

i16 detectType1(strings s, i16 i){
    if (s[i+1][0] == 'e'){
        return 0;
    }else if(s[i+1] == "[" || s[i+1][0] == '.'){
        if (s[i+2][0] == 'e'){
            return 2;
        }
        return 1;
    }else if(labels.find(s[i]) != labels.end()){
        return 1;
    }
}

i16 detectType2(strings s, i16 i){
    //std::cout << s[i+1][0] << std::endl;
    if (s[i] != "MOV" || (s[i+1][0] == 'e' && s[i+2][0] != '#' && s[i+2][0] != '.')){
        return detectType1(s,i+1);
    }else if(s[i+2][0] == '#' || s[i+2][0] == '.'){
        return 3;
    }else if (s[i+1][0] == '.'){
        return 4;
    }else{
        if (s[i+1] == "["){
            while(s[i] != "]"){
                if (s[i][0] == 'e'){
                    return 6;
                }
                i++;
            }
        }
        if (s[i+1][0] == 'e'){
            return 4;
        }else{
            return 5;
        }
    }
}

i16 getInt(std::string s){
    return std::stoi(s,nullptr,0);
}
