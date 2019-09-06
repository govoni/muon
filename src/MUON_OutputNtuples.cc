#include "MUON_OutputNtuples.h"

using namespace std ;

MUON_OutputNtuples::MUON_OutputNtuples (const char *h_name, const char *h_title, int n_max_hits, const char *i_name, const char *i_title, int n_max_interactions) : 
    name_               ("ntuple"),
    n_max_hits_         (n_max_hits),
    n_hits_             (0),
    is_nh_overflow_     (false),
    collection_eff_     (0.0),
    n_max_interactions_ (n_max_interactions),
    n_interactions_     (0),
    is_ni_overflow_     (false),
    tot_en_dep_         (0.),
    ion_en_in_ls_       (0.),
    primary_ionization_ (0.),
//  primary_ionization_incl_deltas_ (0),
    en_loss_via_deltas_ (0.),
    radiative_en_loss_  (0.),
    n_scint_phot_       (0),
    n_chere_phot_       (0),
    n_lost_phot_        (0),
    h_track_id_         (n_max_hits_, 0),                                  
    h_sd_type_          (n_max_hits_, 0),                                  
    h_primary_id_       (n_max_hits_, 0),                                  
    h_secondary_id_     (n_max_hits_, 0),                                  
    h_gen_proc_         (n_max_hits_, 0),                                  
    h_time_             (n_max_hits_, 0.),                                  
    h_wavelength_       (n_max_hits_, 0.),                                  
    h_pos_x_            (n_max_hits_, 0.),                                  
    h_pos_y_            (n_max_hits_, 0.),                                  
    h_pos_z_            (n_max_hits_, 0.),                                  
    h_pol_x_            (n_max_hits_, 0.),                                  
    h_pol_y_            (n_max_hits_, 0.),                                  
    h_pol_z_            (n_max_hits_, 0.),                                  
    t_id_               (n_max_interactions_, 0),                                         
    p_id_               (n_max_interactions_, 0),                                         
    i_id_               (n_max_interactions_, 0),                                         
    i_pos_x_            (n_max_interactions_, 0.),                                          
    i_pos_y_            (n_max_interactions_, 0.),                                         
    i_pos_z_            (n_max_interactions_, 0.),                                         
    i_particle_         (n_max_interactions_, 0),                                         
    i_dE_               (n_max_interactions_, 0.),                                         
    i_time_             (n_max_interactions_, 0.),                                   
    hit_tree_           (h_name, h_title),
    truth_tree_         (i_name, i_title)

