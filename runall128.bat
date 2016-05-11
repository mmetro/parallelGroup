#!/bin/sh

export  BG_MAPCOMMONHEAP=1

sbatch --partition debug    --nodes 4   -n 4     -o out004n00128m0100000a4000000f.txt  --job-name 004n00128m0100000a4000000f  ./run.bat 0  00128 0100000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00128m0100000a4000000f.txt  --job-name 008n00128m0100000a4000000f  ./run.bat 0  00128 0100000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00128m0100000a4000000f.txt  --job-name 016n00128m0100000a4000000f  ./run.bat 0  00128 0100000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00128m0100000a4000000f.txt  --job-name 032n00128m0100000a4000000f  ./run.bat 0  00128 0100000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00128m0100000a4000000f.txt  --job-name 064n00128m0100000a4000000f  ./run.bat 0  00128 0100000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00128m0500000a4000000f.txt  --job-name 004n00128m0500000a4000000f  ./run.bat 0  00128 0500000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00128m0500000a4000000f.txt  --job-name 008n00128m0500000a4000000f  ./run.bat 0  00128 0500000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00128m0500000a4000000f.txt  --job-name 016n00128m0500000a4000000f  ./run.bat 0  00128 0500000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00128m0500000a4000000f.txt  --job-name 032n00128m0500000a4000000f  ./run.bat 0  00128 0500000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00128m0500000a4000000f.txt  --job-name 064n00128m0500000a4000000f  ./run.bat 0  00128 0500000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00128m1000000a4000000f.txt  --job-name 004n00128m1000000a4000000f  ./run.bat 0  00128 1000000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00128m1000000a4000000f.txt  --job-name 008n00128m1000000a4000000f  ./run.bat 0  00128 1000000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00128m1000000a4000000f.txt  --job-name 016n00128m1000000a4000000f  ./run.bat 0  00128 1000000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00128m1000000a4000000f.txt  --job-name 032n00128m1000000a4000000f  ./run.bat 0  00128 1000000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00128m1000000a4000000f.txt  --job-name 064n00128m1000000a4000000f  ./run.bat 0  00128 1000000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00128m0100000a8000000f.txt  --job-name 004n00128m0100000a8000000f  ./run.bat 0  00128 0100000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00128m0100000a8000000f.txt  --job-name 008n00128m0100000a8000000f  ./run.bat 0  00128 0100000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00128m0100000a8000000f.txt  --job-name 016n00128m0100000a8000000f  ./run.bat 0  00128 0100000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00128m0100000a8000000f.txt  --job-name 032n00128m0100000a8000000f  ./run.bat 0  00128 0100000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00128m0100000a8000000f.txt  --job-name 064n00128m0100000a8000000f  ./run.bat 0  00128 0100000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00128m0500000a8000000f.txt  --job-name 004n00128m0500000a8000000f  ./run.bat 0  00128 0500000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00128m0500000a8000000f.txt  --job-name 008n00128m0500000a8000000f  ./run.bat 0  00128 0500000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00128m0500000a8000000f.txt  --job-name 016n00128m0500000a8000000f  ./run.bat 0  00128 0500000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00128m0500000a8000000f.txt  --job-name 032n00128m0500000a8000000f  ./run.bat 0  00128 0500000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00128m0500000a8000000f.txt  --job-name 064n00128m0500000a8000000f  ./run.bat 0  00128 0500000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00128m1000000a8000000f.txt  --job-name 004n00128m1000000a8000000f  ./run.bat 0  00128 1000000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00128m1000000a8000000f.txt  --job-name 008n00128m1000000a8000000f  ./run.bat 0  00128 1000000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00128m1000000a8000000f.txt  --job-name 016n00128m1000000a8000000f  ./run.bat 0  00128 1000000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00128m1000000a8000000f.txt  --job-name 032n00128m1000000a8000000f  ./run.bat 0  00128 1000000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00128m1000000a8000000f.txt  --job-name 064n00128m1000000a8000000f  ./run.bat 0  00128 1000000 8000000 0

