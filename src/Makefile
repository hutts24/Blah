#BLAH_USE_GLUT = 1

All: blah_shared
all :All
cleanAll: clean

clean:
	rm *.o

LIBFLAGS := -lGLU -lSDL

ifdef BLAH_USE_GLUT
	LIBFLAGS := $(LIBFLAGS) -lglut
	BLAHDEFS := -DBLAH_USE_GLUT
endif

BLAHFILES := $(shell echo *.c)

BLAHOBJS := $(subst .c,.o, $(BLAHFILES))

%.o : %.c %.h
	$(CC) -c -fPIC -Wall -Werror -O3 -Winline $< -o $@

blah_shared: $(BLAHOBJS)
	gcc -Wall -O3 -Werror $(BLAHOBJS) $(LIBFLAGS) $(BLAHDEFS) -shared -o libblah.so

blah_static: $(BLAHOBJS)
	gcc -Wall -O3 -Werror $(BLAHOBJS) $(BLAHDEFS) $(BLAHFILES) -o libblah.a

install: blah_shared
	cp *.h /usr/local/include/blah
	cp libblah.so /usr/local/lib
