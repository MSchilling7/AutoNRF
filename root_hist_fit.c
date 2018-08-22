#include <iostream>
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
#include <TMath.h>

using namespace std;
static double Dist(double* x,double* par)
{
  // double scale=par[0];
  // double mu=par[1];
  // double sigma=par[2];
  // double skew=par[3];

  // double temp=(x[0]-mu)/sigma;
  // double normaldist=TMath::Exp(-0.5*temp*temp)*1/TMath::Sqrt(2*TMath::Pi())*1/sigma;
  // double erf=0.5*(1+TMath::Erf(temp*skew*1/TMath::Sqrt(2)));
  // return scale*2/sigma*normaldist*erf;

	// double scale=par[0];
	// double mu=par[1];
	// double b=par[2];
	// double p=par[3];
	// double temp=pow(b,p)/TMath::Gamma(p)*pow(x[0]-mu,p-1)*TMath::Exp(-b*(x[0]-mu));
	// return temp;

	return (par[6]*TMath::LogNormal(x[0],par[2],par[1],par[3])+par[7]*TMath::GammaDist(x[0],par[4],par[1],par[5]))*par[0];
}




void do_stuf()
{

	string str;
	vector<double> Hist;
	double val;
	ifstream Input;

	Input.open("test.txt",std::ifstream::in);
	if(Input.is_open())
	{
		while(getline(Input,str))
		{
			stringstream buffer(str);
			buffer>>val;
			Hist.push_back(val);
		}
	}
	Input.close();
	TH1 *h = new TH1I("h1","h1",100,9,14);
	for(int i=0;i<Hist.size();i++)h->Fill(Hist[i]);
// for(int i=0;i<100;i++)
// {
// 	int bincontent=h->GetBinContent(i);
// 	int integral=h->Integral();
// 	double val_bin=bincontent/integral;
// 	h->SetBinContent(i,val_bin);

// }
		unsigned int maxbin=h->GetMaximumBin();
	double max_x=maxbin*h->GetBinWidth(maxbin)+9;
	double max=h->GetMaximum();


	TF1* ParameterFuncGaus= new TF1("ParameterGaus","[0]*1/(sqrt(2*TMath::Pi()*[2]**2))*exp(-0.5*((x-[1])/[2])**2)",9,14);
	TF1 *func= new TF1("func",Dist,9,14,8);
	func->FixParameter(0,1);
	func->FixParameter(1,9);
	func->SetParameter(2,1);
	func->SetParameter(3,1);
	func->SetParameter(4,1);
	func->SetParameter(5,9);
	func->SetParameter(6,1);
	func->SetParameter(7,1);
	// h->Fit("func","S");
	for(int i=1;i<8;i++)func->FixParameter(i,func->GetParameter(i));
	func->ReleaseParameter(0);
	// h->Fit("func","S");
	double *parameter =new double[8];
	for(int i =0;i<8;i++)parameter[i]=func->GetParameter(i);
	// h->Draw();
	// func->Draw("same");
	double mean=func->Moment(1,9,14,parameter,0.000001);
	double sigma=func->CentralMoment(1,9,14,parameter,0.000001);
	cout<<"mean: "<<mean<<endl;
	cout<<"sigma: "<<sigma<<endl;

	const Int_t nq = 100;
   	Double_t xq[nq];  // position where to compute the quantiles in [0,1]
   	Double_t yq[nq];  // array to contain the quantiles
   	for (Int_t i=0;i<nq;i++) xq[i] = Float_t(i+1)/nq;
   	h->GetQuantiles(nq,yq,xq);

   //show the original histogram in the top pad
   TCanvas *c1 = new TCanvas("c1","demo quantiles",10,10,700,900);
   c1->Divide(1,2);
   c1->cd(1);
   h->Draw();
   func->Draw("same");
   // show the quantiles in the bottom pad
   c1->cd(2);
   gPad->SetGrid();
   TGraph *gr = new TGraph(nq,xq,yq);
   gr->SetMarkerStyle(21);
   gr->Draw("alp");

  // You can choose among the following methods:
   // CSPLINE, LINEAR, POLYNOMIAL,
   // CSPLINE_PERIODIC, AKIMA, AKIMA_PERIODIC
   ROOT::Math::Interpolator inter(nq, ROOT::Math::Interpolation::kLINEAR);
   inter.SetData(nq,xq,yq);

   TH1I *intervall = new TH1I("Conficence Intervall","Conficence Intervall",100,xlow,xhi);
    for(int i=0;i<nq;i++)
    {
        if(i<lower_bound || lower_bound<=i)intervall->SetBinContent(i,0);
        if(i<=lower_bound<i+68 && i<nq)intervall->SetBinContent(i,h->GetBinContent(i));
    }
    intervall->SetFillColorAlpha(kRed,0);
    intervall->Draw();
}




void root_hist_fit()
{
	do_stuf();
}

