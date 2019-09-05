
#ifndef MUON_SteppingAction_h
#define MUON_SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "MUON_DetectorConstruction.h"

class MUON_DetectorConstruction;
//class MUON_SteppingActionMessenger;

class G4Track;
class G4StepPoint;
class G4OpBoundaryProcess;

//enum g4_subprocesses{ OpAbsorption=31, Compton=13, Photoelectric=12, Bremsstrahlung=3, Cherenkov=21, Scintillation=22 };

class MUON_SteppingAction : public G4UserSteppingAction
{
  public:

    MUON_SteppingAction(MUON_DetectorConstruction*);
    virtual ~MUON_SteppingAction();

    virtual void UserSteppingAction(const G4Step*);
 
  private:

    // maximum number of save states
    static G4int fMaxRndmSave;
 
    MUON_DetectorConstruction* fDetector;
    

};

#endif