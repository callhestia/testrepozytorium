#include "Levenshtein.hpp"

#include <string>
#include <vector>    // vector to tablica, której rozmiar ustawiasz w trakcie działania programu
#include <algorithm> // min, max, swap

using namespace std;

int obliczOdleglosc(const string& s1, const string& s2) {
    int n = static_cast<int>(s1.length()); // liczba znaków w pierwszym słowie
    int m = static_cast<int>(s2.length()); // liczba znaków w drugim słowie

    // Przypadki brzegowe: jedno słowo puste
    if (n == 0) return m; // trzeba wstawić m znaków
    if (m == 0) return n; // trzeba usunąć n znaków

    // Tworzymy dwa wiersze – każdy ma m+1 komórek (indeksy 0..m).
    // vector<int>(m + 1, 0)  ==  int arr[m+1] = {0, 0, 0, ...}
    vector<int> prev(m + 1); // wiersz "poprzedni" (i-1)
    vector<int> curr(m + 1); // wiersz "bieżący"  (i)

    // Inicjalizacja pierwszego wiersza: odległość od pustego s1 do prefiksu s2
    // prev[j] = j  oznacza: żeby z "" dostać "abc..." (j znaków) trzeba j wstawień
    for (int j = 0; j <= m; ++j) prev[j] = j;

    // Wypełniamy wiersz po wierszu
    for (int i = 1; i <= n; ++i) {
        curr[0] = i; // odległość od s1[0..i] do pustego s2 = i usunięć

        for (int j = 1; j <= m; ++j) {
            // Czy aktualnie porównywane znaki są takie same?
            int cost = (s1[i - 1] == s2[j - 1]) ? 0 : 1;

            // Trzy możliwe operacje – wybieramy najtańszą:
            int deletion     = prev[j]     + 1;    // usuń znak z s1
            int insertion    = curr[j - 1] + 1;    // wstaw znak do s1
            int substitution = prev[j - 1] + cost; // zamień (lub zostaw gdy cost=0)

            curr[j] = min(deletion, min(insertion, substitution));
        }

        // Bieżący wiersz staje się poprzednim w następnej iteracji.
        // swap() zamienia zawartości obu wektorów w miejscu – szybciej niż kopiowanie.
        swap(prev, curr);
    }

    // Po ostatniej iteracji swap() przeniósł wynik do prev
    return prev[m];
}

// Sprawdza, czy różnica między wyrazami mieści się w progu dopuszczalności.
// prog = 0.3 oznacza: do 30% znaków może się różnić i odpowiedź jest OK.
bool czyBladDopuszczalny(const string& slowo1, const string& slowo2, float prog) {
    int maxDlugosc = static_cast<int>(max(slowo1.length(), slowo2.length()));

    if (maxDlugosc == 0) return true; // oba puste – brak błędu

    int   odleglosc         = obliczOdleglosc(slowo1, slowo2);
    float wspolczynnikBledu = static_cast<float>(odleglosc) / static_cast<float>(maxDlugosc);

    return wspolczynnikBledu <= prog;
}
