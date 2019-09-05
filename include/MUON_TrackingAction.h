#ifndef MUON_TrackingInformation
#define MUON_TrackingInformation 1

#include "G4UserTrackingAction.hh"
#include "globals.hh"

class MUON_TrackingAction : public G4UserTrackingAction {

  public:  
    MUON_TrackingAction( );
   ~MUON_TrackingAction() {};
   
    virtual void PostUserTrackingAction(const G4Track*);
    
//private:
//  PrimaryGeneratorAction* fPrimary;
};

#endif