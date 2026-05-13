#include <iostream>
#include <string>
#include "engine/Levenshtein.hpp" // zakladamy, ze zostawiamy tu funkcje obliczOdleglosc

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        return 1; 
    }

    string poprawna = argv[1];
    string wpisana = argv[2];

    int dystans = obliczOdleglosc(poprawna, wpisana);
    
    cout << dystans << endl;

    return 0;
}