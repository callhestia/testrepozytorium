#include <clocale>
#include <iostream>
#include <locale>
#ifdef _WIN32
#include <windows.h>
#endif
#include "ui/menu.hpp"
#include "storage/file_manager.hpp"

static void ustawLocalePolskie() {
    std::setlocale(LC_ALL, "");

    try {
        std::locale::global(std::locale("pl_PL.UTF-8"));
    } catch (...) {
        try {
            std::locale::global(std::locale(""));
        } catch (...) {
            // jeśli lokalizacja nie jest dostępna, kontynuuj z domyślną
        }
    }

    std::cout.imbue(std::locale());
    std::cin.imbue(std::locale());
    std::wcout.imbue(std::locale());
    std::wcin.imbue(std::locale());

#ifdef _WIN32
    SetConsoleCP(CP_UTF8);
    SetConsoleOutputCP(CP_UTF8);
#endif
}

int main() {
    ustawLocalePolskie();
    auto fiszki = loadDeck();
    int configMode = loadConfig();
    StudyMode tryb = configMode == 1 ? StudyMode::AUTO : StudyMode::MANUAL;

    uruchomMenu(fiszki, tryb);
    saveDeck(fiszki);

    std::cout << "Koniec programu. Deck zapisany do deck.txt\n";
    return 0;
}