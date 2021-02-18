#pragma once

#include <ctime>
#include <string>

std::string getDateTimeString(){
    // to function getDateTime()
    time_t rawtime;
    struct tm * timeinfo;
    char buffer[80];

    time (&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer,sizeof(buffer),"%d/%m/%Y %H:%M:%S",timeinfo);
    
    std::string dateTimeString(buffer);

    return dateTimeString;
}