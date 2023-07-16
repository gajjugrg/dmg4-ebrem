#!/bin/bash
# Author: Gajendra Gurung

# Print out the current time
echo "[submit.sh] Start time: `date -u`"
STARTTIME=`date +%s`
STARTTIME_STRING=`date`

USRNAME=`whoami`
WORKDIR=`pwd`
OUTDIR=$WORKDIR/output
LOGDIR=$WORKDIR/log
ERRDIR=$WORKDIR/err


# Test if the LOGDIR and ERRDIR exists
if [ ! -e $LOGDIR ] ; then
  mkdir $LOGDIR
fi
if [ ! -e $ERRDIR ] ; then
  mkdir $ERRDIR
fi

nruns=2
EXITSTATUS=0


for (( i = 1; i <= nruns; i++))
do 
  /usr/bin/qsub \
        -q normal \
        -o log/qsub_run${i}_log.stdout \
        -e err/qsub_run${i}_err.stderr \
        -l walltime=24:00:00,cput=24:00:00 \
        -N "run${i}" \
        run${i}.sh
done


echo "[submit.sh] End time: `date -u`"
ENDTIME=`date +%s`
ENDTIME_STRING=`date`

ELAPSED_TIME=`echo "$ENDTIME - $STARTTIME" | bc`
HTIME=`echo "$ELAPSED_TIME/3600" | bc`
MTIME=`echo "($ELAPSED_TIME/60) - ($HTIME*60)" | bc`
STIME=`echo "$ELAPSED_TIME - (($ELAPSED_TIME/60)*60)" | bc`

echo "$JOBCNT jobs are submitted successfully!"
echo "------------------------------------------------------------------------"
echo " Start time: $STARTTIME_STRING"
echo " End time: $ENDTIME_STRING"
echo " Elapsed time: ${HTIME}h${MTIME}m${STIME}s"
echo "------------------------------------------------------------------------"

