/**
 * @file Flux.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Flux class
 */

#ifndef FLUX_H
#define FLUX_H 1

#include <vector>
#include <TCanvas.h>
#include <TMath.h>
#include <TMultiGraph.h>
#include <TRandom3.h>
#include <TF1.h>
#include <TGraphErrors.h>
#include <TFrame.h>
#include <TLegend.h>

#include <TCanvas.h>


#include "Functions.h"
#include "Efficiency.h"
#include "DataReader.h"

using std::vector;
using std::string;

#define EX 0
#define E_FINAL 1
#define E_GAMMA 2
#define DE_GAMMA 3
#define J_INITAL 4
#define J_FINAL 5
#define GAMMA 6
#define DGAMMA 7
#define BRANCHING 8
#define DBRANCHING 9
#define W90 10
#define W130 11

#define CORRECT_VOLUME 8
#define CORRECT_DVOLUME 9

#define YSCALE 1.1
#define COLOR_FLUX 1
#define COLOR_FIT 2
#define COLOR_DFIT 4
#define ENDPOINT 1.5


class Flux
{
public:
    /// Constructor for the Flux
    Flux();
    /// Destructor for the Flux
    ~Flux();
    
    /// Copy Constructor for Flux
    Flux(const Flux&):
    FileName(""),
    rfile(""),
    NFit(0),
    NumberOfThreads(0),
    Percent(),
    ExperimentalData(),
    calcEfficiency(),
    ICS(),
    FluxParameter(),
    PhotonFluxData(),
    FitParameterDistribution(),
    Parameter_Flux(),
    DetectorAngles(),
    ScaleParameter(),
    EndPointParameter(),
    Parameter_Efficiency()
    {;};
    void SetRootFile(string rootfile){rfile=rootfile;};
    void SetInputData(vector<vector<vector<double> > >Data){ExperimentalData=Data;};
    void SetFluxParameter(vector<vector<double> > Data){FluxParameter=Data;};
    void SetEfficiencyParameter(vector<vector<double> >EFit){Parameter_Efficiency=EFit;}
    void SetFileName(string Name){FileName=Name.substr(0,Name.length()-4);};
    void SetFitParameters(vector<double>par){Parameter_Flux=par;};
    void SetNThread(unsigned int t){NumberOfThreads=t;};
    void SetDetectorAngles(vector<double> Angles){DetectorAngles=Angles;};
    
    vector<vector<double> > GetPhotonFluxData(){return PhotonFluxData;};
    vector<vector<double> > GetICS(){return ICS;};
    vector<vector<vector<double> > >GetExperimentalData(){return ExperimentalData;};
    
    void CalculateEfficiencyforExperimentalData();
    void CorrectingPeakArea();
    void CalculateICS();
    void CalculateFlux();
    void FitPhotonFlux();
    void PlotPhotonFlux();
    void PlotFitParameters();
    
    
    
private:


    //! Storing the threads:
    vector<TThread*> Threads;
    //! Storing a flag that the thread is running
    vector<bool> ThreadIsInitialized;
    //! Storing a flag that the thread is finished
    vector<bool> ThreadIsFinished;
    static void CallParallelFluxFitThread(void* Adress);

    string FileName= "";
    string rfile = "";
    vector<double>GetParameterFlux(){return Parameter_Flux;}
    bool PhotonFluxFitter(unsigned int NThread);
    unsigned int NFit,NumberOfFits,NumberOfThreads;
    vector<unsigned int>Percent;
    vector<vector<vector<double> > >ExperimentalData;
    vector<vector<vector<double> > >calcEfficiency;
    vector<vector<double> > ICS;
    vector<vector<double> > FluxParameter;
    vector<vector<double> > PhotonFluxData;
    vector<vector<double> > FitParameterDistribution;
    vector<double> Parameter_Flux,DetectorAngles;
    vector<double> ScaleParameter,EndPointParameter;
    vector<vector<double> > Parameter_Efficiency;
    const double hqc= 197.3269788*1000;//keV fm
    

};
class ThreadCallerFlux
{
 public:
  //! Standard constructor
  ThreadCallerFlux(Flux* M, unsigned int ThreadID) {
    m_Fitter = M;
    m_ThreadID = ThreadID;
  }

  //! Return the calling class
  Flux* GetThreadCaller() { return m_Fitter; }
  //! Return the thread ID
  unsigned int GetThreadID() { return m_ThreadID; }

 private:
  //! Store the calling class for retrieval
  Flux* m_Fitter;
  //! ID of the worker thread
  unsigned int m_ThreadID;
};
#endif