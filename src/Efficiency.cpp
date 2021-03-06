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


Efficiency::Efficiency():
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
{
    NumberofParameters=6;
};

// ---------------------------------------------------------
Efficiency::~Efficiency()
{

};

// ---------------------------------------------------------

void Efficiency::CalculateEfficiency()
{
    if(sData.size()==1)for(unsigned int i=0;i<ECalTime.size();i++)sData.push_back(sData[0]);
    vector<unsigned int>sortinglist;
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    for(unsigned int t=0;t<ECalTime.size();t++)
    {
        sortinglist=Functions::Maching2Doubles(ecalData[t],0,'C',sData[t],0,'C');
        
        for(unsigned int i=0;i<ecalData[t].size();i++)
        {
            tempVector.push_back(sData[t][sortinglist[i]][0]);
            tempVector.push_back(sData[t][sortinglist[i]][1]);
            tempVector.push_back(ecalData[t][i][4]/sData[t][sortinglist[i]][2]/ECalTime[t]);
            tempVector.push_back(ecalData[t][i][5]/sData[t][sortinglist[i]][2]/ECalTime[t]);
            temp2DVector.push_back(tempVector);
            tempVector.clear();
        }
        // read.Print2DArray(temp2DVector);
        EfficiencyDataArray.push_back(temp2DVector);
        temp2DVector.clear();
    }

};

// ---------------------------------------------------------

void Efficiency::FitEfficiency()
{
    NFit=(unsigned int) Parameter[0];
    Parameter.erase(Parameter.begin());
    FitParameterDistribution.resize(DetectorAngles.size());
    for(unsigned int i=0;i<FitParameterDistribution.size();i++)FitParameterDistribution[i].resize(NFit);
    Parameter_All.resize(DetectorAngles.size());
    for(unsigned int i=0;i<Parameter_All.size();i++)Parameter_All[i].resize(NumberofParameters);   
    cout<<"Fitting Detector Efficiency..."<<endl;
    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {
        TF1 EFunction("EFunction",Functions::knoll, 0, 10000,NumberofParameters);
        for(unsigned int i = 0 ; i<NumberofParameters;i++){EFunction.ReleaseParameter(i);};
        for(unsigned int i = 0 ; i<NumberofParameters;i++){EFunction.SetParameter(i,Parameter[i]);};
        int datapoints_sim=(int) simulationData[t].size();
        Double_t xvec_sim[datapoints_sim];
        Double_t yvec_sim[datapoints_sim];
        for(unsigned int i = 0;i<simulationData[t].size();i++)
        {
            xvec_sim[i]=simulationData[t][i][0];
            yvec_sim[i]=simulationData[t][i][1];
        }
        TGraphErrors DataGraph((const Int_t)datapoints_sim,xvec_sim,yvec_sim,0,0);
        EFunction.FixParameter(0,Parameter[0]);//Fixing Scale
        DataGraph.Fit(&EFunction,"Q");
        for(unsigned int i=0;i<NumberofParameters;i++)
        {
            Parameter_All[t][i]=EFunction.GetParameter(i);
        }
    }
    Threads.resize(NumberOfThreads);
    for(unsigned int i = 0;i<NumberOfThreads+1;i++)Percent.push_back(0);
    for(unsigned int i = 0 ; i<NumberOfThreads ; i++)ThreadIsFinished.push_back(false);
    for(unsigned int i = 0 ; i<NumberOfThreads ; i++)ThreadIsInitialized.push_back(false);
    if (NumberOfThreads > 1)
    {
            // Start threads
        for (unsigned int t = 0; t < NumberOfThreads; ++t)
        {
            TString Name = "Thread ";
            Name += t;
            // cout<<"Creating thread: "<<Name<<endl;
            TThread* Thread = new TThread(Name, (void(*) (void *)) &CallParallelEfficiencyFitThread, (void*) new ThreadCallerEfficiency(this, t));
            Threads[t] = Thread;
            gSystem->Sleep(10);
            Thread->Run();
            // Wait until thread is initialized:
            while (ThreadIsInitialized[t] == false && ThreadIsFinished[t] == false)
            {
                // Sleep for a while...
                TThread::Sleep(0, 10000000);
            }    
            cout<<Name<<" is running"<<endl;
        }
            bool ThreadsAreRunning = true;
        while (ThreadsAreRunning == true)
        {
            // Sleep for a while...
            TThread::Sleep(0, 10000000);
            ThreadsAreRunning = false;
            for (unsigned int t = 0; t < NumberOfThreads; ++t)
            {
                if(ThreadIsFinished[t] == false)
                {
                    ThreadsAreRunning = true;
                    break;
                }
            }
        }
        // None of the threads are running any more --- kill them
        for (unsigned int t = 0; t < NumberOfThreads; ++t)
        {
            Threads[t]->Kill();
            Threads[t] = 0;
        }
    }
    // Non-threaded mode
    else
    {
        EfficiencyFitter(0);
    }
    cout<<endl;

    cout<<"Detector Efficiency fitted. Check log files!"<<endl;
}

