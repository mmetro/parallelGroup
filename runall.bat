#!/bin/sh

export  BG_MAPCOMMONHEAP=1

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0100000a4000000f.txt  --job-name 004n02048m0100000a4000000f  ./run.bat 0  02048 0100000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0100000a4000000f.txt  --job-name 008n02048m0100000a4000000f  ./run.bat 0  02048 0100000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0100000a4000000f.txt  --job-name 016n02048m0100000a4000000f  ./run.bat 0  02048 0100000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0100000a4000000f.txt  --job-name 032n02048m0100000a4000000f  ./run.bat 0  02048 0100000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0100000a4000000f.txt  --job-name 064n02048m0100000a4000000f  ./run.bat 0  02048 0100000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0100000a4000000f.txt  --job-name 004n04096m0100000a4000000f  ./run.bat 0  04096 0100000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0100000a4000000f.txt  --job-name 008n04096m0100000a4000000f  ./run.bat 0  04096 0100000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0100000a4000000f.txt  --job-name 016n04096m0100000a4000000f  ./run.bat 0  04096 0100000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0100000a4000000f.txt  --job-name 032n04096m0100000a4000000f  ./run.bat 0  04096 0100000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0100000a4000000f.txt  --job-name 064n04096m0100000a4000000f  ./run.bat 0  04096 0100000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0100000a4000000f.txt  --job-name 004n08192m0100000a4000000f  ./run.bat 0  08192 0100000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0100000a4000000f.txt  --job-name 008n08192m0100000a4000000f  ./run.bat 0  08192 0100000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0100000a4000000f.txt  --job-name 016n08192m0100000a4000000f  ./run.bat 0  08192 0100000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0100000a4000000f.txt  --job-name 032n08192m0100000a4000000f  ./run.bat 0  08192 0100000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0100000a4000000f.txt  --job-name 064n08192m0100000a4000000f  ./run.bat 0  08192 0100000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0500000a4000000f.txt  --job-name 004n02048m0500000a4000000f  ./run.bat 0  02048 0500000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0500000a4000000f.txt  --job-name 008n02048m0500000a4000000f  ./run.bat 0  02048 0500000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0500000a4000000f.txt  --job-name 016n02048m0500000a4000000f  ./run.bat 0  02048 0500000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0500000a4000000f.txt  --job-name 032n02048m0500000a4000000f  ./run.bat 0  02048 0500000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0500000a4000000f.txt  --job-name 064n02048m0500000a4000000f  ./run.bat 0  02048 0500000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0500000a4000000f.txt  --job-name 004n04096m0500000a4000000f  ./run.bat 0  04096 0500000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0500000a4000000f.txt  --job-name 008n04096m0500000a4000000f  ./run.bat 0  04096 0500000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0500000a4000000f.txt  --job-name 016n04096m0500000a4000000f  ./run.bat 0  04096 0500000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0500000a4000000f.txt  --job-name 032n04096m0500000a4000000f  ./run.bat 0  04096 0500000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0500000a4000000f.txt  --job-name 064n04096m0500000a4000000f  ./run.bat 0  04096 0500000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0500000a4000000f.txt  --job-name 004n08192m0500000a4000000f  ./run.bat 0  08192 0500000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0500000a4000000f.txt  --job-name 008n08192m0500000a4000000f  ./run.bat 0  08192 0500000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0500000a4000000f.txt  --job-name 016n08192m0500000a4000000f  ./run.bat 0  08192 0500000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0500000a4000000f.txt  --job-name 032n08192m0500000a4000000f  ./run.bat 0  08192 0500000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0500000a4000000f.txt  --job-name 064n08192m0500000a4000000f  ./run.bat 0  08192 0500000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m1000000a4000000f.txt  --job-name 004n02048m1000000a4000000f  ./run.bat 0  02048 1000000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m1000000a4000000f.txt  --job-name 008n02048m1000000a4000000f  ./run.bat 0  02048 1000000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m1000000a4000000f.txt  --job-name 016n02048m1000000a4000000f  ./run.bat 0  02048 1000000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m1000000a4000000f.txt  --job-name 032n02048m1000000a4000000f  ./run.bat 0  02048 1000000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m1000000a4000000f.txt  --job-name 064n02048m1000000a4000000f  ./run.bat 0  02048 1000000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m1000000a4000000f.txt  --job-name 004n04096m1000000a4000000f  ./run.bat 0  04096 1000000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m1000000a4000000f.txt  --job-name 008n04096m1000000a4000000f  ./run.bat 0  04096 1000000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m1000000a4000000f.txt  --job-name 016n04096m1000000a4000000f  ./run.bat 0  04096 1000000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m1000000a4000000f.txt  --job-name 032n04096m1000000a4000000f  ./run.bat 0  04096 1000000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m1000000a4000000f.txt  --job-name 064n04096m1000000a4000000f  ./run.bat 0  04096 1000000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m1000000a4000000f.txt  --job-name 004n08192m1000000a4000000f  ./run.bat 0  08192 1000000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m1000000a4000000f.txt  --job-name 008n08192m1000000a4000000f  ./run.bat 0  08192 1000000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m1000000a4000000f.txt  --job-name 016n08192m1000000a4000000f  ./run.bat 0  08192 1000000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m1000000a4000000f.txt  --job-name 032n08192m1000000a4000000f  ./run.bat 0  08192 1000000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m1000000a4000000f.txt  --job-name 064n08192m1000000a4000000f  ./run.bat 0  08192 1000000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0100000a8000000f.txt  --job-name 004n02048m0100000a8000000f  ./run.bat 0  02048 0100000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0100000a8000000f.txt  --job-name 008n02048m0100000a8000000f  ./run.bat 0  02048 0100000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0100000a8000000f.txt  --job-name 016n02048m0100000a8000000f  ./run.bat 0  02048 0100000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0100000a8000000f.txt  --job-name 032n02048m0100000a8000000f  ./run.bat 0  02048 0100000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0100000a8000000f.txt  --job-name 064n02048m0100000a8000000f  ./run.bat 0  02048 0100000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0100000a8000000f.txt  --job-name 004n04096m0100000a8000000f  ./run.bat 0  04096 0100000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0100000a8000000f.txt  --job-name 008n04096m0100000a8000000f  ./run.bat 0  04096 0100000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0100000a8000000f.txt  --job-name 016n04096m0100000a8000000f  ./run.bat 0  04096 0100000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0100000a8000000f.txt  --job-name 032n04096m0100000a8000000f  ./run.bat 0  04096 0100000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0100000a8000000f.txt  --job-name 064n04096m0100000a8000000f  ./run.bat 0  04096 0100000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0100000a8000000f.txt  --job-name 004n08192m0100000a8000000f  ./run.bat 0  08192 0100000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0100000a8000000f.txt  --job-name 008n08192m0100000a8000000f  ./run.bat 0  08192 0100000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0100000a8000000f.txt  --job-name 016n08192m0100000a8000000f  ./run.bat 0  08192 0100000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0100000a8000000f.txt  --job-name 032n08192m0100000a8000000f  ./run.bat 0  08192 0100000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0100000a8000000f.txt  --job-name 064n08192m0100000a8000000f  ./run.bat 0  08192 0100000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0500000a8000000f.txt  --job-name 004n02048m0500000a8000000f  ./run.bat 0  02048 0500000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0500000a8000000f.txt  --job-name 008n02048m0500000a8000000f  ./run.bat 0  02048 0500000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0500000a8000000f.txt  --job-name 016n02048m0500000a8000000f  ./run.bat 0  02048 0500000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0500000a8000000f.txt  --job-name 032n02048m0500000a8000000f  ./run.bat 0  02048 0500000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0500000a8000000f.txt  --job-name 064n02048m0500000a8000000f  ./run.bat 0  02048 0500000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0500000a8000000f.txt  --job-name 004n04096m0500000a8000000f  ./run.bat 0  04096 0500000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0500000a8000000f.txt  --job-name 008n04096m0500000a8000000f  ./run.bat 0  04096 0500000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0500000a8000000f.txt  --job-name 016n04096m0500000a8000000f  ./run.bat 0  04096 0500000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0500000a8000000f.txt  --job-name 032n04096m0500000a8000000f  ./run.bat 0  04096 0500000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0500000a8000000f.txt  --job-name 064n04096m0500000a8000000f  ./run.bat 0  04096 0500000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0500000a8000000f.txt  --job-name 004n08192m0500000a8000000f  ./run.bat 0  08192 0500000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0500000a8000000f.txt  --job-name 008n08192m0500000a8000000f  ./run.bat 0  08192 0500000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0500000a8000000f.txt  --job-name 016n08192m0500000a8000000f  ./run.bat 0  08192 0500000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0500000a8000000f.txt  --job-name 032n08192m0500000a8000000f  ./run.bat 0  08192 0500000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0500000a8000000f.txt  --job-name 064n08192m0500000a8000000f  ./run.bat 0  08192 0500000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m1000000a8000000f.txt  --job-name 004n02048m1000000a8000000f  ./run.bat 0  02048 1000000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m1000000a8000000f.txt  --job-name 008n02048m1000000a8000000f  ./run.bat 0  02048 1000000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m1000000a8000000f.txt  --job-name 016n02048m1000000a8000000f  ./run.bat 0  02048 1000000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m1000000a8000000f.txt  --job-name 032n02048m1000000a8000000f  ./run.bat 0  02048 1000000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m1000000a8000000f.txt  --job-name 064n02048m1000000a8000000f  ./run.bat 0  02048 1000000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m1000000a8000000f.txt  --job-name 004n04096m1000000a8000000f  ./run.bat 0  04096 1000000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m1000000a8000000f.txt  --job-name 008n04096m1000000a8000000f  ./run.bat 0  04096 1000000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m1000000a8000000f.txt  --job-name 016n04096m1000000a8000000f  ./run.bat 0  04096 1000000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m1000000a8000000f.txt  --job-name 032n04096m1000000a8000000f  ./run.bat 0  04096 1000000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m1000000a8000000f.txt  --job-name 064n04096m1000000a8000000f  ./run.bat 0  04096 1000000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m1000000a8000000f.txt  --job-name 004n08192m1000000a8000000f  ./run.bat 0  08192 1000000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m1000000a8000000f.txt  --job-name 008n08192m1000000a8000000f  ./run.bat 0  08192 1000000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m1000000a8000000f.txt  --job-name 016n08192m1000000a8000000f  ./run.bat 0  08192 1000000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m1000000a8000000f.txt  --job-name 032n08192m1000000a8000000f  ./run.bat 0  08192 1000000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m1000000a8000000f.txt  --job-name 064n08192m1000000a8000000f  ./run.bat 0  08192 1000000 8000000 0

