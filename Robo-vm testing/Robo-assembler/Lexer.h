//
//  Lexer.h
//  Robo-assembler
//
//  Created by Ben Wyngaard on 8/7/17.
//  Copyright © 2017 Ben Wyngaard. All rights reserved.
//

#include <vector>
#include <string>

#ifndef Lexer_h
#define Lexer_h

typedef uint8_t byte;
typedef std::vector<std::string> strings;

enum State{
    START,
    READCHAR,
    READBLOCK,
    SKIP,
    DUMP,
    COMMENT,
    END
};

class Lexer{
    bool isSpace(char c);
    bool isSpecial(char c);
    bool isGroup(char c);
    char endChar, beginChar;
public:
    strings lex(std::string s);
};

#endif /* Lexer_h */
