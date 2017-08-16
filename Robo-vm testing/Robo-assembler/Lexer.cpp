//
//  Lexer.cpp
//  Robo-assembler
//
//  Created by Ben Wyngaard on 8/7/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include "Lexer.h"

strings Lexer::lex(std::string s){
    strings strList;
    char lexeme[256];
    int i=0,j=0;
    State state = START;
    int done = 0;
    int len = s.length();
    int balance = 0;
    while (i < len){
        switch (state) {
            case START:
                if (isSpace(s[i])){
                    state = SKIP;
                }else if (isGroup(s[i])){
                    if (s[i] == '"'){
                        lexeme[j] = s[i];
                        j++;
                        i++;
                    }
                    state = READBLOCK;
                }else if (s[i] == ';'){
                    i++;
                    state = COMMENT;
                }else{
                    state = READCHAR;
                }
                break;

            case READCHAR:
                if (isSpace(s[i])){
                    state = DUMP;
                }else if (isGroup(s[i])){
                    if (s[i] == '"'){
                        lexeme[j] = s[i];
                        j++;
                        i++;
                    }
                    state = READBLOCK;
                }else if(isSpecial(s[i])){
                    if (j == 0){
                        lexeme[j] = s[i];
                        j++;
                        i++;
                    }
                    state = DUMP;
                }else if (s[i] == ';'){
                    i++;
                    state = COMMENT;
                } else{
                    lexeme[j] = s[i];
                    i++;
                    j++;
                }
                break;

            case READBLOCK:
                if (s[i] == beginChar && s[i] != '"'){
                    balance++;
                    lexeme[j] = s[i];
                    i++;
                    j++;
                }else if (s[i] == endChar){
                    balance--;
                    lexeme[j] = s[i];
                    i++;
                    j++;
                    if (balance <= 0){
                        state = DUMP;
                    }
                }else{
                    lexeme[j] = s[i];
                    i++;
                    j++;
                }
                break;

            case SKIP:
                if(isSpace(s[i])){
                    i++;
                }else{
                    state = READCHAR;
                }
                break;

            case DUMP:
                if (j > 0){
                    lexeme[j] = 0;
                    strList.push_back(lexeme);
                    j = 0;
                }
                state = START;
                break;

            case COMMENT:
                if (s[i] != '\n'){
                    i++;
                }else{
                    state = READCHAR;
                }
                break;

            case END:
                i = len;
                break;
        }
    }
    if (j <= 0){
        lexeme[j] = 0;
        strList.push_back(lexeme);
    }
    return strList;
}

bool Lexer::isSpace(char c){
    switch (c) {
        case '\n':
            return true;
            break;
        case '\r':
            return true;
            break;
        case '\t':
            return true;
            break;
        case '\v':
            return true;
            break;
        case ' ':
            return true;
            break;
        case '\f':
            return true;
            break;
        default:
            return false;
    }
}

bool Lexer::isGroup(char c){
    beginChar = c;
    switch (c) {
        case '"':
            endChar = '"';
            return true;
            break;

        default:
            return false;
            break;
    }
}

bool Lexer::isSpecial(char c){
    switch (c) {
        case '[':
            return true;
        case ']':
            return true;
        case '(':
            return true;
        case ')':
            return true;
        case '{':
            return true;
        case '}':
            return true;
        case ',':
            return true;
        case '+':
            return true;
        case '-':
            return true;
        case ':':
            return true;

        default:
            return false;
    }
}
