#!/bin/bash

cd 3rdPartyTools/v4l2rtspserver
git apply ../v4l2rtspserver.patch

cmake -DCMAKE_C_FLAGS="-static -Os" -DLIVE555CFLAGS="-DNEWLOCALE_NOT_USED=1; -DBSD=1; -DSOCKLEN_T=socklen_t; -D_FILE_OFFSET_BITS=64; -D_LARGEFILE_SOURCE=1; -DALLOW_RTSP_SERVER_PORT_REUSE=1; -DNO_OPENSSL=1;" .
make