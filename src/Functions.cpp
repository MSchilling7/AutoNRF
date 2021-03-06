/**
 * @file Functions.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Efficiency class
 */

#include"Functions.h"


Functions::Functions()
{
  
};

// ---------------------------------------------------------
Functions::~Functions()
{
  
};

// ---------------------------------------------------------

double Functions::knoll(double* energy, double *par)
{
  int energy0=1000;
  return
  par[0]*
  exp(
    (
      par[1]*pow(log(energy[0]/energy0),0)+
      par[2]*pow(log(energy[0]/energy0),1)+
      par[3]*pow(log(energy[0]/energy0),2)+
      par[4]*pow(log(energy[0]/energy0),3)+
      par[5]*pow(log(energy[0]/energy0),4)
      )
    ) ;
};

// ---------------------------------------------------------

double Functions::a4(double* energy, double *par)
{
 return
 par[0]*(
  (
    par[1]/energy[0]+
    par[2]*energy[0])
  *exp(
    (
      par[3]/energy[0]+
      par[4]*energy[0]
      )
    )
  );
}; 

// ---------------------------------------------------------

double Functions::jaeckel(double* energy, double *par)
{  
  const double PI = TMath::Pi();
  return par[0]*(
    exp(
      (
        par[1]+
        par[2]*energy[0]+
        par[3]*pow(energy[0],2)
        )*
      2/PI*
      atan(
        exp(
          par[4]+
          par[5]*energy[0]+
          par[6]*pow(energy[0],3))
        )
      )
    -25);
}

// ---------------------------------------------------------

vector<unsigned int> Functions::Maching2Doubles(const vector< vector <double> >& a,const int anumber,const char aCorR,const vector<vector <double> >& b, const int bnumber,const char bCorR){
  vector<double> tempVector;
  vector<vector <double> > diff;
  vector<int> minindex;
  vector<int> jlist;
  double minvalue;
  vector<unsigned int> minlist;
  

  if(aCorR=='C' && bCorR=='C'){
    for(unsigned int i=0;i<a.size();i++){
      for(unsigned int j=0;j<b.size();j++){
        tempVector.push_back(fabs(a[i][anumber]-b[j][bnumber]));
        
      };
      diff.push_back(tempVector);
      tempVector.clear();
      
    }
  }
  
  if(aCorR=='R' && bCorR=='C'){
    for(unsigned int i=0;i<a[anumber].size();i++){
      for(unsigned int j=0;j<b.size();j++){
        tempVector.push_back(fabs(a[anumber][i]-b[j][bnumber]));
        
      };
      diff.push_back(tempVector);
      tempVector.clear();
      
    }
  }
  
  if(aCorR=='C' && bCorR=='R'){
    for(unsigned int i=0;i<a.size();i++){
      for(unsigned int j=0;j<b[bnumber].size();j++){
        tempVector.push_back(fabs(a[i][anumber]-b[bnumber][j]));
        
      };
      diff.push_back(tempVector);
      tempVector.clear();
      
    }
  }
  
  if(aCorR=='R' && bCorR=='R'){
    for(unsigned int i=0;i<a[anumber].size();i++){
      for(unsigned int j=0;j<b[bnumber].size();j++){
        tempVector.push_back(fabs(a[anumber][i]-b[bnumber][j]));
        
      };
      diff.push_back(tempVector);
      tempVector.clear();
      
    }
  }

  for(unsigned int i=0;i<diff.size();i++){
    minvalue = diff[i][0];
    minindex.push_back(0);
    for(unsigned int j=0;j<b.size();j++){
      if(minvalue>diff[i][j]){
        minvalue=diff[i][j];
        minindex[i] = j;
//                     cout<<minvalue<<"\t"<<minindex[i]<<"\t"<<j<<"\t"/*<<a[i][anumber]*/<<endl;
      }
      
    }
    minlist.push_back(minindex[i]);
  }; 
  return minlist;
}

// ---------------------------------------------------------