{
//    hit_tree_   = new TTree (h_name, h_title) ;
//    truth_tree_ = new TTree (i_name, i_title) ;

    cout << "DEBUG[MUON_OutputNtuples][ctor]" << endl ;
    set_branches () ;

    cout << "DEBUG [MUON_OutputNtuples][ctor] done" << endl ;
    G4cout << "MUON_OutputNtuples created" << G4endl ;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void
MUON_OutputNtuples::write ()
{
    hit_tree_.Write () ;
    truth_tree_.Write () ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void
MUON_OutputNtuples::set_branches ()
{

  hit_tree_.Branch ("n_hits"         , &n_hits_         , "n_hits/I") ;
  hit_tree_.Branch ("is_nh_overflow" , &is_nh_overflow_ , "is_nh_overflow/O") ;

  hit_tree_.Branch ("h_track_id_"    , &h_track_id_    ) ;
  hit_tree_.Branch ("h_sd_type_"     , &h_sd_type_     ) ;
  hit_tree_.Branch ("h_primary_id_"  , &h_primary_id_  ) ;
  hit_tree_.Branch ("h_secondary_id_", &h_secondary_id_) ;
  hit_tree_.Branch ("h_gen_proc_"    , &h_gen_proc_    ) ;
  hit_tree_.Branch ("h_time_"        , &h_time_        ) ;
  hit_tree_.Branch ("h_wavelength_"  , &h_wavelength_  ) ;
  hit_tree_.Branch ("h_pos_x_"       , &h_pos_x_       ) ;
  hit_tree_.Branch ("h_pos_y_"       , &h_pos_y_       ) ;
  hit_tree_.Branch ("h_pos_z_"       , &h_pos_z_       ) ;
  hit_tree_.Branch ("h_pol_x_"       , &h_pol_x_       ) ;
  hit_tree_.Branch ("h_pol_y_"       , &h_pol_y_       ) ;
  hit_tree_.Branch ("h_pol_z_"       , &h_pol_z_       ) ;

  truth_tree_.Branch ("n_interactions"    , &n_interactions_     , "n_interactions/I") ;
  truth_tree_.Branch ("is_ni_overflow"    , &is_ni_overflow_     , "is_ni_overflow/O") ;
  truth_tree_.Branch ("tot_en_dep"        , &tot_en_dep_         , "tot_en_dep_/F"      ) ;
  truth_tree_.Branch ("primary_ionization", &primary_ionization_ , "primary_ionization_/F"      ) ;
  truth_tree_.Branch ("en_loss_via_deltas", &en_loss_via_deltas_ , "en_loss_via_deltas_/F") ;
  truth_tree_.Branch ("radiative_en_loss",  &radiative_en_loss_  , "radiative_en_loss_/F") ;
//  truth_tree_.Branch ("ion_en_in_ls"      , &ion_en_in_ls_       , "ion_en_in_ls_/F"      ) ;
  truth_tree_.Branch ("n_scint_phot"      , &n_scint_phot_       , "n_scint_phot/I") ;
  truth_tree_.Branch ("n_chere_phot"      , &n_chere_phot_       , "n_chere_phot/I") ;

  truth_tree_.Branch ("t_id_"          , &t_id_          ) ;
  truth_tree_.Branch ("p_id_"          , &p_id_          ) ;
  truth_tree_.Branch ("i_id_"          , &i_id_          ) ;
  truth_tree_.Branch ("i_pos_x_"       , &i_pos_x_       ) ;
  truth_tree_.Branch ("i_pos_y_"       , &i_pos_y_       ) ;
  truth_tree_.Branch ("i_pos_z_"       , &i_pos_z_       ) ;
  truth_tree_.Branch ("i_particle_"    , &i_particle_    ) ;
  truth_tree_.Branch ("i_dE_"          , &i_dE_          ) ;
  truth_tree_.Branch ("i_time_"        , &i_time_        ) ;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


MUON_OutputNtuples::~MUON_OutputNtuples ()
{
  G4cout << "Deleting MUON_OutputNtuples" << G4endl ;

//  delete hit_tree_ ;
//  delete truth_tree_ ;

  G4cout << "Deleting MUON_OutputNtuples >>> DONE" << G4endl ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


bool 
MUON_OutputNtuples::fill_interaction (int track_id, int parent_id, int interaction_id, int particle,  const G4ThreeVector &pos, float dE, float time ) 
{
    if (n_interactions_ == n_max_interactions_)
      {
        is_ni_overflow_ = true ;
        return false ;
      }

    t_id_.at (n_interactions_)       = track_id ;
    i_id_.at (n_interactions_)       = interaction_id ;
    p_id_.at (n_interactions_)       = parent_id ;
    i_particle_.at (n_interactions_) = particle ;
    i_pos_x_.at (n_interactions_)    = pos.x () / mm ;
    i_pos_y_.at (n_interactions_)    = pos.y () / mm ;
    i_pos_z_.at (n_interactions_)    = pos.z () / mm ;
    i_dE_.at (n_interactions_)       = dE / MeV ;
    i_time_.at (n_interactions_)     = time / ns ; 

    ++n_interactions_ ;

    return true ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


bool 
MUON_OutputNtuples::fill_hit (const MUON_OPHit *a_hit )
{
    if (n_hits_ == n_max_hits_)
    {
        is_nh_overflow_ = true ;
        return false ;
    }

    h_track_id_.at (n_hits_)     = static_cast<int>   (a_hit->get_track_id ()) ;
    h_sd_type_.at (n_hits_)      = static_cast<int>   (a_hit->get_sd_type ()) ;
    h_primary_id_.at (n_hits_)   = static_cast<int>   (a_hit->get_primary_id ()) ;
    h_secondary_id_.at (n_hits_) = static_cast<int>   (a_hit->get_secondary_id ()) ;
    h_gen_proc_.at (n_hits_)     = static_cast<int>   (a_hit->get_gen_proc ()) ;
    h_time_.at (n_hits_)         = static_cast<double>(a_hit->get_time () / ns) ;
    h_wavelength_.at (n_hits_)   = static_cast<float> (a_hit->get_wavelength ()) ;
    h_pos_x_.at (n_hits_)        = static_cast<float> (a_hit->get_position ().x () / mm) ;
    h_pos_y_.at (n_hits_)        = static_cast<float> (a_hit->get_position ().y () / mm) ;
    h_pos_z_.at (n_hits_)        = static_cast<float> (a_hit->get_position ().z () / mm) ;
    h_pol_x_.at (n_hits_)        = static_cast<float> (a_hit->get_polarization ().x ()) ;
    h_pol_y_.at (n_hits_)        = static_cast<float> (a_hit->get_polarization ().y ()) ;
    h_pol_z_.at (n_hits_)        = static_cast<float> (a_hit->get_polarization ().z ()) ;

    ++n_hits_ ;

    return true ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


bool 
MUON_OutputNtuples::fill_all_branches (double value )
{
    if (n_hits_ == n_max_hits_)
    {
        is_nh_overflow_ = true ;
        return false ;
    }

    h_track_id_.at (n_hits_)     = static_cast<int>   (value) ;
    h_sd_type_.at (n_hits_)      = static_cast<int>   (value) ;
    h_primary_id_.at (n_hits_)   = static_cast<int>   (value) ;
    h_secondary_id_.at (n_hits_) = static_cast<int>   (value) ;
    h_gen_proc_.at (n_hits_)     = static_cast<int>   (value) ;
    h_time_.at (n_hits_)         = static_cast<double>(value) ;
    h_wavelength_.at (n_hits_)   = static_cast<float> (value) ;
    h_pos_x_.at (n_hits_)        = static_cast<float> (value) ;
    h_pos_y_.at (n_hits_)        = static_cast<float> (value) ;
    h_pos_z_.at (n_hits_)        = static_cast<float> (value) ;
    h_pol_x_.at (n_hits_)        = static_cast<float> (value) ;
    h_pol_y_.at (n_hits_)        = static_cast<float> (value) ;
    h_pol_z_.at (n_hits_)        = static_cast<float> (value) ;

    ++n_hits_ ;

    return true ;
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void
MUON_OutputNtuples::store_event ()
{
    cout << "DEBUG [MUON_OutputNtuples][store_event] start" << endl ;

    hit_tree_.Fill () ;
//  G4cout << "store_event () :: n_scin: " << n_scint_phot_ << "   n_chere: " << n_chere_phot_ 
//         << "  n_lost: " << n_lost_phot_ << G4endl ;
    n_hits_ = 0 ;
    n_lost_phot_ = 0 ;

    truth_tree_.Fill () ;
//  G4cout << "tot_edep = " << tot_en_dep_ << G4endl
//         << "prim_ion = " << primary_ionization_ << G4endl
//         << "p_ion_wd = " << primary_ionization_incl_deltas_ << G4endl 
//         << "ion_ls   = " << ion_en_in_ls_ << G4endl
//         << "delta_en = " << en_loss_via_deltas_ << G4endl
//         << "rad_en   = " << radiative_en_loss_ << G4endl
//         << "sum (idr) = " << primary_ionization_ + en_loss_via_deltas_ + radiative_en_loss_ << G4endl ;

    n_interactions_ = 0 ;
    n_scint_phot_ = 0 ;
    n_chere_phot_ = 0 ;
    tot_en_dep_ = 0 ;
    ion_en_in_ls_ = 0 ;
    primary_ionization_ = 0 ;
//  primary_ionization_incl_deltas_ = 0 ;
    en_loss_via_deltas_ = 0 ;
    radiative_en_loss_ = 0 ;

    cout << "DEBUG [MUON_OutputNtuples][store_event] done" << endl ;

}
