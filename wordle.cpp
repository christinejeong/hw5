#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here
void generateWords(std::string& current, int index, const std::string& floating, const std::string& original, const std::set<std::string>& dict, std::set<std::string>& results);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    std::set<std::string> results;
    std::string current = in;
    generateWords(current, 0, floating, in, dict, results);
    return results;
}

// Define any helper functions here

// recursively build all words that fits constraints
void generateWords(std::string& current, int index, const std::string& floating, const std::string& original, const std::set<std::string>& dict, std::set<std::string>& results) {
    // base case: fully built word
    if(index == current.length()) {
        // if floating is empty and current word is in dict
        if(floating.empty() && dict.find(current) != dict.end()) {
            // add current word to results
            results.insert(current);
        }
        return;
    }

    // recursive case
    // if position is already filled (not a dash), move to next index
    if(original[index] != '-') {
        generateWords(current, index+1, floating, original, dict, results);
        return;
    }
    // position is a blank -> try all possible letters
    // 1. try all 26 letters
    for(char ch = 'a'; ch <= 'z'; ++ch) {
        current[index] = ch;
        std::string newFloating = floating;

        // if ch is floating letter, remove from floating
        size_t pos = newFloating.find(ch);
        if(pos != std::string::npos) {
            newFloating.erase(pos,1);
            generateWords(current, index+1, newFloating, original, dict, results);
        }
        else {
            // if there are enough blanks to place the rest of floating letter
            int remainingDashes = 0;
            for(size_t i = index+1; i < current.length(); ++i) {
                if(original[i] == '-') {
                    remainingDashes++;
                }
            }
            if(remainingDashes >= static_cast<int>(newFloating.length())) {
                generateWords(current, index+1, newFloating, original, dict, results);
            }
        }
    }
}
