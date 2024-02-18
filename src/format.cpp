#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string elapsedTime{};
    int h = seconds/3600;
    int m = (seconds - h*3600)/60;
    int s = seconds - h*3600 - m*60;

    if(h < 10) {
        elapsedTime += "0" + std::to_string(h);
    }
    else{
        elapsedTime += std::to_string(h);
    }
    elapsedTime += "::";
    if(m < 10) {
        elapsedTime += "0" + std::to_string(m);
    }
    else{
        elapsedTime += std::to_string(m);
    }
    elapsedTime += "::";
    if(s < 10) {
        elapsedTime += "0" + std::to_string(s);
    }
    else{
        elapsedTime += std::to_string(s);
    }
    return elapsedTime;
}