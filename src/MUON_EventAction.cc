#include <vector>

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

#include "g4root.hh" 

#include "MUON_EventAction.h"
//#include "MUON_SiPmtHit.h"
//#include "MUON_FiberHit.h"
//#include "MUON_VesselHit.h"
#include "MUON_Verbosity.h"
//#include "MUON_FiberSD.h"
#include "MUON_OpticalPhotonSD.h"
#include "MUON_OutputNtuples.h"
#include "MUON_GlobalNtuplesPtr.h" 

using namespace std ;

MUON_EventAction::MUON_EventAction() : G4UserEventAction(),
                                       fiber_hit_collection_id_ (-1),
                                       sipmt_hit_collection_id_ (-1)
{
  //cout << "DEBUG[MUON_EventAction][ctor]" << endl ;
  if (MUON_Verbosity::level > 1) 
    G4cout << "MUON_EventAction()" << G4endl ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

MUON_EventAction::~MUON_EventAction ()
{
  //cout << "DEBUG[MUON_EventAction][dtor]" << endl ;
  if (MUON_Verbosity::level>1) 
    G4cout << "~MUON_EventAction()" << G4endl ;
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

void MUON_EventAction::BeginOfEventAction (const G4Event* event)
{
  //cout << "DEBUG[MUON_EventAction][BeginOfEventAction]" << endl ;
  if ( MUON_Verbosity::level>1) 
      G4cout << "Begin of Event Action" << G4endl ;


}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

void 
MUON_EventAction::EndOfEventAction (const G4Event* event)
{
    //cout << "DEBUG[MUON_EventAction][EndOfEventAction]" << endl ;
    G4int eventID = event->GetEventID();

  //if( eventID < 100 || eventID%100 == 0) 
  if ( MUON_Verbosity::level>1) 
      G4cout << "EndOfEventAction  ev:" << eventID << G4endl;

/*
  //PG removing sensitive detectors by now, 
  //PG and filling ntuples directly in the SteppingAction

  /// fetch collection IDs
  if ( fiber_hit_collection_id_ == -1 ) fiber_hit_collection_id_ = G4SDManager::GetSDMpointer ()->GetCollectionID ("FiberHitsCollection") ;
  if ( sipmt_hit_collection_id_ == -1 ) sipmt_hit_collection_id_ = G4SDManager::GetSDMpointer ()->GetCollectionID ("SiPmtHitsCollection") ;

  process_hit_collections (event) ;
*/
//  fetch_event_info (event) ;

  //PG info on the input particles
  global_ntuples_light_ptr->fill_bullet (
      event->GetPrimaryVertex ()->GetX0 () / cm                         ,
      event->GetPrimaryVertex ()->GetY0 () / cm                         ,
      event->GetPrimaryVertex ()->GetZ0 () / cm                         ,
//      event->GetPrimaryVertex ()->GetPrimary ()->GetPx () / GeV         ,   
//      event->GetPrimaryVertex ()->GetPrimary ()->GetPy () / GeV         ,   
//      event->GetPrimaryVertex ()->GetPrimary ()->GetPz () / GeV         ,   
      event->GetPrimaryVertex ()->GetPrimary ()->GetTotalEnergy () / GeV,            
      event->GetPrimaryVertex ()->GetPrimary ()->GetPDGcode ()                  
    ) ;

  global_ntuples_light_ptr->store_event () ;

}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 


void
MUON_EventAction::fetch_event_info (const G4Event* evt)
{
    /// TODO
    //  To be implemented
}


// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- 
 

G4int 
MUON_EventAction::process_hit_collections (const G4Event* evt)
{
  //cout << "DEBUG[MUON_EventAction][process_hit_collections]" << endl ;
  if ( MUON_Verbosity::level>1) 
      G4cout << "EventAction::process_hit_collections()" << G4endl ;

  /// setup container of hit collections
  G4HCofThisEvent* HCofEvent = evt->GetHCofThisEvent () ;

  const int n_collections = 2 ;

  vector<MUON_OPHitCollection *> collection_container (2, 0) ;

  /// fetch hit collections 
  collection_container[0] = (MUON_OPHitCollection*) (HCofEvent->GetHC (fiber_hit_collection_id_)) ;
  collection_container[1] = (MUON_OPHitCollection*) (HCofEvent->GetHC (sipmt_hit_collection_id_)) ;

  /// determine collection size
  G4int n_fiber_hits      = collection_container.at (0)->entries () ;
  G4int n_sipmt_hits      = collection_container.at (1)->entries () ;
  G4int n_tot_hits        = n_fiber_hits + n_sipmt_hits ; 


  if ( MUON_Verbosity::level>0) 
  {
      G4cout << "EndOfEventAction :: n_fiber_hits = " << n_fiber_hits << G4endl ; 
  }

  MUON_OPHit *a_hit;

  for (int iC = 0 ; iC < n_collections ; ++iC)
  {
      int n_hits = collection_container.at (iC)->entries () ;

      for (int iH = 0; iH < n_hits; ++iH)
      {
          a_hit = (*(collection_container.at (iC)))[iH] ;
          global_ntuples_ptr->fill_hit (a_hit) ;
      }
  }
  return n_tot_hits;
}  

