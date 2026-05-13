#ifndef FILE_MANAGER_HPP
#define FILE_MANAGER_HPP

#include <vector>
#include "Flashcard.hpp"

std::vector<Flashcard> loadDeck();
void saveDeck(const std::vector<Flashcard>& deck);

void saveConfig(int studyMode);
int loadConfig();

#endif
