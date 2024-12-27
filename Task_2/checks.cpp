#include "hehe.h"



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
void check_k(int& n) {
    while(true) {
        std::cin >> n;
        if (std::cin.fail() || std::cin.peek() != '\n') {
            std::cin.clear();
            std::cin.ignore(1000000000000, '\n');
            std::cout << "Введите число! " << "\n";
            continue;
        }if(n > 7 || n < 1) {
            std::cin.clear();
            std::cin.ignore(1000000000000, '\n');
            std::cout << "Введите число от 1 до 7 ! " << "\n";
            continue;
        }
        break;
    }
}

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
/*bool isDateFormat(const std::string& datestr) {
    if (datestr.size()!=10 || datestr[2] !='.'||datestr[5] !='.') {
        return false;
    }
    std::string day = datestr.substr(0, 2);
    std::string month = datestr.substr(3, 2);
    std::string year = datestr.substr(6, 4);

    if (!std::isdigit(day[0]) || !std::isdigit(day[1]) || !std::isdigit(month[0]) || !std::isdigit(month[1]) ||!std::isdigit(year[0]) || !std::isdigit(year[1]) ||!std::isdigit(year[2]) || !std::isdigit(year[3])) {
        return false;
          }
    int daynum = std::stoi(day);
    int monthnum = std::stoi(month);


    if (monthnum < 1 || monthnum > 12) {
        return false;
    }
    if (daynum < 1 || daynum > 31) {
        return false;
    }
    if ((monthnum == 4 || monthnum == 6 || monthnum == 9 || monthnum == 11) && daynum > 30) {
        return false;
    }
    if((monthnum==2) && daynum > 28) {
        return false;
    }

    return true;
}*/
/*void check_date_string(std::string& date) {
    while (true) {
        std::cin >> date;
        if(!isDateFormat(date)) {
            std::cin.clear();
            std::cin.ignore(1000000000000, '\n');
            std::cout << "Неверный фоормат. Попробуйте еще раз(DD-MM-YYYY). \n";
            continue;
        }
        break;
    }
}*/