double Functions::Schiff(double *k,double *par)
{   
  k[0]=k[0]/1000;

    int CONST = 111; //Determines the width of the screened Coulomb potential. Value C = 111 adopted in publication above
    double mu=0.5109989; //Electron Mass in MeV*c^2.
//  par[0]=scale
//  par[1]=E0
//  par[2]=Z
    double b=2*par[1]*(par[1]-k[0])*sqrt((int) par[2])/(CONST*mu*k[0]);
    double M=1/(pow(mu*k[0]/(2*par[1]*(par[1]-k[0])),2)+pow(cbrt((int) par[2])/(CONST*(pow(0,2)+1)),2));
    
    double flux=par[0]*
    2*((int)par[2]*(int)par[2])/137*(
      (1.60218e-19)/(9.10938e-31)
      )
    *1/k[0]*(
      (
        ((par[1]*par[1])+((par[1]-k[0])*(par[1]-k[0])))/(par[1]*par[1])-2*(par[1]-k[0])/(3*par[1])
        )*
      TMath::Log(M)+1-2/b*TMath::ATan(b)+
      (par[1]-k[0])/par[1]*
      (
        2/(b*b)*TMath::Log(1+(b*b))+4*(2-(b*b))/(3*(b*b*b))*
        TMath::ATan(b)-8/(3*(b*b)+2/9)
        )*
      (par[1]-k[0])/par[1]*(2/b)
      )
    ;

    return flux;
  }

// ---------------------------------------------------------

double Functions::relError2(const vector <double>& a,const int posVal,const int posError)
  {
    double temp;
    temp= a[posError]/a[posVal];
    return temp*temp;
  }


// ---------------------------------------------------------    

unsigned int Functions::MinIndex(const double& val,const vector<vector<vector<double> > >& array)
{
  double diff=fabs(val-array[0][0][0]);
  unsigned int index=0;
  for(unsigned int i =0;i<array.size();++i)
  {
    if(diff>fabs(val-array[i][0][0]))
    {
      diff=fabs(val-array[i][0][0]);
      index=i;
    }
  }
  return index;
}

// ---------------------------------------------------------    

double Functions::SkewNormal(double* x,double* par)
{
  double scale=par[0];
  double mu=par[1];
  double sigma=par[2];
  double skew=par[3];

  double temp=(x[0]-mu)/sigma;
  double normaldist=TMath::Exp(-0.5*temp*temp)*1/TMath::Sqrt(2*TMath::Pi())*1/sigma;
  double erf=0.5*(1+TMath::Erf(temp*skew*1/TMath::Sqrt(2)));
  return scale*2/sigma*normaldist*erf;
}

// --------------------------------------------------------- 

double Functions::LogNormal(double *x,double*par)
{
  // https://en.wikipedia.org/wiki/Log-normal_distribution
  double scale=par[0];
  double sigma=par[1];
  double mu=par[2];
  double x0=par[3];

  double val=0;
  val=scale*TMath::LogNormal(x[0],sigma,mu,x0);
  return val;
}

// --------------------------------------------------------- 

double Functions::Normal2(double* x,double* par)
{
  double mu=par[0];
  double sigma=par[1];
  double sigma2=par[2];

  double gaus1 = TMath::Gaus(x[0],mu,sigma);
  double gaus2 = TMath::Gaus(x[0],mu,sigma2);

  double val=0;

  if(x[0]<mu)val=gaus1;
  if(x[0]>mu)val=gaus2;

  return val;


}

// --------------------------------------------------------- 

void Functions::ShortestCoverage(const vector<double>& param_array,double boundary[2])
{
  const unsigned int nq =1000; 
  Double_t xq[nq];  // position where to compute the quantiles in [0,1]
  Double_t yq[nq];  // array to contain the quantiles
  for (unsigned int i=0;i<nq;i++) xq[i] = double(i+1)/nq;
  
  double xlow=TMath::MinElement(param_array.size(),&param_array[0]);
  double xhi=TMath::MaxElement(param_array.size(),&param_array[0]);
  
  TH1D hist("","",nq,xlow,xhi);
  for(unsigned int i=0;i<param_array.size();i++)
  {
    hist.Fill(param_array[i]);
  }
  hist.GetQuantiles(nq,yq,xq);

  vector<vector<double>> temp;

  for(unsigned int i=0;i<nq;i++)
  {
    for(unsigned int j=0;j<i;j++)
   {
      if(xq[i]-xq[j]==0.683)
      {
        vector<double> sigma;
        sigma.push_back(xq[i]);
        sigma.push_back(yq[i]);
        sigma.push_back(xq[j]);
        sigma.push_back(yq[j]);
        temp.push_back(sigma);
        sigma.clear();
      }
    }
  }
  double dist=xhi-xlow;
  boundary[0]=xlow;
  boundary[1]=xhi;
  
  for(unsigned int i =0;i<temp.size();i++)
  {
    if(dist>temp[i][1]-temp[i][3])
    {
      dist=temp[i][1]-temp[i][3];
      boundary[0]=temp[i][3];
      boundary[1]=temp[i][1];
    }
  }
}

// --------------------------------------------------------- 
