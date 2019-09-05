
// Physics List
//this
#include "MUON_PhysicsList.h"
#include "MUON_Verbosity.h"

//std
#include <iomanip>

//Geant 4
#include "G4ParticleDefinition.hh"
#include "G4ParticleWithCuts.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4BosonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ProcessManager.hh"
#include "G4ProcessVector.hh"
#include "G4ParticleTypes.hh"
#include "G4ParticleTable.hh"
#include "G4HadronCaptureProcess.hh"
#include "G4Material.hh"
#include "G4ios.hh"
#include "G4UImanager.hh"
#include "globals.hh"

//MBA
#include "G4MuonDecayChannel.hh"
#include "G4SystemOfUnits.hh"
#include "LBE.hh"

// *************************
// *     EM PROCESSES      *
// *************************/


MUON_PhysicsList::MUON_PhysicsList() : G4VUserPhysicsList()
                                    /* physicsMessenger(0)*/ 
{
    SetVerboseLevel(0);

    if(MUON_Verbosity::level>1)
        G4cout << "MUON_PhysicsList()" << G4endl;

 // RANGE CUTS are defined in the SetCuts() method
 //
 // defaultCutValue = 0.1*cm;
 // electronCutValue = 5.0*mm;
 // gammaCutValue = 1*mm;

//  physicsMessenger = new PhysicsMessenger(this);
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


MUON_PhysicsList::~MUON_PhysicsList() 
{
    //delete physicsMessenger;
    //physicsMessenger = 0;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void MUON_PhysicsList::ConstructParticle() 
{
    if(MUON_Verbosity::level>1)
        G4cout << "MUON_PhysicsList::ConstructParticle()" << G4endl;

    /// basically all the families except for the short-lived one
    /// http://geant4.cern.ch/G4UsersDocuments/UsersGuides/ForApplicationDeveloper/html/GettingStarted/particleDef.html
    G4LeptonConstructor leptonConstructor;
                        leptonConstructor.ConstructParticle();
    G4MesonConstructor  mesonConstructor;
                        mesonConstructor.ConstructParticle();
    G4BaryonConstructor baryonConstructor;
                        baryonConstructor.ConstructParticle();
    G4BosonConstructor  bosonConstructor;
                        bosonConstructor.ConstructParticle();
    G4IonConstructor    ionConstructor;
                        ionConstructor.ConstructParticle();


    G4OpticalPhoton::OpticalPhotonDefinition();
    G4Geantino::GeantinoDefinition();

    if(MUON_Verbosity::level>1)
        G4cout << "MUON_PhysicsList::ConstructParticle() :: Done" << G4endl;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void MUON_PhysicsList::ConstructProcess() 
{
    if(MUON_Verbosity::level>1)
        G4cout << "MUON_PhysicsList::ConstructProcess()" << G4endl;
    AddTransportation();
    ConstructEM();

    // ConstructGeneral();
    ConstructOp();
    //  ConstructHad();
    ConstructDecay();
 // ConstructHad_QSGP_BIC_HP();//MBA 4/12/15 for neutrons
 // ConstructHadEl();//MBA 4/12/15 for neutrons
    G4cout << "MUON_PhysicsList::ConstructProcess() :: Done" << G4endl;
}//ConstructProcess


#include "G4EmStandardPhysics.hh"
#include "G4ParticleDefinition.hh"
#include "G4ProcessManager.hh"
#include "G4LossTableManager.hh"
#include "G4EmProcessOptions.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

//#include "G4MultipleScattering.hh"
#include "G4hMultipleScattering.hh"
#include "G4eMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4hIonisation.hh"
#include "G4ionIonisation.hh"

void MUON_PhysicsList::ConstructEM() 
{
    G4cout << "MUON_PhysicsList::ConstructEM()" << G4endl;
    
    auto theParticleIterator=GetParticleIterator();
    //  G4int verbose = 0;
    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        G4double charge = particle->GetPDGCharge();
        
        
        if (particleName == "gamma") {
            
            pmanager->AddDiscreteProcess(new G4PhotoElectricEffect);
            pmanager->AddDiscreteProcess(new G4ComptonScattering);
            pmanager->AddDiscreteProcess(new G4GammaConversion);
            
        } else if (particleName == "e-") {
            /*    G4VProcess* theeminusMultipleScattering = new G4eMultipleScattering();
             G4VProcess* theeminusIonisation         = new G4eIonisation();
             G4VProcess* theeminusBremsstrahlung     = new G4eBremsstrahlung();
             
             // add processes
             pmanager->AddProcess(theeminusMultipleScattering);
             pmanager->AddProcess(theeminusIonisation);
             pmanager->AddProcess(theeminusBremsstrahlung);
             //
             // set ordering for AlongStepDoIt
             pmanager->SetProcessOrdering(theeminusMultipleScattering, idxAlongStep,1);
             pmanager->SetProcessOrdering(theeminusIonisation,         idxAlongStep,2);
             pmanager->SetProcessOrdering(theeminusBremsstrahlung,     idxAlongStep,3);
             //
             // set ordering for PostStepDoIt
             pmanager->SetProcessOrdering(theeminusMultipleScattering, idxPostStep,1);
             pmanager->SetProcessOrdering(theeminusIonisation,         idxPostStep,2);
             pmanager->SetProcessOrdering(theeminusBremsstrahlung,     idxPostStep,3);*/
            
            pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
            pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung(),  -1,-3, 3);
            
        } else if (particleName == "e+") {
            
            /* G4VProcess* theeplusMultipleScattering = new G4eMultipleScattering();
             G4VProcess* theeplusIonisation         = new G4eIonisation();
             G4VProcess* theeplusBremsstrahlung     = new G4eBremsstrahlung();
             G4VProcess* theeplusAnnihilation       = new G4eplusAnnihilation();
             
             pmanager->AddProcess(theeplusMultipleScattering);
             pmanager->AddProcess(theeplusIonisation);
             pmanager->AddProcess(theeplusBremsstrahlung);
             pmanager->AddProcess(theeplusAnnihilation);
             //
             // set ordering for AtRestDoIt
             pmanager->SetProcessOrderingToFirst(theeplusAnnihilation, idxAtRest);
             //
             // set ordering for AlongStepDoIt
             pmanager->SetProcessOrdering(theeplusMultipleScattering, idxAlongStep,1);
             pmanager->SetProcessOrdering(theeplusIonisation,         idxAlongStep,2);
             //
             // set ordering for PostStepDoIt
             pmanager->SetProcessOrdering(theeplusMultipleScattering, idxPostStep,1);
             pmanager->SetProcessOrdering(theeplusIonisation,         idxPostStep,2);
             pmanager->SetProcessOrdering(theeplusBremsstrahlung,     idxPostStep,3);
             pmanager->SetProcessOrdering(theeplusAnnihilation,       idxPostStep,4);*/
            
            pmanager->AddProcess(new G4eMultipleScattering,-1, 1, 1);
            pmanager->AddProcess(new G4eIonisation,        -1, 2, 2);
            pmanager->AddProcess(new G4eBremsstrahlung,    -1,-3, 3);
            pmanager->AddProcess(new G4eplusAnnihilation,   0,-1, 4);
            
        } else if (particleName == "mu+" ||
                   particleName == "mu-"    ) {
            
            pmanager->AddProcess(new G4hMultipleScattering,-1, 1, 1);
            pmanager->AddProcess(new G4MuIonisation,       -1, 2, 2);
            pmanager->AddProcess(new G4MuBremsstrahlung,   -1,-3, 3);
            pmanager->AddProcess(new G4MuPairProduction,   -1,-4, 4);
            
            
        } else if (particleName == "alpha" ||
                   particleName == "He3" ||
                   particleName == "GenericIon") {
            
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4ionIonisation,       -1, 2, 2);
            //     pmanager->AddProcess(new G4hLowEnergyIonisation  -1, 2, 2);
            
        } else if (particleName == "pi+" ||
                   particleName == "pi-" ||
                   particleName == "proton" ) {
            
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
            pmanager->AddProcess(new G4hBremsstrahlung,     -1,-3, 3);
            pmanager->AddProcess(new G4hPairProduction,     -1,-4, 4);
            
        } else if (particleName == "B+" ||
                   particleName == "B-" ||
                   particleName == "D+" ||
                   particleName == "D-" ||
                   particleName == "Ds+" ||
                   particleName == "Ds-" ||
                   particleName == "anti_lambda_c+" ||
                   particleName == "anti_omega-" ||
                   particleName == "anti_proton" ||
                   particleName == "anti_sigma_c+" ||
                   particleName == "anti_sigma_c++" ||
                   particleName == "anti_sigma+" ||
                   particleName == "anti_sigma-" ||
                   particleName == "anti_xi_c+" ||
                   particleName == "anti_xi-" ||
                   particleName == "deuteron" ||
                   particleName == "kaon+" ||
                   particleName == "kaon-" ||
                   particleName == "lambda_c+" ||
                   particleName == "omega-" ||
                   particleName == "sigma_c+" ||
                   particleName == "sigma_c++" ||
                   particleName == "sigma+" ||
                   particleName == "sigma-" ||
                   particleName == "tau+" ||
                   particleName == "tau-" ||
                   particleName == "triton" ||
                   particleName == "xi_c+" ||
                   particleName == "xi-" ||
                   (particleName == "nucleus" && charge != 0)) {
            
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
            
        } else if  ((!particle->IsShortLived()) &&
                    (particle->GetPDGCharge() != 0.0) &&
                    (particle->GetParticleName() != "chargedgeantino")) {
            
            pmanager->AddProcess(new G4hMultipleScattering, -1, 1, 1);
            pmanager->AddProcess(new G4hIonisation,         -1, 2, 2);
            
        }
    }
    
    
    G4cout << "MUON_PhysicsList::ConstructEM() :: Done" << G4endl;
}//ConstructEM


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


