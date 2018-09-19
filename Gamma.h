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

#include "Functions.h"
#include "DataReader.h"

using std::vector;
using std::endl;
using std::cout;

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
    void SetEfficiencyParameter(vector<vector<double> >data){Parameter_Efficiency=data;};
    void SetFluxParameter(vector<double> data){Parameter_Flux=data;};
    void SetTargetMass(vector<double> data){Mass=data;};
    void SetExperimentalData(vector<vector<vector<double> > > data){ExperimentalData=data;};
    void SetDetectorAngles(vector<double> data){DetectorAngles=data;}
    void SetExperimentalDataAngular(vector<vector<double> >data){ExperimentalDataAngular=data;}
    void SetFluxFitParameterDistribution(vector<vector<double> > data){FluxFitParameterDistribution=data;};
    void SetEfficiencyFitParameterDistribution(vector<vector<double> > data){EfficiencyFitParameterDistribution=data;};
    void SetRootFile(string str){rfile=str;}
    void CalculateResults();

    vector<vector<double> > GetResults(){return Results;};
    vector<vector<double> > GetCalculatedFlux(){return Flux;}
    vector<vector<vector<double> > > GetCalculatedEfficiency(){return calcEfficiency;}

private:
    void CalculateFluxResults();
    void CalculateEfficiencyResults();
    void SortExperimentalData();
    void CalculateEnergyResults();
    void CalculateICSResults();

    double GimmeICS(double area,double mass,double flux,double efficiency,double W);

    string rfile;
    vector<double> Mass;
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