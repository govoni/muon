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
        inline void fill_bullet (float x, float y, float z, float E, float PID) 
          {m_bullet_x = x ; m_bullet_y = y ; m_bullet_z = z ; m_bullet_E = E ; m_bullet_PID = PID ; }
        inline void fill_SiPM (unsigned int ID, float E)
          {m_SiPM_energies.at (ID) = E ; }
        inline void update_bullet_lostE (float deltaE)
          {m_bullet_lostE += deltaE ; }  
        inline void set_bullet_lostE (float lostE)
          {m_bullet_lostE = lostE ; }  
        inline void set_bullet_lastZ (float lastZ)
          {m_bullet_lastZ = lastZ ; }  
        inline void set_bullet_lastE (float lastE)
          {m_bullet_lastE = lastE ; }  
        std::string getName () { return m_name ; }

        TTree m_events ;


    private:

        std::string m_name ;

        /// input particle MC info
        float m_bullet_x ;      // the initial x position of the bullet 
        float m_bullet_y ;      // the initial y position of the bullet 
        float m_bullet_z ;      // the initial z position of the bullet 
        float m_bullet_E ;      // the initial energy of the bullet  
        float m_bullet_PID ;    // the PDG PID of the bullet
        float m_bullet_lostE ;  // the total energy loss of the bullet in the material
        float m_bullet_lastZ ;  // the last z coordinate of the bullet, for each event
        float m_bullet_lastE ;  // the last energy value of the bullet, for each event

        /// SiPMs
        std::vector<float> m_SiPM_energies ;
//        std::vector<float> m_SiPM_times ; //PG for future implementation

} ;


#endif
