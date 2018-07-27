/**
 * @file Flux.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Flux class
 */

#include"Flux.h"

using std::cout;
using std::setw;
using std::endl;
using std::istringstream;
using std::getline;


Flux::Flux():
    FileName(""),
    rfile(""),
    NFit(0),
    NumberOfThreads(0),
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
{

};

// ---------------------------------------------------------
Flux::~Flux()
{

};

// ---------------------------------------------------------

void Flux::CorrectingPeakArea()
{   
    Functions func;
    vector<double> tempVector;
    vector< vector <double> > temp2DVector, temp2DVector2;
    vector <int> minlist,minlist2,minlist3;
    vector<vector <double> > Inelastic,Elastic;
    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {

        double temp=0;

    // Get the Gamma-Ray Energy from Input Parameter File for ExperimentalData File
        minlist=func.Maching2Doubles(ExperimentalData[t],0,'C',FluxParameter,E_GAMMA,'C');
        for(unsigned int i=0;i<ExperimentalData[t].size();i++){
            ExperimentalData[t][i][0]=FluxParameter[minlist[i]][E_GAMMA];
            
        }
        minlist.clear();
    // Get an Array of Inelastic Transitions from Input file
        for(unsigned int i=0;i<FluxParameter.size();i++)
        {
            if(FluxParameter[i][E_FINAL]!=0)Inelastic.push_back(FluxParameter[i]);
            if(FluxParameter[i][E_FINAL]==0)Elastic.push_back(FluxParameter[i]);
        };
    // Clean Input Data File from Inelastic Transitions
        for(unsigned int i=0;i<ExperimentalData[t].size();i++)
        {
            for(unsigned int j=0;j<FluxParameter.size();j++)
            {
                if(temp!=FluxParameter[j][EX])
                {
                    temp=FluxParameter[j][EX];
                    if(ExperimentalData[t][i][0]==FluxParameter[j][E_GAMMA])
                    {
                        minlist.push_back(i);
                    }
                }
            }
        }
        for(unsigned int i=0;i<minlist.size();i++)
        {
            temp2DVector.push_back(calcEfficiency[t][minlist[i]]);
            temp2DVector2.push_back(ExperimentalData[t][minlist[i]]);
        }
        ExperimentalData[t]=temp2DVector2;
        calcEfficiency[t]=temp2DVector;
        temp2DVector.clear();
        temp2DVector2.clear();
        minlist.clear();
        
        minlist=func.Maching2Doubles(ExperimentalData[t],0,'C',Elastic,EX,'C');
        for(unsigned int i=0;i<ExperimentalData[t].size();i++)
        {
            if(Elastic[minlist[i]][E_GAMMA]==ExperimentalData[t][i][0])
            {
                temp2DVector.push_back(Elastic[minlist[i]]);
            }
        }
        Elastic=temp2DVector;
        temp2DVector.clear();
        for(unsigned int i=0;i<Elastic.size();i++)
        {
            for(unsigned int j=0;j<Inelastic.size();j++)
            {
                if(Elastic[i][EX]==Inelastic[j][EX])temp2DVector.push_back(Inelastic[j]);
            }
        }
        Inelastic=temp2DVector;
        temp2DVector.clear();
        
        
    //calculate Gi/G0 from Gi/G
        for(unsigned int i=0;i<Elastic.size();i++)
        {
            for(unsigned int j=0;j<Inelastic.size();j++)
            {
                if(Elastic[i][EX]==Inelastic[j][EX])
                {
                    Inelastic[j][BRANCHING]=Inelastic[j][BRANCHING]/Elastic[i][BRANCHING];
                    Inelastic[j][DBRANCHING]=Inelastic[j][BRANCHING]/Elastic[i][BRANCHING]*sqrt(func.relError2(Inelastic[j],BRANCHING,DBRANCHING)+func.relError2(Elastic[i],BRANCHING,DBRANCHING));
                    
                }
            }
        }
        minlist=func.Maching2Doubles(Inelastic,EX,'C',ExperimentalData[t],0,'C');
        minlist2=func.Maching2Doubles(Inelastic,E_FINAL,'C',ExperimentalData[t],0,'C');
        minlist3=func.Maching2Doubles(Inelastic,E_FINAL,'C',Elastic,EX,'C');
    // Feeding Correction of Peak Areas (Top-Down)
        for(unsigned int i=0;i<Inelastic.size();i++)
        {
            double Volume, dVolume;
            if(DetectorAngles[t] == 90)Volume=Inelastic[i][BRANCHING]*calcEfficiency[t][minlist2[i]][2]/calcEfficiency[t][minlist[i]][2]*Inelastic[i][W90]/Elastic[minlist3[i]][W90]*ExperimentalData[t][minlist[i]][4];
            if(DetectorAngles[t] == 130)Volume=Inelastic[i][BRANCHING]*calcEfficiency[t][minlist2[i]][2]/calcEfficiency[t][minlist[i]][2]*Inelastic[i][W130]/Elastic[minlist3[i]][W130]*ExperimentalData[t][minlist[i]][4];
            
            dVolume=Volume*sqrt
            (
                func.relError2(calcEfficiency[t][minlist2[i]],2,3)+
                func.relError2(calcEfficiency[t][minlist[i]],2,3)+
                func.relError2(ExperimentalData[t][minlist[i]],4,5)+
                func.relError2(Inelastic[i],BRANCHING,DBRANCHING)
                );
            Inelastic[i][CORRECT_VOLUME]=Volume;
            Inelastic[i][CORRECT_DVOLUME]=dVolume;
        }
        for(unsigned int i=(unsigned int)Inelastic.size();0<i;--i)
        {
            unsigned int j=i-1;
            double Volume, dVolume;
            Volume=ExperimentalData[t][minlist2[j]][4]-Inelastic[j][CORRECT_VOLUME];
            dVolume=sqrt(ExperimentalData[t][minlist2[j]][5]*ExperimentalData[t][minlist2[j]][5]+Inelastic[j][CORRECT_DVOLUME]*Inelastic[j][CORRECT_DVOLUME]);
            
            ExperimentalData[t][minlist2[j]][4]=Volume;
            ExperimentalData[t][minlist2[j]][5]=dVolume;
        }
        Inelastic.clear();
        Elastic.clear();
        minlist.clear();
        minlist2.clear();
        minlist3.clear();
        tempVector.clear();
        temp2DVector.clear();
        temp2DVector2.clear();
    }
};

