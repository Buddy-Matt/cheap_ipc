#!/bin/bash

echo "*************************"
echo "*Checking out submodules*"
echo "*************************"
echo ""
git submodule update --init

echo ""
echo "********************************************"
echo "*Prepping dockcross for cross-env compiling*"
echo "********************************************"
echo ""
#create dockcross cmd here
cd 3rdPartyTools/dockcross/
docker run --rm dockcross/linux-armv5-uclibc > ../../dockcross-run
cd ../../
chmod +x dockcross-run


echo ""
echo "*******************"
echo "*Compiling libjpeg*"
echo "*******************"
echo ""
./dockcross-run ./build-jpegLib.sh

echo ""
echo "********************"
echo "*Compiling snapshot*"
echo "********************"
echo ""
./dockcross-run ./build-imageGrab.sh


echo ""
echo "************************"
echo "*Creating SD Card image*"
echo "************************"
echo ""

rm -rf SD_Card
mkdir -p SD_Card/Factory
mkdir -p SD_Card/bin
mkdir -p SD_Card/lib
mkdir -p SD_Card/www

#main script to intercept and stop the normal boot stuff
cp -v src/scripts/config.sh SD_Card/Factory/

#libs
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib/ld-linux.so.3 SD_Card/lib
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib/libc.so.6 SD_Card/lib
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/usr/lib/libakuio.so SD_Card/lib
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/usr/lib/libplat_common.so SD_Card/lib
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/usr/lib/libplat_ipcsrv.so SD_Card/lib
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/usr/lib/libplat_thread.so SD_Card/lib
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/usr/lib/libplat_vi.so SD_Card/lib
cp -v 3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/usr/lib/libplat_vpss.so SD_Card/lib

#webserver
cp -v 3rdPartyTools/zsgx1hacks/sdcard/hack/busybox-armv6l SD_Card/bin/busybox
cp -rv src/webserver/. SD_Card/www

#v4l2rtspserver
#cp -v 3rdPartyTools/v4l2rtspserver/v4l2rtspserver SD_Card/bin

#snapshot
cp -v src/imageGrab/myCam SD_Card/bin

echo ""
echo "****************"
echo "*Build Complete*"
echo "****************"
echo ""
echo "Copy the contents of SD_Card to the root of a suitable SD Card, insert it in the camera & power on"
echo ""
