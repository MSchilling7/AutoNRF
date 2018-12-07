/**
 * @file Gamma.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Gamma class
 */

#include"Gamma.h"

Gamma::Gamma()
{
};

// ---------------------------------------------------------
Gamma::~Gamma()
{

};

// ---------------------------------------------------------

void Gamma::CalculateResults()
{
    SortExperimentalData();
    CalculateEnergyResults();
    CalculateFluxResults();
    CalculateEfficiencyResults();
    CalculateICSResults();
}

// ---------------------------------------------------------

void Gamma::CalculateFluxResults()
{
    Functions func;
    vector<double> tempVector;
    unsigned int NumberofParameters = 3;
    TF1 FluxFunc("FluxFunc",Functions::Schiff, 0, 10000,NumberofParameters);
    TF1 FluxFuncLOW("FluxFunc",Functions::Schiff, 0, 10000,NumberofParameters);
    TF1 FluxFuncHIGH("FluxFunc",Functions::Schiff, 0, 10000,NumberofParameters);
    double Scale     =Parameter_Flux[0];
    double ScaleLow  =Parameter_Flux[1];
    double ScaleHigh =Parameter_Flux[2];
    double E0        =Parameter_Flux[3];
    double E0Low     =Parameter_Flux[4];
    double E0High    =Parameter_Flux[5];
    double Z         =Parameter_Flux[6];
    double val       =0;
    double vallow    =0;
    double valhigh   =0;
    FluxFunc.SetParameter(0,Scale);
    FluxFunc.SetParameter(1,E0);
    FluxFunc.SetParameter(2,Z);

    FluxFuncLOW.SetParameter(0,ScaleLow);
    FluxFuncLOW.SetParameter(1,E0Low);
    FluxFuncLOW.SetParameter(2,Z);
    
    FluxFuncHIGH.SetParameter(0,ScaleHigh);
    FluxFuncHIGH.SetParameter(1,E0High);
    FluxFuncHIGH.SetParameter(2,Z);


    for(unsigned int i=0;i<Energy.size();++i)
    {
        val=FluxFunc.Eval(Energy[i][0]);
        vallow=fabs(val-FluxFuncLOW.Eval(Energy[i][0]));
        vallow=vallow*vallow/(val*val);
        vallow=val*sqrt(vallow);
        valhigh=fabs(val-FluxFuncHIGH.Eval(Energy[i][0]));
        valhigh=valhigh*valhigh/(val*val);
        valhigh=val*sqrt(valhigh);
        tempVector.push_back(Energy[i][0]);
        tempVector.push_back(Energy[i][1]);
        tempVector.push_back(val);
        tempVector.push_back(vallow);
        tempVector.push_back(valhigh);
        Flux.push_back(tempVector);
        tempVector.clear();
    }
} 

// ---------------------------------------------------------

void Gamma::CalculateEfficiencyResults()
{
    Functions func;
    vector<double> tempVector;
    vector<vector<double> >temp2DVector;
    unsigned int NumberofParameters = 6;
    TF1 EfficiencyFunction("EfficiencyFunction",Functions::knoll, 0, 10000,NumberofParameters);
    TF1 EfficiencyFunctionLOW("EfficiencyFunction",Functions::knoll, 0, 10000,NumberofParameters);
    TF1 EfficiencyFunctionHIGH("EfficiencyFunction",Functions::knoll, 0, 10000,NumberofParameters);
    double ScaleLow=0;
    double ScaleHigh=0;
    double val=0;
    double vallow=0;
    double valhigh=0;
    for(unsigned int t=0;t<DetectorAngles.size();++t)
    {
        ScaleLow=Parameter_Efficiency[t][1];
        ScaleHigh=Parameter_Efficiency[t][2];
        Parameter_Efficiency[t].erase(Parameter_Efficiency[t].begin()+1);
        Parameter_Efficiency[t].erase(Parameter_Efficiency[t].begin()+1);
        for(unsigned int i=0;i<=NumberofParameters;i++)
        {
            EfficiencyFunction.SetParameter(i,Parameter_Efficiency[t][i]);
        }
        for(unsigned int i=1;i<=NumberofParameters;i++)
        {
            EfficiencyFunctionLOW.SetParameter(i,Parameter_Efficiency[t][i]);
        }
        for(unsigned int i=1;i<=NumberofParameters;i++)
        {
            EfficiencyFunctionHIGH.SetParameter(i,Parameter_Efficiency[t][i]);
        }
        EfficiencyFunctionLOW.SetParameter(0,ScaleLow);
        EfficiencyFunctionHIGH.SetParameter(0,ScaleHigh);
        for(unsigned int i=0;i<ExperimentalData[t].size();++i)
        {
            val=EfficiencyFunction.Eval(ExperimentalData[t][i][0]);
            vallow=fabs(val-EfficiencyFunctionLOW.Eval(ExperimentalData[t][i][0]));
            vallow=vallow*vallow/(val*val)+func.relError2(ExperimentalData[t][i],4,5);
            vallow=val*sqrt(vallow);
            valhigh=fabs(val-EfficiencyFunctionHIGH.Eval(ExperimentalData[t][i][0]));
            valhigh=valhigh*valhigh/(val*val)+func.relError2(ExperimentalData[t][i],4,5);
            valhigh=val*sqrt(valhigh);
            tempVector.push_back(ExperimentalData[t][i][0]);
            tempVector.push_back(ExperimentalData[t][i][1]);
            tempVector.push_back(val);
            tempVector.push_back(vallow);
            tempVector.push_back(valhigh);
            temp2DVector.push_back(tempVector);
            tempVector.clear();
        }
        calcEfficiency.push_back(temp2DVector);
        temp2DVector.clear();
    }
}

