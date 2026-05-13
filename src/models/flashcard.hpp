#pragma once
#include <string>

enum class StudyMode { MANUAL, AUTO };

struct SmResponse {
    int combo;
    double easeFactor;
};

struct Flashcard {
    int id;
    std::string question;
    std::string answer;
    int combo;
    double easeFactor;
};