#ifndef MUON_LsMatProperties_h 
#define MUON_LsMatProperties_h 1

//#include "SystemOfUnits.h"
#include "G4SystemOfUnits.hh"
//#include "MUON_DetectorMessenger.h"
#include "globals.hh"
#include "G4UImessenger.hh"

class MUON_LsMatProperties 
{
    private:
        int     n_data_;
        double *energy_;
        double *ref_index_;
        double *fast_spectrum_;
        double *slow_spectrum_;
        double *scattering_;
        double *absorption_;

        double  light_yield_;
        double  resolution_scale_;
        double  fast_time_const_;
        double  slow_time_const_;
        double  yield_ratio_;
        double  birks_const_;

        double   density_;
        double   loading_fraction_;
        G4String loading_material_;

//        MUON_DetectorMessenger *messenger_;

    public:

        MUON_LsMatProperties();
        ~MUON_LsMatProperties();

        inline int get_n_data() { return n_data_; } ;

        void set_dummy_energy();
        inline double* get_energy() {return energy_;} ;

        void set_dummy_ref_index(double value);
        inline double* get_ref_index() {return ref_index_; } ; 

        void set_dummy_fast_spectrum( int non_empty_bin );
        inline double* get_fast_spectrum() {return fast_spectrum_; } ;

        void set_dummy_slow_spectrum( int non_empty_bin );
        inline double* get_slow_spectrum() {return slow_spectrum_; } ;

        void set_dummy_scattering( double value );
        inline double* get_scattering() {return scattering_; } ;

        void set_dummy_absorption( double value );
        inline double* get_absorption() {return absorption_; } ;

        inline double get_light_yield     (){ return light_yield_      ;} ;
        inline double get_resolution_scale(){ return resolution_scale_ ;} ;
        inline double get_fast_time_const (){ return fast_time_const_  ;} ;
        inline double get_slow_time_const (){ return slow_time_const_  ;} ;
        inline double get_yield_ratio     (){ return yield_ratio_      ;} ;
        inline double get_birks_const     (){ return birks_const_      ;} ;

        inline double    get_density         (){ return density_; } ;
        inline double    get_loading_fraction(){ return loading_fraction_; } ;
        inline G4String& get_loading_material(){ return loading_material_; } ;

        inline void set_light_yield     (double value){ light_yield_      = value; } ;
        inline void set_resolution_scale(double value){ resolution_scale_ = value; } ;
        inline void set_fast_time_const (double value){ fast_time_const_  = value; } ;
        inline void set_slow_time_const (double value){ slow_time_const_  = value; } ;
        inline void set_yield_ratio     (double value){ yield_ratio_      = value; } ;
        inline void set_birks_const     (double value){ birks_const_      = value; } ;

        inline void set_density          ( double          value ){ density_ = value; } ;
        inline void set_loading_fraction ( double          value ){ loading_fraction_ = value; } ;
        inline void set_loading_material ( const G4String& value ){ loading_material_ = value; } ;
};

#endif
