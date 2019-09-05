#include "MUON_LsMatProperties.h" 
//#include "MUON_DetectorMessenger.h"
#include "MUON_Verbosity.h"


MUON_LsMatProperties::MUON_LsMatProperties() : n_data_(10),
                                               energy_(0),
                                               ref_index_(0),
                                               fast_spectrum_(0),
                                               slow_spectrum_(0),
                                               scattering_(0),
                                               absorption_(0),

                                               light_yield_(0.),
                                               resolution_scale_(0.),
                                               fast_time_const_(0.),
                                               slow_time_const_(0.),
                                               yield_ratio_(0.),
                                               birks_const_(1e-9),

                                               density_(0.),
                                               loading_fraction_(0.),
                                               loading_material_("")
{
    if(MUON_Verbosity::level>1)
        G4cout << "MUON_LsMatProperties :: constructor " << G4endl;


    energy_        = new double[n_data_];
    ref_index_     = new double[n_data_];
    fast_spectrum_ = new double[n_data_];
    slow_spectrum_ = new double[n_data_];
    scattering_    = new double[n_data_];
    absorption_    = new double[n_data_];

 // messenger_     = new MUON_DetectorMessenger(this);


    set_dummy_energy();
    set_dummy_ref_index    ( 1.50 );
    set_dummy_fast_spectrum( 2 );
    set_dummy_slow_spectrum( 2 );
    set_dummy_scattering   ( 1.0 * m );
    set_dummy_absorption   ( 1.0 * m );
    set_light_yield        ( 10000.0 / MeV );
    set_resolution_scale   ( 1.0 );
    set_fast_time_const    ( 5.0 * ns);
    set_slow_time_const    ( 20.0 * ns );
    set_yield_ratio        ( 0.8 );

}



MUON_LsMatProperties::~MUON_LsMatProperties()
{
    if(MUON_Verbosity::level>1)
        G4cout << "MUON_LsMatProperties :: destructor " << G4endl;

    if(energy_)                  delete energy_;
    if(ref_index_)               delete ref_index_;
    if(fast_spectrum_)           delete fast_spectrum_;
    if(slow_spectrum_)           delete slow_spectrum_;
    if(scattering_)              delete scattering_;
    if(absorption_)              delete absorption_;

    energy_        = 0;
    ref_index_     = 0;
    fast_spectrum_ = 0;
    slow_spectrum_ = 0;
    scattering_    = 0;
    absorption_    = 0;
}


void 
MUON_LsMatProperties::set_dummy_energy()
{
    for(int i=0; i<n_data_; ++i)
    {
        energy_[i] = (i+1.0) * eV;
    }
}



void 
MUON_LsMatProperties::set_dummy_ref_index( double value )
{
    for(int i=0; i<n_data_; ++i)
    {
        ref_index_[i] = value;
    }
}



void 
MUON_LsMatProperties::set_dummy_fast_spectrum( int non_empty_bin )
{
    for(int i=0; i<n_data_; ++i)
    {
        fast_spectrum_[i] = (i==non_empty_bin ? 1.0 : 0.0 );
    }
}



void 
MUON_LsMatProperties::set_dummy_slow_spectrum( int non_empty_bin )
{
    for(int i=0; i<n_data_; ++i)
    {
        slow_spectrum_[i] = (i==non_empty_bin ? 1.0 : 0.0 );
    }
}



void 
MUON_LsMatProperties::set_dummy_scattering( double value )
{
    if(MUON_Verbosity::level>1)
        G4cout << "MUON_LsMatProperties::set_dummy_scattering( " << value << " )" << G4endl;

    for(int i=0; i<n_data_; ++i)
    {
        scattering_[i] = value;
    }
}



void 
MUON_LsMatProperties::set_dummy_absorption( double value )
{
    if(MUON_Verbosity::level>1)
        G4cout << "MUON_LsMatProperties::set_dummy_absorption( " << value << " )" << G4endl;
    for(int i=0; i<n_data_; ++i)
    {
        absorption_[i] = value;
    }
}

