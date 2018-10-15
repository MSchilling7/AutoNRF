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
    Percent(),
    CalibrationData(),
    calcEfficiency(),
    ICS(),
    FluxParameter(),
    PhotonFluxData(),
    FitParameterDistribution(),
    Parameter_Flux(),
    DetectorAngles(),
    ScaleParameter(),
    EndPointParameter(),
    FittedParameters(),
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
    vector<double> tempVector;
    vector< vector <double> > temp2DVector, temp2DVector2;
    vector <unsigned int> minlist,minlist2,minlist3;
    vector<vector <double> > Inelastic,Elastic;
    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {

        double temp=0;

    // Get the Gamma-Ray Energy from Input Parameter File for CalibrationData File
        minlist=Functions::Maching2Doubles(CalibrationData[t],0,'C',FluxParameter,E_GAMMA,'C');
        for(unsigned int i=0;i<CalibrationData[t].size();i++){
            CalibrationData[t][i][0]=FluxParameter[minlist[i]][E_GAMMA];
            
        }
        minlist.clear();
    // Get an Array of Inelastic Transitions from Input file
        for(unsigned int i=0;i<FluxParameter.size();i++)
        {
            if(FluxParameter[i][E_FINAL]!=0)Inelastic.push_back(FluxParameter[i]);
            if(FluxParameter[i][E_FINAL]==0)Elastic.push_back(FluxParameter[i]);
        };
    // Clean Input Data File from Inelastic Transitions
        for(unsigned int i=0;i<CalibrationData[t].size();i++)
        {
            for(unsigned int j=0;j<FluxParameter.size();j++)
            {
                if(temp!=FluxParameter[j][EX])
                {
                    temp=FluxParameter[j][EX];
                    if(CalibrationData[t][i][0]==FluxParameter[j][E_GAMMA])
                    {
                        minlist.push_back(i);
                    }
                }
            }
        }
        for(unsigned int i=0;i<minlist.size();i++)
        {
            temp2DVector.push_back(calcEfficiency[t][minlist[i]]);
            temp2DVector2.push_back(CalibrationData[t][minlist[i]]);
        }
        CalibrationData[t]=temp2DVector2;
        calcEfficiency[t]=temp2DVector;
        temp2DVector.clear();
        temp2DVector2.clear();
        minlist.clear();
        
        minlist=Functions::Maching2Doubles(CalibrationData[t],0,'C',Elastic,EX,'C');
        for(unsigned int i=0;i<CalibrationData[t].size();i++)
        {
            if(Elastic[minlist[i]][E_GAMMA]==CalibrationData[t][i][0])
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
                    Inelastic[j][DBRANCHING]=Inelastic[j][BRANCHING]/Elastic[i][BRANCHING]*sqrt(Functions::relError2(Inelastic[j],BRANCHING,DBRANCHING)+Functions::relError2(Elastic[i],BRANCHING,DBRANCHING));
                    
                }
            }
        }
        minlist=Functions::Maching2Doubles(Inelastic,EX,'C',CalibrationData[t],0,'C');
        minlist2=Functions::Maching2Doubles(Inelastic,E_FINAL,'C',CalibrationData[t],0,'C');
        minlist3=Functions::Maching2Doubles(Inelastic,E_FINAL,'C',Elastic,EX,'C');
    // Feeding Correction of Peak Areas (Top-Down)
        double Volume=0, dVolume=0;
        for(unsigned int i=0;i<Inelastic.size();i++)
        {
            if(DetectorAngles[t] == 90)Volume=Inelastic[i][BRANCHING]*calcEfficiency[t][minlist2[i]][2]/calcEfficiency[t][minlist[i]][2]*Inelastic[i][W90]/Elastic[minlist3[i]][W90]*CalibrationData[t][minlist[i]][4];
            if(DetectorAngles[t] == 130)Volume=Inelastic[i][BRANCHING]*calcEfficiency[t][minlist2[i]][2]/calcEfficiency[t][minlist[i]][2]*Inelastic[i][W130]/Elastic[minlist3[i]][W130]*CalibrationData[t][minlist[i]][4];
            
            dVolume=Volume*sqrt
            (
                Functions::relError2(calcEfficiency[t][minlist2[i]],2,3)+
                Functions::relError2(calcEfficiency[t][minlist[i]],2,3)+
                Functions::relError2(CalibrationData[t][minlist[i]],4,5)+
                Functions::relError2(Inelastic[i],BRANCHING,DBRANCHING)
                );
            Inelastic[i][CORRECT_VOLUME]=Volume;
            Inelastic[i][CORRECT_DVOLUME]=dVolume;
        }
        for(unsigned int i=(unsigned int)Inelastic.size();0<i;--i)
        {
            unsigned int j=i-1;
            Volume=CalibrationData[t][minlist2[j]][4]-Inelastic[j][CORRECT_VOLUME];
            dVolume=sqrt(CalibrationData[t][minlist2[j]][5]*CalibrationData[t][minlist2[j]][5]+Inelastic[j][CORRECT_DVOLUME]*Inelastic[j][CORRECT_DVOLUME]);
            
            CalibrationData[t][minlist2[j]][4]=Volume;
            CalibrationData[t][minlist2[j]][5]=dVolume;
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
    vector<double> tempVector;
    double hqc= 197.3269788*1000;//keV fm

    for(unsigned int i=0;i<FluxParameter.size();i++){
        tempVector.push_back(FluxParameter[i][2]);
        tempVector.push_back(FluxParameter[i][3]);
        
        double Energy=FluxParameter[i][2];//keV
        double J1=FluxParameter[i][4];
        double J0=FluxParameter[i][5];
        double Gamma=FluxParameter[i][6]/1000;//keV
        double Branching=FluxParameter[i][8];
        
        tempVector.push_back(TMath::Pi()*TMath::Pi()*((2*J1+1)/(2*J0+1))*(hqc/Energy)*(hqc/Energy)*Gamma*Branching);

        tempVector.push_back(tempVector[2]*sqrt(Functions::relError2(FluxParameter[i],6,7)+Functions::relError2(FluxParameter[i],8,9)));
        ICS.push_back(tempVector);
        tempVector.clear();
    }
    
};

