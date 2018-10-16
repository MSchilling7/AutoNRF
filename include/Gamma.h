/**
 * @file Gamma.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Gamma class
 */

#ifndef Gamma_H
#define Gamma_H 1

#define YSCALE 1.1

#include <vector>
#include <TF1.h>
#include <TFile.h>
#include <TLine.h>
#include <TCanvas.h>
#include <TFrame.h>
#include <thread>
#include <chrono>

#include "Functions.h"
#include "Output.h"

using std::vector;
using std::endl;
using std::cout;
using std::thread;

class Gamma
{
public:
    /// Constructor for the Gamma
    Gamma();
    /// Destructor for the Gamma
    ~Gamma();
    
    /// Copy Constructor for Gamma
    Gamma(const Gamma&)

    {;};
    void SetEfficiencyParameter(const vector<vector<double> >& data){Parameter_Efficiency=data;};
    void SetFluxParameter(const vector<double>& data){Parameter_Flux=data;};
    void SetTargetMass(const vector<double>& data){Mass=data;};
    void SetExperimentalData(const vector<vector<vector<double> > >& data){ExperimentalData=data;};
    void SetDetectorAngles(const vector<double>& data){DetectorAngles=data;}
    void SetExperimentalDataAngular(const vector<vector<double> >data){ExperimentalDataAngular=data;}
    void SetFluxFitParameterDistribution(const vector<vector<double> >& data){FluxFitParameterDistribution=data;};
    void SetEfficiencyFitParameterDistribution(const vector<vector<double> >& data){EfficiencyFitParameterDistribution=data;};
    void SetNThread(const unsigned int t){NumberOfThreads=t;};
    void SetRootFile(const string& str){rfile=str;}
    void CalculateResults();

    vector<vector<double> > GetICS()const{return ICS;};
    vector<vector<double> > GetCalculatedFlux()const{return Flux;}
    vector<vector<vector<double> > > GetCalculatedEfficiency()const{return calcEfficiency;}

private:
    void CalculateFluxResults();
    void CalculateEfficiencyResults();
    void SortExperimentalData();
    void CalculateEnergyResults();
    void CalculateICSResults();
    void CalculateICSDist(unsigned int ID,unsigned int fits);
    double GimmeICS(double area,double mass,double flux,double efficiency,double W);
    
    thread CalcICSDistThread(unsigned int ID,unsigned int fits){return thread([=]{CalculateICSDist(ID,fits);});}


    string rfile;
    unsigned int NumberOfThreads;
    vector<double> Mass;
    vector<unsigned int> Percent;
    vector<double> DetectorAngles;
    vector<double> Parameter_Flux;
    vector<vector<double> > Parameter_Efficiency;
    vector<vector<double> > Results;
    vector<vector<double> > ICS;
    vector<vector<double> > ICSDist;
    vector<vector<double> > Energy;
    vector<vector<double> > Flux;
    vector<vector<double> > FluxFitParameterDistribution;
    vector<vector<double> > EfficiencyFitParameterDistribution;
    vector<vector<double> > ExperimentalDataAngular;

    vector<vector<vector<double> > > ExperimentalData;
    vector<vector<vector<double> > > ExperimentalDataSorted;
    vector<vector<vector<double> > > calcEfficiency;
    
};
#endif