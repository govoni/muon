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
  * what is ComputeCriticalEnergy?    
  * how do I get a more useful visualisation?
    * quicker
    * with less particles (energy threshold?)


## on the detector info extraction

  * on the detector types
    * Undefined=0, VerticalFiber=1, HorizontalFiber=2, Fiber=3, Pmt=4, Vessel=8
    * this is chosen in MUON_DetectorConstruction::ConstructSDandField (),
      for the time being classified as fibres, simply.
    * Q: do we want to have two separate detectors, to account for possible interactions
         of other particles than optical photons, in the SiPM or in the fibres? 
  * check how SiPmtHitsCollection is used in the code
     sipmt_SD_ = new MUON_OpticalPhotonSD (sipmt_SDname, "SiPmtHitsCollection", SDType::Pmt) ;
     --> becomes sipmt_hit_collection_id_ in the EventAction
     --> function EventAction::process_hit_collections
     --> hits are saved as objects of the MUON_OPHit class, where OP stays for OpticalPhoton
  * on the expected interaction, i.e. what should be saved
    * in the fibres
      * it's just propagation, so this is basically a counting of photons
    * in the SiPM
      * here it's energy deposition with a given efficiency
        * that I may parameterise afterwards?
        * is it considered already?
  * define in the stepping action what needs to be saved
    * ID the info to be saved from the SiPM
      * for optical photons
      * for other particles  
  * come determino il numero di identificazione di ogni SiPM?
    * stranamente con una particle gun centrale, si accendono SiPM 0 ed 1, mi sarei aspettato circa a meta'
      * sto usando thePrePV->GetCopyNo () per determinare il numero di ID del SiPM
      * seems that the solution is in the Touchables, investigating...
        * bisogna andare in profondita' con GetReplicaNumber, sembra 0 (SiPM) e 2 (fibre).
        * controllare se funziona anche con GetCopyNb
  * l'efficienza di rivelazione del SiPM e' codificata?
  * change SiPMT into SiPM

## primary particle shooting and information

  * how do I generate a flat distribution in x, y of the particles to the detector?
  * how can I determine the total energy loss of a particle, after it exits the detector?
    * the total energy loss that I see in the ntuples created by the executable
      seems somehow independent of the size of the detector I put in the shoot_positron.mac file,
      which is strange... but if the secondaries energy is not accounted for?
      * still not clear to me how to know the energy a single particle lost in a step.
        * there is the energy lost to the material (GetTotalEnergyDeposit)
        * there is the energy lost to secondary particles (how do I calculate it?)
        * http://hypernews.slac.stanford.edu/HyperNews/geant4/get/eventtrackmanage/1043/1/1.html
        * http://hypernews.slac.stanford.edu/HyperNews/geant4/get/eventtrackmanage/1287/1.html
      * ask the particle what's its energy and saving the last value, 
        to then evaluate the lost energy as a difference