// ---------------------------------------------------------

void Flux::CalculateFlux()
{
    double NT=Parameter_Flux[7]*1.660539*1e-24;
    vector<vector<vector<double> > >tempVector;
    vector <unsigned int> minlist;
    double Area;
    double dAreaLow,dAreaHigh;
    PhotonFluxData.resize(CalibrationData[0].size());
    for(unsigned int i=0;i<PhotonFluxData.size();i++)PhotonFluxData[i].resize(5);
    tempVector.resize(DetectorAngles.size());
    for(unsigned int i=0;i<tempVector.size();i++)
        {
            tempVector[i].resize(PhotonFluxData.size());
            for(unsigned int j=0;j<tempVector[i].size();j++)tempVector[i][j].resize(5);
        }


        for(unsigned int t=0;t<DetectorAngles.size();t++)
        {
            cout<<"Calculate Photon Flux for the Detector under "<< (int) DetectorAngles[t]<< " deg..."<<endl;
            minlist=Functions::Maching2Doubles(CalibrationData[t],0,'C',FluxParameter,2,'C');
            for(unsigned int i=0;i<CalibrationData[t].size();i++)
            {
                int j=minlist[i];
                tempVector[t][i][0]=FluxParameter[j][2];
                tempVector[t][i][1]=FluxParameter[j][3];

                PhotonFluxData[i][0]=FluxParameter[j][2];
                PhotonFluxData[i][1]=FluxParameter[j][3];


                    Area=
                    CalibrationData[t][i][4]
                    /Parameter_Flux[5]
                    /calcEfficiency[t][i][2]
                    /ICS[j][2]
                    /FluxParameter[j][10+t]*NT
                    ;

                dAreaLow=Area*
                sqrt(
                    Functions::relError2(Parameter_Flux,5,6)+
                    Functions::relError2(ICS[j],2,3)+
                    Functions::relError2(calcEfficiency[t][i],2,3)+
                    Functions::relError2(CalibrationData[t][i],4,5)
                    );

                dAreaHigh=Area*
                sqrt(
                    Functions::relError2(Parameter_Flux,5,6)+
                    Functions::relError2(ICS[j],2,3)+
                    Functions::relError2(calcEfficiency[t][i],2,4)+
                    Functions::relError2(CalibrationData[t][i],4,5)
                    );
                
                tempVector[t][i][2]=Area;
                tempVector[t][i][3]=dAreaLow;
                tempVector[t][i][4]=dAreaHigh;
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
                PhotonFluxData[j][4]+=tempVector[i][j][4]*tempVector[i][j][4];
            }
        }
        for(unsigned int i=0;i<PhotonFluxData.size();i++)PhotonFluxData[i][2]=PhotonFluxData[i][2]/ (double) DetectorAngles.size();
        for(unsigned int i=0;i<PhotonFluxData.size();i++)PhotonFluxData[i][3]=sqrt(PhotonFluxData[i][3])/(double) DetectorAngles.size();   
        for(unsigned int i=0;i<PhotonFluxData.size();i++)PhotonFluxData[i][4]=sqrt(PhotonFluxData[i][4])/(double) DetectorAngles.size();   
    }   

