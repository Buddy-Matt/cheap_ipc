#!/bin/bash

# cd 3rdPartyTools/anyka_v380ipcam_experiments/apps/ak_snapshot
# make clean
# make
# cd ../../../..

cd 3rdPartyTools
rm -rf jpeg-6b
tar -xvzf jpegsrc.v6b.tar.gz
cd jpeg-6b
./configure
make