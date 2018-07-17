/**
 * @file ReadData.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Efficiency class
 */

#include"Efficiency.h"

using std::cout;
using std::setw;
using std::endl;
using std::istringstream;
using std::getline;


Efficiency::Efficiency()
{

};

// ---------------------------------------------------------
Efficiency::~Efficiency()
{

};

// ---------------------------------------------------------

void Efficiency::CalculateEfficiency()
{
    DataReader read;
    Functions func;
    vector<int>sortinglist;
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    for(unsigned int t=0;t<ECalTime.size();t++)
    {
        sortinglist=func.Maching2Doubles(ecalData[t],0,'C',sData[t],0,'C');
        
        for(unsigned int i=0;i<ecalData[t].size();i++)
        {
            tempVector.push_back(sData[t][sortinglist[i]][0]);
            tempVector.push_back(sData[t][sortinglist[i]][1]);
            tempVector.push_back(ecalData[t][i][4]/sData[t][sortinglist[i]][2]/ECalTime[t]);
            tempVector.push_back(ecalData[t][i][5]/sData[t][sortinglist[i]][2]/ECalTime[t]);
            temp2DVector.push_back(tempVector);
            tempVector.clear();
        }
//         read.Print2DArray(temp2DVector);
        EfficiencyDataArray.push_back(temp2DVector);
        temp2DVector.clear();
    }
};

// ---------------------------------------------------------

void Efficiency::OrganizeData()
{
    if(sData.size()==1)for(unsigned int i=0;i<ECalTime.size();i++)sData.push_back(sData[0]);
    FitParameterDistribution.resize(ECalTime.size());
    NFit=(unsigned int) Parameter[0];
    string str;
    str.append(std::to_string(SIGMA));
    str.append("-#sigma-range");
    if(FitFunctionEnum == Functions::EFunc::Knoll)
    {
        Type="Knoll";
        NumberofParameters = 6;
        EFunction = new TF1("EFunction",Functions::knoll, 0, 10000,NumberofParameters);
        if(ERRORBARS)dupefficiencyfunc=new TF1(str.c_str(),Functions::knoll, 0, 10000,NumberofParameters);
        if(ERRORBARS)ddownefficiencyfunc=new TF1(str.c_str(),Functions::knoll, 0, 10000,NumberofParameters);
    }
//     if(FitFunctionEnum == Functions::EFunc::a4)
//     {
//         Type="a4";
//         NumberofParameters = 5;
//         Fit = new TF1("Fit",Functions::a4, 0, 10000,NumberofParameters);
//         if(ERRORBARS)dupefficiencyfunc=new TF1(str.c_str(),Functions::a4, 0, 10000,NumberofParameters);
//         if(ERRORBARS)ddownefficiencyfunc=new TF1(str.c_str(),Functions::a4, 0, 10000,NumberofParameters);
//     }
//     if(FitFunctionEnum == Functions::EFunc::Jaeckel)
//     {
//         Type="Jäckel";
//         
//         NumberofParameters = 7;
//         Fit = new TF1("Fit",Functions::jaeckel, 0, 10000,NumberofParameters);
//         if(ERRORBARS)dupefficiencyfunc=new TF1(str.c_str(),Functions::jaeckel, 0, 10000,NumberofParameters);
//         if(ERRORBARS)ddownefficiencyfunc=new TF1(str.c_str(),Functions::jaeckel, 0, 10000,NumberofParameters);
//     }
    Parameter.erase(Parameter.begin());
    Parameter_All.resize(ECalTime.size());
    for(unsigned int i=0;i<Parameter_All.size();i++)Parameter_All[i].resize(NumberofParameters);
}

// ---------------------------------------------------------

void Efficiency::FitEfficiency()
{   
    cout<<"Fitting Detector Efficiency..."<<endl;
    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {
        cout<<"Fitting Efficiency Function for the Detector under "<<(int)DetectorAngles[t]<<" deg..."<<endl;
        EfficiencyFitter(NFit,t,NumberofParameters);

    }
    cout<<"Detector Efficiency fitted. Check log files!"<<endl;
}

