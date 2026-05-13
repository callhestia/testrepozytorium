#pragma once

#include "../models/flashcard.hpp"
#include "../config.hpp"
#include <string>

bool sprawdzOdpowiedz(const std::string& odpowiedzUzytkownika, const std::string& poprawnaOdpowiedz,
                      float prog = LEVENSHTEIN_THRESHOLD);

Flashcard aktualizujFiszke(const Flashcard& fiszka, int ocena);

Flashcard krokNauki(const Flashcard& fiszka, StudyMode tryb,
                    const std::string& odpowiedzUzytkownika, int ocena = 0);

SmResponse zaplanujPowtorke(int ocena, int combo, double poprzedniWspolczynnik);