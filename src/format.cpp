#include <string>

#include "format.h"
#include <math.h>
using std::string;

// helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {     
    int hour;
    int minute;
    int second;
    hour = floor(seconds/3600);
    minute = floor(seconds%3600/60);
    second = seconds%3600%60;

   return PadTime(std::to_string(hour),'0')+":"+
   PadTime(std::to_string(minute),'0')+":"+
   PadTime(std::to_string(second),'0');
}

std::string Format::PadTime(std::string str, char ch){
    str.insert(str.begin(), 2-str.size(), ch);
    return str; 
}