vdrift_ref directory has the geant4 stand-alone simulation (DUNE-FD2) for MARLEY events
##To build the code:
##I am running the code in dunegpvm machine
##To build the code use:
source setupg4.sh
#To copy the geant4 files use:

git clone https://github.com/ajibpaudel/MARLEY_g4
#To build and install the code
cd vdrift_ref/vdrift_build
cmake ..
make -j
##To run the executable
##from the vdrift_ref directory use the command
root -l MARLEYg4.C
