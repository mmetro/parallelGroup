#!/bin/sh

sbatch --partition debug               --nodes 4   -n 4     -o output00t-004n2000m500a100000f.txt  --job-name 0b32n2000m500a100000f  ./run.bat 0  2000 500000 3000000
sbatch --partition debug               --nodes 4   -n 4     -o output16t-004n2000m500a100000f.txt  --job-name 8b32n2000m500a100000f  ./run.bat 16 2000 500000 3000000
sbatch --partition debug               --nodes 8   -n 8     -o output00t-008n2000m500a100000f.txt  --job-name 0b32n2000m500a100000f  ./run.bat 0  2000 500000 3000000
sbatch --partition debug               --nodes 8   -n 8     -o output16t-008n2000m500a100000f.txt  --job-name 8b32n2000m500a100000f  ./run.bat 16 2000 500000 3000000
sbatch --partition debug               --nodes 16  -n 16    -o output00t-016n2000m500a100000f.txt  --job-name 0b32n2000m500a100000f  ./run.bat 0  2000 500000 3000000
sbatch --partition debug               --nodes 16  -n 16    -o output16t-016n2000m500a100000f.txt  --job-name 8b32n2000m500a100000f  ./run.bat 16 2000 500000 3000000
sbatch --partition debug               --nodes 32  -n 32    -o output00t-032n2000m500a100000f.txt  --job-name 0b32n2000m500a100000f  ./run.bat 0  2000 500000 3000000
sbatch --partition debug               --nodes 32  -n 32    -o output16t-032n2000m500a100000f.txt  --job-name 8b32n2000m500a100000f  ./run.bat 16 2000 500000 3000000
sbatch --partition small               --nodes 64  -n 64    -o output00t-064n2000m500a100000f.txt  --job-name 0b64n2000m500a100000f  ./run.bat 0  2000 500000 3000000
sbatch --partition small               --nodes 64  -n 64    -o output16t-064n2000m500a100000f.txt  --job-name 8b64n2000m500a100000f  ./run.bat 16 2000 500000 3000000
sbatch --partition medium              --nodes 128 -n 128   -o output00t-128n2000m500a100000f.txt  --job-name 0b128n2000m500a100000f ./run.bat 0  2000 500000 3000000
sbatch --partition medium              --nodes 128 -n 128   -o output16t-128n2000m500a100000f.txt  --job-name 8b128n2000m500a100000f ./run.bat 16 2000 500000 3000000
