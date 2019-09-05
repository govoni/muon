
#ifndef MUON_DetectorConstruction_h
#define MUON_DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4MaterialPropertiesTable.hh"
#include "tls.hh"

#include "MUON_LsMatProperties.h"
#include "MUON_ReadoutUnitGeometry.h"

//#include "MUON_SiPmtSD.h"
//#include "MUON_FiberSD.h"
//#include "MUON_VesselSD.h"
#include "MUON_OpticalPhotonSD.h"

class G4VPhysicalVolume ;
class G4LogicalVolume ;
class G4Material ;
class G4UserLimits ;
class G4GlobalMagFieldMessenger ;
class MUON_DetectorMessenger ;


/// Detector construction class to define materials, geometry
/// and global uniform magnetic field.


class MUON_DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    MUON_DetectorConstruction () ;
    virtual ~MUON_DetectorConstruction () ;

    virtual G4VPhysicalVolume * Construct () ;
    virtual void ConstructSDandField () ;

    // Set methods

    void SetCheckOverlaps (G4bool) ;

    /// define ls composition
    G4Material*  make_cocktail (double density, double loading_fraction, G4String& material) ;

    /// act on MUON_LsMatProperties
    void set_ls_dummy_absorption (double value) ; 
    void set_ls_dummy_scattering (double value) ; 
    void set_ls_birks            (double value) ;
    void set_ls_density          (double value) ;
    void set_ls_loading_fraction (double value) ;
    void set_ls_loading_material (G4String & value) ;

    /// act on G4MaterialPropertiesTable
    void deploy_ls_properties () ;

    void update_geom () ;

    inline G4Material * get_ls_material () { return (logic_ls_ ? logic_ls_->GetMaterial () : 0) ;}

//    void SetCheckOverlaps(G4bool );

    void Set_fibres_spacing                   (G4double var) {fibres_spacing_                   = var ; return ; }
    void Set_ls_depth                         (G4double var) {ls_depth_                         = var ; return ; }
    void Set_ls_size                          (G4double var) {ls_size_                          = var ; return ; }
    void Set_fiber_radius                     (G4double var) {fiber_radius_                     = var ; return ; }
    void Set_outer_cladding_fractional_radius (G4double var) {outer_cladding_fractional_radius_ = var ; return ; }
    void Set_inner_cladding_fractional_radius (G4double var) {inner_cladding_fractional_radius_ = var ; return ; }

  private:

    // methods
    void define_materials () ;
 // G4VPhysicalVolume*  DefineVolumes() ;
    G4VPhysicalVolume*  build_geom () ;
    void check_geom_params () ;
    void print_geom_params () ;
  
    // data members
    G4LogicalVolume * logic_ru_sipmt_ ;    // pointer to the logical SiPmt for scoring
    G4LogicalVolume * logic_fiber_ ;       // pointer to the logical Fiber for scoring
    G4LogicalVolume * logic_ls_ ;          // pointer to the logical LS to allow live material modification

   /*************** 
    *   MATERIALS  * 
    ***************/

    G4Material                * ls ; /// need to be data member in order to change its properties at run time
    G4Material                * lab ;
    MUON_LsMatProperties      * my_ls_properties ;
    G4MaterialPropertiesTable * ls_mpt ;

    G4Material * out_cladding_mat ;
    G4Material * polymethylmethacrylate ; // fiber inner cladding
    G4Material * polystyrene ; // fiber core
    G4Material * black_acrylic ;
    G4Material * air ;

   /*************** 
    *   GEOMETRY   * 
    ***************/
    G4bool is_geom_built ;

    // readout units arrangement
    G4double            fibres_spacing_ ;
    G4double            ls_depth_ ;
    G4double            ls_size_ ;

    // single readout unit geometry
    G4double            fiber_radius_ ;
    G4double            outer_cladding_fractional_radius_ ;
    G4double            inner_cladding_fractional_radius_ ;

   /************************** 
    *   SENSITIVE DETECTORS   * 
    **************************/

    MUON_OpticalPhotonSD * sipmt_SD_ ;
    MUON_OpticalPhotonSD * fiber_SD_ ;

 // MUON_OpticalPhotonSD * optical_photon_sd_ ;

    MUON_DetectorMessenger * messenger_ ;

    static G4ThreadLocal G4GlobalMagFieldMessenger*   fMagFieldMessenger ; 
                                         // magnetic field messenger
    
    G4bool  fCheckOverlaps ; // option to activate checking of volumes overlaps 

} ;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif