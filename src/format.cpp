#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string result = "00:00:00";
    long output = 0;
    if(seconds<=0)
        return result;
    output = seconds / 3600;
    result = TimeValueToString(output) + ":";

    output = (seconds/60)%60;
    result += TimeValueToString(output) + ":";

    output = seconds%60;
    result += TimeValueToString(output);

    return result;
}


string Format::TimeValueToString(long time)
{
    if(time < 10)
        return "0" + std::to_string(time);
    else
        return std::to_string(time);
}