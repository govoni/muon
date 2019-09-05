

#ifndef MUON_ActionInitialization_h
#define MUON_ActionInitialization_h 1

#include "G4VUserActionInitialization.hh"
#include "MUON_DetectorConstruction.h"

//class B4DetectorConstruction;

/// Action initialization class.
///

class MUON_ActionInitialization : public G4VUserActionInitialization
{
  public:
    MUON_ActionInitialization( MUON_DetectorConstruction* det );
    virtual ~MUON_ActionInitialization();

    virtual void BuildForMaster() const;
    virtual void Build() const;

  private :
    MUON_DetectorConstruction* fDetector;
};

#endif

    
