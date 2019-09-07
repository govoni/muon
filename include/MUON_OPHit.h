
#ifndef MUON_OPHit_h
#define MUON_OPHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4String.hh"
#include "tls.hh"

class MUON_OPHit : public G4VHit
{
public:
    MUON_OPHit () ;
    MUON_OPHit (const MUON_OPHit&) ;
    virtual ~MUON_OPHit () ;
    
    // operators
    const MUON_OPHit& operator= (const MUON_OPHit&) ;
    G4int operator== (const MUON_OPHit&) const ;
    
    inline void * operator new (size_t) ;
    inline void  operator delete (void * ) ;
    
    // methods from base class
    virtual void Draw () ;
    virtual void Print () ;
    
    // Set methods
    void set_track_id     (G4int id)           { track_id_    = id ; } ;
    void set_sd_type      (G4int type)         { type_        = type ; } ;
    void set_primary_id   (G4int id)           { primary_id_  = id ; } ;
    void set_secondary_id (G4int id)           { secondary_id_= id ; } ;
    void set_gen_proc     (G4int value)        { gen_proc_    = value ; } ;
    void set_time         (G4double value)     { time_        = value ; } ;
    void set_wavelength   (G4double value)     { wavelength_  = value ; } ;
    void set_position     (G4ThreeVector pos)  { position_    = pos ; } ;
    void set_polarization (G4ThreeVector pol)  { polarization_= pol ; } ;
    
    // Get methods
    G4int    get_track_id     () const {return track_id_    ;} ;
    G4int    get_sd_type      () const {return type_        ;} ;
    G4int    get_primary_id   () const {return primary_id_  ;} ;
    G4int    get_secondary_id () const {return secondary_id_ ;} ;
    G4int    get_gen_proc     () const {return gen_proc_    ;} ;
    G4double get_time         () const {return time_        ;} ;
    G4double get_wavelength   () const {return wavelength_  ;} ;
    const G4ThreeVector &get_position     () const {return position_    ;} ;
    const G4ThreeVector &get_polarization () const {return polarization_ ;} ;


private:
    
    G4int         track_id_ ;
    G4int         type_ ;
    G4int         primary_id_ ;
    G4int         secondary_id_ ;
    G4int         gen_proc_ ;
    G4double      time_ ;
    G4double      wavelength_ ;
    G4ThreeVector position_ ;
    G4ThreeVector polarization_ ;

} ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<MUON_OPHit> MUON_OPHitCollection ;

extern G4ThreadLocal G4Allocator<MUON_OPHit> * MUON_OPHitAllocator ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void * MUON_OPHit::operator new (size_t)
{
    if (!MUON_OPHitAllocator)
        MUON_OPHitAllocator = new G4Allocator<MUON_OPHit> ;

    return (void * ) MUON_OPHitAllocator->MallocSingle () ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void MUON_OPHit::operator delete (void * hit)
{
    MUON_OPHitAllocator->FreeSingle ((MUON_OPHit * ) hit) ;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif