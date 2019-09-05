#ifndef MUON_PhysicsList_h
#define MUON_PhysicsList_h 1

#include "G4VUserPhysicsList.hh"
#include "globals.hh"

class MUON_PhysicsList: public G4VUserPhysicsList {
public:
    MUON_PhysicsList();
    virtual ~MUON_PhysicsList();
    
    void SetSecondaryHad(G4String hadval);
    
private:
    //PhysicsMessenger* physicsMessenger;
    
    G4String SecondaryHadModel;
    
    G4bool gheishahad;
    G4bool bertinihad;
    G4bool binaryhad;
    
    G4double       electronCutValue ;
    G4double       gammaCutValue ;
    
    
protected:
    // Construct particle and physics process
    void ConstructParticle();
    void ConstructProcess();
    void SetCuts();
    
protected:
    // these methods Construct particles
    void ConstructBosons();
    void ConstructLeptons();
    void ConstructMesons();
    void ConstructBaryons();
    
protected:
    // these methods Construct physics processes and register them
    void ConstructGeneral();
    void ConstructEM();
    void ConstructOp();
    void ConstructHad();
    void ConstructHadOld();
    void ConstructHadEl();//MBA 4/12/15
    void ConstructHad_QSGP_BIC_HP();//MBA 4/12/15
    void ConstructDecay(); //MBA 4/12/15
};

#endif