// ---------------------------------------------------------


void Efficiency::EfficiencyFitter(unsigned int NFit_thread, unsigned int NDetector, unsigned int NPar)
{

    vector<vector<vector<double> > >Data;
    vector<vector<vector<double> > >sim;
    vector<double>Part;
    TF1* EFunction_Thread;
    vector<vector<double> >Par;
    EFunction_Thread=GetEFunction();
    sim=GetSimulationData();
    Data=GetEfficiencyDataArray();
    Par=GetFittedParameters();

    bool percent[20]={false};
    string equal="==========";
    string space="          ";


    for(unsigned int i = 0 ; i<NPar;i++){EFunction_Thread->ReleaseParameter(i);};
    for(unsigned int i = 0 ; i<NPar;i++)EFunction_Thread->SetParameter(i,Parameter[i]);
    int datapoints_sim=(int) sim[NDetector].size();
    Double_t xvec_sim[datapoints_sim];
    Double_t yvec_sim[datapoints_sim];
    for(unsigned int i = 0;i<sim[NDetector].size();i++)
    {
        xvec_sim[i]=sim[NDetector][i][0];
        yvec_sim[i]=sim[NDetector][i][1];
    }
    TGraphErrors *DataGraph;
    DataGraph= new TGraphErrors((const Int_t)datapoints_sim,xvec_sim,yvec_sim,0,0);
    EFunction_Thread->FixParameter(0,Parameter[0]);//Fixing Scale
    DataGraph->Fit(EFunction_Thread,"Q");
    for(unsigned int i=1;i<NPar;i++)
    {
        Parameter_All[NDetector][i]=EFunction_Thread->GetParameter(i);
        EFunction_Thread->FixParameter(i,Parameter_All[NDetector][i]);
    }
    delete DataGraph;
    int datapoints_ecal=(int)Data[NDetector].size();
    Double_t xvec_ecal[datapoints_ecal];
    Double_t yvec_ecal[datapoints_ecal];

    EFunction_Thread->ReleaseParameter(0);
    TRandom3* rand =new TRandom3(0);
    for(unsigned int i=0;i<NFit_thread;i++)
    {

        for(unsigned int j = 1;j<11;j++)
        {
            if(j*NFit_thread/10==i+1 && percent[j]==false)
            {
                cout<<" ["<<equal.substr(0,j)<<space.substr(j,10)<<"] "<< j <<"0%"<<endl;
                percent[j]=true;
            }
        }
        


        for(unsigned int j=0;j<Data[NDetector].size();j++)
        {
            xvec_ecal[j]=rand->Gaus(Data[NDetector][j][0],Data[NDetector][j][1]);
            yvec_ecal[j]=rand->Gaus(Data[NDetector][j][2],Data[NDetector][j][3]);
        } 
        DataGraph= new TGraphErrors((const Int_t)datapoints_ecal,xvec_ecal,yvec_ecal,0,0);
        DataGraph->Fit(EFunction,"Q");
        FitParameterDistribution[NDetector].push_back(EFunction->GetParameter(0));
        
        delete DataGraph;
    }
}
    

