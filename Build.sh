#!/bin/bash

echo "Checking out submodules"

git submodule update --init

echo "Compiling v4lrtspserver"
cd 3rdPartyTools/v4l2rtspserver
../dockcross/dockcross-linux-armv5-uclibc cmake -DLIVE555CFLAGS="-DNEWLOCALE_NOT_USED=1  -DBSD=1 -DSOCKLEN_T=socklen_t -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1" .
../dockcross/dockcross-linux-armv5-uclibc make
cp uv4l ../../SD_Card/


#prep anyka
#cd 3rdPartyTools/anyka
#rep -rl arm-none-linux-gnueabi | xargs sed -i s/arm-none-linux-gnueabi/armv5-unknown-linux-gnueabi/g

