#include <fstream>
#include "hehe.h"

void saveBusesToFile(const char* filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Ошибка открытия файла для записи." << std::endl;
        return;
    }
    for (int i = 0; i < countBus; ++i) {
        file.write(reinterpret_cast<const char*>(&Bus[i]), sizeof(busS));
    }
    file.close();
}

void loadBusesFromFile(const char* filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Ошибка открытия файла для чтения." << std::endl;
        return;
    }
    busS tempBus;
    while (file.read(reinterpret_cast<char*>(&tempBus), sizeof(busS))) {
        addBus(tempBus);
    }
    file.close();
}

void updateBusInFile(const char* filename, int busNumber, const busS& updatedBus) {
    std::fstream file(filename, std::ios::in | std::ios::out | std::ios::binary);
    if (!file) {
        std::cerr << "Ошибка открытия файла." << std::endl;
        return;
    }

    busS tempBus;
    bool found = false;
    while (file.read(reinterpret_cast<char*>(&tempBus), sizeof(busS))) {
        if (tempBus.number == busNumber) {
            found = true;
            file.seekp(-static_cast<int>(sizeof(busS)), std::ios::cur);
            file.write(reinterpret_cast<const char*>(&updatedBus), sizeof(busS));
            if (!file) {
                std::cerr << "Ошибка записи в файл." << std::endl;
            }
            break;
        }
    }

    if (!found) {
        std::cout << "Автобус с номером " << busNumber << " не найден." << std::endl;
    }

    file.close();
}
void clearBusesFile(const char* filename) {
    std::ofstream file(filename, std::ios::trunc); // Открыть файл в режиме trunc для очистки
    if (!file) {
        std::cerr << "Ошибка открытия файла для очистки." << std::endl;
        return;
    }
    std::cout << "Файл " << filename << " успешно очищен." << std::endl;
    file.close();
}