// ---------------------------------------------------------

    void Flux::CalculateEfficiencyforCalibrationData()
    {
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
        for(unsigned int t=0;t<DetectorAngles.size();t++)
        {
            ScaleLow=Parameter_Efficiency[t][1];
            ScaleHigh=Parameter_Efficiency[t][2];
            Parameter_Efficiency[t].erase(Parameter_Efficiency[t].begin()+1);
            Parameter_Efficiency[t].erase(Parameter_Efficiency[t].begin()+1);
            for(unsigned int i=0;i<=NumberofParameters;i++){EfficiencyFunction.SetParameter(i,Parameter_Efficiency[t][i]);}
            for(unsigned int i=1;i<=NumberofParameters;i++){EfficiencyFunctionLOW.SetParameter(i,Parameter_Efficiency[t][i]);}
            for(unsigned int i=1;i<=NumberofParameters;i++){EfficiencyFunctionHIGH.SetParameter(i,Parameter_Efficiency[t][i]);}
            EfficiencyFunctionLOW.SetParameter(0,ScaleLow);
            EfficiencyFunctionHIGH.SetParameter(0,ScaleHigh);

            for(unsigned int i=0;i<CalibrationData[t].size();i++)
            {
                if(t==0)val=EfficiencyFunction.Eval(CalibrationData[t][i][0]-93.7859);
                if(t==1)val=EfficiencyFunction.Eval(CalibrationData[t][i][0]-86.8684);
                vallow=fabs(val-EfficiencyFunctionLOW.Eval(CalibrationData[t][i][0]));
                vallow=vallow*vallow/(val*val)+Functions::relError2(CalibrationData[t][i],4,5);
                vallow=val*sqrt(vallow);
                valhigh=fabs(val-EfficiencyFunctionHIGH.Eval(CalibrationData[t][i][0]));
                valhigh=valhigh*valhigh/(val*val)+Functions::relError2(CalibrationData[t][i],4,5);
                valhigh=val*sqrt(valhigh);
                tempVector.push_back(CalibrationData[t][i][0]);
                tempVector.push_back(CalibrationData[t][i][1]);
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

    void Flux::FitPhotonFlux()
    {
        NFit=(unsigned int) Parameter_Flux[0];
        cout<<"Fitting Photo Flux..."<<endl;
        FitParameterDistribution.resize(2);
        FitParameterDistribution[0].resize(NFit);
        FitParameterDistribution[1].resize(NFit);
        Threads.resize(NumberOfThreads);
        for(unsigned int i = 0 ; i<NumberOfThreads ; i++)Percent.push_back(0);
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
        cout<<endl;
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
    double NT=Parameter_Flux[7]*1.660539*1e-24;
    double Scale=0, E0=0;
    bool percent[128]={false};
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

    TF1 rand_func[PhotonFluxData.size()];
    for(unsigned int j=0;j<PhotonFluxData.size();j++)
    {
        rand_func[j]=TF1("",Functions::Normal2,0.5*(PhotonFluxData[j][2]-PhotonFluxData[j][3]),1.5*(PhotonFluxData[j][2]+PhotonFluxData[j][4]),3);
        rand_func[j].SetParameters(PhotonFluxData[j][2],PhotonFluxData[j][3],PhotonFluxData[j][4]);            
    }
    for(unsigned int i=0;i<NFit/NumberOfThreads;i++)
    {

        for(unsigned int j=0;j<PhotonFluxData.size();j++)
        {
            xvec_flux[j]=random.Gaus(PhotonFluxData[j][0],PhotonFluxData[j][1]);
            yvec_flux[j]=rand_func[j].GetRandom();
            yvec_flux[j]=yvec_flux[j]/NT;
        }
        // for (int cc = 0; cc <PhotonFluxData.size(); ++cc)
        // {
            // cout<<std::setw(15)<<yvec_flux[cc]<<endl;
        // }
        // cout<<endl;
        TGraph DataGraph((const Int_t)datapoints_flux,xvec_flux,yvec_flux);
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
                        for(unsigned int p=0;p<NumberOfThreads;p++)
                            {
                                cout<<"Thread "<<p<<": "<<Percent[p]<<"%\t";
                            }
                            cout<<"Total: "<<Percent[Percent.size()-1]<<"%";
                            cout<<std::flush;
                        percent[j]=true;
                        break;
                    }
                }
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
                    FitParameterDistribution[0][ID+NFit/NumberOfThreads*NThread]=Scale*NT;
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
    TF1 Schiff("Schiff-Formula",Functions::Schiff, EMIN, EndPointParameter[0]*1000,3);
    TF1 SchiffDOWN("Schiff-Formula-Down",Functions::Schiff, EMIN, EndPointParameter[1]*1000,3);
    TF1 SchiffUP("Schiff-Formula-Up",Functions::Schiff, EMIN, EndPointParameter[2]*1000,3);
    int datapoints_flux=(int) PhotonFluxData.size()+2;
    Double_t xvec_flux[datapoints_flux];
    Double_t dxvec_flux[datapoints_flux];
    Double_t yvec_flux[datapoints_flux];
    Double_t ddownyvec_flux[datapoints_flux];
    Double_t dupyvec_flux[datapoints_flux];



    xvec_flux[datapoints_flux-1]=0;
    dxvec_flux[datapoints_flux-1]=0;
    yvec_flux[datapoints_flux-1]=0;
    dupyvec_flux[datapoints_flux-1]=0;
    ddownyvec_flux[datapoints_flux-1]=0;

    
    xvec_flux[datapoints_flux-2]=20000;
    dxvec_flux[datapoints_flux-2]=0;
    yvec_flux[datapoints_flux-2]=1e50;
    dupyvec_flux[2]=0;
    ddownyvec_flux[2]=0;

    for(unsigned int i=0;i<PhotonFluxData.size();i++)
    {
        xvec_flux[i]=PhotonFluxData[i][0];
        dxvec_flux[i]=PhotonFluxData[i][1];
        yvec_flux[i]=PhotonFluxData[i][2];
        ddownyvec_flux[i]=PhotonFluxData[i][3];
        dupyvec_flux[i]=PhotonFluxData[i][4];
        // cout<<std::setw(15)<<yvec_flux[i]<<std::setw(15)<<ddownyvec_flux[i]<<std::setw(15)<<dupyvec_flux[i]<<endl;
    }

    TCanvas fluxplot("Photon Flux Plot","Photon Flux Plot", 1600,900);
    fluxplot.SetGrid();
    fluxplot.SetLogy();
    fluxplot.GetFrame()->SetFillColor(21);
    fluxplot.GetFrame()->SetBorderSize(12);
    TGraphAsymmErrors fluxdata(datapoints_flux,xvec_flux,yvec_flux,0,0,ddownyvec_flux,dupyvec_flux);
    // fluxdata.SetMarkerStyle(20);
    fluxdata.SetMarkerStyle(6);
    fluxdata.SetMarkerColor(COLOR_FLUX);
    fluxdata.SetMarkerSize(1);
    fluxdata.SetName("Photo Flux Data");
    fluxdata.SetFillStyle(0);
    fluxdata.GetYaxis()->SetNdivisions(505);   //Sets number of Ticks
    fluxdata.GetYaxis()->SetDecimals(kTRUE); //Sets same digits



    Schiff.SetLineWidth(1);
    Schiff.FixParameter(0,ScaleParameter[0]);
    Schiff.FixParameter(1,EndPointParameter[0]);
    Schiff.FixParameter(2,Parameter_Flux[4]);
    Schiff.SetLineColor(COLOR_FIT);
    Schiff.Write();

    SchiffUP.SetLineWidth(1);
    SchiffUP.FixParameter(0,ScaleParameter[2]);
    SchiffUP.FixParameter(1,EndPointParameter[2]);
    SchiffUP.FixParameter(2,Parameter_Flux[4]);
    SchiffUP.SetLineColor(COLOR_DFIT);
    SchiffUP.SetLineStyle(7);
    SchiffUP.Write();

    SchiffDOWN.SetLineWidth(1);
    SchiffDOWN.FixParameter(0,ScaleParameter[1]);
    SchiffDOWN.FixParameter(1,EndPointParameter[1]);
    SchiffDOWN.FixParameter(2,Parameter_Flux[4]);
    SchiffDOWN.SetLineColor(COLOR_DFIT);
    SchiffDOWN.SetLineStyle(7);
    SchiffDOWN.Write();

    fluxdata.GetXaxis()->SetRangeUser(EMIN,SchiffUP.GetXmax());
    fluxdata.GetYaxis()->SetRangeUser(yvec_flux[datapoints_flux-3]*0.2,yvec_flux[0]*2);

    fluxdata.SetTitle("Photon Flux fitted via Schiff-Formula;Energy in keV;Photon Flux in a.u.");

    fluxdata.Draw("AP");
    Schiff.Draw("same");
    SchiffDOWN.Draw("same");
    SchiffUP.Draw("same");
    
    std::reverse(FileName.begin(),FileName.end());
    stringstream ssFile(FileName);
    FileName="";
    getline(ssFile,FileName,'/');
    std::reverse(FileName.begin(),FileName.end());

    string str=Output::dir+"Flux/Flux_Fit_";
    str.append(FileName);
    str.append(".pdf");
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
    // double NT=Parameter_Flux[7]*1.660539*1e-24;
    // for(unsigned int i=0;i<FitParameterDistribution[0].size();i++)FitParameterDistribution[0][i]=FitParameterDistribution[0][i]*NT;
    double xlow=TMath::MinElement(NFit,&FitParameterDistribution[0][0]);
    double xhi=TMath::MaxElement(NFit,&FitParameterDistribution[0][0]);
    double numberofdigits=pow(10,fabs(ceil(log10(xlow))));
    xlow=xlow*numberofdigits;
    xlow=floor(xlow);
    xlow=xlow/numberofdigits;
    
    xhi=xhi*numberofdigits;
    xhi=ceil(xhi);
    xhi=xhi/numberofdigits;
    TCanvas ParameterHistScale("Scaling Parameter Histogram","Scaling Parameter Distribution",1600,900);
    ParameterHistScale.SetGrid();
    ParameterHistScale.GetFrame()->SetFillColor(21);
    ParameterHistScale.GetFrame()->SetBorderSize(12);

    TH1D ScalingParameter("Scaling_Parameter", "Scaling Parameter Distribution",100,xlow,xhi);
    for(unsigned int i=0;i<FitParameterDistribution[0].size();i++)ScalingParameter.Fill(FitParameterDistribution[0][i]);
        // cout<<endl;
    // for(unsigned int i=0;i<FitParameterDistribution[0].size();i++)cout<<FitParameterDistribution[1][i]<<endl;

    double max=ScalingParameter.GetMaximum();
    ScalingParameter.SetMaximum(YSCALE*max);
    double boundary[2]={0,0};
    Functions::ShortestCoverage(FitParameterDistribution[0],boundary);
    TLine b_lower(boundary[0],0.,boundary[0],ScalingParameter.GetMaximum());
    TLine b_upper(boundary[1],0.,boundary[1],ScalingParameter.GetMaximum());
    b_lower.SetLineColor(2);
    b_lower.SetLineWidth(2);
    b_upper.SetLineColor(2);
    b_upper.SetLineWidth(2);
    ScalingParameter.Draw("");
    b_lower.Draw("same");
    b_upper.Draw("same");
    ScaleParameter.push_back(ScalingParameter.GetBinCenter(ScalingParameter.GetMaximumBin()));
    ScaleParameter.push_back(boundary[0]);
    ScaleParameter.push_back(boundary[1]);

    std::reverse(FileName.begin(),FileName.end());
    stringstream ssFile(FileName);
    FileName="";
    getline(ssFile,FileName,'/');
    std::reverse(FileName.begin(),FileName.end());

    ParameterHistScale.Write();
    string str=Output::dir+"Flux/Flux_Scale_";
    str.append(FileName);
    str.append(".pdf");
    ParameterHistScale.SaveAs(str.c_str());
    cout<<"Distribution of Scale saved. ( "<<str<<" )"<<endl;
    
    xlow=Parameter_Flux[2];
    xhi=Parameter_Flux[3];
    
    // numberofdigits=(int) pow(10,fabs(ceil(log10(FitParameterDistribution[1][0]))));;
    // xlow=xlow*numberofdigits;
    // xlow=floor(xlow);
    // xlow=xlow/numberofdigits;
    
    xhi=xhi*numberofdigits;
    xhi=ceil(xhi);
    xhi=xhi/numberofdigits;
    TCanvas ParameterHistEndpoint("End-Point Parameter Histogram","End-Point Parameter Distribution",1600,900);
    ParameterHistEndpoint.SetGrid();
    ParameterHistEndpoint.GetFrame()->SetFillColor(21);
    ParameterHistEndpoint.GetFrame()->SetBorderSize(12);
    
    TH1D Endpoint("End_Point", "End Point Distribution",100,xlow,xhi);
    for(unsigned int i=0;i<FitParameterDistribution[1].size();i++)Endpoint.Fill(FitParameterDistribution[1][i]);
    Endpoint.SetXTitle("Endpoint in MeV");

    max=Endpoint.GetMaximum();
    Endpoint.SetMaximum(YSCALE*max);
    Functions::ShortestCoverage(FitParameterDistribution[1],boundary);
    TLine c_lower(boundary[0],0.,boundary[0],Endpoint.GetMaximum());
    TLine c_upper(boundary[1],0.,boundary[1],Endpoint.GetMaximum());
    TLine c_mid(Endpoint.GetBinCenter(Endpoint.GetMaximumBin()),0.,Endpoint.GetBinCenter(Endpoint.GetMaximumBin()),Endpoint.GetMaximum());
    c_lower.SetLineColor(2);
    c_lower.SetLineWidth(2);
    c_upper.SetLineColor(2);
    c_upper.SetLineWidth(2);
    c_mid.SetLineColor(1);
    c_mid.SetLineWidth(2);
    Endpoint.Draw("");
    c_lower.Draw("same");
    c_upper.Draw("same");
    c_mid.Draw("same");

    Endpoint.Draw("same");
    EndPointParameter.push_back(Endpoint.GetBinCenter(Endpoint.GetMaximumBin()));
    EndPointParameter.push_back(boundary[0]);
    EndPointParameter.push_back(boundary[1]);

    str=Output::dir+"Flux/Flux_EndPoint_";
    str.append(FileName);
    str.append(".pdf");
    ParameterHistEndpoint.SaveAs(str.c_str());
    cout<<"Distribution of End-Point saved. ( "<<str<<" )"<<endl;
    
    ParameterHistEndpoint.Write();

    RFile->Write();
    // RFile->Close();
}
// ---------------------------------------------------------

vector<double> Flux::GetFittedParameters()
{
    FittedParameters.insert(FittedParameters.end(),ScaleParameter.begin(),ScaleParameter.end());
    FittedParameters.insert(FittedParameters.end(),EndPointParameter.begin(),EndPointParameter.end());
    FittedParameters.push_back(Parameter_Flux[4]);
    return FittedParameters;
}