// ---------------------------------------------------------

void Efficiency::CallParallelEfficiencyFitThread(void* Address)
{
  Efficiency* E = ((ThreadCallerEfficiency *) Address)->GetThreadCaller();
  E->EfficiencyFitter(((ThreadCallerEfficiency*) Address)->GetThreadID());
}

// ---------------------------------------------------------


bool Efficiency::EfficiencyFitter(unsigned int NThread)
{
    double Scale=0;
    string equal="==========";
    string space="          ";
    bool percent[128]={false};
    ThreadIsInitialized[NThread] = true;
    EfficiencyFitFunc FitFunction;


    // possible choices are:
    // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
    //  Minuit2                     Fumili2
    //  Fumili
    //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
    //                              BFGS2, SteepestDescent
    //  GSLMultiFit
    //   GSLSimAn
    //   Genetic

    vector<vector<vector<double> > >Data;
    vector<double>Part;
    TF1 EFunction("EFunction",Functions::knoll, 0, 10000,NumberofParameters);
    vector<vector<double> >Par;
    Data=GetEfficiencyDataArray();
    Par=GetFittedParameters();
    unsigned int NumberofFits=NFit/NumberOfThreads;
	for(unsigned int i=0;i<NumberofFits;i++)
    {
        for(unsigned int NDetector=0;NDetector<DetectorAngles.size();NDetector++)
        {
            for(unsigned int k = 0 ; k<NumberofParameters;k++){EFunction.ReleaseParameter(k);};
            for(unsigned int k = 0 ; k<NumberofParameters;k++)EFunction.FixParameter(k,Par[NDetector][k]);
            EFunction.ReleaseParameter(0);
        
            int datapoints_ecal=(int)Data[NDetector].size();
            // Double_t xvec_ecal[datapoints_ecal];
            // Double_t yvec_ecal[datapoints_ecal];
            // Double_t yvec_sim[datapoints_ecal];

            std::vector<double> yvec_ecal(datapoints_ecal,0);
            std::vector<double> yvec_sim(datapoints_ecal,0);
        
            TRandom3 random(0);
        
            // ROOT::Fit::Fitter TheFitter;
            // TheFitter.Config().SetMinimizer("GSLMultiFit");
            // TheFitter.Config().SetMinimizer("Minuit2","Combined");
            // TheFitter.SetFunction(FitFunction);
            // TheFitter.Config().MinimizerOptions().SetPrintLevel(0);
        
                for(unsigned int j=0;j<Data[NDetector].size();j++)
                {
                    // xvec_ecal[j]=random.Gaus(Data[NDetector][j][0],Data[NDetector][j][1]);
                    yvec_ecal[j]=random.Gaus(Data[NDetector][j][2],Data[NDetector][j][3]);
                    yvec_sim[j]=EFunction.Eval(random.Gaus(Data[NDetector][j][0],Data[NDetector][j][1]));
                }
                // TGraph DataGraph((const Int_t)datapoints_ecal,xvec_ecal,yvec_ecal);
                TThread::Lock();
                unsigned int ID = i;
                for(unsigned int j=1;j<101;j++)
                {
                    if(j*NFit/100/NumberOfThreads==ID+1 && percent[j]==false)
                    {
                        Percent[NThread]=j;
                        unsigned int val=0;
                        for(unsigned int p=0;p<NumberOfThreads;p++)val+=Percent[p];
                        Percent[Percent.size()-1]=val/NumberOfThreads;
                        cout<<"\r";
                        // for(unsigned int p=0;p<NumberOfThreads;p++)
                        //     {
                        //         cout<<"Thread "<<p<<": "<<Percent[p]<<"%\t";
                        //     }
                            cout<<"Total: "<<Percent[Percent.size()-1]<<"%";
                            cout<<std::flush;
                        percent[j]=true;
                        break;
                    }
                }
                TThread::UnLock();
                // ROOT::Fit::BinData d;
                // ROOT::Fit::FillData(d,&DataGraph); 
                // // TheFitter.Config().ParSettings(0).SetName("Scale");
                // TheFitter.Config().ParSettings(0).SetValue(Par[NDetector][0]);
                // // TheFitter.Config().ParSettings(1).SetName("a0");
                // TheFitter.Config().ParSettings(1).SetValue(Par[NDetector][1]);
                // TheFitter.Config().ParSettings(1).Fix();
                // // TheFitter.Config().ParSettings(2).SetName("a1");
                // TheFitter.Config().ParSettings(2).SetValue(Par[NDetector][2]);
                // TheFitter.Config().ParSettings(2).Fix();
                // // TheFitter.Config().ParSettings(3).SetName("a2");
                // TheFitter.Config().ParSettings(3).SetValue(Par[NDetector][3]);
                // TheFitter.Config().ParSettings(3).Fix();
                // // TheFitter.Config().ParSettings(4).SetName("a3");
                // TheFitter.Config().ParSettings(4).SetValue(Par[NDetector][4]);
                // TheFitter.Config().ParSettings(4).Fix();
                // // TheFitter.Config().ParSettings(5).SetName("a4");
                // TheFitter.Config().ParSettings(5).SetValue(Par[NDetector][5]);
            
                // bool ReturnCode = false;
                // ReturnCode = TheFitter.Fit(d);
                // if(ReturnCode == true)
                {
                    // Scale = TheFitter.Result().Parameter(0);
                    // TThread::Printf("Thread %i: Results E0= %.5f     Scale= %.5f    Nr: %i", NThread, E0,Scale,ID);
                    // TThread::Printf("Thread %i: ID=%i     i=%i",NThread,ID,ID+NumberofFits*NThread);
                }
                // FitParameterDistribution[NDetector][ID+NumberofFits*NThread]=Scale;
                // TThread::Printf("%f",ScaleVal(yvec_ecal,yvec_sim));
                FitParameterDistribution[NDetector][ID+NumberofFits*NThread]=ScaleVal(yvec_ecal,yvec_sim);
        }
    }
    ThreadIsFinished[NThread]=true;
    return true;
}
    

