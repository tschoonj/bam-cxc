#include <cstdio>
#include <idl_export.h>
#include <QSharedMemory>
#include <QTextStream>
#include <iostream>
#include <cstdlib>

#define ARRLEN(arr) (sizeof(arr)/sizeof(arr[0]))

#define NPIXELX 264
#define NPIXELY 264
#define NCHANNELS 1024


extern IDL_VPTR IDL_CDECL idl_bam_cxc_get_data(int argc, IDL_VPTR argv[]);
extern IDL_VPTR IDL_CDECL idl_bam_cxc_get_hits(int argc, IDL_VPTR argv[]);
extern IDL_VPTR IDL_CDECL idl_bam_cxc_get_energy(int argc, IDL_VPTR argv[]);
extern IDL_VPTR IDL_CDECL idl_bam_cxc_get_miss(int argc, IDL_VPTR argv[]);
extern IDL_VPTR IDL_CDECL idl_bam_cxc_get_event(int argc, IDL_VPTR argv[]);

static IDL_SYSFUN_DEF2 idl_bam_cxc_functions[] = {
	{(IDL_FUN_RET) idl_bam_cxc_get_data, "BAM_CXC_GET_DATA", 0, 0, 0, 0},
	{(IDL_FUN_RET) idl_bam_cxc_get_hits, "BAM_CXC_GET_HITS", 0, 0, 0, 0},
	{(IDL_FUN_RET) idl_bam_cxc_get_energy, "BAM_CXC_GET_ENERGY", 0, 0, 0, 0},
	{(IDL_FUN_RET) idl_bam_cxc_get_miss, "BAM_CXC_GET_MISS", 0, 0, 0, 0},
	{(IDL_FUN_RET) idl_bam_cxc_get_event, "BAM_CXC_GET_EVENT", 0, 0, 0, 0}
};

IDL_MSG_BLOCK bam_cxc_msg_block;

#define BAM_CXC_ERROR 0

static IDL_MSG_DEF bam_cxc_msg_arr[] = {
	{"BAM_CXC_ERROR", "%NError: %s."}
};

extern "C" int IDL_Load(void) {
	if (!(bam_cxc_msg_block = IDL_MessageDefineBlock("BAM_CXC", ARRLEN(bam_cxc_msg_arr), bam_cxc_msg_arr))) {
		return IDL_FALSE;
	}
	return IDL_SysRtnAdd(idl_bam_cxc_functions, TRUE, IDL_CARRAY_ELTS(idl_bam_cxc_functions));
}

static unsigned long int* get_shared_memory(const char* key) {
	QSharedMemory shared_memory(key);

	//QTextStream(stdout) << shared_memory.nativeKey() << endl;

	if (!shared_memory.attach(QSharedMemory::ReadOnly)) {
		IDL_MessageFromBlock(bam_cxc_msg_block, BAM_CXC_ERROR, IDL_MSG_LONGJMP, "Could not attach to shared memory block %s", key);
	}

	shared_memory.lock();
	int size = shared_memory.size();
	if (size != sizeof(unsigned long int) * NPIXELX * NPIXELY) {
		shared_memory.unlock();
		shared_memory.detach();
		IDL_MessageFromBlock(bam_cxc_msg_block, BAM_CXC_ERROR, IDL_MSG_LONGJMP, "Unexpected size for shared memory block %s", key);
	
	}
	const unsigned long int *data = (const unsigned long int *) shared_memory.data();
	unsigned long int *data_copy = (unsigned long int *) malloc(sizeof(unsigned long int) * NPIXELX * NPIXELY);
	memcpy(data_copy, data, sizeof(unsigned long int) * NPIXELX * NPIXELY);

	shared_memory.unlock();
	shared_memory.detach();

	return data_copy;
}


IDL_VPTR IDL_CDECL idl_bam_cxc_get_data(int argc, IDL_VPTR argv[]) {
	QSharedMemory shared_memory("photo");

	//QTextStream(stdout) << shared_memory.nativeKey() << endl;

	if (!shared_memory.attach(QSharedMemory::ReadOnly)) {
		IDL_MessageFromBlock(bam_cxc_msg_block, BAM_CXC_ERROR, IDL_MSG_LONGJMP, "Could not attach to shared memory block photo");
	}

	shared_memory.lock();
	int size = shared_memory.size();
	if (size != sizeof(unsigned int) * NPIXELX * NPIXELY * NCHANNELS) {
		shared_memory.unlock();
		shared_memory.detach();
		IDL_MessageFromBlock(bam_cxc_msg_block, BAM_CXC_ERROR, IDL_MSG_LONGJMP, "Unexpected size for shared memory block photo");
	
	}
	const unsigned int *data = (const unsigned int *) shared_memory.data();
	unsigned int *data_copy = (unsigned int *) malloc(sizeof(unsigned int) * NPIXELX * NPIXELY * NCHANNELS);
	memcpy(data_copy, data, sizeof(unsigned int) * NPIXELX * NPIXELY * NCHANNELS);

	shared_memory.unlock();
	shared_memory.detach();

	int n_dim = 3;
	IDL_MEMINT dim[3] = {NCHANNELS, NPIXELY, NPIXELX};
	IDL_VPTR rv = IDL_ImportArray(n_dim, dim, IDL_TYP_ULONG, (UCHAR *) data_copy, (IDL_ARRAY_FREE_CB) free, 0);

	return rv;
}

IDL_VPTR IDL_CDECL idl_bam_cxc_get_hits(int argc, IDL_VPTR argv[]) {

	unsigned long int *data = get_shared_memory("hits");
	
	int n_dim = 2;
	IDL_MEMINT dim[2] = {NPIXELY, NPIXELX};
	IDL_VPTR rv = IDL_ImportArray(n_dim, dim, IDL_TYP_ULONG64, (UCHAR *) data, (IDL_ARRAY_FREE_CB) free, 0);

	return rv;
}

IDL_VPTR IDL_CDECL idl_bam_cxc_get_energy(int argc, IDL_VPTR argv[]) {

	unsigned long int *data = get_shared_memory("energy");
	
	int n_dim = 2;
	IDL_MEMINT dim[2] = {NPIXELY, NPIXELX};
	IDL_VPTR rv = IDL_ImportArray(n_dim, dim, IDL_TYP_ULONG64, (UCHAR *) data, (IDL_ARRAY_FREE_CB) free, 0);

	return rv;
}

IDL_VPTR IDL_CDECL idl_bam_cxc_get_miss(int argc, IDL_VPTR argv[]) {

	unsigned long int *data = get_shared_memory("miss");
	
	int n_dim = 2;
	IDL_MEMINT dim[2] = {NPIXELY, NPIXELX};
	IDL_VPTR rv = IDL_ImportArray(n_dim, dim, IDL_TYP_ULONG64, (UCHAR *) data, (IDL_ARRAY_FREE_CB) free, 0);

	return rv;
}

IDL_VPTR IDL_CDECL idl_bam_cxc_get_event(int argc, IDL_VPTR argv[]) {

	unsigned long int *data = get_shared_memory("event");
	
	int n_dim = 2;
	IDL_MEMINT dim[2] = {NPIXELY, NPIXELX};
	IDL_VPTR rv = IDL_ImportArray(n_dim, dim, IDL_TYP_ULONG64, (UCHAR *) data, (IDL_ARRAY_FREE_CB) free, 0);

	return rv;
}
