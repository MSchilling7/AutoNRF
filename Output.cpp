/**
 * @file Output.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Output class
 */

#include"Output.h"

Output::Output()
{
    
};

// ---------------------------------------------------------

Output::~Output()
{
    
};

// ---------------------------------------------------------
//!
//!
//!
void Output::SetDate()  
{
    time_t currentTime;
    struct tm *localTime;

    time( &currentTime );                   // Get the current time
    localTime = localtime( &currentTime );  // Convert the current time to the local time

    int Day    = localTime->tm_mday;
    int Month  = localTime->tm_mon + 1;
    int Year   = localTime->tm_year + 1900;
//     int Hour   = localTime->tm_hour;
//     int Min    = localTime->tm_min;
//     int Sec    = localTime->tm_sec;
    
    
    stringstream Today;        
    Today<<  Day  <<  "-"  <<  Month  <<  "-"  <<  Year;
    Date=Today.str();
}

// ---------------------------------------------------------

void Output::SetFileName(string Name)
{
    Name.erase(Name.end()-4,Name.end());
    Name.append(".log");
    FileName+=dir;
    FileName+=Name;
}

// ---------------------------------------------------------

void Output::WriteLog(vector<vector<double> >array){
    LogFile.open(FileName.c_str(),std::ofstream::app);
    for(unsigned int i=0;i<array.size();i++){
        for(unsigned int j=0;j<array[i].size();j++){
            LogFile  << std::setw(20)<<array[i][j];
        }
        LogFile<<std::endl;
    }
    LogFile.close();
    cout<<"Log was written to "<<FileName<<endl;
}

// ---------------------------------------------------------

void Output::SetPreDataString(string line){
    string str;
    LogFile.open(FileName.c_str(),std::ofstream::app);
    LogFile  <<  "//";
    for(unsigned int i =0;i<line.length();i++){
        if(line[i]==' ' || line[i]==',' || line[i]==';'){
            LogFile<<std::setw(20)<<line.substr(0,i);
            line.erase(line.begin(),line.begin()+i);
        }
        if(i==line.length()-1)LogFile<<std::setw(20)<<line;
    }
    
    
    LogFile<<std::endl;
    LogFile.close();
    
}

// ---------------------------------------------------------
string Output::dir = "";
