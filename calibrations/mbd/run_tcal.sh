#!/usr/bin/env bash
#
# run_tcal.sh <fname>
# do tq timing offset calibrations
# <fname> is the name of the root file with the waveforms

# Check that user specifies a file
if [[ $# -lt 1 ]]
then
  echo "Usage: run_tcal.sh <rootfname>"
  exit -1
fi

mbd_uncalrootf=$1    # DST_UNCALMBD ROOT filename

nevt=0
if [[ $# -gt 1 ]]
then
  nevt=$2
  echo Processing $2 events
fi

echo PATH=$PATH
echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH

# process up to 100K uncalibrated events for t0 calibration
#tcalib_events=100000
tcalib_events=0
if [[ $nevt -ne 0 ]] && [[ $nevt -le 100000 ]]
then
  tcalib_events=${nevt}
fi

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

pass=0
runtype=0 # 0=auau,1=pp
#dbtag="ProdA_2024"
dbtag=""
if [[ -n "$PASS0DIR" ]]
then
  dbtag=""
fi

echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${tcalib_events},${runtype},\"${dbtag}\"\)
root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${tcalib_events},${runtype},\"${dbtag}\"\)

