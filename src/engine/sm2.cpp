#include "../models/flashcard.hpp"
#include "engine.hpp"

using namespace std;

// Z sygnatury zniknął 'poprzedniInterwal'
SmResponse zaplanujPowtorke(int ocena, int combo, double poprzedniWspolczynnik) {
    double wspolczynnik;

    if (ocena > 3) {
        // Odpowiedź poprawna
        combo++;
        wspolczynnik = poprzedniWspolczynnik + (0.1 - (5 - ocena) * (0.08 + (5 - ocena) * 0.02));
    } else {
        // Odpowiedź błędna – resetujemy serię
        combo = 0;
        wspolczynnik = poprzedniWspolczynnik;
    }

    // Współczynnik łatwości nie może spaść poniżej 1.3
    if (wspolczynnik < 1.3) {
        wspolczynnik = 1.3;
    }

    // Zwracamy paczkę bez interwału!
    return {combo, wspolczynnik}; 
}