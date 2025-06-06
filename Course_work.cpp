#include "UI.h" // Подключаем заголовочный файл вашего UI
#include <cstdlib> 

int main() {
    std::system("chcp 1251");

    UI consoleUI;
    consoleUI.run();

    return 0;
}