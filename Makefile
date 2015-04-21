#CC=/opt/wisermind_LS2H64/bin/mips64el-linux-gcc
CC=gcc

CFLAGS = -D__Linux__ -g -Wall
LDFLAGS +=  -lpthread -lm

EXEC = embedhub
OBJS = embedhub.c bitmap.c readcfg.c


all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS) $(LDLIBS$(LDLIBS-$(@)))
	
#	cp -rf $(EXEC) /nfs

clean:
	rm -rf *.o *.bak $(EXEC) 
