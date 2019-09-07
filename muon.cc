#include "MUON_DetectorConstruction.h"
#include "MUON_ActionInitialization.h"

/*
#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif
*/

#include "G4RunManager.hh"

#include "G4UImanager.hh"
//#include "FTFP_BERT.hh"
//#include "G4StepLimiterPhysics.hh"
#include "Randomize.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "MUON_PhysicsList.h"
#include "MUON_Verbosity.h"
#include "MUON_OutputNtuples.h" 
#include "MUON_GlobalNtuplesPtr.h"

#include "TROOT.h"

using namespace std ;
MUON_OutputNtuples      * global_ntuples_ptr ;
MUON_OutputNtuplesLight * global_ntuples_light_ptr ;

int main (int argc, char ** argv)
{
  // Detect interactive mode (if no arguments) and define UI session
  //

  MUON_Verbosity::level = 1 ;
  global_ntuples_ptr = 0 ;
  global_ntuples_light_ptr = 0 ;

  G4UIExecutive * ui = 0 ;
  if (argc == 1) {
    ui = new G4UIExecutive (argc, argv) ;
  }

  // Choose the Random engine
  G4Random::setTheEngine (new CLHEP::RanecuEngine) ;
  
  // Construct the default run manager
  //
/*
#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager ;
  cout << "Multithread running enabled" << endl ;
#else
  G4RunManager* runManager = new G4RunManager ;
  cout << "Multithread running NOT enabled" << endl ;
#endif
*/

  G4RunManager* runManager = new G4RunManager ;

  MUON_DetectorConstruction * det = new MUON_DetectorConstruction () ;

  // Set mandatory initialization classes
  //
  runManager->SetUserInitialization (det) ;

//G4VModularPhysicsList* physicsList = new FTFP_BERT;
//physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  MUON_PhysicsList *physicsList = new MUON_PhysicsList ;
  runManager->SetUserInitialization (physicsList) ;

    
  // Set user action classes
  runManager->SetUserInitialization (new MUON_ActionInitialization (det)) ;
  
  // >>> MY DEBUG <<<<
//G4cout << "runManager->InitializeGeometry()" << G4endl;
//runManager->InitializeGeometry();

//G4cout << "runManager->InitializePhysics()" << G4endl;
//runManager->InitializePhysics();

//G4cout << "runManager->Initialize()" << G4endl;
//runManager->Initialize();

//G4cout << G4endl << G4endl;


  // https://root-forum.cern.ch/t/error-llvm-symbols-exposed-to-cling/23597
  gROOT->GetInterpreter () ;

  // Initialize visualization
  //
  G4VisManager * visManager = new G4VisExecutive () ;
  if(MUON_Verbosity::level == 2)
      visManager->SetVerboseLevel ("Quiet") ;

  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  G4cout << "Initialize visual manager" << G4endl ;
  visManager->Initialize () ;

  // Get the pointer to the User Interface manager
  G4UImanager * UImanager = G4UImanager::GetUIpointer () ;

  // Process macro or start UI session
  //
  if ( ! ui ) {
    // barch mode
    G4cout << "Exectute " << argv[1] << G4endl ;
    G4String command = "/control/execute " ;
    G4String fileName = argv[1] ;
    UImanager->ApplyCommand (command+fileName) ;
    G4cout << argv[1] << " Execution : Done" << G4endl;
  }
  else {  
    // interactive mode
    UImanager->ApplyCommand ("/control/execute init_vis.mac") ;
    if (ui->IsGUI ()) {
      cout << "setting up GUI" << endl ;
      UImanager->ApplyCommand ("/control/execute gui.mac") ;
    }
    ui->SessionStart () ;
    delete ui ;
  }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

//  delete visManager;
//  delete runManager;

  delete global_ntuples_ptr ;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
