/**
 * @file main.cpp
 * @author z 2
 * @date 06-06-2017
 * @brief Main cpp for the project
 */

#include<iostream>
#include <iomanip> 
#include<cstdlib>
#include<stdlib.h>
#include<getopt.h>
#include <chrono>
#include <ctime>
#include <ratio>
#include<omp.h>


#include"DataReader.h"
#include"Efficiency.h"
#include"Flux.h"
// #include"Gamma.h"

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
  bool source       = false;
  bool efficiency   = false;
  bool simulation   = false;
  bool photonflux   = false;
  bool experimental = false;
  int c           = 0;
  
  string InputFile;
  string SourceDataFile;
  string ECalibrationDataFile;
  string SimulationDataFile;
  string FCalibrationDataFile;
  string ExperimentalDataFile;
  
  unsigned int TNumber=1;
  
  while (c > -1)
  {
      static struct option long_options[] =
      {
          {"input",                  required_argument,       0, 'i'},
          {"source",                 required_argument,       0, 's'},
          {"efficiency",             required_argument,       0, 'e'},
          {"simulation",             required_argument,       0, 'S'},
          {"flux",                   required_argument,       0, 'f'},
          {"thread",                 required_argument,       0, 't'},
          {"experimental",           required_argument,       0, 'E'},
          {"help",                   no_argument,             0, 'h'},
          {"help",                   no_argument,             0, '?'}
      };
      /* getopt_long stores the option index here. */
      int option_index = 0;

      c = getopt_long (argc, argv, "i:s:e:S:f:t:E:h:?",
         long_options, &option_index);

      switch (c)
      {
        case 0:
        
        break;

        case 'i':
        InputFile=optarg;
        input=true;
        break;
        
        case 's':
        SourceDataFile=optarg;
        source=true;
        break;

        case 'e':
        ECalibrationDataFile=optarg;
        efficiency=true;
        break;   
        
        case 'S':
        SimulationDataFile=optarg;
        simulation=true;
        break;  

        case 'f':
        FCalibrationDataFile=optarg;
        photonflux=true;
        break;
        
        case 't':
        TNumber=atoi(optarg);            
        
        case 'E':
        ExperimentalDataFile=optarg;
        experimental=true;
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
        cout << "-s, --source" <<endl;
        cout << "-e, --efficiency" << endl;
        cout << "-S, --simulation" << endl;
        cout << "-f, --flux" << endl;
        cout << "-t, --thread" << endl;
        cout << "-E, --experimental" << endl;
        cout << "========================================================================" << endl;
        cout << "The Data should be in a propper way." << endl;
        cout << "There is no problems for , or ; for inbetween." <<endl;
        cout << endl;
        cout << "   Energy   ΔEnergy   FWHM   EFWHM   Area   ΔArea   TailL   ΔTailL" << endl;
        cout << endl;
        cout << "========================================================================" << endl;
        cout << "#1\tTime of the Measurement" << endl;
        cout << "#2\tFitting-Mode (0=Knoll, 1=a4-Model, 2=Jäckel, 3=All)" << endl;
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
input=true;

high_resolution_clock::time_point start = high_resolution_clock::now();
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
// // // 
// // // Read Data
// // // 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //
vector <vector <string> > DataFileArray;
vector<Functions::EFunc> FitFunctionArray;
vector<vector<double> > FitParameterArray_Efficency,FitParameterArray_Flux;
vector<double> DetectorAngles;
vector<double> ECalTime;

DataReader read;
if(input){
    source=true;
    efficiency=true;
    simulation=true;
    photonflux=true;
    experimental=true;
    
    
//         read.SetInputName(InputFile);
    read.SetInputName("input.par");
    read.SetInputParameter();
    DataFileArray=read.GetDataFileArray();
    FitFunctionArray=read.GetFitFunctionArray();
    FitParameterArray_Efficency=read.GetFitParameterArray_Efficency();
    FitParameterArray_Flux=read.GetFitParameterArray_Flux();        
    DetectorAngles=read.GetDetectorAnglesArray();
    ECalTime=read.GetECalTimeArray();
}
if(!input)DataFileArray.resize(1);

// // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
// // // 
// // // Loop over Input Data or Using Flag Input
// // // 
// // // // // // // // // // // // // // // // // // // // // // // // // // // // //
for(unsigned int i=0;i<DataFileArray.size();i++)
{
    string ISourceDataFile       = DataFileArray[i][0];
    string IECalibrationDataFile = DataFileArray[i][1];
    string ISimulationDataFile   = DataFileArray[i][2];
    string IFCalibrationDataFile = DataFileArray[i][3];
    string IExperimentalDataFile = DataFileArray[i][4];
    
    Functions::EFunc FunctionEnum=FitFunctionArray[i];
    
    vector<double> Parameter_Efficency=FitParameterArray_Efficency[i];
    vector<double> Parameter_Flux=FitParameterArray_Flux[i];
    vector<vector<double > >  FluxCalibrationData;
    vector<vector<vector<double> > >  SData;
    vector<vector<vector<double> > >  ECalData;
    vector<vector<vector<double> > >  SimulationData;
    vector<vector<vector<double> > >  ExperimentalData;
    
    vector<vector<vector<double> > > CalculatedEfficiency;
    
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Source Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    
    if(source){
        if(ISourceDataFile!="default" && ISourceDataFile!="DEFAULT")
        {
            read.SetInputName(ISourceDataFile);
            if(ISourceDataFile=="flag" || ISourceDataFile=="FLAG")read.SetInputName(SourceDataFile);
            SData=read.GetSourceDataArray();
            read.SetSourceData();
//                 read.Print2DArray(SData);
        }
    }
    if(!source || ISourceDataFile=="default" || ISourceDataFile=="DEFAULT"){
        read.SetInputName("56CoData.dat");
        ISourceDataFile="56CoData.dat";
        read.SetSourceData();
        SData=read.GetSourceDataArray();
//             read.Print2DArray(SData);
    }
    
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Efficiency Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

    if(efficiency){
        if(IECalibrationDataFile!="default" && IECalibrationDataFile!="DEFAULT")
        {
            read.SetInputName(IECalibrationDataFile);
            if(IECalibrationDataFile=="flag" || IECalibrationDataFile=="FLAG")read.SetInputName(ECalibrationDataFile);
            read.SetECalData();
            ECalData=read.GetECalDataArray();
//                 read.Print2DArray(ECalData);
        }
    }
    
    if(!efficiency || IECalibrationDataFile=="default" || IECalibrationDataFile=="DEFAULT"){
        read.SetInputName("ECalData.dat");
        IECalibrationDataFile="ECalData.dat";
        read.SetExperimentalData();
        ECalData=read.GetExperimentalDataArray();
//             read.Print2DArray(ECalData);
    }
    
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Simulation Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

    if(simulation){
        if(ISimulationDataFile!="default" && ISimulationDataFile!="DEFAULT")
        {
            read.SetInputName(ISimulationDataFile);
            if(ISimulationDataFile=="flag" || ISimulationDataFile=="FLAG")read.SetInputName(SimulationDataFile);
            read.SetSimulationData();
            SimulationData=read.GetSimulationDataArray();
//                 read.Print2DArray(SimulationData);
        }
    }
    
    if(!simulation || ISimulationDataFile=="default" || ISimulationDataFile=="DEFAULT"){
        read.SetInputName("SimulationData.dat");
        ISimulationDataFile="SimulationData.dat";
        read.SetSimulationData();
        SimulationData=read.GetSimulationDataArray();
//             read.Print2DArray(SimulationData);
    }

        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Flux Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //

    if(photonflux){
        if(IFCalibrationDataFile!="default" && IFCalibrationDataFile!="DEFAULT")
        {
            read.SetInputName(IFCalibrationDataFile);
            if(IFCalibrationDataFile=="flag" || IFCalibrationDataFile=="FLAG")read.SetInputName(FCalibrationDataFile);
            read.SetFluxCalibrationData();
            FluxCalibrationData=read.GetFluxCalibrationDataArray();
//                 read.Print2DArray(FluxCalibrationData);
        }
    }
    
    if(!photonflux|| IFCalibrationDataFile=="default" ||IFCalibrationDataFile=="DEFAULT"){
        read.SetInputName("Bor_Parameter.dat");
        IFCalibrationDataFile="Bor_Parameter.dat";
        read.SetFluxCalibrationData();
        FluxCalibrationData=read.GetFluxCalibrationDataArray();
//             read.Print2DArray(FluxCalibrationData);
    }

        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Read Experimental Data
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    
    if(experimental){
        if(IExperimentalDataFile!="default" && IExperimentalDataFile!="DEFAULT")
        {
            read.SetInputName(IExperimentalDataFile);
            if(IExperimentalDataFile=="flag" ||IExperimentalDataFile=="FLAG")read.SetInputName(ExperimentalDataFile);
            read.SetExperimentalData();
            ExperimentalData=read.GetExperimentalDataArray();
//                 read.Print2DArray(ExperimentalData);
        }
    }
    
    if(!experimental || IExperimentalDataFile=="default" ||IExperimentalDataFile=="DEFAULT"){
        read.SetInputName("ExperimentalData.dat");
        IExperimentalDataFile="ExperimentalData.dat";
        read.SetExperimentalData();
        ExperimentalData=read.GetExperimentalDataArray();
//             read.Print2DArray(ExperimentalData);
    }

    cout<<"Reading Data Complete!"<<endl;
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // // 
        // // // Efficiency Calculation and Fitting
        // // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // // //
    Efficiency effi;
    if(efficiency)effi.SetFileName(ECalibrationDataFile);
    if(input)effi.SetFileName(IECalibrationDataFile);
    effi.SetSourceDataArray(SData);
    effi.SetExperimentalDataArray(ECalData);
    effi.SetSimulationDataArray(SimulationData);
    effi.SetFitParameterVector(Parameter_Efficency);
    effi.SetFitFunctionEnum(FunctionEnum);
    effi.SetECalTime(ECalTime);
    effi.SetDetectorAngles(DetectorAngles);
    effi.OrganizeData();
    effi.CalculateEfficiency();
    CalculatedEfficiency=effi.GetEfficiencyDataArray();
    cout<<"Calculated Efficiencies for all Detectors"<<endl;
        // read.Print3DArray(CalculatedEfficiency);
    effi.FitEfficiency();
            effi.PlotScaleDist();
            effi.PlotEFunc();
        for(unsigned int t=0;t<DetectorAngles.size();t++)
        {
        string FileName="Efficiency_from_";
        FileName.append(IECalibrationDataFile.substr(0,IECalibrationDataFile.size()-4));
        FileName.append("_");
        FileName.append(std::to_string((int)DetectorAngles[t]));
        FileName.append("_deg.log");
        out.SetFileName(FileName);
        out.FileChecker();
        out.SetPreDataString("Energy dEnergy Efficiency dEfficiency");
        out.WriteLog(CalculatedEfficiency[t]);
        }
    
        // // // // // // // // // // // // // // // // // // // // // // // // // // // //     
        // // 
        // // Flux Calculation and Fitting
        // // 
        // // // // // // // // // // // // // // // // // // // // // // // // // // // //
        
        // cout<<"Filename: "<<IExperimentalDataFile<<endl;
        // cout<<"Filename: "<<ExperimentalDataFile<<endl;


        vector<vector<double> > ICS,PhotonFlux;
        Flux flux;
            if(photonflux)flux.SetFileName(ExperimentalDataFile);
            if(input)flux.SetFileName(IExperimentalDataFile);
            flux.SetNThread(TNumber);
            flux.SetDetectorAngles(DetectorAngles);
            flux.SetInputData(ExperimentalData);
            flux.SetFluxParameter(FluxCalibrationData);
            flux.SetEfficiencyParameter(effi.GetFittedParameters());
            flux.SetFitParameters(Parameter_Flux);
            flux.SetEFunctionEnum(FunctionEnum);
            flux.CalculateEfficiencyforExperimentalData();
            flux.CorrectingPeakArea();
            ExperimentalData=flux.GetExperimentalData();
            cout<<"Corrected PeakAreas:"<<endl<<endl;
            read.Print3DArray(ExperimentalData);
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
    
}	

high_resolution_clock::time_point stop = high_resolution_clock::now();
duration<double> delta_t = duration_cast< duration<double>>(stop - start);
cout << endl <<  "> main.cpp: Execution took " << delta_t.count() << " seconds" << endl<<endl;



cout << "That's a nice Programm" << endl;
}