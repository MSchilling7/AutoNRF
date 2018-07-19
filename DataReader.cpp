/**
 * @file DataReader.h
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the DataReader class
 */

#include"DataReader.h"

using std::cout;
using std::setw;
using std::endl;
using std::istringstream;
using std::getline;


DataReader::DataReader()
{
    
};

// ---------------------------------------------------------
DataReader::~DataReader()
{
    
};

// ---------------------------------------------------------

void DataReader::SetInputParameter()
{
    std::string::size_type sz;
    vector<double> tempVector;
    string ScourceFile;
    string ECalFile;
    string SimulationFile;
    string FCalFile;
    string ExperimentalDataFile;
    string fitfuncstring;
    Functions::EFunc fitfunc;
    double timeM;
    string nfit_str,mass_str,dmass_str;
    double scale;
    double a0;
    double a1;
    double a2;
    double a3;
    double a4;
    double a5;
    
    double E0max;
    double E0min;
    double ProtonNumber;
    double Angles;

    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open()){
//              file opened successfully so we are here
        cout  <<  "File Opened success!!! Reading Input Parameters from file into arrays"  <<  endl;
        int currentline=0;
        while(getline(Filestream,line)){
            if(line[0] != '/' && line[1] != '/'){
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                
                istringstream buffer(line);
                if(currentline+2 %2 ==0)
                {
                    buffer >>
                    ScourceFile           >>
                    ECalFile              >>
                    SimulationFile        >>
                    fitfuncstring         >>
                    nfit_str              >>
                    scale                 >>
                    a0                    >> 
                    a1                    >> 
                    a2                    >> 
                    a3                    >> 
                    a4                    >>
                    a5             ;
                    if(fitfuncstring=="Knoll"){fitfunc=Functions::EFunc::Knoll;}
                    if(fitfuncstring=="a4"){fitfunc=Functions::EFunc::a4;}
                    if(fitfuncstring=="Jaeckel"){fitfunc=Functions::EFunc::Jaeckel;}
                    
                DataFileline.push_back(ScourceFile);                    //0
                DataFileline.push_back(ECalFile);                       //1
                DataFileline.push_back(SimulationFile);                 //2
                fitfuncarray.push_back(fitfunc); 
                tempVector.push_back((int)stod(nfit_str,&sz));     //0
                tempVector.push_back(scale);    //1
                tempVector.push_back(a0);       //2
                tempVector.push_back(a1);       //3
                tempVector.push_back(a2);       //4
                tempVector.push_back(a3);       //5
                tempVector.push_back(a4);       //6
                tempVector.push_back(a5);       //7
                fitParameterarray_Efficency.push_back(tempVector);
                tempVector.clear();
                for(;;)
                {
                    buffer>>timeM;
                    if(!buffer.fail())ETimes.push_back(timeM);
                    if(buffer.fail())break;
                }
            }
            if(currentline+2 %2 !=0)
            {
                buffer>>
                FCalFile>>
                nfit_str>>
                scale>>
                E0min>>
                E0max>>
                ProtonNumber>>
                mass_str>>
                dmass_str>>
                ExperimentalDataFile;
                
                
                
                DataFileline.push_back(FCalFile);                       //3
                DataFileline.push_back(ExperimentalDataFile);           //4
                tempVector.push_back((int)stod(nfit_str,&sz));           //0
                tempVector.push_back(scale);                 //1
                tempVector.push_back(E0min);                 //2
                tempVector.push_back(E0max);                 //3
                tempVector.push_back(ProtonNumber);          //4
                tempVector.push_back(stod(mass_str,&sz));    //5
                tempVector.push_back(stod(dmass_str,&sz));   //6
                
                for(;;)
                {
                    buffer>>Angles;
                    if(!buffer.fail())DetectorAngles.push_back(Angles);//7
                    if(buffer.fail())break;
                }
                
                fitParameterarray_Flux.push_back(tempVector);
                tempVector.clear();
            }
            
            if(currentline+2 %2)
            {
                DataFilearray.push_back(DataFileline);
                DataFileline.clear();
            }
            
            
            
            tempVector.clear();
        }
        if(line[0] == '/' && line[1] == '/')currentline=currentline-1;
        currentline++;
    };
    cout  <<  "Your Parameters could be imported! Yeah!"  <<  endl;}
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(InputParameter)"  <<  endl;std::exit(0);};
}
// ---------------------------------------------------------

