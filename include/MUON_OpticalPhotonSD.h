
#ifndef MUON_OpticalPhotonSD_h
#define MUON_OpticalPhotonSD_h 1

#include "G4VSensitiveDetector.hh"

#include "MUON_OPHit.h"

#include <vector>

class G4Step;
class G4HCofThisEvent;

enum class SDType     { Undefined=0, VerticalFiber=1, HorizontalFiber=2, Fiber=3, Pmt=4, Vessel=8 };
enum       my_op_proc { my_undefined_proc=-9, my_scintillation=0, my_cherenkov=1}; 
enum       g4_op_proc { g4_scintillation=22, g4_cherenkov = 21};

/// allowing the use of masks on SDType
inline G4bool operator&( const SDType& lhs, const SDType& rhs) 
{
    return ( static_cast<G4int>(lhs) & static_cast<G4int>(rhs) );
}

class MUON_OpticalPhotonSD : public G4VSensitiveDetector
{
  public:
    MUON_OpticalPhotonSD(const G4String& name, const G4String& hitsCollectionName, SDType type = SDType::Undefined);
    virtual ~MUON_OpticalPhotonSD();
  
    // methods from base class
    virtual void   Initialize  (G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits (G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent (G4HCofThisEvent* hitCollection);

    /// this meethod could in principle be avoided: from the stepping action
    /// the hit could sent directly to the OutputNtuples class to be stored
    /// provided that the SD_type is retrieved within the stepping action
    G4bool process_hit( const G4Step* step, const G4TouchableHistory* history );

    inline SDType get_sd_type() {return type_;};
    inline G4String get_name() { return SensitiveDetectorName; };

   
  private:
    MUON_OPHitCollection* hit_collection_;
    G4String sd_name_;
    SDType type_;
    const G4double eV_to_nm_;
 
};


#endif
