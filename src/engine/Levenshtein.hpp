#pragma once

#include <string>

// Oblicza odległość Levenshteina między dwoma łańcuchami znaków
int obliczOdleglosc(const std::string& s1, const std::string& s2);

// prog: maksymalny dopuszczalny współczynnik błędu (0.0 – 1.0), domyślnie 0.3 (30%)
bool czyBladDopuszczalny(const std::string& slowo1, const std::string& slowo2, float prog = 0.3f);
