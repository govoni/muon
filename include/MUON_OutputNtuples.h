#ifndef MUON_OutputNtuples_h
#define MUON_OutputNtuples_h 1

#include <vector>
#include <string>

#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"
#include "TTree.h"
#include "MUON_OPHit.h"

class MUON_OutputNtuples
{
    public:
        MUON_OutputNtuples(const char *h_name, const char *h_title, int n_max_hits, const char *i_name, const char *i_title, int n_max_interactions); 
        ~MUON_OutputNtuples();
        void set_branches();
        void write() ;
        bool fill_all_branches( double value);
        void store_event() ;

        bool fill_hit( const MUON_OPHit *a_hit );
        bool fill_interaction( int track_id, int parent_id, int interaction_id, int particle,  const G4ThreeVector &pos, float dE, float time ); 

        inline void add_edep( double e )    { tot_en_dep_   += (e/MeV); };
        inline void add_ls_ion (double e)   { ion_en_in_ls_       += (e/MeV); };
        inline void add_prim_ion( double e ){ primary_ionization_ += (e/MeV); }; 
//      inline void add_prim_ion_incl_deltas( double e ){ primary_ionization_incl_deltas_ += (e/MeV); }; 
        inline void add_en_loss_via_deltas(double e)    { en_loss_via_deltas_  += (e/MeV); };
        inline void add_radiative_en_loss (double e)    { radiative_en_loss_   += (e/MeV); };

        inline void add_scint_phot( int n ) { n_scint_phot_ += n ; } ;
        inline void add_chere_phot( int n ) { n_chere_phot_ += n ; } ;
        inline void add_lost_phot ( int n ) { n_lost_phot_ += n ; } ;

        std::string getName () { return name_ ; } ;

        TTree hit_tree_ ;
        TTree truth_tree_ ;
//        TTree * hit_tree_ ;
//        TTree * truth_tree_ ;


    private:

        std::string name_ ;

        float       tot_en_dep_;
        float       ion_en_in_ls_;
        float       primary_ionization_;
//      float       primary_ionization_incl_deltas_;
        float       en_loss_via_deltas_;
        float       radiative_en_loss_;
        int         n_scint_phot_;
        int         n_chere_phot_;
        int         n_lost_phot_;
        int         n_max_interactions_;
        int         n_interactions_;
        bool        is_ni_overflow_;
        float       collection_eff_;
        int         n_max_hits_;
        bool        is_nh_overflow_;
        int         n_hits_;

        std::vector<int>     t_id_ ;
        std::vector<int>     p_id_ ;
        std::vector<int>     i_id_ ;
        std::vector<int>     i_particle_ ;
        std::vector<float>   i_pos_x_ ;
        std::vector<float>   i_pos_y_ ;
        std::vector<float>   i_pos_z_ ;
        std::vector<float>   i_dE_ ;
        std::vector<float>   i_time_ ;
        std::vector<int>     h_track_id_ ;
        std::vector<int>     h_sd_type_ ;
        std::vector<int>     h_primary_id_ ;
        std::vector<int>     h_secondary_id_ ;
        std::vector<int>     h_gen_proc_ ;
        std::vector<double>  h_time_ ;
        std::vector<float>   h_wavelength_ ;
        std::vector<float>   h_pos_x_ ;
        std::vector<float>   h_pos_y_ ;
        std::vector<float>   h_pos_z_ ;
        std::vector<float>   h_pol_x_ ;
        std::vector<float>   h_pol_y_ ;
        std::vector<float>   h_pol_z_ ;
};


#endif
