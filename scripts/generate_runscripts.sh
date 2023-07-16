#!/bin/bash

nruns=2
proccnt=0
WORKDIR=`pwd`
for((i=1; i<=nruns; i++))
do 
  outfile="run${i}.sh"
  filename="ebrem${i}.root"
  echo -e "#!/bin/bash\n" > $outfile
  echo -e "# This file is automatically generated by generate_runscripts.sh" >> $outfile
  echo -e "WORKDIR=${WORKDIR}\n" >> $outfile
  echo -e '$WORKDIR'"/../dmg4brem" '$WORKDIR'"/cardg.g4mac $filename" >> $outfile
  proccnt=$((proccnt+1))
done 

echo "$proccnt macro files are generated."