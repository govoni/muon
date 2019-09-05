#include "MUON_TrackingAction.h"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4ProcessType.hh"
#include "G4VProcess.hh"

#include "MUON_GlobalNtuplesPtr.h"

using namespace std ;

MUON_TrackingAction::MUON_TrackingAction() :G4UserTrackingAction()
{ 
  cout << "DEBUG[MUON_TrackingAction][ctor]" << endl ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void MUON_TrackingAction::PostUserTrackingAction(const G4Track* track)
{
    // cout << "DEBUG[MUON_TrackingAction][PostUserTrackingAction]" << endl ;
    G4ParticleDefinition * particleDefinition =  track->GetDefinition () ;
    G4int pdgid = particleDefinition->GetPDGEncoding () ;

    /// only in the case of gammas, keep track of the creation vertex
    if(pdgid == 22)
    {
        G4int         track_id       = track->GetTrackID();
        G4ThreeVector start_position = track->GetVertexPosition();
        G4double      start_time     = track->GetGlobalTime() - track->GetLocalTime() ;
        G4ProcessType proc_type      = G4ProcessType::fNotDefined ;
        G4int         proc_subtype   = 0;

        if( track->GetCreatorProcess() != NULL )
        {
            proc_type      = track->GetCreatorProcess()->GetProcessType();
            proc_subtype   = track->GetCreatorProcess()->GetProcessSubType(); 
        }

        /// interaction_id is negative, because technically speaking the process 
        /// responsible for the creation of this track belongs to the parent track
        G4int    interaction_id      = -1 * (static_cast<G4int>(proc_type) * 1000 + proc_subtype);

     // G4cout << "Tracking Action ::" << G4endl;
     // G4cout << "tr id: " << track_id << "  int id: " << interaction_id << "  time: " << start_time << "  pos (" << start_position.x() 
     //      << "," << start_position.y() << "," << start_position.z() << ")" << G4endl;


        /// not able to store photons in output storage >>> do nothing
        if(!global_ntuples_ptr) return;

        global_ntuples_ptr->fill_interaction( track->GetTrackID(),
                                              track->GetParentID(),  
                                              interaction_id, 
                                              pdgid,
                                              start_position,
                                              0., // dE
                                              start_time );
    } // if(pdgid == 22)

}
