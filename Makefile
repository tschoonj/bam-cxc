all: bam_cxc.so

#Mac OS X
#IDL_CFLAGS=-I/Applications/exelis/idl/external/include 

#Linux
IDL_CFLAGS=-I/usr/local/exelis/idl/external/include 

#Mac OS X
#bam_cxc.so: bam_cxc_idl.cpp
#	g++ $(shell pkg-config --cflags QtCore) $(IDL_CFLAGS) -c -fno-common -o bam_cxc_idl.o bam_cxc_idl.cpp
#	g++ -Wl,-undefined -Wl,dynamic_lookup -o bam_cxc.so -bundle bam_cxc_idl.o $(shell pkg-config --libs QtCore)

bam_cxc.so: bam_cxc_idl.cpp
	g++ $(shell pkg-config --cflags QtCore) $(IDL_CFLAGS) -c -fPIC -DPIC -o bam_cxc_idl.o bam_cxc_idl.cpp
	g++ -shared -fPIC -Wl,-soname -Wl,bam_cxc.so -o bam_cxc.so bam_cxc_idl.o $(shell pkg-config --libs QtCore)

clean:
	rm -f bam_cxc_idl.o bam_cxc.so
