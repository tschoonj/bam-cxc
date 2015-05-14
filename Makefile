all: fibonacci bam_cxc.so

fibonacci: fibonacci.cpp
	g++ -o fibonacci $(shell pkg-config --cflags QtCore) fibonacci.cpp $(shell pkg-config --libs QtCore)

bam_cxc.so: bam_cxc_idl.cpp
	g++ $(shell pkg-config --cflags QtCore) -I/Applications/exelis/idl/external/include -c -fno-common -o bam_cxc_idl.o bam_cxc_idl.cpp
	g++ -Wl,-undefined -Wl,dynamic_lookup -o bam_cxc.so -bundle bam_cxc_idl.o $(shell pkg-config --libs QtCore)

clean:
	rm -f fibonacci bam_cxc_idl.o bam_cxc.so
