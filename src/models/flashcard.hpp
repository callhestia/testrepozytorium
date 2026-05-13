#pragma once
#include <string>

enum class StudyMode { MANUAL, AUTO };

struct SmResponse {
    int combo;
    double easeFactor;
};

struct Flashcard {
    int id = 0;
    std::string question;
    std::string answer;
    int combo = 0;
    double easeFactor = 2.5;
    long nextReview = 0;
    float difficulty = 2.5f;
    int interval = 1;
    int streak = 0;
};