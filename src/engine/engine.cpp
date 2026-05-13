#include "engine.hpp"
#include "../models/flashcard.hpp"
#include "Levenshtein.hpp"
#include <algorithm>

using namespace std;

// Sprawdza odpowiedź użytkownika z tolerancją błędów (przekazuje do czyBladDopuszczalny)
bool sprawdzOdpowiedz(const string& odpowiedzUzytkownika, const string& poprawnaOdpowiedz,
                      float prog) {
    return czyBladDopuszczalny(odpowiedzUzytkownika, poprawnaOdpowiedz, prog);
}

// Aktualizuje fiszkę na podstawie oceny (0–5) i zwraca jej nową wersję
Flashcard aktualizujFiszke(const Flashcard& fiszka, int ocena) {
    SmResponse wynik = zaplanujPowtorke(ocena, fiszka.combo, fiszka.easeFactor);

    Flashcard zaktualizowana = fiszka;
    zaktualizowana.combo      = wynik.combo;
    zaktualizowana.easeFactor = wynik.easeFactor;

    return zaktualizowana;
}

// Przelicza błąd Levenshteina (0.0–1.0) na ocenę SM-2 (0–5)
static int przeliczBladNaOcene(const string& odpowiedzUzytkownika,
                                const string& poprawnaOdpowiedz) {
    int maxLen = static_cast<int>(max(odpowiedzUzytkownika.length(), poprawnaOdpowiedz.length()));
    if (maxLen == 0) return 5; // oba puste – traktujemy jako sukces

    int distance = obliczOdleglosc(odpowiedzUzytkownika, poprawnaOdpowiedz);
    double wspolczynnikBledu = static_cast<double>(distance) / static_cast<double>(maxLen);

    if (wspolczynnikBledu == 0.0)  return 5; // idealna odpowiedź
    if (wspolczynnikBledu <= 0.10) return 4; // drobna literówka
    if (wspolczynnikBledu <= 0.20) return 3; // kilka błędów
    if (wspolczynnikBledu <= 0.35) return 2; // częściowa odpowiedź
    if (wspolczynnikBledu <= 0.50) return 1; // bardzo niedokładna
    return 0;                                // całkowicie błędna
}

static int ograniczOcene(int ocena) {
    if (ocena < 0) return 0;
    if (ocena > 5) return 5;
    return ocena;
}

// Wykonuje jeden krok nauki: ocenia odpowiedź i zwraca zaktualizowaną fiszkę
Flashcard krokNauki(const Flashcard& fiszka, StudyMode tryb,
                    const string& odpowiedzUzytkownika, int ocena) {
    int rozwiazanaOcena = 0;

    switch (tryb) {
        case StudyMode::MANUAL:
            // Użytkownik dostarcza ocenę; przycinamy do zakresu 0–5
            rozwiazanaOcena = ograniczOcene(ocena);
            break;

        case StudyMode::AUTO:
            // Automatyczna ocena na podstawie Levenshteina
            rozwiazanaOcena = przeliczBladNaOcene(odpowiedzUzytkownika, fiszka.answer);
            break;
    }

    return aktualizujFiszke(fiszka, rozwiazanaOcena);
}

