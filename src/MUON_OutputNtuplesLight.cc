#include "MUON_OutputNtuplesLight.h"

#include <algorithm>

using namespace std ;

MUON_OutputNtuplesLight::MUON_OutputNtuplesLight (const char * name, int SiPM_number) :
    m_name (name),
    m_bullet_x (0.),  
    m_bullet_y (0.), 
    m_bullet_z (0.), 
    m_bullet_E (-1.), 
    m_bullet_PID (-1.), 
    m_SiPM_energies (SiPM_number, 0),
    m_events (name, name)
{
    set_branches () ;
    reset () ; // melius abundare quam deficiere
    G4cout << "MUON_OutputNtuplesLight created" << G4endl ;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void
MUON_OutputNtuplesLight::write ()
{
    m_events.Write () ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void
MUON_OutputNtuplesLight::set_branches ()
{
  m_events.Branch ("bullet_x     " , &m_bullet_x     , "bullet_x/F") ;
  m_events.Branch ("bullet_y     " , &m_bullet_y     , "bullet_y/F") ;
  m_events.Branch ("bullet_z     " , &m_bullet_z     , "bullet_z/F") ;
  m_events.Branch ("bullet_E     " , &m_bullet_E     , "bullet_E/F") ;
  m_events.Branch ("bullet_PID   " , &m_bullet_PID   , "bullet_PID/F") ;
  m_events.Branch ("SiPM_energies" , &m_SiPM_energies) ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void
MUON_OutputNtuplesLight::reset ()
{
  m_bullet_x      = 0. ;
  m_bullet_y      = 0. ;
  m_bullet_z      = 0. ;
  m_bullet_E      = -1. ;
  m_bullet_PID    = -1. ;
  std::fill (m_SiPM_energies.begin (), m_SiPM_energies.end (), 0.) ;
  return ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void
MUON_OutputNtuplesLight::store_event ()
{
  m_events.Fill () ;
  reset () ;
  return ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


MUON_OutputNtuplesLight::~MUON_OutputNtuplesLight ()
{
  G4cout << "[MUON_OutputNtuplesLight][dtor]" << G4endl ;
}


