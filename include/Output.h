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
#include <sstream>
#include <iterator>  

using std::istringstream;
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
    void WriteLog(vector<vector<double> >array);
    void WriteLogT(vector<vector<double> >array);
    void WriteLog(vector<double>array);
    
    string GetDate()const{return Date;};
    static string dir;
    
private:
    string Date;
    string FileName;
    string FitName;
    ofstream LogFile;

};
#endif