//////////////////////////////////////////////////////////////////////////////
// Optics             ////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////

#include "G4Cerenkov.hh"
#include "G4OpRayleigh.hh"
#include "G4OpBoundaryProcess.hh"

//#include "BxOpAbsorptionReemission.hh"
#include "G4OpAbsorption.hh"

//#include "BxScintillation.hh"
#include "G4Scintillation.hh"
#include "G4OpWLS.hh"//MBA 03/10/15

void MUON_PhysicsList::ConstructOp()
{
    
    G4cout << "MUON_PhysicsList::ConstructOp()" << G4endl;
    
    G4Scintillation* theScintProcess = new G4Scintillation("Scintillation");
    // G4Scintillation* theBetaScintillationProcess = new G4Scintillation("Scintillation",11);
    //G4Scintillation* theMuScintillationProcess = new G4Scintillation("Scintillation",13);
    //   theScintProcess -> SetTrackSecondariesFirst(true);
 // theScintProcess -> SetTrackSecondariesFirst(false);
    //theScintProcess -> SetScintillationYieldFactor(1.);
    theScintProcess -> SetTrackSecondariesFirst(true);
    
    G4Cerenkov*     theCerenkovProcess = new G4Cerenkov("Cerenkov");
    G4OpRayleigh*   theRayleighScatteringProcess = new G4OpRayleigh();
    
    G4OpAbsorption* theAbsorptionProcess = new G4OpAbsorption();
    G4OpBoundaryProcess* theBoundaryProcess = new G4OpBoundaryProcess();
    G4OpWLS* theReemissionProcess = new G4OpWLS();
    
    
    //theBoundaryProcess->DumpPhysicsTable();
    //theAbsorptionProcess->DumpPhysicsTable();
    
    G4cout << "PhysicsList :: Scintillation Physics Table" << G4endl;
    theScintProcess->DumpPhysicsTable();
    G4cout << "PhysicsList :: End of Scintillation Physics Table" << G4endl;
    
    
    theAbsorptionProcess->SetVerboseLevel(0);
    theRayleighScatteringProcess->SetVerboseLevel(0);
    theBoundaryProcess->SetVerboseLevel(0);
    theScintProcess->SetVerboseLevel(0);
    theReemissionProcess->SetVerboseLevel(0);

    theCerenkovProcess->SetTrackSecondariesFirst(true);
    //theCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumberOfPhoton);
 // theCerenkovProcess->SetMaxNumPhotonsPerStep(300);
    
    /*theMuScintillationProcess->SetTrackSecondariesFirst(true);
     theBetaScintillationProcess->SetTrackSecondariesFirst(true);
     theProtonScintillationProcess->SetTrackSecondariesFirst(true);
     theAlphaScintillationProcess->SetTrackSecondariesFirst(true);
     */
    
    
    auto theParticleIterator=GetParticleIterator();

    theParticleIterator->reset();
    while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4ProcessManager* pmanager = particle->GetProcessManager();
        G4String particleName = particle->GetParticleName();
        
        
        // if (GetOPscintIsActivate()){
        if (particleName == "e+" || particleName == "e-" || particleName == "mu+" || particleName == "mu-"  || particleName == "proton")
        {
            // if (theScintProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(theScintProcess);
            pmanager->SetProcessOrdering(theScintProcess, idxAtRest);
            pmanager->SetProcessOrdering(theScintProcess, idxPostStep);
        }
        /*  if (particleName == "mu+" || particleName == "mu-")   {
         pmanager->AddProcess(theMuScintillationProcess);
         pmanager->SetProcessOrdering(theMuScintillationProcess, idxAtRest);
         pmanager->SetProcessOrdering(theMuScintillationProcess, idxPostStep);
         }
         if (particleName == "proton")   {
         pmanager->AddProcess(theProtonScintillationProcess);
         pmanager->SetProcessOrdering(theProtonScintillationProcess, idxAtRest);
         pmanager->SetProcessOrdering(theProtonScintillationProcess, idxPostStep);
         }
         if (particleName == "alpha")   {
         pmanager->AddProcess(theAlphaScintillationProcess);
         pmanager->SetProcessOrdering(theAlphaScintillationProcess, idxAtRest);
         pmanager->SetProcessOrdering(theAlphaScintillationProcess, idxPostStep);
         }*/
        //}
        
        // if (GetOPcherIsActivate()){
        if (theCerenkovProcess->IsApplicable(*particle)) {
            pmanager->AddProcess(theCerenkovProcess);
            pmanager->SetProcessOrderingToLast(theCerenkovProcess,idxPostStep);
            // pmanager->AddContinuousProcess(theCerenkovProcess);
            //pmanager->SetProcessOrderingToLast(theCerenkovProcess, idxAtRest);
            //pmanager->SetProcessOrderingToLast(theCerenkovProcess, idxPostStep);
        }
        //  }
        
        if (particleName == "opticalphoton") {
            G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
            /*if(AbsorptionIsActive)*/ pmanager->AddDiscreteProcess(theAbsorptionProcess);
            /*if(RayleighIsActive) */  pmanager->AddDiscreteProcess(theRayleighScatteringProcess);
            pmanager->AddDiscreteProcess(theReemissionProcess);//MBA 3/10/15
            pmanager->AddDiscreteProcess(theBoundaryProcess);
        }
    }
    G4cout << "MUON_PhysicsList::ConstructOp() :: Done" << G4endl;
}


