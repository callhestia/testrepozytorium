// menu.cpp — interfejs konsolowy, Mikołaj
#include "menu.hpp"
#include "../engine/engine.hpp"
#include "../engine/Levenshtein.hpp"
#include "../storage/file_manager.hpp"
#include "../storage/Flashcard.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <algorithm>  // transform (do lowercase)
#include <cctype>     // tolower

using namespace std;

// -----------------------------------------------------------------------
// Funkcje pomocnicze UI
// -----------------------------------------------------------------------

void czyscEkran() {
    system("cls");
}

void rysujLinie() {
    cout << "\n------------------------------------------\n";
}

void czekajNaEnter() {
    cout << "\n[Nacisnij Enter, aby kontynuowac...]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Zwraca fiszki gotowe do nauki (nextReview <= teraz)
vector<Flashcard*> wybierzFiszkiDoNauki(vector<Flashcard>& fiszki) {
    vector<Flashcard*> doNauki;
    long teraz = static_cast<long>(time(nullptr));

    for (auto& f : fiszki) {
        if (f.nextReview <= teraz) {
            doNauki.push_back(&f);
        }
    }
    return doNauki;
}

// Zwraca status odpowiedzi: 0=idealnie, 1=literówka, 2=błąd
// Używa Levenshteina — spójna logika z silnikiem Franka
int obliczStatusOdpowiedzi(const string& odpowiedz, const Flashcard& f) {
    if (odpowiedz == f.answer)               return 0; // idealne dopasowanie
    if (sprawdzOdpowiedz(odpowiedz, f.answer)) return 1; // mieści się w progu tolerancji
    return 2;                                            // zbyt duży błąd
}

// -----------------------------------------------------------------------
// Tryb nauki: WPISYWANIE (StudyMode::AUTO)
// Użytkownik wpisuje odpowiedź, ocena automatyczna przez Levenshteina
// -----------------------------------------------------------------------

void trybWpisywania(vector<Flashcard*>& fiszki) {
    if (fiszki.empty()) {
        cout << "\nBrak fiszek do powtorki na dzis! Wróc jutro.\n";
        czekajNaEnter();
        return;
    }

    czyscEkran();
    cout << "=== TRYB WPISYWANIA ===\n";
    cout << "Fiszek do przejrzenia: " << fiszki.size() << "\n";
    cout << "Wpisuj odpowiedzi — algorytm sam oceni literowki.\n";
    czekajNaEnter();

    int poprawne = 0, literowki = 0, bledy = 0;

    for (auto* fp : fiszki) {
        czyscEkran();
        rysujLinie();
        cout << "PYTANIE: " << fp->question << "\n";
        rysujLinie();
        cout << "Twoja odpowiedz: ";

        string odpowiedz;
        getline(cin, odpowiedz);

        // Usuwamy zbędne spacje na początku i końcu
        while (!odpowiedz.empty() && odpowiedz.front() == ' ')
            odpowiedz.erase(odpowiedz.begin());
        while (!odpowiedz.empty() && odpowiedz.back() == ' ')
            odpowiedz.pop_back();

        // Sprawdzamy status przed aktualizacją fiszki
        int status = obliczStatusOdpowiedzi(odpowiedz, *fp);

        // Przekazujemy do silnika Franka — fiszka zostaje zaktualizowana
        *fp = krokNauki(*fp, StudyMode::AUTO, odpowiedz);

        rysujLinie();
        if (status == 0) {
            cout << "  [IDEALNIE!] Swietna robota.\n";
            poprawne++;
        } else if (status == 1) {
            cout << "  [PRAWIE!] Mala literowka — nie przejmuj sie.\n";
            cout << "  Poprawna odpowiedz: " << fp->answer << "\n";
            literowki++;
        } else {
            cout << "  [BLAD] Nastepnym razem bedzie lepiej.\n";
            cout << "  Poprawna odpowiedz: " << fp->answer << "\n";
            bledy++;
        }

        czekajNaEnter();
    }

    // Podsumowanie sesji
    czyscEkran();
    rysujLinie();
    cout << "  === KONIEC SESJI — WPISYWANIE ===\n\n";
    cout << "  Idealnie:  " << poprawne  << "\n";
    cout << "  Literowki: " << literowki << "\n";
    cout << "  Bledy:     " << bledy     << "\n";
    rysujLinie();
    czekajNaEnter();
}

// -----------------------------------------------------------------------
// Tryb nauki: SAMOOCENA (StudyMode::MANUAL)
// Użytkownik sam ocenia swoją odpowiedź w skali 0–5
// -----------------------------------------------------------------------

void trybSamooceny(vector<Flashcard*>& fiszki) {
    if (fiszki.empty()) {
        cout << "\nBrak fiszek do powtorki na dzis!\n";
        czekajNaEnter();
        return;
    }

    czyscEkran();
    cout << "=== TRYB SAMOOCENY ===\n";
    cout << "Fiszek do przejrzenia: " << fiszki.size() << "\n\n";
    cout << "Skala ocen:\n";
    cout << "  5 - Odpowiedzialem idealnie\n";
    cout << "  4 - Drobna literowka\n";
    cout << "  3 - Poprawna odpowiedz, ale z wahaniem\n";
    cout << "  2 - Zla odpowiedz, ale przypomnialo mi sie\n";
    cout << "  1 - Bardzo slaba odpowiedz\n";
    cout << "  0 - Zupelnie nie wiedzialem\n";
    czekajNaEnter();

    for (auto* fp : fiszki) {
        czyscEkran();
        rysujLinie();
        cout << "PYTANIE: " << fp->question << "\n";
        rysujLinie();
        cout << "[Nacisnij Enter zeby zobaczyc odpowiedz...]\n";
        cin.get();

        cout << "\nODPOWIEDZ: " << fp->answer << "\n";
        rysujLinie();

        // Pobieramy ocenę, walidujemy zakres 0–5
        int ocena = -1;
        while (ocena < 0 || ocena > 5) {
            cout << "Jak Ci poszlo? (0-5): ";
            cin >> ocena;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                ocena = -1;
                cout << "Wpisz liczbe od 0 do 5.\n";
            }
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        // Przekazujemy ocenę do silnika Franka
        *fp = krokNauki(*fp, StudyMode::MANUAL, "", ocena);

        // Informacja zwrotna dla użytkownika
        if (ocena >= 4) {
            cout << "  Swietnie!.\n";
        } else if (ocena == 3) {
            cout << "  Dobrze. Nastepna powtorka za " << fp->interval << " dni.\n";
        } else {
            cout << "  Ta fiszka wróci wkrótce.\n";
        }

        czekajNaEnter();
    }

    czyscEkran();
    cout << "\n  Sesja zakonczona! Fiszki zostana zapisane.\n";
    czekajNaEnter();
}

// -----------------------------------------------------------------------
// Podmenu: dodawanie nowej fiszki ręcznie z klawiatury
// -----------------------------------------------------------------------

void dodajFiszke(vector<Flashcard>& fiszki) {
    czyscEkran();
    cout << "=== DODAJ NOWA FISZKE ===\n\n";
    cout << "Uwaga: nie uzywaj znaku | w tresci!\n\n";

    Flashcard nowaFiszka;

    cout << "Pytanie: ";
    getline(cin, nowaFiszka.question);

    // Walidacja — pytanie nie moze byc puste
    if (nowaFiszka.question.empty()) {
        cout << "\nPytanie nie moze byc puste. Anulowano.\n";
        czekajNaEnter();
        return;
    }

    cout << "Odpowiedz: ";
    getline(cin, nowaFiszka.answer);

    if (nowaFiszka.answer.empty()) {
        cout << "\nOdpowiedz nie moze byc pusta. Anulowano.\n";
        czekajNaEnter();
        return;
    }

    // Wartości domyślne nowej fiszki (jak w specyfikacji projektu)
    nowaFiszka.nextReview = 0L;   // 0 = nowa, dostepna od razu
    nowaFiszka.difficulty = 2.5f; // domyslny wspolczynnik SM-2
    nowaFiszka.interval   = 1;
    nowaFiszka.streak     = 0;

    fiszki.push_back(nowaFiszka);
    saveDeck(fiszki);

    cout << "\n  Fiszka dodana i zapisana w deck.txt\n";
    czekajNaEnter();
}

// -----------------------------------------------------------------------
// Podmenu: ustawienia (zmiana trybu nauki)
// -----------------------------------------------------------------------

void podmenuUstawien(StudyMode& trybNauki) {
    czyscEkran();
    cout << "=== USTAWIENIA ===\n\n";
    cout << "Aktualny tryb: "
         << (trybNauki == StudyMode::MANUAL ? "Samoocena (0-5)" : "Wpisywanie")
         << "\n\n";
    cout << "1. Tryb samooceny  — sam oceniasz sie w skali 0-5\n";
    cout << "2. Tryb wpisywania — wpisujesz odpowiedz, program sprawdza literowki\n";
    cout << "0. Wróc do menu\n\n";
    cout << "Wybor: ";

    int wybor;
    cin >> wybor;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    if (wybor == 1) {
        trybNauki = StudyMode::MANUAL;
        saveConfig(0); // zapisujemy do config.txt przez modul Dawida
        cout << "\n  Tryb zmieniony na: Samoocena\n";
        czekajNaEnter();
    } else if (wybor == 2) {
        trybNauki = StudyMode::AUTO;
        saveConfig(1);
        cout << "\n  Tryb zmieniony na: Wpisywanie\n";
        czekajNaEnter();
    }
    // wybor == 0 lub inny: wracamy bez zmian
}

// -----------------------------------------------------------------------
// Statystyki talii
// -----------------------------------------------------------------------

void wyswietlStatystyki(const vector<Flashcard>& fiszki) {
    czyscEkran();
    cout << "=== STATYSTYKI TALII ===\n\n";

    if (fiszki.empty()) {
        cout << "  Talia jest pusta. Dodaj fiszki przez menu!\n";
        czekajNaEnter();
        return;
    }

    long teraz = static_cast<long>(time(nullptr));
    int doNauki = 0, nowe = 0, odlozone = 0;

    for (const auto& f : fiszki) {
        if (f.nextReview == 0L)         nowe++;
        else if (f.nextReview <= teraz) doNauki++;
        else                            odlozone++;
    }

    cout << "  Lacznie fiszek:             " << fiszki.size() << "\n";
    cout << "  Nowych (nie uczonych):      " << nowe          << "\n";
    cout << "  Do powtorki dzis:           " << doNauki       << "\n";
    cout << "  Odlozonych na pozniej:      " << odlozone      << "\n\n";

    // Najlepsze fiszki (najwyższy streak)
    if (!fiszki.empty()) {
        const Flashcard* best = &fiszki[0];
        for (const auto& f : fiszki) {
            if (f.streak > best->streak) best = &f;
        }
        if (best->streak > 0) {
            cout << "  Najlepsza passa: \""
                 << best->question << "\" ("
                 << best->streak << " z rzędu)\n";
        }
    }

    czekajNaEnter();
}

// -----------------------------------------------------------------------
// GŁÓWNA PĘTLA MENU — wywoływana z main.cpp
// -----------------------------------------------------------------------

void uruchomMenu(vector<Flashcard>& fiszki, StudyMode& trybNauki) {
    bool dziala = true;

    while (dziala) {
        czyscEkran();
        cout << "==========================================\n";
        cout << "               WKUWATOR\n";
        cout << "==========================================\n\n";

        // Pokaż ile fiszek czeka
        auto doNauki = wybierzFiszkiDoNauki(fiszki);
        if (!doNauki.empty()) {
            cout << "  *** " << doNauki.size()
                 << " fiszek czeka na powtorke! ***\n";
        } else {
            cout << "  Brak fiszek do powtorki — wróc jutro.\n";
        }

        cout << "\n  Tryb: "
             << (trybNauki == StudyMode::MANUAL ? "Samoocena" : "Wpisywanie")
             << "\n\n";

        cout << "  1. Ucz sie\n";
        cout << "  2. Dodaj fiszke\n";
        cout << "  3. Statystyki\n";
        cout << "  4. Ustawienia\n";
        cout << "  0. Wyjdz i zapisz\n\n";
        cout << "  Wybor: ";

        int wybor;
        cin >> wybor;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (wybor) {
            case 1:
                if (trybNauki == StudyMode::AUTO) trybWpisywania(doNauki);
                else                              trybSamooceny(doNauki);
                break;
            case 2:
                dodajFiszke(fiszki);
                break;
            case 3:
                wyswietlStatystyki(fiszki);
                break;
            case 4:
                podmenuUstawien(trybNauki);
                break;
            case 0:
                dziala = false;
                break;
            default:
                cout << "\n  Nieznana opcja. Wpisz 0-4.\n";
                czekajNaEnter();
        }
    }
    saveDeck(fiszki);
}