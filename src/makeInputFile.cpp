#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

int main()
{
	std::string input;
	std::cout<<"What should be the name of the file?"<<std::endl;
	std::getline(std::cin,input);
	std::ofstream file;
	file.open(input.c_str());
	input="";
	if(file.is_open())
	{
		file<<"//Efficency"<<std::endl;
		file<<std::setw(15)<<"//Source";
		file<<std::setw(15)<<"ECal";
		file<<std::setw(15)<<"Sim";
		file<<std::setw(15)<<"NFit";
		file<<std::setw(15)<<"Scale";
		file<<std::setw(15)<<"a0";
		file<<std::setw(15)<<"a1";
		file<<std::setw(15)<<"a2";
		file<<std::setw(15)<<"a3";
		file<<std::setw(15)<<"a4";
		file<<std::setw(15)<<"LiveTimes";
		file<<std::endl;

		std::cout<<"Where is the Source file located? (default = 56CoData.dat)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="56CoData.dat";
		file<<std::setw(15)<<input;
		input="";
		
		std::cout<<"Where is the Efficency Calibration file located? (default = ECal.dat)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="ECal.dat";
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Where is the Simulation file located? (default = sim.dat)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="sim.dat";
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the number of fits for the Efficency? (default = 1e5)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="1e5";
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Do you want to continue with the default start parameter \"scale = 1, a0=0, a1=0, a2=0, a3=0, a4=0\" for Efficency Calibration fits? (y/n; default = y)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "" || input == "y" || input == "Yes" || input == "yes" || input == "YES" || input == "Y")
		{
			file<<std::setw(15)<<1;
			file<<std::setw(15)<<0;
			file<<std::setw(15)<<0;
			file<<std::setw(15)<<0;
			file<<std::setw(15)<<0;
			file<<std::setw(15)<<0;

		}
		else
		{
			std::cout<<"Give me the start parameter \"scale\" for Efficency Calibration fits? (default = 1)"<<std::endl;
			std::getline(std::cin,input);
			if(input == "")input="1";
			file<<std::setw(15)<<input;
			input="";

			std::cout<<"Give me the start parameter \"a0\" for Efficency Calibration fits? (default = 0)"<<std::endl;
			std::getline(std::cin,input);
			if(input == "")input="0";
			file<<std::setw(15)<<input;
			input="";

			std::cout<<"Give me the start parameter \"a1\" for Efficency Calibration fits? (default = 0)"<<std::endl;
			std::getline(std::cin,input);
			if(input == "")input="0";
			file<<std::setw(15)<<input;
			input="";

			std::cout<<"Give me the start parameter \"a2\" for Efficency Calibration fits? (default = 0)"<<std::endl;
			std::getline(std::cin,input);
			if(input == "")input="0";
			file<<std::setw(15)<<input;
			input="";

			std::cout<<"Give me the start parameter \"a3\" for Efficency Calibration fits? (default = 0)"<<std::endl;
			std::getline(std::cin,input);
			if(input == "")input="0";
			file<<std::setw(15)<<input;
			input="";

			std::cout<<"Give me the start parameter \"a4\" for Efficency Calibration fits? (default = 0)"<<std::endl;
			std::getline(std::cin,input);
			if(input == "")input="0";
			file<<std::setw(15)<<input;
			input="";

		}
		int numberofdetectors=0;
		std::cout<<"How many Detectors did you use? (default = 2)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="2";
		numberofdetectors=stoi(input);
		input="";

		for (int i = 0; i < numberofdetectors; ++i)
		{
			std::cout<<"Give me the lifetime of Detector #"<<i<<" for the Efficency Calibration?"<<std::endl;
			std::getline(std::cin,input);
			if(input == ""){std::cout<<"No lifetime given!!!"<<std::endl;exit(0);}
			file<<std::setw(15)<<input;
			input="";
		}
		file<<std::endl;
		file<<"//Flux"<<std::endl;
		file<<std::setw(15)<<"//FluxParameter";
		file<<std::setw(15)<<"Cal";
		file<<std::setw(15)<<"NFit";
		file<<std::setw(15)<<"scale";
		file<<std::setw(15)<<"E0min";
		file<<std::setw(15)<<"E0max";
		file<<std::setw(15)<<"Z";
		file<<std::setw(15)<<"Mass";
		file<<std::setw(15)<<"dMass";
		file<<std::setw(15)<<"amu";
		file<<std::setw(15)<<"LifeTimes";
		file<<std::endl;

		std::cout<<"Where is the Flux Calibration Parameter file located? (default = Bor_Parameter.dat)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="Bor_Parameter.dat";
		file<<std::setw(15)<<input;
		input="";
		
		std::cout<<"Where is the file with the fitted Calibration Peaks located? (default = BorPoints.dat)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="BorPoints.dat";
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the number of fits for the Flux? (default = 1e5)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="1e5";
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the start parameter \"scale\" for Flux Calibration fits? (default = 1)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="1";
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the start parameter \"E0min\" for Flux Calibration fits?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No parameter given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the start parameter \"E0max\" for Flux Calibration fits?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No parameter given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the start parameter \"Z (Number of Protons in Radiator)\" for Flux Calibration fits?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No parameter given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the mass of the Flux Calibration target?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No mass given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the mass uncertainty of the Flux Calibration target?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No parameter given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the atomic mass of of the Flux Calibration target?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No parameter given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		for (int i = 0; i < numberofdetectors; ++i)
		{
			std::cout<<"Give me the lifetime of Detector #"<<i<<" of the Experiment?"<<std::endl;
			std::getline(std::cin,input);
			if(input == ""){std::cout<<"No lifetime given!!!"<<std::endl;exit(0);}
			file<<std::setw(15)<<input;
			input="";
		}

		file<<std::endl;

		file<<std::setw(15)<<"//Experimental";
		file<<std::setw(15)<<"AnularDist";
		file<<std::setw(15)<<"Mass";
		file<<std::setw(15)<<"dMass";
		file<<std::setw(15)<<"amu";
		file<<std::setw(15)<<"Angels";
		file<<std::endl;

		std::cout<<"Where is the fitted Experimental Peak file located? (default = ExperimentalData.dat)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="default";
		file<<std::setw(15)<<input;
		input="";
		
		std::cout<<"Where is the file with the Angular Distribution for the Experimental Peaks located? (default = ExperimentalDataAngular.dat)"<<std::endl;
		std::getline(std::cin,input);
		if(input == "")input="default";
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the mass of the Experimental target?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No mass given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the mass uncertainty of the Experimental target?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No parameter given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		std::cout<<"Give me the atomic mass of of the Flux Calibration target?"<<std::endl;
		std::getline(std::cin,input);
		if(input == ""){std::cout<<"No parameter given!!!"<<std::endl;exit(0);}
		file<<std::setw(15)<<input;
		input="";

		for (int i = 0; i < numberofdetectors; ++i)
		{
			std::cout<<"Give me the angle of Detector #"<<i<<" of the Experiment?"<<std::endl;
			std::getline(std::cin,input);
			if(input == ""){std::cout<<"No lifetime given!!!"<<std::endl;exit(0);}
			file<<std::setw(15)<<input;
			input="";
		}
		file<<std::endl;


	}
	file.close();
}