//MBA 4/12/15   elastic processes for hadrons
#include "G4HadronElasticPhysics.hh"
void MUON_PhysicsList::ConstructHadEl() {
    G4cout << "MUON_PhysicsList:: Hadronic Elastic Scattering Active" << G4endl;
    G4HadronElasticPhysics* hElPhysicsList = new G4HadronElasticPhysics(1);//verbose
    hElPhysicsList->SetVerboseLevel(0);
    hElPhysicsList->ConstructProcess();
    G4cout << "MUON_PhysicsList:: Hadronic Elastic Scattering :: Done" << G4endl;
}



//QGSP BINARY cascade NEUTRONHP model
//MBA nel 2013 era la meglio per i neutroni
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
void MUON_PhysicsList::ConstructHad_QSGP_BIC_HP() {
    G4cout << "Hadronic Physics Active: QSGP_BIC_HP model" << G4endl;
    G4HadronPhysicsQGSP_BIC_HP *hadPhysicsList = new G4HadronPhysicsQGSP_BIC_HP;
    hadPhysicsList->SetVerboseLevel(0);
    hadPhysicsList->ConstructProcess();
    G4cout << "Hadronic Physics Active: QSGP_BIC_HP model :: Done" << G4endl;
    
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


#include "G4Decay.hh"

void MUON_PhysicsList::ConstructDecay() {
    G4cout << "PhysicsList: decays "<<G4endl;
    // Add Decay Process
    G4Decay* theDecayProcess = new G4Decay();

    auto theParticleIterator=GetParticleIterator();
    theParticleIterator->reset();

    while( (*theParticleIterator)() ){
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4String              particleName      = particle->GetParticleName();
        if(particleName != "mu+"){  
            G4ProcessManager* pmanager = particle->GetProcessManager();
            if (theDecayProcess->IsApplicable(*particle)) { 
                pmanager ->AddProcess(theDecayProcess);
                // set ordering for PostStepDoIt and AtRestDoIt
                pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
                pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
            }
        }
    }
}//ConstructGeneral


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void MUON_PhysicsList::SetCuts()
{
    
    ///special for low energy physics
    G4double lowlimit=250*eV;  
    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowlimit,100.*GeV);
    
    /// set cut values for gamma at first and for e- second and next for e+,
    /// because some processes for e+/e- need cut values for gamma
     
    double cut_em = defaultCutValue;
    G4double cut_had= defaultCutValue;
    G4double cut_ion= 1000*mm;
    
   
    /// NO REASON AT THE MOMENT TO OVERRIDE DEFULT CUT VALUES
 // SetCutValue(gammaCutValue, "gamma");
 // SetCutValue(electronCutValue, "e-");
 // SetCutValue(electronCutValue, "e+");
 // 
 // SetCutValue(cut_had, "proton");
 // SetCutValue(cut_had, "anti_proton");
 // SetCutValue(cut_had, "neutron");
 // 
 // SetCutValue(cut_ion, "alpha");
 // SetCutValue(cut_ion, "GenericIon");

    SetCutsWithDefault(); 
    
    
}