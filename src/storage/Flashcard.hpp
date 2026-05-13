#ifndef FLASHCARD_HPP
#define FLASHCARD_HPP

#include <string>

struct Flashcard {
    std::string question;
    std::string answer;
    long nextReview;
    float difficulty;
    int interval;
    int streak;
};

#endif
