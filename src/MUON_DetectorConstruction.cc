/*
LS = Liquid Scintillator
*/


#include <algorithm>
#include <cmath>
 
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4RunManager.hh"
#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4SDManager.hh"
#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "MUON_DetectorConstruction.h"
#include "MUON_DetectorMessenger.h"
#include "MUON_OpticalPhotonSD.h"
#include "MUON_Verbosity.h"
#include "MUON_LsMatProperties.h"
#include "MUON_OutputLog.h"

using namespace std ;

G4ThreadLocal G4GlobalMagFieldMessenger* MUON_DetectorConstruction::fMagFieldMessenger = 0 ;
 
MUON_DetectorConstruction::MUON_DetectorConstruction () :
  G4VUserDetectorConstruction (),
  logic_ru_sipmt_ (NULL), 
  logic_fiber_ (NULL), 
  fCheckOverlaps (false),
  logic_ls_ (0),
//  ls_mpt (0),
  ls (0),
  air (0),
  is_geom_built (false),
  sipmt_SD_ (0),
  fiber_SD_ (0)
{

    MUON_OutputLog::log_cache << "MUON_DetectorConstruction::MUON_DetectorConstruction ()" << G4endl ;

    /// detector parameter initialization
    ls_size_                             = 1 * m ;
    fibres_spacing_                      = 10.0  * cm ;
    ls_depth_                            = 10.0 * cm ;    
    fiber_radius_                        = 0.25 * mm ;
    outer_cladding_fractional_radius_    = 0.03 ;
    inner_cladding_fractional_radius_    = 0.03 ;

  //define_materials () ;


    /// LS properties are instantiated together with the detector 
    /// so that they can be modified at any time
    my_ls_properties = new MUON_LsMatProperties () ;

    messenger_ = new MUON_DetectorMessenger (this) ;

    /// LS material property table is instantiated on demand in
    /// deploy_ls_properties (), so that several tables can be used
    /// at runtime
    //ls_mpt = new G4MaterialPropertiesTable () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 
 
MUON_DetectorConstruction::~MUON_DetectorConstruction ()
{
  cout << "DEBUG[MUON_DetectorConstruction][dtor] entering " << endl ;

  delete my_ls_properties ;
//  delete ls_mpt ;
  delete messenger_ ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

G4VPhysicalVolume* MUON_DetectorConstruction::Construct ()
{
  cout << "DEBUG[MUON_DetectorConstruction][Construct]" << endl ;

  MUON_OutputLog::log_cache << "***************************************" << G4endl ;
  MUON_OutputLog::log_cache << "*   DEFAULT GEOMETRY IMPLEMENTATION   *" << G4endl ;
  MUON_OutputLog::log_cache << "***************************************" << G4endl ;

  MUON_OutputLog::log_cache << "MUON_DetectorConstruction::Construct ()" << G4endl ;

  /// define all materials but LS
  define_materials () ;

  /// define LS properties
  deploy_ls_properties () ; 

  /// build geometry and return pointer to World volume
  return build_geom () ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

void MUON_DetectorConstruction::define_materials ()
{
  cout << "DEBUG[MUON_DetectorConstruction][define_materials]" << endl ;
  if (MUON_Verbosity::level>1)
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::DefineMaterials ()" << G4endl ;

  G4Element *C  = new G4Element ("Carbon", "C" , 6., 12.01*g/mole) ; 
  G4Element *H  = new G4Element ("Hydrogen", "H" , 1., 1.01*g/mole) ;
  G4Element *O  = new G4Element ("Oxygen", "O", 8., 16.00*g/mole) ; 
  G4Element *N  = new G4Element ("Nitrogen", "N", 7., 14.01*g/mole) ;
  G4Element *Si = new G4Element ("Silicon", "Si", 14., 28.09*g/mole) ;
  G4Element *Al = new G4Element ("Aluminium", "Al", 13., 26.98*g/mole) ;
  G4Element *S  = new G4Element ("Sulfur", "S", 16., 32.066*g/mole) ;
//  G4Element *Pb = new G4Element ("Lead", "Pb", 82., 207.2*g/mole) ;

  // DUMMY FIBER OPTCAL PROPERTIES
  G4double energy_10[10] = 
  { 1.0 * eV,
    2.0 * eV,
    3.0 * eV,
    4.0 * eV,
    5.0 * eV,
    6.0 * eV,
    7.0 * eV,
    8.0 * eV,
    9.0 * eV,
    10.0 * eV} ;

  G4double out_clad_n[10] = 
  { 1.42,
    1.42,
    1.42,
    1.42,
    1.42,
    1.42,
    1.42,
    1.42,
    1.42,
    1.42 } ;

  G4double inn_clad_n[10] = 
  { 1.49,
    1.49,
    1.49,
    1.49,
    1.49,
    1.49,
    1.49,
    1.49,
    1.49,
    1.49 } ;

  G4double core_n[10] = 
  { 1.59, 
    1.59, 
    1.59, 
    1.59, 
    1.59, 
    1.59, 
    1.59, 
    1.59, 
    1.59, 
    1.59 } ;

  G4double fib_att_len[10] = 
  { 16 * m, 
    16 * m, 
    16 * m, 
    16 * m, 
    16 * m, 
    16 * m, 
    16 * m, 
    16 * m, 
    16 * m, 
    16 * m } ;

  G4double core_att_len[10] = 
  { 7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm, 
    7.0E-1 * mm } ;

  G4double black_10[10] = 
  { 1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm, 
    1.0E-9 * mm } ;
  // end of DUMMYFIBER OPTCAL PROPERTIES 

  //  DEFINE LAB-BASED LS
  // --- --- --- --- --- --- 
    
  lab = new G4Material ("LAB", 0.859*g/cm3, 5) ;
  lab->AddElement (C,  0.87924) ;
  lab->AddElement (H,  0.1201) ;
  lab->AddElement (O,  0.00034) ;
  lab->AddElement (N,  0.00027) ;
  lab->AddElement (S,  0.00005) ;

  /// Start with undoped Liquid Scintillator
  ls = lab ;

  /// LS properties are initialized at the moment of geoemtry building
  /// (method Construct () and update_geom ())
  // deploy_ls_properties () ;

  G4NistManager * man = G4NistManager::Instance () ;
  air = man->FindOrBuildMaterial ("G4_AIR") ;

  black_acrylic = new G4Material ("BlackAcrylic", 1.18*g/cm3, 3) ;
  black_acrylic->AddElement (C, 0.59984) ;
  black_acrylic->AddElement (H, 0.08055) ;
  black_acrylic->AddElement (O, 0.31961) ;

  G4MaterialPropertiesTable * black_mpt = new G4MaterialPropertiesTable () ;

  polystyrene = new G4Material ("Polystyrene", 1.05*g/cm3, 2) ;
  polystyrene->AddElement (C, 0.5) ;
  polystyrene->AddElement (H, 0.5) ;

  G4MaterialPropertiesTable * polystyrene_mpt = new G4MaterialPropertiesTable () ;
  polystyrene_mpt->AddProperty ("RINDEX",    energy_10, core_n, 10) ;
  polystyrene_mpt->AddProperty ("ABSLENGTH", energy_10, core_att_len, 10) ;
  polystyrene->SetMaterialPropertiesTable (polystyrene_mpt) ;

  polymethylmethacrylate = new G4Material ("PMMA", 1.19*g/cm3, 3) ;
  polymethylmethacrylate->AddElement (C, 0.336449) ;
  polymethylmethacrylate->AddElement (H, 0.532710) ;
  polymethylmethacrylate->AddElement (O, 0.130841) ;

  G4MaterialPropertiesTable * polymethylmethacrylate_mpt = new G4MaterialPropertiesTable () ;
  polymethylmethacrylate_mpt->AddProperty ("RINDEX",    energy_10, inn_clad_n, 10) ;
  polymethylmethacrylate_mpt->AddProperty ("ABSLENGTH", energy_10, fib_att_len, 10) ;
  polymethylmethacrylate->SetMaterialPropertiesTable (polymethylmethacrylate_mpt) ;

  out_cladding_mat = new G4Material ("OuterCladding", 1.43*g/cm3, 3) ;
  out_cladding_mat->AddElement (C, 0.336449) ;  /// WRONG COMPOSITION
  out_cladding_mat->AddElement (H, 0.532710) ;  /// JUST A PLACEHOLDER
  out_cladding_mat->AddElement (O, 0.130841) ;

  G4MaterialPropertiesTable * out_cladding_mpt = new G4MaterialPropertiesTable () ;
  out_cladding_mpt->AddProperty ("RINDEX",    energy_10, out_clad_n, 10) ;
  out_cladding_mpt->AddProperty ("ABSLENGTH", energy_10, fib_att_len, 10) ;
  out_cladding_mat->SetMaterialPropertiesTable (out_cladding_mpt) ;

  black_mpt->AddProperty ("RINDEX", energy_10,    core_n, 10) ;  /// PLACEHOLDER
  black_mpt->AddProperty ("ABSLENGTH", energy_10, black_10, 10) ;
  black_acrylic->SetMaterialPropertiesTable (black_mpt) ;

  // Print materials
  if (MUON_Verbosity::level > 2)
      MUON_OutputLog::log_cache << * (G4Material::GetMaterialTable ()) << G4endl ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void 
MUON_DetectorConstruction::ConstructSDandField ()
{
/*
  //PG removing sensitive detectors by now, 
  //PG and filling ntuples directly in the SteppingAction

  // Sensitive detectors 
  G4String sipmt_SDname  = "MUON/HoriSiPmtSD" ;
  G4String fiber_SDname  = "MUON/HoriFiberSD" ;

  MUON_OutputLog::log_cache << "---------------------------------------" << G4endl ;
  MUON_OutputLog::log_cache << "***       sensitive detectors       ***" << G4endl ;

  //PG these two are then looked for in MUON_EventAtion
  //PG for the time being fibres are classified as Fibres generically,
  //PG not split into horizontal or vertical ones
  sipmt_SD_ = new MUON_OpticalPhotonSD (sipmt_SDname, "SiPmtHitsCollection", SDType::Pmt) ;
  fiber_SD_ = new MUON_OpticalPhotonSD (fiber_SDname, "FiberHitsCollection", SDType::Fiber) ;

  /// new since Geant4 v10.3
  G4SDManager::GetSDMpointer ()->AddNewDetector (sipmt_SD_) ;
  G4SDManager::GetSDMpointer ()->AddNewDetector (fiber_SD_) ;
  
  if (is_geom_built)
   {
       if (MUON_Verbosity::level>1)
           MUON_OutputLog::log_cache  << "MUON_DetectorConstruction::ConstructSDandField () :: Setting Sensitive Detectors" << G4endl ;
 
      SetSensitiveDetector (logic_ru_sipmt_, sipmt_SD_) ;
      SetSensitiveDetector (logic_fiber_,    fiber_SD_) ;
  }
*/  
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

G4Material *
MUON_DetectorConstruction::make_cocktail (double density, double loading_fraction, G4String& material)
{
  cout << "DEBUG[MUON_DetectorConstruction][make_cocktail]" << endl ;

  /// this method is meant to load the liquid scintillator with a non-native isotope
  if (loading_fraction < 1e-6) return lab ;

  /// define here the list of all material suitable to load LSi at runtime
  G4NistManager* man = G4NistManager::Instance () ;
  G4Material *Pb  = man->FindOrBuildMaterial ("G4_Pb") ;

  /// start the cocktail with simple LAB
  G4Material *a_isotope = lab ;

  /// list here all the available options in terms of loading isotope
  if (material.contains ("Pb") && loading_fraction > 1e-6) a_isotope = Pb ;

  if (MUON_Verbosity::level>1)
  {
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::make_cocktail ()" << G4endl ;
      MUON_OutputLog::log_cache << "                           density = " << density << G4endl ;
      MUON_OutputLog::log_cache << "                           loading fraction = " << loading_fraction << G4endl ;
      MUON_OutputLog::log_cache << "                           loading material = " << a_isotope->GetName () << G4endl ;
  }

  /// define a new name for the doped LS to avoid conflict with previously defined materials
  G4String ls_name ("LS") ;
           ls_name += "_doped_w_" ;
           ls_name += material ;
           ls_name += "_at_" ;
           ls_name += G4UIcommand::ConvertToString (loading_fraction) ;

  G4Material* a_ls = new G4Material (ls_name, density*g/cm3, 2) ;
              a_ls->AddMaterial (lab, 1./ (1.+loading_fraction)) ;
              a_ls->AddMaterial (a_isotope, loading_fraction/ (1.+loading_fraction)) ;

  return a_ls ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

void 
MUON_DetectorConstruction::deploy_ls_properties ()
{
  cout << "DEBUG[MUON_DetectorConstruction][deploy_ls_properties]" << endl ;

  if (MUON_Verbosity::level>1)
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::deploy_ls_properties ()" << G4endl ;
  
  // DEFINE NEW LS 
  // --- --- --- --- --- --- ---   
  /// if (ls) delete ls ; /// ownership of materials is not mine !!!
  ls = make_cocktail (my_ls_properties->get_density (), 
                      my_ls_properties->get_loading_fraction (),
                      my_ls_properties->get_loading_material ()) ;

  MUON_OutputLog::log_cache << "LS PROPERTIES (external input)" << G4endl ;
  MUON_OutputLog::log_cache << "Cocktail Name:                       " << ls->GetName ()  << G4endl ;
  MUON_OutputLog::log_cache << "LS Density:                          " <<  my_ls_properties->get_density () << G4endl ;
  MUON_OutputLog::log_cache << "LS Loading Fraction:                 " <<  my_ls_properties->get_loading_fraction () << G4endl ;
  MUON_OutputLog::log_cache << "LS Loading Material:                 " <<  my_ls_properties->get_loading_material () << G4endl ;
  MUON_OutputLog::log_cache << MUON_OutputLog::ls_cache.str () ;
  MUON_OutputLog::log_cache << "---------------------------------------" <<  G4endl ;

  /// resetting LS log cache
  MUON_OutputLog::ls_cache.str ("") ;
  MUON_OutputLog::ls_cache.clear () ;

  if (MUON_Verbosity::level>1)
  {
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::deploy_ls_properties () :: cocktail prepared" << G4endl ;
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::deploy_ls_properties () :: cocktail name: " << ls->GetName () << G4endl ;
  }

 // if (ls_mpt) delete ls_mpt ; /// if deleted causes seg fault -> G4 ownership?

  G4MaterialPropertiesTable *a_ls_mpt = new G4MaterialPropertiesTable () ;

  a_ls_mpt->AddProperty ("RINDEX",                  my_ls_properties->get_energy (), my_ls_properties->get_ref_index (),     my_ls_properties->get_n_data ()) ;
  a_ls_mpt->AddProperty ("ABSLENGTH",               my_ls_properties->get_energy (), my_ls_properties->get_absorption (),    my_ls_properties->get_n_data ()) ;
  a_ls_mpt->AddProperty ("FASTCOMPONENT",           my_ls_properties->get_energy (), my_ls_properties->get_fast_spectrum (), my_ls_properties->get_n_data ()) ;
  a_ls_mpt->AddProperty ("SLOWCOMPONENT",           my_ls_properties->get_energy (), my_ls_properties->get_slow_spectrum (), my_ls_properties->get_n_data ()) ;
  a_ls_mpt->AddProperty ("RAYLEIGH",                my_ls_properties->get_energy (), my_ls_properties->get_scattering (),    my_ls_properties->get_n_data ()) ;
  a_ls_mpt->AddConstProperty ("SCINTILLATIONYIELD", my_ls_properties->get_light_yield ()) ;
  a_ls_mpt->AddConstProperty ("RESOLUTIONSCALE",    my_ls_properties->get_resolution_scale ()) ;
  a_ls_mpt->AddConstProperty ("FASTTIMECONSTANT",   my_ls_properties->get_fast_time_const ()) ;
  a_ls_mpt->AddConstProperty ("SLOWTIMECONSTANT",   my_ls_properties->get_slow_time_const ()) ;
  a_ls_mpt->AddConstProperty ("YIELDRATIO",         my_ls_properties->get_yield_ratio ()) ;

  ls->SetMaterialPropertiesTable (a_ls_mpt) ;

  ls->GetIonisation ()->SetBirksConstant (my_ls_properties->get_birks_const () *CLHEP::mm/CLHEP::MeV) ;

  if (MUON_Verbosity::level>1)
  {
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::deploy_ls_properties () :: LS properties updated" << G4endl ;
  }


  /// composition of logical volumes relying on new LS definition
  /// is updated at the moment of building new geometry
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void 
MUON_DetectorConstruction::update_geom ()
{
  cout << "DEBUG[MUON_DetectorConstruction][update_geom]" << endl ;

  MUON_OutputLog::log_cache << "MUON_DetectorConstruction::update_geom ()" << G4endl << G4endl ;
  MUON_OutputLog::log_cache << "***************************************" << G4endl ;
  MUON_OutputLog::log_cache << "*   UPDATED GEOMETRY IMPLEMENTATION   *" << G4endl ;
  MUON_OutputLog::log_cache << "***************************************" << G4endl ;

  if (MUON_Verbosity::level>1)
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::update_geom () :: opening geometry" << G4endl ;

  /// reset geometry
  G4GeometryManager::GetInstance ()->OpenGeometry () ;
  G4PhysicalVolumeStore::GetInstance ()->Clean () ;
  G4LogicalVolumeStore::GetInstance ()->Clean () ;
  G4SolidStore::GetInstance ()->Clean () ;

  /// NB geometry needs to be opened not just to modify positioning of detector's components
  /// bul also in order to modify materials
  /// http://geant4.cern.ch/G4UsersDocuments/UsersGuides/ForApplicationDeveloper/html/Detector/geomDynamic.html

  if (MUON_Verbosity::level>1)
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::update_geom () :: updating LS properties" << G4endl ;

  /// update LS properites
  deploy_ls_properties () ; 

  if (MUON_Verbosity::level>1)
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::update_geom () :: updating geometry" << G4endl ;

  /// update detector geometry
  G4RunManager::GetRunManager ()->DefineWorldVolume (build_geom ()) ;

  if (MUON_Verbosity::level>1)
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::update_geom () :: updating sensitive volumes" << G4endl ;

  /// update sensitive volumes
  /// http://hypernews.slac.stanford.edu/HyperNews/geant4/get/geometry/1090/2.html
  logic_ru_sipmt_->SetSensitiveDetector (sipmt_SD_) ;
  logic_fiber_->SetSensitiveDetector (fiber_SD_) ;


  /// issue flag to re-optimize geometry and physics before to run
  G4RunManager::GetRunManager ()->GeometryHasBeenModified () ;
  G4RunManager::GetRunManager ()->PhysicsHasBeenModified () ; 

  if (MUON_Verbosity::level > 1)
      MUON_OutputLog::log_cache << "MUON_DetectorConstruction::update_geom () :: geometry now closed" << G4endl ;

  /// debug: check the material associated to LS logical volume
  G4LogicalVolumeStore *pLVStore = G4LogicalVolumeStore::GetInstance () ;
  G4int nLV = pLVStore->size () ;
  G4int iLV = 0 ;
  G4LogicalVolume *pLV = 0 ;

  for (iLV = 0 ; iLV < nLV && MUON_Verbosity::level > 1 ; ++iLV)
  {
      pLV = (*pLVStore)[iLV] ;
      if (pLV->GetName () == "LS")
      {
          MUON_OutputLog::log_cache << "MUON_DetectorConstruction::update_geom () :: LS Material : " << pLV->GetMaterial ()->GetName () << G4endl ;
       // pLV->GetMaterial ()->GetMaterialPropertiesTable ()->DumpTable () ;
      }
  }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


G4VPhysicalVolume* 
MUON_DetectorConstruction::build_geom ()
{
  cout << "DEBUG[MUON_DetectorConstruction][build_geom]" << endl ;
  cout << "DEBUG Distance between fibres in a plane:  " <<  fibres_spacing_/mm << " mm" << endl ;

  MUON_OutputLog::log_cache  << "MUON_DetectorConstruction::build_geom ()" << G4endl ;

  check_geom_params () ;    
  print_geom_params () ;

  /// pmt geometry
  G4double sipmt_radius      = fiber_radius_ ;
  G4double sipmt_sens_radius = fiber_radius_ * (1.0 - outer_cladding_fractional_radius_) ;
  G4double sipm_thickness    = 1. * mm ; 
  
  /// geometry of fibres and their envelope
  G4double fiber_length         =  ls_size_ ;
  G4double fiber_env_length     =  ls_size_ + 2. * sipm_thickness ;
       
  /// scintillator slab thikness
  G4double ls_z_size = ls_depth_ ;

  /// external world
  G4double world_x_size = ls_size_ + 1.0 * m ; // the sipm size should be less than 1 m
  G4double world_y_size = ls_size_ + 1.0 * m ;
  G4double world_z_size = 1.1 * ls_z_size ;

  int fibres_per_plane = floor (ls_size_ / fibres_spacing_) - 1 ;

//    /// info dump
//    MUON_OutputLog::log_cache << "---------------------------------------" << G4endl ;
//    MUON_OutputLog::log_cache << "***   geometry derived parameters   ***" << G4endl ;
//    MUON_OutputLog::log_cache << "LS size (x,y,z): (" << ls_vessel_x_size/m << ", " << ls_vessel_y_size/m << ", " << ls_vessel_z_size/m << ") m" << G4endl ;
   
  //PG geometry for a muon detector test
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  G4Box             * world_s = new G4Box ("World", 0.5 * world_x_size, 0.5 * world_y_size, 0.5 * world_z_size) ;
  G4LogicalVolume   * world_l = new G4LogicalVolume (world_s, air, "World") ;
  G4VPhysicalVolume * world_p = new G4PVPlacement (0,                 // no rotation
                                                   G4ThreeVector (),  // at (0,0,0)
                                                   world_l,           // its logical volume
                                                   "World",           // its name
                                                   0,                 // its mother volume
                                                   false,             // no boolean operation
                                                   0,                 // copy number
                                                   fCheckOverlaps) ;  // overlaps checking
  
  //PG Liquid Scintillator
  G4Box             * ls_s = new G4Box ("LS", 0.5 * ls_size_, 0.5 * ls_size_, 0.5 * ls_z_size) ;
  G4LogicalVolume   * ls_l = new G4LogicalVolume (ls_s, ls, "LS") ;
  logic_ls_ = ls_l ;
  G4VPhysicalVolume * ls_p = new G4PVPlacement (0, G4ThreeVector (), ls_l, "LS", world_l, false, 0, fCheckOverlaps) ;
 
  //PG fiber readout unit construction starts here
  //PG .... .... .... .... .... .... .... .... .... .... .... 

  /// Readout Unit
  G4Tubs             * ru_s = new G4Tubs ("ru_s", 0, fiber_radius_, 0.5 * fiber_env_length, 0, twopi) ;
  //PG                                    pName, pRMin,    pRMax,          pDz,           pSPhi, pDPhi
  G4LogicalVolume    * ru_l = new G4LogicalVolume (ru_s, ls, "z_readout_unit") ;
  //PG                                                  pSolid, pMaterial,  name

  // Fiber Envelope
  G4Tubs          * fiber_env_s = new G4Tubs ("fiber_env_s", 0., fiber_radius_, 0.5 * fiber_env_length, 0*deg, 360*deg) ;
  G4LogicalVolume * fiber_env_l = new G4LogicalVolume (fiber_env_s, out_cladding_mat, "Fiber Envelope") ;
  //PG                                                    pSolid,           pMaterial,           name

  // Fiber outer cladding
  G4Tubs          * fiber_outer_cladding_s   = new G4Tubs ("fiber_outer_cladding_s", 0. , fiber_radius_, 0.5 * fiber_length, 0*deg, 360*deg) ;
  G4LogicalVolume * fiber_outer_cladding_l   = new G4LogicalVolume (fiber_outer_cladding_s, out_cladding_mat, "Fiber Outer Cladding") ;
  //PG why doesn't this end at the inner cladding radius? FIXME

  // Fiber inner cladding
  G4Tubs          * fiber_inner_cladding_s   = new G4Tubs ("fiber_inner_cladding_s", 0. , 
                                                    (1.-outer_cladding_fractional_radius_) * fiber_radius_, 0.5 * fiber_length, 0*deg, 360*deg) ;
  G4LogicalVolume * fiber_inner_cladding_l   = new G4LogicalVolume (fiber_inner_cladding_s, polymethylmethacrylate, "Fiber Inner Cladding") ;
  //PG why doesn't this end at the core radius? FIXME

  //  Fiber Core
  G4Tubs          * fiber_core_s   = new G4Tubs ("fiber_core_s", 0.,
                                                (1.-outer_cladding_fractional_radius_-inner_cladding_fractional_radius_) * fiber_radius_, 
                                                  0.5 * fiber_length, 0*deg, 360*deg) ;
  G4LogicalVolume * fiber_core_l  = new G4LogicalVolume (fiber_core_s, polystyrene, "Fiber Core") ;

  // SiPMT
  G4Tubs            * ru_sipmt_s = new G4Tubs ("ru_sipmt_s", 0., sipmt_radius, 0.5 * sipm_thickness, 0*deg, 360*deg) ;
  G4LogicalVolume   * ru_sipmt_l = new G4LogicalVolume (ru_sipmt_s, black_acrylic, "RU SiPMT") ;

  // SiPMT Sensitive Volume
  G4Tubs            * ru_sipmt_sens_s = new G4Tubs ("ru_sipmt_sens_s", 0., sipmt_sens_radius , (0.5 * sipm_thickness)/2., 0*deg, 360*deg) ;
  G4LogicalVolume   * ru_sipmt_sens_l = new G4LogicalVolume (ru_sipmt_sens_s, polystyrene, "RU Sensitive SiPMT") ;
  G4VPhysicalVolume * ru_sipmt_sens_p = new G4PVPlacement (0, G4ThreeVector (0.,0.,-0.5 * sipm_thickness + (0.5 * sipm_thickness)/2.), ru_sipmt_sens_l, "SiPMTsens", ru_sipmt_l, false, 0, fCheckOverlaps) ;    
  //PG                                                    pRot,   t(rans)late,                                                           pCurrentLogical, pName,    pMotherLogical, pMany, pCopyNo,  pSurfChk

  /// Fill fiber envelopes with actual fibers and SiPm
  G4VPhysicalVolume* fiber_outer_cladding_p = new G4PVPlacement (0, G4ThreeVector (), fiber_outer_cladding_l, "OuterCladding", fiber_env_l, false, 0, fCheckOverlaps) ;
  G4VPhysicalVolume* fiber_inner_cladding_p = new G4PVPlacement (0, G4ThreeVector (), fiber_inner_cladding_l, "InnerCladding", fiber_outer_cladding_l, false, 0, fCheckOverlaps) ;
  G4VPhysicalVolume* fiber_core_p           = new G4PVPlacement (0, G4ThreeVector (), fiber_core_l, "FiberCore", fiber_inner_cladding_l, false, 0, fCheckOverlaps) ;
  G4VPhysicalVolume* ru_sipmt_top_p         = new G4PVPlacement (0, G4ThreeVector (0.,0.,0.5 * fiber_env_length - 0.5 * sipm_thickness), ru_sipmt_l, "SiPMT", fiber_env_l, false, 0, fCheckOverlaps) ;
  
  G4RotationMatrix rot_sipmt ;
                   rot_sipmt.rotateX (180.*deg) ; 
  
  G4VPhysicalVolume* ru_sipmt_bot_p = new G4PVPlacement (G4Transform3D (rot_sipmt, G4ThreeVector (0.,0.,-0.5 * fiber_env_length + 0.5 * sipm_thickness)), ru_sipmt_l, "SiPMT", fiber_env_l, false, 1, fCheckOverlaps) ;

  G4VPhysicalVolume * physFiberEn1 = new G4PVPlacement (0, G4ThreeVector (), fiber_env_l ,"FiberEnv", ru_l, false, 0, fCheckOverlaps) ; 

  G4double first_fibre_position = -0.5 * fibres_spacing_ * (fibres_per_plane) + 0.5 * fibres_spacing_ ;

  //PG placing the grid of fibres parallel to the y axis
  G4RotationMatrix rot_ru_h ;
  rot_ru_h.rotateX (90. * deg) ; 

  for (int ix = 0 ; ix < fibres_per_plane ; ++ix) 
    {
      G4double xpos = first_fibre_position + static_cast<G4double> (ix) * fibres_spacing_ ; 
      G4double ypos = 0. ;
      G4double zpos = 0. ;
      //PG one actual piece of detector, this creates a G4VPhysicalVolume* 
      new G4PVPlacement (G4Transform3D (rot_ru_h, G4ThreeVector (xpos,ypos,zpos)),
                         ru_l,
                         "readout_unit",
                         ls_l,
                         false,
                         ix,
                         fCheckOverlaps) ;
    }
  //PG if I wanted to add another grid in the other direction, here is the rotation I should use:
  // G4RotationMatrix rot_ru_v ;
  // rot_ru_v.rotateY (90. * deg) ; 

  //PG here my changes finish
  //PG ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 

  /// needed for scoring //FIXME why this, i.e. why not using the same pointer already?
  logic_fiber_    = fiber_core_l ;
  logic_ru_sipmt_ = ru_sipmt_sens_l ;
  
  /// visualization attributes
  G4Colour white  (G4Colour::White  ()) ;  
  G4Colour grey   (G4Colour::Gray   ()) ;  
  G4Colour black  (G4Colour::Black  ()) ; 
  G4Colour red    (G4Colour::Red    ()) ; 
  G4Colour green  (G4Colour::Green  ()) ; 
  G4Colour blue   (G4Colour::Blue   ()) ; 
  G4Colour cyan   (G4Colour::Cyan   ()) ; 
  G4Colour magenta (G4Colour::Magenta ()) ; 
  G4Colour yellow (G4Colour::Yellow ()) ; 
  
  // Visualization attributes
//  G4VisAttributes * vis_white   = new G4VisAttributes (white ) ;
  G4VisAttributes * vis_gray    = new G4VisAttributes (gray  ) ;
//  G4VisAttributes * vis_black   = new G4VisAttributes (black ) ;
//  G4VisAttributes * vis_red     = new G4VisAttributes (red   ) ;
//  G4VisAttributes * vis_green   = new G4VisAttributes (green ) ;
//  G4VisAttributes * vis_blue    = new G4VisAttributes (blue  ) ;
//  G4VisAttributes * vis_cyan    = new G4VisAttributes (cyan  ) ;
//  G4VisAttributes * vis_yellow  = new G4VisAttributes (yellow) ;

  G4VisAttributes * vis_fibre   = new G4VisAttributes (magenta) ;
  G4VisAttributes * vis_sipm    = new G4VisAttributes (green) ;
  G4VisAttributes * vis_ls      = new G4VisAttributes (cyan) ;
  
  vis_fibre->SetForceSolid (true) ;
  vis_sipm->SetForceSolid (true) ;
//  vis_ls->SetForceSolid (true) ;

  world_l->SetVisAttributes (vis_gray) ;
  ls_l   ->SetVisAttributes (vis_ls) ;
  ru_l   ->SetVisAttributes (vis_fibre) ;
   
  G4VisAttributes * fiber_inn_vis_att = new G4VisAttributes (G4Colour (1.0,0.0,0.0)) ;
  G4VisAttributes * fiber_out_vis_att = new G4VisAttributes (G4Colour (0.0,1.0,0.0)) ;
  G4VisAttributes * fiber_cor_vis_att = new G4VisAttributes (G4Colour (0.0,0.0,1.0)) ;
  
  fiber_outer_cladding_l->SetVisAttributes (vis_fibre) ;
  fiber_inner_cladding_l->SetVisAttributes (vis_fibre) ;
  fiber_core_l->SetVisAttributes (vis_fibre) ;
//  fiber_env_l->SetVisAttributes (vis_fibre) ;
  fiber_env_l->SetVisAttributes (G4VisAttributes::GetInvisible ()) ;
  ru_sipmt_l->SetVisAttributes (vis_sipm) ;
  
  is_geom_built = true ;

  if (MUON_Verbosity::level > 1)
      MUON_OutputLog::log_cache << G4endl ;

  return world_p ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void
MUON_DetectorConstruction::print_geom_params ()
{
  cout << "DEBUG[MUON_DetectorConstruction][print_geom_params]" << endl ;

  MUON_OutputLog::log_cache << G4endl << "***    geometry input parameters    ***" << G4endl ;

  MUON_OutputLog::log_cache << "Distance between fibres in a plane:  " <<  fibres_spacing_/mm << " mm" << G4endl ;
  MUON_OutputLog::log_cache << "scintillator slab depth:             " <<  ls_depth_/mm  << " mm" << G4endl ;

  MUON_OutputLog::log_cache << "Fiber Radius:                        " <<  fiber_radius_/mm << " mm" << G4endl ;
  MUON_OutputLog::log_cache << "Fraction of Outer Cladding:          " <<  outer_cladding_fractional_radius_ << G4endl ;
  MUON_OutputLog::log_cache << "Fraction of Inner Cladding:          " <<  inner_cladding_fractional_radius_ << G4endl ;
  return ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void
MUON_DetectorConstruction::check_geom_params ()
{
  cout << "DEBUG[MUON_DetectorConstruction][check_geom_params]" << endl ;

  char line[500] ;

  if (not (fibres_spacing_    > 0.)) 
  {
      sprintf (line, "Invalid fibres_spacing_ (%f mm)", fibres_spacing_/mm) ;
      G4Exception ("MUON_DetectorConstruction::check_geom_params ()",
                   "001",
                   G4ExceptionSeverity::FatalException,
                   line) ;
  }

  if (not (ls_depth_     > 0.))
  {
      sprintf (line, "Invalid ls_depth_ (%f mm)", ls_depth_/mm) ;
      G4Exception ("MUON_DetectorConstruction::check_geom_params ()",
                   "001",
                   G4ExceptionSeverity::FatalException,
                   line) ;
  }

  if (not (ls_size_     > 0.))
  {
      sprintf (line, "Invalid ls_size_ (%f mm)", ls_size_/mm) ;
      G4Exception ("MUON_DetectorConstruction::check_geom_params ()",
                   "001",
                   G4ExceptionSeverity::FatalException,
                   line) ;
  }

  if (fiber_radius_ < 0. || fiber_radius_ > ls_size_ || fiber_radius_ > ls_depth_)
  {
      sprintf (line, "Invalid fiber_radius_ (%f mm)", fiber_radius_/mm) ;
      G4Exception ("MUON_DetectorConstruction::check_geom_params ()",
                   "001",
                   G4ExceptionSeverity::FatalException,
                   line) ;
  }

  if (outer_cladding_fractional_radius_ < 0. || outer_cladding_fractional_radius_ > 1.0)
  {
      sprintf (line, "Invalid outer_cladding_fractional_radius_ (%f mm)", outer_cladding_fractional_radius_/mm) ;
      G4Exception ("MUON_DetectorConstruction::check_geom_params ()",
                   "001",
                   G4ExceptionSeverity::FatalException,
                   line) ;
  }

  if (inner_cladding_fractional_radius_ < 0. || (outer_cladding_fractional_radius_+inner_cladding_fractional_radius_) > 1.0)
  {
      sprintf (line, "Invalid inner_cladding_fractional_radius_ (%f mm) - outer cladding is %f mm", 
              inner_cladding_fractional_radius_/mm,
              outer_cladding_fractional_radius_/mm) ;
      G4Exception ("MUON_DetectorConstruction::check_geom_params ()",
                   "001",
                   G4ExceptionSeverity::FatalException,
                   line) ;
  }
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void 
MUON_DetectorConstruction::set_ls_dummy_absorption (double value)
{
    if (MUON_Verbosity::level>1)
        MUON_OutputLog::log_cache << "MUON_DetectorConstruction::set_ls_dummy_absorption" << G4endl ;
    my_ls_properties->set_dummy_absorption (value) ;

    MUON_OutputLog::ls_cache << "LS Absorption:                       " <<  value/m << " m" << G4endl ;
}


void 
MUON_DetectorConstruction::set_ls_dummy_scattering (double value)
{
    if (MUON_Verbosity::level>1)
        MUON_OutputLog::log_cache << "MUON_DetectorConstruction::set_ls_dummy_scattering" << G4endl ;
    my_ls_properties->set_dummy_scattering (value) ;

    MUON_OutputLog::ls_cache << "LS Scattering:                       " <<  value/cm << " cm" << G4endl ;
}


void 
MUON_DetectorConstruction::set_ls_density (double value)
{
    if (MUON_Verbosity::level>1)
        MUON_OutputLog::log_cache << "MUON_DetectorConstruction::set_ls_density" << G4endl ;

    my_ls_properties->set_density (value) ;
}


void 
MUON_DetectorConstruction::set_ls_birks (double value)
{
    if (MUON_Verbosity::level>1)
        MUON_OutputLog::log_cache << "MUON_DetectorConstruction::set_ls_birks" << G4endl ;

    my_ls_properties->set_birks_const (value) ;
}


void 
MUON_DetectorConstruction::set_ls_loading_fraction (double value)
{
    if (MUON_Verbosity::level>1)
        MUON_OutputLog::log_cache << "MUON_DetectorConstruction::set_ls_loading_fraction" << G4endl ;

    my_ls_properties->set_loading_fraction (value) ;
}


void 
MUON_DetectorConstruction::set_ls_loading_material (G4String & value)
{
    if (MUON_Verbosity::level>1)
        MUON_OutputLog::log_cache << "MUON_DetectorConstruction::set_ls_loading_material" << G4endl ;

    my_ls_properties->set_loading_material (value) ;
}

