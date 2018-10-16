/**
 * @file DataReader.h
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Efficiency class
 */

#ifndef DataReader_H
#define DataReader_H 1
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream> 
#include <vector>
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
    void SetCalibrationFluxData();
    void SetSimulationData();
    void SetCalibrationFluxParameter();
    void SetAngularDistribution();
    static void Print2DArray (const vector<vector<double> >& array);
    static void Print3DArray (const vector<vector<vector<double> > >& array);
    static void Print1DArray (const vector<int>& array);
    static void Print1DArray (const vector<double>& array);
    static void Print1DArray (const vector<string>& array);
    
    vector<vector<double> > GetFitParameterArray_Efficency()const{return fitParameterarray_Efficency;};
    vector<vector<double> > GetFitParameterArray_Flux()const{return fitParameterarray_Flux;};
    vector<vector<double> > GetExperimentalDataAngularArray()const{return experimentalDataAngular;};
    vector<double> GetDetectorAnglesArray()const{return DetectorAngles;};
    vector<double> GetECalTimeArray()const{return ETimes;};
    vector<double> GetTargetMassArray()const{return Mass;};
    vector <vector <string> > GetDataFileArray()const{return DataFilearray;};
    
    vector<vector<double> > GetCalibrationFluxParameterArray()const{return fluxcalibrationData;};
    vector<vector<vector<double> > > GetSourceDataArray()const{return sData;};
    vector<vector<vector<double> > > GetECalDataArray()const{return ecalData;};
    vector<vector<vector<double> > > GetSimulationDataArray()const{return simulationData;};
    vector<vector<vector<double> > > GetExperimentalDataArray()const{return experimentalData;};
    vector<vector<vector<double> > > GetCalibrationFluxDataArray()const{return calibrationfluxdata;};
    
    

private:
    
    const char* GetFileName()const{return inputFileName.c_str();};
    
    string line;
    string inputFileName;
    
    
    vector<vector<double> > fitParameterarray_Efficency,fitParameterarray_Flux,experimentalDataAngular;
    vector<double> DetectorAngles,ETimes,Mass;
    vector<string> DataFileline;
    vector <vector <string> > DataFilearray;
    
    
    vector<vector<double> > fluxcalibrationData;
    vector<vector<vector<double> > > sData;
    vector<vector<vector<double> > > ecalData;
    vector<vector<vector<double> > > simulationData;
    vector<vector<vector<double> > > experimentalData;
    vector<vector<vector<double> > > calibrationfluxdata;
};

// ------------------------------------------------------
#endif