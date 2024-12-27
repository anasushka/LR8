//
// Created by anastacia on 12/27/24.
//
#ifndef HEADER_H
#define HEADER_H
#include <string>
#include <iostream>
#include "hehe.h"
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <cstdlib>
#include <cstring>

typedef struct busS {
    int number;
    char type[100];
    char place[100];
    char timetogo[6];
    char timetocome[6];
    union {
        char alternativeType[100];
        char alternativePlace[100];
    };
}busS;

#endif //HEADER_H
