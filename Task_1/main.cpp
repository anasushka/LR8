#include "hehe.h"

int main() {
int choice;
    const char* filename="buses.dat";
    loadBusesFromFile(filename);
    for(int i = 0; i < countBus; i++) {
        sendtoElastic(Bus[i]);
    }
    do {
        std::cout << "\n0 Завершить программу.\n1 Добавить расписание. \n2 Показать все расписания.\n3 Удалить автобус.\n4 Изменить автобус.\n5 Найти элемент.\n6 Найти и отсортировать по времени и месту.\n";

        check_n(choice);

        switch (choice) {
            case 1: {
                std::cout<< "\n1 Добавить фиксированное количество расписаний\n2 Вводить расписания пока не появится структура с заданным местом назначения\n3 Диалоговое окно\n";
                int choice2;
                check_n(choice2);
                if (choice2 == 1) {
                    std::cout << "Введите количество : ";
                    int k;
                    check_n(k);
                    input_k_Buses(k);

                    clearElastic();
                    for(int i = 0; i < countBus; i++) {
                        sendtoElastic(Bus[i]);
                    }
                    saveBusesToFile(filename);

                }else if(choice2== 2) {
                    char TOplace[100];
                    std::cout << "Введите место назначения для остановки ввода : ";
                    std::cin.ignore();
                    std::cin.getline(TOplace, sizeof(TOplace));
                    inputwhileplace(TOplace);
                    clearElastic();
                    for(int i = 0; i < countBus; i++) {
                        sendtoElastic(Bus[i]);
                    }
                    saveBusesToFile(filename);
                }else if(choice2 == 3) {
                    maybeonemoreBus();
                    clearElastic();
                    for(int i = 0; i < countBus; i++) {
                        sendtoElastic(Bus[i]);
                    }
                    saveBusesToFile(filename);
                }else {
                    std::cout << "Введите 1,2 или 3.";
                }

                break;
            }
            case 2: {
                allBuses();
                break;
            }
            case 3 : {
                int number;
                std::cout << "Введите номер автобуса, который вы хотите удалить : ";
                check_n(number);
                deleteBus(number);
                deleteFromElastic(number);
                saveBusesToFile(filename);
                break;
            }
            case 4 : {
                int number;
                std::cout << "Введите номер автобуса, который вы хотите изменить : ";
                check_n(number);
                int index=updateBus(number);
                if(index != -1) {
                    updateInElastic(index);
                    updateBusInFile(filename, index, Bus[index]);
                }

                break;
            }
            case 5 : {
                std::string searchxyina;
                std::cout << "Введите значение, чтобы найти элемент : ";
                std::cin >> searchxyina;
                searchElastic(searchxyina);
                break;
            }
            case 6: {
                std::string searchPlace, searchTime;
                std::cout << "Введите пункт назначения : ";
                std::cin >> searchPlace;
                std::cout << "Введите время, раньше которого должен прибыть автобус : ";
                check_time_string(searchTime);

                searchPlaceAndTime(searchPlace, searchTime);
                if(searchCount > 0) {
                    selectionSortDescending(searchBus, searchCount);
                }else {
                    std::cout << "Нету подходящих автобусов:("<< std::endl;
                }
                break;
            }
            case 7 : {
                clearBusesFile(filename);
            }
            case 0: {
                clearElastic();
                break;
            }
            default: {
                std::cout << "Введите число подходящее.."<< std::endl;
                break;
            }
        }
    }while(choice != 0);
free(Bus);
return 0;
}