// ---------------------------------------------------------

void Flux::CalculateICS()
{
    Functions func;
    vector<double> tempVector;
    

    for(unsigned int i=0;i<FluxParameter.size();i++){
        tempVector.push_back(FluxParameter[i][2]);
        tempVector.push_back(FluxParameter[i][3]);
        
        double Energy=FluxParameter[i][2];
        double J1=FluxParameter[i][4];
        double J0=FluxParameter[i][5];
        double Gamma=FluxParameter[i][6];
        double Branching=FluxParameter[i][8];
        
        //Gamma is in eV to get the right Value convert it to keV
        tempVector.push_back(TMath::Pi()*TMath::Pi()*((2*J1+1)/(2*J0+1))*(hqc/Energy)*(hqc/Energy)*Gamma*(1e-3)*Branching);

        tempVector.push_back(tempVector[2]*sqrt(func.relError2(FluxParameter[i],6,7)+func.relError2(FluxParameter[i],8,9)));
        ICS.push_back(tempVector);
        tempVector.clear();
    }
    
};

// ---------------------------------------------------------

void Flux::CalculateFlux()
{

    Functions func;
    vector<vector<vector<double> > >tempVector;
    vector <int> minlist;
    double Area;
    double dArea;
    PhotonFluxData.resize(ExperimentalData[0].size());
    for(unsigned int i=0;i<PhotonFluxData.size();i++)PhotonFluxData[i].resize(4);
    tempVector.resize(DetectorAngles.size());
    for(unsigned int i=0;i<tempVector.size();i++)
        {
            tempVector[i].resize(PhotonFluxData.size());
            for(unsigned int j=0;j<tempVector[i].size();j++)tempVector[i][j].resize(4);
        }


        for(unsigned int t=0;t<DetectorAngles.size();t++)
        {
            cout<<"Calculate Photon Flux for the Detector under "<< (int) DetectorAngles[t]<< " deg..."<<endl;
            minlist=func.Maching2Doubles(ExperimentalData[t],0,'C',FluxParameter,2,'C');
            for(unsigned int i=0;i<ExperimentalData[t].size();i++)
            {
                int j=minlist[i];
                tempVector[t][i][0]=FluxParameter[j][2];
                tempVector[t][i][1]=FluxParameter[j][3];

                PhotonFluxData[i][0]=FluxParameter[j][2];
                PhotonFluxData[i][1]=FluxParameter[j][3];


                if(DetectorAngles[t] == 90)
                {
                    Area=
                    ExperimentalData[t][i][4]
                    /Parameter_Flux[5]
                    /calcEfficiency[t][i][2]
                    /ICS[j][2]
                    /FluxParameter[j][10]
                    ;
                }

                if(DetectorAngles[t] == 130)
                {
                    Area=
                    ExperimentalData[t][i][4]
                    /Parameter_Flux[5]
                    /calcEfficiency[t][i][2]
                    /ICS[j][2]
                    /FluxParameter[j][11]
                    ;
                }


                dArea=Area*
                sqrt(
                    func.relError2(Parameter_Flux,5,6)+
                    func.relError2(ICS[j],2,3)
                    +func.relError2(calcEfficiency[t][i],2,3)
                    );
                
                tempVector[t][i][2]=Area;
                tempVector[t][i][3]=dArea;
                // cout<<std::setw(15)<<FluxParameter[j][2]<<std::setw(15)<<FluxParameter[j][3]<<std::setw(15)<<Area<<std::setw(15)<<dArea<<endl;
                for(unsigned k=0;k<tempVector[t][i].size();k++)cout<<std::setw(15)<<tempVector[t][i][k];
                    cout<<endl;
            }
        }
        cout<<endl<<"Check if the Photonflux from the different Detectors agree within the uncertainty!"<<endl<<endl;
        for(unsigned int i=0;i<tempVector.size();i++)
        {
            for(unsigned int j=0;j<tempVector[i].size();j++)
            {
                PhotonFluxData[j][2]+=tempVector[i][j][2];
                PhotonFluxData[j][3]+=tempVector[i][j][3]*tempVector[i][j][3];
            }
        }
        for(unsigned int i=0;i<PhotonFluxData.size();i++)PhotonFluxData[i][2]=PhotonFluxData[i][2]/ (double) DetectorAngles.size();
        for(unsigned int i=0;i<PhotonFluxData.size();i++)PhotonFluxData[i][3]=sqrt(PhotonFluxData[i][3])/(double) DetectorAngles.size();   
    }   