// ---------------------------------------------------------

    void Efficiency::PlotScaleDist()
    {
        for(unsigned int t=0;t<ECalTime.size();t++)
        {
            double xlow=TMath::MinElement(NFit,&FitParameterDistribution[t][0]);
            double xhi=TMath::MaxElement(NFit,&FitParameterDistribution[t][0]);

            int numberofdigits=10;
            xlow=xlow*numberofdigits;
            xlow=floor(xlow);
            xlow=xlow/numberofdigits;

            xhi=xhi*numberofdigits;
            xhi=ceil(xhi);
            xhi=xhi/numberofdigits;

            TCanvas *ParameterHist;
            ParameterHist = new TCanvas("Scaling Parameter Histogram","Scaling Parameter Distribution",1600,900);
            ParameterHist->SetGrid();
            ParameterHist->GetFrame()->SetFillColor(21);
            ParameterHist->GetFrame()->SetBorderSize(12);

            TH1I *ScalingParameter=new TH1I("Scaling Parameter", "Scaling Parameter Distribution",100,xlow,xhi);
            for(unsigned int i=0;i<FitParameterDistribution[t].size();i++)ScalingParameter->Fill(FitParameterDistribution[t][i]);
            TF1* ParameterFuncGaus= new TF1("Parameter Gaus","[0]*1/(sqrt(2*TMath::Pi()*[2]**2))*exp(-0.5*((x-[1])/[2])**2)",xlow,xhi);
            ParameterFuncGaus->FixParameter(1,ScalingParameter->GetMean(1));
            ParameterFuncGaus->FixParameter(2,ScalingParameter->GetStdDev(1));
            ScalingParameter->Fit(ParameterFuncGaus,"Q");
            FitParameterMean=ScalingParameter->GetMean(1);
            FitParameterSigma=ScalingParameter->GetStdDev(1);


            Output out;
            out.SetDate();
            ScalingParameter->Draw("same");
            ParameterFuncGaus->Draw("same");
            string str="ParameterPlot_Scale";
            str.append(FileName);
            str.append("_");
            str.append(std::to_string((int)DetectorAngles[t]));
            str.append("_deg_");
            str.append(out.GetDate());
            str.append(".pdf");
            str="Output/"+str;
            ParameterHist->SaveAs(str.c_str());
            cout<<"Distribution of Scale saved. ( "<<str<<" )"<<endl;

            Parameter_All[t][0]=FitParameterMean;
            Parameter_All[t].push_back(FitParameterSigma);
            delete ParameterHist;
            delete ScalingParameter;
        }

    }
