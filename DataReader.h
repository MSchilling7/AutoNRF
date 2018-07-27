/**
 * @file DataReader.h
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Efficiency class
 */

#ifndef DataReader_H
#define DataReader_H 1
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream> 
#include <cstring>
#include <vector>
#include <ctime>
#include <time.h>
#include <Rtypes.h>
#include <algorithm>
#include "Functions.h"

using std::vector;
using std::string;
using std::ifstream;
using std::sort;
class DataReader
{
public:
    /// Constructor for the DataReader
    DataReader();
    /// Destructor for the DataReader
    ~DataReader();
    
    /// Copy Constructor for DataReader
    DataReader(const DataReader&)
    {;};
    
    
    void SetInputName(string inputName){inputFileName=inputName;};
    void SetInputParameter();
    void SetSourceData();
    void SetECalData();
    void SetExperimentalData();
    void SetSimulationData();
    void SetFluxCalibrationData();
    void Print2DArray(vector<vector<double> > array);
    void Print3DArray(vector<vector<vector<double> > > array);
    void Print1DArray(vector<int> array);
    void Print1DArray(vector<double> array);
    void Print1DArray(vector<string> array);
    
    vector<vector<double> >GetFitParameterArray_Efficency()const{return fitParameterarray_Efficency;};
    vector<vector<double> >GetFitParameterArray_Flux()const{return fitParameterarray_Flux;};
    vector<double> GetDetectorAnglesArray()const{return DetectorAngles;};
    vector<double> GetECalTimeArray()const{return ETimes;};
    vector <vector <string> > GetDataFileArray()const{return DataFilearray;};
    
    vector<vector<double> > GetFluxCalibrationDataArray()const{return fluxcalibrationData;};
    vector<vector<vector<double> > > GetSourceDataArray()const{return sData;};
    vector<vector<vector<double> > > GetECalDataArray()const{return ecalData;};
    vector<vector<vector<double> > > GetSimulationDataArray()const{return simulationData;};
    vector<vector<vector<double> > > GetExperimentalDataArray()const{return experimentalData;};
    
    

private:
    
    unsigned int InputLines;
    
    const char* GetFileName()const{return inputFileName.c_str();};
    
    string line;
    string inputFileName;
    
    
    vector<vector<double> > fitParameterarray_Efficency,fitParameterarray_Flux;
    vector<double> DetectorAngles,ETimes;
    vector<string> DataFileline;
    vector <vector <string> > DataFilearray;
    
    
    vector<vector<double> > fluxcalibrationData;
    vector<vector<vector<double> > > sData;
    vector<vector<vector<double> > > ecalData;
    vector<vector<vector<double> > > simulationData;
    vector<vector<vector<double> > > experimentalData;
};

// ------------------------------------------------------
#endif