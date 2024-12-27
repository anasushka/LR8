

#ifndef HEHE_H
#define HEHE_H
#include "header.h"



extern Train* train;
extern int countTrain;
extern Train* searchTrain;
extern int searchCount;



int getDayOfWeek(const char* date);
void arrayTrain();
void addTrain(const Train& newBus);
Train inputTrain();
void input_k_train(int k);
void choiceTrain();
void allTrain();
int updateBus(const std::string& timestr);
void deleteTrain(const std::string& timestr);


void saveToFile(const char* filename);
void loadFromFile(const char* filename);
void updateInFile(const char* filename, int index);
void deleteFromFile(const char* filename, int index);

void check_date_string(std::string& date);

void check_n(int& n);
bool isDateFormat(const std::string& datestr);
void check_k(int& k);
bool isTimeFormat(const std::string& timestr);


void check_time_string(std::string& time);


curl_slist* heheaders();
bool cURL(const std::string& jsonStr, const std::string& Url, std::string& response); ;
void sendtoElastic(const Train& Bus);
void search_mNkt(int availableSeats, const std::string& placeTOgo,int DayOfTheWeek, const std::string& timeEarlier);
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

bool clearElastic();
void deleteFromElastic(const std::string& number);
void updateInElastic(int index) ;


#endif //HEHE_H
