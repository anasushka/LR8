#include "hehe.h"

bool isTimeFormat(const std::string& timestr) {
    if (timestr.size()!=5 || timestr[2] !=':') {
        return false;
    }
    std::string hours = timestr.substr(0,2);
    std::string minutes = timestr.substr(3,2);

    if (!std::isdigit(hours[0])||!std::isdigit(hours[1])) {
        return false;
    }
    int hoursnum = std::stoi(hours);
    int minutesnum = std::stoi(minutes);

    if(hoursnum < 0 || hoursnum > 23 || minutesnum < 0 || minutesnum > 59) {
        return false;
    }
    return true;
}

void check_n(int& n) {
    while(true) {
        std::cin >> n;
        if (std::cin.fail() || std::cin.peek() != '\n') {
            std::cin.clear();
            std::cin.ignore(1000000000000, '\n');
            std::cout << "Введите число! " << "\n";
            continue;
        }
        break;
    }
}

void check_time(char time[6]) {
    while (true) {
        std::cin.getline(time, sizeof(time));
        std::string timestr=std::string(time);
        if(strlen(time)==0) {
            break;
        }else if(!isTimeFormat(timestr)) {
            std::cin.clear();
            std::cin.ignore(1000000000, '\n');
            std::cout << "Неверный фоормат. Попробуйте еще раз(HH:mm). \n";
            continue;
        }
        break;
    }
}
void check_time_string(std::string& time) {
    while (true) {
        std::cin >> time;
        if(!isTimeFormat(time)) {
            std::cin.clear();
            std::cin.ignore(1000000000000, '\n');
            std::cout << "Неверный фоормат. Попробуйте еще раз(HH:mm). \n";
            continue;
        }
        break;
    }
}