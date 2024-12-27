

#ifndef HEHE_H
#define HEHE_H
#include "header.h"

extern busS* Bus;
extern int countBus;
extern busS* searchBus;
extern int searchCount;
extern busS* onesearchBus;
extern int onesearchCount;

void arrayBus();
void addBus(const busS& newBus);
busS inputBus();
void input_k_Buses(int k);
void maybeonemoreBus();
void allBuses();
int updateBus(int number);
void deleteBus(int number);
void selectionSortDescending(busS* array, int size);
void inputwhileplace(const char* TOplace);

void saveBusesToFile(const char* filename);
void loadBusesFromFile(const char* filename);
void updateBusInFile(const char* filename, int busNumber, const busS& updatedBus);
void clearBusesFile(const char* filename);

void check_n(int& n);
bool isTimeFormat(const std::string& timestr);
void check_time(char time[6]);
void check_time_string(std::string& time);

curl_slist* heheaders();
bool cURL(const std::string& jsonStr, const std::string& Url, std::string& response); ;
void sendtoElastic(const busS& Bus);
void searchPlaceAndTime(const std::string& searchPlace, const std::string& searchTime);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
bool clearElastic();
void deleteFromElastic(int number);
void updateInElastic(int index) ;
void searchElastic(const std::string& searchxyina);

#endif //HEHE_H
