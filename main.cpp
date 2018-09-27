/**
 * @file main.cpp
 * @author z 2
 * @date 06-06-2017
 * @brief Main cpp for the project
 */

#include<getopt.h>
#include<TFile.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include"DataReader.h"
#include"Efficiency.h"
#include"Flux.h"
#include"Gamma.h"

#include "Functions.h"
#include "Output.h"





int main(int argc, char **argv)
{

  using namespace std::chrono;
  using std::endl;
  using std::cout;
  using std::setw;
  using std::string;
  
  bool input        = false;
  bool rootbool     = false;
  bool threadbool   = false;
  bool dirbool      = false;
  int c           = 0;
  
  string InputFile ="";
  string rfile="";
  string rootfile="";
  string dirname="";

  
  unsigned int TNumber=1;
  
  while (c > -1)
  {
      static struct option long_options[] =
      {
          {"input",                  required_argument,       0, 'i'},
          {"thread",                 required_argument,       0, 't'},
          {"root",                   required_argument,       0, 'r'},
          {"dir",                    required_argument,       0, 'd'},
          {"help",                   no_argument,             0, 'h'},
          {"help",                   no_argument,             0, '?'}
      };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "i:t:r:d:h:?",
         long_options, &option_index);

      switch (c)
      {
        case 0:
        
        break;

        case 'i':
        InputFile=optarg;
        input=true;
        break;
        
        case 't':
        TNumber=atoi(optarg);
        threadbool=true;
        break;            
        
        case 'r':
        rfile=optarg;
        rootbool=true;
        break;

        case 'd':
        dirname=optarg;
        dirbool=true;
        break;

        case 'h':
        case '?':
        cout << "                       NRF with ROOT " << endl;
        cout << "Usage: ./AutoNRF [OPTIONS] file" << endl;
        cout << "Informations: ./AutoNRF -? or ./AutoNRF -h or ./AutoNRF --help" << endl;
        cout << endl;
        cout << endl;
        cout << "Options:" << endl;
        cout << "-i, --input" <<endl;
        cout << "-t, --thread" << endl;
        cout << "-d, --dir" << endl;
        cout << "========================================================================" << endl;
        cout << "The Data should be in a propper way." << endl;
        cout << "There is no problems for , or ; for inbetween." <<endl;
        cout << endl;
        cout << "   Energy   ΔEnergy   FWHM   EFWHM   Area   ΔArea   TailL   ΔTailL" << endl;
        cout << endl;
        cout << "========================================================================" << endl;
        cout << "#3\tNumber of Fits that should be done (Baysian Aproach)" << endl;
        cout << "#1\tPath to Peakfile" << endl;
        cout << "#4\tStarting Value for a0" << endl;
        cout << "#5\tStarting Value for a1" << endl;
        cout << "#6\tStarting Value for a2" << endl;
        cout << "#7\tStarting Value for a3" << endl;
        cout << "#8\tStarting Value for a4" << endl;
        cout << "#9\tStarting Value for a5" << endl;
        cout << "========================================================================" << endl;
        return 0;
        break;


        default:
        break;
    }
}
gErrorIgnoreLevel = 1001;//Supress ROOT Info output
Output out;
out.SetDate();

high_resolution_clock::time_point start = high_resolution_clock::now();
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
// // // 
// // // Read Data
// // // 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //
vector <vector <string> > DataFileArray(1,vector<string>(1,""));
vector<vector<double> > FitParameterArray_Efficency(1,vector<double>(1,1));
vector<vector<double> > FitParameterArray_Flux(1,vector<double>(1,1));
vector<double> DetectorAngles(1,1);
vector<double> ECalTime(1,1);

struct stat st = {}; //Struct to check if Directory "Output" exists
if (stat("Output", &st) == -1)
{
    mkdir("Output", 0777);
    cout<<"Created directory /Output/"<<endl<<endl;
}
dirname="Output/"+dirname;
if(!dirbool)dirname="Output/"+out.GetDate();

