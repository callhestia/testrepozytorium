#include "menu.hpp"

// Wewnętrzny stan trybu nauki – domyślnie MANUAL
static StudyMode aktualnyTryb = StudyMode::MANUAL;

StudyMode pobierzTrybNauki() {
    return aktualnyTryb;
}

void zmienTrybNauki() {
    switch (aktualnyTryb) {
        case StudyMode::MANUAL: aktualnyTryb = StudyMode::AUTO;   break;
        case StudyMode::AUTO:   aktualnyTryb = StudyMode::MANUAL; break;
    }
}

string nazwaTrybu(StudyMode tryb) {
    switch (tryb) {
        case StudyMode::MANUAL: return "MANUAL";
        case StudyMode::AUTO:   return "AUTO";
    }
    return "NIEZNANY";
}