void DataReader::SetSourceData()
{
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    double sourceenergy;
    double esourceenergy;
    double sourceintensity;
    double esourceintensity;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open()){
//              file opened successfully so we are here
        cout  <<  "File Opened success!!!y!!!. Reading Efficiency Calibration Data from file into array"  <<  endl;
        while(getline(Filestream,line)){
            if(line[0] != '/' && line[1] != '/'){
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
            }
            istringstream buffer(line);
            buffer >>
            sourceenergy     >>
            esourceenergy    >>
            sourceintensity  >>
            esourceintensity;
            
            
            
            tempVector.push_back(sourceenergy);
            tempVector.push_back(esourceenergy);
            tempVector.push_back(sourceintensity);
            tempVector.push_back(esourceintensity);
            
            temp2DVector.push_back(tempVector);
            tempVector.clear();
            if(line[0] == '/' && line[1] == '/')
            {
                sort(temp2DVector.begin(),temp2DVector.end());
                sData.push_back(temp2DVector);
                temp2DVector.clear();
            }
        }
        if(!temp2DVector.empty())
        {            
            sort(temp2DVector.begin(),temp2DVector.end());
            sData.push_back(temp2DVector);
            temp2DVector.clear();
        }
        cout  <<  "Source Data could be imported! Yeah!"  <<  endl;}
        else{cout  <<  "Noooope! Dont gimme a piece of crap!(Source)"  <<  endl;std::exit(0);};
    }

// ---------------------------------------------------------
    
    
    void DataReader::SetFluxCalibrationData()
    {
        vector<double> tempVector;
        double Ex;
        double Ef;
        double EGamma;
        double dEGamma;
        double J0;
        double J1;
        double Gamma;
        double dGamma;
        double Branch;
        double dBranch;
        double W90;
        double W130;
        ifstream Filestream;
        Filestream.open(GetFileName(),std::ifstream::in);
        if(Filestream.is_open()){
//              file opened successfully so we are here
            cout  <<  "File Opened successfully!!! Reading Photon Flux Calibration Data from file into array"  <<  endl;
            while(getline(Filestream,line))
            {
                if(line[0] != '/' && line[1] != '/')
                {
                    for(unsigned int i=0; i<line.size() ;i++)
                    {
                        if(line[i] == ',' || line[i] == ';')
                        {
                            line[i]=' ';
                        };
                    }
                    istringstream buffer(line);
                    buffer >>
                    Ex         >>
                    Ef         >>
                    EGamma     >>
                    dEGamma    >>
                    J0         >>
                    J1         >>
                    Gamma      >>
                    dGamma     >>
                    Branch     >>
                    dBranch    >>
                    W90        >>
                    W130;
                    
                    tempVector.push_back(Ex);
                    tempVector.push_back(Ef);
                    tempVector.push_back(EGamma);
                    tempVector.push_back(dEGamma);
                    tempVector.push_back(J0);
                    tempVector.push_back(J1);
                    tempVector.push_back(Gamma);
                    tempVector.push_back(dGamma);
                    tempVector.push_back(Branch);
                    tempVector.push_back(dBranch);
                    tempVector.push_back(W90);
                    tempVector.push_back(W130);
                    
                    fluxcalibrationData.push_back(tempVector);
                    tempVector.clear();
                }
                sort(fluxcalibrationData.begin(),fluxcalibrationData.end());
            }
            cout  <<  "Flux Calibration Data could be imported! Yeah!"  <<  endl;}
            else{cout  <<  "Noooope! Dont gimme a piece of crap!(FluxCalibrationData)"  <<  endl;std::exit(0);};
        }