if (stat(dirname.c_str(), &st) == -1)
{
    mkdir(dirname.c_str(), 0777);
    cout<<"Created directory /"<<dirname.c_str()<<"/"<<endl<<endl;
}
string folder=dirname;
folder+="/Efficiency";
if (stat(folder.c_str(), &st) == -1)
{
    mkdir(folder.c_str(), 0777);
    cout<<"Created directory /"<<folder.c_str()<<"/"<<endl<<endl;
}
folder=dirname;
folder+="/Flux";
if (stat(folder.c_str(), &st) == -1)
{
    mkdir(folder.c_str(), 0777);
    cout<<"Created directory /"<<folder.c_str()<<"/"<<endl<<endl;
}
folder=dirname;
folder+="/Results";
if (stat(folder.c_str(), &st) == -1)
{
    mkdir(folder.c_str(), 0777);
    cout<<"Created directory /"<<folder.c_str()<<"/"<<endl<<endl;
}


dirname+="/";
if(!threadbool)TNumber=1;
if(!rootbool)
    {
        rootfile=dirname;
        rootfile+="default.root";
    }
if(rootbool)
    {
        rootfile=dirname;
        rootfile+=rfile;
    }
TFile* RFile=TFile::Open(rootfile.c_str(),"RECREATE");
RFile->mkdir("Efficiency");
RFile->mkdir("Flux");
RFile->mkdir("Results");
RFile->Write();
RFile->Close();
Output::dir=dirname;

DataReader read;
if(input)
{
    
    read.SetInputName(InputFile);
    read.SetInputParameter();
    DataFileArray=read.GetDataFileArray();
    FitParameterArray_Efficency=read.GetFitParameterArray_Efficency();
    FitParameterArray_Flux=read.GetFitParameterArray_Flux();        
    DetectorAngles=read.GetDetectorAnglesArray();
    ECalTime=read.GetECalTimeArray();
}
if(!input)
{
    cout<<"No input file given!"<<endl;exit(0);
}

// // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
// // // 
// // // Loop over Input Data or Using Flag Input
// // // 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //
for(unsigned int i=0;i<DataFileArray.size();i++)
{
    string ISourceDataFile                   = DataFileArray[i][0];
    string IECalibrationDataFile             = DataFileArray[i][1];
    string ISimulationDataFile               = DataFileArray[i][2];
    string IFCalibrationParameterFile        = DataFileArray[i][3];
    string IFCalibrationFluxDataFile         = DataFileArray[i][4];
    string IExperimentalDataFile             = DataFileArray[i][5];
    string IExperimentalAngularDataFile      = DataFileArray[i][6];

    vector<double> Parameter_Efficency(1,1);
    Parameter_Efficency=FitParameterArray_Efficency[i];
    vector<double> TargetMass(1,1);
    vector<double> Parameter_Flux(1,1);
    Parameter_Flux=FitParameterArray_Flux[i];
    vector<vector<double> >  FluxCalibrationData(1,vector<double>(1,1));
    vector<vector<double> >  ExperimentalDataAngular(1,vector<double>(1,1));
    vector<vector<vector<double> > >  SData(1,vector<vector<double> >(1,vector<double>(1,1)));
    vector<vector<vector<double> > >  ECalData(1,vector<vector<double> >(1,vector<double>(1,1)));
    vector<vector<vector<double> > >  SimulationData(1,vector<vector<double> >(1,vector<double>(1,1)));
    vector<vector<vector<double> > >  ExperimentalData(1,vector<vector<double> >(1,vector<double>(1,1)));
    vector<vector<vector<double> > >  CalibrationFluxData(1,vector<vector<double> >(1,vector<double>(1,1)));
    vector<vector<vector<double> > >  CalculatedEfficiency(1,vector<vector<double> >(1,vector<double>(1,1)));
    
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Source Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    
        if(ISourceDataFile!="default" && ISourceDataFile!="DEFAULT")
        {
            read.SetInputName(ISourceDataFile);
            read.SetSourceData();
            SData=read.GetSourceDataArray();
        }
        if(ISourceDataFile=="default" || ISourceDataFile=="DEFAULT")
        {
            read.SetInputName("56CoData.dat");
            ISourceDataFile="56CoData.dat";
            read.SetSourceData();
            SData=read.GetSourceDataArray();
        }
    
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Efficiency Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

    
        if(IECalibrationDataFile!="default" && IECalibrationDataFile!="DEFAULT")
        {
            read.SetInputName(IECalibrationDataFile);
            read.SetECalData();
            ECalData=read.GetECalDataArray();
        }
        if(IECalibrationDataFile=="default" || IECalibrationDataFile=="DEFAULT")
        {
            read.SetInputName("ECalData.dat");
            IECalibrationDataFile="ECalData.dat";
            read.SetECalData();
            ECalData=read.GetExperimentalDataArray();
        }    
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Simulation Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

        if(ISimulationDataFile!="default" && ISimulationDataFile!="DEFAULT")
        {
            read.SetInputName(ISimulationDataFile);
            read.SetSimulationData();
            SimulationData=read.GetSimulationDataArray();
        }
        if(ISimulationDataFile=="default" || ISimulationDataFile=="DEFAULT")
        {
            read.SetInputName("SimulationData.dat");
            ISimulationDataFile="SimulationData.dat";
            read.SetSimulationData();
            SimulationData=read.GetSimulationDataArray();
        }

        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Flux Data Parameter
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

        if(IFCalibrationParameterFile!="default" && IFCalibrationParameterFile!="DEFAULT")
        {
            read.SetInputName(IFCalibrationParameterFile);
            read.SetCalibrationFluxParameter();
            FluxCalibrationData=read.GetCalibrationFluxParameterArray();
        }
        if(IFCalibrationParameterFile=="default" ||IFCalibrationParameterFile=="DEFAULT")
        { 
            read.SetInputName("Bor_Parameter.dat");
            IFCalibrationParameterFile="Bor_Parameter.dat";
            read.SetCalibrationFluxParameter();
            FluxCalibrationData=read.GetCalibrationFluxParameterArray();
        }

        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Flux Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    
        if(IFCalibrationFluxDataFile!="default" && IFCalibrationFluxDataFile!="DEFAULT")
        {
            read.SetInputName(IFCalibrationFluxDataFile);
            read.SetCalibrationFluxData();
            CalibrationFluxData=read.GetCalibrationFluxDataArray();
        }
        if(IFCalibrationFluxDataFile=="default" ||IFCalibrationFluxDataFile=="DEFAULT")
        {
            read.SetInputName("BorPoints.dat");
            IFCalibrationFluxDataFile="BorPoints.dat";
            read.SetCalibrationFluxData();
            CalibrationFluxData=read.GetCalibrationFluxDataArray();
        }

        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Experimental Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    
        if(IExperimentalDataFile!="default" && IExperimentalDataFile!="DEFAULT")
        {
            read.SetInputName(IExperimentalDataFile);
            read.SetExperimentalData();
            ExperimentalData=read.GetExperimentalDataArray();
        }
        if(IExperimentalDataFile=="default" ||IExperimentalDataFile=="DEFAULT")
        {
            read.SetInputName("ExperimentalData.dat");
            IExperimentalDataFile="ExperimentalData.dat";
            read.SetExperimentalData();
            ExperimentalData=read.GetExperimentalDataArray();
        }

        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Experimental Angular Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    
        if(IExperimentalAngularDataFile!="default" && IExperimentalAngularDataFile!="DEFAULT")
        {
            read.SetInputName(IExperimentalAngularDataFile);
            read.SetAngularDistribution();
            ExperimentalDataAngular=read.GetExperimentalDataAngularArray();
        }
        if(IExperimentalAngularDataFile=="default" ||IExperimentalAngularDataFile=="DEFAULT")
        {
            read.SetInputName("ExperimentalDataAngular.dat");
            IExperimentalAngularDataFile="ExperimentalDataAngular.dat";
            read.SetAngularDistribution();
            ExperimentalDataAngular=read.GetExperimentalDataAngularArray();
        }

        TargetMass=read.GetTargetMassArray();

    cout<<"Reading Data Complete!"<<endl;
    // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
    // // // 
    // // // Efficiency Calculation and Fitting
    // // // 
    // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    Efficiency effi;
    effi.SetFileName(IECalibrationDataFile);
    effi.SetRootFile(rootfile);
    effi.SetNThread(TNumber);
    effi.SetSourceDataArray(SData);
    effi.SetExperimentalDataArray(ECalData);
    effi.SetSimulationDataArray(SimulationData);
    effi.SetFitParameterVector(Parameter_Efficency);
    effi.SetECalTime(ECalTime);
    effi.SetDetectorAngles(DetectorAngles);
    effi.CalculateEfficiency();
    CalculatedEfficiency=effi.GetEfficiencyDataArray();
    cout<<"Calculated Efficiencies for all Detectors"<<endl;
        // read.Print3DArray(effi.GetEfficiencyDataArray());
    effi.FitEfficiency();
    effi.PlotScaleDist();
    effi.PlotEFunc();
    string FileName="";
    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {
        FileName="Efficiency/Efficiency_";
        FileName.append(std::to_string((int)DetectorAngles[t]));
        FileName.append("_");
        FileName.append(IECalibrationDataFile.substr(0,IECalibrationDataFile.size()-4));
        FileName.append(".log");
        out.SetFileName(FileName);
        out.FileChecker();
        out.SetPreDataString("Energy dEnergy Efficiency dEfficiency");
        out.WriteLog(CalculatedEfficiency[t]);
        FileName="";
    }
    vector<vector<double> > Efficiency_Parameters(1,vector<double>(1,1));
    Efficiency_Parameters=effi.GetFittedParameters();

    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {

        FileName="Efficiency/Efficiency_";
        FileName.append(std::to_string((int)DetectorAngles[t]));
        FileName.append("_FitParameters");
        FileName.append(".log");
        out.SetFileName(FileName);
        out.FileChecker();
        out.SetPreDataString("Scale ScaleLOW ScaleHIGH a0 a1 a2 a3 a4");
        out.WriteLog(Efficiency_Parameters[t]);
        FileName="";
    }

    // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
    // // 
    // // Flux Calculation and Fitting
    // // 
    // // // // // // // // // // // // // // // // // // // // // // // // // // // //
        
    vector<vector<double> > ICS(1,vector<double>(1,1));
    vector<vector<double> >PhotonFlux(1,vector<double>(1,1));
    Flux flux;
    flux.SetFileName(IExperimentalDataFile);
    flux.SetRootFile(rootfile);
    flux.SetNThread(TNumber);
    flux.SetDetectorAngles(DetectorAngles);
    flux.SetCalibrationData(CalibrationFluxData);
    flux.SetFluxParameter(FluxCalibrationData);
    flux.SetEfficiencyParameter(effi.GetFittedParameters());
    flux.SetFitParameters(Parameter_Flux);
    flux.CalculateEfficiencyforCalibrationData();
    flux.CorrectingPeakArea();
    CalibrationFluxData=flux.GetCalibrationData();
    // cout<<"Corrected PeakAreas:"<<endl<<endl;
    // read.Print3DArray(CalibrationFluxData);
    cout<<endl;
    flux.CalculateICS();
    ICS=flux.GetICS();
    cout<<"Integrated Cross Section:"<<endl<<endl;
    read.Print2DArray(ICS);
    flux.CalculateFlux();
    PhotonFlux=flux.GetPhotonFluxData();
    cout<<"Photon Flux:"<<endl<<endl;
    read.Print2DArray(PhotonFlux);
    flux.FitPhotonFlux();
    flux.PlotFitParameters();
    flux.PlotPhotonFlux();

    FileName="Flux/Flux_";
    FileName.append(IFCalibrationFluxDataFile.substr(0,IFCalibrationFluxDataFile.size()-4));
    FileName.append(".log");
    out.SetFileName(FileName);
    out.FileChecker();
    out.SetPreDataString("Energy dEnergy Flux dmin_Flux dmax_Flux");
    out.WriteLog(PhotonFlux);
    FileName="";

    FileName="Flux/Flux_FitParameters";
    FileName.append(".log");
    out.SetFileName(FileName);
    out.FileChecker();
    out.SetPreDataString("Scale ScaleLOW ScaleHIGH EndPoint EndPointLOW EndPointHIGH ProtonNumber");
    out.WriteLog(flux.GetFittedParameters());
    FileName="";

    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {
        FileName="Flux/Efficiency_";
        FileName.append(std::to_string((int)DetectorAngles[t]));
        FileName.append("_");
        FileName.append(IFCalibrationFluxDataFile.substr(0,IFCalibrationFluxDataFile.size()-4));
        FileName.append(".log");
        out.SetFileName(FileName);
        out.FileChecker();
        out.SetPreDataString("Energy dEnergy Efficiency dmin_Efficiency dmax_Efficiency");
        out.WriteLog(flux.GetCalcEfficiencyCalibrationData()[t]);
        FileName="";
    }

    Gamma gamma;
    gamma.SetRootFile(rootfile);
    gamma.SetNThread(TNumber);
    gamma.SetEfficiencyParameter(effi.GetFittedParameters());
    gamma.SetFluxParameter(flux.GetFittedParameters());
    gamma.SetDetectorAngles(DetectorAngles);
    gamma.SetExperimentalData(ExperimentalData);
    gamma.SetExperimentalDataAngular(ExperimentalDataAngular);
    gamma.SetTargetMass(TargetMass);
    gamma.SetEfficiencyFitParameterDistribution(effi.GetFitParameterDistribution());
    gamma.SetFluxFitParameterDistribution(flux.GetFitParameterDistribution());
    gamma.CalculateResults();

    FileName="Results/Experimental_Flux_";
    FileName.append(IExperimentalDataFile.substr(0,IExperimentalDataFile.size()-4));
    FileName.append(".log");
    out.SetFileName(FileName);
    out.FileChecker();
    out.SetPreDataString("Energy dEnergy Flux dmin_Flux dmax_Flux");
    out.WriteLog(gamma.GetCalculatedFlux());
    FileName="";

    for(unsigned int t=0;t<DetectorAngles.size();t++)
    {
        FileName="Results/Experimental_Efficiency";
        FileName.append(std::to_string((int)DetectorAngles[t]));
        FileName.append("_");
        FileName.append(IExperimentalDataFile.substr(0,IExperimentalDataFile.size()-4));
        FileName.append(".log");
        out.SetFileName(FileName);
        out.FileChecker();
        out.SetPreDataString("Energy dEnergy Efficiency dmin_Efficiency dmax_Efficiency");
        out.WriteLog(gamma.GetCalculatedEfficiency()[t]);
        FileName="";
    }

    
}

string join="gs -dBATCH -dNOPAUSE -q -sDEVICE=pdfwrite -dAutoRotatePages=/None -sOutputFile=";
join+=Output::dir;
join+="final.pdf ";
join+=Output::dir;
join+="*/*.pdf ";


system(join.c_str());
high_resolution_clock::time_point stop = high_resolution_clock::now();
duration<double> delta_t = duration_cast< duration<double>>(stop - start);
cout <<std::fixed<< endl <<  "> main.cpp: Execution took " << delta_t.count() << " seconds" << endl<<endl;



cout << "That's a nice Programm" << endl;
}
