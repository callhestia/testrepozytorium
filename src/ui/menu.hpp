#pragma once

#include "../models/flashcard.hpp"
#include <string>

using namespace std;

// Zwraca aktualny tryb nauki
StudyMode pobierzTrybNauki();

// Rotuje tryb: MANUAL -> AUTO -> MANUAL -> ...
void zmienTrybNauki();

// Zwraca czytelną nazwę podanego trybu ("MANUAL" lub "AUTO")
string nazwaTrybu(StudyMode tryb);
