
#include "MUON_ActionInitialization.h"
#include "MUON_PrimaryGeneratorAction.h"
#include "MUON_SteppingAction.h"
#include "MUON_RunAction.h"
#include "MUON_EventAction.h"
#include "MUON_TrackingAction.h"

using namespace std ; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MUON_ActionInitialization::MUON_ActionInitialization(MUON_DetectorConstruction* det)
 : G4VUserActionInitialization(),
    fDetector(det)
{
G4cout << "MUON_ActionInitialization::" <<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MUON_ActionInitialization::~MUON_ActionInitialization()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MUON_ActionInitialization::BuildForMaster() const
{
  SetUserAction(new MUON_RunAction(fDetector));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MUON_ActionInitialization::Build() const
{
    G4cout << "MUON_ActionInitialization::Build()" << G4endl;

/// MUON_PrimaryGeneratorAction is needed but parameters are overridden by mac file
/// Possibly the way to retrieve them is through a messenger class 
    SetUserAction(new MUON_PrimaryGeneratorAction);
                                                   
/// Needed to init root file
    SetUserAction(new MUON_RunAction(fDetector) ); 
    SetUserAction(new MUON_EventAction);
    SetUserAction(new MUON_SteppingAction(fDetector));
    SetUserAction(new MUON_TrackingAction);
}  

