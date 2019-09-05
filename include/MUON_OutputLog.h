#ifndef MUON_OutputLog_h
#define MUON_OutputLog_h

#include <fstream>
#include <iostream>
#include <sstream>

class MUON_OutputLog {
    public:
        static std::ofstream *logfile;
        static std::stringstream log_cache;
        static std::stringstream ls_cache;

};

#endif