// ---------------------------------------------------------

void Gamma::CalculateEnergyResults()
{
    double temp=0,temp2=0;
    vector<double>tempVector;
    for(unsigned int i = 0;i<ExperimentalDataSorted.size();++i)
    {
        for(unsigned int j=0;j<ExperimentalDataSorted[i].size();++j)
        {
            temp+=ExperimentalDataSorted[i][j][0];
            temp2+=ExperimentalDataSorted[i][j][1]*ExperimentalDataSorted[i][j][1];
        }
        temp=temp/((double) ExperimentalDataSorted[i].size());
        temp2=sqrt(temp2/((double) ExperimentalDataSorted[i].size()));
        tempVector.push_back(temp);
        tempVector.push_back(temp2);
        temp=0;
        temp2=0;
        Energy.push_back(tempVector);
        tempVector.clear();
    }
}


// ---------------------------------------------------------

void Gamma::SortExperimentalData()
{
    vector<unsigned int> tempVector{};
    vector<vector<unsigned int> > temp2DVector{}; 
    vector<vector<double> > temp2DVectorD{}; 
    vector<vector<vector<double> > > temp3DVector{}; 
    for(unsigned int i = 0;i<ExperimentalData.size();++i)
    {
        tempVector.push_back((unsigned int)ExperimentalData[i].size());
        tempVector.push_back(i);
        temp2DVector.push_back(tempVector);
        tempVector.clear();
    }
    std::sort(temp2DVector.begin(),temp2DVector.end());
    std::reverse(temp2DVector.begin(),temp2DVector.end());
    temp3DVector=ExperimentalData;
    temp2DVectorD=EfficiencyFitParameterDistribution;
    for(unsigned int i =0;i<ExperimentalData.size();++i)
    {
        ExperimentalData[i]=temp3DVector[temp2DVector[i][1]];
        EfficiencyFitParameterDistribution[i]=temp2DVectorD[temp2DVector[i][1]];
    }
    temp2DVectorD=Parameter_Efficiency;
    for(unsigned int i =0;i<Parameter_Efficiency.size();++i)Parameter_Efficiency[i]=temp2DVectorD[temp2DVector[i][1]];

    temp2DVectorD=ExperimentalDataAngular;
    for(unsigned int i=0;i<ExperimentalDataAngular.size();++i)
    {
        for(unsigned int j=1;j<ExperimentalDataAngular[i].size();++j)
        {
            ExperimentalDataAngular[i][j]=temp2DVectorD[i][temp2DVector[j-1][1]+1];
        }
    }
    ExperimentalDataSorted.resize(ExperimentalData[0].size());
    for(unsigned int i=0;i<ExperimentalDataSorted.size();++i)
    {
        ExperimentalDataSorted[i].resize(ExperimentalData.size());
        for(unsigned int j=0;j<ExperimentalDataSorted[i].size();j++)ExperimentalDataSorted[i][j].resize(ExperimentalData[0][0].size());
    }
    for(unsigned int i = 0; i < ExperimentalData.size(); ++i)
    {
        if(i==0)
        {
            for(unsigned int j =0;j<ExperimentalData[i].size();++j)
            {
                ExperimentalDataSorted[j][i]=ExperimentalData[i][j];
            }
        }
        for(unsigned int j =0;j<ExperimentalData[i].size();++j)
        {
            ExperimentalDataSorted[Functions::MinIndex(ExperimentalData[i][j][0],ExperimentalDataSorted)][i]=ExperimentalData[i][j];
        }
     
    }
    for(unsigned int i = 0;i<ExperimentalDataSorted.size();++i)
    {
        for(unsigned int j=0;j<ExperimentalDataSorted[i].size();++j)
        {
            if(ExperimentalDataSorted[i][j][0]==0.)
            {   
                ExperimentalDataSorted[i].erase(ExperimentalDataSorted[i].begin()+j);
            }
        }
    }
    // for(unsigned int i = 0;i<ExperimentalDataSorted.size();++i)
    // {
        // for(unsigned int j=0;j<ExperimentalDataSorted[i].size();++j)
        // {
            // cout<<std::setw(15)<<ExperimentalDataSorted[i][j][0];
        // }
        // cout<<endl;
    // }

}

