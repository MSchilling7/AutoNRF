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
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open())
    {
//              file opened successfully so we are here
        cout  <<  "File Opened success!!! Reading Input Parameters from file into arrays"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/')
            {
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());


                for(unsigned int i=0;i<3;i++)
                {
                    DataFileline.push_back(words1[i]);//Source ECal Sim
                }
                for(int i=3;i<10;i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));//NFit Scale a0 a1 a2 a3 a4
                }
                fitParameterarray_Efficency.push_back(tempVector);
                tempVector.clear();
                for(unsigned int i=10;i<words1.size();i++)
                {
                    ETimes.push_back(stod(words1[i],&sz));
                }


                for(;;)
                {
                    getline(Filestream,line);
                    if(line[0] != '/' && line[1] != '/')
                    {
                        for(unsigned int i=0; i<line.size() ;i++)
                        {
                            if(line[i] == ',' || line[i] == ';')
                            {
                                line[i]=' ';
                            };
                        }
                        break;
                    }
                }
                istringstream buffer2(line);
                vector<string> words2(std::istream_iterator<std::string>{buffer2},std::istream_iterator<std::string>());


                for(unsigned int i=0;i<2;i++)
                {
                    DataFileline.push_back(words2[i]);
                }
                for(unsigned int i=2;i<words2.size();i++)
                {
                    tempVector.push_back(stod(words2[i],&sz));//nfit scale emin emax protonnumber mass dmass LifeTimes;
                }
                fitParameterarray_Flux.push_back(tempVector);
                tempVector.clear();

                for(;;)
                {
                    getline(Filestream,line);
                    if(line[0] != '/' && line[1] != '/')
                    {
                        for(unsigned int i=0; i<line.size() ;i++)
                        {
                            if(line[i] == ',' || line[i] == ';')
                            {
                                line[i]=' ';
                            };
                        }
                        break;
                    }
                }

                istringstream buffer3(line);
                vector<string> words3(std::istream_iterator<std::string>{buffer3},std::istream_iterator<std::string>());

                for(unsigned int i=0;i<2;i++)
                {
                    DataFileline.push_back(words3[i]);
                }
                for(unsigned int i=2;i<5;i++)
                {
                    Mass.push_back(stod(words3[i],&sz));
                }
                for(unsigned int i=5;i<words3.size();i++)
                {
                    DetectorAngles.push_back((int)stod(words3[i],&sz));
                }
                DataFilearray.push_back(DataFileline);
                DataFileline.clear();


            }
        };
        cout  <<  "Your Parameters could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(InputParameter)"  <<  endl;std::exit(0);};
}
// ---------------------------------------------------------

void DataReader::SetSourceData()
{
    std::string::size_type sz;
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open())
    {
//              file opened successfully so we are here
        cout  <<  "File Opened success!!!y!!!. Reading Efficiency Calibration Data from file into array"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/')
            {
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());
                for(unsigned int i=0;i<words1.size();i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));
                }
                temp2DVector.push_back(tempVector);
                tempVector.clear();
            }
            if(line[0] == '/' && line[1] == '/')
            {
                if(temp2DVector.empty())continue;
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
        cout  <<  "Source Data could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(Source)"  <<  endl;std::exit(0);};
}

// ---------------------------------------------------------
    
    
void DataReader::SetCalibrationFluxParameter()
{
    std::string::size_type sz;
    vector<double> tempVector;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open())
    {   //              file opened successfully so we are here
        cout  <<  "File Opened successfully!!! Reading Photon Flux Calibration Data from file into array"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/')
            {
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());
                for(unsigned int i=0;i<words1.size();i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));
                }

                fluxcalibrationData.push_back(tempVector);
                tempVector.clear();
            }
            sort(fluxcalibrationData.begin(),fluxcalibrationData.end());
        }
        cout  <<  "Flux Calibration Parameter could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(FluxCalibrationData)"  <<  endl;std::exit(0);};
}

// ---------------------------------------------------------
        
void DataReader::SetECalData()
{
    std::string::size_type sz;
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open()){//              file opened successfully so we are here
        cout  <<  "File Opened successfully!!! Reading Peak Data from file into array"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/')
            {
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());
                for(unsigned int i=0;i<words1.size();i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));
                }
                temp2DVector.push_back(tempVector);
                tempVector.clear();
            }
            if(line[0] == '/' && line[1] == '/')
            {
                if(temp2DVector.empty())continue;
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
        cout  <<  "Your Efficiency Data could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(ECalData)"  <<  endl;std::exit(0);};
}

