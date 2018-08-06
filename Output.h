/**
 * @file Output.h
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief Header file for the Output class
 */
#ifndef Output_H
#define Output_H 1
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream> 
#include <vector>

using std::string;
using std::stringstream;
using std::vector;
using std::ofstream;
using std::cout;
using std::endl;

class Output
{
public:
    /// Constructor for the Output
    Output();
    /// Destructor for the Output
    ~Output();
    
    /// Copy Constructor for Output
    Output(const Output&)
    {;};
    void FileChecker(){if(std::ifstream(FileName))std::remove(FileName.c_str());};
    void SetDate();
    void SetFileName(string Name);
    void SetPreDataString(string line);
//     string FilePathPDF(string FitName, string FileName);
    void WriteLog(vector<vector<double> >array);
    
    string GetDate(){return Date;};
    
private:
    string Date;
    string FileName;
    string FitName;
    ofstream LogFile;

};

// ------------------------------------------------------

/*        
        
        
    string merge_strings(string a, string b){
        stringstream temp;
        temp<<a<<b;
        return temp.str();
    }

    string merge_strings(string a, string b, string c){
        stringstream temp;
        temp<<a<<b<<c;
        return temp.str();
    }
    
    
    string merge_strings(string a, string b, string c, string d, string e){
        stringstream temp;
        temp<<a<<b<<c<<d<<e;
        return temp.str();
    }*/

#endif