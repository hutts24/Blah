#BLAH_USE_GLUT = 1

All: blah_shared
all :All
cleanAll: clean

LIBFLAGS := -lGLU -lSDL

ifdef BLAH_USE_GLUT
	LIBFLAGS := $(LIBFLAGS) -lglut
	BLAHDEFS := -DBLAH_USE_GLUT
endif

BLAHFILES := $(wildcard *.c)

BLAHOBJS := $(patsubst %.c,%.o, $(BLAHFILES))

#TODO - define macro to delete files for Windows or Unix
clean:
	del $(BLAHOBJS)

%.o : %.c %.h
	gcc -c -std=c17 -fPIC -Wall -Werror -O3 -Winline $< -o $@

blah_shared: $(BLAHOBJS)
	gcc -Wall -O3 -Werror $(BLAHOBJS) $(LIBFLAGS) $(BLAHDEFS) -shared -o libblah.so

blah_static: $(BLAHOBJS)
	gcc -Wall -O3 -Werror $(BLAHOBJS) $(BLAHDEFS) $(BLAHFILES) -o libblah.a

install: blah_shared
	cp *.h /usr/local/include/blah
	cp libblah.so /usr/local/lib
