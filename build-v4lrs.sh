#!/bin/bash

cd 3rdPartyTools/v4l2rtspserver
git apply ../v4l2rtspserver.patch

cmake -DCMAKE_C_FLAGS="-Os" -DLIVE555CFLAGS="-DNEWLOCALE_NOT_USED=1; -DBSD=1; -DSOCKLEN_T=socklen_t; -D_FILE_OFFSET_BITS=64; -D_LARGEFILE_SOURCE=1; -DALLOW_RTSP_SERVER_PORT_REUSE=1; -DNO_OPENSSL=1;" .
make VERBOSE=1

cd /work/3rdPartyTools/v4l2rtspserver

ls

/usr/xcc/arm-unknown-linux-uclibcgnueabi/bin/arm-unknown-linux-uclibcgnueabi-g++ --sysroot=/usr/xcc/arm-unknown-linux-uclibcgnueabi/arm-unknown-linux-uclibcgnueabi/sysroot -I /usr/xcc/arm-unknown-linux-uclibcgnueabi/include/ -O3 -DNDEBUG -rdynamic CMakeFiles/v4l2rtspserver.dir/main.cpp.o -o v4l2rtspserver   -L/work/3rdPartyTools/v4l2rtspserver/libv4l2cpp/../../anyka_v380ipcam_experiments/akv300-extract/libplat/lib  -L/work/3rdPartyTools/v4l2rtspserver/libv4l2cpp/../../anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib  -Wl,-rpath,/work/3rdPartyTools/v4l2rtspserver/libv4l2cpp/../../anyka_v380ipcam_experiments/akv300-extract/libplat/lib:/work/3rdPartyTools/v4l2rtspserver/libv4l2cpp/../../anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib: -static liblibv4l2rtspserver.a -static-libgcc -static-libstdc++ libv4l2cpp/liblibv4l2cpp.a -lplat_vi