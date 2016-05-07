#!/bin/sh
#SBATCH --mail-type=ALL
#SBATCH --mail-user=jacobn2@rpi.edu
#SBATCH -t 60
export  BG_MAPCOMMONHEAP=1
srun antfarm $1 $2 $3 $4
# additional calls to srun may be placed in the script, they will all use the same partition 
