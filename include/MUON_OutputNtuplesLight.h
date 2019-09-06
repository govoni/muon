#ifndef MUON_OutputNtuplesLight_h
#define MUON_OutputNtuplesLight_h 1

#include <vector>
#include <string>

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "TTree.h"
#include "MUON_OPHit.h"

class MUON_OutputNtuplesLight
{
    public:
        MUON_OutputNtuplesLight (const char * name, int SiPM_number) ;
        ~MUON_OutputNtuplesLight () ;
        void set_branches () ;
        void write () ;
        void reset () ;
        void store_event () ;
        inline void fill_bullet (float x, float y, float E, float PID) 
          {m_bullet_x = x ; m_bullet_y = y ; m_bullet_E = E ; m_bullet_PID = PID ;}
        inline void fill_SiPM (unsigned int ID, float E)
          {m_SiPM_energies.at (ID) = E ;}
        std::string getName () { return m_name ; } ;

        TTree m_events ;


    private:

        std::string m_name ;

        /// input particle MC info
        float  m_bullet_x ;   
        float  m_bullet_y ;
        float  m_bullet_E ;
        float  m_bullet_PID ;

        /// SiPMs
        std::vector<float> m_SiPM_energies ;
//        std::vector<float> m_SiPM_times ; //PG for future implementation

} ;


#endif
