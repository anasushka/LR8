#include "hehe.h"


busS* Bus=nullptr;
int countBus=0;
int N=0;
//функция для увелечения размера массива
void arrayBus() {
    int newN=(N==0)?2:N*2;  //увеличиваем емкость массива
    busS* newBus=(busS*)malloc(newN*sizeof(busS));
    for(int i=0;i<countBus;i++) {
        newBus[i]=Bus[i];
    }
    free(Bus);
    Bus = newBus;
    N = newN;
}
//добавление нового автобусика
void addBus(const busS& newBus) {
    if(countBus==N) {
        arrayBus();
    }
    Bus[countBus++]=newBus;
}
//ввод с клавы всего
busS inputBus() {
    busS newBus;
    std::cout << "Введите номер автобуса : ";
    check_n(newBus.number);
    std::cin.ignore();

    std::cout << "Введите тип автобуса : ";
    std::cin.getline(newBus.type, sizeof(newBus.type));


    std::cout << "Введите место назначения :";
    std::cin.getline(newBus.place, sizeof(newBus.place));

    std::cout << "Введите время отправления(оставьте пустым для автозаполнения если вам все равно): ";
    check_time(newBus.timetogo);
    //std::cin.getline(newBus.timetogo, sizeof(newBus.timetogo));
    //автозаполнение
    if(strlen(newBus.timetogo)==0) {
        strcpy(newBus.timetogo, "06:00");
    }
    std::cout << "Введите время прибытия : ";

    check_time(newBus.timetocome);
    if(strlen(newBus.timetocome)==0) {
        strcpy(newBus.timetocome, "18:00");
    }
    return newBus;
}
//фиксированное количество с клавы
void input_k_Buses(int k) {
    for(int i = 0; i < k ; i++) {
        addBus(inputBus());
    }
}
//ввод до появления заданного
void inputwhileplace(const char* TOplace) {
    while(true) {
        busS newBus=inputBus();
        addBus(newBus);
        if (strcmp(newBus.place, TOplace) == 0) {
            break;
        }
    }
}
//ввод с диалогом о необходимости продолжить
void maybeonemoreBus() {
    char choice = 'y';
    while (choice == 'y' || choice == 'Y') {
        addBus(inputBus());
        std::cout << "Хотите продолжить ввод? (y/n): ";
        std::cin >> choice;
        std::cin.ignore();
    }
}
//список всех автобусиков
void allBuses() {
    if(countBus ==0) {
        std::cout << "Нету автобусов :'(";
        return;
    }
    for (int i=0;i<countBus;i++) {
        std::cout <<"\nНомер автобуса : "<< Bus[i].number << ".\n";
        std::cout <<"Тип : "<< Bus[i].type << "."<<std::endl;
        std::cout <<"Место назвачение : "<< Bus[i].place << ".\n";
        std::cout <<"Время отправления : "<< Bus[i].timetogo << ".\n";
        std::cout <<"Время прибытия : "<< Bus[i].timetocome << ".\n";
        std::cout <<"---------------------------------------------------------\n";
    }
}

void deleteBus(int number) {
    for(int i=0;i<countBus;i++) {
        if(Bus[i].number == number) {
            for(int j=i;j<countBus-1;j++) {
                Bus[j] = Bus[j + 1];
            }
            countBus--;
            std::cout << "Автобусик удален.\n";
            return;
        }
    }
    std::cout << "Автобус с таким номером не найден.\n";
}

int updateBus(int number) {
    for (int i = 0; i < countBus; i++) {
        if (Bus[i].number == number) {
            std::cin.ignore();
            std::cout << "Введите новый тип автобуса: ";
            std::cin.getline(Bus[i].type, sizeof(Bus[i].type));
            std::cout << "Введите новое место назначения: ";
            std::cin.getline(Bus[i].place, sizeof(Bus[i].place));
            std::cout << "Введите новое время отправления (HH:MM): ";
            check_time(Bus[i].timetogo);
            std::cout << "Введите новое время прибытия (HH:MM): ";
            check_time(Bus[i].timetocome);
            std::cout << "Расписание обновлено.\n";
            return i;
        }
    }
    std::cout << "Расписание с указанным номером автобуса не найдено.\n";
    return -1;
}

void selectionSortDescending(busS* searchBus, int countBus) {
    for (int i = 0; i < countBus - 1; ++i) {
        int maxIndex = i;
        for (int j = i + 1; j < countBus; ++j) {
            if (strcmp(searchBus[j].timetocome, searchBus[maxIndex].timetocome) > 0) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            busS temp = searchBus[i];
            searchBus[i] = searchBus[maxIndex];
            searchBus[maxIndex] = temp;
        }
    }
    for (int i = 0; i < countBus; ++i) {
        std::cout << "Номер автобуса: " << searchBus[i].number << ".\n";
        std::cout << "Тип: " << searchBus[i].type << ".\n";
        std::cout << "Место назначения: " << searchBus[i].place << ".\n";
        std::cout << "Время отправления: " << searchBus[i].timetogo << ".\n";
        std::cout << "Время прибытия: " << searchBus[i].timetocome << ".\n";
        std::cout << "--------------------------------------\n";
    }
    free(searchBus);
}
