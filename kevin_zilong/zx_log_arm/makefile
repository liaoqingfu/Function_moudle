
LIBNAME  = libzxlog.so

#CC       := /opt/buildroot-gcc463/usr/bin/mipsel-linux-gcc
CC       := /opt/hisi-linux/x86-arm/arm-hisiv300-linux/target/bin/arm-hisiv300-linux-gcc

SRC	     = $(wildcard ./src/*.c)
OBJS	 = $(SRC:.c=.o)

CFLAGS   += -I./inc 
#CFLAGS   += -I/opt/buildroot-gcc463/usr/mipsel-buildroot-linux-uclibc/include
CFLAGS   += -I /home/work/Hi3518E_SDK_V1.0.4.0/osdrv/opensource/toolchain/arm-hisiv300-linux/arm-hisiv300-linux/target/usr/include
#CFLAGS   += -I /home/work/Hi3518E_SDK_V1.0.4.0/osdrv/opensource/toolchain/arm-hisiv300-linux/arm-hisiv300-linux/target/usr/include/linux
                 
CFLAGS   += -Wall -fPIC -DLINUX -DHAVE_CONFIG_H 
#-finline-functions

LDFLAGS  = -L./lib
LDFLAGS  += -L /osdrv/pub/rootfs_uclibc/lib/
#LDFLAGS  += -L /opt/hisi-linux/x86-arm/arm-hisiv300-linux/target/lib
LDFLAGS += -L /home/work/Hi3518E_SDK_V1.0.4.0/osdrv/opensource/toolchain/arm-hisiv300-linux/arm-hisiv300-linux/target/a9_soft

LDFLAGS  += -shared

#-O2 -Wl,--gc-sections

LIB      = -lm -lsupp -ldl -lpthread
#

all: clean $(LIBNAME)

$(LIBNAME): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIB)

romfs:
	$(ROMFSINST) /lib/$(LIBNAME)


clean:
	-rm -f $(LIBNAME) *~ ./src/*.o