// ---------------------------------------------------------
        
        void DataReader::SetECalData()
        {
            vector<double> tempVector;
            vector<vector<double> > temp2DVector;

            double energyD;
            double eenergyD;
            double fwhmD;
            double efwhmD;
            double volumeD;
            double evolumeD;
            double tailleftD;
            double etailleftD;
            ifstream Filestream;
            Filestream.open(GetFileName(),std::ifstream::in);
            if(Filestream.is_open()){
//              file opened successfully so we are here
                cout  <<  "File Opened successfully!!! Reading Peak Data from file into array"  <<  endl;
                while(getline(Filestream,line))
                {
                    if(line[0] != '/' && line[1] != '/')
                    {
                        for(unsigned int i=0; i<line.size() ;i++)
                        {
                            if(line[i] == ',' || line[i] == ';')
                            {
                                line[i]=' ';
                            };
                        }
                        istringstream buffer(line);
                        buffer >>   energyD  >> 
                        eenergyD  >> 
                        fwhmD  >> 
                        efwhmD  >> 
                        volumeD  >> 
                        evolumeD  >> 
                        tailleftD  >> 
                        etailleftD;
                        
                        tempVector.push_back(energyD);
                        tempVector.push_back(eenergyD);
                        tempVector.push_back(fwhmD);
                        tempVector.push_back(efwhmD);
                        tempVector.push_back(volumeD);
                        tempVector.push_back(evolumeD);
                        tempVector.push_back(etailleftD);
                        
                        temp2DVector.push_back(tempVector);
                        tempVector.clear();
                    }
                    if(line[0] == '/' && line[1] == '/')
                    {
                        sort(temp2DVector.begin(),temp2DVector.end());
                        ecalData.push_back(temp2DVector);
                        temp2DVector.clear();
                    }
                }
                if(!temp2DVector.empty())
                {            
                    sort(temp2DVector.begin(),temp2DVector.end());
                    ecalData.push_back(temp2DVector);
                    temp2DVector.clear();
                }
                cout  <<  "Your Data could be imported! Yeah!"  <<  endl;}
                else{cout  <<  "Noooope! Dont gimme a piece of crap!(ECalData)"  <<  endl;std::exit(0);};
            }

// ---------------------------------------------------------
            
            void DataReader::SetExperimentalData()
            {
                
                vector<double> tempVector;
                vector<vector<double> > temp2DVector;
                double energyD;
                double eenergyD;
                double fwhmD;
                double efwhmD;
                double volumeD;
                double evolumeD;
                double tailleftD;
                double etailleftD;
                
                ifstream Filestream;
                Filestream.open(GetFileName(),std::ifstream::in);
                if(Filestream.is_open()){
//              file opened successfully so we are here
                    cout  <<  "File Opened successfully!!! Reading Peak Data from file into array"  <<  endl;
                    while(getline(Filestream,line)){
                        if(line[0] != '/' && line[1] != '/'){
                            for(unsigned int i=0; i<line.size() ;i++)
                            {
                                if(line[i] == ',' || line[i] == ';')
                                {
                                    line[i]=' ';
                                };
                            }
                            istringstream buffer(line);
                            buffer >>   energyD  >> 
                            eenergyD  >> 
                            fwhmD  >> 
                            efwhmD  >> 
                            volumeD  >> 
                            evolumeD  >> 
                            tailleftD  >> 
                            etailleftD;
                            
                            tempVector.push_back(energyD);
                            tempVector.push_back(eenergyD);
                            tempVector.push_back(fwhmD);
                            tempVector.push_back(efwhmD);
                            tempVector.push_back(volumeD);
                            tempVector.push_back(evolumeD);
                            tempVector.push_back(tailleftD);
                            tempVector.push_back(etailleftD);
                            
                            temp2DVector.push_back(tempVector);
                            tempVector.clear();
                        }
                        if(line[0] == '/' && line[1] == '/')
                        {
                            sort(temp2DVector.begin(),temp2DVector.end());
                            experimentalData.push_back(temp2DVector);
                            temp2DVector.clear();
                        }
                    }
                    if(!temp2DVector.empty())
                    {            
                        sort(temp2DVector.begin(),temp2DVector.end());
                        experimentalData.push_back(temp2DVector);
                        temp2DVector.clear();
                    }
                    cout  <<  "Your Data could be imported! Yeah!"  <<  endl;}
                    else{cout  <<  "Noooope! Dont gimme a piece of crap!(PeakData)"  <<  endl;std::exit(0);};
                }

