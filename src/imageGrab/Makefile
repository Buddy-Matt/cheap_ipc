EXP_DIR = ../../3rdPartyTools/anyka_v380ipcam_experiments
PLAT_WORKDIR = $(EXP_DIR)/akv300-extract
BIN = myCam
CUR_PATH = $(shell echo $(PWD))
LIB_SEARCH_PATHS := \
	$(PLAT_WORKDIR)/libplat/lib \
    $(EXP_DIR)/rootfs_v1basic/rootfs/lib \
    ../../3rdPartyTools/jpeg-6b

	
INCLUDES := \
    $(CUR_PATH) \
	-I$(PLAT_WORKDIR)/libplat/include \
    -I$(PLAT_WORKDIR)/libmpi/include \
    -I../../3rdPartyTools/jpeg-6b

STATIC_LIBS := 

LIBS := akuio \
        akispsdk \
        plat_common \
        plat_thread \
        plat_vi \
        plat_vpss \
        plat_ipcsrv \
        libjpeg
#        mpi_venc \
#        plat_venc_cb \
#        akv_encode

CFLAGS += -fno-strict-aliasing -Os -w -D_GNU_SOURCE -std=c99 -fms-extensions

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := .
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
COMMA := ,

LDFLAGS := $(LDFLAGS) \
    $(patsubst %,-L%,$(LIB_SEARCH_PATHS)) \
    $(patsubst %,-Wl$(COMMA)-rpath-link=%,$(LIB_SEARCH_PATHS)) \
    -Wl,-Bstatic \
    -Wl,--start-group $(foreach n, $(STATIC_LIBS), -l$(patsubst lib%,%,$(patsubst %.a,%,$(notdir $(n))))) -Wl,--end-group \
    -Wl,-Bdynamic \
    $(foreach y, $(LIBS), -l$(patsubst lib%,%,$(patsubst %.so,%,$(notdir $(y)))))

    #-Wl,-rpath-link=$(subst $(space),:,$(strip $(LIB_SEARCH_PATHS))) \

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

$(BIN): $(OBJS)
	$(CC) -o $@ $(OBJS) $(LDFLAGS)

all:$(BIN)

$(OBJ_DIR):
	mkdir -p $@

clean:
	@$(RM) -rv $(BIN) $(OBJ_DIR) # The @ disables the echoing of the command





#/usr/xcc/arm-unknown-linux-uclibcgnueabi/bin/arm-unknown-linux-uclibcgnueabi-gcc -o myCam obj/main.o  -L../../3rdPartyTools/anyka_v380ipcam_experiments/akv300-extract/libplat/lib -L../../3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib -Wl,-rpath-link=../../3rdPartyTools/anyka_v380ipcam_experiments/akv300-extract/libplat/lib ../../3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib: -Wl,-Bstatic -Wl,--start-group  -Wl,--end-group -Wl,-Bdynamic  -lakuio  -lakispsdk  -lplat_common  -lplat_thread  -lplat_vi  -lplat_vpss

#/usr/xcc/arm-unknown-linux-uclibcgnueabi/bin/arm-unknown-linux-uclibcgnueabi-gcc -o myCam obj/main.o  -L../../3rdPartyTools/anyka_v380ipcam_experiments/akv300-extract/libplat/lib -L../../3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib -Wl, -rpath-link=../../3rdPartyTools/anyka_v380ipcam_experiments/akv300-extract/libplat/lib -rpath-link=../../3rdPartyTools/anyka_v380ipcam_experiments/rootfs_v1basic/rootfs/lib -Wl,-Bstatic -Wl,--start-group  -Wl,--end-group -Wl,-Bdynamic  -lakuio  -lakispsdk  -lplat_common  -lplat_thread  -lplat_vi  -lplat_vpss