// ---------------------------------------------------------

double Gamma::GimmeICS(double area,double mass,double flux,double efficiency,double W)
{
    double val=0;
    val=area/(mass*flux*efficiency*W);
    return val;
}

// --------------------------------------------------------

void Gamma::CalculateICSDist(unsigned int ID,unsigned int fits)
{
    cout<<"Thread "<<ID<<" is running"<<endl;
    unsigned int NumberofParameters = 3;
    TF1 FluxFunc("FluxFunc",Functions::Schiff, 0, 10000,NumberofParameters);

    NumberofParameters = 6;
    TF1 EfficiencyFunc("EfficiencyFunc",Functions::knoll, 0, 10000,NumberofParameters);

    TH1D EfficiencyDist[EfficiencyFitParameterDistribution.size()];
    TH1D FluxDist[FluxFitParameterDistribution.size()];

    double min;
    double max;
    for(unsigned int i=0;i<EfficiencyFitParameterDistribution.size();++i)
    {
        min=TMath::MinElement(EfficiencyFitParameterDistribution[i].size(),&EfficiencyFitParameterDistribution[i][0]);
        max=TMath::MaxElement(EfficiencyFitParameterDistribution[i].size(),&EfficiencyFitParameterDistribution[i][0]);
        EfficiencyDist[i]=TH1D("","",100,min,max);
        for(unsigned int j=0;j<EfficiencyFitParameterDistribution[i].size();++j)EfficiencyDist[i].Fill(EfficiencyFitParameterDistribution[i][j]);
    }
    for(unsigned int i=0;i<FluxFitParameterDistribution.size();++i)
    {
        min=TMath::MinElement(FluxFitParameterDistribution[i].size(),&FluxFitParameterDistribution[i][0]);
        max=TMath::MaxElement(FluxFitParameterDistribution[i].size(),&FluxFitParameterDistribution[i][0]);
        FluxDist[i]=TH1D("","",100,min,max);
        for(unsigned int j=0;j<FluxFitParameterDistribution[i].size();++j)FluxDist[i].Fill(FluxFitParameterDistribution[i][j]);
    }

    TRandom3 random(0);
    double energy=0;
    double mass=0;
    double flux=0;
    double efficiency=0;
    double area=0;
    double W=0;
    double ICSVal=0;
    unsigned int IDFit=0;
    bool percent[128];



    vector<unsigned int> minlist;
    for(unsigned int l=0;l<fits;++l)
    {
        for(unsigned int i =0;i<ExperimentalDataSorted.size();++i)
        {
            minlist=Functions::Maching2Doubles(ExperimentalDataSorted[i],0,'C',ExperimentalDataAngular,0,'C');
            IDFit=l;
            for(unsigned int j = 0; j < ExperimentalDataSorted[i].size(); ++j)
            {
                energy=random.Gaus(ExperimentalDataSorted[i][j][0],ExperimentalDataSorted[i][j][1]);
                area=random.Gaus(ExperimentalDataSorted[i][j][4],ExperimentalDataSorted[i][j][5]);
                mass=random.Gaus(Mass[0],Mass[1]);
                FluxFunc.SetParameter(0,FluxDist[0].GetRandom());
                FluxFunc.SetParameter(1,FluxDist[1].GetRandom());
                FluxFunc.SetParameter(2,Parameter_Flux[6]);
                flux=FluxFunc.Eval(energy);
                EfficiencyFunc.SetParameter(0,EfficiencyDist[j].GetRandom());
                for(unsigned int m =1;l<Parameter_Efficiency[j].size();++l)EfficiencyFunc.SetParameter(m,Parameter_Efficiency[j][m]);
                efficiency=EfficiencyFunc.Eval(energy);
                W=ExperimentalDataAngular[minlist[j]][j+1];
                ICSVal+=GimmeICS(area,mass,flux,efficiency,W);
                // if(j==0 && i==0)cout<<std::setw(15)<<ICSVal<<std::setw(15)<<energy<<std::setw(15)<<area<<std::setw(15)<<mass<<std::setw(15)<<flux<<std::setw(15)<<efficiency<<std::setw(15)<<W<<endl;
            }   
            ICSVal=ICSVal/((double)ExperimentalDataSorted[i].size());
            ICSDist[i][l+ID*fits]=ICSVal;
            ICSVal=0;

            for(unsigned int j=1;j<101;++j)
            {
                if((unsigned int)(j*fits/100.)==IDFit+1 && percent[j]==false)
                {
                    Percent[ID]=j;
                    unsigned int val=0;
                    for(unsigned int p=0;p<NumberOfThreads;++p)val+=Percent[p];
                    Percent[Percent.size()-1]=val/NumberOfThreads;
                    cout<<"\r";
                    // for(unsigned int p=0;p<NumberOfThreads;++p)
                    //     {
                    //         cout<<std::fixed<<"Thread "<<p<<": "<<Percent[p]<<"%\t";
                    //     }
                        cout<<"Total: "<<std::fixed<<Percent[Percent.size()-1]<<"%";
                        cout<<std::flush;
                    percent[j]=true;
                    break;
                }
            }
        }
    }
}