// ---------------------------------------------------------

    void Flux::CalculateEfficiencyforExperimentalData()
    {
        Functions func;
        vector<double> tempVector;
        vector<vector<double> >temp2DVector;
        unsigned int NumberofParameters;
            NumberofParameters = 6;
            TF1 EfficiencyFunction("EfficiencyFunction",Functions::knoll, 0, 10000,NumberofParameters);

        for(unsigned int t=0;t<DetectorAngles[t];t++)
        {
            double Scale=Parameter_Efficiency[t][0];
            double dScale=Parameter_Efficiency[t][1];
            Parameter_Efficiency[t].erase(Parameter_Efficiency[t].begin()+1);
        for(unsigned int i=0;i<=NumberofParameters;i++){EfficiencyFunction.SetParameter(i,Parameter_Efficiency[t][i]);}
            for(unsigned int i=0;i<ExperimentalData[t].size();i++)
            {
                double val=EfficiencyFunction.Eval(ExperimentalData[t][i][0]);
                double dval=val*sqrt(func.relError2(ExperimentalData[t][i],4,5)+sqrt(dScale*dScale/(Scale*Scale)));
                tempVector.push_back(ExperimentalData[t][i][0]);
                tempVector.push_back(ExperimentalData[t][i][1]);
                tempVector.push_back(val);
                tempVector.push_back(dval);

                temp2DVector.push_back(tempVector);
                tempVector.clear();
            }
            calcEfficiency.push_back(temp2DVector);
            temp2DVector.clear();
        }
    }

