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




mkdir -p SD_Card/Factory
mkdir -p SD_Card/bin
mkdir -p SD_Card/www

#main script to intercept and stop the normal boot stuff
cp src/scripts/config.sh SD_Card/Factory/
#webserver
cp 3rdPartyTools/zsgx1hacks/sdcard/hack/busybox-armv6l SD_Card/bin/
cp -r src/webserver SD_Card/www

#v4l2rtspserver
cp 3rdPartyTools/v4l2rtspserver/v4l2rtspserver SD_Card/bin