// ---------------------------------------------------------

    void Efficiency::PlotScaleDist()
    {
        TFile* RFile=TFile::Open(rfile.c_str(),"update");

        for(unsigned int t=0;t<DetectorAngles.size();t++)
        {
            string dirname="Efficiency/Detector ";
            dirname.append(std::to_string((int)DetectorAngles[t]));
            RFile->mkdir(dirname.c_str());
            RFile->cd(dirname.c_str());
            double xlow=TMath::MinElement(NFit,&FitParameterDistribution[t][0]);
            double xhi=TMath::MaxElement(NFit,&FitParameterDistribution[t][0]);

            int numberofdigits=10;
            xlow=xlow*numberofdigits;
            xlow=floor(xlow);
            xlow=xlow/numberofdigits;

            xhi=xhi*numberofdigits;
            xhi=ceil(xhi);
            xhi=xhi/numberofdigits;

            string names="Scaling Parameter Histogram ";
            string title="Scaling Parameter Distribution ";
            title+=std::to_string((int)DetectorAngles[t]);
            names+=std::to_string((int)DetectorAngles[t]);

            TCanvas ParameterHist(names.c_str(),title.c_str(),1600,900);
            ParameterHist.SetGrid();
            ParameterHist.GetFrame()->SetFillColor(21);
            ParameterHist.GetFrame()->SetBorderSize(12);

            
            TH1I ScalingParameter(names.c_str(),title.c_str(),100,xlow,xhi);
            for(unsigned int i=0;i<FitParameterDistribution[t].size();i++)ScalingParameter.Fill(FitParameterDistribution[t][i]);
            double max=ScalingParameter.GetMaximum();
            ScalingParameter.SetMaximum(YSCALE*max);
            double boundary[2]={0,0};


            Functions::ShortestCoverage(FitParameterDistribution[t],boundary);


            TLine b_lower(boundary[0],0,boundary[0],ScalingParameter.GetMaximum());
            TLine b_upper(boundary[1],0,boundary[1],ScalingParameter.GetMaximum());

            b_lower.SetLineColor(2);
            b_lower.SetLineWidth(2);

            b_upper.SetLineColor(2);
            b_upper.SetLineWidth(2);

            ScalingParameter.Draw("");
            b_lower.Draw("same");
            b_upper.Draw("same");

            std::reverse(FileName.begin(),FileName.end());
            stringstream ssFile(FileName);
            FileName="";
            getline(ssFile,FileName,'/');
            std::reverse(FileName.begin(),FileName.end());

            string str=Output::dir+"Efficiency/Efficiency_Scale_";
            str.append(FileName);
            str.append("_");
            str.append(std::to_string((int)DetectorAngles[t]));
            str.append("_deg");
            str.append(".pdf");
            ParameterHist.SaveAs(str.c_str());
            cout<<"Distribution of Scale saved. ( "<<str<<" )"<<endl;
            ParameterHist.Write();

            Parameter_All[t][0]=ScalingParameter.GetBinCenter(ScalingParameter.GetMaximumBin());
            Parameter_All[t].insert(Parameter_All[t].begin()+1,{boundary[0],boundary[1]});
        }
        RFile->Write();
        // RFile->Close();
    }