// ---------------------------------------------------------
void DataReader::SetExperimentalData()
{                
    std::string::size_type sz;
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open())
    {   //              file opened successfully so we are here
        cout  <<  "File Opened successfully!!! Reading Peak Data from file into array"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/'){
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());
                for(unsigned int i=0;i<words1.size();i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));
                }
                temp2DVector.push_back(tempVector);
                tempVector.clear();
            }
            if(line[0] == '/' && line[1] == '/')
            {
                if(temp2DVector.empty())continue;
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
        cout  <<  "Your Experimental Data could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(PeakData)"  <<  endl;std::exit(0);};
}

// ---------------------------------------------------------
void DataReader::SetCalibrationFluxData()
{                
    std::string::size_type sz;
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open())
    {   //              file opened successfully so we are here
        cout  <<  "File Opened successfully!!! Reading Peak Data from file into array"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/'){
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());
                for(unsigned int i=0;i<words1.size();i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));
                }
                temp2DVector.push_back(tempVector);
                tempVector.clear();
            }
            if(line[0] == '/' && line[1] == '/')
            {
                if(temp2DVector.empty())continue;
                sort(temp2DVector.begin(),temp2DVector.end());
                calibrationfluxdata.push_back(temp2DVector);
                temp2DVector.clear();
            }
            
        }
        if(!temp2DVector.empty())
        {            
            sort(temp2DVector.begin(),temp2DVector.end());
            calibrationfluxdata.push_back(temp2DVector);
            temp2DVector.clear();
        }
        cout  <<  "Your Flux Calibration Data could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(FluxCalibrationData)"  <<  endl;std::exit(0);};
}

// ---------------------------------------------------------

void DataReader::SetSimulationData()
{
    std::string::size_type sz;
    vector<double> tempVector;
    vector<vector<double> > temp2DVector;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open())
    {   //              file opened successfully so we are here
        cout  <<  "File Opened successfully!!! Reading Simulation Data from file into array"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/')
            {
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());
                for(unsigned int i=0;i<words1.size();i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));
                }
                temp2DVector.push_back(tempVector);
                tempVector.clear();
            }
            if(line[0] == '/' && line[1] == '/')
            {
                if(temp2DVector.empty())continue;
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
        cout  <<  "Your Simulation Data could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(SimulationData)"  <<  endl;std::exit(0);};
}



// ---------------------------------------------------------

void DataReader::SetAngularDistribution()
{
    std::string::size_type sz;
    vector<double> tempVector;
    ifstream Filestream;
    Filestream.open(GetFileName(),std::ifstream::in);
    if(Filestream.is_open())
    {   //              file opened successfully so we are here
        cout  <<  "File Opened successfully!!! Reading AngularDistribution Data from file into array"  <<  endl;
        while(getline(Filestream,line))
        {
            if(line.empty())continue;
            if(line[0] != '/' && line[1] != '/')
            {
                for(unsigned int i=0; i<line.size() ;i++)
                {
                    if(line[i] == ',' || line[i] == ';')
                    {
                        line[i]=' ';
                    };
                }
                istringstream buffer1(line);
                vector<string> words1(std::istream_iterator<std::string>{buffer1},std::istream_iterator<std::string>());
                for(unsigned int i=0;i<words1.size();i++)
                {
                    tempVector.push_back(stod(words1[i],&sz));
                }
                experimentalDataAngular.push_back(tempVector);
                tempVector.clear();
            }
            if(line[0] == '/' && line[1] == '/')
            {
                continue;
            }
            
        }
        sort(experimentalDataAngular.begin(),experimentalDataAngular.end());
        cout  <<  "Your AngularDistribution Data could be imported! Yeah!"  <<  endl;
    }
    else{cout  <<  "Noooope! Dont gimme a piece of crap!(AngularDistribution)"  <<  endl;std::exit(0);};
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

void DataReader::Print1DArray(vector<int> array)
{
    for(unsigned int i=0;i<array.size();i++)cout  << std::scientific <<  array[i]  <<  endl;
    cout<<endl;
};

// ---------------------------------------------------------

void DataReader::Print1DArray(vector<double> array)
{
    for(unsigned int i=0;i<array.size();i++)cout  << std::scientific <<  array[i]  <<  endl;
    cout<<endl;
};

// ---------------------------------------------------------

void DataReader::Print1DArray(vector<string> array)
{
    for(unsigned int i=0;i<array.size();i++)cout  <<  array[i]  <<  endl;
    cout<<endl;
};
