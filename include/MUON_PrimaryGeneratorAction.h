
#ifndef MUON_PrimaryGeneratorAction_h
#define MUON_PrimaryGeneratorAction_h 1

#include "G4GeneralParticleSource.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"

//class G4ParticleGun;
class G4Event;
class G4GeneralParticleSource;

/// The primary generator action class with particle gum.
///
/// It defines a single particle which hits the Tracker 
/// perpendicular to the input face. The type of the particle
/// can be changed via the G4 build-in commands of G4ParticleGun class 
/// (see the macros provided with this example).

class MUON_PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    MUON_PrimaryGeneratorAction();    
    virtual ~MUON_PrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event* );

    // G4ParticleGun* GetParticleGun() {return fParticleGun;}
    G4GeneralParticleSource* GetParticleGun() {return fParticleGun;}
  
    // Set methods
    void SetRandomFlag(G4bool );

  private:
 // G4ParticleGun*          fParticleGun; // G4 particle gun
    G4GeneralParticleSource*          fParticleGun; // G4 particle gun
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif