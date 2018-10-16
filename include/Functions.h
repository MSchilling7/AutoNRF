/**
 * @file Functions.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Functions class
 */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H 1
#include <cmath>
#include <TMath.h>
#include <vector>
#include <TThread.h>
#include <TF1.h>
#include <TH1.h>
#include <TRandom3.h>
#include <TGraphErrors.h>

#include "Math/WrappedTF1.h"
#include "Math/WrappedMultiTF1.h"
#include "Fit/BinData.h"
#include "Fit/UnBinData.h"
#include "HFitInterface.h"
#include "Fit/Fitter.h"


using std::vector;

class Functions
{
public:
    /// Constructor for the Functions
    Functions();
    /// Destructor for the Functions
    ~Functions();
    
    /// Copy Constructor for Functions
    Functions(const Functions&)
    {;};

    static double knoll(double* energy, double *par);
    static double a4(double *energy, double *par);
    static double jaeckel(double *energy, double *par);
    static double Schiff(double *k,double *par);
    static double SkewNormal(double *x,double *par);
    static double LogNormal(double*x,double*par);
    static double Normal2(double*x,double*par);

    static void ShortestCoverage(const vector<double>& param_array,double boundary[2]);
    

    static vector<unsigned int> Maching2Doubles(const vector< vector <double> >& a,const int anumber,const char aCorR,const  vector<vector <double> >& b, const int bnumber,const char bCorR);
    static double relError2(const vector <double>& a,const int posVal,const int posError);
    static unsigned int MinIndex(const double& val,const vector<vector<vector<double> > >& array);
private:

};

// ---------------------------------------------------------

class FluxFitFunc : public ROOT::Math::IParamFunction { 
public:
  void SetParameters(const double* p) { std::copy(p,p+NPar(),fp);}
  const double* Parameters() const { return fp; }
  ROOT::Math::IGenFunction* Clone() const { 
    FluxFitFunc* f = new FluxFitFunc(); 
    f->SetParameters(fp);
    return f;
  };
  unsigned int NPar() const { return 3; }
private:
  double DoEvalPar(double k, const double* par) const
  { 
      k=k/1000;

    int CONST = 111; //Determines the width of the screened Coulomb potential. Value C = 111 adopted in publication above
    double mu=0.5109989; //Electron Mass in MeV*c^2.
//  par[0]=scale
//  par[1]=E0
//  par[2]=Z
    double b=2*par[1]*(par[1]-k)*sqrt((int) par[2])/(CONST*mu*k);
    double M=1/(pow(mu*k/(2*par[1]*(par[1]-k)),2)+pow(cbrt((int) par[2])/(CONST*(pow(0,2)+1)),2));
    
    double flux=par[0]*
    2*((int)par[2]*(int)par[2])/137*(
      (1.60218e-19)/(9.10938e-31)
      )
    *1/k*(
      (
        ((par[1]*par[1])+((par[1]-k)*(par[1]-k)))/(par[1]*par[1])-2*(par[1]-k)/(3*par[1])
        )*
      TMath::Log(M)+1-2/b*TMath::ATan(b)+
      (par[1]-k)/par[1]*
      (
        2/(b*b)*TMath::Log(1+(b*b))+4*(2-(b*b))/(3*(b*b*b))*
        TMath::ATan(b)-8/(3*(b*b)+2/9)
        )*
      (par[1]-k)/par[1]*(2/b)
      )
    ;

    return flux;
  }
  double fp[3];  
};

class EfficiencyFitFunc : public ROOT::Math::IParamFunction { 
public:
  void SetParameters(const double* p) { std::copy(p,p+NPar(),fp);}
  const double* Parameters() const { return fp; }
  ROOT::Math::IGenFunction* Clone() const { 
    EfficiencyFitFunc* e = new EfficiencyFitFunc(); 
    e->SetParameters(fp);
    return e;
  };
  unsigned int NPar() const { return 6; }
private:
  double DoEvalPar(double energy, const double* par) const
  { 
    double energy0=1000;
    return
    par[0]*
      exp(
      (
        par[1]*pow(log(energy/energy0),0)+
        par[2]*pow(log(energy/energy0),1)+
        par[3]*pow(log(energy/energy0),2)+
        par[4]*pow(log(energy/energy0),3)+
        par[5]*pow(log(energy/energy0),4)
      )
    ) 
    ;

  }
  double fp[6];  
};

#endif