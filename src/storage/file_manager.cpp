#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <locale>
#include "file_manager.hpp"

using namespace std;

vector<Flashcard> loadDeck(){
    vector<Flashcard> deck;
    std::locale loc;
    try {
        loc = std::locale("");
    } catch (...) {
        loc = std::locale::classic();
    }

    ifstream file;
    file.imbue(loc);
    file.open("deck.txt");
    if(!file.is_open()){
        ofstream newFile("deck.txt");
        newFile.imbue(loc);
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
    std::locale loc;
    try {
        loc = std::locale("");
    } catch (...) {
        loc = std::locale::classic();
    }

    ofstream file;
    file.imbue(loc);
    file.open("deck.txt");
    if(!file.is_open()) return;
    for(const auto& card : deck){
        file<<card.question<<"|"<<card.answer<<"|"<<card.difficulty<<std::endl;
    }
    file.close();
}

int loadConfig(){
    std::locale loc;
    try {
        loc = std::locale("");
    } catch (...) {
        loc = std::locale::classic();
    }

    ifstream file;
    file.imbue(loc);
    file.open("config.txt");
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
    std::locale loc;
    try {
        loc = std::locale("");
    } catch (...) {
        loc = std::locale::classic();
    }

    ofstream file;
    file.imbue(loc);
    file.open("config.txt");
    if(file.is_open()){
        file<<mode;
    }
}
