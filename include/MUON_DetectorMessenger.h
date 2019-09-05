#ifndef MUON_DetectorMessenger_h
#define MUON_DetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

//class MUON_LsMatProperties ;
class MUON_DetectorConstruction ; 
class G4UIdirectory ;
class G4UIcmdWithAString ;
class G4UIcmdWithADoubleAndUnit ;
class G4UIcmdWithADouble ;
class G4UIcmdWithoutParameter ;
class G4UIcmdWithAnInteger ;
class G4UIcmdWithAString ;

class MUON_DetectorMessenger: public G4UImessenger
{
  public:
    MUON_DetectorMessenger( MUON_DetectorConstruction * det ) ;
    virtual ~MUON_DetectorMessenger() ;
    
    virtual void SetNewValue (G4UIcommand* , G4String) ;
    
  private:
    MUON_DetectorConstruction * det_ ;

    G4UIdirectory *           ofos_directory_ ;
    G4UIdirectory *           ls_directory_ ;
    G4UIdirectory *           geom_directory_ ;
    G4UIdirectory *           detector_directory_ ;

    /// liquid scintillator commands
    G4UIcmdWithADoubleAndUnit * absorption_length_cmd_ ;
    G4UIcmdWithADoubleAndUnit * scattering_length_cmd_ ;
    G4UIcmdWithADouble        * birks_cmd_ ;
    G4UIcmdWithADouble        * density_cmd_ ;
    G4UIcmdWithADouble        * loading_fraction_cmd_ ;
    G4UIcmdWithAString        * loading_material_cmd_ ;
 // G4UIcmdWithoutParameter   * deploy_properties_cmd_ ;

    /// geometry commands
    G4UIcmdWithADoubleAndUnit * ls_size_cmd_                          ;
    G4UIcmdWithADoubleAndUnit * fibres_spacing_cmd_                   ;
    G4UIcmdWithADoubleAndUnit * ls_depth_cmd_                         ;
    G4UIcmdWithADoubleAndUnit * fiber_radius_cmd_                     ;
    G4UIcmdWithADouble        * outer_cladding_fractional_radius_cmd_ ;
    G4UIcmdWithADouble        * inner_cladding_fractional_radius_cmd_ ;

    G4UIcmdWithoutParameter   * build_geometry_cmd_ ;

} ;



#endif