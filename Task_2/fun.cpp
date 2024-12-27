#include "hehe.h"

int getDayOfWeek(const char* date) {
    int day, month, year;
    sscanf(date, "%d.%d.%d", &day, &month, &year);

    std::tm time_in = {};
    time_in.tm_year = year - 1900;
    time_in.tm_mon = month - 1;
    time_in.tm_mday = day;

    std::mktime(&time_in);
    return time_in.tm_wday; // 0 = воскресенье, ..., 6 = суббота
}

Train* train=nullptr;
int countTrain=0;
int N=0;

//функция для увелечения размера массива
void arrayTrain() {
    int newN=(N==0)?2:N*2;  //???
    Train* newTrain= new Train[newN];
    for(int i=0;i<countTrain;i++) {
        newTrain[i]=train[i];
    }
    delete[] train;
    train = newTrain;
    N = newN;
}
//добавление нового  поезда
void addTrain(const Train& newTrain) {
    if(countTrain==N) {
        arrayTrain();
    }
    train[countTrain++]=newTrain;
}
//ввод с клавы всего
Train inputTrain() {
    Train newTrain;

    std::cin.ignore();
    std::cout << "Введите дату выезда(DD.MM.YYYY) : ";
    std::cin.getline(newTrain.date, sizeof(newTrain.date));

    newTrain.dayoftheweek = getDayOfWeek(newTrain.date);

    std::cout << "Введите пункт назначения :";
    std::cin.getline(newTrain.place, sizeof(newTrain.place));

    std::cout << "Введите время отправления(HH:MM) : ";
    std::cin.getline(newTrain.timetogo, sizeof(newTrain.timetogo));
    std::cout << "Число свободных место : ";
    std::cin >> newTrain.freeSeats;
    return newTrain;
}
//фиксированное количество с клавы
void input_k_train(int k) {
    for(int i = 0; i < k ; i++) {
        addTrain(inputTrain());
    }
}

//добавление поездов с выбором продолжать или нет
void choiceTrain() {
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        addTrain(inputTrain());

        std::cout << "Хотите продолжить ввод? (y/n): ";
        std::cin >> choice;
        std::cin.ignore();
    }
}
//список всех автобусиков
void allTrain() {
    if(countTrain ==0) {
        std::cout << "Нету поездов :'(";
        return;
    }
    for (int i=0;i<countTrain;i++) {

        std::cout <<"\nДата выезда : "<< train[i].date << "."<<std::endl;
        std::cout <<"Место назвачение : "<< train[i].place << ".\n";
        std::cout <<"Время отправления : "<< train[i].timetogo << ".\n";
        std::cout <<"Количество свободных мест : "<< train[i].freeSeats << ".\n";
        //std::cout << train[i].dayoftheweek << std::endl;
        std::cout <<"---------------------------------------------------------";
    }
}

void deleteTrain(const std::string& timestr) {
    for(int i=0;i<countTrain;i++) {
        if(std::string(train[i].timetogo) == timestr) {
            for(int j=i;j<countTrain-1;j++) {
                train[j] = train[j + 1];
            }
            countTrain--;
            std::cout << "Поезд удален.\n";
        }
    }
    std::cout << "Поезда с таким временем отправления не найден.\n";
}

int updateBus(const std::string& number) {
    std::cin.ignore(10000000,'\n');
    for (int i = 0; i < countTrain; i++) {
        if (std::string(train[i].timetogo) == number) {
            std::cout << "Введите новую дату выезда(DD.MM.YYYY) : ";
            std::cin.getline(train[i].date, sizeof(train[i].date));

            train[i].dayoftheweek = getDayOfWeek(train[i].date);

            std::cout << "Введите новый пункт назначения :";
            std::cin.getline(train[i].place, sizeof(train[i].place));

            std::string temp=train[i].timetogo;
            std::cout << "Введите новое время время отправления(HH:MM)(оставьте пустым если хотите оставить то что было) : ";
            std::cin.getline(train[i].timetogo, sizeof(train[i].timetogo));
            //автозаполнение
            if(strlen(train[i].timetogo)==0) {
                strcpy(train[i].timetogo, temp.c_str());
            }
            std::cout << "Число свободных место : ";
            std::cin >> train[i].freeSeats;
            return i;
        }
    }
    std::cout << "Расписание с указанным номером автобуса не найдено.\n";
    return -1;
}