// ---------------------------------------------------------

    void Efficiency::PlotEFunc()
    {
        TFile* RFile=TFile::Open(rfile.c_str(),"update");

        TF1 EFunction("EFunction",Functions::knoll, 0, 10000,NumberofParameters);
        TF1 EFunctionUP("EFunction",Functions::knoll, 0, 10000,NumberofParameters);
        TF1 EFunctionDOWN("EFunction",Functions::knoll, 0, 10000,NumberofParameters);
        for(unsigned int t=0;t<DetectorAngles.size();t++)
        {
            string dirname="Efficiency/Detector ";
            dirname.append(std::to_string((int)DetectorAngles[t]));
            RFile->cd(dirname.c_str());
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
            Double_t scale=Parameter_All[t][0];

            for(unsigned int i = 0;i<simulationData[t].size();i++)yvec_sim[i]=yvec_sim[i]*scale;

            string names="Efficiency Data Plot ";
            names+=std::to_string((int)DetectorAngles[t]);
            TCanvas efficiencyplot(names.c_str(),names.c_str(),1600,900);
            efficiencyplot.SetGrid();
            efficiencyplot.GetFrame()->SetFillColor(21);
            efficiencyplot.GetFrame()->SetBorderSize(12);

            names="Efficiency Data";
            names+=std::to_string((int)DetectorAngles[t]);

            TGraphErrors efficiencydata(datapoints_ecal,xvec_ecal,yvec_ecal,dxvec_ecal,dyvec_ecal);
            efficiencydata.SetMarkerStyle(20);
            efficiencydata.SetMarkerColor(COLOR_ECAL);
            efficiencydata.SetMarkerSize(1);
            efficiencydata.SetName(names.c_str());
            efficiencydata.SetTitle(names.c_str());
            efficiencydata.SetFillStyle(0);
            efficiencydata.SetMaximum(YSCALE*TMath::MaxElement(datapoints_ecal,yvec_ecal));
            efficiencydata.SetMinimum(0);
            efficiencydata.GetYaxis()->SetNdivisions(505);   //Sets number of Ticks
            efficiencydata.GetYaxis()->SetDecimals(kTRUE); //Sets same digits
        
            names="Efficiency Simulation ";
            names+=std::to_string((int)DetectorAngles[t]);
            TGraphErrors efficiencysim(datapoints_sim,xvec_sim,yvec_sim,0,0);
            efficiencysim.SetMarkerStyle(20);
            efficiencysim.SetMarkerColor(COLOR_SIM);
            efficiencysim.SetMarkerSize(1);
            efficiencysim.SetLineColor(COLOR_SIM);
            efficiencysim.SetName(names.c_str());
            efficiencysim.SetTitle(names.c_str());
            efficiencysim.SetFillStyle(0);
            efficiencysim.SetMaximum(YSCALE*TMath::MaxElement(datapoints_sim,yvec_sim));
            efficiencysim.SetMinimum(0);
            efficiencysim.GetYaxis()->SetNdivisions(505);   //Sets number of Ticks
            efficiencysim.GetYaxis()->SetDecimals(kTRUE); //Sets same digits

            names="Efficiency Function ";
            names+=std::to_string((int)DetectorAngles[t]);
            EFunction.SetParameter(0,Parameter_All[t][0]);
            for(unsigned int i=1;i<NumberofParameters;i++)
            {
                EFunction.SetParameter(i,Parameter_All[t][i+2]);
            }
            EFunction.SetLineWidth(1);
            EFunction.SetLineColor(COLOR_FIT);
            EFunction.SetName(names.c_str());
            EFunction.SetName(names.c_str());
        
            if(ERRORBARS)
            {   
                EFunctionUP.SetParameter(0,Parameter_All[t][1]);
                EFunctionDOWN.SetParameter(0,Parameter_All[t][2]);
                for(unsigned int i=1;i<NumberofParameters;i++)
                {
                    EFunctionUP.SetParameter(i,Parameter_All[t][i+2]);
                    EFunctionDOWN.SetParameter(i,Parameter_All[t][i+2]);
                }
                EFunctionUP.SetLineColor(COLOR_DFIT);
                EFunctionUP.SetLineStyle(7);
                EFunctionUP.SetLineWidth(1);
                EFunctionDOWN.SetLineColor(COLOR_DFIT);
                EFunctionDOWN.SetLineStyle(7);
                EFunctionDOWN.SetLineWidth(1);
                EFunctionUP.SetName("1-Sigma-Range");
            }

        TMultiGraph *mg = new TMultiGraph();
        mg->Add(&efficiencysim);
        mg->Add(&efficiencydata);
        string title="Efficiency fitted via Knoll-Model ";
        title.append(std::to_string((int) DetectorAngles[t]));
        title.append(" deg;Energy in keV;Efficiency in a.u.;");
        mg->SetTitle(title.c_str());
        mg->Draw("AP");
        
        EFunction.Draw("same");
        if(ERRORBARS)EFunctionUP.Draw("same");
        if(ERRORBARS)EFunctionDOWN.Draw("same");
        TLegend *legend;
        legend=new TLegend(0.7,0.5,0.9,0.9);
        legend->AddEntry(&EFunction,EFunction.GetName(),"l");
        legend->AddEntry(&efficiencydata,efficiencydata.GetName(),"l");
        legend->AddEntry(&efficiencysim,efficiencysim.GetName(),"l");
        if(ERRORBARS)legend->AddEntry(&EFunctionUP,EFunctionUP.GetName(),"l");
        legend->Draw();
        
        std::reverse(FileName.begin(),FileName.end());
        stringstream ssFile(FileName);
        FileName="";
        getline(ssFile,FileName,'/');
        std::reverse(FileName.begin(),FileName.end());


        string str=Output::dir+"Efficiency/Efficiency_Fit_";
        str.append(std::to_string((int)DetectorAngles[t]));
        str.append("_");
        str.append(FileName);
        str.append(".pdf");
        efficiencyplot.SaveAs(str.c_str());
        cout<<"Efficiency of Detector under "<<(int)DetectorAngles[t]<< "deg saved. ( "<<str<<" )"<<endl;
        efficiencyplot.Write();
        EFunction.Write();
        efficiencydata.Write();
        efficiencysim.Write();
    }
    RFile->Write();
    // RFile.Close();
}
