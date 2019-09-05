
#include "MUON_OutputLog.h" 

std::ofstream* MUON_OutputLog::logfile = 0;
std::stringstream MUON_OutputLog::log_cache;
std::stringstream MUON_OutputLog::ls_cache;