// ---------------------------------------------------------

    void Flux::FitPhotonFlux()
    {
        NFit=(unsigned int) Parameter_Flux[0];
        cout<<"Fitting Photo Flux..."<<endl;
        FitParameterDistribution.resize(2);
        FitParameterDistribution[0].resize(NFit);
        FitParameterDistribution[1].resize(NFit);
        Threads.resize(NumberOfThreads);
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
                TThread* Thread = new TThread(Name, (void(*) (void *)) &CallParallelFluxFitThread, (void*) new ThreadCallerFlux(this, t));
                Threads[t] = Thread;

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
            PhotonFluxFitter(0);
        }

        gSystem->Sleep(100);
        // cout<<"All threads have finished"<<endl;
        // for(unsigned int i=0;i<FitParameterDistribution[0].size();i++)cout<<setw(15)<<FitParameterDistribution[0][i]<<setw(15)<<FitParameterDistribution[1][i]<<endl;
        }

// ---------------------------------------------------------

void Flux::CallParallelFluxFitThread(void* Address)
{
  Flux* F = ((ThreadCallerFlux*) Address)->GetThreadCaller();
  F->PhotonFluxFitter(((ThreadCallerFlux*) Address)->GetThreadID());
}

// ---------------------------------------------------------

bool Flux::PhotonFluxFitter(unsigned int NThread)
{
    double Scale, E0;
    string equal="==========";
    string space="          ";
    bool percent[20]={false};
    ThreadIsInitialized[NThread] = true;
    FluxFitFunc FitFunction;


    // possible choices are:
    // Minuit /Minuit2             Migrad, Simplex,Combined,Scan  (default is Migrad)
    //  Minuit2                     Fumili2
    //  Fumili
    //  GSLMultiMin                ConjugateFR, ConjugatePR, BFGS,
    //                              BFGS2, SteepestDescent
    //  GSLMultiFit
    //   GSLSimAn
    //   Genetic

    int datapoints_flux=(int)PhotonFluxData.size();
    Double_t xvec_flux[datapoints_flux];
    Double_t yvec_flux[datapoints_flux];
    TRandom3 random(0);

    ROOT::Fit::Fitter TheFitter;
    TheFitter.Config().SetMinimizer("GSLMultiFit");
    // TheFitter.Config().SetMinimizer("Minuit2","Combined");
    TheFitter.SetFunction(FitFunction);
    TheFitter.Config().MinimizerOptions().SetPrintLevel(0);
    for(unsigned int i=0;i<NFit/NumberOfThreads;i++)
    {

        for(unsigned int j=0;j<PhotonFluxData.size();j++)
        {
            xvec_flux[j]=random.Gaus(PhotonFluxData[j][0],PhotonFluxData[j][1]);
            yvec_flux[j]=random.Gaus(PhotonFluxData[j][2],PhotonFluxData[j][3]);
        }
        TGraph DataGraph((const Int_t)datapoints_flux,xvec_flux,yvec_flux);
        for(unsigned int j=1;j<11;j++)
        {
            if(j*NFit/10/NumberOfThreads==i+1 && percent[j]==false)
            {
                cout<<"Thread "<<NThread<<": ["<<equal.substr(0,j)<<space.substr(j,10)<<"] "<< j <<"0%"<<endl;
                percent[j]=true;
            }
        }

        TThread::Lock();
        unsigned int ID = i;
        // cout<<i<<endl;
        TThread::UnLock();

        ROOT::Fit::BinData d;
        ROOT::Fit::FillData(d,&DataGraph); 
        TheFitter.Config().ParSettings(0).SetName("Scale");
        TheFitter.Config().ParSettings(0).SetValue(Parameter_Flux[1]);
        TheFitter.Config().ParSettings(1).SetName("Endpoint");
        TheFitter.Config().ParSettings(1).SetValue(Parameter_Flux[2]*0.5+Parameter_Flux[3]*0.5);
        TheFitter.Config().ParSettings(1).SetLimits(Parameter_Flux[2],Parameter_Flux[3]);
        TheFitter.Config().ParSettings(2).SetName("Z");
        TheFitter.Config().ParSettings(2).SetValue(Parameter_Flux[4]);
        TheFitter.Config().ParSettings(2).Fix();
        
        bool ReturnCode = false;
        ReturnCode = TheFitter.Fit(d);
        if(ReturnCode == true)
        {
            Scale = TheFitter.Result().Parameter(0);
            E0    = TheFitter.Result().Parameter(1);
            // TThread::Printf("Thread %i: Results E0= %.5f     Scale= %.5f    Nr: %i", NThread, E0,Scale,ID);
            // TThread::Printf("Thread %i: ID=%i     i=%i",NThread,ID,ID+NumberOfFits*NThread);


        }
        TThread::Lock();
            if(0<Scale && Parameter_Flux[2]*1.01<E0 && E0<0.99*Parameter_Flux[3])
            {
                    FitParameterDistribution[0][ID+NFit/NumberOfThreads*NThread]=Scale;
                    FitParameterDistribution[1][ID+NFit/NumberOfThreads*NThread]=E0;
            }
            else
            {
                --i;
            }
        TThread::UnLock();
    }
    ThreadIsFinished[NThread]=true;
    return true;
}

