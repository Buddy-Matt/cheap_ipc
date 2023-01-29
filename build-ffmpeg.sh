#!/bin/bash

cd 3rdPartyTools/FFmpeg

DISABLE_AUTODETECT_LIST=$(./configure -h | grep '\[autodetect\]' | grep -o -P "\\-\\-[^ ]+" | sed -E "s/--(enable|disable)/--disable/g")

EVERYTHING_LIST="
    --disable-everything \
    --enable-decoder=h264,aac,mp3 \
    --enable-encoder=aac,libmp3lame \
    --enable-parser=h264,aac,mp3 \
    --enable-demuxer=flv,mov,mpegts,h264,aac,mp3,live_flv \
    --enable-muxer=flv,mov,mpegts \
    --enable-protocol=file,rtmp,pipe,hls,rtp,rtsp \
    --enable-bsf=aac_adtstoasc,h264_mp4toannexb \
    --enable-indev=v4l2"



mkdir -p build
cd build
LDFLAGS="-static"
../configure \
    --arch=armv4l --target-os=linux --cross-prefix=arm-unknown-linux-uclibcgnueabi- \
	${EVERYTHING_LIST} \
    --disable-swscale --disable-postproc --disable-doc \
    --enable-pthreads --enable-gpl --enable-version3 \
    --enable-avcodec --enable-avformat --enable-swresample --enable-avfilter \
    --disable-programs --enable-ffmpeg --enable-small \
    --enable-static
make -j4


