# a simple simulation of a muon detector made of LiquidO

## useful pieces of information

  * to source a complete LCG environment:
     ```source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_96 x86_64-centos7-gcc8-opt```
  * original simulation code of LiquidO is [here](https://svn.in2p3.fr/liquido/SW/trunk/simulation/)
  * to have the mac working, always use ```ssh -Y``` and, on your mac, 
    enable “indirect GL” in XQuartz, with the command
    ```defaults write org.macosforge.xquartz.X11 enable_iglx -bool true```
  * documentation on Geometry in Geant4 at this [link](https://indico.cern.ch/event/776050/contributions/3240647/attachments/1788893/2913535/Geometry2.pdf)
  * Geant4 [example](https://github.com/Geant4/geant4/tree/master/examples/basic/B2)
  * [Getting started with Geant4](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/BackupVersions/V10.4/html/GettingStarted/gettingStarted.html)

## the code

  * ```OFOS_DetectorConstruction```: where the geometry of the detector is defined
    * remember: for each object one needs:
      * a solid = shape and size
      * a logical volume = solid + properties  (material, whether it's sensitive...)
      * a physical volume = location of the object
      * NB the physical volume is placed inside a logical volume somehow, 
           so it seems it's enough to replicate the most external one of them all
  * ```OFOS_DetectorMessenger```: pass the infoes from the mac file to the Detector Construction
  * ```OFOS_OpticalPhotonSD```: behaviour of the sensitive detectors in the geome
    * the single elements are identified by the copy number, I think
  * the simulation process is composed of a run of events, 
    which are processed in the following hierarchically organised actions:
    * ```OFOS_RunAction```: Begin and end of the [run](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/BackupVersions/V10.4/html/Fundamentals/run.html)
    * ```OFOS_EventAction```: Begin and end of an [event](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/BackupVersions/V10.4/html/Fundamentals/event.html)
    * ```OFOS_TrackingAction```: Begin and end of [tracking](http://geant4-userdoc.web.cern.ch/geant4-userdoc/UsersGuides/ForApplicationDeveloper/BackupVersions/V10.4/html/TrackingAndPhysics/tracking.html#access-to-track-and-step-information)
    * ```OFOS_SteppingAction```: At each step in which the tracking process is divided

## the geometry

  * A single slab of scintillating LiquidO, with square section in the (x,y) plane, 
    is filled in the middle with an array of fibres,
    parallel to the base surface.
  * each readout unit (ru_l) is composed of a fibre and a SiPM, and contains an overall envelope (fiber_env_l)
  * each fibre is composed of an outer cladding (fiber_outer_cladding_l), inner cladding (fiber_inner_cladding_l), and core (fiber_core_l)
  * each SiPM is composed of an overall volume (ru_sipmt_l) and a sensitive volume (ru_sipmt_sens_l)
  * there's a SiPM on each side of each fibre (ru_sipmt_top_p, ru_sipmt_bot_p)