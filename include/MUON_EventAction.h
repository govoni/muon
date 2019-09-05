#ifndef MUON_EventAction_h
#define MUON_EventAction_h 1

#include <vector>

#include "G4UserEventAction.hh"

#include "globals.hh"



class MUON_EventAction : public G4UserEventAction
{

public:
  MUON_EventAction();
  virtual ~MUON_EventAction();
  

  // data members
   G4int  fiber_hit_collection_id_;
   G4int  sipmt_hit_collection_id_;

  virtual void  BeginOfEventAction(const G4Event* );
  virtual void    EndOfEventAction(const G4Event* );
  

private:
 
    G4int process_hit_collections ( const G4Event* evt );
    void fetch_event_info( const G4Event* evt );

};



#endif