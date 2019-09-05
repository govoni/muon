# ongoing work and todos

## status and open questions

  * questions on geometry
      * ask about the overlapping cladding and core volumes (Marco or Marco)    
      * what is the difference between ru_l and fiber_env_l?
  * where are SiPMs? On one, or two sides of fibres? it seems on both sides
  * understand how to setup the sensitive detectors
    * why is there only one single SiPM sensitive detector? 
      as there's only one logical volume for the SiPM
    * how do I separate the reading from different SiPM's? GetCopyNumber
  * check what sensitive volumes are --> MUON_OpticalPhotonSD
  * multithreading removed, that was not the problem though    
  * find out what is saved, and remove what is unnecessary for now
    * add a condensed info:
      * the (x,y) coordinate of the MCtruth particles 
      * the signal at the SiPM, for each event


## on the detector info extraction

  * check how SiPmtHitsCollection is used in the code
     sipmt_SD_ = new MUON_OpticalPhotonSD (sipmt_SDname, "SiPmtHitsCollection", SDType::Pmt) ;
     --> becomes sipmt_hit_collection_id_ in the EventAction
     --> function EventAction::process_hit_collections
     --> hits are saved as objects of the MUON_OPHit class, where OP stays for OpticalPhoton
       


