#!/bin/sh

export  BG_MAPCOMMONHEAP=1

sbatch --partition debug    --nodes 4   -n 4     -o out004n00256m0100000a4000000f.txt  --job-name 004n00256m0100000a4000000f  ./run.bat 0  00256 0100000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00256m0100000a4000000f.txt  --job-name 008n00256m0100000a4000000f  ./run.bat 0  00256 0100000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00256m0100000a4000000f.txt  --job-name 016n00256m0100000a4000000f  ./run.bat 0  00256 0100000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00256m0100000a4000000f.txt  --job-name 032n00256m0100000a4000000f  ./run.bat 0  00256 0100000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00256m0100000a4000000f.txt  --job-name 064n00256m0100000a4000000f  ./run.bat 0  00256 0100000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00512m0100000a4000000f.txt  --job-name 004n00512m0100000a4000000f  ./run.bat 0  00512 0100000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00512m0100000a4000000f.txt  --job-name 008n00512m0100000a4000000f  ./run.bat 0  00512 0100000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00512m0100000a4000000f.txt  --job-name 016n00512m0100000a4000000f  ./run.bat 0  00512 0100000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00512m0100000a4000000f.txt  --job-name 032n00512m0100000a4000000f  ./run.bat 0  00512 0100000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00512m0100000a4000000f.txt  --job-name 064n00512m0100000a4000000f  ./run.bat 0  00512 0100000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n01024m0100000a4000000f.txt  --job-name 004n01024m0100000a4000000f  ./run.bat 0  01024 0100000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n01024m0100000a4000000f.txt  --job-name 008n01024m0100000a4000000f  ./run.bat 0  01024 0100000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n01024m0100000a4000000f.txt  --job-name 016n01024m0100000a4000000f  ./run.bat 0  01024 0100000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n01024m0100000a4000000f.txt  --job-name 032n01024m0100000a4000000f  ./run.bat 0  01024 0100000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n01024m0100000a4000000f.txt  --job-name 064n01024m0100000a4000000f  ./run.bat 0  01024 0100000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00256m0500000a4000000f.txt  --job-name 004n00256m0500000a4000000f  ./run.bat 0  00256 0500000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00256m0500000a4000000f.txt  --job-name 008n00256m0500000a4000000f  ./run.bat 0  00256 0500000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00256m0500000a4000000f.txt  --job-name 016n00256m0500000a4000000f  ./run.bat 0  00256 0500000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00256m0500000a4000000f.txt  --job-name 032n00256m0500000a4000000f  ./run.bat 0  00256 0500000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00256m0500000a4000000f.txt  --job-name 064n00256m0500000a4000000f  ./run.bat 0  00256 0500000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00512m0500000a4000000f.txt  --job-name 004n00512m0500000a4000000f  ./run.bat 0  00512 0500000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00512m0500000a4000000f.txt  --job-name 008n00512m0500000a4000000f  ./run.bat 0  00512 0500000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00512m0500000a4000000f.txt  --job-name 016n00512m0500000a4000000f  ./run.bat 0  00512 0500000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00512m0500000a4000000f.txt  --job-name 032n00512m0500000a4000000f  ./run.bat 0  00512 0500000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00512m0500000a4000000f.txt  --job-name 064n00512m0500000a4000000f  ./run.bat 0  00512 0500000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n01024m0500000a4000000f.txt  --job-name 004n01024m0500000a4000000f  ./run.bat 0  01024 0500000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n01024m0500000a4000000f.txt  --job-name 008n01024m0500000a4000000f  ./run.bat 0  01024 0500000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n01024m0500000a4000000f.txt  --job-name 016n01024m0500000a4000000f  ./run.bat 0  01024 0500000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n01024m0500000a4000000f.txt  --job-name 032n01024m0500000a4000000f  ./run.bat 0  01024 0500000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n01024m0500000a4000000f.txt  --job-name 064n01024m0500000a4000000f  ./run.bat 0  01024 0500000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00256m1000000a4000000f.txt  --job-name 004n00256m1000000a4000000f  ./run.bat 0  00256 1000000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00256m1000000a4000000f.txt  --job-name 008n00256m1000000a4000000f  ./run.bat 0  00256 1000000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00256m1000000a4000000f.txt  --job-name 016n00256m1000000a4000000f  ./run.bat 0  00256 1000000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00256m1000000a4000000f.txt  --job-name 032n00256m1000000a4000000f  ./run.bat 0  00256 1000000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00256m1000000a4000000f.txt  --job-name 064n00256m1000000a4000000f  ./run.bat 0  00256 1000000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00512m1000000a4000000f.txt  --job-name 004n00512m1000000a4000000f  ./run.bat 0  00512 1000000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00512m1000000a4000000f.txt  --job-name 008n00512m1000000a4000000f  ./run.bat 0  00512 1000000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00512m1000000a4000000f.txt  --job-name 016n00512m1000000a4000000f  ./run.bat 0  00512 1000000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00512m1000000a4000000f.txt  --job-name 032n00512m1000000a4000000f  ./run.bat 0  00512 1000000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00512m1000000a4000000f.txt  --job-name 064n00512m1000000a4000000f  ./run.bat 0  00512 1000000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n01024m1000000a4000000f.txt  --job-name 004n01024m1000000a4000000f  ./run.bat 0  01024 1000000 4000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n01024m1000000a4000000f.txt  --job-name 008n01024m1000000a4000000f  ./run.bat 0  01024 1000000 4000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n01024m1000000a4000000f.txt  --job-name 016n01024m1000000a4000000f  ./run.bat 0  01024 1000000 4000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n01024m1000000a4000000f.txt  --job-name 032n01024m1000000a4000000f  ./run.bat 0  01024 1000000 4000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n01024m1000000a4000000f.txt  --job-name 064n01024m1000000a4000000f  ./run.bat 0  01024 1000000 4000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00256m0100000a8000000f.txt  --job-name 004n00256m0100000a8000000f  ./run.bat 0  00256 0100000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00256m0100000a8000000f.txt  --job-name 008n00256m0100000a8000000f  ./run.bat 0  00256 0100000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00256m0100000a8000000f.txt  --job-name 016n00256m0100000a8000000f  ./run.bat 0  00256 0100000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00256m0100000a8000000f.txt  --job-name 032n00256m0100000a8000000f  ./run.bat 0  00256 0100000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00256m0100000a8000000f.txt  --job-name 064n00256m0100000a8000000f  ./run.bat 0  00256 0100000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00512m0100000a8000000f.txt  --job-name 004n00512m0100000a8000000f  ./run.bat 0  00512 0100000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00512m0100000a8000000f.txt  --job-name 008n00512m0100000a8000000f  ./run.bat 0  00512 0100000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00512m0100000a8000000f.txt  --job-name 016n00512m0100000a8000000f  ./run.bat 0  00512 0100000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00512m0100000a8000000f.txt  --job-name 032n00512m0100000a8000000f  ./run.bat 0  00512 0100000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00512m0100000a8000000f.txt  --job-name 064n00512m0100000a8000000f  ./run.bat 0  00512 0100000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n01024m0100000a8000000f.txt  --job-name 004n01024m0100000a8000000f  ./run.bat 0  01024 0100000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n01024m0100000a8000000f.txt  --job-name 008n01024m0100000a8000000f  ./run.bat 0  01024 0100000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n01024m0100000a8000000f.txt  --job-name 016n01024m0100000a8000000f  ./run.bat 0  01024 0100000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n01024m0100000a8000000f.txt  --job-name 032n01024m0100000a8000000f  ./run.bat 0  01024 0100000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n01024m0100000a8000000f.txt  --job-name 064n01024m0100000a8000000f  ./run.bat 0  01024 0100000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00256m0500000a8000000f.txt  --job-name 004n00256m0500000a8000000f  ./run.bat 0  00256 0500000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00256m0500000a8000000f.txt  --job-name 008n00256m0500000a8000000f  ./run.bat 0  00256 0500000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00256m0500000a8000000f.txt  --job-name 016n00256m0500000a8000000f  ./run.bat 0  00256 0500000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00256m0500000a8000000f.txt  --job-name 032n00256m0500000a8000000f  ./run.bat 0  00256 0500000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00256m0500000a8000000f.txt  --job-name 064n00256m0500000a8000000f  ./run.bat 0  00256 0500000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00512m0500000a8000000f.txt  --job-name 004n00512m0500000a8000000f  ./run.bat 0  00512 0500000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00512m0500000a8000000f.txt  --job-name 008n00512m0500000a8000000f  ./run.bat 0  00512 0500000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00512m0500000a8000000f.txt  --job-name 016n00512m0500000a8000000f  ./run.bat 0  00512 0500000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00512m0500000a8000000f.txt  --job-name 032n00512m0500000a8000000f  ./run.bat 0  00512 0500000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00512m0500000a8000000f.txt  --job-name 064n00512m0500000a8000000f  ./run.bat 0  00512 0500000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n01024m0500000a8000000f.txt  --job-name 004n01024m0500000a8000000f  ./run.bat 0  01024 0500000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n01024m0500000a8000000f.txt  --job-name 008n01024m0500000a8000000f  ./run.bat 0  01024 0500000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n01024m0500000a8000000f.txt  --job-name 016n01024m0500000a8000000f  ./run.bat 0  01024 0500000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n01024m0500000a8000000f.txt  --job-name 032n01024m0500000a8000000f  ./run.bat 0  01024 0500000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n01024m0500000a8000000f.txt  --job-name 064n01024m0500000a8000000f  ./run.bat 0  01024 0500000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00256m1000000a8000000f.txt  --job-name 004n00256m1000000a8000000f  ./run.bat 0  00256 1000000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00256m1000000a8000000f.txt  --job-name 008n00256m1000000a8000000f  ./run.bat 0  00256 1000000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00256m1000000a8000000f.txt  --job-name 016n00256m1000000a8000000f  ./run.bat 0  00256 1000000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00256m1000000a8000000f.txt  --job-name 032n00256m1000000a8000000f  ./run.bat 0  00256 1000000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00256m1000000a8000000f.txt  --job-name 064n00256m1000000a8000000f  ./run.bat 0  00256 1000000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n00512m1000000a8000000f.txt  --job-name 004n00512m1000000a8000000f  ./run.bat 0  00512 1000000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n00512m1000000a8000000f.txt  --job-name 008n00512m1000000a8000000f  ./run.bat 0  00512 1000000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n00512m1000000a8000000f.txt  --job-name 016n00512m1000000a8000000f  ./run.bat 0  00512 1000000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n00512m1000000a8000000f.txt  --job-name 032n00512m1000000a8000000f  ./run.bat 0  00512 1000000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n00512m1000000a8000000f.txt  --job-name 064n00512m1000000a8000000f  ./run.bat 0  00512 1000000 8000000 0

sbatch --partition debug    --nodes 4   -n 4     -o out004n01024m1000000a8000000f.txt  --job-name 004n01024m1000000a8000000f  ./run.bat 0  01024 1000000 8000000 0
sbatch --partition debug    --nodes 8   -n 8     -o out008n01024m1000000a8000000f.txt  --job-name 008n01024m1000000a8000000f  ./run.bat 0  01024 1000000 8000000 0
sbatch --partition debug    --nodes 16  -n 16    -o out016n01024m1000000a8000000f.txt  --job-name 016n01024m1000000a8000000f  ./run.bat 0  01024 1000000 8000000 0
sbatch --partition debug    --nodes 32  -n 32    -o out032n01024m1000000a8000000f.txt  --job-name 032n01024m1000000a8000000f  ./run.bat 0  01024 1000000 8000000 0
sbatch --partition small    --nodes 64  -n 64    -o out064n01024m1000000a8000000f.txt  --job-name 064n01024m1000000a8000000f  ./run.bat 0  01024 1000000 8000000 0

