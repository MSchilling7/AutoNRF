/**
 * @file Efficiency.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Efficiency class
 */

#ifndef Efficiency_H
#define Efficiency_H 1

#include <vector>
#include <numeric>
#include <functional> 
#include <TCanvas.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TRandom3.h>
#include <TH1.h>
#include <TGraphErrors.h>
#include <TLegend.h>
#include <TFile.h>
#include <TLine.h>
#include <TFrame.h>

using std::vector;
using std::string;

#include "Output.h"
#include "Functions.h"
#include "DataReader.h"

#define YSCALE 1.1
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
    Efficiency(const Efficiency&):
    rfile(""),
    FileName(""),
    NumberOfThreads(0),
    NumberofParameters(0),
    NFit(0),
    FitParameterMean(0),
    FitParameterSigma(0),
    Percent(),
    Parameter(),
    ECalTime(),
    DetectorAngles(),
    FitParameterDistribution(),
    Parameter_All(),
    sData(),
    ecalData(),
    simulationData(),
    EfficiencyDataArray()
    {;};
    void SetRootFile(const string& rootfile){rfile=rootfile;};
    void SetECalTime(const vector<double>& Time){ECalTime=Time;};
    void SetDetectorAngles(const vector<double>& Angles){DetectorAngles=Angles;};
    void SetSourceDataArray(const vector<vector<vector<double> > >& SData){sData=SData;};
    void SetExperimentalDataArray(const vector<vector<vector<double> > >&  ECalData){ecalData=ECalData;};
    void SetSimulationDataArray(const vector<vector<vector<double> > >& SimData){simulationData=SimData;};
    void SetFitParameterVector(const vector<double>& par){Parameter=par;}
    void SetNThread(unsigned int t){NumberOfThreads=t;};
    void SetFileName(const string& Name){FileName=Name.substr(0,Name.length()-4);};
    void CalculateEfficiency();
    void FitEfficiency();
    void PlotScaleDist();
    void PlotEFunc();
    
    vector<vector<double> >GetFittedParameters()const{return Parameter_All;};
    vector<vector<vector<double> > >  GetEfficiencyDataArray()const{return EfficiencyDataArray;};
    vector<vector<double> > GetFitParameterDistribution()const{return FitParameterDistribution;};
private:

    //! Storing the threads:
    vector<TThread*> Threads;
    //! Storing a flag that the thread is running
    vector<bool> ThreadIsInitialized;
    //! Storing a flag that the thread is finished
    vector<bool> ThreadIsFinished;
    static void CallParallelEfficiencyFitThread(void* Adress);

    bool EfficiencyFitter(unsigned int NThread);
    double ScaleVal(const std::vector<double>& x,const std::vector<double>& y){double mean=std::inner_product(x.begin(), x.end(), y.begin(), 0.0,std::plus<double>(),std::divides<double>()); mean=mean/((double)x.size()); return mean;}
    

    string rfile;
    string FileName;
    unsigned int NumberOfThreads;
    unsigned int NumberofParameters;
    unsigned int NFit;
    double FitParameterMean;
    double FitParameterSigma;
    vector<unsigned int> Percent;
    vector<double> Parameter;
    vector<double> ECalTime;
    vector<double>DetectorAngles;
    vector<vector<double> >FitParameterDistribution;
    vector<vector<double> >Parameter_All;
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