#include "hehe.h"

int main() {
    int choice;
    const char* filename="trains.txt";
    loadFromFile(filename);
    for(int i = 0; i < countTrain; i++) {
        sendtoElastic(train[i]);
    }
    do {
        std::cout << "\n0 Завершить программу.\n1 Добавить расписание поезда. \n2 Показать все расписания.\n3 Удалить поезд.\n4 Изменить поезд.\n5 Найти есть ли поезда, соответствующие условиям оргкомитета международной конференции .\n";
        check_n(choice);

        switch (choice) {
            case 1: {
                std::cout<< "\n1 Добавить фиксированное количество расписаний\n2 Диалоговое окно\n";
                int choice2;
                std::cin >> choice2;
                if (choice2 == 1) {
                    std::cout << "Введите количество : ";
                    int k;
                    check_n(k);
                    input_k_train(k);
                    clearElastic();
                    for(int i = 0; i < countTrain; i++) {
                        sendtoElastic(train[i]);
                    }
                    saveToFile(filename);
                }else if(choice2 == 2) {
                    choiceTrain();
                    clearElastic();
                    for(int i = 0; i < countTrain; i++) {
                        sendtoElastic(train[i]);
                    }
                    saveToFile(filename);
                }else {
                    std::cout << "Введите 1,2 или 3.";
                }
                break;
            }
            case 2: {
                allTrain();
                break;
            }
            case 3 : {
                 std::string timestr;
                 std::cout << "Введите время отправления поезда , который вы хотите удалить : ";
                 check_time_string(timestr);
                 deleteTrain(timestr);
                 deleteFromElastic(timestr);
                saveToFile(filename);
                 break;
             }
             case 4 : {
                 std::string timestr;
                 std::cout << "Введите время отправления автобуса, который вы хотите изменить : ";
                 check_time_string(timestr);
                 int index=updateBus(timestr);
                 if(index != -1) {
                     updateInElastic(index);
                     updateInFile(filename, index);
                 }
                 break;
             }
            case 5: {
                int m,k;
                std::string N, t;
                std::cout << "Введите какое количество мест вам нужно зарезервировать : ";
                check_n(m);
                std::cout << "Введите пункт назначения : ";
                std::cin >> N;
                std::cout << "Введите день недели(0-вс, 1-пн, 2-вт, 3-ср, 4-чт, 5-пт, 6-сб) : ";
                check_k(k);
                std::cout << "Введите время, не позднее которого должен отправляться поезд (HH:mm) : ";
                check_time_string(t);

                search_mNkt(m, N, k,t);
                if(searchCount < 0) {
                    std::cout << "Невозможно выполнить заказ. "<< std::endl;
                }
            }case 0: {
                clearElastic();
                break;
                }
            default: {
                std::cout << "Введите число подходящее.."<< std::endl;
                break;
            }
        }
    }while(choice != 0);
    delete[] train;
    return 0;
}