#include <bits/stdc++.h>
#include "file_manager.hpp"
#include "Flashcard.hpp"

using namespace std;

vector<Flashcard> loadDeck(){
    vector<Flashcard> deck;
    ifstream file("deck.txt");
    if(!file.is_open()){
        ofstream newFile("deck.txt");
        return deck;
    }
    string line;
    while(getline(file, line)){
        if(line.empty()) continue;
        stringstream ss(line);
        string item;
        Flashcard card;
        try{
            getline(ss,card.question, '|');
            getline(ss,card.answer,'|');
            getline(ss,item,'|'); card.difficulty = stof(item);
            deck.push_back(card);
        }
        catch(...){
            continue;
        }
    }
    file.close();
    return deck;
}

void saveDeck(const vector<Flashcard>& deck){
    ofstream file("deck.txt");
    if(!file.is_open()) return;
    for(const auto& card : deck){
        file<<card.question<<"|"<<card.answer<<"|"<<card.difficulty<<endl;
    }
    file.close();
}

int loadConfig(){
    ifstream file("config.txt");
    int mode = 0;
    if(file.is_open()){
        file>>mode;
    }
    else{
        saveConfig(0);
    }
return mode;
}

void saveConfig(int mode){
    ofstream file("config.txt");
    if(file.is_open()){
        file<<mode;
    }
}

int main()
{
    cout << "=== TEST MODULU ZARZADZANIA DANYMI ===" << endl;
    vector<Flashcard> talia = loadDeck();
    cout << "Wczytano fiszek z pliku: " << talia.size() << endl;
    if (talia.empty()) {
        cout << "[2] Plik byl pusty lub nie istnial. Tworze testowa fiszke..." << endl;
        Flashcard f;
        f.question = "Stolica Francji";
        f.answer = "Paryz";
        f.difficulty = 2.5;
        talia.push_back(f);
        saveDeck(talia);
        cout << "Zapisano testowa fiszke do deck.txt" << endl;
    } else {
        cout << "Pierwsza fiszka w talii to: " << talia[0].question << endl;
    }
    int tryb = loadConfig();
    cout << "Aktualny tryb nauki (z config.txt): " << tryb << endl;
    cout << "=======================================" << endl;
    cout << "Jesli widzisz te napisy, Twoj modul DZIALA!" << endl;
    return 0;
}
