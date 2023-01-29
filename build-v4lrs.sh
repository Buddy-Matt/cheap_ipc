#!/bin/bash

cd 3rdPartyTools
rm v4l2rtspserver -r
git clone git@github.com:mpromonet/v4l2rtspserver.git
cd v4l2rtspserver
cmake -DLIVE555CFLAGS="-DNEWLOCALE_NOT_USED=1  -DBSD=1 -DSOCKLEN_T=socklen_t -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE=1 -DALLOW_RTSP_SERVER_PORT_REUSE=1" .
make