// ---------------------------------------------------------

void Gamma::CalculateICSResults()
{
    cout<<"Calculate Integrated Cross Sections..."<<endl;
    double u=1.660539e-24;//   (#/g)
    Mass[0]=Mass[0]/(Mass[2]*u);
    Mass[1]=Mass[1]/(Mass[2]*u);
    double min=0,max=0;
    vector<double> tempVector;

    TH1D ICSHist[ExperimentalDataSorted.size()];
    Percent.resize(NumberOfThreads+1);

    unsigned int maxFits=(unsigned int)std::max(EfficiencyFitParameterDistribution[0].size(),FluxFitParameterDistribution[0].size());
    ICSDist.resize(ExperimentalDataSorted.size());
    for(unsigned int i = 0; i < ICSDist.size(); ++i)ICSDist[i].resize(maxFits);
    unsigned int FitsperThread=(unsigned int) maxFits/NumberOfThreads;

    thread t[NumberOfThreads];
    for(unsigned int i=0;i<NumberOfThreads;++i)
    {
        t[i]=CalcICSDistThread(i,FitsperThread);
        std::this_thread::sleep_for (std::chrono::milliseconds(1));
    }
    for(unsigned int i=0;i<NumberOfThreads;++i)t[i].join();
    cout<<endl;

    
    TFile* RFile=TFile::Open(rfile.c_str(),"update");
    RFile->mkdir("Results/ICS");
    RFile->cd("Results/ICS");
    string NameOfHist="";
    double boundary[2]={0,0};
    for(unsigned int i =0;i<Energy.size();++i)
    {
        min=TMath::MinElement(ICSDist[i].size(),&ICSDist[i][0]);
        max=TMath::MaxElement(ICSDist[i].size(),&ICSDist[i][0]);
        NameOfHist=std::to_string((int)Energy[i][0]);
        NameOfHist="ICS_"+NameOfHist;
        NameOfHist+="_keV_Peak";
        ICSHist[i]=TH1D(NameOfHist.c_str(),NameOfHist.c_str(),100,min,max);
        for(unsigned int j=0;j<ICSDist[i].size();++j)ICSHist[i].Fill(ICSDist[i][j]);
        Functions::ShortestCoverage(ICSDist[i],boundary);

        max=ICSHist[i].GetMaximum();
        ICSHist[i].SetMaximum(YSCALE*max);

        TLine b_lower(boundary[0],0,boundary[0],ICSHist[i].GetMaximum());
        TLine b_upper(boundary[1],0,boundary[1],ICSHist[i].GetMaximum());

        b_lower.SetLineColor(2);
        b_lower.SetLineWidth(2);

        b_upper.SetLineColor(2);
        b_upper.SetLineWidth(2);

        TCanvas Can_ICSHist(NameOfHist.c_str(),NameOfHist.c_str(),1600,900);
        Can_ICSHist.SetGrid();
        Can_ICSHist.GetFrame()->SetFillColor(21);
        Can_ICSHist.GetFrame()->SetBorderSize(12);

        ICSHist[i].Draw("");
        b_lower.Draw("same");
        b_upper.Draw("same");

        tempVector.push_back(Energy[i][0]);
        tempVector.push_back(Energy[i][1]);
        tempVector.push_back(ICSHist[i].GetBinCenter(ICSHist[i].GetMaximumBin()));
        tempVector.push_back(boundary[0]);
        tempVector.push_back(boundary[1]);
        ICS.push_back(tempVector);
        tempVector.clear();

        Can_ICSHist.Write();
        ICSHist[i].Write();
        NameOfHist=Output::dir+"Results/"+NameOfHist;
        NameOfHist+=".pdf";
        Can_ICSHist.SaveAs(NameOfHist.c_str());
        cout<<"Distribution of ICS saved. ( "<<NameOfHist<<" )"<<endl;
    }


}
