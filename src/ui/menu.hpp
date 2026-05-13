// menu.hpp
#pragma once
#include "../storage/Flashcard.hpp"
#include <vector>

// Główna pętla interfejsu — wywołaj z main.cpp
void uruchomMenu(std::vector<Flashcard>& fiszki, StudyMode& trybNauki);