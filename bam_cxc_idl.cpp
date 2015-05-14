#include <cstdio>
#include <idl_export.h>
#include <QSharedMemory>
#include <iostream>
#include <cstdlib>

#define ARRLEN(arr) (sizeof(arr)/sizeof(arr[0]))

extern IDL_VPTR IDL_CDECL idl_bam_cxc_get_data(int argc, IDL_VPTR argv[]);

static IDL_SYSFUN_DEF2 idl_bam_cxc_functions[] = {
	{(IDL_FUN_RET) idl_bam_cxc_get_data, "BAM_CXC_GET_DATA", 0, 0, 0, 0}
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


IDL_VPTR IDL_CDECL idl_bam_cxc_get_data(int argc, IDL_VPTR argv[]) {
	QSharedMemory shared_memory("fibonacci");

	if (!shared_memory.attach(QSharedMemory::ReadOnly)) {
		IDL_MessageFromBlock(bam_cxc_msg_block, BAM_CXC_ERROR, IDL_MSG_LONGJMP, "Could not attach to shared memory block fibonacci");
	}

	shared_memory.lock();
	int size = shared_memory.size();
	if (size != sizeof(int)*1000) {
		IDL_MessageFromBlock(bam_cxc_msg_block, BAM_CXC_ERROR, IDL_MSG_LONGJMP, "Unexpected size for shared memory block fibonacci");
	
	}
	const int *data = (const int *) shared_memory.data();
	int *data_copy = (int *) malloc(sizeof(int)*1000);
	memcpy(data_copy, data, sizeof(int)*1000);

	shared_memory.unlock();
	shared_memory.detach();

	int n_dim = 1;
	IDL_MEMINT dim[1] = {1000};
	IDL_VPTR rv = IDL_ImportArray(n_dim, dim, IDL_TYP_LONG, (UCHAR *) data_copy, (IDL_ARRAY_FREE_CB) free, 0);

	return rv;
}