// ---------------------------------------------------------

void Flux::PlotPhotonFlux()
{
    TFile* RFile=TFile::Open(rfile.c_str(),"update");
    RFile->cd("Flux");
    TF1 Schiff("Schiff-Formula",Functions::Schiff, 0, EndPointParameter[0]*ENDPOINT*1000,3);
    int datapoints_flux=(int) PhotonFluxData.size();
    Double_t xvec_flux[datapoints_flux];
    Double_t dxvec_flux[datapoints_flux];
    Double_t yvec_flux[datapoints_flux];
    Double_t dyvec_flux[datapoints_flux];


    for(unsigned int i=0;i<PhotonFluxData.size();i++)
    {
        xvec_flux[i]=PhotonFluxData[i][0];
        dxvec_flux[i]=PhotonFluxData[i][1];
        yvec_flux[i]=PhotonFluxData[i][2];
        dyvec_flux[i]=PhotonFluxData[i][3];
    }

    TCanvas fluxplot("Photon Flux Plot","Photon Flux Plot", 1600,900);
    fluxplot.SetGrid();
    fluxplot.SetLogy();
    fluxplot.GetFrame()->SetFillColor(21);
    fluxplot.GetFrame()->SetBorderSize(12);
    TGraphErrors fluxdata(datapoints_flux,xvec_flux,yvec_flux,dxvec_flux,dyvec_flux);
    fluxdata.SetMarkerStyle(20);
    fluxdata.SetMarkerColor(COLOR_FLUX);
    fluxdata.SetMarkerSize(1);
    fluxdata.SetName("Photo Flux Data");
    fluxdata.SetFillStyle(0);
    fluxdata.SetMaximum(YSCALE*TMath::MaxElement(datapoints_flux,yvec_flux));
    fluxdata.SetMinimum(1e-5);
    fluxdata.GetYaxis()->SetNdivisions(505);   //Sets number of Ticks
    fluxdata.GetYaxis()->SetDecimals(kTRUE); //Sets same digits
    
    Schiff.SetLineWidth(1);
    Schiff.FixParameter(0,ScaleParameter[0]);
    Schiff.FixParameter(1,EndPointParameter[0]);
    Schiff.FixParameter(2,Parameter_Flux[4]);
    Schiff.SetLineColor(COLOR_FIT);
    Schiff.Write();
    
    TMultiGraph *mg = new TMultiGraph();
    string title="Photon Flux fitted via Schiff-Formula;Energy in keV;Photo Flux in a.u.";
    mg->Add(&fluxdata);
    mg->SetTitle(title.c_str());
    mg->Draw("AP");
    Schiff.Draw("same");
    
    Output out;
    out.SetDate();
    string str="Photon_Flux_Fit_Plot_";
    str.append(FileName);
    str.append("_");
    str.append(out.GetDate());
    str.append(".pdf");
    str="Output/"+str;
    fluxplot.SaveAs(str.c_str());
    cout<<"Photonflux Plot saved. ( "<<str<<" )"<<endl;
    
    fluxdata.Write();
    fluxplot.Write();
    RFile->Write();
    // RFile->Close();
}

