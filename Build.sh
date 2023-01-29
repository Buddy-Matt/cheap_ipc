#!/bin/bash

echo "Checking out submodules"

git submodule update --init

#create dockcross cmd here
cd 3rdPartyTools/dockcross/
docker run --rm dockcross/linux-armv5-uclibc > ../../dockcross-run
cd ../../
chmod +x dockcross-run

echo "Compiling v4lrtspserver"
./dockcross-run ./build-v4lrs.sh
cp 3rdPartyTools/v4l2rtspserver.git/uv4l SD_Card/


#prep anyka
#cd 3rdPartyTools/anyka
#rep -rl arm-none-linux-gnueabi | xargs sed -i s/arm-none-linux-gnueabi/armv5-unknown-linux-gnueabi/g