// ---------------------------------------------------------

                void DataReader::SetSimulationData()
                {
                    vector<double> tempVector;
                    vector<vector<double> > temp2DVector;
                    double energyS;
                    double volumeS;

                    ifstream Filestream;
                    Filestream.open(GetFileName(),std::ifstream::in);
                    if(Filestream.is_open()){
//              file opened successfully so we are here
                        cout  <<  "File Opened successfully!!! Reading Simulation Data from file into array"  <<  endl;
                        while(getline(Filestream,line))
                        {
                            if(line[0] != '/' && line[1] != '/')
                            {
                                for(unsigned int i=0; i<line.size() ;i++)
                                {
                                    if(line[i] == ',' || line[i] == ';')
                                    {
                                        line[i]=' ';
                                    };
                                }
                                istringstream buffer(line);
                                buffer >>   energyS  >> 
                                volumeS;
                                
                                tempVector.push_back(energyS);
                                tempVector.push_back(volumeS);
                                
                                temp2DVector.push_back(tempVector);
                                tempVector.clear();
                            }
                            if(line[0] == '/' && line[1] == '/')
                            {
                                sort(temp2DVector.begin(),temp2DVector.end());
                                simulationData.push_back(temp2DVector);
                                temp2DVector.clear();
                            }
                        }
                        if(!temp2DVector.empty())
                        {            
                            sort(temp2DVector.begin(),temp2DVector.end());
                            simulationData.push_back(temp2DVector);
                            temp2DVector.clear();
                        }
                        cout  <<  "Your Data could be imported! Yeah!"  <<  endl;}
                        else{cout  <<  "Noooope! Dont gimme a piece of crap!(SimulationData)"  <<  endl;std::exit(0);};
                    }

// ---------------------------------------------------------

                    void DataReader::Print2DArray(vector<vector<double> > array){
                        for(unsigned int i=0;i<array.size();i++)
                        {
                            for(unsigned int j=0;j<array[i].size();j++){
                                cout  << std::scientific <<  setw(15)  <<  array[i][j];
                            }
                            cout  <<  endl;
                        }
                        cout <<endl;
                    };

// ---------------------------------------------------------

                    void DataReader::Print3DArray(vector<vector<vector<double> > > array)
                    {
                        for(unsigned int i=0;i<array.size();i++)
                        {
                            Print2DArray(array[i]);
                            cout<<endl<<endl;
                        }
                    }

// ---------------------------------------------------------

                    void DataReader::Print1DArray(vector<int> array){
                        for(unsigned int i=0;i<array.size();i++)cout  << std::scientific <<  array[i]  <<  endl;
                            cout<<endl;
                    };

// ---------------------------------------------------------

                    void DataReader::Print1DArray(vector<double> array){
                        for(unsigned int i=0;i<array.size();i++)cout  << std::scientific <<  array[i]  <<  endl;
                            cout<<endl;
                        
};// ---------------------------------------------------------

void DataReader::Print1DArray(vector<string> array){
    for(unsigned int i=0;i<array.size();i++)cout  <<  array[i]  <<  endl;
        cout<<endl;
};
