#!/bin/sh

export  BG_MAPCOMMONHEAP=1

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0100000a1000000f.txt  --job-name 0b004n02048m0100000a1000000f  ./run.bat 0  02048 0100000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0100000a1000000f.txt  --job-name 0b008n02048m0100000a1000000f  ./run.bat 0  02048 0100000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0100000a1000000f.txt  --job-name 0b016n02048m0100000a1000000f  ./run.bat 0  02048 0100000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0100000a1000000f.txt  --job-name 0b032n02048m0100000a1000000f  ./run.bat 0  02048 0100000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0100000a1000000f.txt  --job-name 0b064n02048m0100000a1000000f  ./run.bat 0  02048 0100000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0100000a1000000f.txt  --job-name 0b004n04096m0100000a1000000f  ./run.bat 0  04096 0100000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0100000a1000000f.txt  --job-name 0b008n04096m0100000a1000000f  ./run.bat 0  04096 0100000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0100000a1000000f.txt  --job-name 0b016n04096m0100000a1000000f  ./run.bat 0  04096 0100000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0100000a1000000f.txt  --job-name 0b032n04096m0100000a1000000f  ./run.bat 0  04096 0100000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0100000a1000000f.txt  --job-name 0b064n04096m0100000a1000000f  ./run.bat 0  04096 0100000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0100000a1000000f.txt  --job-name 0b004n08192m0100000a1000000f  ./run.bat 0  08192 0100000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0100000a1000000f.txt  --job-name 0b008n08192m0100000a1000000f  ./run.bat 0  08192 0100000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0100000a1000000f.txt  --job-name 0b016n08192m0100000a1000000f  ./run.bat 0  08192 0100000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0100000a1000000f.txt  --job-name 0b032n08192m0100000a1000000f  ./run.bat 0  08192 0100000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0100000a1000000f.txt  --job-name 0b064n08192m0100000a1000000f  ./run.bat 0  08192 0100000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0500000a1000000f.txt  --job-name 0b004n02048m0500000a1000000f  ./run.bat 0  02048 0500000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0500000a1000000f.txt  --job-name 0b008n02048m0500000a1000000f  ./run.bat 0  02048 0500000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0500000a1000000f.txt  --job-name 0b016n02048m0500000a1000000f  ./run.bat 0  02048 0500000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0500000a1000000f.txt  --job-name 0b032n02048m0500000a1000000f  ./run.bat 0  02048 0500000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0500000a1000000f.txt  --job-name 0b064n02048m0500000a1000000f  ./run.bat 0  02048 0500000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0500000a1000000f.txt  --job-name 0b004n04096m0500000a1000000f  ./run.bat 0  04096 0500000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0500000a1000000f.txt  --job-name 0b008n04096m0500000a1000000f  ./run.bat 0  04096 0500000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0500000a1000000f.txt  --job-name 0b016n04096m0500000a1000000f  ./run.bat 0  04096 0500000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0500000a1000000f.txt  --job-name 0b032n04096m0500000a1000000f  ./run.bat 0  04096 0500000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0500000a1000000f.txt  --job-name 0b064n04096m0500000a1000000f  ./run.bat 0  04096 0500000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0500000a1000000f.txt  --job-name 0b004n08192m0500000a1000000f  ./run.bat 0  08192 0500000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0500000a1000000f.txt  --job-name 0b008n08192m0500000a1000000f  ./run.bat 0  08192 0500000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0500000a1000000f.txt  --job-name 0b016n08192m0500000a1000000f  ./run.bat 0  08192 0500000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0500000a1000000f.txt  --job-name 0b032n08192m0500000a1000000f  ./run.bat 0  08192 0500000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0500000a1000000f.txt  --job-name 0b064n08192m0500000a1000000f  ./run.bat 0  08192 0500000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m1000000a1000000f.txt  --job-name 0b004n02048m1000000a1000000f  ./run.bat 0  02048 1000000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m1000000a1000000f.txt  --job-name 0b008n02048m1000000a1000000f  ./run.bat 0  02048 1000000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m1000000a1000000f.txt  --job-name 0b016n02048m1000000a1000000f  ./run.bat 0  02048 1000000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m1000000a1000000f.txt  --job-name 0b032n02048m1000000a1000000f  ./run.bat 0  02048 1000000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m1000000a1000000f.txt  --job-name 0b064n02048m1000000a1000000f  ./run.bat 0  02048 1000000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m1000000a1000000f.txt  --job-name 0b004n04096m1000000a1000000f  ./run.bat 0  04096 1000000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m1000000a1000000f.txt  --job-name 0b008n04096m1000000a1000000f  ./run.bat 0  04096 1000000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m1000000a1000000f.txt  --job-name 0b016n04096m1000000a1000000f  ./run.bat 0  04096 1000000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m1000000a1000000f.txt  --job-name 0b032n04096m1000000a1000000f  ./run.bat 0  04096 1000000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m1000000a1000000f.txt  --job-name 0b064n04096m1000000a1000000f  ./run.bat 0  04096 1000000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m1000000a1000000f.txt  --job-name 0b004n08192m1000000a1000000f  ./run.bat 0  08192 1000000 1000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m1000000a1000000f.txt  --job-name 0b008n08192m1000000a1000000f  ./run.bat 0  08192 1000000 1000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m1000000a1000000f.txt  --job-name 0b016n08192m1000000a1000000f  ./run.bat 0  08192 1000000 1000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m1000000a1000000f.txt  --job-name 0b032n08192m1000000a1000000f  ./run.bat 0  08192 1000000 1000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m1000000a1000000f.txt  --job-name 0b064n08192m1000000a1000000f  ./run.bat 0  08192 1000000 1000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0100000a2000000f.txt  --job-name 0b004n02048m0100000a2000000f  ./run.bat 0  02048 0100000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0100000a2000000f.txt  --job-name 0b008n02048m0100000a2000000f  ./run.bat 0  02048 0100000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0100000a2000000f.txt  --job-name 0b016n02048m0100000a2000000f  ./run.bat 0  02048 0100000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0100000a2000000f.txt  --job-name 0b032n02048m0100000a2000000f  ./run.bat 0  02048 0100000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0100000a2000000f.txt  --job-name 0b064n02048m0100000a2000000f  ./run.bat 0  02048 0100000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0100000a2000000f.txt  --job-name 0b004n04096m0100000a2000000f  ./run.bat 0  04096 0100000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0100000a2000000f.txt  --job-name 0b008n04096m0100000a2000000f  ./run.bat 0  04096 0100000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0100000a2000000f.txt  --job-name 0b016n04096m0100000a2000000f  ./run.bat 0  04096 0100000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0100000a2000000f.txt  --job-name 0b032n04096m0100000a2000000f  ./run.bat 0  04096 0100000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0100000a2000000f.txt  --job-name 0b064n04096m0100000a2000000f  ./run.bat 0  04096 0100000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0100000a2000000f.txt  --job-name 0b004n08192m0100000a2000000f  ./run.bat 0  08192 0100000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0100000a2000000f.txt  --job-name 0b008n08192m0100000a2000000f  ./run.bat 0  08192 0100000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0100000a2000000f.txt  --job-name 0b016n08192m0100000a2000000f  ./run.bat 0  08192 0100000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0100000a2000000f.txt  --job-name 0b032n08192m0100000a2000000f  ./run.bat 0  08192 0100000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0100000a2000000f.txt  --job-name 0b064n08192m0100000a2000000f  ./run.bat 0  08192 0100000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m0500000a2000000f.txt  --job-name 0b004n02048m0500000a2000000f  ./run.bat 0  02048 0500000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m0500000a2000000f.txt  --job-name 0b008n02048m0500000a2000000f  ./run.bat 0  02048 0500000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m0500000a2000000f.txt  --job-name 0b016n02048m0500000a2000000f  ./run.bat 0  02048 0500000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m0500000a2000000f.txt  --job-name 0b032n02048m0500000a2000000f  ./run.bat 0  02048 0500000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m0500000a2000000f.txt  --job-name 0b064n02048m0500000a2000000f  ./run.bat 0  02048 0500000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m0500000a2000000f.txt  --job-name 0b004n04096m0500000a2000000f  ./run.bat 0  04096 0500000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m0500000a2000000f.txt  --job-name 0b008n04096m0500000a2000000f  ./run.bat 0  04096 0500000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m0500000a2000000f.txt  --job-name 0b016n04096m0500000a2000000f  ./run.bat 0  04096 0500000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m0500000a2000000f.txt  --job-name 0b032n04096m0500000a2000000f  ./run.bat 0  04096 0500000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m0500000a2000000f.txt  --job-name 0b064n04096m0500000a2000000f  ./run.bat 0  04096 0500000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m0500000a2000000f.txt  --job-name 0b004n08192m0500000a2000000f  ./run.bat 0  08192 0500000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m0500000a2000000f.txt  --job-name 0b008n08192m0500000a2000000f  ./run.bat 0  08192 0500000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m0500000a2000000f.txt  --job-name 0b016n08192m0500000a2000000f  ./run.bat 0  08192 0500000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m0500000a2000000f.txt  --job-name 0b032n08192m0500000a2000000f  ./run.bat 0  08192 0500000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m0500000a2000000f.txt  --job-name 0b064n08192m0500000a2000000f  ./run.bat 0  08192 0500000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n02048m1000000a2000000f.txt  --job-name 0b004n02048m1000000a2000000f  ./run.bat 0  02048 1000000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n02048m1000000a2000000f.txt  --job-name 0b008n02048m1000000a2000000f  ./run.bat 0  02048 1000000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n02048m1000000a2000000f.txt  --job-name 0b016n02048m1000000a2000000f  ./run.bat 0  02048 1000000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n02048m1000000a2000000f.txt  --job-name 0b032n02048m1000000a2000000f  ./run.bat 0  02048 1000000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n02048m1000000a2000000f.txt  --job-name 0b064n02048m1000000a2000000f  ./run.bat 0  02048 1000000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n04096m1000000a2000000f.txt  --job-name 0b004n04096m1000000a2000000f  ./run.bat 0  04096 1000000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n04096m1000000a2000000f.txt  --job-name 0b008n04096m1000000a2000000f  ./run.bat 0  04096 1000000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n04096m1000000a2000000f.txt  --job-name 0b016n04096m1000000a2000000f  ./run.bat 0  04096 1000000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n04096m1000000a2000000f.txt  --job-name 0b032n04096m1000000a2000000f  ./run.bat 0  04096 1000000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n04096m1000000a2000000f.txt  --job-name 0b064n04096m1000000a2000000f  ./run.bat 0  04096 1000000 2000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n08192m1000000a2000000f.txt  --job-name 0b004n08192m1000000a2000000f  ./run.bat 0  08192 1000000 2000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n08192m1000000a2000000f.txt  --job-name 0b008n08192m1000000a2000000f  ./run.bat 0  08192 1000000 2000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n08192m1000000a2000000f.txt  --job-name 0b016n08192m1000000a2000000f  ./run.bat 0  08192 1000000 2000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n08192m1000000a2000000f.txt  --job-name 0b032n08192m1000000a2000000f  ./run.bat 0  08192 1000000 2000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n08192m1000000a2000000f.txt  --job-name 0b064n08192m1000000a2000000f  ./run.bat 0  08192 1000000 2000000 0

