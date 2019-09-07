#include <iostream>
#include <string>
#include <cstdio>
#include <time.h>
#include <fstream>

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "g4root.hh"
#include "G4SystemOfUnits.hh"
#include "G4EmCalculator.hh"
#include "G4Electron.hh"
#include "G4UnitsTable.hh"

#include "TFile.h"

#include "MUON_RunAction.h"
#include "MUON_EventAction.h"
#include "MUON_Verbosity.h"
#include "MUON_OutputLog.h"
#include "MUON_OutputNtuples.h"
#include "MUON_GlobalNtuplesPtr.h"

#include "MUON_DetectorConstruction.h"
#include "MUON_PrimaryGeneratorAction.h"

using namespace std ;

std::string 
MUON_RunAction::get_current_time ()
{
    cout << "DEBUG[MUON_RunAction][get_current_time]" << endl ;
    time_t raw_time ;
    struct tm * time_info ;

    time (&raw_time) ;  /* get current time */
    time_info = localtime (&raw_time) ;

    int year  = time_info->tm_year - 100 ;
    int month = time_info->tm_mon + 1 ;
    int day   = time_info->tm_mday ;
    int hour  = time_info->tm_hour ;
    int min   = time_info->tm_min ;
    int sec   = time_info->tm_sec ;


    char date[100] ;
    sprintf (date, "%d%s%d%s%d%s%d%s%d%s%d", year,  (month<10 ? "0" : "" ) , 
                                            month, (day<10 ? "0": ""), 
                                            day,   (hour<10 ? "0" : ""), 
                                            hour,  (min<10 ? "0" : ""),
                                            min ,  (sec<10 ? "0": ""),
                                            sec) ;

    std::string out (date) ;
    return out ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


MUON_RunAction::MUON_RunAction (MUON_DetectorConstruction *det) :
    G4UserRunAction (), // ,messenger (0)
    output_hit_file_ (0),
    detector_ ( det )
{ 
    cout << "DEBUG[MUON_RunAction][ctor]" << endl ;
    G4cout << "MUON_RunAction::MUON_RunAction ()" << G4endl ;
    // set printing event number per each 100 events
    G4RunManager::GetRunManager ()->SetPrintProgress (1) ;     

    // messenger = new LIMbuSRunActionMessenger (this) ; 
    
    if (MUON_Verbosity::level > 0)
        G4cout << G4endl << "MUON_RunAction::MUON_RunAction::CreateNtuple" << G4endl << G4endl ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


MUON_RunAction::~MUON_RunAction ()
{
    cout << "DEBUG[MUON_RunAction][dtor]" << endl ;

//PG move this in the main file, where the variable is declared
//    delete global_ntuples_ptr ; 
//    global_ntuples_ptr = 0 ;

    if (MUON_Verbosity::level>0)
        G4cout << "MUON_RunAction deleted" << G4endl ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void MUON_RunAction::BeginOfRunAction (const G4Run* a_run)
{ 
    cout << "DEBUG[MUON_RunAction][BeginOfRunAction]" << endl ;

    std::string current_time = get_current_time () ;

    char run_name[100] ;
    sprintf (run_name, "_%d", a_run->GetRunID ()) ;

    out_filename = "MUON_" ;
    out_filename += current_time ;
    out_filename += run_name ;
    out_filename += "_data.root" ;


    output_hit_file_ = new TFile (out_filename.data (), "recreate") ;
    G4cout << "RunAction :: filename = " << out_filename << G4endl ;

    G4cout << "RunAction :: Init Ntuples" << G4endl ;
    global_ntuples_ptr = new MUON_OutputNtuples ("op_hits",  "Optical Photon Hits"          , int (1e6),
                                                 "mc_truth", "Electromagnetic Interactions" , int (1e3)) ; 
    global_ntuples_light_ptr = new MUON_OutputNtuplesLight ("events", 20) ; 
    //FIXME get the number of SiPM somehow!
    out_filename = "MUON_output_" ;
    out_filename += current_time ;
    out_filename += run_name ;
    out_filename += ".root" ;

    log_filename = "MUON_" ;
    log_filename += current_time ;
    log_filename += run_name ;
    log_filename += "_log.txt" ;

    MUON_OutputLog::logfile = new std::ofstream (log_filename) ; 
 // OutputLog::logfile = 0 ;

    MUON_OutputLog::log_cache << G4endl ;
    MUON_OutputLog::log_cache << "***************************************" << G4endl ;
    MUON_OutputLog::log_cache << "*        BEGIN OF RUN ACTION          *" << G4endl ;
    MUON_OutputLog::log_cache << "***************************************" << G4endl ;

    G4double density     = detector_->get_ls_material ()->GetDensity () ;  
    G4double radl        = detector_->get_ls_material ()->GetRadlen () ;

    MUON_OutputLog::log_cache << "LS Material: " <<  detector_->get_ls_material ()->GetName () << G4endl ;
    MUON_OutputLog::log_cache << "Density:                             " << G4BestUnit (density,"Volumic Mass") << G4endl ;
    MUON_OutputLog::log_cache << "Rad length:                          " << G4BestUnit (radl,   "Length") << G4endl ;

    ComputeElectronCriticalEnergy () ;

    /// dump all the info that was stored in the buffer up to now
    * (MUON_OutputLog::logfile) << MUON_OutputLog::log_cache.rdbuf () ;
    MUON_OutputLog::logfile->flush () ;

    if (MUON_Verbosity::level > 0)
        G4cout << "MUON_RunAction::MUON_RunAction::begin finished" << G4endl ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


/// from Geant4-10.4.0/examples/extended/electromagnetic/TestEm0
void MUON_RunAction::ComputeElectronCriticalEnergy ()
{
    cout << "DEBUG[MUON_RunAction][ComputeElectronCriticalEnergy]" << endl ;

    // compute e- critical energy (Rossi definition) and Moliere radius.
    // Review of Particle Physics - Eur. Phys. J. C3 (1998) page 147
    //
    G4EmCalculator emCal ;
      
    const G4Material* material = detector_->get_ls_material () ;
    const G4double radl = material->GetRadlen () ;
    G4double ekin = 5*MeV ;
    G4double deioni ;
    G4double err  = 1., errmax = 0.001 ;
    G4int    iter = 0 , itermax = 10 ;  
    while (err > errmax && iter < itermax) 
    {
        iter++ ;          
        deioni  = radl*
                  emCal.ComputeDEDX (ekin,G4Electron::Electron (),"eIoni",material) ;
        err = std::abs (deioni - ekin)/ekin ;
        ekin = deioni ;
    }

    MUON_OutputLog::log_cache << "Critical Energy (Rossi):            "  << std::setw (8) << G4BestUnit (ekin,"Energy") << G4endl ;
           
    //Pdg formula (only for single material)
    G4double pdga[2] = { 610*MeV, 710*MeV } ;
    G4double pdgb[2] = { 1.24, 0.92 } ;
    G4double EcPdg = 0. ;
    
    if (material->GetNumberOfElements () == 1) {
      G4int istat = 0 ;
      if (material->GetState () == kStateGas) istat = 1 ;  
      G4double Zeff = material->GetZ () + pdgb[istat] ;
      EcPdg = pdga[istat]/Zeff ;
      G4cout << "\t\t\t (from Pdg formula : " 
             << std::setw (8) << G4BestUnit (EcPdg,"Energy") << ")" ;    
    }
       

    const G4double Es = 21.2052*MeV ;
    G4double rMolier1 = Es/ekin, rMolier2 = rMolier1*radl ;

 // MUON_OutputLog::log_cache << "Moliere Radius:                      "  << std::setw (8) << rMolier1 << G4endl
 //                           << "X0 = "  << std::setw (8) << G4BestUnit (rMolier2,"Length") << G4endl ;
           
    if (material->GetNumberOfElements () == 1) 
    {
       G4double rMPdg = radl*Es/EcPdg ;
       G4cout << "\t (from Pdg formula : " 
              << std::setw (8) << G4BestUnit (rMPdg,"Length") << ")" ;    
     }         

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void MUON_RunAction::EndOfRunAction (const G4Run* )
{
    cout << "DEBUG[MUON_RunAction][EndOfRunAction]" << endl ;

 // global_ntuples_ptr->write () ; <-- It causes the tree headers to be written twice since
 //                                  tree->write () method is implicitly called by file->write ()
    output_hit_file_->Write () ;
    output_hit_file_->Close () ;

    MUON_OutputLog::logfile->close () ;

    G4cout << "EndOfRunAction :: all files closed " << G4endl ;

 // delete global_ntuples_ptr ; <-- NOT ALLOWED
 //   global_ntuples_ptr = 0 ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......