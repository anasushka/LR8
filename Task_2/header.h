//
// Created by anastacia on 12/27/24.
//

#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <iostream>
#include <ctime>
#include <cstring>
#include <fstream>
#include <sstream>
#include "hehe.h"
#include <curl/curl.h>
#include <jsoncpp/json/json.h>

typedef struct traininfo {
    union{
        char date[11];//12.12.2006
        int dateValue;
    };
    int dayoftheweek;
    char place[100];
    char timetogo[6];//11:11
    int freeSeats;
}Train;

#endif //HEADER_H