// ---------------------------------------------------------

void Flux::PlotFitParameters()
{
    TFile* RFile=TFile::Open(rfile.c_str(),"update");
    RFile->cd("Flux");
    double xlow=TMath::MinElement(NFit,&FitParameterDistribution[0][0]);
    double xhi=TMath::MaxElement(NFit,&FitParameterDistribution[0][0]);
    int numberofdigits=(int)pow(10,fabs(ceil(log10(FitParameterDistribution[0][0]))));
    xlow=xlow*numberofdigits;
    xlow=floor(xlow);
    xlow=xlow/numberofdigits;
    
    xhi=xhi*numberofdigits;
    xhi=ceil(xhi);
    xhi=xhi/numberofdigits;

    TH1D ScalingParameter("Scaling Parameter", "Scaling Parameter Distribution",100,xlow,xhi);
    for(unsigned int i=0;i<FitParameterDistribution[0].size();i++)ScalingParameter.Fill(FitParameterDistribution[0][i]);
    TF1 ParameterFuncGaus("Parameter Gaus",Functions::SkewNormal,xlow,xhi,4);
    ParameterFuncGaus.FixParameter(1,ScalingParameter.GetMean(1));
    ParameterFuncGaus.FixParameter(2,ScalingParameter.GetStdDev(1));
    ParameterFuncGaus.FixParameter(3,0);
    ScalingParameter.Fit("Parameter Gaus","Q");
    ScaleParameter.push_back(ScalingParameter.GetMean(1));
    ScaleParameter.push_back(ScalingParameter.GetStdDev(1));
    ParameterFuncGaus.Write();

    TCanvas ParameterHistScale("Scaling Parameter Histogram","Scaling Parameter Distribution",1600,900);
    ParameterHistScale.SetGrid();
    ParameterHistScale.GetFrame()->SetFillColor(21);
    ParameterHistScale.GetFrame()->SetBorderSize(12);
    ParameterHistScale.Write();

    Output out;
    out.SetDate();
    ScalingParameter.Draw("same");
    ParameterFuncGaus.Draw("same");
    string str="ParameterPlot_Scale_Flux_";
    str.append(FileName);
    str.append("_");
    str.append(out.GetDate());
    str.append(".pdf");
    str="Output/"+str;
    ParameterHistScale.SaveAs(str.c_str());
    cout<<"Distribution of Scale saved. ( "<<str<<" )"<<endl;
    
    for(unsigned int i=0;i<4;i++)ParameterFuncGaus.ReleaseParameter(i);
    
    xlow=Parameter_Flux[2];
    xhi=Parameter_Flux[3];
    
    numberofdigits=(int) pow(10,fabs(ceil(log10(FitParameterDistribution[1][0]))));;
    xlow=xlow*numberofdigits;
    xlow=floor(xlow);
    xlow=xlow/numberofdigits;
    
    xhi=xhi*numberofdigits;
    xhi=ceil(xhi);
    xhi=xhi/numberofdigits;
    
    TH1D Endpoint("End Point", "End Point Distribution",100,xlow,xhi);
    for(unsigned int i=0;i<FitParameterDistribution[1].size();i++)Endpoint.Fill(FitParameterDistribution[1][i]);
    Endpoint.SetXTitle("Endpoint in MeV");

    TCanvas ParameterHistEndpoint("End-Point Parameter Histogram","End-Point Parameter Distribution",1600,900);
    ParameterHistEndpoint.SetGrid();
    ParameterHistEndpoint.GetFrame()->SetFillColor(21);
    ParameterHistEndpoint.GetFrame()->SetBorderSize(12);
    Endpoint.Draw("same");

    str="ParameterPlot_EndPoint_Flux_";
    str.append(FileName);
    str.append("_");
    str.append(out.GetDate());
    str.append(".pdf");
    str="Output/"+str;
    ParameterHistEndpoint.SaveAs(str.c_str());
    cout<<"Distribution of End-Point saved. ( "<<str<<" )"<<endl;
    
    EndPointParameter.push_back(10);

    ParameterHistEndpoint.Write();

    RFile->Write();
    // RFile->Close();
}