// ---------------------------------------------------------

    void Efficiency::PlotEFunc()
    {
        for(unsigned int t=0;t<ECalTime.size();t++)
        {

            TGraphErrors *efficiencydata,*efficiencysim;
            int datapoints_ecal=(int) EfficiencyDataArray[t].size();
            int datapoints_sim=(int) simulationData[t].size();
            Double_t xvec_ecal[datapoints_ecal];
            Double_t dxvec_ecal[datapoints_ecal];
            Double_t yvec_ecal[datapoints_ecal];
            Double_t dyvec_ecal[datapoints_ecal];
            Double_t xvec_sim[datapoints_sim];
            Double_t yvec_sim[datapoints_sim];

            for(unsigned int i = 0;i<EfficiencyDataArray[t].size();i++)
            {
                xvec_ecal[i]=EfficiencyDataArray[t][i][0];
                dxvec_ecal[i]=EfficiencyDataArray[t][i][1];
                yvec_ecal[i]=EfficiencyDataArray[t][i][2];
                dyvec_ecal[i]=EfficiencyDataArray[t][i][3];
            }


            for(unsigned int i = 0;i<simulationData[t].size();i++)
            {
                xvec_sim[i]=simulationData[t][i][0];
                yvec_sim[i]=simulationData[t][i][1];
            }
            Double_t scale=TMath::MaxElement(datapoints_ecal,yvec_ecal)/TMath::MaxElement(datapoints_sim,yvec_sim);

            for(unsigned int i = 0;i<simulationData[t].size();i++)yvec_sim[i]=yvec_sim[i]*scale;

            TCanvas *efficiencyplot;
                efficiencyplot = new TCanvas("efficiencyplot","Efficiency Data Plot",1600,900);
                efficiencyplot->SetGrid();
                efficiencyplot->GetFrame()->SetFillColor(21);
                efficiencyplot->GetFrame()->SetBorderSize(12);

            efficiencydata = new TGraphErrors(datapoints_ecal,xvec_ecal,yvec_ecal,dxvec_ecal,dyvec_ecal);
            efficiencydata->SetMarkerStyle(20);
            efficiencydata->SetMarkerColor(COLOR_ECAL);
            efficiencydata->SetMarkerSize(1);
            efficiencydata->SetName("Efficiency Data");
            efficiencydata->SetFillStyle(0);
            efficiencydata->SetMaximum(YSCALE*TMath::MaxElement(datapoints_ecal,yvec_ecal));
            efficiencydata->SetMinimum(0);
        efficiencydata->GetYaxis()->SetNdivisions(505);   //Sets number of Ticks
        efficiencydata->GetYaxis()->SetDecimals(kTRUE); //Sets same digits
        
        efficiencysim = new TGraphErrors(datapoints_sim,xvec_sim,yvec_sim,0,0);
        efficiencysim->SetMarkerStyle(20);
        efficiencysim->SetMarkerColor(COLOR_SIM);
        efficiencysim->SetMarkerSize(1);
        efficiencysim->SetLineColor(COLOR_SIM);
        efficiencysim->SetName("Simulation");
        efficiencysim->SetFillStyle(0);
        efficiencysim->SetMaximum(YSCALE*TMath::MaxElement(datapoints_sim,yvec_sim));
        efficiencysim->SetMinimum(0);
        efficiencysim->GetYaxis()->SetNdivisions(505);   //Sets number of Ticks
        efficiencysim->GetYaxis()->SetDecimals(kTRUE); //Sets same digits

        EFunction->SetParameter(0,Parameter_All[t][0]);
        for(unsigned int i=2;i<NumberofParameters+1;i++){

            EFunction->SetParameter(i,Parameter_All[t][i]);
        }
        EFunction->SetLineWidth(1);
        EFunction->SetLineColor(COLOR_FIT);
        
        if(ERRORBARS)
        {
            dupefficiencyfunc->SetParameter(0,Parameter_All[t][0]+SIGMA*FitParameterSigma);
            ddownefficiencyfunc->SetParameter(0,Parameter_All[t][0]-SIGMA*FitParameterSigma);
            for(unsigned int i=1;i<NumberofParameters;i++)
            {
                dupefficiencyfunc->SetParameter(i,Parameter_All[t][i]);
                ddownefficiencyfunc->SetParameter(i,Parameter_All[t][i]);
            }
            dupefficiencyfunc->SetLineColor(COLOR_DFIT);
            dupefficiencyfunc->SetLineStyle(7);
            dupefficiencyfunc->SetLineWidth(1);
            ddownefficiencyfunc->SetLineColor(COLOR_DFIT);
            ddownefficiencyfunc->SetLineStyle(7);
            ddownefficiencyfunc->SetLineWidth(1);
        } 

        TMultiGraph *mg = new TMultiGraph();
        mg->Add(efficiencysim);
        mg->Add(efficiencydata);
        string title="Efficiency fitted via ";
        title.append(Type);
        title.append("-Model;Energy in keV; Efficiency in a.u.;");
        mg->SetTitle(title.c_str());
        mg->Draw("AP");
        
        EFunction->Draw("same");
        if(ERRORBARS)dupefficiencyfunc->Draw("same");
        if(ERRORBARS)ddownefficiencyfunc->Draw("same");
        TLegend *legend;
        legend=new TLegend(0.7,0.5,0.9,0.9);
        legend->AddEntry(EFunction,EFunction->GetName(),"l");
        legend->AddEntry(efficiencydata,efficiencydata->GetName(),"l");
        legend->AddEntry(efficiencysim,efficiencysim->GetName(),"l");
        if(ERRORBARS)legend->AddEntry(dupefficiencyfunc,dupefficiencyfunc->GetName(),"l");
        legend->Draw();
        

        Output out;
        out.SetDate();
        string str="Efficiency_Fit_Plot_";
        str.append(FileName);
        str.append("_");
        str.append(std::to_string((int)DetectorAngles[t]));
        str.append("_deg_");
        str.append(out.GetDate());
        str.append(".pdf");
        str="Output/"+str;
        efficiencyplot->SaveAs(str.c_str());
        cout<<"Efficiency of Detector under "<<(int)DetectorAngles[t]<< "deg saved. ( "<<str<<" )"<<endl;
        


        delete efficiencyplot;
        Parameter_All[t].insert(Parameter_All[t].begin()+1,1,Parameter_All[t][NumberofParameters]);
        Parameter_All[t].pop_back();
    }
}