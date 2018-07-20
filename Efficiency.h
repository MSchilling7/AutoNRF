/**
 * @file Efficiency.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Efficiency class
 */

#ifndef Efficiency_H
#define Efficiency_H 1

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <cmath>
#include <algorithm>
#include <vector>
#include <iomanip>  
#include <time.h>
#include <TROOT.h>
#include <TCanvas.h>
#include <TApplication.h>
#include <TMath.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TRandom3.h>
#include <TH1F.h>
#include <TGraphErrors.h>
#include <TFrame.h>
#include <TVirtualFitter.h>
#include <TMatrixDSym.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TLegend.h>
#include <TFile.h>

#include <thread> 
#include <mutex>  
#include <chrono>

using std::vector;
using std::string;
using std::ifstream;
using std::thread;

#include "Output.h"
#include "Functions.h"
#include "DataReader.h"

#define YSCALE 1.1
#define SIGMA 10
#define ERRORBARS true

#define COLOR_FIT 2
#define COLOR_SIM 15
#define COLOR_ECAL 1
#define COLOR_DFIT 4

class Efficiency
{
public:
    /// Constructor for the Efficiency
    Efficiency();
    /// Destructor for the Efficiency
    ~Efficiency();
    
    /// Copy Constructor for Efficiency
    Efficiency(const Efficiency&)
    {;};
    void SetRootFile(string rootfile){rfile=rootfile;};
    void SetECalTime(vector<double> Time){ECalTime=Time;};
    void SetDetectorAngles(vector<double> Angles){DetectorAngles=Angles;};
    void SetSourceDataArray(vector<vector<vector<double> > > SData){sData=SData;};
    void SetExperimentalDataArray(vector<vector<vector<double> > >  ECalData){ecalData=ECalData;};
    void SetSimulationDataArray(vector<vector<vector<double> > > SimData){simulationData=SimData;};
    void SetFitParameterVector(vector<double> par){Parameter=par;}
    void SetFitFunctionEnum(Functions::EFunc a){FitFunctionEnum=a;};
    void SetNThread(unsigned int t){NThread=t;};
    void SetFileName(string Name){FileName=Name.substr(0,Name.length()-4);};
    void CalculateEfficiency();
    void OrganizeData();
    void FitEfficiency();
    void PlotScaleDist();
    void PlotEFunc();
    
    vector<vector<double> >GetFittedParameters(){return Parameter_All;};
    vector<vector<vector<double> > >  GetEfficiencyDataArray(){return EfficiencyDataArray;};
private:

    //! Storing the threads:
    vector<TThread*> Threads;
    //! Storing a flag that the thread is running
    vector<bool> ThreadIsInitialized;
    //! Storing a flag that the thread is finished
    vector<bool> ThreadIsFinished;
    static void CallParallelEfficiencyFitThread(void* Adress);


    string rfile = "";
    TFile* RFile;
    unsigned int NDetector;
    bool EfficiencyFitter(unsigned int NThread);
    unsigned int NThread;
    TF1* GetEFunction(){return EFunction;}
    vector<vector<vector<double> > >GetSimulationData(){return simulationData;}
    unsigned int NumberofParameters, NFit;
    Functions::EFunc FitFunctionEnum;
    string Type;
    string FileName;
    TF1 *EFunction;
    TF1 *dupefficiencyfunc,*ddownefficiencyfunc;
    vector<double> Parameter;
    double FitParameterMean;
    double FitParameterSigma;
    vector<vector<double> >FitParameterDistribution,Parameter_All;
    vector<double> ECalTime,DetectorAngles;
    vector<vector<vector<double> > > sData;
    vector<vector<vector<double> > >  ecalData;
    vector<vector<vector<double> > >  simulationData;
    vector<vector<vector<double> > > EfficiencyDataArray;
    
};
class ThreadCallerEfficiency
{
 public:
  //! Standard constructor
  ThreadCallerEfficiency(Efficiency* M, unsigned int ThreadID) {
    m_Fitter = M;
    m_ThreadID = ThreadID;
  }

  //! Return the calling class
  Efficiency* GetThreadCaller() { return m_Fitter; }
  //! Return the thread ID
  unsigned int GetThreadID() { return m_ThreadID; }

 private:
  //! Store the calling class for retrieval
  Efficiency* m_Fitter;
  //! ID of the worker thread
  unsigned int m_ThreadID;
};
#endif