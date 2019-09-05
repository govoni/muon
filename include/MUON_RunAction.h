
#ifndef MUON_RunAction_h
#define MUON_RunAction_h 1

#include <string>

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4String.hh"
//#include "TFile.h"
//#include "LIMbuSRunActionMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class TFile;
//class LIMbuSRunActionMessenger;

/// Run action class

class MUON_DetectorConstruction;
class MUON_PrimaryGeneratorAction;

class MUON_RunAction : public G4UserRunAction
{


  void InitRootFile();
  G4String out_filename;
  G4String log_filename;
  
private:
//  LIMbuSRunActionMessenger* messenger; 
    MUON_DetectorConstruction *detector_;
  
  public:
    MUON_RunAction( MUON_DetectorConstruction *det );
    virtual ~MUON_RunAction();

    virtual void BeginOfRunAction(const G4Run* run);
    virtual void   EndOfRunAction(const G4Run* run);
//
    void InitTree();
    void SetOutputFileName(G4String val){ out_filename=val;}
    std::string get_current_time();
    void ComputeElectronCriticalEnergy();

    TFile *output_hit_file_;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif