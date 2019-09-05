#include "MUON_DetectorMessenger.h"
#include "MUON_LsMatProperties.h"
#include "MUON_DetectorConstruction.h"
#include "MUON_ReadoutUnitGeometry.h"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"

//#include "CLHEP/Units/SystemOfUnits.h"
#include "G4UnitsTable.hh"

using namespace std ;


MUON_DetectorMessenger::MUON_DetectorMessenger( MUON_DetectorConstruction *det ) : 
  G4UImessenger (),
  det_(det)
{
    ofos_directory_ = new G4UIdirectory("/MUON/");
    ofos_directory_->SetGuidance("UI commands to modify MUON settings");
    
    ls_directory_ = new G4UIdirectory("/MUON/ls/");
    ls_directory_->SetGuidance("Liquid Scintillator Parameters");

    geom_directory_ = new G4UIdirectory("/MUON/geom/");
    geom_directory_->SetGuidance("Geometry Parameters");

    detector_directory_ = new G4UIdirectory("/MUON/detector/");
    detector_directory_->SetGuidance("Detector-related global commands");

    absorption_length_cmd_ = new G4UIcmdWithADoubleAndUnit("/MUON/ls/absorption",this);
    absorption_length_cmd_->SetGuidance("LS Absorption Length");
    absorption_length_cmd_->SetParameterName("ls_absorption",false);
    absorption_length_cmd_->SetUnitCategory("Length");
    absorption_length_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    scattering_length_cmd_ = new G4UIcmdWithADoubleAndUnit("/MUON/ls/scattering",this);
    scattering_length_cmd_->SetGuidance("LS Scattering Length");
    scattering_length_cmd_->SetParameterName("ls_scattering",false);
    scattering_length_cmd_->SetUnitCategory("Length");
    scattering_length_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

 // G4UnitDefinition ( "g/cm3" , "g/cm3", "Density", g/cm3 );

    birks_cmd_ = new G4UIcmdWithADouble("/MUON/ls/birks",this);
    birks_cmd_->SetGuidance("LS Birks' Constant [mm/MeV]");
    birks_cmd_->SetParameterName("birks_const", false);
    birks_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    density_cmd_ = new G4UIcmdWithADouble("/MUON/ls/density",this);
    density_cmd_->SetGuidance("LS Density");
    density_cmd_->SetParameterName("ls_density",false);
//  density_cmd_->SetOmittable(false);
    density_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    loading_fraction_cmd_ = new G4UIcmdWithADouble("/MUON/ls/loading_fraction",this);
    loading_fraction_cmd_->SetGuidance("Grams of Loading Isotope per Grams of LAB");
    loading_fraction_cmd_->SetParameterName("ls_loading_fraction",false);
    loading_fraction_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);

    loading_material_cmd_ =  new G4UIcmdWithAString("/MUON/ls/loading_material", this);
    loading_material_cmd_->SetGuidance("Isotope to be loaded in LAB");
    loading_material_cmd_->SetParameterName("ls_loading_material",false);
    loading_material_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);       

    /// this command is now embedded in detector/build
    /// in order to deploy ls properties before modifying the geometry
 // deploy_properties_cmd_ =  new G4UIcmdWithoutParameter("/MUON/ls/deploy_properties", this);
 // deploy_properties_cmd_->SetGuidance("Instantiate LS Material Property Table");
 // deploy_properties_cmd_->AvailableForStates(G4State_PreInit,G4State_Idle);       

    ls_size_cmd_ = new G4UIcmdWithADoubleAndUnit ("/MUON/geom/ls_size", this) ;
    ls_size_cmd_->SetGuidance ("lateral size of the scintillating slab") ;
    ls_size_cmd_->SetUnitCategory ("Length") ;
    ls_size_cmd_->AvailableForStates (G4State_PreInit, G4State_Idle) ;

    fibres_spacing_cmd_ = new G4UIcmdWithADoubleAndUnit ("/MUON/geom/fibres_spacing", this) ;
    fibres_spacing_cmd_->SetGuidance ("spacing between fibres axes in each readout plane") ;
    fibres_spacing_cmd_->SetUnitCategory ("Length") ;
    fibres_spacing_cmd_->AvailableForStates (G4State_PreInit, G4State_Idle) ;

    ls_depth_cmd_ = new G4UIcmdWithADoubleAndUnit ("/MUON/geom/ls_depth", this) ;
    ls_depth_cmd_->SetGuidance ("distance between readout planes") ;
    ls_depth_cmd_->SetUnitCategory ("Length") ;
    ls_depth_cmd_->AvailableForStates (G4State_PreInit, G4State_Idle) ;

    fiber_radius_cmd_ = new G4UIcmdWithADoubleAndUnit ("/MUON/geom/fiber_radius", this) ;
    fiber_radius_cmd_->SetGuidance ("fibre radius") ;
    fiber_radius_cmd_->SetUnitCategory ("Length") ;
    fiber_radius_cmd_->AvailableForStates (G4State_PreInit, G4State_Idle) ;

    outer_cladding_fractional_radius_cmd_ = new G4UIcmdWithADouble ("/MUON/geom/out_cladding_frac", this) ;
    outer_cladding_fractional_radius_cmd_->SetGuidance ("Outer Cladding Fractional Radius") ;
    outer_cladding_fractional_radius_cmd_->AvailableForStates (G4State_PreInit, G4State_Idle) ;

    inner_cladding_fractional_radius_cmd_ = new G4UIcmdWithADouble ("/MUON/geom/inn_cladding_frac", this) ;
    inner_cladding_fractional_radius_cmd_->SetGuidance ("Inner Cladding Fractional Radius") ;
    inner_cladding_fractional_radius_cmd_->AvailableForStates (G4State_PreInit, G4State_Idle) ;

    build_geometry_cmd_= new G4UIcmdWithoutParameter ("/MUON/detector/build", this) ;
    build_geometry_cmd_->SetGuidance ("Build detector") ;
    build_geometry_cmd_->AvailableForStates (G4State_PreInit, G4State_Idle) ;       
 
}



// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 



MUON_DetectorMessenger::~MUON_DetectorMessenger()
{
    cout << "DEBUG[MUON_DetectorMessenger][dtor]" << endl ;
    G4cout << ":~MUON_DetectorMessenger()" << G4endl ;

    delete ofos_directory_                       ;
    delete ls_directory_                         ;
    delete geom_directory_                       ;
    delete detector_directory_                   ;
    delete absorption_length_cmd_                ;
    delete scattering_length_cmd_                ;
    delete birks_cmd_                            ;
    delete density_cmd_                          ;
    delete loading_fraction_cmd_                 ;
    delete loading_material_cmd_                 ;
    delete ls_size_cmd_                          ;
    delete fibres_spacing_cmd_                   ;
    delete ls_depth_cmd_                         ;
    delete fiber_radius_cmd_                     ;
    delete outer_cladding_fractional_radius_cmd_ ;
    delete inner_cladding_fractional_radius_cmd_ ;
    delete build_geometry_cmd_                   ;

    G4cout << "Done" << G4endl;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void MUON_DetectorMessenger::SetNewValue (G4UIcommand * command, G4String newValue)
{
  cout << "DEBUG[MUON_DetectorMessenger][SetNewValue]" << endl ;
  cout << "DEBUG[MUON_DetectorMessenger][SetNewValue] " << newValue << endl ;

  if( command == absorption_length_cmd_ )
      { det_->set_ls_dummy_absorption(absorption_length_cmd_->GetNewDoubleValue(newValue));}
  
  if( command == scattering_length_cmd_ )
      { det_->set_ls_dummy_scattering(scattering_length_cmd_->GetNewDoubleValue(newValue));}

  if( command == birks_cmd_ )
      { det_->set_ls_birks(birks_cmd_->GetNewDoubleValue(newValue));}

  if( command == density_cmd_ )
      { det_->set_ls_density(density_cmd_->GetNewDoubleValue(newValue));}

  if( command == loading_fraction_cmd_ )
      { det_->set_ls_loading_fraction(loading_fraction_cmd_->GetNewDoubleValue(newValue));}

  if( command == loading_material_cmd_ )
      { det_->set_ls_loading_material(newValue);}

  if (command == ls_size_cmd_)
    det_->Set_ls_size (ls_size_cmd_->GetNewDoubleValue (newValue)) ;

  if (command == fibres_spacing_cmd_)
    det_->Set_fibres_spacing (fibres_spacing_cmd_->GetNewDoubleValue (newValue)) ;

  if (command == ls_depth_cmd_)
    det_->Set_ls_depth (ls_depth_cmd_->GetNewDoubleValue (newValue)) ;

  if (command == fiber_radius_cmd_)
    det_->Set_fiber_radius (fiber_radius_cmd_->GetNewDoubleValue (newValue)) ;

  if (command == outer_cladding_fractional_radius_cmd_)
    det_->Set_outer_cladding_fractional_radius (outer_cladding_fractional_radius_cmd_->GetNewDoubleValue (newValue)) ;

  if (command == inner_cladding_fractional_radius_cmd_)
    det_->Set_inner_cladding_fractional_radius (inner_cladding_fractional_radius_cmd_->GetNewDoubleValue (newValue)) ;

  if (command == build_geometry_cmd_)
    {
      cout << "DEBUG[MUON_DetectorMessenger][SetNewValue] build geometry" << endl ;
      G4cout << "MUON_DetectorMessenger :: calling update_geom ()" << G4endl ;
      det_->update_geom () ;
    }
    

}
