#pragma once

static const char* APP_NAME = "Wkuwator";

static const char* DECK_PATH = "decks/example.txt";

// Maksymalna dopuszczalna liczba fiszek w pamięci?
static const int MAX_CARDS = 1000;

// Domyślny próg błędu Levenshteina (0.0–1.0):
// 0.3 oznacza, że do 30% znaków może się różnić i odpowiedź nadal jest akceptowana
static const float LEVENSHTEIN_THRESHOLD = 0.3f;
