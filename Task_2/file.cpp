#include "hehe.h"

void saveToFile(const char* filename) {
    std::ofstream file(filename);
    if(!file) {
        std::cerr << "Ошибка при октрытие файла."<< std::endl;
    }
    for (int i=0; i < countTrain; ++i) {
        file << train[i].date<< " ";
        file << train[i].place << " ";
        file << train[i].timetogo<< " ";
        file << train[i].freeSeats << "\n";
    }
    file.close();
}

void loadFromFile(const char* filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Ошибка при октрытие файла."<< std::endl;
    }
    Train tempTrain;
    countTrain=0;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);//создаем поток чтобы разбить строку на части по пробелам
        if (!(iss >> tempTrain.date >> tempTrain.place >> tempTrain.timetogo >> tempTrain.freeSeats)) {
            std::cerr << "Ошибка чтения строки: " << line << "\n";
            continue; // Переходим к следующей строке в случае ошибки
        }
        tempTrain.dayoftheweek = getDayOfWeek(tempTrain.date);
        addTrain(tempTrain); // Добавляем поезд в массив
    }
    file.close();
}

void updateInFile(const char* filename, int index) {
    if (index < 0 || index >= countTrain) {
        std::cerr << "Некорректный индекс.\n";
        return;
    }
    saveToFile(filename);
}