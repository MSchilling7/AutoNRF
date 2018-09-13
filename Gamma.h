/**
 * @file Gamma.cpp
 * @author Marcel Schilling
 * @date 21-06-2017
 * @brief CPP file for the Gamma class
 */

#ifndef Gamma_H
#define Gamma_H 1

class Gamma
{
public:
    /// Constructor for the Gamma
    Gamma();
    /// Destructor for the Gamma
    ~Gamma();
    
    /// Copy Constructor for Gamma
    Gamma(const Gamma&)

    {;};
    void SetCalculatedEfficiency(){};
    void SetCalculatedFlux(){};
    void SetMass();
    void SetExperimentalData(){};


private:



    
};
#endif