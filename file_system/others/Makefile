CFLAGS = -c -g -ansi -pedantic -Wall -std=gnu99 `pkg-config fuse --cflags --libs` -I../include/

LDFLAGS = `pkg-config fuse --cflags --libs`

# Uncomment on of the following three lines to compile
HEADERS = ../include/disk_emu.h ../include/sfs_api.h ../include/linkedList.h ../include/helpers.h
#SOURCES= ../src/disk_emu.c ../src/sfs_api.c ../src/linkedList.c ../src/helpers.c ../testers/fuse_wrap_old.c
SOURCES= ../src/disk_emu.c ../src/sfs_api.c ../src/linkedList.c ../src/helpers.c ../testers/fuse_wrap_new.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sfs

all: $(SOURCES) $(HEADERS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(OBJECTS) $(HEADERS) $(LDFLAGS) -o $@

.c.o:
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *~ $(EXECUTABLE)
