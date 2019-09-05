
if [ "$#" -gt 1 ]
  then 
    source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_96 x86_64-centos7-gcc8-dbg
    echo "setup env with debug options"
    echo "remember to compile with debug options:"
    echo "cmake  -DCMAKE_BUILD_TYPE=Debug .."
  else
    source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_96 x86_64-centos7-gcc8-opt
    echo "setup env with optimised options"
fi

# source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_96 x86_64-centos7-gcc8-dbg
# source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_96 x86_64-centos7-gcc8-opt

# source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_95 x86_64-centos7-gcc8-dbg
# source /cvmfs/sft.cern.ch/lcg/views/setupViews.sh LCG_95 x86_64-centos7-gcc8-opt
