/*
 * Copyright (C) 2009-2010 Advanced Micro Devices, Inc.  All Rights Reserved.
 */

/*
 *  Copyright (C) 2007. QLogic Corporation. All Rights Reserved.
 */

/*
 * Copyright 2003, 2004, 2005, 2006 PathScale, Inc.  All Rights Reserved.
 */

/*

  Copyright (C) 2000, 2001 Silicon Graphics, Inc.  All Rights Reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it would be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  

  Further, this software is distributed without any warranty that it is
  free of the rightful claim of any third person regarding infringement 
  or the like.  Any license provided herein, whether implied or 
  otherwise, applies only to this software file.  Patent licenses, if 
  any, provided herein do not apply to combinations of this program with 
  other software, or any other product whatsoever.  

  You should have received a copy of the GNU General Public License along
  with this program; if not, write the Free Software Foundation, Inc., 59
  Temple Place - Suite 330, Boston MA 02111-1307, USA.

  Contact information:  Silicon Graphics, Inc., 1600 Amphitheatre Pky,
  Mountain View, CA 94043, or:

  http://www.sgi.com

  For further information regarding this notice, see:

  http://oss.sgi.com/projects/GenInfo/NoticeExplan
 ***************************************************************************
  This file is created by Xiaonan(Daniel) Tian from HPCTools, University of Houston
  (daniel.xntian@gmail.com) for OpenUH OpenACC compiler.
  It is intended to lower the OpenACC pragma.
*/

#include <stdint.h>
#ifdef USE_PCH
#include "be_com_pch.h"
#endif /* USE_PCH */
#pragma hdrstop

/* Header of wn_mp_dg.cxx
*  csc.
*/
#include <sys/types.h>
#if defined(BUILD_OS_DARWIN)
#include <darwin_elf.h>
#else /* defined(BUILD_OS_DARWIN) */
#include <elf.h>
#endif /* defined(BUILD_OS_DARWIN) */

#define USE_STANDARD_TYPES          /* override unwanted defines in "defs.h" */

#include <bstring.h>
#include "wn.h"
#include "wn_util.h"
#include "erglob.h"
#include "errors.h"
#include "strtab.h"                 /* for strtab */
#include "symtab.h"                 /* for symtab */
#include "irbdata.h"                /* for inito */
#include "dwarf_DST_mem.h"          /* for DST */
#include "pu_info.h"
#ifdef __MINGW32__
#include <WINDOWS.h>
#endif /* __MINGW32__ */
#include "ir_bwrite.h"
#include "ir_reader.h"
#include "ir_bcom.h"
#include "region_util.h"            /* for RID */
#include "dep_graph.h"
#include "cxx_hash.h"
#include "wn_mp.h"        /* for wn_mp_dg.cxx's extern functions */
#include "file_util.h"      /* For Last_Pathname_Component */

/* wn_mp_dg.cxx header end.
*  csc.
*/

#include <string.h>

#if ! defined(BUILD_OS_DARWIN)
#include <elf.h>
#endif /* ! defined(BUILD_OS_DARWIN) */
#include "alloca.h"
#include "cxx_template.h"
#include "defs.h"
#include "glob.h"
#include "errors.h"
#include "erglob.h"
#include "erbe.h"
#include "tracing.h"
#include "strtab.h"

#include "symtab.h"

#include "wn.h"
#include "wn_util.h"
#include "wn_simp.h"
#include "stblock.h"
#include "data_layout.h"
#include "targ_sim.h"
#include "targ_const.h"
#include "config_targ.h"
#include "config_asm.h"
#include "const.h"
#include "ttype.h"
#include "wn_pragmas.h"
#include "wn_lower.h"
#include "region_util.h"
#include "wutil.h"
#include "wn_map.h"
#include "pu_info.h"
#include "dwarf_DST.h"
#include "dwarf_DST_producer.h"
#include "dwarf_DST_mem.h"
#include "config.h"
#include "standardize.h"
#include "irbdata.h"
#include "omp_lower.h"
#include "privatize_common.h"
#include "cxx_hash.h"
#include "wn_mp.h"
#include "mempool.h"
#include "parmodel.h"	// for NOMINAL_PROCS
#include "fb_info.h"
#include "fb_whirl.h"
#include "be_symtab.h"
#ifdef KEY
#include "wn_lower.h"
#include "config_opt.h"
#endif
#include "alias_analyzer.h"
#include <map>
using namespace std;

  
typedef enum {
	PAR_FUNC_ACC_NONE = 0,
	PAR_FUNC_ACC_KERNEL,
	PAR_FUNC_ACC_DEVICE,
	PAR_FUNC_ACC_LAST = PAR_FUNC_ACC_DEVICE
} PAR_FUNC_ACC_TYPE;


typedef struct {
	ST* hostName;
	ST* deviceName;
	WN* wnSize;
	WN* wnStart;
}ACC_DATA_ST_MAP;

typedef enum 
{
	ACC_KDATA_UNKOWN = 0,
//MTYPE_I1=2, 		 /*   8-bit integer */
	ACC_KDATA_UINT8,
//MTYPE_I2=3, 		 /*  16-bit integer */
	ACC_KDATA_UINT16,
//MTYPE_I4=4, 		 /*  32-bit integer */
	ACC_KDATA_UINT32,
//MTYPE_I8=5, 		 /*  64-bit integer */
	ACC_KDATA_UINT64,
//MTYPE_U1=6, 		 /*   8-bit unsigned integer */
	ACC_KDATA_INT8,
//MTYPE_U2=7, 		 /*  16-bit unsigned integer */
	ACC_KDATA_INT16,
//MTYPE_U4=8, 		 /*  32-bit unsigned integer */
	ACC_KDATA_INT32,
//MTYPE_U8=9, 		 /*  64-bit unsigned integer */
	ACC_KDATA_INT64,
//MTYPE_F4=10,		 /*  32-bit IEEE floating point */
	ACC_KDATA_FLOAT,
//MTYPE_F8=11,		 /*  64-bit IEEE floating point */
	ACC_KDATA_DOUBLE	
}ACC_KERNEL_DATA_TYPE;

static INT64 GetKernelParamType(ST* pParamST)
{
	
    TY_IDX ty = ST_type(pParamST);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    TYPE_ID typeID;
    if(kind == KIND_POINTER)
	{		
		TY_IDX pty = TY_pointed(ty);
		typeID = TY_mtype(pty);
	}
	else if(kind == KIND_SCALAR)
	{
		typeID = TY_mtype(ty);
	}
	else
	{
		Is_True(FALSE, ("Wrong Kernel Parameter Kind Type in GetKernelParamType 1."));
	}

	switch(typeID)
	{
	case MTYPE_I1: 		 /*   8-bit integer */
		return ACC_KDATA_UINT8;
	case MTYPE_I2:  		 /*  16-bit integer */
		return ACC_KDATA_UINT16;
	case MTYPE_I4:  		 /*  32-bit integer */
		return ACC_KDATA_UINT32;
	case MTYPE_I8:  		 /*  64-bit integer */
		return ACC_KDATA_UINT64;
	case MTYPE_U1:  		 /*   8-bit unsigned integer */
		return ACC_KDATA_INT8;
	case MTYPE_U2:  		 /*  16-bit unsigned integer */
		return ACC_KDATA_INT16;
	case MTYPE_U4:  		 /*  32-bit unsigned integer */
		return ACC_KDATA_INT32;
	case MTYPE_U8:  		 /*  64-bit unsigned integer */
		return ACC_KDATA_INT64;
	case MTYPE_F4: 		 /*  32-bit IEEE floating point */
		return ACC_KDATA_FLOAT;
	case MTYPE_F8: 		 /*  64-bit IEEE floating point */
		return ACC_KDATA_DOUBLE;
	default:
		Is_True(FALSE, ("Wrong Kernel Parameter Type ID in GetKernelParamType 2."));
	}
	return ACC_KDATA_UNKOWN;
}

typedef enum {

	ACCRUNTIME_NONE = 0,
	ACCRUNTIME_FIRST = 1,

	ACCR_SETUP              = 1,
	ACCR_CLEANUP            = 2,

	ACCR_SYNC				= 3,	/* Not really needed? to be deleted*/	
	ACCR_DEVICEMEMMALLOC			= 4,	
	ACCR_DEVICEMEMIN			= 5,
	ACCR_DEVICEMEMOUT			= 6,
	ACCR_LAUNCHKERNEL		= 7,
	ACCR_KERNELPARAMPUSH_POINTER	= 8,
	ACCR_PRESENT_COPY		= 9,
	ACCR_PRESENT_COPYIN		= 10,
	ACCR_PRESENT_COPYOUT	= 11,
	ACCR_PRESENT_CREATE		= 12,
	ACCR_SET_GANG_NUM_X		= 13,
	ACCR_SET_GANG_NUM_Y		= 14,
	ACCR_SET_GANG_NUM_Z		= 15,
	ACCR_SET_VECTOR_NUM_X	= 16,
	ACCR_SET_VECTOR_NUM_Y	= 17,
	ACCR_SET_VECTOR_NUM_Z	= 18,
	ACCR_DMEM_RELEASE		= 19,
	ACCR_MAP_DREGION		= 20,
	ACCR_KERNELPARAMPUSH_SCALAR	= 21,
	ACCR_KERNELPARAMPUSH_DOUBLE = 22,
	ACCR_REDUCTION_BUFF_MALLOC = 23,
	ACCR_FINAL_REDUCTION_ALGORITHM = 24,
	ACCR_FREE_ON_DEVICE = 25,
	ACCR_SETUP_DEFAULT_TOLOGY = 26,
	ACCR_SETUP_GANG_TOLOGY 	  = 27,
	ACCR_SETUP_VECTOR_TOLOGY 	  = 28,
	ACCR_RESET_DEFAULT_TOLOGY = 29,
	ACCR_GET_DEVICE_ADDR = 30,
	ACCR_UPDATE_HOST_VARIABLE = 31,
	ACCR_UPDATE_DEVICE_VARIABLE 	= 32,
	ACCR_WAIT_SOME_OR_ALL_STREAM	= 33,
	ACCR_SYNCTHREADS				= 34, //this one is only used in kernel functions
	ACCR_PRINTF_DBG				= 35,
	ACCR_GET_NUM_GANGS			= 36,
	ACCR_GET_NUM_WORKERS		= 37,
	ACCR_GET_NUM_VECTORS		= 38,	
	ACCR_GET_TOTAL_VECTORS		= 39,	
	ACCR_GET_TOTAL_GANGS		= 40,
	ACCR_GET_TOTAL_GANGS_WORKERS = 41,
	ACCR_CALL_LOCAL_REDUCTION	= 42,
	ACCR_DYNAMIC_LAUNCH_KERNEL	= 43,
	ACCRUNTIME_LAST 		= ACCR_DYNAMIC_LAUNCH_KERNEL
} OACCRUNTIME;

static const char *accr_names [ACCRUNTIME_LAST + 1] = {
  "",				/* MPRUNTIME_NONE */
  "__accr_setup",			/* ACCR_SETUP */
  "__accr_cleanup",		/* ACCR_CLEANUP */
  "__accr_sync",    	/* ACCR_SYNC */
  "__accr_malloc_on_device",  	/* ACCR_CUDAMALLOC */
  "__accr_memin_h2d",		/*ACCR_CUDAMEMIN*/
  "__accr_memout_d2h",		/*ACCR_CUDAMEMOUT*/
  "__accr_launchkernel",	/*ACCR_LAUNCHKERNEL*/
  "__accr_push_kernel_param_pointer", /*ACCR_KERNELPARAMPUSH*/
  "__accr_present_copy",		/*ACCR_PRESENT_COPY*/
  "__accr_present_copyin",		/*ACCR_PRESENT_COPYIN*/
  "__accr_present_copyout",		/*ACCR_PRESENT_COPYOUT*/
  "__accr_present_create",		/*ACCR_PRESENT_CREATE*/
  "__accr_set_gang_num_x",		/*ACCR_SET_GANG_NUM_X*/
  "__accr_set_gang_num_y",		/*ACCR_SET_GANG_NUM_Y*/
  "__accr_set_gang_num_z",		/*ACCR_SET_GANG_NUM_Z*/
  "__accr_set_vector_num_x",	/*ACCR_SET_VECTOR_NUM_X*/
  "__accr_set_vector_num_y",	/*ACCR_SET_VECTOR_NUM_Y*/
  "__accr_set_vector_num_z",	/*ACCR_SET_VECTOR_NUM_Z*/
  "__accr_dmem_release",		/*ACCR_DMEM_RELEASE*/
  "__accr_map_data_region",		/*map data region*/  
  "__accr_push_kernel_param_scalar", 
  "__accr_push_kernel_param_double",
  "__accr_reduction_buff_malloc",
  "__accr_final_reduction_algorithm",
  "__accr_free_on_device",
  "__accr_set_default_gang_vector",
  "__accr_set_gangs",
  "__accr_set_vectors",
  "__accr_reset_default_gang_vector",
  "__accr_get_device_addr",
  "__accr_update_host_variable",
  "__accr_update_device_variable",
  "__accr_wait_some_or_all_stream",
  "__syncthreads",
  "printf",
  "__accr_get_num_gangs",
  "__accr_get_num_workers",
  "__accr_get_num_vectors",
  "__accr_get_total_num_vectors",
  "__accr_get_total_num_gangs",
  "__accr_get_total_gangs_workers",
  "__accr_call_local_reduction",
  "__accr_dynamic_launch_kernel"
};


/*  This table contains ST_IDX entries entries for each of the MP
    runtime routines.  These entries allow efficient sharing of all
    calls to a particular runtime routine. */

static ST_IDX accr_sts [ACCRUNTIME_LAST + 1] = {
  ST_IDX_ZERO,	 /* ACCRUNTIME_NONE */
  ST_IDX_ZERO,	 /* ACCR_SETUP */
  ST_IDX_ZERO,	 /* ACCR_CLEANUP */
  ST_IDX_ZERO,   /* ACCR_SYNC */
  ST_IDX_ZERO,   /* ACCR_CUDAMALLOC */
  ST_IDX_ZERO,   /* ACCR_CUDAMEMIN */
  ST_IDX_ZERO,   /* ACCR_CUDAMEMOUT */
  ST_IDX_ZERO,	 /*ACCR_LAUNCHKERNEL*/
  ST_IDX_ZERO,	 /*ACCR_KERNELPARAMPUSH*/
  ST_IDX_ZERO,	/*ACCR_PRESENT_COPY*/
  ST_IDX_ZERO,	/*ACCR_PRESENT_COPYIN*/
  ST_IDX_ZERO,	/*ACCR_PRESENT_COPYOUT*/
  ST_IDX_ZERO,	/*ACCR_PRESENT_CREATE*/
  ST_IDX_ZERO,	/*ACCR_SET_GANG_NUM_X*/
  ST_IDX_ZERO,	/*ACCR_SET_GANG_NUM_Y*/
  ST_IDX_ZERO,	/*ACCR_SET_GANG_NUM_Z*/
  ST_IDX_ZERO,	/*ACCR_SET_VECTOR_NUM_X*/
  ST_IDX_ZERO,	/*ACCR_SET_VECTOR_NUM_Y*/
  ST_IDX_ZERO,	/*ACCR_SET_VECTOR_NUM_Z*/
  ST_IDX_ZERO,  /*ACCR_DMEM_RELEASE*/
  ST_IDX_ZERO,  /*MAP DATA REGION*/
  ST_IDX_ZERO,  /*ACCR_PUSH_KERNEL_PARAM_INT*/
  ST_IDX_ZERO,  /*ACCR_PUSH_KERNEL_PARAM_DOUBLE*/
  ST_IDX_ZERO,  /*ACCR_REDUCTION_BUFF_MALLOC*/
  ST_IDX_ZERO,  /*ACCR_FINAL_REDUCTION_ALGORITHM*/
  ST_IDX_ZERO,	/**ACCR_FREE_ON_DEVICE*/
  ST_IDX_ZERO,	/*ACCR_SETUP_DEFAULT_TOLOGY*/
  ST_IDX_ZERO,	/*ACCR_SETUP_GANG_TOLOGY*/
  ST_IDX_ZERO, 	/*ACCR_SETUP_VECTOR_TOLOGY*/
  ST_IDX_ZERO, 	/*ACCR_RESET_DEFAULT_TOLOGY*/
  ST_IDX_ZERO, 	/*ACCR_GET_DEVICE_ADDR*/
  ST_IDX_ZERO, 	/*ACCR_UPDATE_HOST_VARIABLE*/
  ST_IDX_ZERO,	/*ACCR_UPDATE_DEVICE_VARIABLE*/
  ST_IDX_ZERO,	/*ACCR_WAIT_SOME_OR_ALL_STREAM*/
  ST_IDX_ZERO,	/*ACCR_SYNCTHREADS*/
  ST_IDX_ZERO,	/*FOR DEBUG*/
  ST_IDX_ZERO,	/*ACCR_GET_NUM_GANGS*/
  ST_IDX_ZERO,	/*ACCR_GET_NUM_WORKERS*/
  ST_IDX_ZERO,	/*ACCR_GET_NUM_VECTORS*/
  ST_IDX_ZERO,  /*ACCR_GET_TOTAL_VECTORS*/
  ST_IDX_ZERO,   /*ACCR_GET_TOTAL_GANGS*/
  ST_IDX_ZERO,	/*ACCR_GET_TOTAL_GANGS_WORKERS*/
  ST_IDX_ZERO,	/*ACCR_CALL_LOCAL_REDUCTION*/
  ST_IDX_ZERO	/*ACCR_DYNAMIC_LAUNCH_KERNEL*/
};


typedef enum {
  ACC_VAR_NONE        = 0,
  ACC_VAR_LOCAL       = 1,	//declare in the kernel function
  ACC_VAR_COPY        = 2,
  ACC_VAR_COPYIN      = 3,
  ACC_VAR_COPYOUT     = 4,
  ACC_VAR_REDUCTION   = 5,  
  ACC_VAR_VECTOR_REDUCTION   = 6
} ACC_VAR_TYPE;

typedef struct {
  ACC_VAR_TYPE   vtype;
  TYPE_ID    mtype;
  BOOL       has_offset;
  
  TY_IDX     ty;
  
  ST        *orig_st;
  WN_OFFSET  orig_offset;
  ST        *new_st;
  WN_OFFSET  new_offset;
  OPERATOR   reduction_opr; /* specified in REDUCTION pragma */
} ACC_VAR_TABLE;

ST_IDX Make_ACCRuntime_ST ( OACCRUNTIME rop );

#define GET_ACCRUNTIME_ST(x) (accr_sts[x] == ST_IDX_ZERO ? \
                             Make_ACCRuntime_ST(x) : accr_sts[x])
typedef enum 
{
	ACC_NONE_SPECIFIED,
	ACC_VECTOR = 1,
	ACC_GANG_VECTOR,
	ACC_WORKER,
	ACC_GANG,
	ACC_GANG_WORKER,
	ACC_WORKER_VECTOR,
	ACC_GANG_WORKER_VECTOR
}ACC_LOOP_TYPE;

typedef enum ACC_LOOP_LEVEL
{
	ACC_OUTTER_LOOP,
	ACC_MIDDER_LOOP,
	ACC_INNER_LOOP
}ACC_LOOP_LEVEL;

typedef struct ACC_ReductionMap
{
	ST* hostName;//host reduction ST
	ST* deviceName; //device memory ST, allocated in host side
	ST* st_Inkernel; //st in the kernel. For each reduction, there is a respective st* in the kernel, usually as kernel parameter.
	ST* reduction_kenels; //an independent kernel carry the final reduction
	ST* st_private_var; //private var in kernel to store reduction result
	WN* wn_private_var;
	ST* st_local_array;//local memory buffer for reduction in the block
	OPERATOR ReductionOpr; 
	ACC_LOOP_LEVEL acc_looplevel;
	ACC_LOOP_LEVEL acc_stmt_location; //where is the reduction stmt
	ACC_LOOP_TYPE looptype;
	WN* wn_IndexOpr;
	WN* wn_assignment2Array; //this is used in kernel
	WN* wn_initialAssign; //this is used in kernel
	WN* wn_assignBack2PrivateVar; //this is used in kernel
	ST* st_num_of_element;	
	ST* st_backupValue;		
	WN* wn_backupValue;	
	WN* wn_backupStmt;
	ACC_ReductionMap()
	{
		hostName = NULL;
		deviceName = NULL;
		st_Inkernel = NULL;
		reduction_kenels = NULL;
		st_private_var = NULL;
		wn_private_var = NULL;
		st_local_array = NULL;
		wn_IndexOpr = NULL;
		wn_assignment2Array = NULL;
		wn_initialAssign = NULL;
		wn_assignBack2PrivateVar = NULL;
		st_num_of_element = NULL;
		st_backupValue = NULL;
		wn_backupValue = NULL;
		wn_backupStmt = NULL;
	}
	
}ACC_ReductionMap;

typedef struct ACC_Reduction_Item
{
	ACC_LOOP_LEVEL acc_looplevel;
	WN* acc_wn_reduction;
}ACC_Reduction_Item;


//structure for preserve loop info
typedef struct
{
	WN* init;
	WN* condition;
	WN* incr;
	WN* acc_test_stmt; //the whole test statement
	ACC_LOOP_TYPE looptype;
	WN* vectors;	//a node contains number of vectors
	WN* gangs;		//a node contains number of gangs
	WN* workers;	//a node contains number of workers
	vector<ACC_ReductionMap> reductionmap;
	BOOL isIndependent;
	BOOL isSeq;
	WN* acc_private;
	INT32 private_count;
	//vector<ACC_Reduction_Item> acc_reduction;
	INT32 reduction_count;
	WN* acc_collapse;
	WN* acc_loopbody;
	ST * acc_index_st;		/* User forloop index variable ST */
	ST * acc_limit_st;
    TYPE_ID acc_index_type;
	char* szIndexName;
	char* szLimitName;
	ST * acc_newIndex;
	ST * acc_newLimit;
	WN* wn_regionbody;
	//this is used in parallel region, there may be some redundenccy execution code
	//WN * wn_following_nodes;
	//for nonperfect loopnest
	//for(.....)
	//{
	//	prehands nodes
	//	for(....){}
	//	afterhand nodes
	//}
	WN * wn_prehand_nodes;
	WN * wn_afterhand_nodes;
}FOR_LOOP_INFO;

typedef enum
{
	ACC_DU_UDEF = 0,
	ACC_DU_DEF,
	ACC_DU_USE,
	ACC_DU_USEDEF,
	ACC_DU_DEFUSE,
	ACC_LIVE_PRIVATE,
	ACC_LIVE_IN,
	ACC_LIVE_OUT,
	ACC_LIVE_INOUT
}ACC_DU_Liveness;



typedef struct ACC_VAR_Liveness
{
  ACC_DU_Liveness   DU_Live_type;
  ST* var_st;
  //WN_OFFSET  var_offset;
  //BOOL       has_offset;
  //TY_IDX     ty;
  //TYPE_ID    mtype;
  ACC_VAR_Liveness *pNext;
  ACC_VAR_Liveness *pPrev;
} ACC_VAR_Liveness;

static ACC_VAR_Liveness* acc_pHeadLiveness;

typedef struct 
{
	INT32 loopnum; //how many loopnest in this loop
	vector<FOR_LOOP_INFO> acc_forloop;
	WN* wn_following_nodes;
}ACC_LOOP_INFO;

typedef struct PARALLEL_LOOP_INFO
{
	INT32 loopnum; //how many loops in this parallel region
	vector<ACC_LOOP_INFO> acc_loopinfo;
	WN* wn_prehand_nodes;
}PARALLEL_LOOP_INFO;

ACC_LOOP_INFO acc_loopinfo;
PARALLEL_LOOP_INFO acc_parallel_loop_info;
ST* st_shared_array_4parallelRegion = NULL;
TY_IDX ty_shared_array_in_parallel = 0;

map<TYPE_ID, ST*> acc_global_memory_for_reduction_host;		//used in host side
map<TYPE_ID, ST*> acc_global_memory_for_reduction_device;	//used in host side
map<TYPE_ID, ST*> acc_global_memory_for_reduction_param;	//used in kernel parameters
map<TYPE_ID, ST*> acc_global_memory_for_reduction_block;	//used in a single block

//The following are using for shared memory 
ST* acc_st_shared_memory;
map<TYPE_ID, ST*> acc_shared_memory_for_reduction_block;	//used in device side

//////////////////////////////////////////////////////////////////////////
map<ST*, ACC_ReductionMap> acc_reduction_tab_map; //ST in Host side

typedef enum ReductionUsingMem
{
	ACC_RD_SHARED_MEM=0,
	ACC_RD_GLOBAL_MEM
}ReductionUsingMem;

typedef enum ReductionRolling
{
	ACC_RD_UNROLLING=0,
	ACC_RD_ROLLING
}ReductionRolling;

typedef enum ReductionExeMode
{
	ACC_RD_LAUNCH_KERNEL,
	ACC_RD_DYNAMICPARAL_EXE	
}ReductionExeMode;

static ReductionUsingMem acc_reduction_mem = ACC_RD_SHARED_MEM;
static ReductionRolling acc_reduction_rolling = ACC_RD_UNROLLING;
static ReductionExeMode acc_reduction_exemode;

#define WN_ACC_Compare_Trees(x,y)	(WN_Simp_Compare_Trees(x,y))  

typedef struct ACC_DREGION__ENTRY
{
	WN* acc_data_clauses;
	WN* acc_data_length;
}ACC_DREGION__ENTRY;

vector<ACC_DREGION__ENTRY> acc_dregion_pcreate;
vector<ACC_DREGION__ENTRY> acc_dregion_pcopy;
vector<ACC_DREGION__ENTRY> acc_dregion_pcopyin;
vector<ACC_DREGION__ENTRY> acc_dregion_pcopyout;
vector<ACC_DREGION__ENTRY> acc_dregion_present;
vector<ACC_DREGION__ENTRY> acc_dregion_host;
vector<ACC_DREGION__ENTRY> acc_dregion_device;
vector<ACC_DREGION__ENTRY> acc_dregion_private;
vector<ACC_DREGION__ENTRY> acc_dregion_fprivate;



typedef ST * ACC_SHARED_TABLE;

// Generic type for parallel runtime routines
static TY_IDX accruntime_ty = TY_IDX_ZERO;

  // TRUE if ACC region we're currently processing has the compiler-
  // generated flag set on its first pragma (the one that identifies it
  // as a PARALLEL_REGION, PDO, etc.), FALSE otherwise
static BOOL cacc_gen_construct;
// What kind of construct we're lowering.  We don't distinguish among
  // most of the simpler types. Note that mpt is set according to the
  // outermost construct by lower_mp (say, to MPP_PARALLEL_REGION), and
  // when we reach an inner construct (say, MPP_PDO) we save the old value
  // of mpt, set it to something appropriate for the inner construct until
  // we're done processing that construct, then restore the old value of mpt.
static ACCP_process_type acc_t;

static WN *acc_host_nodes;
static WN *acc_device_nodes;
static WN *acc_dlength_node;
static WN *acc_stmt_block;		/* Original statement nodes */
static WN *acc_cont_nodes;		/* Statement nodes after acc code */
static WN *acc_if_node;		/* Points to (optional) if node */
static WN *acc_num_gangs_node;
static WN *acc_num_workers_node;
static WN *acc_vector_length_node;
static WN *acc_collapse_node;
static WN *acc_gang_node;
static WN *acc_worker_node;
static WN *acc_vector_node;
static WN *acc_seq_node;
static WN *acc_independent_node;

static WN *acc_reduction_nodes;	/* Points to (optional) reduction nodes */
static INT32 acc_reduction_count; 

static WN *acc_copy_nodes;		/* Points to (optional) shared nodes */
static WN *acc_copyin_nodes;	/* Points to (optional) copyin nodes */
static WN *acc_copyout_nodes;	/* Points to (optional) copyout nodes */
static WN *acc_wait_nodes;	/* Points to (optional) acc wait pragma nodes */
static WN *acc_parms_nodes;	/* Points to (optional) parmeter nodes */

static WN *acc_create_nodes;
static WN *acc_present_nodes;
static WN *acc_present_or_copy_nodes;
static WN *acc_present_or_copyin_nodes;
static WN *acc_present_or_copyout_nodes;
static WN *acc_present_or_create_nodes;
static WN *acc_deviceptr_nodes;
static WN *acc_private_nodes;
static WN *acc_firstprivate_nodes;


static WN* acc_AsyncExpr = NULL;
static WN* acc_async_nodes;   /* async int expression */
static WN* acc_clause_intnum;	/*Int expression, it's for integer expression, e.g, in wait pragma*/

//This table is used to localize the ST  which is used in kernel/parallel region.
//static ACC_VAR_TABLE* acc_local_var_table;		//All the data in the kernel function
static map<ST*, ACC_VAR_TABLE> acc_local_new_var_map;     //used to replace var st in kernel function
//static map<ST*, ST*> acc_local_new_var_map;     //used to replace var st in kernel function

//static ACC_VAR_TABLE *acc_copy_var_table;	    	/* Table of variable substitutions */
//static ACC_VAR_TABLE *acc_copyin_var_table;	    	/* Table of variable substitutions */
//static ACC_VAR_TABLE *acc_copyout_var_table;	    /* Table of variable substitutions */


static ACC_SHARED_TABLE *acc_copyout_table;  /* Table of shared ST's */
static ACC_SHARED_TABLE *acc_copyin_table;  /* Table of shared ST's */
static ACC_SHARED_TABLE *acc_copy_table;  /* Table of shared ST's */
static ACC_SHARED_TABLE *acc_parm_table;  /* Table of shared ST's */

typedef struct acc_reduction_kernels_pair
{
	ST* st_kernels_fun;
	OPERATOR ReductionOpr; 
	TY_IDX opr_ty; //double, float, int, short, etc. It's scalar variables.
	ACC_LOOP_TYPE looptype;
}acc_reduction_kernels_pair;

static vector<acc_reduction_kernels_pair> acc_reduction_kernels_maps;
static vector<acc_reduction_kernels_pair> acc_reduction_devices_maps; //devices function for reduction
static map<ST*, ST*> acc_reduction_device_reduction_call;

static ST *acc_reduction_proc;	/* reduction for ACC process */
static ST *acc_parallel_proc;	/* Extracted parallel/kernels process */
static SYMTAB_IDX acc_psymtab;	/* Parent symbol table */
static SYMTAB_IDX acc_csymtab;	/* Child symbol table */
static INT32 acc_func_level;	/* Parallel function stab level */
static ST *acc_local_taskargs;  /* Microtask local task args */
static WN *acc_pragma_block;	/* Parallel funciton pragma block */
static WN *acc_reference_block;	/* Parallel funciton reference block */
static WN *acc_parallel_func;	/* General Kernels function */
static WN *acc_reduction_func;	/* reduction kernel function */
static WN *acc_last_microtask; /* func entry for last created microtask */
static WN *acc_replace_block;	/* Replacement nodes to be returned */

static DST_INFO_IDX  acc_nested_dst;

static PU_Info *acc_ppuinfo;	/* Parent PU info structure */


static INT64 acc_line_number;	/* Line number of acc parallel/kernel region */

static TY_IDX acc_region_ty = TY_IDX_ZERO;

static ST * acc_local_gtid;		/* Microtask local gtid */
static ST * acc_old_gtid_st = NULL;
static vector<ST *>  kernel_param;
//static vector<ST *>  reduction_kernel_param;
static vector<ACC_DATA_ST_MAP*> device_copy;
static vector<ACC_DATA_ST_MAP*> device_copyin;
static vector<ACC_DATA_ST_MAP*> device_copyout;
static vector<ACC_DATA_ST_MAP*> device_create;
static vector<ACC_DATA_ST_MAP*> device_present;
static vector<ACC_DATA_ST_MAP*> device_present_or_copy;
static vector<ACC_DATA_ST_MAP*> device_present_or_copyin;
static vector<ACC_DATA_ST_MAP*> device_present_or_copyout;
static vector<ACC_DATA_ST_MAP*> device_present_or_create;
//static vector<ACC_DATA_ST_MAP*> device_deviceptr;
//static vector<ACC_DATA_ST_MAP*> device_device_resident;

//For forloop analysis
static WN * acc_test_stmt;
static vector<WN *> acc_base_node;		  /* Parallel forloop base */
static vector<WN *> acc_limit_node;		/* Parallel forloop limit */
static WN *acc_ntrip_node;		/* Parallel forloop trip count, iterations in this loop */
static vector<WN *> acc_stride_node;		/* Parallel forloop stride */
static WN *acc_doloop_body;
static UINT32 acc_collapse_count;   /* collapse count */
static UINT32 acc_loopnest_depth;
static vector<ST *> acc_forloop_index_st;		/* User forloop index variable ST */
static vector<TYPE_ID> acc_forloop_index_type;	/* User forloop index variable type */

static UINT32 kernel_parameters_count = 0;
static UINT32 kernel_tmp_variable_count = 0; //not index, New ST, name didn't appear in host code.
static char acc_tmp_name_prefix[] = "__acc_tmp_";

static ST *glbl_threadIdx_x;
static ST *glbl_threadIdx_y;
static ST *glbl_threadIdx_z;
static ST *glbl_blockIdx_x;
static ST *glbl_blockIdx_y;
static ST *glbl_blockIdx_z;
static ST *glbl_blockDim_x;
static ST *glbl_blockDim_y;
static ST *glbl_blockDim_z;
static ST *glbl_gridDim_x;
static ST *glbl_gridDim_y;
static ST *glbl_gridDim_z;

WN* threadidx;
WN* threadidy;
WN* threadidz;

WN* blockidx;
WN* blockidy;
WN* blockidz;

WN* blockdimx;
WN* blockdimy;
WN* blockdimz;

WN* griddimx;
WN* griddimy;
WN* griddimz;
	

static WN* acc_wn_reduction_index;
static WN_MAP_TAB *acc_pmaptab;	/* Parent map table */
static WN_MAP_TAB *acc_cmaptab;	/* Child map table */

static const mINT32 ACC_NUM_HASH_ELEMENTS = 1021;
static vector<ST*> acc_kernel_functions_st; //ST list of kernel functions created


INT32 acc_region_num = 1;	 // MP region number within parent PU
INT32 acc_construct_num = 1; // construct number within MP region

//structure for preserve data region info
typedef struct
{
	vector<ACC_DREGION__ENTRY> acc_dregion_pcreate;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopy;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopyin;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopyout;
	vector<ACC_DREGION__ENTRY> acc_dregion_present;
	vector<ACC_DREGION__ENTRY> acc_dregion_private;
	vector<ACC_DREGION__ENTRY> acc_dregion_fprivate;
	WN * acc_if_node;
	WN * acc_copy_nodes;		/* Points to (optional) copy nodes */
	WN * acc_copyin_nodes;	/* Points to (optional) copyin nodes */
	WN * acc_copyout_nodes;	/* Points to (optional) copyout nodes */	
	WN * acc_create_nodes;  /*copy/copyin/copyout/create will merge into pcopy/pcopyin/pcopyout/pcreate*/
	/////////////////////////////////////////////////////////
	WN * acc_present_nodes;
	vector<ACC_DATA_ST_MAP*> presentMap;
	WN * acc_present_or_copy_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyMap;
	WN * acc_present_or_copyin_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyinMap;
	WN * acc_present_or_copyout_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyoutMap;
	WN * acc_present_or_create_nodes;
	vector<ACC_DATA_ST_MAP*> pcreateMap;
	WN * acc_deviceptr_nodes;
	vector<ST*> dptrList;
    ACC_VAR_Liveness* acc_pHeadLiveness;
	WN* wn_cont_nodes;
	WN* wn_stmt_block;
}SingleDRegionInfo;

typedef struct 
{
	UINT32 Depth;
	vector<SingleDRegionInfo> DRegionInfo;
}DataRegion;

static DataRegion acc_nested_dregion_info;

//structure for preserve kernels region info
typedef struct
{
	vector<ACC_DREGION__ENTRY> acc_dregion_pcreate;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopy;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopyin;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopyout;
	vector<ACC_DREGION__ENTRY> acc_dregion_present;
	vector<ACC_DREGION__ENTRY> acc_dregion_private;
	vector<ACC_DREGION__ENTRY> acc_dregion_fprivate;
	//////////////////////////////////////////////////////
	WN * acc_if_node;
	WN * acc_copy_nodes;		/* Points to (optional) copy nodes */
	WN * acc_copyin_nodes;	/* Points to (optional) copyin nodes */
	WN * acc_copyout_nodes;	/* Points to (optional) copyout nodes */	
	WN * acc_create_nodes;  /*copy/copyin/copyout/create will merge into pcopy/pcopyin/pcopyout/pcreate*/
	/////////////////////////////////////////////////////////
	WN * acc_present_nodes;
	vector<ACC_DATA_ST_MAP*> presentMap;
	WN * acc_present_or_copy_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyMap;
	WN * acc_present_or_copyin_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyinMap;
	WN * acc_present_or_copyout_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyoutMap;
	WN * acc_present_or_create_nodes;
	vector<ACC_DATA_ST_MAP*> pcreateMap;
	WN * acc_deviceptr_nodes;
	vector<ST*> dptrList;
	WN* acc_async;
	//kernel launch parameters, not in spec, 
	//when the def/use module done, it will be removed.
	WN* acc_param; 
    ACC_VAR_Liveness* acc_pHeadLiveness;
}KernelsRegionInfo;


//structure for preserve parallel region info
typedef struct
{
	vector<ACC_DREGION__ENTRY> acc_dregion_pcreate;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopy;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopyin;
	vector<ACC_DREGION__ENTRY> acc_dregion_pcopyout;
	vector<ACC_DREGION__ENTRY> acc_dregion_present;
	vector<ACC_DREGION__ENTRY> acc_dregion_private;
	vector<ACC_DREGION__ENTRY> acc_dregion_fprivate;
	////////////////////////////////////////////////////////////////
	WN * acc_if_node;
	WN * acc_copy_nodes;		/* Points to (optional) copy nodes */
	WN * acc_copyin_nodes;	/* Points to (optional) copyin nodes */
	WN * acc_copyout_nodes;	/* Points to (optional) copyout nodes */	
	WN * acc_create_nodes;  /*copy/copyin/copyout/create will merge into pcopy/pcopyin/pcopyout/pcreate*/
	/////////////////////////////////////////////////////////
	WN * acc_present_nodes;
	vector<ACC_DATA_ST_MAP*> presentMap;
	WN * acc_present_or_copy_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyMap;
	WN * acc_present_or_copyin_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyinMap;
	WN * acc_present_or_copyout_nodes;
	vector<ACC_DATA_ST_MAP*> pcopyoutMap;
	WN * acc_present_or_create_nodes;
	vector<ACC_DATA_ST_MAP*> pcreateMap;
	WN * acc_deviceptr_nodes;
	vector<ST*> dptrList;
	WN* acc_async;
	WN* acc_num_gangs;
	WN* acc_num_workers;
	WN* acc_vector_length;
	WN* acc_reduction;
	vector<ACC_DATA_ST_MAP*> reductionMap;
	WN* acc_private;
	vector<ACC_DATA_ST_MAP*> privateMap;
	WN* acc_firstprivate;
	vector<ACC_DATA_ST_MAP*> firstPrivateMap;	
	WN* acc_param; 
    ACC_VAR_Liveness* acc_pHeadLiveness;	
}ParallelRegionInfo;

typedef struct
{
	ST* st_host;
	ST*	st_device;
}KernelParameter;

vector<KernelParameter> acc_kernelLaunchParamList;
vector<KernelParameter> acc_additionalKernelLaunchParamList; //main for reduction

static ST* ACC_GenSingleCreateAndMallocDeviceMem(WN* l, 
						vector<ACC_DATA_ST_MAP*>* pDMap, WN* ReplacementBlock);
static WN* ACC_GenIsPCreate(WN* node);
static WN* ACC_GenIsPCopyIn(WN* node);
static WN* ACC_GenIsPCopyIn(WN* node);
static void ACC_GenDeviceCreateCopyInOut(WN* nodes, vector<ACC_DATA_ST_MAP*>* pDMap, 
								WN* ReplacementBlock, BOOL MemIn);
static void ACC_GenDataCopyOut(vector<ACC_DATA_ST_MAP*>* pDMap, WN* ReplacementBlock);
static void ACC_Process_Clause_Pragma(WN * tree);
static WN* ACC_Generate_KernelParameters(WN* paramlist, void* pRegionInfo, BOOL isKernelRegion);
static WN* ACC_Process_KernelsRegion( WN * tree, WN* wn_cont );
static WN* ACC_Process_ParallelRegion( WN * tree, WN* wn_cont );
static WN* ACC_Process_HostDataRegion( WN * tree );
static WN* ACC_Process_WaitRegion( WN * tree );
static WN* ACC_Process_UpdateRegion( WN * tree );
static WN* ACC_Process_CacheRegion( WN * tree );
static WN* ACC_Process_DeclareRegion( WN * tree );
static WN* ACC_Process_DataRegion( WN * tree );
static WN *LaunchKernel (int index, WN* wn_replace_block, BOOL bParallelRegion);
static ST* ACC_GenerateReduction_Kernels_TopLoop(ACC_ReductionMap* pReduction_map);
static WN* ACC_LoadDeviceSharedArrayElem(WN* wn_array_offset, ST* st_array);
static WN* ACC_Get_Init_Value_of_Reduction(OPERATOR ReductionOpr);
static char* ACC_Get_ScalarName_of_Reduction(TYPE_ID typeID);
static WN* ACC_Gen_Call_Local_Reduction(ST* st_device_func, ST* st_inputdata);
static ST* ACC_GenerateWorkerVectorReduction_unrolling(ACC_ReductionMap* pReduction_map);
static ST* ACC_GenerateVectorReduction_unrolling(ACC_ReductionMap* pReduction_map);
static ST* ACC_GenerateWorkerReduction_unrolling(ACC_ReductionMap* pReduction_map);
static ST* ACC_GenerateWorkerVectorReduction_rolling(ACC_ReductionMap* pReduction_map);
static ST* ACC_GenerateVectorReduction_rolling(ACC_ReductionMap* pReduction_map);
static ST* ACC_GenerateWorkerReduction_rolling(ACC_ReductionMap* pReduction_map);
static WN* Gen_Sync_Threads();



/*
To fix PV 553472, Walk_and_Localize() has to update alias information when
renaming variables in orphaned MP constructs.  These updates require that
we maintain parent pointers.  The following data structure allows
Walk_and_Localize() to maintain a list of parent pointers, for orphaned
constructs only.
*/

class ACC_Localize_Parent_Stack {
public:
  BOOL orphaned;
  STACK<WN *> parent_stack; // a node's parent is always below it on the stack
    // If _orphaned is FALSE, wn may be NULL; otherwise wn must be the top
    // of the Whirl tree to be localized
  ACC_Localize_Parent_Stack(BOOL _orphaned, WN *top) : orphaned(_orphaned),
    parent_stack(Malloc_Mem_Pool) { Push(top); }
  void Push(WN *wn) 
  { 
  	if (orphaned) 
		parent_stack.Push(wn); 
  }
  void Pop() 
  { 
  	if (orphaned) 
		 parent_stack.Pop(); 
  }
};


/********************************************************************/
/*Create local table which will be used for kernel function ST replacement*/
static void 
ACC_Create_Local_VariableTab ( ACC_VAR_TABLE * vtab, ACC_VAR_TYPE vtype, ST* old_st,
						WN_OFFSET old_offset, ST* new_st);

/********************************************************************/
/**replace the symtable with local symbol table in kernel function**/
static WN *
ACC_Walk_and_Localize (WN * tree);


static void 
ACC_Create_Func_DST ( char * st_name )
{
  DST_INFO_IDX	dst = PU_Info_pu_dst( Current_PU_Info );
  DST_INFO	*info = DST_INFO_IDX_TO_PTR(dst);
  DST_ASSOC_INFO *assoc;
  USRCPOS       srcpos;

  USRCPOS_srcpos(srcpos) = acc_line_number;
  acc_nested_dst =  DST_mk_subprogram( srcpos,
			st_name,
			DST_INVALID_IDX,	/* return type */
			DST_INVALID_IDX,	/* for weak symbols */
			ST_st_idx(acc_parallel_proc),
			DW_INL_not_inlined,
			DW_VIRTUALITY_none,
			0,
			FALSE,			/* declaration */
			FALSE,			/* prototype */
#ifdef KEY
                        FALSE,                  // is_artificial
#endif
			FALSE			/* external */
			);
  (void)DST_append_child( dst, acc_nested_dst );
}


/*  Create either a preg or a temp depending on presence of C++ exception
    handling.  */

static void 
ACC_Device_Create_Preg_or_Temp ( TYPE_ID mtype, const char *name, ST **st,
				  WN_OFFSET *ofst )
{
	ST *new_st;
  
	new_st = New_ST (CURRENT_SYMTAB);
	ST_Init (new_st,
	         Save_Str2 ( "__accd_temp_", name ),
	         CLASS_VAR,
	         SCLASS_AUTO,
	         EXPORT_LOCAL,
	         MTYPE_To_TY (mtype));
	Set_ST_is_temp_var ( new_st );
	*st = new_st;
	*ofst = 0;
}

static UINT32 acc_reg_tmp_count = 0; 
static void 
ACC_Host_Create_Preg_or_Temp ( TYPE_ID mtype, const char *name, ST **st)
{
	ST *new_st;
	char szTmp[256];
	sprintf(szTmp, "__acch_temp_%s_%d", name, acc_reg_tmp_count);
    acc_reg_tmp_count++;
	
	new_st = New_ST (CURRENT_SYMTAB);
	ST_Init (new_st,
	         Save_Str (szTmp),
	         CLASS_VAR,
	         SCLASS_AUTO,
	         EXPORT_LOCAL,
	         MTYPE_To_TY (mtype));
	Set_ST_is_temp_var ( new_st );
	*st = new_st;
}


static void
acc_my_Get_Return_Pregs(PREG_NUM *rreg1, PREG_NUM *rreg2, mTYPE_ID type,
                    const char *file, INT line)
{
  if (WHIRL_Return_Info_On) {
    RETURN_INFO return_info = Get_Return_Info(Be_Type_Tbl(type),
                                              Use_Simulated);
    if (RETURN_INFO_count(return_info) <= 2) {
      *rreg1 = RETURN_INFO_preg(return_info, 0);
      *rreg2 = RETURN_INFO_preg(return_info, 1);
    } else
      Fail_FmtAssertion("file %s, line %d: more than 2 return registers",
                        file, line);

  } else
    Get_Return_Pregs(type, MTYPE_UNKNOWN, rreg1, rreg2);

  FmtAssert(*rreg1 != 0 && *rreg2 == 0, ("bad return pregs"));
} // my_Get_Return_Pregs()

#define ACC_GET_RETURN_PREGS(rreg1, rreg2, type) \
  acc_my_Get_Return_Pregs(&rreg1, &rreg2, type, __FILE__, __LINE__)


// Return a non-structure field from offset, if there's multiple field
// with the same offset, return the first.
// This routine can return empty fld handler.
static FLD_HANDLE 
ACC_Get_FLD_From_Offset_r(const TY_IDX ty_idx, const UINT64 offset, UINT* field_id)
{
  Is_True(Is_Structure_Type(ty_idx), ("need to be a structure type"));

  UINT64 cur_offset = 0;

  FLD_ITER fld_iter = Make_fld_iter(TY_fld(ty_idx));
  do {
    if ( field_id != NULL )
      (*field_id) ++;
    FLD_HANDLE fld(fld_iter);       

    // we assume that we will not see bit-fields here.
    cur_offset = FLD_ofst(fld);

    if (cur_offset == offset)
    {
      // check type
      TY_IDX cur_fld_idx = FLD_type(fld);
      if (!Is_Structure_Type(cur_fld_idx))
        return fld;
    }

    TY_IDX cur_fld_idx = FLD_type(fld);
    if (TY_kind(cur_fld_idx) == KIND_STRUCT &&
        TY_fld(cur_fld_idx) != FLD_HANDLE())
    {
      // it's possible that the new_offset becomes negative
      // because of unions. 
      INT64 new_offset = offset - cur_offset;
      if (new_offset < 0) 
	  	return FLD_HANDLE();
      FLD_HANDLE fld1 = ACC_Get_FLD_From_Offset_r(cur_fld_idx, new_offset, field_id);
      if (!fld1.Is_Null()) return fld1;
    }

  } while (!FLD_last_field(fld_iter++));

  return FLD_HANDLE();
}

// Return a non-structure field from offset, if there's multiple field
// with the same offset, return the first.
// This routine will assert if it cannot find a valid field.
static FLD_HANDLE 
ACC_Get_FLD_From_Offset(const TY_IDX ty_idx, const UINT64 offset, UINT *field_id= NULL)
{
  if (field_id != NULL)
	  *field_id= 0;
  FLD_HANDLE fld = ACC_Get_FLD_From_Offset_r(ty_idx, offset, field_id);
  FmtAssert(!fld.Is_Null(),("cannot find field from offset"));
  return fld;
}

static void
Gen_ACC_LS_get_fld_id_and_ty(ST *st, WN_OFFSET offset, BOOL scalar_only, UINT &field_id, TY_IDX &ty, TY_IDX &result_ty)
{
  ty = ST_type(st);
  result_ty = ty;
#ifdef KEY // bug 7259
  if (scalar_only && TY_kind(ty) == KIND_STRUCT )
  {
    FLD_HANDLE fld = ACC_Get_FLD_From_Offset(ty, offset, &field_id);
    result_ty = FLD_type(fld);
  }
#endif
#ifdef KEY // bug 10681
  if (scalar_only && TY_kind(ty) == KIND_ARRAY)
    ty = TY_etype(ty);
#endif
  return; 
}
/*  Generate an appropriate load WN based on an ST.  */

static WN *
Gen_ACC_Load( ST * st, WN_OFFSET offset, BOOL scalar_only )
{
  UINT field_id = 0;
  WN *wn;
  TY_IDX ty;
  TY_IDX result_ty;
  
  Gen_ACC_LS_get_fld_id_and_ty(st, offset, scalar_only, field_id, ty, result_ty);

  wn = WN_Ldid ( TY_mtype(result_ty), offset, st, ty ,field_id);

  return (wn);
}

/*  Generate an appropriate store WN based on an ST.  */

static WN *
Gen_ACC_Store( ST * st, WN_OFFSET offset, WN * value, BOOL scalar_only)
{
  UINT  field_id = 0;
  WN *wn;
  TY_IDX ty;
  TY_IDX result_ty;

  Gen_ACC_LS_get_fld_id_and_ty(st, offset, scalar_only, field_id, ty, result_ty);
  
  wn = WN_Stid ( TY_mtype(result_ty), offset, st, ty, value, field_id );
  WN_linenum(wn) = acc_line_number;

  return (wn);
}


/*******************************************************************/

/* I move the content of wn_mp_dg.cxx into here.
*  For Copy_Non_MP_Tree need some stuff of this file.
*  csc.
*/

typedef  HASH_TABLE<VINDEX16,VINDEX16> VV_HASH_TABLE;
typedef STACK<VINDEX16> V_STACK;
static MEM_POOL ACC_Dep_Pool;
//static data type is false in default, here, I just want to make it readable
static BOOL acc_dep_pool_initialized = false; 
void ACC_Create_Vertices(WN *wn, VV_HASH_TABLE *parent_to_child,
                V_STACK *parent_vertices,
                ARRAY_DIRECTED_GRAPH16 *parent_graph,
                ARRAY_DIRECTED_GRAPH16 *child_graph);

// Fix up the array dependence graph during MP lowering
void 
ACC_Fix_Dependence_Graph(PU_Info *parent_pu_info,
                                PU_Info *child_pu_info, WN *child_wn)
{
  ARRAY_DIRECTED_GRAPH16 *parent_graph =
     (ARRAY_DIRECTED_GRAPH16 *) PU_Info_depgraph_ptr(parent_pu_info);
  if (!parent_graph) { // no parent, no child
    Set_PU_Info_depgraph_ptr(child_pu_info,NULL);
    return;
  }
  if (!acc_dep_pool_initialized) {
    MEM_POOL_Initialize(&ACC_Dep_Pool,"MP_Dep_Pool",FALSE);
    acc_dep_pool_initialized = TRUE;
  }
  MEM_POOL_Push(&ACC_Dep_Pool);

  // Create a new dependence graph for the child region
  ARRAY_DIRECTED_GRAPH16 *child_graph  =
            CXX_NEW(ARRAY_DIRECTED_GRAPH16(100, 500,
                WN_MAP_DEPGRAPH, DEP_ARRAY_GRAPH), Malloc_Mem_Pool);
  Set_PU_Info_depgraph_ptr(child_pu_info,child_graph);
  Set_PU_Info_state(child_pu_info,WT_DEPGRAPH,Subsect_InMem);

  // mapping from the vertices in the parent graph to the corresponding
  // ones in the child
  VV_HASH_TABLE *parent_to_child =
    CXX_NEW(VV_HASH_TABLE(200,&ACC_Dep_Pool),&ACC_Dep_Pool);
  // a list of the parent vertices in the region
  V_STACK *parent_vertices = CXX_NEW(V_STACK(&ACC_Dep_Pool),&ACC_Dep_Pool);
  ACC_Create_Vertices(child_wn,parent_to_child,parent_vertices,parent_graph,
                                                        child_graph);

  // copy the edges, erase them from the parent graph
  INT i;
  for (i=0; i<parent_vertices->Elements(); i++) {
    VINDEX16 parent_v = parent_vertices->Bottom_nth(i);
    VINDEX16 child_v = parent_to_child->Find(parent_v);
    Is_True(child_v,("child_v missing "));
    EINDEX16 e;
    while (e = parent_graph->Get_Out_Edge(parent_v)) {
      VINDEX16 parent_sink = parent_graph->Get_Sink(e);
      VINDEX16 child_sink = parent_to_child->Find(parent_sink);
      Is_True(child_sink,("child_sink missing "));
      child_graph->Add_Edge(child_v,child_sink,
                parent_graph->Dep(e),parent_graph->Is_Must(e));

      parent_graph->Remove_Edge(e);
    }
  }
  for (i=0; i<parent_vertices->Elements(); i++) {
    // remove the vertex from the parent graph
    // since removing the vertex cleans the wn map, reset it
    VINDEX16 parent_v = parent_vertices->Bottom_nth(i);
    VINDEX16 child_v = parent_to_child->Find(parent_v);
    WN *wn = parent_graph->Get_Wn(parent_v);
    parent_graph->Delete_Vertex(parent_v);
    child_graph->Set_Wn(child_v,wn);
  }
  CXX_DELETE(parent_to_child,&ACC_Dep_Pool);
  CXX_DELETE(parent_vertices,&ACC_Dep_Pool);
  MEM_POOL_Pop(&ACC_Dep_Pool);
}

// walk the child, find all the vertices, create corresponding vertices
// in the child graph, fill up the hash table and stack
void 
ACC_Create_Vertices(WN *wn, VV_HASH_TABLE *parent_to_child,
                V_STACK *parent_vertices,
                ARRAY_DIRECTED_GRAPH16 *parent_graph,
                ARRAY_DIRECTED_GRAPH16 *child_graph)
{
  OPCODE opcode = WN_opcode(wn);
  if (opcode == OPC_BLOCK) 
 {
    WN *kid = WN_first (wn);
    while (kid) 
	{
      ACC_Create_Vertices(kid,parent_to_child,parent_vertices,parent_graph,
                                                        child_graph);
      kid = WN_next(kid);
    }
    return;
  }
  if (OPCODE_is_load(opcode) || OPCODE_is_store(opcode)
      || OPCODE_is_call(opcode)) 
 {
    VINDEX16 parent_v = parent_graph->Get_Vertex(wn);
    if (parent_v) 
	{
      // can't overflow since parent graph has
      // at least the same number of vertices
      VINDEX16 child_v = child_graph->Add_Vertex(wn);
      parent_to_child->Enter(parent_v,child_v);
      parent_vertices->Push(parent_v);
    }
  }
  for (INT kidno=0; kidno<WN_kid_count(wn); kidno++) {
    ACC_Create_Vertices(WN_kid(wn,kidno),parent_to_child,parent_vertices,
                                        parent_graph,child_graph);
  }
}


static BOOL 
ACC_Identical_Pragmas ( WN * wn1, WN * wn2 );

/*
Transfer all maps (except WN_MAP_FEEDBACK) associated with each node in the
tree from the parent mapset to the kid's.
*/

static void
ACC_Transfer_Maps_R ( WN_MAP_TAB * parent, WN_MAP_TAB * child, WN * tree,
                  RID * root_rid );

static void
ACC_Transfer_Maps ( WN_MAP_TAB * parent, WN_MAP_TAB * child, WN * tree,
                RID * root_rid )
{
    // to preserve WN_MAP_FEEDBACK in child map table, copy its contents
    // to fb_map
  HASH_TABLE<WN *, INT32> fb_map(ACC_NUM_HASH_ELEMENTS, Malloc_Mem_Pool);
  WN_ITER *wni = WN_WALK_TreeIter(tree);

  for ( ; wni; wni = WN_WALK_TreeNext(wni)) {
    WN *wn = WN_ITER_wn(wni);

    fb_map.Enter(wn, IPA_WN_MAP32_Get(child, WN_MAP_FEEDBACK, wn));
  }

  ACC_Transfer_Maps_R(parent, child, tree, root_rid); // overwrites WN_MAP_FEEDBACK

    // now restore values for WN_MAP_FEEDBACK from fb_map
  HASH_TABLE_ITER<WN *, INT32> fb_map_iter(&fb_map);
  WN *wn;
  INT32 val;

  while (fb_map_iter.Step(&wn, &val))
    IPA_WN_MAP32_Set(child, WN_MAP_FEEDBACK, wn, val);

//  parent->_is_used[WN_MAP_FEEDBACK] = is_used;  // restore the flag
} // Transfer_Maps

// this function does the real work
static void
ACC_Transfer_Maps_R ( WN_MAP_TAB * parent, WN_MAP_TAB * child, WN * tree,
                  RID * root_rid )
{
  WN *node;
  INT32 i;

  if (tree) {
    if (WN_opcode(tree) == OPC_BLOCK) {
      for (node = WN_first(tree); node; node = WN_next(node))
	ACC_Transfer_Maps_R ( parent, child, node, root_rid );
    } else
      for (i = 0; i < WN_kid_count(tree); i++)
	ACC_Transfer_Maps_R ( parent, child, WN_kid(tree, i), root_rid );

    if (WN_map_id(tree) != -1) {
      RID *rid = REGION_get_rid ( tree );
      IPA_WN_Move_Maps_PU ( parent, child, tree );
      if (WN_opcode(tree) == OPC_REGION) {
	Is_True(root_rid != NULL, ("Transfer_Maps_R, NULL root RID"));
	RID_unlink ( rid );
	RID_Add_kid ( rid, root_rid );
      } 
    }
  }
} // Transfer_Maps_R


ST_IDX Make_ACCRuntime_ST ( OACCRUNTIME rop )
{
  Is_True(rop >= ACCRUNTIME_FIRST && rop <= ACCRUNTIME_LAST,
          ("Make_ACCRuntime_ST: bad rop == %d", (INT) rop));

    // If the global type doesn't exist, create it and its pointer type.
  if (accruntime_ty == TY_IDX_ZERO) {
    TY &mpr_ty = New_TY ( accruntime_ty );
    TY_Init(mpr_ty, 0, KIND_FUNCTION, MTYPE_UNKNOWN,
            Save_Str(".accruntime"));
    Set_TY_align(accruntime_ty, 1);

    TYLIST_IDX parm_idx;
    TYLIST& parm_list = New_TYLIST(parm_idx);
    Set_TY_tylist(mpr_ty, parm_idx);
    Set_TYLIST_type(parm_list, Be_Type_Tbl(MTYPE_I4));  // I4 return type
      // are there really no parameters? -- DRK
    Set_TYLIST_type(New_TYLIST(parm_idx), TY_IDX_ZERO); // end of parm list

    TY_IDX ty_idx;
    TY &ty = New_TY ( ty_idx );
    TY_Init(ty, Pointer_Size, KIND_POINTER, Pointer_Mtype,
      Save_Str ( ".accruntime_ptr" ));
    Set_TY_pointed(ty, accruntime_ty);

    Set_TY_align(ty_idx, Pointer_Size); // unnecessary? TY_Init does
                                        // not set alignment -- DRK
  }

  PU_IDX pu_idx;
  PU& pu = New_PU(pu_idx);
  PU_Init(pu, accruntime_ty, CURRENT_SYMTAB);

  /*  Create the ST, fill in all appropriate fields and enter into the */
  /*  global symbol table.  */

  ST *st = New_ST ( GLOBAL_SYMTAB );
  ST_Init(st, Save_Str ( accr_names[rop] ), CLASS_FUNC, SCLASS_EXTERN,
    EXPORT_PREEMPTIBLE, pu_idx);

  Allocate_Object ( st );

  accr_sts[rop] = ST_st_idx(*st);
  return accr_sts[rop];
}

inline WN_OFFSET WN_offsetx ( WN *wn )
{
  OPERATOR opr;
  opr = WN_operator(wn);
  if ((opr == OPR_PRAGMA) || (opr == OPR_XPRAGMA)) {
    return (WN_pragma_arg1(wn));
  } else {
    return (WN_offset(wn));
  }
}

//WN node must have a kid which includes buffer region
//wnArr is a pragma wn node which includes variable declaration
static WN* ACC_GetArraySizeInUnit(ACC_DREGION__ENTRY dEntry)
{
	WN* wnArr = dEntry.acc_data_clauses;
	WN* wnLength = dEntry.acc_data_length;
	
	ST* stArr = WN_st(wnArr);
	TY_IDX ty = ST_type(stArr);
	TY_KIND kind = TY_kind(ty);
	
	WN* wnLower = WN_kid0(wnArr);
	OPCODE oplower = WN_opcode(wnLower);
	WN* wnUpper = wnLength;
	OPCODE opupper = WN_opcode(wnUpper);
	//Two cases: array with no region limite which mean the entire array; buffer with region declaration
	if((oplower==OPC_I4INTCONST) && (oplower==opupper)
			&& (WN_const_val(wnLower) == 0)
			&& (WN_const_val(wnUpper) == 0))
	{
		//////////////////////////////////////////////////////////////////////
		//Array		
		Is_True(kind == KIND_ARRAY, ("not array type in ACC_GetArraySize."));
		UINT32 isize = TY_size(ty);
		return WN_Intconst(MTYPE_I4, isize);
	}
	else
	{
		WN* wnBoundary = WN_COPY_Tree(wnUpper);
		return wnBoundary;
	}
	return NULL;
}

static int ACC_Get_Array_TotalDim(WN* wnArr)
{	
	ST* stArr = WN_st(wnArr);
	TY_IDX ty = ST_type(stArr);
	TY_KIND kind = TY_kind(ty);
	int idim = 1;
	if(TY_kind(ty) == KIND_POINTER)
		return 1;
	if(TY_kind(TY_etype(ty)) == KIND_SCALAR)
		return 1;
	else if(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
	{
		//multi-dimensional array
		//ty = TY_etype(ty);
		while(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
		{
			ty = TY_etype(ty);
			idim ++;
		}
	}
	return idim;
}

static UINT32 ACC_Get_ElementSizeForMultiArray(WN* wnArr)
{
	ST* stArr = WN_st(wnArr);
	TY_IDX ty = ST_type(stArr);
	TY_KIND kind = TY_kind(ty);
	int idim = 1;
	if(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
	{
		//multi-dimensional array
		//ty = TY_etype(ty);
		while(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
		{
			ty = TY_etype(ty);
			idim ++;
		}
	}
	
	//if(TY_kind(TY_etype(ty)) == KIND_SCALAR)
	return TY_size(TY_etype(ty));
	//return idim;
}


static TY_IDX ACC_Get_ElementTYForMultiArray(ST* stArr)
{
	//ST* stArr = WN_st(wnArr);
	TY_IDX ty = ST_type(stArr);
	TY_KIND kind = TY_kind(ty);
	int idim = 1;
	if(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
	{
		//multi-dimensional array
		//ty = TY_etype(ty);
		while(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
		{
			ty = TY_etype(ty);
			idim ++;
		}
	}
	
	//if(TY_kind(TY_etype(ty)) == KIND_SCALAR)
	return TY_etype(ty);
	//return idim;
}


static WN* ACC_Get_Specified_Dim(WN* wnArr, int n)
{

	ST* stArr = WN_st(wnArr);
	TY_IDX ty = ST_type(stArr);
	TY_KIND kind = TY_kind(ty);
	int idim = 1;
	int upper_bound;
	int lower_bound;
	
	if(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY && n > 0)
	{
		//multi-dimensional array
		//ty = TY_etype(ty);
		while(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
		{
			if(idim == n)
			{
				if(ARB_const_stride(TY_arb(ty))
						&& ARB_const_lbnd(TY_arb(ty)) && ARB_const_ubnd(TY_arb(ty)))
				{			
					upper_bound = ARB_ubnd_val(TY_arb(ty));
					lower_bound = ARB_lbnd_val(TY_arb(ty));
					break;
				}
				else
					Fail_FmtAssertion(("%s is not a static array in ACC_Get_Specified_Dim."), ST_name(stArr));
			}
			ty = TY_etype(ty);
			idim ++;
		}
	}
	
	if(TY_kind(ty) == KIND_ARRAY)
	{
		if(ARB_const_stride(TY_arb(ty))
				&& ARB_const_lbnd(TY_arb(ty)) && ARB_const_ubnd(TY_arb(ty)))
		{			
			upper_bound = ARB_ubnd_val(TY_arb(ty));
			lower_bound = ARB_lbnd_val(TY_arb(ty));
		}
		else
			Fail_FmtAssertion(("%s is not a static array in ACC_Get_Specified_Dim."), ST_name(stArr));
	}
	
	WN* wn_dim = WN_Intconst(MTYPE_U4, (upper_bound - lower_bound + 1));
	return wn_dim;
}


static WN* ACC_Load_MultiDimArray_StartAddr(WN* wnArr)
{
	ST* base = WN_st(wnArr);
	int idim = ACC_Get_Array_TotalDim(wnArr);
	WN *arr_ref = WN_Create( OPCODE_make_op(OPR_ARRAY, Pointer_Mtype,MTYPE_V), 1+2*idim);
    WN_element_size(arr_ref) = ACC_Get_ElementSizeForMultiArray(wnArr);
    WN_array_base(arr_ref) = WN_Lda(Pointer_type, 0, base);

				          
	for( int j=0; j<idim; j++ )
	{
		 //assume the index type to be I8. also assume the dim-size I8 type.
		 //TODO: make it more adaptive. csc
		 WN_array_index( arr_ref, j ) = WN_Intconst(MTYPE_U4, 0);
		 WN_array_dim( arr_ref, j ) = ACC_Get_Specified_Dim(wnArr, j);;
	}

    //WN_array_index(arr_ref,0) = WN_Intconst(MTYPE_U4, 0);
    //WN_array_dim(arr_ref,0) = WN_Intconst(MTYPE_U4, idim);
    return arr_ref;
}


//WN node must have a kid which includes buffer region
//wnArr is a pragma wn node which includes variable declaration
//return the offset of the array buffer. It is in byte.
static WN* ACC_GetArrayStart(ACC_DREGION__ENTRY dEntry)
{
	WN* wnArr = dEntry.acc_data_clauses;
	WN* wnLength = dEntry.acc_data_length;
	
	ST* stArr = WN_st(wnArr);
	TY_IDX ty = ST_type(stArr);
	TY_KIND kind = TY_kind(ty);
	
	WN* wnLower = WN_kid0(wnArr);
	OPCODE oplower = WN_opcode(wnLower);
	WN* wnUpper = wnLength;
	OPCODE opupper = WN_opcode(wnUpper);
	//Two cases: array with no region limite which mean the entire array; buffer with region declaration
	if((oplower==OPC_I4INTCONST)
			&& (WN_const_val(wnLower) == 0))
	{
		//////////////////////////////////////////////////////////////////////
		//Array		
		//Is_True(kind == KIND_ARRAY, ("not array type in ACC_GetArrayStart."));
		//UINT32 isize = TY_size(ty);
		return WN_Intconst(MTYPE_U4, 0);
	}
	else
	{
		WN* wnStart;
		WN* wnElementsize;
		if(kind == KIND_ARRAY)
		{
			TY_IDX tyElement = TY_etype(ty);
			INT32 elemSize = TY_size(tyElement);
			wnElementsize = WN_Intconst(MTYPE_U4, elemSize);
		}
		else if(kind == KIND_POINTER)
		{
			TY_IDX tyPointed = TY_pointed(ty);
			INT32 elemSize = TY_size(tyPointed);
			wnElementsize = WN_Intconst(MTYPE_U4, elemSize);
		}
		wnStart = WN_Binary(OPR_MPY, MTYPE_U4, 
							wnElementsize, WN_COPY_Tree(wnLower));
		return wnStart;
	}
	return NULL;
}


//WN node must have a kid which includes buffer region
//return the size of the array buffer. It is in byte.
static WN* ACC_GetArraySize(ACC_DREGION__ENTRY dEntry)
{
	WN* wnArr = dEntry.acc_data_clauses;
	WN* wnLength = dEntry.acc_data_length;
	
	ST* stArr = WN_st(wnArr);
	TY_IDX ty = ST_type(stArr);
	TY_KIND kind = TY_kind(ty);
	
	WN* wnLower = WN_kid0(wnArr);
	OPCODE oplower = WN_opcode(wnLower);
	WN* wnUpper = wnLength;
	OPCODE opupper = WN_opcode(wnUpper);
	//Two cases: array with no region limite which mean the entire array; buffer with region declaration
	if((oplower==OPC_I4INTCONST) && (oplower==opupper)
			&& (WN_const_val(wnLower) == 0)
			&& (WN_const_val(wnUpper) == 0))
	{
		//////////////////////////////////////////////////////////////////////
		//Array		
		Is_True(kind == KIND_ARRAY, ("not array type in ACC_GetArraySize."));
		//process the multi dimensional array
		UINT32 isize;
		if(TY_kind(TY_etype(ty)) != KIND_SCALAR 
				&& TY_kind(TY_etype(ty)) == KIND_ARRAY)
		{
			if(ARB_const_stride(TY_arb(ty))
				&& ARB_const_lbnd(TY_arb(ty)) && ARB_const_ubnd(TY_arb(ty)))
			{
				isize = ARB_stride_val(TY_arb(ty));
				int upper_bound = ARB_ubnd_val(TY_arb(ty));
				int lower_bound = ARB_lbnd_val(TY_arb(ty));
				isize = isize * (upper_bound - lower_bound + 1);
			}
			else
				Fail_FmtAssertion(("%s is not a static array in ACC_GetArraySize."), ST_name(stArr));
					
		}
		else
			isize = TY_size(ty);
		return WN_Intconst(MTYPE_U4, isize);
	}
	else
	{
		WN* wnBoundary = WN_COPY_Tree(wnUpper);
		WN* wnWholeSize;
		WN* wnElementsize;
		if(kind == KIND_ARRAY)
		{
			TY_IDX tyElement = TY_etype(ty);
			INT32 elemSize = TY_size(tyElement);
			wnElementsize = WN_Intconst(MTYPE_U4, elemSize);
		}
		else if(kind == KIND_POINTER)
		{
			TY_IDX tyPointed = TY_pointed(ty);
			INT32 elemSize = TY_size(tyPointed);
			wnElementsize = WN_Intconst(MTYPE_U4, elemSize);
		}
		wnWholeSize = WN_Binary(OPR_MPY, MTYPE_U4, 
							wnElementsize, wnBoundary);
		return wnWholeSize;
	}
	return NULL;
}


/*device Malloc memory*/
static WN *
Gen_DeviceMalloc( ST* st_hmem, ST *st_dmem, WN* wnSize) 
{
	WN * wn;
	WN* wnx;
	wn = WN_Create(OPC_VCALL, 3);	
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_DEVICEMEMMALLOC);
  
	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);

	
	if(TY_kind(ST_type(st_hmem)) == KIND_ARRAY)
		wnx = WN_Lda( Pointer_type, 0, st_hmem);
	else
  		wnx = WN_Ldid(Pointer_type, 0, st_hmem, ST_type(st_hmem));	
    WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);
	
	wnx = WN_Lda( Pointer_type, 0, st_dmem);
    WN_kid(wn, 1) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_REFERENCE);
  	//
	WN_kid(wn, 2) = WN_CreateParm(MTYPE_U4, wnSize, 
		  Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
	
  	return wn;
}


static WN *
Gen_DataD2H (ST *Src, ST *Dst, WN* wnSize, WN* wnStart) 
{
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 5 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_DEVICEMEMOUT);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  wnx = WN_Ldid(Pointer_type, 0, Src, ST_type(Src));


  //WN* multiArrayT; //

  //wnx = WN_Lda( Pointer_type, 0, Src);
  WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);

  //if the host is multi dim array, it will be different  
	if(TY_kind(ST_type(Dst)) == KIND_ARRAY)
		wnx = WN_Lda( Pointer_type, 0, Dst);
	else
  		wnx = WN_Ldid(Pointer_type, 0, Dst, ST_type(Dst));
  //if(ACC_Get_Array_TotalDim(wnx) > 1)
  //	wnx = ACC_Load_MultiDimArray_StartAddr(wnx);
  //wnx = WN_Lda( Pointer_type, 0, Dst);
  WN_kid(wn, 1) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);
  
  WN_kid(wn, 2) = WN_CreateParm(MTYPE_U4, wnSize, 
		  Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
  
  WN_kid(wn, 3) = WN_CreateParm(MTYPE_U4, wnStart, 
		  Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
  
  if(acc_AsyncExpr)
  {
  	WN_kid(wn, 4) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(acc_AsyncExpr), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  }
  else 
  	WN_kid(wn, 4) = WN_CreateParm(MTYPE_I4, WN_Intconst(MTYPE_I4, -2), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

static WN *
Gen_DataH2D (ST *Src, ST *Dst, WN* wnSize, WN* wnStart) 
{
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 5);
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_DEVICEMEMIN);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  
  if(TY_kind(ST_type(Src)) == KIND_ARRAY)
	  wnx = WN_Lda( Pointer_type, 0, Src);
  else
  	  wnx = WN_Ldid(Pointer_type, 0, Src, ST_type(Src));
  //WN* multiArrayT;
  //if(ACC_Get_Array_TotalDim(wnx) > 1)
  //	multiArrayT = ACC_Load_MultiDimArray_StartAddr(wnx);
  //WN* multiArrayT = ACC_Load_MultiDimArray_StartAddr(wnx);
  //wnx = WN_Lda( Pointer_type, 0, Src);
  WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);

  
  wnx = WN_Ldid(Pointer_type, 0, Dst, ST_type(Dst));

  //wnx = WN_Lda( Pointer_type, 0, Dst);
  WN_kid(wn, 1) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);
  
  WN_kid(wn, 2) = WN_CreateParm(MTYPE_U4, wnSize, 
		  Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
  
  WN_kid(wn, 3) = WN_CreateParm(MTYPE_U4, wnStart, 
  		  Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
  
  if(acc_AsyncExpr)
  {
  	WN_kid(wn, 4) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(acc_AsyncExpr), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  }
  else 
  	WN_kid(wn, 4) = WN_CreateParm(MTYPE_I4, WN_Intconst(MTYPE_I4, -2), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

/*
Create a DST entry for a local variable in either the parent subprogram or
the nested subprogram.
*/

static void 
ACC_Add_DST_variable ( ST *st, DST_INFO_IDX parent_dst, 
			       INT64 line_number, DST_INFO_IDX type_idx )
{
  DST_INFO      *info;
  DST_INFO_IDX  dst, child_idx;
  DST_ATTR_IDX attr_idx;
  DST_ASSOC_INFO *assoc;
  DST_BASETYPE *attr;
  USRCPOS       srcpos;
  INT32         typesize;
  static DST_INFO_IDX int32_idx = {DST_INVALID_BLOCK_IDX, DST_INVALID_BYTE_IDX};
  static DST_INFO_IDX int64_idx = {DST_INVALID_BLOCK_IDX, DST_INVALID_BYTE_IDX};
  DST_INFO_IDX  int_idx;
  DST_IDX	cmp;
  char         *name;

  /* don't do anything if without -g option */
  if (Debug_Level == 0)
    return;

  if (DST_IS_NULL( type_idx )) {
    /* For variables which do not exist in the original program, there
    ** are no type information for them.  We need to search in the DST
    ** tree for the corresponding type entry
    */
    if (TY_kind(ST_type(st)) == KIND_POINTER) {
      typesize = TY_size(TY_pointed(ST_type(st)));
    } else {
      typesize = TY_size(ST_type(st));
    }

    const char *int_name1, *int_name2, *int_name3;
    DST_INFO_IDX *int_idx_p;

    switch (typesize) {
    case 4:
      int_name1 = "int"; int_name2 = "INTEGER*4"; int_name3 = "INTEGER_4";
      int_idx_p = &int32_idx;
      break;
    case 8:
#ifndef KEY
      int_name1 = "long long"; 
#else
      // Bug 7287 - C and C++ programs have "long long int" and 
      // "long long unsigned int" as basetype, but no "long long".
      int_name1 = "long long int";
#endif
      int_name2 = "INTEGER*8"; int_name3 = "INTEGER_8";
      int_idx_p = &int64_idx;
      break;
    default:
      Fail_FmtAssertion("can't handle typesize == %d", (INT) typesize);
    }

    if (DST_IS_NULL(*int_idx_p) ) {
      cmp = DST_get_compile_unit();
      info = DST_INFO_IDX_TO_PTR( cmp );
      attr_idx = DST_INFO_attributes( info );
      child_idx = DST_COMPILE_UNIT_first_child(
                  DST_ATTR_IDX_TO_PTR(attr_idx, DST_COMPILE_UNIT));
      while (!DST_IS_NULL(child_idx)) {
        info = DST_INFO_IDX_TO_PTR( child_idx );
        if (DST_INFO_tag( info ) == DW_TAG_base_type) {
          attr = DST_ATTR_IDX_TO_PTR(DST_INFO_attributes(info), DST_BASETYPE);
          name = DST_STR_IDX_TO_PTR( DST_FORMAL_PARAMETER_name(attr));
#ifdef KEY //bug 11848: name can not be null
         if(name == NULL)
            Is_True(0, ("Base type should have a name in a DST entry"));
          else
#endif
          if (!strcmp(name, int_name1) || !strcmp(name, int_name2) ||
              !strcmp(name, int_name3)) {
            *int_idx_p = child_idx;
            break;
          }
        }
        child_idx = DST_INFO_sibling(DST_INFO_IDX_TO_PTR(child_idx));
      }
      if (DST_IS_NULL(child_idx)) {
          // type not emitted by frontend, so we have to insert it
        *int_idx_p = DST_mk_basetype(int_name1, DW_ATE_signed, typesize);
        (void) DST_append_child(parent_dst, *int_idx_p);
      }
    }
    int_idx = *int_idx_p;

  }

  USRCPOS_srcpos(srcpos) = line_number;
  if (ST_sclass(st) == SCLASS_FORMAL_REF) {
    dst = DST_mk_formal_parameter( srcpos,
			ST_name( st ),
			int_idx,	/* type DST_IDX */
			ST_st_idx(st),	/* symbol */
			DST_INVALID_IDX,
			DST_INVALID_IDX,
			FALSE,
			FALSE,
			FALSE,   // is_artificial
			FALSE ); // is_declaration_only
    DST_SET_deref( DST_INFO_flag( DST_INFO_IDX_TO_PTR (dst) ) );
  } else
    dst = DST_mk_variable( srcpos,
			ST_name( st ),
			int_idx, 	/* type DST_IDX */
			0 /* offset */,
			ST_st_idx(st),	/* symbol */
			DST_INVALID_IDX,
			FALSE,		/* memory allocated */
			TRUE, 		/* parameter has sc_auto */
			FALSE,		/* sc_extern || sc_unspecified */
			FALSE);		/* is_artificial */

  (void)DST_append_child( parent_dst, dst );
}

/*****************************************/
static void Push_Kernel_Parameters(WN* wn_replaceBlock, BOOL bParallel)
{
	WN* wnx, *wn;
	int parm_id = 0;
	int i=0;
	
	
	//WN_Set_Call_IS_KERNEL_LAUNCH(wn);
	//first copyin, then copyout, then copy, then param pragma
	//
	for(i=0; i<acc_kernelLaunchParamList.size(); i++)
	{
		ST* hostST = acc_kernelLaunchParamList[i].st_host;
		//in case of deviceptr, the host st is NULL.
		hostST = hostST ? hostST : acc_kernelLaunchParamList[i].st_device;
		TY_IDX ty = ST_type(hostST);
		TY_KIND kind = TY_kind(ty);//ST_name(old_st)
		TYPE_ID typeID;
		if(kind == KIND_POINTER || kind == KIND_ARRAY)
		{		
			INT64 acc_dtype = 
					GetKernelParamType(acc_kernelLaunchParamList[i].st_device);
			wn = WN_Create(OPC_VCALL, 1);
			WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_KERNELPARAMPUSH_POINTER);
			
			WN_Set_Call_Non_Data_Mod(wn);
			WN_Set_Call_Non_Data_Ref(wn);
			WN_Set_Call_Non_Parm_Mod(wn);
			WN_Set_Call_Non_Parm_Ref(wn);
			WN_Set_Call_Parm_Ref(wn);
			WN_linenum(wn) = acc_line_number;
			
	  		//wnx = WN_Ldid(Pointer_type, 0, acc_kernelLaunchParamList[i].st_device, 
			//					ST_type(acc_kernelLaunchParamList[i].st_device));
			
			wnx = WN_Lda( Pointer_type, 0, acc_kernelLaunchParamList[i].st_device);
			//First, pointer
		    WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
		                       WN_ty(wnx), WN_PARM_BY_REFERENCE);
			
			//Attach two the replacement block
			WN_INSERT_BlockLast(wn_replaceBlock, wn);
		}
		else if(kind == KIND_SCALAR)
		{
			ST* old_st = acc_kernelLaunchParamList[i].st_host;
			map<ST*, ACC_ReductionMap>::iterator itor = acc_reduction_tab_map.find(old_st);	
			//find the symbol in the reduction, then ignore it in the parameters.
			if(itor != acc_reduction_tab_map.end())
				continue;
			//////////////////////////////////////////////////
			wn = WN_Create(OPC_VCALL, 1);
			WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_KERNELPARAMPUSH_SCALAR);
			
			WN_Set_Call_Non_Data_Mod(wn);
			WN_Set_Call_Non_Data_Ref(wn);
			WN_Set_Call_Non_Parm_Mod(wn);
			WN_Set_Call_Non_Parm_Ref(wn);
			WN_Set_Call_Parm_Ref(wn);
			WN_linenum(wn) = acc_line_number;
			
	  		//wnx = WN_Ldid(Pointer_type, 0, acc_kernelLaunchParamList[i].st_device, 
			//					ST_type(acc_kernelLaunchParamList[i].st_device));
			
			wnx = WN_Lda( Pointer_type, 0, old_st);
			//First, pointer
		    WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
		                       WN_ty(wnx), WN_PARM_BY_REFERENCE);
			
			//Attach two the replacement block
			WN_INSERT_BlockLast(wn_replaceBlock, wn);
		}
		else
		{
			Is_True(FALSE, ("Wrong Kernel Parameter Kind Type. in Push_Kernel_Parameters 1"));
		}
	}
	//launch the reduction parameters
	i=0;
	//Only parallel region doing the reduction this way
	for(i=0; (bParallel && (i<acc_additionalKernelLaunchParamList.size())); i++)
	{
		KernelParameter parmList = acc_additionalKernelLaunchParamList[i];
		////////////////////////////////////////////////////////////////////////////
		ST* st_device = parmList.st_device;
		TY_IDX ty = ST_type(st_device);
		TY_KIND kind = TY_kind(ty);//ST_name(old_st)
		TYPE_ID typeID;
		if(kind == KIND_POINTER || kind == KIND_ARRAY)
		{
			wn = WN_Create(OPC_VCALL, 1 );
			WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_KERNELPARAMPUSH_POINTER);
			
			WN_Set_Call_Non_Data_Mod(wn);
			WN_Set_Call_Non_Data_Ref(wn);
			WN_Set_Call_Non_Parm_Mod(wn);
			WN_Set_Call_Non_Parm_Ref(wn);
			WN_Set_Call_Parm_Ref(wn);
			WN_linenum(wn) = acc_line_number;
			
	  		wnx = WN_Lda(Pointer_type, 0, st_device);
			//First, pointer
			WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
							 		WN_ty(wnx), WN_PARM_BY_VALUE);
			//Attach two the replacement block
			WN_INSERT_BlockLast(wn_replaceBlock, wn);
		}
		else
		{
			Is_True(FALSE, ("Wrong Kernel Parameter Kind Type in Push_Kernel_Parameters 2."));
		}
	}
}


/*****************************************/
//This function is only called by ACC_Create_MicroTask function.
static void Create_kernel_parameters_ST(WN* kparamlist, BOOL isParallel)
{	
	kernel_param.clear();
	WN* wn;
	
	//kernel_param is used for kernel function parameter generation
	//The sequence of parameters must be exactly the same 
	//as the sequence parameters pushed into kernel launch stack
	//for (wn = kparamlist; wn; wn = WN_next(wn))
	//{
	//	kernel_parameters_count ++;
	//}.
	//this var may be deleted later.it is useless.
	//ACC_VAR_TYPE vtype = ACC_VAR_COPYIN;	
	WN        *l;
	/* Do locals */
	for (l = kparamlist; l; l = WN_next(l)) 
	{
		ST *old_st = WN_st(l);
		WN_OFFSET old_offset = WN_offsetx(l);		
	    TY_IDX ty = ST_type(old_st);
	    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
	    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
		ST *new_st;
		sprintf ( localname, "__d_%s", ST_name(old_st) );
		if (kind == KIND_POINTER)
		{
			TY_IDX pty = TY_pointed(ty);
			TY_IDX ty_p = Make_Pointer_Type(pty);
			//in case of 64bit machine, the alignment becomes 8bytes
			//Set_TY_align(ty_p, 4);
			new_st = New_ST( CURRENT_SYMTAB );
			ST_Init(new_st,
					Save_Str( localname ),
					CLASS_VAR,
					SCLASS_FORMAL,
					EXPORT_LOCAL,
					ty_p);
			Set_ST_is_value_parm( new_st );
			kernel_param.push_back(new_st);
			//ACC_Create_Local_VariableTab(v, vtype, old_st, old_offset, karg);
		}
		else if (kind == KIND_ARRAY)
		{
			TY_IDX etype = ACC_Get_ElementTYForMultiArray(old_st);
			TY_IDX ty_p = Make_Pointer_Type(etype);
			//Set_TY_align(ty_p, 4);
			new_st = New_ST( CURRENT_SYMTAB );
			ST_Init(new_st,
					Save_Str( localname ),
					CLASS_VAR,
					SCLASS_FORMAL,
					EXPORT_LOCAL,
					ty_p);
			Set_ST_is_value_parm( new_st );
			kernel_param.push_back(new_st);
		}
		else if (kind == KIND_SCALAR)
		{
			map<ST*, ACC_ReductionMap>::iterator itor = acc_reduction_tab_map.find(old_st);	
			//find the symbol in the reduction, then ignore it in the parameters.
			if(itor != acc_reduction_tab_map.end())
				continue;
			new_st = New_ST( CURRENT_SYMTAB );
			ST_Init(new_st,
					Save_Str( localname ),
					CLASS_VAR,
					SCLASS_FORMAL,
					EXPORT_LOCAL,
					ty);
			Set_ST_is_value_parm( new_st );
			kernel_param.push_back(new_st);
		}
		ACC_VAR_TABLE var;
		if(old_offset)
		{
			var.has_offset = TRUE;
			var.orig_offset = old_offset;
		}
		else
			var.has_offset = FALSE;
		var.orig_st = old_st;
		var.new_st = new_st;
		acc_local_new_var_map[old_st] = var;
	}
	int i=0;
	//reduction @Parallel region
	if(isParallel && acc_parallel_loop_info.loopnum>0)
	{
		UINT32 loopinfoIdx = 0;
		ACC_LOOP_INFO acc_loopinfo_local;
		while(loopinfoIdx<acc_parallel_loop_info.loopnum)
		{
			acc_loopinfo_local = acc_parallel_loop_info.acc_loopinfo[loopinfoIdx];
			i = 0;
			while(i < acc_loopinfo_local.acc_forloop[0].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo_local.acc_forloop[0].reductionmap[i];
				////////////////////////////////////////////////////////////////////////////
				//it is in the caller function ST.
				//ST* st_device = reductionmap.deviceName;
				ST* st_host = reductionmap.hostName;

				ST* st_reduction_private_var = New_ST( CURRENT_SYMTAB );
				//sprintf ( reduction_localname, "device_%s", ST_name(reductionMap.hostName));
				ST_Init(st_reduction_private_var, Save_Str2("__private_", ST_name(st_host)), CLASS_VAR, 
							SCLASS_AUTO, EXPORT_LOCAL, ST_type(st_host));
				reductionmap.st_private_var = st_reduction_private_var;
				reductionmap.wn_private_var = WN_Ldid(TY_mtype(ST_type(st_reduction_private_var)), 
						0, st_reduction_private_var, ST_type(st_reduction_private_var));

				
				WN_OFFSET old_offset = 0;		
			    TY_IDX ty_elem = ST_type(st_host);
			    //TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			    char* localname = (char *) alloca(strlen(ST_name(st_host))+10);	
				
				sprintf ( localname, "__reduction_%s", ST_name(st_host) );
				//if (kind == KIND_POINTER)
				//{
				//TY_IDX pty = TY_pointed(ty);
				TY_IDX ty_p = Make_Pointer_Type(ty_elem);
				//Set_TY_align(ty_p, 4);
				ST *karg = NULL;
				karg = New_ST( CURRENT_SYMTAB );
				ST_Init(karg,
						Save_Str( localname ),
						CLASS_VAR,
						SCLASS_FORMAL,
						EXPORT_LOCAL,
						ty_p);
				Set_ST_is_value_parm( karg );
				kernel_param.push_back(karg);
				KernelParameter kernelParameter;
				kernelParameter.st_host = st_host;
				kernelParameter.st_device = reductionmap.deviceName;
				acc_additionalKernelLaunchParamList.push_back(kernelParameter);
				reductionmap.st_Inkernel = karg;
				reductionmap.looptype = acc_loopinfo.acc_forloop[0].looptype;
				//v->reduction_opr = reductionmap.ReductionOpr;
				//}
				ACC_VAR_TABLE var;
				if(old_offset)
				{
					var.has_offset = TRUE;
					var.orig_offset = old_offset;
				}
				else
					var.has_offset = FALSE;
				var.orig_st = st_host;
				var.new_st = st_reduction_private_var;
				acc_local_new_var_map[st_host] = var;
				acc_loopinfo_local.acc_forloop[0].reductionmap[i] = reductionmap;
				i ++;
			}
			i = 0;
			while(acc_loopinfo_local.loopnum>1 && i<acc_loopinfo_local.acc_forloop[1].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo_local.acc_forloop[1].reductionmap[i];
				////////////////////////////////////////////////////////////////////////////
				//it is in the caller function ST.
				//ST* st_device = reductionmap.deviceName; 		
				ST* st_host = reductionmap.hostName;
				TYPE_ID typeID = TY_mtype(ST_type(st_host));

				ST* st_reduction_private_var = New_ST( CURRENT_SYMTAB );
				//sprintf ( reduction_localname, "device_%s", ST_name(reductionMap.hostName));
				ST_Init(st_reduction_private_var, Save_Str2("__private_", ST_name(st_host)), CLASS_VAR, 
							SCLASS_AUTO, EXPORT_LOCAL, ST_type(st_host));
				reductionmap.st_private_var = st_reduction_private_var;
				reductionmap.wn_private_var = WN_Ldid(TY_mtype(ST_type(st_reduction_private_var)), 
						0, st_reduction_private_var, ST_type(st_reduction_private_var));
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					////////////////////////////////////////////
					ST* reduction_param = acc_global_memory_for_reduction_param[typeID];
					if(!reduction_param)
					{
						WN_OFFSET old_offset = 0;		
					    TY_IDX ty_elem = ST_type(st_host);
					    //TY_KIND kind = TY_kind(ty);//ST_name(old_st)
					    char* localname = (char *) alloca(strlen(ST_name(st_host))+10);
						
						sprintf ( localname, "__reduction_%s", ST_name(st_host) );
						//if (kind == KIND_POINTER)
						//{
						//TY_IDX pty = TY_pointed(ty);
						TY_IDX ty_p = Make_Pointer_Type(ty_elem);
						//Set_TY_align(ty_p, 4);
						ST *karg = NULL;
						karg = New_ST( CURRENT_SYMTAB );
						ST_Init(karg,
								Save_Str( localname ),
								CLASS_VAR,
								SCLASS_FORMAL,
								EXPORT_LOCAL,
								ty_p);
						Set_ST_is_value_parm( karg );
						kernel_param.push_back(karg);
						KernelParameter kernelParameter;
						kernelParameter.st_host = st_host;
						kernelParameter.st_device = reductionmap.deviceName;
						acc_additionalKernelLaunchParamList.push_back(kernelParameter);
						acc_global_memory_for_reduction_param[typeID] = karg;
						reductionmap.st_Inkernel = karg;
						//}
					}
					else
					{
						reductionmap.st_Inkernel = reduction_param;
					}
				}
				else if(acc_reduction_mem == ACC_RD_SHARED_MEM)
				{
					////////////////////////////////////////////
					//ST* reduction_param = acc_shared_memory_for_reduction_device[typeID];
					reductionmap.st_Inkernel = acc_st_shared_memory;
					
				}
				
				reductionmap.looptype = acc_loopinfo.acc_forloop[1].looptype;
				ACC_VAR_TABLE var;
				var.has_offset = FALSE;
				var.orig_st = st_host;
				var.new_st = reductionmap.st_private_var;
				acc_local_new_var_map[st_host] = var;
				acc_loopinfo_local.acc_forloop[1].reductionmap[i] = reductionmap;
				i ++;
			}
			i = 0;
			while(acc_loopinfo_local.loopnum==3 && i<acc_loopinfo_local.acc_forloop[2].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo_local.acc_forloop[2].reductionmap[i];
				////////////////////////////////////////////////////////////////////////////
				//it is in the caller function ST.
				//ST* st_device = reductionmap.deviceName; 		
				ST* st_host = reductionmap.hostName;
				TYPE_ID typeID = TY_mtype(ST_type(st_host));

				ST* st_reduction_private_var = New_ST( CURRENT_SYMTAB );
				//sprintf ( reduction_localname, "device_%s", ST_name(reductionMap.hostName));
				ST_Init(st_reduction_private_var, Save_Str2("__private_", ST_name(st_host)), CLASS_VAR, 
							SCLASS_AUTO, EXPORT_LOCAL, ST_type(st_host));
				reductionmap.st_private_var = st_reduction_private_var;
				reductionmap.wn_private_var = WN_Ldid(TY_mtype(ST_type(st_reduction_private_var)), 
						0, st_reduction_private_var, ST_type(st_reduction_private_var));
				////////////////////////////////////////////
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					ST* reduction_param = acc_global_memory_for_reduction_param[typeID];
					if(!reduction_param)
					{
						WN_OFFSET old_offset = 0;		
					    TY_IDX ty_elem = ST_type(st_host);
					    char* localname = (char *) alloca(strlen(ST_name(st_host))+10);	
						
						sprintf ( localname, "__reduction_%s", ST_name(st_host) );
						TY_IDX ty_p = Make_Pointer_Type(ty_elem);
						//Set_TY_align(ty_p, 4);
						ST *karg = NULL;
						karg = New_ST( CURRENT_SYMTAB );
						ST_Init(karg,
								Save_Str( localname ),
								CLASS_VAR,
								SCLASS_FORMAL,
								EXPORT_LOCAL,
								ty_p);
						Set_ST_is_value_parm( karg );
						kernel_param.push_back(karg);
						KernelParameter kernelParameter;
						kernelParameter.st_host = st_host;
						kernelParameter.st_device = reductionmap.deviceName;
						acc_additionalKernelLaunchParamList.push_back(kernelParameter);
						acc_global_memory_for_reduction_param[typeID] = karg;
						reductionmap.st_Inkernel = karg;
						//}
					}
					else
					{
						reductionmap.st_Inkernel = reduction_param;
					}
				}
				else if(acc_reduction_mem == ACC_RD_SHARED_MEM)
				{
					reductionmap.st_Inkernel = acc_st_shared_memory;
				}
				reductionmap.looptype = acc_loopinfo.acc_forloop[2].looptype;
				ACC_VAR_TABLE var;
				var.has_offset = FALSE;
				var.orig_st = st_host;
				var.new_st = reductionmap.st_private_var;
				acc_local_new_var_map[st_host] = var;
				acc_loopinfo_local.acc_forloop[2].reductionmap[i] = reductionmap;				
				i ++;
			}
			acc_parallel_loop_info.acc_loopinfo[loopinfoIdx] = acc_loopinfo_local;
			loopinfoIdx++;
		}			
	}
	//put the reduction in kernel here.

}
/*
Create MicroTask for Working threads.  This includes creating the following:
the corresponding nested symbol table; entries for the TY, PU, and ST
tables; debugging information; PU_Info object; and Whirl tree.
Current_PU_Info is set to point to the new nested function, and the
parallel function's symtab becomes CURRENT_SYMTAB.
*/

static void ACC_Create_MicroTask ( PAR_FUNC_ACC_TYPE func_type, 
											void* pRInfo, BOOL isParallelRegion)
{
  KernelsRegionInfo* pKRInfo;
  ParallelRegionInfo* pPRInfo;
  if(isParallelRegion)
  	pPRInfo = (ParallelRegionInfo*) pRInfo;
  else
  	pKRInfo = (KernelsRegionInfo*)pRInfo;
    // should be merged up after done. Currently reserved for Debug
  const char *construct_type_str = "accrg";
  char temp_str[64];

  // generate new name for nested function

  // should PAR regions and PAR DO's be numbered separately? -- DRK
  const char *old_st_name = ST_name(PU_Info_proc_sym(Current_PU_Info));
  char *st_name = (char *) alloca(strlen(old_st_name) + 32);

  sprintf ( st_name, "__%s_%s_%d_%d", construct_type_str, old_st_name,
	      acc_region_num, acc_construct_num );
  
  acc_construct_num ++;
  // get function prototype

  TY_IDX func_ty_idx = TY_IDX_ZERO;

  if  (func_ty_idx == TY_IDX_ZERO) 
  {
    // create new type for function, and type for pointer to function

    TY& ty = New_TY(func_ty_idx);
    sprintf(temp_str, ".%s", construct_type_str);
    TY_Init(ty, 0, KIND_FUNCTION, MTYPE_UNKNOWN, Save_Str(temp_str));
    Set_TY_align(func_ty_idx, 1);
	
    Set_TY_has_prototype(func_ty_idx);

    TYLIST_IDX parm_idx;
    TYLIST& parm_list = New_TYLIST(parm_idx);
    Set_TY_tylist(ty, parm_idx);
    Set_TYLIST_type(parm_list, Be_Type_Tbl(MTYPE_V));  // return type

    /* turn this off if don't want to use taskargs struct */
    //else if (0)

    Set_TYLIST_type(New_TYLIST(parm_idx), TY_IDX_ZERO); // end of parm list

    // now create a type for a pointer to this function
    TY_IDX ptr_ty_idx;
    TY &ptr_ty = New_TY(ptr_ty_idx);
    sprintf(temp_str, ".%s_ptr", construct_type_str);
    TY_Init(ptr_ty, Pointer_Size, KIND_POINTER, Pointer_Mtype,
            Save_Str(temp_str));
    Set_TY_pointed(ptr_ty, func_ty_idx);
  }


  // create new PU and ST for nested function

  PU_IDX pu_idx;
  PU& pu = New_PU(pu_idx);
  PU_Init(pu, func_ty_idx, CURRENT_SYMTAB);

/*
Many questions of DRK's about flags:

is_pure and no_side_effects shouldn't be set due to output ref. parms?
does no_delete matter?
have no idea: needs_fill_align_lowering, needs_t9, put_in_elf_section,
  has_return_address, has_inlines, calls_{set,long}jmp, namelist
has_very_high_whirl and mp_needs_lno should have been handled already
is inheriting pu_recursive OK?
*/

  Set_PU_no_inline(pu);
  Set_PU_is_nested_func(pu);
  Set_PU_acc(pu);
  Set_PU_has_acc(pu);
  
#ifdef KEY
  //Set_PU_acc_lower_generated(pu);
#endif // KEY
    // child PU inherits language flags from parent
  if (PU_c_lang(Current_PU_Info_pu()))
    Set_PU_c_lang(pu);
  if (PU_cxx_lang(Current_PU_Info_pu()))
    Set_PU_cxx_lang(pu);
  if (PU_f77_lang(Current_PU_Info_pu()))
    Set_PU_f77_lang(pu);
  if (PU_f90_lang(Current_PU_Info_pu()))
    Set_PU_f90_lang(pu);
  if (PU_java_lang(Current_PU_Info_pu()))
    Set_PU_java_lang(pu);

  Set_FILE_INFO_has_acc(File_info);  // is this true after acc lowerer?--DRK
  
  //TY_IDX	   funtype = ST_pu_type(st);
  //BOOL		   has_prototype = TY_has_prototype(funtype);

  acc_parallel_proc = New_ST(GLOBAL_SYMTAB);
  ST_Init(acc_parallel_proc,
          Save_Str (st_name),
          CLASS_FUNC,
          SCLASS_TEXT,
          EXPORT_LOCAL,
          TY_IDX(pu_idx));
  //Set_ST_addr_passed(acc_parallel_proc);
  Set_ST_ACC_kernels_func(acc_parallel_proc);
  Set_ST_sfname_idx(acc_parallel_proc, Save_Str(Src_File_Name));

  Allocate_Object ( acc_parallel_proc );
  
  acc_kernel_functions_st.push_back(acc_parallel_proc);


  // create nested symbol table for parallel function

  New_Scope(CURRENT_SYMTAB + 1,
            Malloc_Mem_Pool,  // find something more appropriate--DRK
            TRUE);
  acc_csymtab = CURRENT_SYMTAB;
  acc_func_level = CURRENT_SYMTAB;
  Scope_tab[acc_csymtab].st = acc_parallel_proc;

  Set_PU_lexical_level(pu, CURRENT_SYMTAB);

  ACC_Create_Func_DST ( st_name );


  // pre-allocate in child as many pregs as there are in the parent

  for (UINT32 i = 1; i < PREG_Table_Size(acc_psymtab); i++) {
    PREG_IDX preg_idx;
    PREG &preg = New_PREG(acc_csymtab, preg_idx);
      // share name with corresponding parent preg
    Set_PREG_name_idx(preg,
      PREG_name_idx((*Scope_tab[acc_psymtab].preg_tab)[preg_idx]));
  }
	
  //create shared meory here
  acc_st_shared_memory = New_ST( CURRENT_SYMTAB);
  ST_Init(acc_st_shared_memory,
	  Save_Str( "__device_shared_memory_reserved"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_F8));//Put this variables in local table
  Set_ST_ACC_shared_array(acc_st_shared_memory);
  // create ST's for parameters

  ST *arg_gtid = NULL;
  ST *task_args = NULL;
  if(isParallelRegion)
  	Create_kernel_parameters_ST(pPRInfo->acc_param, TRUE);
  else  	
  	Create_kernel_parameters_ST(pKRInfo->acc_param, FALSE);
  
  //acc_local_taskargs = NULL;
  //More parameters
  /*WN* l;
  for (l = acc_parms_nodes; l; l = WN_next(l)) 
  {
	ST* pST = WN_st(l);
	TY_IDX tmp_ty = ST_type(pST);    
	TYPE_ID tmp_tid = TY_mtype(tmp_ty);
	WN* param = WN_Ldid(tmp_tid, 0, pST, tmp_ty);
	WN_kid(wn, parm_id) = WN_CreateParm(tmp_tid, param, tmp_ty, WN_PARM_BY_VALUE);
	parm_id ++;
  }*/
  //////////////////////////////////////////////////////////////////////
  /* declare some global variables for threadIdx and blockIdx */
  
  glbl_threadIdx_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_threadIdx_x,
      Save_Str( "__nv50_threadIdx_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_threadIdx_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_threadIdx_y,
      Save_Str( "__nv50_threadIdx_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_threadIdx_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_threadIdx_z,
      Save_Str( "__nv50_threadIdx_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockIdx_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockIdx_x,
      Save_Str( "__nv50_blockIdx_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockIdx_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockIdx_y,
      Save_Str( "__nv50_blockIdx_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockIdx_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockIdx_z,
      Save_Str( "__nv50_blockIdx_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockDim_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockDim_x,
      Save_Str( "__nv50_blockdim_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockDim_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockDim_y,
      Save_Str( "__nv50_blockdim_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockDim_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockDim_z,
      Save_Str( "__nv50_blockdim_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));

	
  glbl_gridDim_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_gridDim_x,
      Save_Str( "__nv50_griddim_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  
  glbl_gridDim_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_gridDim_y,
      Save_Str( "__nv50_griddim_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  
  glbl_gridDim_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_gridDim_z,
      Save_Str( "__nv50_griddim_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  

    // TODO: other procedure specific arguments should
    // be handled here.

    // create WHIRL tree for nested function

  acc_parallel_func = WN_CreateBlock ( );
  acc_reference_block = WN_CreateBlock ( );
  acc_pragma_block = WN_CreateBlock ( );
#ifdef KEY
  WN *current_pu_tree = PU_Info_tree_ptr(Current_PU_Info);
  WN *thread_priv_prag = WN_first(WN_func_pragmas(PU_Info_tree_ptr(Current_PU_Info)));
  
#endif
  // Currently, don't pass data via arguments.
  UINT arg_cnt = 1;
  /* turn this off if don't want to use taskargs struct */
  //if (is_task_region) arg_cnt = 2;

  //UINT slink_arg_pos = arg_cnt - 1;
  WN *func_entry = WN_CreateEntry ( kernel_param.size(), acc_parallel_proc,
                                    acc_parallel_func, acc_pragma_block,
                                    acc_reference_block );
  acc_last_microtask = func_entry;

  //if (has_gtid) {
  //  WN_kid0(func_entry) = WN_CreateIdname ( 0, arg_gtid );
  //} 
  UINT ikid = 0;
  //vector<ST*>::iterator itor = kernel_param.begin();
  while(ikid < kernel_param.size())
  {
     WN_kid(func_entry, ikid) = WN_CreateIdname ( 0, kernel_param[ikid] );
	 //ACC_Add_DST_variable ( kernel_param[ikid], acc_nested_dst, acc_line_number, DST_INVALID_IDX );
  	 ikid ++;
  }

     // TODO: variable arguments list should be added here.

  WN_linenum(func_entry) = acc_line_number;


  // create PU_Info for nested function
  
  PU_Info *parallel_pu = TYPE_MEM_POOL_ALLOC ( PU_Info, Malloc_Mem_Pool );
  PU_Info_init ( parallel_pu );
  Set_PU_Info_tree_ptr (parallel_pu, func_entry );

  PU_Info_proc_sym(parallel_pu) = ST_st_idx(acc_parallel_proc);
  PU_Info_maptab(parallel_pu) = acc_cmaptab = WN_MAP_TAB_Create(MEM_pu_pool_ptr);
  PU_Info_pu_dst(parallel_pu) = acc_nested_dst;
  Set_PU_Info_state(parallel_pu, WT_SYMTAB, Subsect_InMem);
  Set_PU_Info_state(parallel_pu, WT_TREE, Subsect_InMem);
  Set_PU_Info_state(parallel_pu, WT_PROC_SYM, Subsect_InMem);
  Set_PU_Info_flags(parallel_pu, PU_IS_COMPILER_GENERATED);

  // don't copy nystrom points to analysis, alias_tag map
  // mp function's points to analysis will be analyzed locally.
  AliasAnalyzer *aa = AliasAnalyzer::aliasAnalyzer();
  if (aa) {
    // Current_Map_Tab is update to PU_Info_maptab(parallel_pu) in PU_Info_maptab
    Is_True(PU_Info_maptab(parallel_pu) == Current_Map_Tab,
        ("parallel_pu's PU's maptab isn't parallel_pu\n"));
    Current_Map_Tab = acc_pmaptab;
    WN_MAP_Set_dont_copy(aa->aliasTagMap(), TRUE);
    WN_MAP_Set_dont_copy(WN_MAP_ALIAS_CGNODE, TRUE);
    Current_Map_Tab = PU_Info_maptab(parallel_pu);
  }
  else {
    Current_Map_Tab = acc_pmaptab;
    WN_MAP_Set_dont_copy(WN_MAP_ALIAS_CGNODE, TRUE);
    Current_Map_Tab = PU_Info_maptab(parallel_pu);
  }

    // use hack to save csymtab using parallel_pu, so we can restore it
    // later when we lower parallel_pu; this is necessary because the
    // new symtab routines can't maintain more than one chain of symtabs
    // in memory at one time, and we lower the parent PU all the way to
    // CG before we lower any of the nested MP PUs
        // Save_Local_Symtab expects this
  Set_PU_Info_symtab_ptr(parallel_pu, NULL);
  Save_Local_Symtab(acc_csymtab, parallel_pu);

  Is_True(PU_Info_state(parallel_pu, WT_FEEDBACK) == Subsect_Missing,
          ("there should be no feedback for parallel_pu"));

  RID *root_rid = RID_Create ( 0, 0, func_entry );
  RID_type(root_rid) = RID_TYPE_func_entry;
  Set_PU_Info_regions_ptr ( parallel_pu, root_rid );
  Is_True(PU_Info_regions_ptr(parallel_pu) != NULL,
	 ("ACC_Create_MicroTask, NULL root RID"));

  PU_Info *tpu = PU_Info_child(Current_PU_Info);

    // add parallel_pu after last child MP PU_Info item in parent's list
  if (tpu && PU_Info_state(tpu, WT_SYMTAB) == Subsect_InMem &&
      PU_acc(PU_Info_pu(tpu)) ) {
    PU_Info *npu;

    while ((npu = PU_Info_next(tpu)) &&
	   PU_Info_state(npu, WT_SYMTAB) == Subsect_InMem &&
	   PU_acc(PU_Info_pu(npu)) )
      tpu = npu;

    PU_Info_next(tpu) = parallel_pu;
    PU_Info_next(parallel_pu) = npu;
  } else {
    PU_Info_child(Current_PU_Info) = parallel_pu;
    PU_Info_next(parallel_pu) = tpu;
  }


  // change some global state; need to clean this up--DRK

  Current_PU_Info = parallel_pu;
  Current_pu = &Current_PU_Info_pu();
  Current_Map_Tab = acc_pmaptab;

  //if (has_gtid)
  //  Add_DST_variable ( arg_gtid, nested_dst, line_number, DST_INVALID_IDX );
  //Add_DST_variable ( arg_slink, nested_dst, line_number, DST_INVALID_IDX );

}


// A VLA that is scoped within a parallel construct
// will have its ALLOCA generated by the front end,
// and it doesn't need a new ALLOCA when localized.

static vector<ST*> acc_inner_scope_vla;

static void 
ACC_Gather_Inner_Scope_Vlas(WN *wn)
{
  if (WN_operator(wn) == OPR_STID && WN_operator(WN_kid0(wn)) == OPR_ALLOCA) {
    acc_inner_scope_vla.push_back(WN_st(wn));    
  }
  else if (WN_operator(wn) == OPR_BLOCK) {
    for (WN *kid = WN_first(wn); kid; kid = WN_next(kid)) {
      ACC_Gather_Inner_Scope_Vlas(kid);
    }
  }
  else {
    for (INT kidno = 0; kidno < WN_kid_count(wn); kidno++) {
      ACC_Gather_Inner_Scope_Vlas(WN_kid(wn, kidno));
    }
  }
}

/*
* When switching scope, use this call to save some global vars.
* When the scope is switched back, use Pop_Some_Globals to restore them.
* csc.
*/
static void 
ACC_Push_Some_Globals( )
{

  acc_old_gtid_st = acc_local_gtid;

}

/*
* Restore globals.
* csc.
*/
static void 
ACC_Pop_Some_Globals( )
{

	// TODO: when enable true nested-parallelism, 
	// a stack style pop/push should be implemented.
  acc_local_gtid = acc_old_gtid_st;
  acc_old_gtid_st = NULL;
}

/*usually, the kernel/loop/parallel cluases are applied to the loop statement.
return the ST of the loop size variable*/
static ST* ACC_AnalysisForLoop()
{
}


/* End the Content of wn_mp_dg.cxx.
*  csc.
*/

/* standardize do comp operations.
 * required by RTL.
 * LT -> LE, GT -> LE
 * csc.
 * must be called before Extract_Do_Info
 */

static void
ACC_Standardize_ForLoop (WN* do_tree)
{
  if (WN_operator(WN_end(do_tree)) == OPR_GE 
      || WN_operator(WN_end(do_tree)) == OPR_LE )
  {
    // need to do nothing.
    return;
  }
  else
  {
    WN_Upper_Bound_Standardize(do_tree, WN_end(do_tree), TRUE);
  }
}


static WN* 
WN_Integer_Cast(WN* tree, TYPE_ID to, TYPE_ID from)
{
  if (from != to)
    return WN_CreateExp1(OPCODE_make_op(OPR_CVT, to, from), tree);
  else
    return tree;
}


/*
* Extract do info for acc scheduling. 
*/

static void 
ACC_Extract_Per_Level_Do_Info ( WN * do_tree, UINT32 level_id )
{
  // standardize do tree.
  acc_test_stmt = WN_COPY_Tree(WN_end(do_tree));
  ACC_Standardize_ForLoop(do_tree);

  WN        *do_idname  = WN_index(do_tree);
  ST        *do_id_st   = WN_st(do_idname);
  WN_OFFSET  do_id_ofst = WN_offsetx(do_idname);
  WN        *do_init;
  WN        *do_limit;
  WN        *do_stride;
  WN		*doloop_body;
  BOOL      was_kid0 = FALSE;

  /* Extract mp scheduling info from do */

  do_init = WN_kid0(WN_start(do_tree));
  //WN_kid0(WN_start(do_tree)) = NULL;

#ifdef KEY
  {
    // bug 5767: handle cvt
    WN * kid0 = WN_kid0 (WN_end (do_tree));
    if (WN_operator (kid0) == OPR_CVT)
      kid0 = WN_kid0 (kid0);

    WN * kid1 = WN_kid1 (WN_end (do_tree));
    if (WN_operator (kid1) == OPR_CVT)
      kid1 = WN_kid0 (kid1);

    if (WN_operator (kid0) == OPR_LDID &&
        WN_st (kid0) == do_id_st &&
        WN_offsetx (kid0) == do_id_ofst)
    { // kid0
      was_kid0 = TRUE;
      do_limit = WN_kid1 (WN_end (do_tree));
      WN_kid1 (WN_end (do_tree)) = NULL;
    }
    else if (WN_operator (kid1) == OPR_LDID &&
             WN_st (kid1) == do_id_st &&
             WN_offsetx (kid1) == do_id_ofst)
    { // kid1
      do_limit = WN_kid0 (WN_end (do_tree));
      WN_kid0 (WN_end (do_tree)) = NULL;
    }
    else
    { // try again
      WN_Upper_Bound_Standardize ( do_tree, WN_end(do_tree), TRUE );
      // handle cvt
      kid0 = WN_kid0 (WN_end (do_tree));
      if (WN_operator (kid0) == OPR_CVT)
        kid0 = WN_kid0 (kid0);

      kid1 = WN_kid1 (WN_end (do_tree));
      if (WN_operator (kid1) == OPR_CVT)
        kid1 = WN_kid0 (kid1);

      if (WN_operator (kid0) == OPR_LDID &&
          WN_st (kid0) == do_id_st &&
          WN_offsetx (kid0) == do_id_ofst)
      { // kid0
        was_kid0 = TRUE;
        do_limit = WN_kid1 (WN_end (do_tree));
        WN_kid1 (WN_end (do_tree)) = NULL;
      }
      else if (WN_operator (kid1) == OPR_LDID &&
               WN_st (kid1) == do_id_st &&
               WN_offsetx (kid1) == do_id_ofst)
      { // kid1
        do_limit = WN_kid0 (WN_end (do_tree));
        WN_kid0 (WN_end (do_tree)) = NULL;
      }
      else // fail
        Fail_FmtAssertion ( "malformed limit test in ACC forloop processing" );
    }
  }
#else
  if ((WN_operator(WN_kid0(WN_end(do_tree))) == OPR_LDID) &&
      (WN_st(WN_kid0(WN_end(do_tree))) == do_id_st) &&
      (WN_offsetx(WN_kid0(WN_end(do_tree))) == do_id_ofst)) {
    was_kid0 = TRUE;
    do_limit = WN_kid1(WN_end(do_tree));
    WN_kid1(WN_end(do_tree)) = NULL;
  } else if ((WN_operator(WN_kid1(WN_end(do_tree))) == OPR_LDID) &&
	     (WN_st(WN_kid1(WN_end(do_tree))) == do_id_st) &&
	     (WN_offsetx(WN_kid1(WN_end(do_tree))) == do_id_ofst)) {
    do_limit = WN_kid0(WN_end(do_tree));
    WN_kid0(WN_end(do_tree)) = NULL;
  } else {
    WN_Upper_Bound_Standardize ( do_tree, WN_end(do_tree), TRUE );
    if ((WN_operator(WN_kid0(WN_end(do_tree))) == OPR_LDID) &&
	(WN_st(WN_kid0(WN_end(do_tree))) == do_id_st) &&
	(WN_offsetx(WN_kid0(WN_end(do_tree))) == do_id_ofst)) {
      was_kid0 = TRUE;
      do_limit = WN_kid1(WN_end(do_tree));
      WN_kid1(WN_end(do_tree)) = NULL;
    } else if ((WN_operator(WN_kid1(WN_end(do_tree))) == OPR_LDID) &&
	       (WN_st(WN_kid1(WN_end(do_tree))) == do_id_st) &&
	       (WN_offsetx(WN_kid1(WN_end(do_tree))) == do_id_ofst)) {
      do_limit = WN_kid0(WN_end(do_tree));
      WN_kid0(WN_end(do_tree)) = NULL;
    } else {
      Fail_FmtAssertion ( "malformed limit test in ACC processing" );
    }
  }
#endif

  if ((WN_operator(WN_kid0(WN_kid0(WN_step(do_tree)))) == OPR_LDID) &&
      (WN_st(WN_kid0(WN_kid0(WN_step(do_tree)))) == do_id_st) &&
      (WN_offsetx(WN_kid0(WN_kid0(WN_step(do_tree)))) == do_id_ofst))
  {
    do_stride = WN_COPY_Tree ( WN_kid1(WN_kid0(WN_step(do_tree))) );
#ifdef KEY
    if (WN_operator (WN_kid0 (WN_step (do_tree))) == OPR_SUB)
    { // the loop goes down, don't miss '-' in (- non-const-stride)
      OPCODE negop = OPCODE_make_op (OPR_NEG, WN_rtype (do_stride), MTYPE_V);
      do_stride = WN_CreateExp1 (negop, do_stride);
    }
#endif // KEY
  }
  else
    do_stride = WN_COPY_Tree ( WN_kid0(WN_kid0(WN_step(do_tree))) );

  /* Generate mp scheduling expressions */
  doloop_body = WN_do_body(do_tree);
  acc_base_node.push_back(do_init);
    // used by Rewrite_Do, need to be copied ?
  acc_limit_node.push_back(WN_COPY_Tree( do_limit ));
  acc_stride_node.push_back(do_stride);
  acc_doloop_body = WN_do_body(do_tree);

  TYPE_ID current_index_type = acc_forloop_index_type[level_id];
  if (acc_collapse_count == 1) {
    if (((WN_operator(WN_end(do_tree)) == OPR_LT) && was_kid0) ||
        ((WN_operator(WN_end(do_tree)) == OPR_GT) && !was_kid0)) { 
      WN* wn_exp0 = WN_Sub(current_index_type, do_limit, WN_COPY_Tree(do_init));
      wn_exp0 = WN_Integer_Cast(wn_exp0, current_index_type, WN_rtype(wn_exp0));
      WN* wn_exp1 = WN_Add(current_index_type, wn_exp0, WN_COPY_Tree(do_stride));
      wn_exp1 = WN_Integer_Cast(wn_exp1, current_index_type, WN_rtype(wn_exp1));
      WN* wn_exp2 = WN_Sub(current_index_type, wn_exp1, WN_Intconst(current_index_type, 1));
      wn_exp2 = WN_Integer_Cast(wn_exp2, current_index_type, WN_rtype(wn_exp2));
      WN* wn_exp3 = WN_Div(current_index_type, wn_exp2, WN_COPY_Tree(do_stride));
      acc_ntrip_node = wn_exp3; 
    } else if (((WN_operator(WN_end(do_tree)) == OPR_GT) && was_kid0) ||
               ((WN_operator(WN_end(do_tree)) == OPR_LT) && !was_kid0)) { 
      WN* wn_exp0 = WN_Sub(current_index_type, do_limit, WN_COPY_Tree(do_init));
      wn_exp0 = WN_Integer_Cast(wn_exp0, current_index_type, WN_rtype(wn_exp0));
      WN* wn_exp1 = WN_Add(current_index_type, wn_exp0, WN_Intconst(current_index_type, 1));
      wn_exp1 = WN_Integer_Cast(wn_exp1, current_index_type, WN_rtype(wn_exp1));
      WN* wn_exp2 = WN_Add(current_index_type, wn_exp1, WN_COPY_Tree(do_stride));
      wn_exp2 = WN_Integer_Cast(wn_exp2, current_index_type, WN_rtype(wn_exp2));
      WN* wn_exp3 = WN_Div(current_index_type, wn_exp2, WN_COPY_Tree(do_stride));
      acc_ntrip_node = wn_exp3; 
    } else { 
      WN* wn_exp0 = WN_Sub(current_index_type, do_limit, WN_COPY_Tree(do_init));
      wn_exp0 = WN_Integer_Cast(wn_exp0, current_index_type, WN_rtype(wn_exp0));
      WN* wn_exp1 = WN_Add(current_index_type, wn_exp0, WN_COPY_Tree(do_stride));
      wn_exp1 = WN_Integer_Cast(wn_exp1, current_index_type, WN_rtype(wn_exp1));
      WN* wn_exp2 = WN_Div(current_index_type, wn_exp1, WN_COPY_Tree(do_stride));
      acc_ntrip_node = wn_exp2; 
    } 
  }

}

/*
* Extract do info for mp scheduling. 
*/

static void 
ACC_Extract_Do_Info ( WN * do_tree )
{
  acc_base_node.clear();
  acc_limit_node.clear();
  acc_stride_node.clear();
  acc_doloop_body = NULL;
  for (UINT32 i = 0; i < acc_collapse_count; i++) {
    ACC_Extract_Per_Level_Do_Info(do_tree, i);
    do_tree = WN_first(WN_do_body(do_tree));
  }
}

static void
ACC_Extract_Index_Info ( WN* pdo_node )
{
  WN *prev_pdo = NULL;
  acc_forloop_index_st.clear();
  acc_forloop_index_type.clear();
  for (UINT32 i = 0; i < acc_collapse_count; i++) {
    if (WN_operator(pdo_node) != OPR_DO_LOOP) {
      /* in case collapse count exceeds number of do loops */
      acc_collapse_count = i;
      pdo_node = prev_pdo;
      break;
    }
    ST * tmp_do_index_st = WN_st(WN_index(pdo_node));
    TYPE_ID tmp_do_index_type = TY_mtype(ST_type(tmp_do_index_st));
    if (tmp_do_index_type == MTYPE_I1 || tmp_do_index_type == MTYPE_I2)
      tmp_do_index_type = MTYPE_I4;
    else if (tmp_do_index_type == MTYPE_U1 || tmp_do_index_type == MTYPE_U2)
      tmp_do_index_type = MTYPE_U4;
    acc_forloop_index_st.push_back(tmp_do_index_st);
    acc_forloop_index_type.push_back(tmp_do_index_type);
    prev_pdo = pdo_node;
    pdo_node = WN_first(WN_do_body(pdo_node));
  }
}

static WN* GenFinalReductionAlgorithm(ST* st_dbuffer, ST* st_host, 
				ST* st_reduction_kernel_name, ST* st_num_of_element, UINT32 iTypesize)
{
	//Set_ST_name_idx
	WN * wn;
	WN* wnx;
	wn = WN_Create(OPC_VCALL, 5);	
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_FINAL_REDUCTION_ALGORITHM);
  
	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
		
	wnx = WN_Lda( Pointer_type, 0, st_host);
    WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_REFERENCE);
  
  	wnx = WN_Ldid(Pointer_type, 0, st_dbuffer, ST_type(st_dbuffer));	
    WN_kid(wn, 1) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);

	
	char* kernelname = ST_name(st_reduction_kernel_name);
	WN* wn_kernelname = WN_LdaString(kernelname,0, strlen(kernelname)+1);
	WN_kid(wn, 2) = WN_CreateParm(Pointer_type, wn_kernelname, 
						 		WN_ty(wn_kernelname), WN_PARM_BY_VALUE);
	
  	wnx = WN_Ldid(TY_mtype(ST_type(st_num_of_element)), 0, st_num_of_element, ST_type(st_num_of_element));	
    WN_kid(wn, 3) = WN_CreateParm(TY_mtype(ST_type(st_num_of_element)), wnx, 
		  							ST_type(st_num_of_element), WN_PARM_BY_VALUE);
	
    WN_kid(wn, 4) = WN_CreateParm(MTYPE_U4, WN_Intconst(MTYPE_U4, iTypesize), 
		  							Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
	
  	return wn;
}


static WN* GenReductionMalloc(ST* st_device, WN* wnSize)
{
	WN * wn;
	WN* wnx;
	wn = WN_Create(OPC_VCALL, 2);	
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_REDUCTION_BUFF_MALLOC);
  
	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
		
	wnx = WN_Lda( Pointer_type, 0, st_device);
    WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_REFERENCE);
  
	WN_kid(wn, 1) = WN_CreateParm(MTYPE_I4, wnSize, Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
	
  	return wn;
}

/*static WN* ACC_Process_SingleReductionNode(WN* reductionode, WN* wn_replace_block) 
{
	//reductionmap	
	ST *old_st = WN_st(reductionode);
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	WN_OFFSET old_offset = WN_offsetx(reductionode);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__device_reduction_%s", ST_name(old_st) );
	
	if (kind == KIND_SCALAR)
	{
		TY_IDX ty_p = Make_Pointer_Type(ty);
		ST *karg = NULL;
		WN *device_addr = NULL;
		ACC_ReductionMap reductionMap;
		karg = New_ST( CURRENT_SYMTAB );
		ST_Init(karg,
				Save_Str( localname ),
				CLASS_VAR,
				SCLASS_AUTO,
				EXPORT_LOCAL,
				ty_p);
		
		//Set_ST_is_value_parm( karg );
		reductionMap.hostName = old_st;
		reductionMap.deviceName = karg;
		reductionMap.ReductionOpr = (OPERATOR)WN_pragma_arg2(reductionode);
		
		reductionMap.reduction_kenels = ACC_GenerateReduction_Kernels_TopLoop(reductionode);

		WN* wn_genReductionMalloc = GenReductionMalloc(karg, acc_dtype);
		WN_INSERT_BlockLast(wn_replace_block, wn_genReductionMalloc);
		acc_loopinfo.acc_forloop[0].reductionmap.push_back(reductionMap);
		//return WN_mallocall;
		//Gen_DataH2D(old_st, karg, isize);
		//device_addr = WN_Lda( Pointer_type, 0, karg);
	}
	else 
		Fail_FmtAssertion (
      "out of WN kind in ACC_Process_SingleReductionNode(...), Reduction variables should be SCALAR.");
	return NULL;	
}*/

static void ACC_Global_Shared_Memory_Reduction(WN* wn_replace_block)
{
   //Generate some information for reduction algorithm
   //if(acc_loopinfo.acc_forloop[0].reductionmap.size() != 0)
   {
		
		//ST* st_reduction_idx1 = New_ST( CURRENT_SYMTAB );
		//char reduction_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);

		//ST* st_reduction_idx2 = New_ST( CURRENT_SYMTAB );

		//sprintf ( reduction_idx_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		//kernel_tmp_variable_count ++;


		//ST_Init(st_reduction_idx2, Save_Str(reduction_idx_localname), CLASS_VAR, 
		//					SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
		/////////////////////////////////////////////////////////////////////////////////
		//For the top level reduction, Index can be catergrized into several types
		//
		//for 3 level nested loop
		//if reduction is in outter body
		//if reduction is in midder body
		//if reduction is in inner body
		if(acc_loopinfo.loopnum == 3)
		{
			INT32 RDIdx = 0;
			while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
			{
				//WN* reduction_Idx1;
				//WN* reduction_Idx2;
				WN* wn_If_stmt_test = NULL;
				ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
				ST* st_reduction_private_var = reductionMap.st_private_var;
				
				if(reductionMap.acc_stmt_location == ACC_OUTTER_LOOP)
				{
					reductionMap.wn_IndexOpr = blockidx;
					//
					WN* test1 = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_x)), 
								WN_COPY_Tree(threadidx), 
								WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_x)), 0));
					WN* test2 = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_y)), 
								WN_COPY_Tree(threadidy), 
								WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_y)), 0));
					wn_If_stmt_test = WN_Binary (OPR_CAND, Boolean_type, test1, test2);
				}
				else if(reductionMap.acc_stmt_location == ACC_MIDDER_LOOP)
				{
					//Index = (thread.y * gridim.x) + blockId.x
					WN* wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					WN_COPY_Tree(threadidy), WN_COPY_Tree(griddimx));
					wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(blockidx));
					//////////////////////////////////////////////////////////////////////////
					reductionMap.wn_IndexOpr = wn_reduction_Idx;
					//wn_If_stmt
					wn_If_stmt_test = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_x)), 
								WN_COPY_Tree(threadidx), 
								WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_x)), 0));
				}
				else //if(reductionMap.acc_stmt_location == ACC_INNER_LOOP)
				{
					//Index = (ThreadId.y * griddim.x + blockId.x) *blockdim.x +thread.x
					WN* wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					WN_COPY_Tree(threadidy), WN_COPY_Tree(griddimx));
					wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(blockidx));
					wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(blockdimx));
					wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(threadidx));
					//////////////////////////////////////////////////////////////////////////
					reductionMap.wn_IndexOpr = wn_reduction_Idx;
				}
				//if reduction is ADD, init value is 0;
				WN* wn_reduction_init;
				WN* wn_array_loc, *wn_stmt;
				int initvalue = 0;
				TY_IDX ty_red = ST_type(reductionMap.hostName);
				wn_reduction_init = ACC_Get_Init_Value_of_Reduction(reductionMap.ReductionOpr);
				wn_reduction_init = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   					reductionMap.st_private_var, ST_type(reductionMap.st_private_var), wn_reduction_init); 
				reductionMap.wn_initialAssign = wn_reduction_init;
	   			//WN_INSERT_BlockLast( wn_replace_block,  wn_reduction_init);	
				//////////////////////////////////////////////////////////////////////////////////
				wn_array_loc = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(reductionMap.wn_IndexOpr),
									reductionMap.st_Inkernel);
				wn_reduction_init = WN_Istore(TY_mtype(ty_red), 0, 
									Make_Pointer_Type(ty_red), wn_array_loc, 
									WN_COPY_Tree(reductionMap.wn_private_var));

				if(wn_If_stmt_test)
				{
					wn_stmt = WN_CreateBlock();
					WN_INSERT_BlockLast(wn_stmt,  wn_reduction_init);
					reductionMap.wn_assignment2Array = WN_CreateIf(wn_If_stmt_test, wn_stmt, WN_CreateBlock());
				}
				else
					reductionMap.wn_assignment2Array = wn_reduction_init;
				//reductionMap.wn_private_var;
				
				/////////////////////////////////////////////////////////////////
				acc_loopinfo.acc_forloop[0].reductionmap[RDIdx] = reductionMap;
				/////////////////////////////////////////////////////////////////	
				RDIdx ++;
			}  
			RDIdx = 0;
			while(RDIdx < acc_loopinfo.acc_forloop[1].reductionmap.size())
			{
				//among workers, if reduction stmt is in inner body, the reduction will be in this gang/block
				//shared memory	
				ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[1].reductionmap[RDIdx];
				ST* st_reduction_private_var = reductionMap.st_private_var;
				//////////////////////////////////////////////////////////////////////////////////
				//acc_global_memory_for_reduction_block
				TY_IDX ty_red = ST_type(reductionMap.hostName);
				TYPE_ID typeID = TY_mtype(ty_red);
				ST* st_backupValue = New_ST(CURRENT_SYMTAB); 
				ST_Init(st_backupValue,
				  Save_Str2("__private_backup_", ST_name(reductionMap.hostName)),
				  CLASS_VAR,
				  SCLASS_AUTO,
				  EXPORT_LOCAL,
				  ty_red);
				reductionMap.st_backupValue = st_backupValue;
				reductionMap.wn_backupValue = WN_Ldid(TY_mtype(ST_type(st_backupValue)), 
						0, st_backupValue, ST_type(st_backupValue));				
				reductionMap.wn_backupStmt = WN_Stid(TY_mtype(ST_type(st_backupValue)), 0, 
	   					st_backupValue, ST_type(st_backupValue), reductionMap.wn_private_var);
				
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					ST* local_RedArray = acc_global_memory_for_reduction_block[typeID];
					if(!local_RedArray)
					{
						TY_IDX ty = Be_Type_Tbl(typeID);//Make_Array_Type(TY_mtype(ty), 1, 1024);
						TY_IDX ty_array = Make_Pointer_Type(ty);
						ST* local_array_red = New_ST(CURRENT_SYMTAB); 
						ST_Init(local_array_red,
						  Save_Str2("__gdata_", ACC_Get_ScalarName_of_Reduction(typeID)),
						  CLASS_VAR,
						  SCLASS_AUTO,
						  EXPORT_LOCAL,
						  ty_array);

						WN* wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
		   					WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockdimy));
						wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)),
		   					wn_offset, WN_COPY_Tree(blockidx));
						wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
		   					wn_offset, WN_Intconst(TY_mtype(ST_type(glbl_blockDim_x)),  TY_size(ty_red)));
						WN* wn_base = WN_Ldid(Pointer_type, 0, reductionMap.st_Inkernel, 
									ST_type(reductionMap.st_Inkernel));
						wn_offset = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
		   					wn_offset, wn_base);
						wn_offset = WN_Stid(Pointer_type, 0, local_array_red, ST_type(local_array_red), wn_offset); 
		   				WN_INSERT_BlockLast( wn_replace_block,  wn_offset);
						acc_global_memory_for_reduction_block[typeID] = local_array_red;
						reductionMap.st_local_array = local_array_red;
						//Set_ST_ACC_shared_array(*st_shared_array_4parallelRegion);
						//ty_shared_array_in_parallel = ty_array;
					}
					else
						reductionMap.st_local_array = local_RedArray;
				}
				else if(acc_reduction_mem == ACC_RD_SHARED_MEM)
				{
					ST* local_RedArray = acc_shared_memory_for_reduction_block[typeID];
					if(!local_RedArray)
					{
						TY_IDX ty = Be_Type_Tbl(typeID);//Make_Array_Type(TY_mtype(ty), 1, 1024);
						TY_IDX ty_array = Make_Pointer_Type(ty);
						ST* local_array_red = New_ST(CURRENT_SYMTAB); 
						ST_Init(local_array_red,
						  Save_Str2("__shdata_", ACC_Get_ScalarName_of_Reduction(typeID)),
						  CLASS_VAR,
						  SCLASS_AUTO,
						  EXPORT_LOCAL,
						  ty_array);

						WN* wn_base = WN_Ldid(Pointer_type, 0, acc_st_shared_memory, 
									ST_type(acc_st_shared_memory));
						WN* wn_typecast = WN_Stid(Pointer_type, 0, local_array_red, ST_type(local_array_red), wn_base); 
		   				WN_INSERT_BlockLast( wn_replace_block,  wn_typecast);
						acc_shared_memory_for_reduction_block[typeID] = local_array_red;
						reductionMap.st_local_array = local_array_red;
						//Set_ST_ACC_shared_array(*st_shared_array_4parallelRegion);
						//ty_shared_array_in_parallel = ty_array;
					}
					else
						reductionMap.st_local_array = local_RedArray;
				}
				//ST* old_st = reductionMap.hostName;
				//TY_IDX ty = ST_type(old_st);
				//there are 2 situations
				//reduction in vector -body
				WN* wn_if_stmt_test = NULL;
				if(reductionMap.acc_stmt_location == ACC_INNER_LOOP)
				{
					//array index
					//tid = threadIdx.y * blockdim.x + threadIdx.x
					WN* wn_ArrIndex = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
										WN_COPY_Tree(threadidy), WN_COPY_Tree(blockdimx));
					wn_ArrIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
										wn_ArrIndex, WN_COPY_Tree(threadidx));
					reductionMap.wn_IndexOpr = wn_ArrIndex;
					//wn_ArrIndex = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), wn_ArrIndex);
				}
				//reduction in worker -body
				else //if(reductionMap.acc_stmt_location == ACC_MIDDER_LOOP)
				{
					WN* wn_ArrIndex = WN_COPY_Tree(threadidy);
					reductionMap.wn_IndexOpr = wn_ArrIndex;
					wn_if_stmt_test = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_x)), 
								WN_COPY_Tree(threadidx), 
								WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_x)), 0));
				}
				
				//if reduction is ADD, init value is 0;
				WN* wn_reduction_init;
				WN* wn_array_loc, *wn_stmt;
				int initvalue = 0;
				//Init reduction private data
				wn_reduction_init = ACC_Get_Init_Value_of_Reduction(reductionMap.ReductionOpr);
				wn_reduction_init = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   					reductionMap.st_private_var, ST_type(reductionMap.st_private_var), wn_reduction_init); 
	   			
	   			reductionMap.wn_initialAssign = wn_reduction_init;	
				//////////////////////////////////////////////////////////////////////////////////
				wn_array_loc = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(reductionMap.wn_IndexOpr),
									reductionMap.st_local_array);
				wn_reduction_init = WN_Istore(TY_mtype(ty_red), 0, 
									Make_Pointer_Type(ty_red), wn_array_loc, 
									WN_COPY_Tree(reductionMap.wn_private_var));
				
				if(wn_if_stmt_test)
				{
					wn_stmt = WN_CreateBlock();
					WN_INSERT_BlockLast(wn_stmt,  wn_reduction_init);
					reductionMap.wn_assignment2Array = WN_CreateIf(wn_if_stmt_test, wn_stmt, WN_CreateBlock());
				}
				else
					reductionMap.wn_assignment2Array = wn_reduction_init;
				////////////////////////////////////////////////////////////////////////////////////////////
				WN* wn_storeback2PrivateVar = ACC_LoadDeviceSharedArrayElem(WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_x)), 0), 
										reductionMap.st_local_array);	
				wn_storeback2PrivateVar = WN_Iload(typeID, 0,  ty_red, wn_storeback2PrivateVar);
				wn_storeback2PrivateVar = WN_Binary(reductionMap.ReductionOpr, typeID, 
											wn_storeback2PrivateVar, reductionMap.wn_backupValue);
				wn_storeback2PrivateVar = WN_Stid(typeID, 0, 
	   										reductionMap.st_private_var, ST_type(reductionMap.st_private_var), 
	   										wn_storeback2PrivateVar);
				reductionMap.wn_assignBack2PrivateVar = wn_storeback2PrivateVar;
				////////////////////////////////////////////////////////////////////////////////////////////
				acc_loopinfo.acc_forloop[1].reductionmap[RDIdx] = reductionMap;
				RDIdx ++;
			}
			
			RDIdx = 0;
			while(RDIdx < acc_loopinfo.acc_forloop[2].reductionmap.size())
			{
				//among workers, if reduction stmt is in inner body, the reduction will be in this gang/block
				//shared memory	
				ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[2].reductionmap[RDIdx];
				ST* st_reduction_private_var = reductionMap.st_private_var;
				//////////////////////////////////////////////////////////////////////////////////
				//acc_global_memory_for_reduction_block
				TY_IDX ty_red = ST_type(reductionMap.hostName);
				TYPE_ID typeID = TY_mtype(ty_red);
				ST* st_backupValue = New_ST(CURRENT_SYMTAB); 
				ST_Init(st_backupValue,
				  Save_Str2("__private_backup_", ST_name(reductionMap.hostName)),
				  CLASS_VAR,
				  SCLASS_AUTO,
				  EXPORT_LOCAL,
				  ty_red);
				reductionMap.st_backupValue = st_backupValue;
				reductionMap.wn_backupValue = WN_Ldid(typeID, 
						0, st_backupValue, ST_type(st_backupValue));				
				reductionMap.wn_backupStmt = WN_Stid(TY_mtype(ST_type(st_backupValue)), 0, 
	   					st_backupValue, ST_type(st_backupValue), reductionMap.wn_private_var);
				///////////////////////////////////////////////////////////////////////////////////
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					ST* local_RedArray = acc_global_memory_for_reduction_block[typeID];
					if(!local_RedArray)
					{
						TY_IDX ty = Be_Type_Tbl(typeID);//Make_Array_Type(TY_mtype(ty), 1, 1024);
						TY_IDX ty_array = Make_Pointer_Type(ty);
						ST* local_arra_red = New_ST(CURRENT_SYMTAB); 
						ST_Init(local_arra_red,
						  Save_Str2("__gdata_", ACC_Get_ScalarName_of_Reduction(typeID)),
						  CLASS_VAR,
						  SCLASS_AUTO,
						  EXPORT_LOCAL,
						  ty_array);

						WN* wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
		   					WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockdimy));
						wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)),
		   					wn_offset, WN_COPY_Tree(blockidx));
						wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
		   					wn_offset, WN_Intconst(TY_mtype(ST_type(glbl_blockDim_x)),  TY_size(ty_red)));
						
						WN* wn_base = WN_Ldid(Pointer_type, 0, reductionMap.st_Inkernel, 
									ST_type(reductionMap.st_Inkernel));
						wn_offset = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
		   					wn_offset, wn_base);
						wn_offset = WN_Stid(Pointer_type, 0, local_arra_red, ST_type(local_arra_red), wn_offset); 
		   				WN_INSERT_BlockLast( wn_replace_block,  wn_offset);
						acc_global_memory_for_reduction_block[typeID] = local_arra_red;
						reductionMap.st_local_array = local_arra_red;
						//Set_ST_ACC_shared_array(*st_shared_array_4parallelRegion);
						//ty_shared_array_in_parallel = ty_array;
					}
					else
						reductionMap.st_local_array = local_RedArray;
				}				
				else if(acc_reduction_mem == ACC_RD_SHARED_MEM)
				{
					ST* local_RedArray = acc_shared_memory_for_reduction_block[typeID];
					if(!local_RedArray)
					{
						TY_IDX ty = Be_Type_Tbl(typeID);//Make_Array_Type(TY_mtype(ty), 1, 1024);
						TY_IDX ty_array = Make_Pointer_Type(ty);
						ST* local_array_red = New_ST(CURRENT_SYMTAB); 
						ST_Init(local_array_red,
						  Save_Str2("__shdata_", ACC_Get_ScalarName_of_Reduction(typeID)),
						  CLASS_VAR,
						  SCLASS_AUTO,
						  EXPORT_LOCAL,
						  ty_array);

						WN* wn_base = WN_Ldid(Pointer_type, 0, acc_st_shared_memory, 
									ST_type(acc_st_shared_memory));
						WN* wn_typecast = WN_Stid(Pointer_type, 0, local_array_red, ST_type(local_array_red), wn_base); 
		   				WN_INSERT_BlockLast( wn_replace_block,  wn_typecast);
						acc_shared_memory_for_reduction_block[typeID] = local_array_red;
						reductionMap.st_local_array = local_array_red;
						//Set_ST_ACC_shared_array(*st_shared_array_4parallelRegion);
						//ty_shared_array_in_parallel = ty_array;
					}
					else
						reductionMap.st_local_array = local_RedArray;
				}
				//ST* old_st = reductionMap.hostName;
				//TY_IDX ty = ST_type(old_st);
				//there are 2 situations
				//reduction in vector -body
				//WN* wn_if_stmt_test = NULL;
				//reduction in worker -body
				 //if(reductionMap.acc_stmt_location == ACC_MIDDER_LOOP)
				//{
				
				WN* wn_ArrIndex = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
									WN_COPY_Tree(threadidy), WN_COPY_Tree(blockdimx));
				wn_ArrIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
									wn_ArrIndex, WN_COPY_Tree(threadidx));
				reductionMap.wn_IndexOpr = wn_ArrIndex;
				//wn_if_stmt_test = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_x)), 
				//			WN_COPY_Tree(threadidx), 
				//			WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_x)), 0));
				//}
				
				//if reduction is ADD, init value is 0;
				WN* wn_reduction_init;
				WN* wn_array_loc, *wn_stmt1;
				int initvalue = 0;
				//TY_IDX ty_red = ST_type(reductionMap.hostName);
				//Init reduction private data
				wn_reduction_init = ACC_Get_Init_Value_of_Reduction(reductionMap.ReductionOpr);
				wn_reduction_init = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   					reductionMap.st_private_var, ST_type(reductionMap.st_private_var), wn_reduction_init); 
	   			reductionMap.wn_initialAssign = wn_reduction_init;	
				//////////////////////////////////////////////////////////////////////////////////
				wn_array_loc = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(reductionMap.wn_IndexOpr),
									reductionMap.st_local_array);
				wn_reduction_init = WN_Istore(TY_mtype(ty_red), 0, 
									Make_Pointer_Type(ty_red), wn_array_loc,
									WN_COPY_Tree(reductionMap.wn_private_var));

				//if(wn_if_stmt_test)
				//{
				//	wn_stmt = WN_CreateBlock();
				//	WN_INSERT_BlockLast( ,  wn_reduction_init);
				//	reductionMap.wn_assignment2Array = WN_CreateIf(wn_if_stmt_test, wn_stmt, WN_CreateBlock());
				//}
				//else
				reductionMap.wn_assignment2Array = wn_reduction_init;
				////////////////////////////////////////////////////////////////////////////////////////////
				WN* wn_storeBackIndex = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
									WN_COPY_Tree(threadidy), WN_COPY_Tree(blockdimx));
				WN* wn_storeback2PrivateVar = ACC_LoadDeviceSharedArrayElem(wn_storeBackIndex, 
										reductionMap.st_local_array);	
				wn_storeback2PrivateVar = WN_Iload(typeID, 0,  ty_red, wn_storeback2PrivateVar);
				wn_storeback2PrivateVar = WN_Binary(reductionMap.ReductionOpr, typeID, 
											wn_storeback2PrivateVar, reductionMap.wn_backupValue);
				wn_storeback2PrivateVar = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   										reductionMap.st_private_var, ST_type(reductionMap.st_private_var), 
	   										wn_storeback2PrivateVar);
				reductionMap.wn_assignBack2PrivateVar = wn_storeback2PrivateVar;
				////////////////////////////////////////////////////////////////////////////////////////////
				acc_loopinfo.acc_forloop[2].reductionmap[RDIdx] = reductionMap;
				RDIdx ++;
			}
		}
		//for 2 level nested loop
		//for reduction in outter body
		//for reduction in inner body
		//
		else if(acc_loopinfo.loopnum == 2)			
		{
			INT32 RDIdx = 0;
			while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
			{
				//WN* reduction_Idx1;
				//WN* reduction_Idx2;
				//WN* reduction_Idx3;
				WN* wn_If_stmt_test = NULL;
				ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
				ST* st_reduction_private_var = reductionMap.st_private_var;
				
				if(reductionMap.acc_stmt_location == ACC_OUTTER_LOOP && reductionMap.looptype ==ACC_GANG)
				{
					reductionMap.wn_IndexOpr = blockidx;
					WN* test1 = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_x)), 
								WN_COPY_Tree(threadidx), 
								WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_x)), 0));
					WN* test2 = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_y)), 
								WN_COPY_Tree(threadidy), 
								WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_y)), 0));
					wn_If_stmt_test = WN_Binary (OPR_CAND, Boolean_type, test1, test2);
				}
				else if(reductionMap.acc_stmt_location == ACC_OUTTER_LOOP && reductionMap.looptype ==ACC_GANG_WORKER)
				{
					//Index = (thread.y * gridim.x) + blockId.x
					WN* wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					WN_COPY_Tree(threadidy), WN_COPY_Tree(griddimx));
					wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(blockidx));
					//////////////////////////////////////////////////////////////////////////
					reductionMap.wn_IndexOpr = wn_reduction_Idx;
					
					wn_If_stmt_test = WN_Relational (OPR_EQ, TY_mtype(ST_type(glbl_threadIdx_y)), 
								WN_COPY_Tree(threadidx), 
								WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_y)), 0));
				}
				else if(reductionMap.acc_stmt_location = ACC_INNER_LOOP)
				{
					//Index = (ThreadId.y * griddim.x + blockId.x) *blockdim.x +thread.x
					WN* wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					WN_COPY_Tree(threadidy), WN_COPY_Tree(griddimx));
					wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(blockidx));
					wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(blockdimx));
					wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)),
	   					wn_reduction_Idx, WN_COPY_Tree(threadidx));
					//////////////////////////////////////////////////////////////////////////
					reductionMap.wn_IndexOpr = wn_reduction_Idx;
				}				
				
				//if reduction is ADD, init value is 0;
				WN* wn_reduction_init;
				WN* wn_array_loc, *wn_stmt;
				int initvalue = 0;
				TY_IDX ty_red = ST_type(reductionMap.hostName);
				wn_reduction_init = ACC_Get_Init_Value_of_Reduction(reductionMap.ReductionOpr);
				wn_reduction_init = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   					reductionMap.st_private_var, ST_type(reductionMap.st_private_var), wn_reduction_init); 
				reductionMap.wn_initialAssign = wn_reduction_init;
	   			//WN_INSERT_BlockLast( wn_replace_block,  wn_reduction_init);	
				//////////////////////////////////////////////////////////////////////////////////
				wn_array_loc = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(reductionMap.wn_IndexOpr),
									reductionMap.st_Inkernel);
				wn_reduction_init = WN_Istore(TY_mtype(ty_red), 0, 
									Make_Pointer_Type(ty_red), wn_array_loc, 
									WN_COPY_Tree(reductionMap.wn_private_var));

				if(wn_If_stmt_test)
				{
					wn_stmt = WN_CreateBlock();
					WN_INSERT_BlockLast(wn_stmt,  wn_reduction_init);
					reductionMap.wn_assignment2Array = WN_CreateIf(wn_If_stmt_test, wn_stmt, WN_CreateBlock());
				}
				else
					reductionMap.wn_assignment2Array = wn_reduction_init;
				//reductionMap.wn_private_var;
				/////////////////////////////////////////////////////////////////
				acc_loopinfo.acc_forloop[0].reductionmap[RDIdx] = reductionMap;
				/////////////////////////////////////////////////////////////////
				RDIdx ++;
			}
			
			RDIdx = 0;
			while(RDIdx < acc_loopinfo.acc_forloop[1].reductionmap.size())
			{				
				//among workers, if reduction stmt is in inner body, the reduction will be in this gang/block
				//shared memory	
				ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[1].reductionmap[RDIdx];
				ST* st_reduction_private_var = reductionMap.st_private_var;
				//////////////////////////////////////////////////////////////////////////////////
				//acc_global_memory_for_reduction_block
				TY_IDX ty_red = ST_type(reductionMap.hostName);
				TYPE_ID typeID = TY_mtype(ty_red);
				ST* st_backupValue = New_ST(CURRENT_SYMTAB); 
				ST_Init(st_backupValue,
				  Save_Str2("__private_backup_", ST_name(reductionMap.hostName)),
				  CLASS_VAR,
				  SCLASS_AUTO,
				  EXPORT_LOCAL,
				  ty_red);
				reductionMap.st_backupValue = st_backupValue;
				reductionMap.wn_backupValue = WN_Ldid(typeID, 
						0, st_backupValue, ST_type(st_backupValue));				
				reductionMap.wn_backupStmt = WN_Stid(TY_mtype(ST_type(st_backupValue)), 0, 
	   					st_backupValue, ST_type(st_backupValue), reductionMap.wn_private_var);
				//////////////////////////////////////////////////////////////////
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					ST* local_RedArray = acc_global_memory_for_reduction_block[typeID];
					if(!local_RedArray)
					{
						TY_IDX ty = Be_Type_Tbl(typeID);//Make_Array_Type(TY_mtype(ty), 1, 1024);
						TY_IDX ty_array = Make_Pointer_Type(ty);
						ST* local_arra_red = New_ST(CURRENT_SYMTAB); 
						ST_Init(local_arra_red,
						  Save_Str2("__gdata_", ACC_Get_ScalarName_of_Reduction(typeID)),
						  CLASS_VAR,
						  SCLASS_AUTO,
						  EXPORT_LOCAL,
						  ty_array);

						WN* wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
		   					WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockdimy));
						wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)),
		   					wn_offset, WN_COPY_Tree(blockidx));
						wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
		   					wn_offset, WN_Intconst(TY_mtype(ST_type(glbl_blockDim_x)),  TY_size(ty_red)));
						
						WN* wn_base = WN_Ldid(Pointer_type, 0, reductionMap.st_Inkernel, 
									ST_type(reductionMap.st_Inkernel));
						wn_offset = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
		   					wn_offset, wn_base);
						wn_offset = WN_Stid(Pointer_type, 0, local_arra_red, ST_type(local_arra_red), wn_offset); 
		   				WN_INSERT_BlockLast( wn_replace_block,  wn_offset);
						acc_global_memory_for_reduction_block[typeID] = local_arra_red;
						reductionMap.st_local_array = local_arra_red;
						//Set_ST_ACC_shared_array(*st_shared_array_4parallelRegion);
						//ty_shared_array_in_parallel = ty_array;
					}
					else
						reductionMap.st_local_array = local_RedArray;
				}
				else if(acc_reduction_mem == ACC_RD_SHARED_MEM)
				{
					ST* local_RedArray = acc_shared_memory_for_reduction_block[typeID];
					if(!local_RedArray)
					{
						TY_IDX ty = Be_Type_Tbl(typeID);//Make_Array_Type(TY_mtype(ty), 1, 1024);
						TY_IDX ty_array = Make_Pointer_Type(ty);
						ST* local_array_red = New_ST(CURRENT_SYMTAB); 
						ST_Init(local_array_red,
						  Save_Str2("__shdata_", ACC_Get_ScalarName_of_Reduction(typeID)),
						  CLASS_VAR,
						  SCLASS_AUTO,
						  EXPORT_LOCAL,
						  ty_array);

						WN* wn_base = WN_Ldid(Pointer_type, 0, acc_st_shared_memory, 
									ST_type(acc_st_shared_memory));
						WN* wn_typecast = WN_Stid(Pointer_type, 0, local_array_red, 
											ST_type(local_array_red), wn_base); 
		   				WN_INSERT_BlockLast( wn_replace_block,  wn_typecast);
						acc_shared_memory_for_reduction_block[typeID] = local_array_red;
						reductionMap.st_local_array = local_array_red;
						//Set_ST_ACC_shared_array(*st_shared_array_4parallelRegion);
						//ty_shared_array_in_parallel = ty_array;
					}
					else
						reductionMap.st_local_array = local_RedArray;
				}
				//ST* old_st = reductionMap.hostName;
				//TY_IDX ty = ST_type(old_st);
				//there are 2 situations
				//reduction in vector -body
				WN* wn_if_stmt_test = NULL;
				WN* wn_ArrIndex = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
									WN_COPY_Tree(threadidy), WN_COPY_Tree(blockdimx));
				wn_ArrIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
									wn_ArrIndex, WN_COPY_Tree(threadidx));
				reductionMap.wn_IndexOpr = wn_ArrIndex;
				
				//if reduction is ADD, init value is 0;
				WN* wn_reduction_init;
				WN* wn_array_loc, *wn_stmt1;
				int initvalue = 0;
				//TY_IDX ty_red = ST_type(reductionMap.hostName);
				//Init reduction private data
				wn_reduction_init = ACC_Get_Init_Value_of_Reduction(reductionMap.ReductionOpr);
				wn_reduction_init = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   					reductionMap.st_private_var, ST_type(reductionMap.st_private_var), wn_reduction_init); 
	   			
	   			reductionMap.wn_initialAssign = wn_reduction_init;	
				//////////////////////////////////////////////////////////////////////////////////
				wn_array_loc = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(reductionMap.wn_IndexOpr),
									reductionMap.st_Inkernel);
				wn_reduction_init = WN_Istore(TY_mtype(ty_red), 0, 
									Make_Pointer_Type(ty_red), wn_array_loc, 
									WN_COPY_Tree(reductionMap.wn_private_var));

				
				reductionMap.wn_assignment2Array = wn_reduction_init;
				////////////////////////////////////////////////////////////////////////////////////////////
				WN* wn_storeBackIndex;
				if(reductionMap.looptype == ACC_VECTOR)
					wn_storeBackIndex = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
									WN_COPY_Tree(threadidy), WN_COPY_Tree(blockdimx));
				else if(reductionMap.looptype == ACC_WORKER_VECTOR)
					wn_storeBackIndex = WN_Intconst(TY_mtype(ST_type(glbl_threadIdx_x)), 0);
					
				WN* wn_storeback2PrivateVar = ACC_LoadDeviceSharedArrayElem(wn_storeBackIndex, 
										reductionMap.st_local_array);	
				wn_storeback2PrivateVar = WN_Iload(typeID, 0,  ty_red, wn_storeback2PrivateVar);
				wn_storeback2PrivateVar = WN_Binary(reductionMap.ReductionOpr, typeID, 
											wn_storeback2PrivateVar, reductionMap.wn_backupValue);
				wn_storeback2PrivateVar = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   										reductionMap.st_private_var, ST_type(reductionMap.st_private_var), 
	   										wn_storeback2PrivateVar);
				reductionMap.wn_assignBack2PrivateVar = wn_storeback2PrivateVar;
				////////////////////////////////////////////////////////////////////////////////////////////
				
				acc_loopinfo.acc_forloop[1].reductionmap[RDIdx] = reductionMap;
				RDIdx ++;
			}
		}
		// for single level nested loop
		//
		else if(acc_loopinfo.loopnum == 1)	
		{
			INT32 RDIdx = 0;
			while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
			{
				//WN* reduction_Idx1;
				//WN* reduction_Idx2;
				//WN* reduction_Idx3;
				ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
				ST* st_reduction_private_var = reductionMap.st_private_var;
				
				//ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];			
				//Index = (ThreadId.y * griddim.x + blockId.x) *blockdim.x +thread.x
				WN* wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
   					WN_COPY_Tree(threadidy), WN_COPY_Tree(griddimx));
				wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
   					wn_reduction_Idx, WN_COPY_Tree(blockidx));
				wn_reduction_Idx = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
   					wn_reduction_Idx, WN_COPY_Tree(blockdimx));
				wn_reduction_Idx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)),
   					wn_reduction_Idx, WN_COPY_Tree(threadidx));
				//////////////////////////////////////////////////////////////////////////);
				reductionMap.wn_IndexOpr = wn_reduction_Idx;
				
				//if reduction is ADD, init value is 0;
				//if reduction is ADD, init value is 0;
				WN* wn_reduction_init;
				WN* wn_array_loc, *wn_stmt1;
				int initvalue = 0;
				TY_IDX ty_red = ST_type(reductionMap.hostName);
				wn_reduction_init = ACC_Get_Init_Value_of_Reduction(reductionMap.ReductionOpr);
				wn_reduction_init = WN_Stid(TY_mtype(ST_type(reductionMap.st_private_var)), 0, 
	   					reductionMap.st_private_var, ST_type(reductionMap.st_private_var), wn_reduction_init); 
				reductionMap.wn_initialAssign = wn_reduction_init;
	   			//WN_INSERT_BlockLast( wn_replace_block,  wn_reduction_init);	
				//////////////////////////////////////////////////////////////////////////////////
				wn_array_loc = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(reductionMap.wn_IndexOpr),
									reductionMap.st_Inkernel);
				wn_reduction_init = WN_Istore(TY_mtype(ty_red), 0, 
									Make_Pointer_Type(ty_red), wn_array_loc, 
									WN_COPY_Tree(reductionMap.wn_private_var));

				reductionMap.wn_assignment2Array = wn_reduction_init;
				/////////////////////////////////////////////////////////////
				/////////////////////////////////////////////////////////////
				acc_loopinfo.acc_forloop[0].reductionmap[RDIdx] = reductionMap;
				/////////////////////////////////////////////////////////////	
				RDIdx ++;
			}
		}
	   
   	}
}

/*
Transform for statement into GPU kernel statements.
written by daniel tian.
return a tree to replace for_tree, and for_tree should not be
contained in other trees.

This is for parallel region transformation
*/
static WN *
ACC_Transform_SingleForLoop(ParallelRegionInfo* pPRInfo, WN* wn_replace_block)
{
	//Retrieve all the information need, then switch the sym table.
	WN* IndexGenerationBlock;
	int i;
	WN* wn_InnerIndexInit = NULL;
	WN* wn_MidIndexInit = NULL;
	WN* wn_OuterIndexInit = NULL;
	WN* wn_InnerIndexStep = NULL;
	WN* wn_MidIndexStep = NULL;
	WN* wn_OuterIndexStep = NULL;
	//Set up predefined variable in CUDA
	threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));
	///////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////
   IndexGenerationBlock = WN_CreateBlock ();
	//Call reduction generation
	ACC_Global_Shared_Memory_Reduction(IndexGenerationBlock);
   //reduction[reduction_idx] = 0;
   //acc_wn_reduction_index;   
   //ACC_VAR_TABLE* accVar = NULL;   		
   //////////////////////////////////////////////////////////////////////////////////////
   ///////////////////////////////////////////////////////////////////

	if(acc_loopinfo.loopnum == 1)
	{
		//map across gangs and vectors(blocks, threads), 
		//make sure the threads per block less than 1024
		//no matter what 's other clauses in this statement
	    //generate whirl for add
	    //
	   ACC_LOOP_TYPE looptype = acc_loopinfo.acc_forloop[0].looptype;
	   //If the reduction is not NULL, we need to conside it. acc_reduction;
		//INT32 reduction_count;
	   /*if(acc_loopinfo.acc_forloop[0].acc_reduction)
   	   {
   	  	  WN* reductionode = acc_loopinfo.acc_forloop[0].acc_reduction;		  
   	   }*/
	  ST* st_index = acc_loopinfo.acc_forloop[0].acc_index_st;
	  TYPE_ID IndexType = acc_loopinfo.acc_forloop[0].acc_index_type;
	  ST* st_new_tmp;
	  
	  st_new_tmp = New_ST( CURRENT_SYMTAB );
	  char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);

	  sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
	  kernel_tmp_variable_count ++;


	  ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
				SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
	  
	  WN* wn_index = WN_Ldid(TY_mtype(ST_type(st_index)), 0, st_index, ST_type(st_index));
	  
	  INT32 RDIdx = 0;
	  while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
      {
		ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
		WN_INSERT_BlockLast( IndexGenerationBlock,  reductionMap.wn_initialAssign);
		RDIdx ++;
  	  }
	  
   	  if(looptype == ACC_GANG_VECTOR || looptype ==ACC_NONE_SPECIFIED)
   	  {
		   //ST* st_limit = acc_loopinfo.acc_forloop[0].acc_newLimit;//acc_loopinfo.acc_forloop[0].condition
		   //////////////////////////////////////////////////////////////////////////////////////
		   WN* IteratorIndexOpLhs1 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		   											WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockidx));
		   IteratorIndexOpLhs1 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
		   											IteratorIndexOpLhs1, WN_COPY_Tree(threadidx));
		   WN* IteratorIndexOp = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, 
		   											ST_type(st_index), IteratorIndexOpLhs1);
		   WN_INSERT_BlockLast( IndexGenerationBlock,  IteratorIndexOp);
		   //WN* wn_index = WN_Ldid(TY_mtype(ST_type(st_index)), 0, st_index, ST_type(st_index));
		   
		   wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(wn_index), WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
		   wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, 
		   										ST_type(st_index), wn_OuterIndexInit);
		   //WN_INSERT_BlockFirst ( acc_stmt_block,  IteratorIndex);
		   
		   //IndexGenerationBlock = WN_CreateBlock ();	
		   //WN_INSERT_BlockLast( IndexGenerationBlock,  wn_OuterIndexInit);
		   
		   //GridWidthInThreads = blockDim.x * gridDim.x
		   WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)),
		    							WN_COPY_Tree(blockdimx), WN_COPY_Tree(griddimx));
		   WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
										ST_type(st_new_tmp), GridWidthInThreads);
		   WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
   	   }
	  else if(looptype == ACC_GANG_WORKER_VECTOR)
  	  {  	  	   
		   //GridWidthInThreads = blockDim.y * blockDim.x * gridDim.x
		   WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)),
		    							WN_COPY_Tree(blockdimx), WN_COPY_Tree(griddimx));
		   GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)),
		    							WN_COPY_Tree(GridWidthInThreads), WN_COPY_Tree(blockdimy));
		   
		   WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
										ST_type(st_new_tmp), GridWidthInThreads);
		   WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
		   //init the index
		   WN* IteratorIndexOpLhs1 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		   									WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockdimy));
		   IteratorIndexOpLhs1 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		   									IteratorIndexOpLhs1, WN_COPY_Tree(blockidx));		   
		   WN* IteratorIndexOp = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, 
		   									ST_type(st_index), IteratorIndexOpLhs1);
		   WN_INSERT_BlockLast( IndexGenerationBlock,  IteratorIndexOp);

		   
		   IteratorIndexOpLhs1 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		   										WN_COPY_Tree(threadidy), WN_COPY_Tree(blockdimx));
		   IteratorIndexOpLhs1 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
		   										IteratorIndexOpLhs1, WN_COPY_Tree(threadidx));
		   IteratorIndexOpLhs1 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
		   										WN_COPY_Tree(wn_index), IteratorIndexOpLhs1);
		   IteratorIndexOpLhs1 = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, 
		   										ST_type(st_index), IteratorIndexOpLhs1);		   
		   WN_INSERT_BlockLast( IndexGenerationBlock,  IteratorIndexOpLhs1);
		   
		   wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(wn_index), WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
		   
		   wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, 
		   					ST_type(st_index), wn_OuterIndexInit);
		   
  	  }
	   //Create do While
	   //WN* wn_index = WN_Ldid(IndexType, 0, st_index, ST_type(st_index));
	   //WN* wn_limit = WN_Ldid(IndexType, 0, st_limit, ST_type(st_limit));
	   WN* wn_forloop_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
	   //WN_Relational (OPR_LT, TY_mtype(ST_type(st_index)), wn_index, wn_limit);
	   
	   /******************************************************************/
	   //While BODY
	   /******************************************************************/
	   //WN* Do_block = WN_CreateBlock ();
	   //WN* doLoopBody = acc_loopinfo.acc_forloop[0].acc_loopbody;
	   
	   //WN* ConditionalExe = WN_Relational (OPR_GE, TY_mtype(ST_type(st_index)), 
		//						WN_COPY_Tree(wn_index), 
		//						WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
	   
		//WN* doLoopBody = WN_CreateIf(ConditionalExe, 
		//			acc_loopinfo.acc_forloop[0].acc_loopbody, 
		//			WN_CreateBlock());

	   
	   
	   //WN_INSERT_BlockFirst ( Do_block,  doLoopBody);		  

		// i = i + GridWidthInThreads;
	    //load i
		//wn_index = WN_Ldid(IndexType, 0, st_index, ST_type(st_index));		
		//load GridWidthInThreads
		WN* GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
		//i + GridWidthInThreads;
		wn_OuterIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_index)), 
								WN_COPY_Tree(wn_index), GridWidthInThreads);
		//store i
		wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, 
								ST_type(st_index), wn_OuterIndexStep);

		WN* wn_loopidame = WN_CreateIdname(0,st_index);
		WN* wn_forloop = WN_CreateDO(wn_loopidame, wn_OuterIndexInit, wn_forloop_test,
							wn_OuterIndexStep, acc_loopinfo.acc_forloop[0].acc_loopbody, NULL);
		//WN_INSERT_BlockLast(Do_block, wn_index);
		
		//Do something here to tranform the loop body: 
	    //remove the loop and leave the loop body;
	    //use the block /thread info in cuda;
	    //WN_INSERT_BlockLast ( acc_parallel_func, kernelfun_block );
		
	   /***********************************************/
	   //WN* whileDO = WN_CreateWhileDo(test, Do_block);
	   //localize the ST for the kernel body
	   //ACC_Localize_Parent_Stack lps(FALSE, NULL);
	   WN_INSERT_BlockLast(IndexGenerationBlock, wn_forloop);
	   RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
			WN_INSERT_BlockLast( IndexGenerationBlock,  reductionMap.wn_assignment2Array);
			//launch dynamic parallism here
			RDIdx ++;
		}
	}
	else if(acc_loopinfo.loopnum == 2)
	{
		//Outer loop, distribute across gangs, y direct
		//Inner loop, distribute both gangs and threads, ()	
		//m * n
	   ST* st_OuterIndex = acc_loopinfo.acc_forloop[0].acc_index_st;
	   TYPE_ID OuterIndexType = acc_loopinfo.acc_forloop[0].acc_index_type;
	   //ST* st_OuterLimit = acc_loopinfo.acc_forloop[0].acc_newLimit;
	   
	   ST* st_InnerIndex = acc_loopinfo.acc_forloop[1].acc_index_st;
	   TYPE_ID InnerIndexType = acc_loopinfo.acc_forloop[1].acc_index_type;
	   //ST* st_InnerLimit = acc_loopinfo.acc_forloop[1].acc_newLimit;
	   ACC_LOOP_TYPE OuterType = acc_loopinfo.acc_forloop[0].looptype;
	   ACC_LOOP_TYPE InnerType = acc_loopinfo.acc_forloop[1].looptype;

	   //IndexGenerationBlock = WN_CreateBlock ();
	   
		WN* wn_InnerIndex = WN_Ldid(TY_mtype(ST_type(st_InnerIndex)), 
								0, st_InnerIndex, ST_type(st_InnerIndex));
		WN* wn_OuterIndex = WN_Ldid(TY_mtype(ST_type(st_OuterIndex)), 
								0, st_OuterIndex, ST_type(st_OuterIndex));
		//WN* wn_OuterLimit = WN_Ldid(TY_mtype(ST_type(st_OuterLimit)), 
		//							0, st_OuterLimit, ST_type(st_OuterLimit));
		//WN* wn_InnerLimit = WN_Ldid(TY_mtype(ST_type(st_InnerLimit)), 
		//							0, st_InnerLimit, ST_type(st_InnerLimit));
		
		//WN* OuterTest = WN_Relational (OPR_LT, TY_mtype(ST_type(st_OuterIndex)), 
		//							WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(wn_OuterLimit));
		//WN* InnerTest = WN_Relational (OPR_LT, TY_mtype(ST_type(st_InnerIndex)), 
		//							WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(wn_InnerLimit));
		
		WN* OuterIteratorIndexOp;			
		WN* InnerIteratorIndexOp;
		WN* wn_InnerLoopBody = WN_CreateBlock ();
		WN* wn_OutterLoopbody = WN_CreateBlock ();
		
	    UINT32 RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
			WN_INSERT_BlockLast( IndexGenerationBlock,  reductionMap.wn_initialAssign);
			RDIdx ++;
		}

	   if(OuterType ==  ACC_GANG && InnerType == ACC_VECTOR)
	   {
			//WN* InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(threadidx));
					
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(threadidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);

			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitIndexOp);
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(blockidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);			
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);

			
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(griddimx));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);
			
	   }
	   else if(OuterType ==  ACC_GANG && InnerType == ACC_WORKER_VECTOR)
	   {
			//Init part
			//i=blockIdx.x
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(blockidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);

			WN* InnerInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(threadidy), WN_COPY_Tree(blockdimx));
			//j=blockIdx.x * blockDim.x ;
			InnerInitndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerInitndexOp);
			WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitndexOp);
			//j = j + threadIdx.x;
			InnerInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(threadidx));
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					InnerInitndexOp, WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitndexOp);
			
		   ST* st_new_tmp = New_ST( CURRENT_SYMTAB );
		   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.x * gridDim.x
		    WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockdimy));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
										ST_type(st_new_tmp), GridWidthInThreads);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////
		
			//load GridWidthInThreads
			GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), GridWidthInThreads);

			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(griddimx));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);

	   }
	   else if(OuterType ==  ACC_GANG_WORKER && InnerType == ACC_VECTOR)
	   {
			//Init part
			//i=blockIdx.x * blockDim.y + threadIdx.y
			WN* OuterInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(blockidx), WN_COPY_Tree(blockdimy));
			//i=blockIdx.x * blockDim.y ;
			OuterInitndexOp = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterInitndexOp);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitndexOp);
			//i = i+ threadIdx.y;
			OuterInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(threadidy));
			
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					OuterInitndexOp, WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitndexOp);
			

			//j=threadIdx.x
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(threadidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			
			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitIndexOp);


		   ST* st_new_tmp = New_ST( CURRENT_SYMTAB );
		   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.y * gridDim.x
		    WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimx));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
										ST_type(st_new_tmp), GridWidthInThreads);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////

			
			//load GridWidthInThreads
			GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), GridWidthInThreads);

			
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);
			
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));
			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
	   }	   
	   else
	   {
	   		Is_True(FALSE, ("2 level Loop Combination is wrong@acc_lower:ACC_Transform_MultiForLoop."));
	   }
	   /***********************************************************/
	   /////////////////////////////////////////////////////////////
	   //Let's begin analysis the statement in this kernel block
	    //WN* ConditionalExeL = WN_Relational (OPR_GE, TY_mtype(ST_type(st_OuterIndex)), 
		//							WN_COPY_Tree(wn_OuterIndex), 
		//							WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
	    //WN* ConditionalExeR = WN_Relational (OPR_GE, TY_mtype(ST_type(st_InnerIndex)), 
		//							WN_COPY_Tree(wn_InnerIndex), 
		//							WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
	    //WN* ConditionalExe = WN_LAND (ConditionalExeL, ConditionalExeR);
		//WN* doLoopBody = WN_CreateIf(ConditionalExe, 
		//			acc_loopinfo.acc_forloop[1].acc_loopbody, 
		//			WN_CreateBlock());
					
		//Two nested DO-WHILE  block
		//WN* InnerDOBlock = WN_CreateBlock();
		//WN* OuterDOBlock = WN_CreateBlock();
		

		//WN_INSERT_BlockLast( wn_InnerLoopBody,  doLoopBody);
		//WN_INSERT_BlockLast( wn_InnerLoopBody,  InnerIteratorIndexOp);
		//WN* InnerTest = WN_COPY_Tree(acc_loopinfo.acc_forloop[1].acc_test_stmt);
		//WN* whileDO = WN_CreateWhileDo(InnerTest, wn_InnerLoopBody);


		//WN_INSERT_BlockLast( wn_OutterLoopbody,  whileDO);			
		//WN_INSERT_BlockLast( wn_OutterLoopbody,  OuterIteratorIndexOp);
		WN* wn_Outer_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
		WN* wn_Inner_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[1].acc_test_stmt);
		
	   WN* wn_Innerloopidame = WN_CreateIdname(0,st_InnerIndex);
	   WN* wn_innerforloop = WN_CreateDO(wn_Innerloopidame, wn_InnerIndexInit, wn_Inner_for_test, wn_InnerIndexStep, acc_loopinfo.acc_forloop[1].acc_loopbody, NULL);

	   //handling nonperfect loopnest
	   //WN* wn_prehand_nodes = acc_loopinfo.acc_forloop[0].wn_prehand_nodes;
	   if(acc_loopinfo.acc_forloop[0].wn_prehand_nodes)
	   {
	   		WN_INSERT_BlockLast( wn_OutterLoopbody,	acc_loopinfo.acc_forloop[0].wn_prehand_nodes);
			//wn_prehand_nodes = WN_next(wn_prehand_nodes);
	   }
	    /********************************************************************************/
	    /********************************************************************************/
	    RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[1].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[1].reductionmap[RDIdx];
			WN_INSERT_BlockLast( wn_OutterLoopbody,  reductionMap.wn_backupStmt);
			WN_INSERT_BlockLast( wn_OutterLoopbody,  reductionMap.wn_initialAssign);
			RDIdx ++;
		}
	    /********************************************************************************/
	   
	   WN_INSERT_BlockLast( wn_OutterLoopbody,	wn_innerforloop);
	    /********************************************************************************/
	    /********************************************************************************/
		RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[1].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[1].reductionmap[RDIdx];
			WN_INSERT_BlockLast( wn_OutterLoopbody,  reductionMap.wn_assignment2Array);
			//Call inner local reduction
			/****************************************************************************/
			if(reductionMap.reduction_kenels)
			{
				WN* wn_call = Gen_Sync_Threads();
				WN_INSERT_BlockLast( wn_OutterLoopbody,  wn_call);
				wn_call = ACC_Gen_Call_Local_Reduction(reductionMap.reduction_kenels, reductionMap.st_local_array);
				WN_INSERT_BlockLast( wn_OutterLoopbody,  wn_call);
			}
			WN_INSERT_BlockLast( wn_OutterLoopbody,  reductionMap.wn_assignBack2PrivateVar);
			//Call inner local reduction
			/****************************************************************************/
			RDIdx ++;
		}
	    /********************************************************************************/
	   
	   //handling nonperfect loopnest
	   //WN* wn_afterhand_nodes = acc_loopinfo.acc_forloop[0].wn_afterhand_nodes;
	   if(acc_loopinfo.acc_forloop[0].wn_afterhand_nodes)
	   {
	   		WN_INSERT_BlockLast( wn_OutterLoopbody,	acc_loopinfo.acc_forloop[0].wn_afterhand_nodes);
			//wn_afterhand_nodes = WN_next(wn_afterhand_nodes);
	   }

	   //Create Outer forloop
	   WN* wn_Outerloopidame = WN_CreateIdname(0,st_OuterIndex);
	   WN* wn_Outerforloop = WN_CreateDO(wn_Outerloopidame, wn_OuterIndexInit, wn_Outer_for_test, wn_OuterIndexStep, wn_OutterLoopbody, NULL);

	   
		//whileDO = WN_CreateWhileDo(OuterTest, wn_OutterLoopbody);
		///////////////////////////////////////////////////////   
		//localize the ST for the kernel body
		//ACC_Localize_Parent_Stack lps(FALSE, NULL);		
		WN_INSERT_BlockLast( IndexGenerationBlock,  wn_Outerforloop);
	    /********************************************************************************/
	    /********************************************************************************/
		RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
			WN_INSERT_BlockLast( IndexGenerationBlock,  reductionMap.wn_assignment2Array);
			RDIdx ++;
		}
	    /********************************************************************************/
	   /***********************************************************/
	   //WN_INSERT_BlockFirst ( acc_stmt_block,  IteratorIndex);
	}
	else if(acc_loopinfo.loopnum == 3)
	{
		//Outer loop, distribute across gangs, y direct
		//Inner loop, distribute both gangs and threads, ()		
	   ST* st_OuterIndex = acc_loopinfo.acc_forloop[0].acc_index_st;
	   TYPE_ID OuterIndexType = acc_loopinfo.acc_forloop[0].acc_index_type;
	   //ST* st_OutLimit = acc_loopinfo.acc_forloop[0].acc_newLimit;
	   
	   ST* st_MidIndex = acc_loopinfo.acc_forloop[1].acc_index_st;
	   TYPE_ID MidIndexType = acc_loopinfo.acc_forloop[1].acc_index_type;
	   //ST* st_MidLimit = acc_loopinfo.acc_forloop[1].acc_newLimit;
	   
	   ST* st_InnerIndex = acc_loopinfo.acc_forloop[2].acc_index_st;
	   TYPE_ID InnerIndexType = acc_loopinfo.acc_forloop[2].acc_index_type;
	   //ST* st_InnerLimit = acc_loopinfo.acc_forloop[2].acc_newLimit;
	   ACC_LOOP_TYPE OutterType = acc_loopinfo.acc_forloop[0].looptype;
	   ACC_LOOP_TYPE MidType = acc_loopinfo.acc_forloop[1].looptype;
	   ACC_LOOP_TYPE InnerType = acc_loopinfo.acc_forloop[2].looptype;

	   //IndexGenerationBlock = WN_CreateBlock ();
	   
		WN* wn_InnerIndex = WN_Ldid(TY_mtype(ST_type(st_InnerIndex)), 
								0, st_InnerIndex, ST_type(st_InnerIndex));
		WN* wn_MidIndex = WN_Ldid(TY_mtype(ST_type(st_MidIndex)), 
								0, st_MidIndex, ST_type(st_MidIndex));
		WN* wn_OuterIndex = WN_Ldid(TY_mtype(ST_type(st_OuterIndex)), 
								0, st_OuterIndex, ST_type(st_OuterIndex));
		
		WN* OuterIteratorIndexOp;				
		WN* MidIteratorIndexOp;		
		WN* InnerIteratorIndexOp;
		//Two nested DO-WHILE  block
		WN* InnerDOBlock = WN_CreateBlock();
		WN* MidDOBlock = WN_CreateBlock();
		WN* OuterDOBlock = WN_CreateBlock();

	    /********************************************************************************/
	    /********************************************************************************/
		UINT32 RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
			WN_INSERT_BlockLast( IndexGenerationBlock,  reductionMap.wn_initialAssign);
			RDIdx ++;
		}
	    /********************************************************************************/

	   //this is the only combination
	   if(OutterType ==  ACC_GANG && MidType == ACC_WORKER &&InnerType == ACC_VECTOR)
	   {			
	   		//i=blockIdx.x
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(blockidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			//WN* OuterInitIndexOp = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
			//							st_OuterIndex, ST_type(st_OuterIndex), WN_COPY_Tree(blockidx));
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);
			//j=blockIdx.y * blockDim.y ;
			WN* MidInitndexOp;// = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_MidIndex)), 
							//			WN_COPY_Tree(blockidy), WN_COPY_Tree(blockdimy));
			//MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
			//							st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			//MidInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
			//							WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(threadidy));
			
			//j = threadIdx.y;
	   		wn_MidIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(threadidy), WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_MidIndexInit = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), wn_MidIndexInit);
			//MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
			//							st_MidIndex, ST_type(st_MidIndex), WN_COPY_Tree(threadidy));
			
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			
			//k=threadidx.x
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(threadidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[2].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			//WN* InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(threadidx));
			
			//WN_INSERT_BlockLast( MidDOBlock,  InnerInitIndexOp);
			
		   //ST* st_new_tmp = New_ST( CURRENT_SYMTAB );
		   //char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   //sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   //kernel_tmp_variable_count ++;

			
		   //ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
		   //			SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.y * gridDim.y
		    //WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		   // 										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimy));
			//WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
			//							ST_type(st_new_tmp), GridWidthInThreads);
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////
										
			wn_InnerIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexStep);
			//load GridWidthInThreads
			//GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
			wn_MidIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(blockdimy));

			
			wn_MidIndexStep = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), wn_MidIndexStep);
			
			wn_OuterIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(griddimx));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexStep);			

	   }   
	   
	   
	   /***********************************************************/
	   /////////////////////////////////////////////////////////////
		//Let's begin analysis the statement in this kernel block	   
		//localize the ST for the kernel body
		//ACC_Localize_Parent_Stack lps(FALSE, NULL);
	   WN* wn_Outer_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
	   WN* wn_Mid_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[1].acc_test_stmt);
	   WN* wn_Inner_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[2].acc_test_stmt);
	   //create inner for loop	   
	   WN* wn_Innerloopidame = WN_CreateIdname(0,st_InnerIndex);
	   WN* wn_innerforloop = WN_CreateDO(wn_Innerloopidame, wn_InnerIndexInit, wn_Inner_for_test, wn_InnerIndexStep, acc_loopinfo.acc_forloop[2].acc_loopbody, NULL);

	   //handling nonperfect loopnest
	   //WN* wn_prehand_nodes = acc_loopinfo.acc_forloop[1].wn_prehand_nodes;
	   if(acc_loopinfo.acc_forloop[1].wn_prehand_nodes)
	   {
	   		WN_INSERT_BlockLast( MidDOBlock, acc_loopinfo.acc_forloop[1].wn_prehand_nodes);
			//wn_prehand_nodes = WN_next(wn_prehand_nodes);
	   }
		
	    /********************************************************************************/
	    /********************************************************************************/
		RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[2].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[2].reductionmap[RDIdx];
			WN_INSERT_BlockLast( MidDOBlock,  reductionMap.wn_backupStmt);
			WN_INSERT_BlockLast( MidDOBlock,  reductionMap.wn_initialAssign);
			RDIdx ++;
		}
	    /********************************************************************************/
	   
	   WN_INSERT_BlockLast( MidDOBlock,	wn_innerforloop);
		
	    /********************************************************************************/
	    /********************************************************************************/
		RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[2].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[2].reductionmap[RDIdx];
			WN_INSERT_BlockLast( MidDOBlock,  reductionMap.wn_assignment2Array);
			//Call inner local reduction
			/****************************************************************************/
			if(reductionMap.reduction_kenels)
			{
				WN* wn_call = Gen_Sync_Threads();
				WN_INSERT_BlockLast( MidDOBlock,  wn_call);
				wn_call = ACC_Gen_Call_Local_Reduction(reductionMap.reduction_kenels, reductionMap.st_local_array);
				WN_INSERT_BlockLast( MidDOBlock,  wn_call);
			}
			WN_INSERT_BlockLast( MidDOBlock,  reductionMap.wn_assignBack2PrivateVar);
			//Call inner local reduction
			/****************************************************************************/
			RDIdx ++;
		}
	    /********************************************************************************/
	   
	   //handling nonperfect loopnest	   
	   //WN* wn_afterhand_nodes = acc_loopinfo.acc_forloop[1].wn_afterhand_nodes;
	   if(acc_loopinfo.acc_forloop[1].wn_afterhand_nodes)
	   {
	   		WN_INSERT_BlockLast( MidDOBlock, acc_loopinfo.acc_forloop[1].wn_afterhand_nodes);
			//wn_afterhand_nodes = WN_next(wn_afterhand_nodes);
	   }

	   //create Mid for loop	   
	   WN* wn_Midloopidame = WN_CreateIdname(0,st_MidIndex);
	   WN* wn_Midforloop = WN_CreateDO(wn_Midloopidame, wn_MidIndexInit, wn_Mid_for_test, wn_MidIndexStep, MidDOBlock, NULL);

	   //handling nonperfect loopnest 
	   //wn_prehand_nodes = acc_loopinfo.acc_forloop[0].wn_prehand_nodes;
	   if(acc_loopinfo.acc_forloop[0].wn_prehand_nodes)
	   {
	   		WN_INSERT_BlockLast( OuterDOBlock,	acc_loopinfo.acc_forloop[0].wn_prehand_nodes);
			//wn_prehand_nodes = WN_next(wn_prehand_nodes);
	   }
	    /********************************************************************************/
	    /********************************************************************************/
		RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[1].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[1].reductionmap[RDIdx];
			WN_INSERT_BlockLast( OuterDOBlock,  reductionMap.wn_backupStmt);
			WN_INSERT_BlockLast( OuterDOBlock,  reductionMap.wn_initialAssign);
			RDIdx ++;
		}
	    /********************************************************************************/
	   
	   WN_INSERT_BlockLast( OuterDOBlock,	wn_Midforloop);
		
	    /********************************************************************************/
	    /********************************************************************************/
		RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[1].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[1].reductionmap[RDIdx];
			WN_INSERT_BlockLast( OuterDOBlock,  reductionMap.wn_assignment2Array);
			//Call inner local reduction
			/****************************************************************************/
			if(reductionMap.reduction_kenels)
			{
				WN* wn_call = Gen_Sync_Threads();
				WN_INSERT_BlockLast( OuterDOBlock,  wn_call);
				wn_call = ACC_Gen_Call_Local_Reduction(reductionMap.reduction_kenels, reductionMap.st_local_array);
				WN_INSERT_BlockLast( OuterDOBlock,  wn_call);
			}
			//Get the value after the reduction
			WN_INSERT_BlockLast( OuterDOBlock,  reductionMap.wn_assignBack2PrivateVar);
			//Call inner local reduction
			/****************************************************************************/
			RDIdx ++;
		}
	    /********************************************************************************/
	   
	   //handling nonperfect loopnest
	   //wn_afterhand_nodes = acc_loopinfo.acc_forloop[0].wn_afterhand_nodes;
	   if(acc_loopinfo.acc_forloop[0].wn_afterhand_nodes)
	   {
	   		WN_INSERT_BlockLast( OuterDOBlock,	acc_loopinfo.acc_forloop[0].wn_afterhand_nodes);
			//wn_afterhand_nodes = WN_next(wn_afterhand_nodes);
	   }
	   

	   //Create Outer forloop
	   WN* wn_Outerloopidame = WN_CreateIdname(0,st_OuterIndex);
	   WN* wn_Outerforloop = WN_CreateDO(wn_Outerloopidame, wn_OuterIndexInit, wn_Outer_for_test, wn_OuterIndexStep, OuterDOBlock, NULL);

		WN_INSERT_BlockLast( IndexGenerationBlock,  wn_Outerforloop);
		RDIdx = 0;
		while(RDIdx < acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			ACC_ReductionMap reductionMap = acc_loopinfo.acc_forloop[0].reductionmap[RDIdx];
			WN_INSERT_BlockLast( IndexGenerationBlock,  reductionMap.wn_assignment2Array);
			RDIdx ++;
		}
		/****************************************************************/
		//Dynamic launch reduction kernel or return host and let host handle everything else
		///////////////////////////////////////////////////////
	   
	   /***********************************************************/
	}
	else
	{
		//Not support yet
		Is_True(FALSE, ("3 Level Loop Combination is wrong@acc_lower:ACC_Transform_MultiForLoop."));
	}
	
   	WN_INSERT_BlockLast ( wn_replace_block, IndexGenerationBlock );
	
}


/*
Transform for statement into GPU kernel statements.
written by daniel tian.
return a tree to replace for_tree, and for_tree should not be
contained in other trees..
*/
static WN *
ACC_Transform_MultiForLoop(KernelsRegionInfo* pKRInfo)
{
	//Retrieve all the information need, then switch the sym table.
	WN* IndexGenerationBlock;
	int i;
	/*for(i=0; i<acc_loopinfo.loopnum; i++)
	{
		ST* old_indexst = acc_loopinfo.acc_forloop[i].acc_index_st;
		char* index_localname = (char *) alloca(strlen(ST_name(old_indexst))+10);
		sprintf ( index_localname, "%s", ST_name(old_indexst));
		acc_loopinfo.acc_forloop[i].szIndexName = index_localname;

		if(WN_kid_count(acc_loopinfo.acc_forloop[i].condition)>1)
		{
			ST* old_limitst = WN_st(WN_kid(acc_loopinfo.acc_forloop[i].condition,0));
			char* limit_localname = (char *) alloca(strlen(ST_name(old_limitst))+10);
			sprintf ( limit_localname, "%s", ST_name(old_limitst));
			acc_loopinfo.acc_forloop[i].szLimitName = limit_localname;
		}
		else if(OPCODE_operator(WN_opcode(acc_loopinfo.acc_forloop[i].condition)) == OPR_INTCONST);
			acc_loopinfo.acc_forloop[i].szLimitName = NULL;
	}*/
	//Here get the def/use list

	//swich the table
	/* Initialization. */	
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	//outline
	ACC_Push_Some_Globals( );
    ACC_Create_MicroTask( PAR_FUNC_ACC_KERNEL, (void*)pKRInfo, FALSE);
	//
	int kernel_index_count = 0;
	
	//create private list
	for(i=0; i<acc_loopinfo.loopnum; i++)
	{
		WN* wn_node = acc_loopinfo.acc_forloop[i].acc_private;
	    char szlocalname[256];	  
		for(; wn_node; wn_node=WN_next(wn_node))
		{
			ST* st_private = WN_st(wn_node);
			TY_IDX index_ty = ST_type(st_private);
		    ST* st_new_private = New_ST( CURRENT_SYMTAB );
	    	sprintf ( szlocalname, "%s", ST_name(st_private));
			ST_Init(st_new_private,
		      Save_Str( szlocalname),
		      CLASS_VAR,
		      SCLASS_AUTO,
		      EXPORT_LOCAL,
		      index_ty);//Put this variables in local table
			ACC_VAR_TABLE var;
			var.has_offset = FALSE;
			var.orig_st = st_private;
			var.new_st = st_new_private;
		    acc_local_new_var_map[st_private] = var;
		}		
	}
	
	if(acc_private_nodes)
	{
		WN* wn_node = acc_private_nodes;
	    char szlocalname[256];	  
		for(; wn_node; wn_node=WN_next(wn_node))
		{
			ST* st_private = WN_st(wn_node);
			TY_IDX index_ty = ST_type(st_private);
		    ST* st_new_private = New_ST( CURRENT_SYMTAB );
	    	sprintf ( szlocalname, "%s", ST_name(st_private));
			ST_Init(st_new_private,
		      Save_Str( szlocalname),
		      CLASS_VAR,
		      SCLASS_AUTO,
		      EXPORT_LOCAL,
		      index_ty);//Put this variables in local table
			ACC_VAR_TABLE var;
			var.has_offset = FALSE;
			var.orig_st = st_private;
			var.new_st = st_new_private;
		    acc_local_new_var_map[st_private] = var;
		}		
	}
	//Set up predefined variable in CUDA
	WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));
	///////////////////////////////////////////////////////////////////
   IndexGenerationBlock = WN_CreateBlock ();	
	//Generate some information for reduction algorithm
    //////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////

	if(acc_loopinfo.loopnum == 1)
	{
		//map across gangs and vectors(blocks, threads), 
		//make sure the threads per block less than 1024
		//no matter what 's other clauses in this statement
	    //generate whirl for add
	    //
		//doloop = WN_CreateDO(index, start, end, step, body, NULL);
		//index = WN_CreateIdname(0,WN_st_idx(lcv));
	   //If the reduction is not NULL, we need to conside it. acc_reduction;
		//INT32 reduction_count;
	   //if(acc_loopinfo.acc_forloop[0].acc_reduction)
   	   //{
   	  	//  WN* reductionode = acc_loopinfo.acc_forloop[0].acc_reduction;		  
   	   //}
	   ST* st_index = acc_loopinfo.acc_forloop[0].acc_index_st;
	   TYPE_ID IndexType = acc_loopinfo.acc_forloop[0].acc_index_type;
	   //sST* st_limit = acc_loopinfo.acc_forloop[0].acc_newLimit;//acc_loopinfo.acc_forloop[0].condition
	   //////////////////////////////////////////////////////////////////////////////////////
	   WN* IteratorIndexOpLhs1 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockidx));
	   
	   WN* IteratorIndexOp = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, ST_type(st_index), IteratorIndexOpLhs1);
	   WN* wn_index = WN_Ldid(TY_mtype(ST_type(st_index)), 0, st_index, ST_type(st_index));
	   WN* IteratorIndexOpLhs2 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(wn_index), WN_COPY_Tree(threadidx));
	   IteratorIndexOpLhs2 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), IteratorIndexOpLhs2, 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
	   
	   WN* IteratorIndexInit = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, ST_type(st_index), IteratorIndexOpLhs2);
	   //WN_INSERT_BlockFirst ( acc_stmt_block,  IteratorIndex);
	   
	   IndexGenerationBlock = WN_CreateBlock ();	
	   WN_INSERT_BlockLast( IndexGenerationBlock,  IteratorIndexOp);
	   //WN_INSERT_BlockLast( IndexGenerationBlock,  IteratorIndexInit);
	   
	   ST* st_new_tmp = New_ST( CURRENT_SYMTAB );
	   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
	   
	   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
	   kernel_tmp_variable_count ++;

		
	   ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
	   			SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
		//GridWidthInThreads = blockDim.x * gridDim.x
		//blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
		//			0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	    WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(blockdimx), WN_COPY_Tree(griddimx));
		WN* wn_WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp), GridWidthInThreads);
		WN_INSERT_BlockLast( IndexGenerationBlock,  wn_WidthOp);

		GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
		WN* wn_IndexStepRhs = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_index)), WN_COPY_Tree(wn_index), GridWidthInThreads);
		WN* wn_IndexStep = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, ST_type(st_index), wn_IndexStepRhs);
		//
	   //Create do While
	   //wn_index = WN_Ldid(IndexType, 0, st_index, ST_type(st_index));
	   //WN* wn_limit = WN_Ldid(IndexType, 0, st_limit, ST_type(st_limit));
	   WN* wn_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
	   WN* wn_loopidame = WN_CreateIdname(0,st_index);
	   WN* wn_forloop = WN_CreateDO(wn_loopidame, IteratorIndexInit, wn_for_test, wn_IndexStep, acc_loopinfo.acc_forloop[0].acc_loopbody, NULL);
	   //WN_Relational (OPR_LT, TY_mtype(ST_type(st_index)), wn_index, wn_limit);
	   
	   /******************************************************************/
	   //While BODY
	   /******************************************************************/
	   /*WN* Do_block = WN_CreateBlock ();
	   //WN* doLoopBody = acc_loopinfo.acc_forloop[0].acc_loopbody;
	   
		WN* ConditionalExe = WN_Relational (OPR_GE, TY_mtype(ST_type(st_index)), 
								WN_COPY_Tree(wn_index), 
								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
	   
		WN* doLoopBody = WN_CreateIf(ConditionalExe, 
					acc_loopinfo.acc_forloop[0].acc_loopbody, 
					WN_CreateBlock());

	   
	   
	   WN_INSERT_BlockFirst ( Do_block,  doLoopBody);		  

		// i = i + GridWidthInThreads;
	    //load i
		wn_index = WN_Ldid(IndexType, 0, st_index, ST_type(st_index));		
		//load GridWidthInThreads
		GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
		//i + GridWidthInThreads;
		WN* NewIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_index)), wn_index, GridWidthInThreads);
		//store i
		wn_index = WN_Stid(TY_mtype(ST_type(st_index)), 0, st_index, ST_type(st_index), NewIndex);
		WN_INSERT_BlockLast(Do_block, wn_index);
		
		//Do something here to tranform the loop body: 
	    //remove the loop and leave the loop body;
	    //use the block /thread info in cuda;
	    //WN_INSERT_BlockLast ( acc_parallel_func, kernelfun_block );
		
	   
	   WN* whileDO = WN_CreateWhileDo(test, Do_block);*/
	   //localize the ST for the kernel body
	   WN_INSERT_BlockLast(IndexGenerationBlock, wn_forloop);
	   ACC_Walk_and_Localize(IndexGenerationBlock);
	}
	else if(acc_loopinfo.loopnum == 2)
	{
		//Outer loop, distribute across gangs, y direct
		//Inner loop, distribute both gangs and threads, ()	
		//m * n
	   ST* st_OuterIndex = acc_loopinfo.acc_forloop[0].acc_index_st;
	   TYPE_ID OuterIndexType = acc_loopinfo.acc_forloop[0].acc_index_type;
	   //ST* st_OuterLimit = acc_loopinfo.acc_forloop[0].acc_newLimit;
	   
	   ST* st_InnerIndex = acc_loopinfo.acc_forloop[1].acc_index_st;
	   TYPE_ID InnerIndexType = acc_loopinfo.acc_forloop[1].acc_index_type;
	   //ST* st_InnerLimit = acc_loopinfo.acc_forloop[1].acc_newLimit;
	   ACC_LOOP_TYPE OuterType = acc_loopinfo.acc_forloop[0].looptype;
	   ACC_LOOP_TYPE InnerType = acc_loopinfo.acc_forloop[1].looptype;

	   IndexGenerationBlock = WN_CreateBlock ();
	   
		WN* wn_InnerIndex = WN_Ldid(TY_mtype(ST_type(st_InnerIndex)), 
								0, st_InnerIndex, ST_type(st_InnerIndex));
		WN* wn_OuterIndex = WN_Ldid(TY_mtype(ST_type(st_OuterIndex)), 
								0, st_OuterIndex, ST_type(st_OuterIndex));
		//WN* wn_OuterLimit = WN_Ldid(TY_mtype(ST_type(st_OuterLimit)), 
		//							0, st_OuterLimit, ST_type(st_OuterLimit));
		//WN* wn_InnerLimit = WN_Ldid(TY_mtype(ST_type(st_InnerLimit)), 
		//							0, st_InnerLimit, ST_type(st_InnerLimit));
		
		//WN* OuterTest = WN_Relational (OPR_LT, TY_mtype(ST_type(st_OuterIndex)), 
		//							WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(wn_OuterLimit));
		//WN* InnerTest = WN_Relational (OPR_LT, TY_mtype(ST_type(st_InnerIndex)), 
		//							WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(wn_InnerLimit));
		
		//WN* OuterIteratorIndexOp;			
		//WN* InnerIteratorIndexOp;
		WN* wn_InnerLoopBody = WN_CreateBlock ();
		WN* wn_OutterLoopbody = WN_CreateBlock ();
		WN* wn_InnerIndexInit = NULL;
		WN* wn_OuterIndexInit = NULL;
		WN* wn_InnerIndexStep = NULL;
		WN* wn_OuterIndexStep = NULL;
		

	   if(OuterType ==  ACC_GANG && InnerType == ACC_VECTOR)
	   {
			//wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(threadidx));
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(threadidx), 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);

			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitIndexOp);
			
			//wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
			//							st_OuterIndex, ST_type(st_OuterIndex), WN_COPY_Tree(blockidx));
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(blockidx), 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterIndexInit);

			
			wn_InnerIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexStep);
			
			wn_OuterIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(griddimx));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexStep);
			
	   }
	   else if(OuterType ==  ACC_GANG && InnerType == ACC_GANG_VECTOR)
	   {
			//Init part
			//i=blockIdx.y
			//wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
			//							st_OuterIndex, ST_type(st_OuterIndex), WN_COPY_Tree(blockidy));
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(blockidy), 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  wn_OuterIndexInit);

			wn_InnerIndexInit = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(blockidx), WN_COPY_Tree(blockdimx));
			//j=blockIdx.x * blockDim.x ;
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			WN_INSERT_BlockLast( wn_OutterLoopbody,  wn_InnerIndexInit);
			//j = j + threadIdx.x;
			wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(threadidx));
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), wn_InnerIndexInit, 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitndexOp);
			
		   ST* st_new_tmp = New_ST( CURRENT_SYMTAB );
		   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.x * gridDim.x
		    WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimx), WN_COPY_Tree(griddimx));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
										ST_type(st_new_tmp), GridWidthInThreads);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////
		
			//load GridWidthInThreads
			GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
			wn_InnerIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), GridWidthInThreads);

			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexStep);
			
			wn_OuterIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(griddimy));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexStep);

	   }
	   else if(OuterType ==  ACC_GANG_VECTOR && InnerType == ACC_VECTOR)
	   {
			//Init part
			//i=blockIdx.x * blockDim.y + threadIdx.y
			WN* OuterInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(blockidx), WN_COPY_Tree(blockdimy));
			//i=blockIdx.x * blockDim.y ;
			OuterInitndexOp = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterInitndexOp);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitndexOp);
			//i = i+ threadIdx.y;
			wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(threadidy));
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), wn_OuterIndexInit, 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			
			wn_OuterIndexInit= WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			//wn_InnerIndexStepWN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitndexOp);
			

			//j=threadIdx.x
			//wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(threadidx));
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(threadidx), 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			
			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitIndexOp);


		   ST* st_new_tmp = New_ST( CURRENT_SYMTAB );
		   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.y * gridDim.x
		    WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimx));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
										ST_type(st_new_tmp), GridWidthInThreads);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////

			
			//load GridWidthInThreads
			GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
			wn_OuterIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), GridWidthInThreads);

			
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexStep);
			
			wn_InnerIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));
			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexStep);
	   }
	   else if(OuterType ==  ACC_GANG_VECTOR && InnerType == ACC_GANG_VECTOR)
       {
			//Init part, outter index
			//i=blockIdx.y * blockDim.y + threadIdx.y
			WN* OuterInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(blockidy), WN_COPY_Tree(blockdimy));
			//i=blockIdx.y * blockDim.y ;
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterInitndexOp);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  wn_OuterIndexInit);
			//i = i+ threadIdx.y;
			wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(threadidy));
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), wn_OuterIndexInit, 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitndexOp);
			//Init part, inner index
			//j=blockIdx.x * blockDim.x + threadIdx.x
			WN* InnerInitIndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(blockidx), WN_COPY_Tree(blockdimx));
			//j=blockIdx.x * blockDim.x ;
			InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerInitIndexOp);
			
			WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitIndexOp);
			//j = j+ threadIdx.x;
			wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(threadidx));
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), wn_InnerIndexInit, 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitIndexOp);
						
			/////////////////////////////////////////////////////////////////////////////////////
			//j=threadIdx.y
			//WN* InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(threadidy));
			
			//WN_INSERT_BlockLast( wn_OutterLoopbody,  InnerInitIndexOp);

		   //Out Index update operations
		   ST* st_new_tmpOut = New_ST( CURRENT_SYMTAB );
		   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmpOut, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.x * gridDim.x
		    WN* GridWidthInThreadsOut = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimy));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpOut)), 0, st_new_tmpOut, 
										ST_type(st_new_tmpOut), GridWidthInThreadsOut);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);

			////////////////////////////////////////////////////////////////	
			//Inner Index update Operations
		   ST* st_new_tmpInner = New_ST( CURRENT_SYMTAB );
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmpInner, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.x * gridDim.x
		    WN* GridWidthInThreadsInner = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimx), WN_COPY_Tree(griddimx));
			WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpInner)), 0, st_new_tmpInner, 
										ST_type(st_new_tmpInner), GridWidthInThreadsInner);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////

			
			//load GridWidthInThreads
			GridWidthInThreadsOut= WN_Ldid(TY_mtype(ST_type(st_new_tmpOut)), 0, 
										st_new_tmpOut, ST_type(st_new_tmpOut));
			
			wn_OuterIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), GridWidthInThreadsOut);

			
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexStep);
			
			GridWidthInThreadsInner= WN_Ldid(TY_mtype(ST_type(st_new_tmpInner)), 0, 
										st_new_tmpInner, ST_type(st_new_tmpInner));
			
			wn_InnerIndexStep = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), GridWidthInThreadsInner);
			//InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
			//							WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(griddimy));
			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexStep);
   	   }
	   else
	   {
			//Not support yet
			Is_True(FALSE, ("this loop scheduling for 2-level nested loop @acc_lower:ACC_Transform_MultiForLoop."));
	   }
	   
	   /***********************************************************/
	   /////////////////////////////////////////////////////////////
	   //Let's begin analysis the statement in this kernel block
	   //WN* ConditionalExeL = WN_Relational (OPR_GE, TY_mtype(ST_type(st_OuterIndex)), 
	   //						WN_COPY_Tree(wn_OuterIndex), 
		//							WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
	   // WN* ConditionalExeR = WN_Relational (OPR_GE, TY_mtype(ST_type(st_InnerIndex)), 
		//							WN_COPY_Tree(wn_InnerIndex), 
		//							WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
	    //WN* ConditionalExe = WN_LAND (ConditionalExeL, ConditionalExeR);
		//WN* doLoopBody = WN_CreateIf(ConditionalExe, 
		//			acc_loopinfo.acc_forloop[1].acc_loopbody, 
		//			WN_CreateBlock());
					
		//Two nested DO-WHILE  block
		//WN* InnerDOBlock = WN_CreateBlock();
		//WN* OuterDOBlock = WN_CreateBlock();
	   WN* wn_Outer_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
	   WN* wn_Inner_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[1].acc_test_stmt);
	   //create inner for loop	   
	   WN* wn_Innerloopidame = WN_CreateIdname(0,st_InnerIndex);
	   WN* wn_innerforloop = WN_CreateDO(wn_Innerloopidame, wn_InnerIndexInit, wn_Inner_for_test, wn_InnerIndexStep, acc_loopinfo.acc_forloop[1].acc_loopbody, NULL);

	   //handling nonperfect loopnest
	   if(acc_loopinfo.acc_forloop[0].wn_prehand_nodes)
	   	WN_INSERT_BlockLast( wn_OutterLoopbody,	acc_loopinfo.acc_forloop[0].wn_prehand_nodes);
	   
	   WN_INSERT_BlockLast( wn_OutterLoopbody,	wn_innerforloop);
	   
	   //handling nonperfect loopnest
	   if(acc_loopinfo.acc_forloop[0].wn_afterhand_nodes)
	   	WN_INSERT_BlockLast( wn_OutterLoopbody,	acc_loopinfo.acc_forloop[0].wn_afterhand_nodes);

	   //Create Outer forloop
	   WN* wn_Outerloopidame = WN_CreateIdname(0,st_OuterIndex);
	   WN* wn_Outerforloop = WN_CreateDO(wn_Outerloopidame, wn_OuterIndexInit, wn_Outer_for_test, wn_OuterIndexStep, wn_OutterLoopbody, NULL);

	   
	   //WN_INSERT_BlockLast( wn_InnerLoopBody,  doLoopBody);
	   //WN_INSERT_BlockLast( wn_InnerLoopBody,  InnerIteratorIndexOp);
		//WN* InnerTest = WN_COPY_Tree(acc_loopinfo.acc_forloop[1].acc_test_stmt);
		//WN* whileDO = WN_CreateWhileDo(InnerTest, wn_InnerLoopBody);


		//WN_INSERT_BlockLast( wn_OutterLoopbody,  whileDO);			
		//WN_INSERT_BlockLast( wn_OutterLoopbody,  OuterIteratorIndexOp);
		//WN* OuterTest = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
		//whileDO = WN_CreateWhileDo(OuterTest, wn_OutterLoopbody);
		///////////////////////////////////////////////////////   
		//localize the ST for the kernel body
		ACC_Localize_Parent_Stack lps(FALSE, NULL);		
		WN_INSERT_BlockLast( IndexGenerationBlock,  wn_Outerforloop);
		ACC_Walk_and_Localize(IndexGenerationBlock);
	   
	   /***********************************************************/
	   //WN_INSERT_BlockFirst ( acc_stmt_block,  IteratorIndex);
	}
	else if(acc_loopinfo.loopnum == 3)
	{
		//Outer loop, distribute across gangs, y direct
		//Inner loop, distribute both gangs and threads, ()		
	   ST* st_OuterIndex = acc_loopinfo.acc_forloop[0].acc_index_st;
	   TYPE_ID OuterIndexType = acc_loopinfo.acc_forloop[0].acc_index_type;
	   //ST* st_OutLimit = acc_loopinfo.acc_forloop[0].acc_newLimit;
	   
	   ST* st_MidIndex = acc_loopinfo.acc_forloop[1].acc_index_st;
	   TYPE_ID MidIndexType = acc_loopinfo.acc_forloop[1].acc_index_type;
	   //ST* st_MidLimit = acc_loopinfo.acc_forloop[1].acc_newLimit;
	   
	   ST* st_InnerIndex = acc_loopinfo.acc_forloop[2].acc_index_st;
	   TYPE_ID InnerIndexType = acc_loopinfo.acc_forloop[2].acc_index_type;
	   //ST* st_InnerLimit = acc_loopinfo.acc_forloop[2].acc_newLimit;
	   ACC_LOOP_TYPE OutterType = acc_loopinfo.acc_forloop[0].looptype;
	   ACC_LOOP_TYPE MidType = acc_loopinfo.acc_forloop[1].looptype;
	   ACC_LOOP_TYPE InnerType = acc_loopinfo.acc_forloop[2].looptype;

	   IndexGenerationBlock = WN_CreateBlock ();
	   
		WN* wn_InnerIndex = WN_Ldid(TY_mtype(ST_type(st_InnerIndex)), 
								0, st_InnerIndex, ST_type(st_InnerIndex));
		WN* wn_MidIndex = WN_Ldid(TY_mtype(ST_type(st_MidIndex)), 
								0, st_MidIndex, ST_type(st_MidIndex));
		WN* wn_OuterIndex = WN_Ldid(TY_mtype(ST_type(st_OuterIndex)), 
								0, st_OuterIndex, ST_type(st_OuterIndex));
		
		WN* OuterIteratorIndexOp;				
		WN* MidIteratorIndexOp;		
		WN* InnerIteratorIndexOp;
		//Two nested DO-WHILE  block
		WN* InnerDOBlock = WN_CreateBlock();
		WN* MidDOBlock = WN_CreateBlock();
		WN* OuterDOBlock = WN_CreateBlock();
		WN* wn_InnerIndexInit = NULL;
		WN* wn_MidIndexInit = NULL;
		WN* wn_OuterIndexInit = NULL;
		WN* wn_InnerIndexStep = NULL;
		WN* wn_MidIndexStep = NULL;
		WN* wn_OuterIndexStep = NULL;

	   if(OutterType ==  ACC_GANG && MidType == ACC_GANG_VECTOR &&InnerType == ACC_VECTOR)
	   {			
	   		//i=blockIdx.x
			//WN* OuterInitIndexOp = WN_Stid(TY_mtype(ST_type(st_OutIndex)), 0, 
			//							st_OutIndex, ST_type(st_OutIndex), WN_COPY_Tree(blockidx));
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(blockidx), 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);

			WN* MidInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(blockidy), WN_COPY_Tree(blockdimy));
			//j=blockIdx.y * blockDim.y ;
			MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);
			WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			//j = j + threadIdx.y;
			MidInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(threadidy));
			
	   		wn_MidIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
							MidInitndexOp, WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_MidIndexInit = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), wn_MidIndexInit);
			
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			
			//k=threadidx.x
			//WN* InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(threadidx));
			
			
			//WN_INSERT_BlockLast( MidDOBlock,  InnerInitIndexOp);
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
							WN_COPY_Tree(threadidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[2].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			
		   ST* st_new_tmp = New_ST( CURRENT_SYMTAB );
		   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmp, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.y * gridDim.y
		    WN* GridWidthInThreads = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimy));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, 
										ST_type(st_new_tmp), GridWidthInThreads);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			//load GridWidthInThreads
			GridWidthInThreads = WN_Ldid(TY_mtype(ST_type(st_new_tmp)), 0, st_new_tmp, ST_type(st_new_tmp));
			MidIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), GridWidthInThreads);

			
			wn_MidIndexStep = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidIteratorIndexOp);
			
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(griddimx));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);
			

	   }
	   else if(OutterType ==  ACC_VECTOR && MidType == ACC_GANG_VECTOR &&InnerType == ACC_GANG_VECTOR)
	   {			
	   		//i=threadIdx.z
			//WN* OuterInitIndexOp = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
			//							st_OuterIndex, ST_type(st_OuterIndex), WN_COPY_Tree(threadidz));
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(threadidz), 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);

			//j=blockIdx.y * blockDim.y ;
			WN* MidInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(blockidy), WN_COPY_Tree(blockdimy));
			MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);
			WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			//j = j + threadIdx.y;
			MidInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(threadidy));
			
	   		wn_MidIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
							MidInitndexOp, WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_MidIndexInit = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), wn_MidIndexInit);
			
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			
			//k=blockIdx.x * blockDim.x ;
			WN* InnerInitIndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(blockidx), WN_COPY_Tree(blockdimx));
			InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerInitIndexOp);
			WN_INSERT_BlockLast( MidDOBlock,  InnerInitIndexOp);
			//k=k+threadidx.x
			InnerInitIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(threadidx));
			
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
							InnerInitIndexOp, WN_COPY_Tree(acc_loopinfo.acc_forloop[2].init));
			
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			
			//WN_INSERT_BlockLast( MidDOBlock,  InnerInitIndexOp);
			//InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(threadidx));
			
			//WN_INSERT_BlockLast( MidDOBlock,  InnerInitIndexOp);
			
		   ST* st_new_tmpMid = New_ST( CURRENT_SYMTAB );
		   char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		   sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		   kernel_tmp_variable_count ++;

			
		   ST_Init(st_new_tmpMid, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.y * gridDim.y
		    WN* GridWidthInThreadsMid = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimy));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpMid)), 0, st_new_tmpMid, 
										ST_type(st_new_tmpMid), GridWidthInThreadsMid);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
		    //////////////////////////////////////////////////////////////////////////////
		    ST* st_new_tmpInner = New_ST( CURRENT_SYMTAB );
		    sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		    kernel_tmp_variable_count ++;

			
		    ST_Init(st_new_tmpInner, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.x * gridDim.x
		    WN* GridWidthInThreadsInner = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimx), WN_COPY_Tree(griddimx));
			WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpInner)), 0, st_new_tmpInner, 
										ST_type(st_new_tmpInner), GridWidthInThreadsInner);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//Finished init part
			/////////////////////////////////////////////
			GridWidthInThreadsInner = WN_Ldid(TY_mtype(ST_type(st_new_tmpInner)), 0, st_new_tmpInner, ST_type(st_new_tmpInner));
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), GridWidthInThreadsInner);

			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);

			
			//InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
			//							WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));
			//InnerIteratorIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			//load GridWidthInThreads
			GridWidthInThreadsMid = WN_Ldid(TY_mtype(ST_type(st_new_tmpMid)), 0, st_new_tmpMid, ST_type(st_new_tmpMid));
			MidIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), GridWidthInThreadsMid);

			
			wn_MidIndexStep = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidIteratorIndexOp);
			
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(blockdimz));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);			

	   }
	   else if(OutterType ==  ACC_VECTOR && MidType == ACC_GANG_VECTOR &&InnerType == ACC_GANG)
	   {			
	   		//i=threadIdx.x
			//WN* OuterInitIndexOp = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
			//							st_OuterIndex, ST_type(st_OuterIndex), WN_COPY_Tree(threadidx));
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);

	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(threadidx), 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			//j=blockIdx.y * blockDim.y ;
			WN* MidInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(blockidy), WN_COPY_Tree(blockdimy));
			MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);
			WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			//j = j + threadIdx.y;
			MidInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(threadidy));			
			
	   		wn_MidIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
								MidInitndexOp, WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			
			wn_MidIndexInit = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), wn_MidIndexInit);
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			
			//k=blockIdx.x ;			
			//WN* InnerInitIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), WN_COPY_Tree(blockidx));
			
			//WN_INSERT_BlockLast( MidDOBlock,  InnerInitIndexOp);
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
								WN_COPY_Tree(blockidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[2].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);

		  	//GridWidthInThreadsMid = blockdimy * griddimy, this will be used to update the mid index.
		    ST* st_new_tmpMid = New_ST( CURRENT_SYMTAB );
		    char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		    sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		    kernel_tmp_variable_count ++;

			
		    ST_Init(st_new_tmpMid, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.y * gridDim.y
		    WN* GridWidthInThreadsMid = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimy));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpMid)), 0, st_new_tmpMid, 
										ST_type(st_new_tmpMid), GridWidthInThreadsMid);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
		    
			//Finished init part
			/////////////////////////////////////////////
			//Inner index update
			//GridWidthInThreadsInner = WN_Ldid(TY_mtype(ST_type(st_new_tmpInner)), 0, st_new_tmpInner, ST_type(st_new_tmpInner));
			//InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
			//							WN_COPY_Tree(wn_InnerIndex), GridWidthInThreadsInner);

			
			//InnerIteratorIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(griddimx));
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			//Mid index update
			GridWidthInThreadsMid = WN_Ldid(TY_mtype(ST_type(st_new_tmpMid)), 0, st_new_tmpMid, ST_type(st_new_tmpMid));
			MidIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), GridWidthInThreadsMid);

			
			wn_MidIndexStep = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidIteratorIndexOp);
			//Outer index update
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(blockdimx));
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);			

	   } 
	   else if(OutterType ==  ACC_GANG_VECTOR && MidType == ACC_GANG_VECTOR &&InnerType == ACC_GANG_VECTOR)
	   {			
	   		//i=blockIdx.z * blockDim.z
	   		
			//j=blockIdx.y * blockDim.y ;
			WN* OuterInitIndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(blockidz), WN_COPY_Tree(blockdimz));
			
			wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(OuterInitIndexOp), WN_COPY_Tree(threadidz));
			
			//MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
			//							st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), wn_OuterIndexInit, 
	   								WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);

			//j=blockIdx.y * blockDim.y ;
			WN* MidInitndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(blockidy), WN_COPY_Tree(blockdimy));
			//j = j + threadIdx.y;
			wn_MidIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(MidInitndexOp), WN_COPY_Tree(threadidy));

			wn_MidIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
							wn_MidIndexInit, WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_MidIndexInit = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), wn_MidIndexInit);
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			//j = j + threadIdx.y;
			//MidInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
			//							WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(threadidy));
			
			//MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
			//							st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);
			
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);

			
			//k=blockIdx.x * blockDim.x ;
			WN* InnerInitIndexOp = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(blockidx), WN_COPY_Tree(blockdimx));
			//k = k + threadIdx.x;
			InnerInitIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(InnerInitIndexOp), WN_COPY_Tree(threadidx));
					
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
						InnerInitIndexOp, WN_COPY_Tree(acc_loopinfo.acc_forloop[2].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
			
			//WN_INSERT_BlockLast( MidDOBlock,  InnerInitIndexOp);

		  	//GridWidthInThreadsMid = blockdimy * griddimy, this will be used to update the mid index.
		  	
		    ST* st_new_tmpOuter = New_ST( CURRENT_SYMTAB );
		    char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		    sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		    kernel_tmp_variable_count ++;

			
		    ST_Init(st_new_tmpOuter, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.z * gridDim.z
		    WN* GridWidthInThreadsOuter = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_z)), 
		    										WN_COPY_Tree(blockdimz), WN_COPY_Tree(griddimz));
			WN* WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpOuter)), 0, st_new_tmpOuter, 
										ST_type(st_new_tmpOuter), GridWidthInThreadsOuter);
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			////////////////////////////////////////////////////////////////////////////////
		    ST* st_new_tmpMid = New_ST( CURRENT_SYMTAB );
		    //char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		    sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		    kernel_tmp_variable_count ++;

			
		    ST_Init(st_new_tmpMid, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.y * gridDim.y
		    WN* GridWidthInThreadsMid = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_y)), 
		    										WN_COPY_Tree(blockdimy), WN_COPY_Tree(griddimy));
			WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpMid)), 0, st_new_tmpMid, 
										ST_type(st_new_tmpMid), GridWidthInThreadsMid);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
			//////////////////////////////////////////////////////////
		    ST* st_new_tmpInner = New_ST( CURRENT_SYMTAB );
		    //char tmp_localname[256];// = (char *) alloca(strlen(ST_name(acc_tmp_name_prefix))+10);
		   
		    sprintf ( tmp_localname, "%s%d", acc_tmp_name_prefix, kernel_tmp_variable_count);
		    kernel_tmp_variable_count ++;

			
		    ST_Init(st_new_tmpInner, Save_Str( tmp_localname), CLASS_VAR, 
						SCLASS_AUTO, EXPORT_LOCAL, Be_Type_Tbl(MTYPE_U4));
			//GridWidthInThreads = blockDim.x * gridDim.x
		    WN* GridWidthInThreadsInner = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), 
		    										WN_COPY_Tree(blockdimx), WN_COPY_Tree(griddimx));
			WidthOp = WN_Stid(TY_mtype(ST_type(st_new_tmpInner)), 0, st_new_tmpInner, 
										ST_type(st_new_tmpInner), GridWidthInThreadsInner);
			
			WN_INSERT_BlockLast( IndexGenerationBlock,  WidthOp);
		    
			//Finished init part
			/////////////////////////////////////////////
			//Inner index update
			GridWidthInThreadsInner = WN_Ldid(TY_mtype(ST_type(st_new_tmpInner)), 0, 
											st_new_tmpInner, ST_type(st_new_tmpInner));
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), GridWidthInThreadsInner);

			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			
			//InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
			//							WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(griddimx));
			//InnerIteratorIndexOp = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
			//							st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			//Mid index update
			GridWidthInThreadsMid = WN_Ldid(TY_mtype(ST_type(st_new_tmpMid)), 0, st_new_tmpMid, ST_type(st_new_tmpMid));
			MidIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), GridWidthInThreadsMid);

			
			wn_MidIndexStep = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidIteratorIndexOp);
			//Outer index update
			
			GridWidthInThreadsOuter = WN_Ldid(TY_mtype(ST_type(st_new_tmpOuter)), 0, 
											st_new_tmpOuter, ST_type(st_new_tmpOuter));
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), GridWidthInThreadsOuter);

			
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);
			
			//OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OutIndex)), 
			//							WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(blockdimx));
			//OuterIteratorIndexOp = WN_Stid(TY_mtype(ST_type(st_OutIndex)), 0, 
			//							st_OutIndex, ST_type(st_OutIndex), OuterIteratorIndexOp);			

	   }
	   else if(OutterType ==  ACC_GANG && MidType == ACC_GANG &&InnerType == ACC_VECTOR)
	   {			
	   		//i=blockIdx.z * blockDim.z
	   		
			//j=blockIdx.y * blockDim.y ;
			WN* OuterInitIndexOp;// = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_OutIndex)), 
								 //	WN_COPY_Tree(blockidz), WN_COPY_Tree(blockdimz));
			
			//OuterInitIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OutIndex)), 
			//							WN_COPY_Tree(OuterInitIndexOp), WN_COPY_Tree(threadidz));
			
			//MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
			//							st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);	
	   		wn_OuterIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
	   					WN_COPY_Tree(blockidy), WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
			wn_OuterIndexInit = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), wn_OuterIndexInit);
			
			//WN_INSERT_BlockLast( IndexGenerationBlock,  OuterInitIndexOp);

			//j=blockIdx.y * blockDim.y ;
			WN* MidInitndexOp;// = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_MidIndex)), 
								//		WN_COPY_Tree(blockidy), WN_COPY_Tree(blockdimy));
			//j = j + threadIdx.y;
			//MidInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
			//							WN_COPY_Tree(MidInitndexOp), WN_COPY_Tree(threadidy));

			
			wn_MidIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
	   					WN_COPY_Tree(blockidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
			wn_MidIndexInit = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), wn_MidIndexInit);
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);
			//j = j + threadIdx.y;
			//MidInitndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
			//							WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(threadidy));
			
			//MidInitndexOp = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
			//							st_MidIndex, ST_type(st_MidIndex), MidInitndexOp);
			
			//WN_INSERT_BlockLast( OuterDOBlock,  MidInitndexOp);

			
			//k=blockIdx.x * blockDim.x ;
			WN* InnerInitIndexOp;// = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_InnerIndex)), 
								 //		WN_COPY_Tree(blockidx), WN_COPY_Tree(blockdimx));
			//k = k + threadIdx.x;
			//InnerInitIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
			//							WN_COPY_Tree(InnerInitIndexOp), WN_COPY_Tree(threadidx));
			
	   		wn_InnerIndexInit = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)), 
	   					WN_COPY_Tree(threadidx), WN_COPY_Tree(acc_loopinfo.acc_forloop[2].init));
			wn_InnerIndexInit = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), wn_InnerIndexInit);
		    
			//Finished init part
			/////////////////////////////////////////////
			//Inner index update
			//GridWidthInThreadsInner = WN_Ldid(TY_mtype(ST_type(st_new_tmpInner)), 0, 
			//								st_new_tmpInner, ST_type(st_new_tmpInner));
			InnerIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_InnerIndex)), 
										WN_COPY_Tree(wn_InnerIndex), WN_COPY_Tree(blockdimx));

			
			wn_InnerIndexStep = WN_Stid(TY_mtype(ST_type(st_InnerIndex)), 0, 
										st_InnerIndex, ST_type(st_InnerIndex), InnerIteratorIndexOp);
			
			MidIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_MidIndex)), 
										WN_COPY_Tree(wn_MidIndex), WN_COPY_Tree(griddimx));

			
			wn_MidIndexStep = WN_Stid(TY_mtype(ST_type(st_MidIndex)), 0, 
										st_MidIndex, ST_type(st_MidIndex), MidIteratorIndexOp);
			//Outer index update
			
			//GridWidthInThreadsOuter = WN_Ldid(TY_mtype(ST_type(st_new_tmpOuter)), 0, 
			//								st_new_tmpOuter, ST_type(st_new_tmpOuter));
			OuterIteratorIndexOp = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_OuterIndex)), 
										WN_COPY_Tree(wn_OuterIndex), WN_COPY_Tree(griddimy));

			
			wn_OuterIndexStep = WN_Stid(TY_mtype(ST_type(st_OuterIndex)), 0, 
										st_OuterIndex, ST_type(st_OuterIndex), OuterIteratorIndexOp);

	   }
	   else
	   {
			//Not support yet
			Is_True(FALSE, ("this loop scheduling for 3-level nested loop @acc_lower:ACC_Transform_MultiForLoop."));
	   }
	
	   
	   
	   /***********************************************************/
	   /////////////////////////////////////////////////////////////
	   //Let's begin analysis the statement in this kernel block	   

		//WN* doLoopBody = acc_loopinfo.acc_forloop[2].acc_loopbody;	
	   WN* wn_Outer_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
	   WN* wn_Mid_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[1].acc_test_stmt);
	   WN* wn_Inner_for_test = WN_COPY_Tree(acc_loopinfo.acc_forloop[2].acc_test_stmt);
	   //create inner for loop	   
	   WN* wn_Innerloopidame = WN_CreateIdname(0,st_InnerIndex);
	   WN* wn_innerforloop = WN_CreateDO(wn_Innerloopidame, wn_InnerIndexInit, wn_Inner_for_test, wn_InnerIndexStep, acc_loopinfo.acc_forloop[2].acc_loopbody, NULL);

	   //handling nonperfect loopnest
	   if(acc_loopinfo.acc_forloop[1].wn_prehand_nodes)
	   	WN_INSERT_BlockLast( MidDOBlock,	acc_loopinfo.acc_forloop[1].wn_prehand_nodes);
	   
	   WN_INSERT_BlockLast( MidDOBlock,	wn_innerforloop);
	   
	   //handling nonperfect loopnest
	   if(acc_loopinfo.acc_forloop[1].wn_afterhand_nodes)
	   	WN_INSERT_BlockLast( MidDOBlock,	acc_loopinfo.acc_forloop[1].wn_afterhand_nodes);

	   //create inner for loop	   
	   WN* wn_Midloopidame = WN_CreateIdname(0,st_MidIndex);
	   WN* wn_Midforloop = WN_CreateDO(wn_Midloopidame, wn_MidIndexInit, wn_Mid_for_test, wn_MidIndexStep, MidDOBlock, NULL);

	   //handling nonperfect loopnest
	   if(acc_loopinfo.acc_forloop[0].wn_prehand_nodes)
	   	WN_INSERT_BlockLast( OuterDOBlock,	acc_loopinfo.acc_forloop[0].wn_prehand_nodes);
	   
	   WN_INSERT_BlockLast( OuterDOBlock,	wn_Midforloop);
	   
	   //handling nonperfect loopnest
	   if(acc_loopinfo.acc_forloop[0].wn_afterhand_nodes)
	   	WN_INSERT_BlockLast( OuterDOBlock,	acc_loopinfo.acc_forloop[0].wn_afterhand_nodes);
	   

	   //Create Outer forloop
	   WN* wn_Outerloopidame = WN_CreateIdname(0,st_OuterIndex);
	   WN* wn_Outerforloop = WN_CreateDO(wn_Outerloopidame, wn_OuterIndexInit, wn_Outer_for_test, wn_OuterIndexStep, OuterDOBlock, NULL);


	   
	   
	    /*WN* ConditionalExeL = WN_Relational (OPR_GE, TY_mtype(ST_type(st_OuterIndex)), 
									WN_COPY_Tree(wn_OuterIndex), 
									WN_COPY_Tree(acc_loopinfo.acc_forloop[0].init));
		
	    WN* ConditionalExeM = WN_Relational (OPR_GE, TY_mtype(ST_type(st_MidIndex)), 
									WN_COPY_Tree(wn_MidIndex), 
									WN_COPY_Tree(acc_loopinfo.acc_forloop[1].init));
		
	    WN* ConditionalExeR = WN_Relational (OPR_GE, TY_mtype(ST_type(st_InnerIndex)), 
									WN_COPY_Tree(wn_InnerIndex), 
									WN_COPY_Tree(acc_loopinfo.acc_forloop[2].init));
	    WN* ConditionalExe = WN_LAND (ConditionalExeL, ConditionalExeM);
	    	ConditionalExe = WN_LAND (ConditionalExe, ConditionalExeR);
		WN* doLoopBody = WN_CreateIf(ConditionalExe, 
					acc_loopinfo.acc_forloop[2].acc_loopbody, 
					WN_CreateBlock());


		WN_INSERT_BlockLast( InnerDOBlock,  doLoopBody);
		WN_INSERT_BlockLast( InnerDOBlock,  InnerIteratorIndexOp);
		WN* InnerTest = WN_COPY_Tree(acc_loopinfo.acc_forloop[2].acc_test_stmt);
		WN* whileDO = WN_CreateWhileDo(InnerTest, InnerDOBlock);

		WN_INSERT_BlockLast( MidDOBlock,	whileDO);			
		WN_INSERT_BlockLast( MidDOBlock,	MidIteratorIndexOp);
		WN* MidTest = WN_COPY_Tree(acc_loopinfo.acc_forloop[1].acc_test_stmt);
		whileDO = WN_CreateWhileDo(MidTest, MidDOBlock);
		

		WN_INSERT_BlockLast( OuterDOBlock,  whileDO);			
		WN_INSERT_BlockLast( OuterDOBlock,  OuterIteratorIndexOp);
		WN* OuterTest = WN_COPY_Tree(acc_loopinfo.acc_forloop[0].acc_test_stmt);
		whileDO = WN_CreateWhileDo(OuterTest, OuterDOBlock);   */
		//localize the ST for the kernel body
		ACC_Localize_Parent_Stack lps(FALSE, NULL);
		WN_INSERT_BlockLast( IndexGenerationBlock,  wn_Outerforloop);
		ACC_Walk_and_Localize(IndexGenerationBlock);
		///////////////////////////////////////////////////////
	   
	   /***********************************************************/
	}
	else
	{
		//Not support yet
		Is_True(FALSE, ("more than 3 Level Loop Combination is not supported.@acc_lower:ACC_Transform_MultiForLoop."));
	}
	
   	WN_INSERT_BlockLast ( acc_parallel_func, IndexGenerationBlock );
	/* Transfer any mappings for nodes moved from parent to parallel function */

	ACC_Transfer_Maps ( acc_pmaptab, acc_cmaptab, acc_parallel_func, 
		  PU_Info_regions_ptr(Current_PU_Info) );

	/* Create a new dependence graph for the child  and move all the 
	 appropriate vertices from the parent to the child graph */

	Current_Map_Tab = acc_cmaptab;
	ACC_Fix_Dependence_Graph ( acc_ppuinfo, Current_PU_Info, acc_parallel_func ); 
	Current_Map_Tab = acc_pmaptab;

  
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );
}

/*Process and anlysis the loop during kernel region*/
static void ACC_Extract_ACC_LoopNest_Info( WN * tree )
{
  INT32      i;
  INT32      vsize;
  WN        *wn;
  WN        *wn1;
  WN        *wn2;
  WN        *cur_node;
  WN        *first_node;
  WN        *prev_node;
  WN        *next_node;
  WN        *pdo_node;
  WN        *chunk_wn = NULL;
  WN        *body_block;
  WN        *while_block;
  ST        *return_st;
  WN_OFFSET  return_ofst;
  PREG_NUM   rreg1, rreg2;
  BOOL       while_seen = FALSE;
  BOOL       is_acc;
  BOOL is_region;
  WN* acc_collapse;
  WN* acc_gang;
  WN* acc_worker;
  WN* acc_vector;
  BOOL isIndependent = FALSE;
  BOOL isSeq = FALSE;
  WN* acc_private;
  INT32 acc_loopprivate_count = 0;
  WN* acc_reduction;
  INT32 acc_loopreduction_count = 0;

  Is_True(acc_t == ACCP_LOOP_REGION, ("not inside a for loop"));
  cur_node = WN_first(WN_region_pragmas(tree));
  
  /*FmtAssert (cur_node &&
             WN_opcode(cur_node) == OPC_PRAGMA &&
             WN_pragma(cur_node) == WN_PRAGMA_ACC_LOOP_BEGIN,
             ("ACC LOOP pragma in kernel region: Unexpected first pragma node"));*/
  is_acc = WN_pragma_acc(cur_node);

  next_node = WN_next(cur_node);

  FOR_LOOP_INFO forloopinfo;
  BZERO ( &forloopinfo, sizeof(forloopinfo));
  //acc_loopinfo.acc_forloop
  while ((cur_node = next_node)) 
  {

    next_node = WN_next(cur_node);

    if (((WN_opcode(cur_node) == OPC_PRAGMA) ||
         (WN_opcode(cur_node) == OPC_XPRAGMA)) &&
        (WN_pragmas[WN_pragma(cur_node)].users & PUSER_ACC)) 
    {
		switch (WN_pragma(cur_node)) 
		{

		  case WN_PRAGMA_ACC_CLAUSE_COLLAPSE:
		    if (acc_collapse)
		      WN_DELETE_Tree ( acc_collapse );
		    acc_collapse = cur_node;
			forloopinfo.acc_collapse = acc_collapse;
		    break;

		  case WN_PRAGMA_ACC_CLAUSE_GANG:
		    //WN_DELETE_Tree ( cur_node );
			//forloopinfo.looptype = ACC_GANG;
			if(forloopinfo.looptype == ACC_VECTOR)
				forloopinfo.looptype = ACC_GANG_VECTOR;
			
			else if(forloopinfo.looptype == ACC_WORKER)				
				forloopinfo.looptype = ACC_GANG_WORKER;
			
			else if(forloopinfo.looptype == ACC_WORKER_VECTOR)				
				forloopinfo.looptype = ACC_GANG_WORKER_VECTOR;
			
			else
				forloopinfo.looptype = ACC_GANG;
			forloopinfo.gangs = cur_node;
		    break;

		  case WN_PRAGMA_ACC_CLAUSE_WORKER:
		    //WN_DELETE_Tree ( cur_node );		
			
			if(forloopinfo.looptype == ACC_VECTOR)				
				forloopinfo.looptype = ACC_WORKER_VECTOR;
			else if(forloopinfo.looptype == ACC_GANG)				
				forloopinfo.looptype = ACC_GANG_WORKER;
			else if(forloopinfo.looptype == ACC_GANG_VECTOR)				
				forloopinfo.looptype = ACC_GANG_WORKER_VECTOR;
			else
				forloopinfo.looptype = ACC_WORKER;
			
			forloopinfo.workers = cur_node;
		    break;

		  case WN_PRAGMA_ACC_CLAUSE_VECTOR:
		    //WN_DELETE_Tree ( cur_node );
			if(forloopinfo.looptype == ACC_GANG)
				forloopinfo.looptype = ACC_GANG_VECTOR;
			else if(forloopinfo.looptype == ACC_GANG_WORKER)				
				forloopinfo.looptype = ACC_GANG_WORKER_VECTOR;
			else if(forloopinfo.looptype == ACC_WORKER)				
				forloopinfo.looptype = ACC_WORKER_VECTOR;
			else
				forloopinfo.looptype = ACC_VECTOR;
			forloopinfo.vectors = cur_node;
		    break;

		  case WN_PRAGMA_ACC_CLAUSE_SEQ:
		    forloopinfo.isSeq = true;
		    WN_DELETE_Tree ( cur_node );
		    break;

		  case WN_PRAGMA_ACC_CLAUSE_INDEPENDENT:
		    forloopinfo.isIndependent = true;
		    WN_DELETE_Tree ( cur_node );	
		    break;

		  case WN_PRAGMA_ACC_CLAUSE_PRIVATE:
		    for (wn = forloopinfo.acc_private; wn; wn = WN_next(wn))
		      if (ACC_Identical_Pragmas(cur_node, wn))
				break;
			  
		    if (wn == NULL) 
			{
		      WN_next(cur_node) = forloopinfo.acc_private;
		      forloopinfo.acc_private = cur_node;
		      ++forloopinfo.private_count;
		    } 
			else
		      WN_Delete ( cur_node );
		    break;
			
		  case WN_PRAGMA_ACC_CLAUSE_REDUCTION:
		  	{
				if (WN_opcode(cur_node) != OPC_PRAGMA &&
	                WN_operator(WN_kid0(cur_node)) == OPR_ARRAY &&
	                OPCODE_has_sym(WN_opcode(WN_kid0(WN_kid0(cur_node)))) == 0) 
                {
                	WN_DELETE_Tree ( cur_node );
                } 
			    else 
				{
					ACC_ReductionMap acc_reductionMap;
					acc_reductionMap.acc_looplevel = (ACC_LOOP_LEVEL)acc_loopinfo.loopnum;
					acc_reductionMap.hostName = WN_st(cur_node);
					acc_reductionMap.ReductionOpr = (OPERATOR)WN_pragma_arg2(cur_node);
	                //WN_next(cur_node) = forloopinfo.acc_reduction;
	                forloopinfo.reductionmap.push_back(acc_reductionMap);
		        	++forloopinfo.reduction_count;
					acc_reduction_count ++;
              	}
		  	}
		    break;
			
		  default:
	         Fail_FmtAssertion ("out of context pragma (%s) in ACC {top-level pragma} processing",
	                             WN_pragmas[WN_pragma(cur_node)].name);

		}

  } 

 }
  
#ifdef KEY
  WN_DELETE_FromBlock (WN_region_pragmas(tree), cur_node);
#else
  WN_Delete ( cur_node );
#endif
  acc_collapse_count = 1;

  /*while (cur_node = next_node) 
  {

    next_node = WN_next(cur_node);
  }*/

  body_block = WN_region_body(tree);
  first_node = pdo_node = WN_first(body_block);

  
  if (pdo_node) 
  {
	  prev_node = WN_prev(pdo_node);//At this time, it's not necessary
	  if (prev_node) 
	  {
	        // add synchronization to sandwich code before PDO
	      WN *code_before_pdo = WN_CreateBlock();
	      WN_EXTRACT_ItemsFromBlock(body_block, first_node, prev_node);
	      WN_first(code_before_pdo) = first_node;
	      WN_last(code_before_pdo) = prev_node;
	      WN_INSERT_BlockBefore(body_block, pdo_node, code_before_pdo);
	      prev_node = WN_prev(pdo_node);
	  }
	  
	  WN *code_after_pdo = NULL;  // BLOCK for sandwich code after PDO (if any)
	  if (WN_next(pdo_node)) //Do something here, //At this time, it's not necessary
	  {
		  WN* next_node = WN_next(pdo_node);
		  WN* last_node = WN_last(body_block);
		  code_after_pdo = WN_CreateBlock();
		  WN_EXTRACT_ItemsFromBlock(body_block, next_node, last_node);
		  WN_first(code_after_pdo) = next_node;
		  WN_last(code_after_pdo) = last_node;
	  }
	  
	  WN_EXTRACT_FromBlock(body_block, pdo_node);

	  /* Determine user's real do index and type. */
	  ACC_Extract_Index_Info(pdo_node);
	  
	  forloopinfo.acc_index_st = acc_forloop_index_st[0];
	  forloopinfo.acc_index_type = acc_forloop_index_type[0];
	  
	  /* Translate do statement itself. */
	  ACC_Extract_Do_Info ( pdo_node );
  
	  forloopinfo.init = acc_base_node[0];
	  forloopinfo.condition = acc_limit_node[0];
	  forloopinfo.incr = acc_stride_node[0];
	  forloopinfo.acc_loopbody = acc_doloop_body;
	  forloopinfo.acc_test_stmt = acc_test_stmt;
	  forloopinfo.wn_regionbody = WN_COPY_Tree(pdo_node);

	  acc_loopinfo.acc_forloop.push_back(forloopinfo);
	  acc_loopinfo.loopnum ++;

	  //Traverse first.
	  int loopcount = 0;
	  int current_loop_level = acc_loopinfo.loopnum;
	  WN* wn_handlist = NULL, *wn_cur_node; 
	  //currently, only triple nested loop is supported.
	  if(acc_loopinfo.loopnum == 3)
	  	return;
	  //WN* wn_afterhandlist = NULL
	  for (cur_node = WN_first(acc_doloop_body); cur_node; cur_node = next_node) 
	  {

		prev_node = WN_prev(cur_node);
		next_node = WN_next(cur_node);
		//Process the loop region which is inside the kernel region
		if ((is_region = (WN_opcode(cur_node) == OPC_REGION &&
                             WN_first(WN_region_pragmas(cur_node)) &&
                             WN_opcode(WN_first(
			        WN_region_pragmas(cur_node))) == OPC_PRAGMA) ) &&
	       WN_pragma(WN_first(WN_region_pragmas(cur_node))) ==
						WN_PRAGMA_ACC_LOOP_BEGIN) 
			{	
				acc_loopinfo.acc_forloop[current_loop_level - 1].wn_prehand_nodes
						= wn_handlist;
				wn_handlist = NULL;
				loopcount ++;
				Is_True(loopcount == 1 && acc_loopinfo.loopnum <= 4, 
								("no multi ACC loop in one ACC LOOP"));
	  			ACC_Extract_ACC_LoopNest_Info(cur_node);
			}
			else if(WN_opcode(cur_node) != OPC_REGION_EXIT
					&& WN_opcode(cur_node) != OPC_LABEL)
			{
				if(wn_handlist == NULL)
				{
					//wn_cur_node = wn_handlist = WN_COPY_Tree(cur_node);
					wn_handlist = WN_CreateBlock();
					wn_cur_node = WN_COPY_Tree(cur_node);
					WN_next(wn_cur_node) = NULL;
					WN_prev(wn_cur_node) = NULL;
					WN_INSERT_BlockLast(wn_handlist, wn_cur_node);
					//WN_next(wn_handlist) = NULL;
					//WN_prev(wn_handlist) = NULL;
				}
				else
				{
					//wn_handlist = WN_COPY_Tree(cur_node);
					//WN_next(wn_cur_node) = WN_COPY_Tree(cur_node);
					//WN_prev(WN_next(wn_cur_node)) = wn_cur_node;
					//wn_cur_node = WN_next(wn_cur_node);
					//WN_next(wn_cur_node) = NULL;					
					wn_cur_node = WN_COPY_Tree(cur_node);
					WN_next(wn_cur_node) = NULL;
					WN_prev(wn_cur_node) = NULL;
					WN_INSERT_BlockLast(wn_handlist, wn_cur_node);
				}
			 }
	  }
	  
	  acc_loopinfo.acc_forloop[current_loop_level - 1].wn_afterhand_nodes
						= wn_handlist;
	  //ACC_Extract_ACC_LoopNest_Info(pdo_node);
	  //Here generate the kernel code
      //WN *return_wn = ACC_Transform_ForLoop( pdo_node);
	  //Begin outline the kernel function
	  /////////////////////////////////////////////////
	  //debug only
	  //fdump_tree(stdout, return_wn);
  }
}

/****************************************************************
Setup the number of GANG and VECTOR, or maybe worker in the future
*****************************************************************/
//Set Gangs
static WN* Gen_Set_Gangs_Num_X(WN* wn_num)
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 1 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SET_GANG_NUM_X);
  if(wn_num == NULL)
  	wn_num = WN_Intconst(MTYPE_I4, 0);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  WN_kid(wn, 0) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(wn_num), Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

static WN* Gen_Set_Gangs_Num_Z(WN* wn_num)
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 1 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SET_GANG_NUM_Z);
  if(wn_num == NULL)
  	wn_num = WN_Intconst(MTYPE_I4, 0);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  WN_kid(wn, 0) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(wn_num), Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}


static WN* Gen_Set_Gangs_Num_Y(WN* wn_num)
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 1 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SET_GANG_NUM_Y);
  if(wn_num == NULL)
  	wn_num = WN_Intconst(MTYPE_I4, 0);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  WN_kid(wn, 0) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(wn_num), Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

//Set vectors
static WN* Gen_Set_Vector_Num_X(WN* wn_num)
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 1 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SET_VECTOR_NUM_X);
  if(wn_num == NULL)
  	wn_num = WN_Intconst(MTYPE_I4, 0);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  WN_kid(wn, 0) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(wn_num), Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

static WN* Gen_Set_Vector_Num_Y(WN* wn_num)
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 1 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SET_VECTOR_NUM_Y);
  if(wn_num == NULL)
  	wn_num = WN_Intconst(MTYPE_I4, 0);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  WN_kid(wn, 0) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(wn_num), Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

static WN* Gen_Set_Vector_Num_Z(WN* wn_num)
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 1 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SET_VECTOR_NUM_Z);
  if(wn_num == NULL)
  	wn_num = WN_Intconst(MTYPE_I4, 0);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  WN_kid(wn, 0) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(wn_num), Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

static WN* Gen_Set_Vectors_Toplogy()
{
	return NULL;
}

static WN* Gen_Set_Default_Toplogy()
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 0 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SETUP_DEFAULT_TOLOGY);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  return wn;
}

static WN* Gen_Reset_Default_Toplogy()
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 0 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_RESET_DEFAULT_TOLOGY);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  return wn;
}


static void ACC_Setup_GPU_toplogy_parallel(ParallelRegionInfo* pPRInfo, WN* replace_block)
{
	WN* toplogy = Gen_Set_Default_Toplogy();
	WN_INSERT_BlockLast(replace_block, toplogy);
	WN* gangs_num = pPRInfo->acc_num_gangs;
	WN* workers_num = pPRInfo->acc_num_workers;
	WN* vectors_length = pPRInfo->acc_vector_length;
	WN* wnGangsNumExpr;
	WN* wnWorkersNumExpr;
	WN* wnVectorsNumExpr;
	if(gangs_num)
	{
		wnGangsNumExpr = WN_kid0(gangs_num);			
		if((WN_opcode(wnGangsNumExpr) == OPC_I4INTCONST 
						&& WN_const_val(wnGangsNumExpr) != 0) 
						|| (WN_opcode(wnGangsNumExpr) != OPC_I4INTCONST))
		{
			WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnGangsNumExpr);
			WN_INSERT_BlockLast(replace_block, wn_gangs_set);
		}
	}
	if(workers_num)
	{
		wnWorkersNumExpr = WN_kid0(workers_num);
		if((WN_opcode(wnWorkersNumExpr) == OPC_I4INTCONST 
						&& WN_const_val(wnWorkersNumExpr) != 0) 
						|| (WN_opcode(wnWorkersNumExpr) != OPC_I4INTCONST))
		{
			WN* wn_workers_set = Gen_Set_Vector_Num_Y(wnWorkersNumExpr);
			WN_INSERT_BlockLast(replace_block, wn_workers_set);
		}
	}
	if(vectors_length)
	{
		wnVectorsNumExpr = WN_kid0(vectors_length);
		if((WN_opcode(wnVectorsNumExpr) == OPC_I4INTCONST 
						&& WN_const_val(wnVectorsNumExpr) != 0) 
						|| (WN_opcode(wnVectorsNumExpr) != OPC_I4INTCONST))
		{
			WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnVectorsNumExpr);
			WN_INSERT_BlockLast(replace_block, wn_vectors_set);
		}
	}
}

static void ACC_Setup_GPU_toplogy_kernels(WN* replace_block)
{
	WN* toplogy = Gen_Set_Default_Toplogy();
	WN_INSERT_BlockLast(replace_block, toplogy);
	//for(WN* wn_reduction = acc_loopinfo.acc_forloop[0]
	//	WN* wnNumExpr = WN_kid0(wnArr);
	if(acc_loopinfo.loopnum == 1)
	{
		FOR_LOOP_INFO loopinfo = acc_loopinfo.acc_forloop[0];	
		WN* wnGangsNumExpr = NULL;
		WN* wnVectorsNumExpr = NULL;
		if(loopinfo.gangs)
		{
			wnGangsNumExpr = WN_kid0(loopinfo.gangs);			
			if((WN_opcode(wnGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnGangsNumExpr) != 0) 
							|| (WN_opcode(wnGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
		}
		if(loopinfo.vectors)
		{
			wnVectorsNumExpr = WN_kid0(loopinfo.vectors);
			if((WN_opcode(wnVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnVectorsNumExpr) != 0) 
							|| (WN_opcode(wnVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
	}
	else if(acc_loopinfo.loopnum == 2)
	{
		FOR_LOOP_INFO OuterLoopInfo = acc_loopinfo.acc_forloop[0];	
		WN* wnOuterGangsNumExpr = NULL;		
		WN* wnOuterVectorsNumExpr = NULL;
		if(OuterLoopInfo.gangs)
			wnOuterGangsNumExpr = WN_kid0(OuterLoopInfo.gangs);	
		if(OuterLoopInfo.vectors)	
			wnOuterVectorsNumExpr = WN_kid0(OuterLoopInfo.vectors);
		//////////////////////////////////////////////////////////
		WN* wnInnerGangsNumExpr = NULL;
		WN* wnInnerVectorsNumExpr = NULL;
		FOR_LOOP_INFO InnerLoopInfo = acc_loopinfo.acc_forloop[1];	
		if(InnerLoopInfo.gangs)
			wnInnerGangsNumExpr = WN_kid0(InnerLoopInfo.gangs);	
		if(InnerLoopInfo.vectors)	
			wnInnerVectorsNumExpr = WN_kid0(InnerLoopInfo.vectors);

		if(OuterLoopInfo.looptype == ACC_GANG && InnerLoopInfo.looptype == ACC_VECTOR)
		{
			if((WN_opcode(wnOuterGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterGangsNumExpr) != 0) 
							|| (WN_opcode(wnOuterGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnOuterGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		else if(OuterLoopInfo.looptype == ACC_GANG_VECTOR && InnerLoopInfo.looptype == ACC_VECTOR)
		{
			if((WN_opcode(wnOuterGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterGangsNumExpr) != 0) 
							|| (WN_opcode(wnOuterGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnOuterGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			if((WN_opcode(wnOuterVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterVectorsNumExpr) != 0) 
							|| (WN_opcode(wnOuterVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Y(wnOuterVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}

			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		else if(OuterLoopInfo.looptype == ACC_GANG && InnerLoopInfo.looptype == ACC_GANG_VECTOR)
		{
			if((WN_opcode(wnOuterGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterGangsNumExpr) != 0) 
							|| (WN_opcode(wnOuterGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Y(wnOuterGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			
			if((WN_opcode(wnInnerGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerGangsNumExpr) != 0) 
							|| (WN_opcode(wnInnerGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnInnerGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}

			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		else if(OuterLoopInfo.looptype == ACC_GANG_VECTOR && InnerLoopInfo.looptype == ACC_GANG_VECTOR)
		{
			if((WN_opcode(wnOuterGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterGangsNumExpr) != 0) 
							|| (WN_opcode(wnOuterGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Y(wnOuterGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			
			if((WN_opcode(wnInnerGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerGangsNumExpr) != 0) 
							|| (WN_opcode(wnInnerGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnInnerGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}

			if((WN_opcode(wnOuterVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterVectorsNumExpr) != 0) 
							|| (WN_opcode(wnOuterVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Y(wnOuterVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
			
			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		/*else
		{
			//crash&report error message
			Is_True(FALSE, ("2 Level of Loop Combination is wrong@acc_lower:ACC_Transform_MultiForLoop."));
		}*/
		
	}
	else if(acc_loopinfo.loopnum == 3)
	{
		WN* wnOuterGangsNumExpr = NULL;		
		WN* wnOuterVectorsNumExpr = NULL;
		
		WN* wnMidGangsNumExpr = NULL;		
		WN* wnMidVectorsNumExpr = NULL;
		
		WN* wnInnerGangsNumExpr = NULL;		
		WN* wnInnerVectorsNumExpr = NULL;
		
		FOR_LOOP_INFO OuterLoopInfo = acc_loopinfo.acc_forloop[0];
		FOR_LOOP_INFO MidLoopInfo = acc_loopinfo.acc_forloop[1];
		FOR_LOOP_INFO InnerLoopInfo = acc_loopinfo.acc_forloop[2];
		
		if(OuterLoopInfo.gangs)
			wnOuterGangsNumExpr = WN_kid0(OuterLoopInfo.gangs);	
		if(OuterLoopInfo.vectors)	
			wnOuterVectorsNumExpr = WN_kid0(OuterLoopInfo.vectors);
		///////////////////////////////////////////////////////////////////////////////
		if(MidLoopInfo.gangs)
			wnMidGangsNumExpr = WN_kid0(MidLoopInfo.gangs);	
		if(MidLoopInfo.vectors)	
			wnMidVectorsNumExpr = WN_kid0(MidLoopInfo.vectors);
		//////////////////////////////////////////////////////////////////////////////
		if(InnerLoopInfo.gangs)
			wnInnerGangsNumExpr = WN_kid0(InnerLoopInfo.gangs);	
		if(InnerLoopInfo.vectors)	
			wnInnerVectorsNumExpr = WN_kid0(InnerLoopInfo.vectors);

		if(OuterLoopInfo.looptype == ACC_VECTOR && MidLoopInfo.looptype == ACC_GANG_VECTOR
					&& InnerLoopInfo.looptype == ACC_GANG_VECTOR)
		{
			if((WN_opcode(wnMidGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnMidGangsNumExpr) != 0) 
							|| (WN_opcode(wnMidGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Y(wnMidGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			
			if((WN_opcode(wnInnerGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerGangsNumExpr) != 0) 
							|| (WN_opcode(wnInnerGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnInnerGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}

			if((WN_opcode(wnOuterVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterVectorsNumExpr) != 0) 
							|| (WN_opcode(wnOuterVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Z(wnOuterVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
			
			if((WN_opcode(wnMidVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnMidVectorsNumExpr) != 0) 
							|| (WN_opcode(wnMidVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Y(wnMidVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
			
			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		else if(OuterLoopInfo.looptype == ACC_GANG && MidLoopInfo.looptype == ACC_GANG_VECTOR
					&& InnerLoopInfo.looptype == ACC_VECTOR)
		{
			if((WN_opcode(wnOuterGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterGangsNumExpr) != 0) 
							|| (WN_opcode(wnOuterGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnOuterGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			
			if((WN_opcode(wnMidGangsNumExpr) == OPC_I4INTCONST
							&& WN_const_val(wnMidGangsNumExpr) != 0) 
							|| (WN_opcode(wnMidGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Y(wnMidGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}

			if((WN_opcode(wnMidVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnMidVectorsNumExpr) != 0) 
							|| (WN_opcode(wnMidVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Y(wnMidVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
			
			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		else if(OuterLoopInfo.looptype == ACC_VECTOR && MidLoopInfo.looptype == ACC_GANG_VECTOR
					&& InnerLoopInfo.looptype == ACC_GANG)
		{
			if((WN_opcode(wnMidGangsNumExpr) == OPC_I4INTCONST
							&& WN_const_val(wnMidGangsNumExpr) != 0) 
							|| (WN_opcode(wnMidGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Y(wnMidGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}

			
			if((WN_opcode(wnInnerGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerGangsNumExpr) != 0) 
							|| (WN_opcode(wnInnerGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnInnerGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			if((WN_opcode(wnOuterVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterVectorsNumExpr) != 0) 
							|| (WN_opcode(wnOuterVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnOuterVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
			
			if((WN_opcode(wnMidVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnMidVectorsNumExpr) != 0) 
							|| (WN_opcode(wnMidVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Y(wnMidVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		else if(OuterLoopInfo.looptype == ACC_GANG_VECTOR && MidLoopInfo.looptype == ACC_GANG_VECTOR
					&& InnerLoopInfo.looptype == ACC_GANG_VECTOR)
		{
			//set gangs
			if((WN_opcode(wnOuterGangsNumExpr) == OPC_I4INTCONST
							&& WN_const_val(wnOuterGangsNumExpr) != 0) 
							|| (WN_opcode(wnOuterGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Z(wnOuterGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			
			if((WN_opcode(wnMidGangsNumExpr) == OPC_I4INTCONST
							&& WN_const_val(wnMidGangsNumExpr) != 0) 
							|| (WN_opcode(wnMidGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Y(wnMidGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}

			
			if((WN_opcode(wnInnerGangsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerGangsNumExpr) != 0) 
							|| (WN_opcode(wnInnerGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_X(wnInnerGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			//set vectors
			if((WN_opcode(wnOuterVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnOuterVectorsNumExpr) != 0) 
							|| (WN_opcode(wnOuterVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Z(wnOuterVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}			
			if((WN_opcode(wnMidVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnMidVectorsNumExpr) != 0) 
							|| (WN_opcode(wnMidVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_Y(wnMidVectorsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
			
			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		else if(OuterLoopInfo.looptype == ACC_GANG && MidLoopInfo.looptype == ACC_GANG
					&& InnerLoopInfo.looptype == ACC_VECTOR)
		{
			//set gangs
			if((WN_opcode(wnOuterGangsNumExpr) == OPC_I4INTCONST
							&& WN_const_val(wnOuterGangsNumExpr) != 0) 
							|| (WN_opcode(wnOuterGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Z(wnOuterGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}
			
			if((WN_opcode(wnMidGangsNumExpr) == OPC_I4INTCONST
							&& WN_const_val(wnMidGangsNumExpr) != 0) 
							|| (WN_opcode(wnMidGangsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_gangs_set = Gen_Set_Gangs_Num_Y(wnMidGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_gangs_set);
			}			
			
			if((WN_opcode(wnInnerVectorsNumExpr) == OPC_I4INTCONST 
							&& WN_const_val(wnInnerVectorsNumExpr) != 0) 
							|| (WN_opcode(wnInnerVectorsNumExpr) != OPC_I4INTCONST))
			{
				WN* wn_vectors_set = Gen_Set_Vector_Num_X(wnInnerGangsNumExpr);
				WN_INSERT_BlockLast(replace_block, wn_vectors_set);
			}
		}
		/*else
		{
			//crash&report error message
			Is_True(FALSE, ("3 Level of Loop Combination is wrong@acc_lower:ACC_Setup_GPU_toplogy_kernelsv."));
		}*/
	}
}

static WN* ACC_GenGetTotalNumVectors()
{
	WN * wn;
	wn = WN_Create(OPC_I4CALL, 0 );
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_GET_TOTAL_VECTORS);

	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
	WN_linenum(wn) = acc_line_number;

	return wn;
}

static WN* ACC_GenGetTotalNumGangs()
{
	WN * wn;
	wn = WN_Create(OPC_I4CALL, 0 );
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_GET_TOTAL_GANGS);

	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
	WN_linenum(wn) = acc_line_number;

	return wn;
}

static WN* ACC_GenGetTotalNumGangsWorkers()
{
	WN * wn;
	wn = WN_Create(OPC_I4CALL, 0 );
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_GET_TOTAL_GANGS_WORKERS);

	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
	WN_linenum(wn) = acc_line_number;

	return wn;
}


static BOOL ACC_Locate_Reduction(WN* tree, ST* st_rd, OPERATOR oprRD)
{
  OPCODE op;
  OPERATOR opr;
  INT32 i;
  WN *r;
  WN *temp;
  ST *old_sym;
  WN_OFFSET old_offset;
  ACC_VAR_TABLE *w;
  BOOL bFound = FALSE;

  /* Ignore NULL subtrees. */

  if (tree == NULL)
    return FALSE;

  /* Initialization. */

  op = WN_opcode(tree);
  opr = OPCODE_operator(op);

  /* Look for and replace any nodes referencing localized symbols */
  if (opr == OPR_STID)
  {
    old_sym = WN_st(tree);
	WN* Kid0 = WN_kid0(tree);
	if(WN_kid_count(Kid0) == 2)
	{	
		ST* st_Kid0 = WN_has_sym(WN_kid0(Kid0))? WN_st(WN_kid0(Kid0)) : NULL;
		ST* st_Kid1 = WN_has_sym(WN_kid1(Kid0))? WN_st(WN_kid1(Kid0)) : NULL;
		OPCODE opc_kid0 = WN_opcode(Kid0);
		OPERATOR opr_kid0 = OPCODE_operator(opc_kid0);
		if(st_rd == old_sym && opr_kid0 == oprRD && ((st_Kid0==st_rd)|| (st_Kid1 == st_rd)))
			return TRUE;
	}
  } 
  /* Walk all children */
  if (op == OPC_BLOCK) 
  {
    r = WN_first(tree);
    while (r) 
	{ // localize each node in block
      bFound = ACC_Locate_Reduction ( r, st_rd, oprRD);
	  if(bFound)
		return bFound;
      r = WN_next(r);
      
   }
  }
  else 
  {
    for (i=0; i < WN_kid_count(tree); i++)
    {
      bFound = ACC_Locate_Reduction ( WN_kid(tree, i), st_rd, oprRD);
	  if(bFound)
		return bFound;
    }
  }
  return FALSE;
}

static void ACC_ProcessReduction_Parallel(ParallelRegionInfo* pPRInfo, WN* wn_replace_block)
{	
	int i;
	//Process reduction
	i=0;
	while(i < acc_parallel_loop_info.loopnum)
	{
		acc_loopinfo = acc_parallel_loop_info.acc_loopinfo[i];
		/***************************************************************************/
		/*************************Check the reduction here*****************************/
		/***************************************************************************/
		int iRdIdx;
		//Only need to take care the top level reduction. Reduction operation in a single block will be handled
		//in ACC_Transform_SingleForLoop. 
		if(acc_loopinfo.loopnum == 3)
		{
			iRdIdx = 0;
			while(iRdIdx<acc_loopinfo.acc_forloop[0].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[0].reductionmap[iRdIdx];
				ST *old_st = reductionmap.hostName;
				//Locate Reduction stmt
				FOR_LOOP_INFO* pForInfo0 = &acc_loopinfo.acc_forloop[0];
				FOR_LOOP_INFO* pForInfo1 = &acc_loopinfo.acc_forloop[1];
				FOR_LOOP_INFO* pForInfo2 = &acc_loopinfo.acc_forloop[2];
				BOOL isLocated = FALSE;			
				reductionmap.looptype = acc_loopinfo.acc_forloop[0].looptype;
				//ACC_Locate_Reduction(&acc_loopinfo.acc_forloop[0]);
				//locate where is the reduction statement.
				//Scan outter loop body
				if(pForInfo0->wn_prehand_nodes)
				{
					isLocated = ACC_Locate_Reduction(pForInfo0->wn_prehand_nodes, 
						old_st, reductionmap.ReductionOpr);
				}
				
				if(!isLocated && pForInfo0->wn_afterhand_nodes)
				{
					isLocated = ACC_Locate_Reduction(pForInfo0->wn_afterhand_nodes, 
						old_st, reductionmap.ReductionOpr);
				}
				//Find it, then set location, else keeep searching
				if(isLocated)
					reductionmap.acc_stmt_location = ACC_OUTTER_LOOP;
				else
				{
					//Scan inner loop body
					if(pForInfo1->wn_prehand_nodes)
					{
						isLocated = ACC_Locate_Reduction(pForInfo1->wn_prehand_nodes, 
							old_st, reductionmap.ReductionOpr);
					}
					
					if(!isLocated && pForInfo1->wn_afterhand_nodes)
					{
						isLocated = ACC_Locate_Reduction(pForInfo1->wn_afterhand_nodes, 
							old_st, reductionmap.ReductionOpr);
					}
					if(isLocated)
						reductionmap.acc_stmt_location = ACC_MIDDER_LOOP;
					else 
						reductionmap.acc_stmt_location = ACC_INNER_LOOP;
				}
				/////////////////////////////////////////////////////////////////////////				
				ST* st_num_vectors;
				PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */;
				if(reductionmap.acc_stmt_location == ACC_INNER_LOOP)
					WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumVectors());
				else if(reductionmap.acc_stmt_location == ACC_MIDDER_LOOP)//count workers
					WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumGangsWorkers());
				else if(reductionmap.acc_stmt_location == ACC_OUTTER_LOOP)//count gangs
					//each worker carries one buffer unit
					WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumGangs());
				/////////////////////////////////////////////////////////////////////////////////////////////
				ACC_Host_Create_Preg_or_Temp(MTYPE_I4, "_total_num_of_vectors", &st_num_vectors);
				ACC_GET_RETURN_PREGS(rreg1, rreg2, MTYPE_I4);
				WN* wn_return = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), rreg1, Return_Val_Preg, ST_type(st_num_vectors));
				WN* temp_node = WN_Stid(TY_mtype(ST_type(st_num_vectors)), 0, 
												st_num_vectors, ST_type(st_num_vectors), wn_return);
				WN_INSERT_BlockLast( wn_replace_block, temp_node);

				/////////////////////////////////////////////////////////////
				WN* vector_num = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), 
							0, st_num_vectors, ST_type(st_num_vectors));

				///////////////////////////////////////////////////////////////////////
				WN* alloc_size = WN_Binary(OPR_MPY, MTYPE_I4, WN_COPY_Tree(vector_num),
						WN_Intconst(MTYPE_I4, TY_size(ST_type(reductionmap.hostName))));
				//malloc device addr
							
				//reductionmap	
				//ST *old_st = reductionmap.hostName;	
			    TY_IDX ty = ST_type(old_st);
			    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			    char* localname = (char *) alloca(strlen(ST_name(old_st))+128);
				sprintf ( localname, "__device_reduction_%s", ST_name(old_st) );
				TY_IDX ty_p = Make_Pointer_Type(ty);
				ST *st_device = NULL;
				//WN *device_addr = NULL;
				st_device = New_ST( CURRENT_SYMTAB );
				ST_Init(st_device,
						Save_Str( localname ),
						CLASS_VAR,
						SCLASS_AUTO,
						EXPORT_LOCAL,
						ty_p);		
				reductionmap.deviceName = st_device;
				reductionmap.st_num_of_element = st_num_vectors;
				//call the acc malloc
				WN_INSERT_BlockLast( wn_replace_block, GenReductionMalloc(st_device, alloc_size));				
		
				reductionmap.reduction_kenels = ACC_GenerateReduction_Kernels_TopLoop(&reductionmap);
				acc_loopinfo.acc_forloop[0].reductionmap[iRdIdx] = reductionmap;
				acc_reduction_tab_map[reductionmap.hostName] = reductionmap;
				iRdIdx ++;
			}
			//check the Midder loop reduction
			iRdIdx = 0;
			while(iRdIdx<acc_loopinfo.acc_forloop[1].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[1].reductionmap[iRdIdx];
				ST *old_st = reductionmap.hostName;
				TY_IDX ty = ST_type(old_st);
				TYPE_ID mtypeID = TY_mtype(ty);
				reductionmap.looptype = acc_loopinfo.acc_forloop[1].looptype;
				//Locate Reduction stmt
				FOR_LOOP_INFO* pForInfo1 = &acc_loopinfo.acc_forloop[1];
				FOR_LOOP_INFO* pForInfo2 = &acc_loopinfo.acc_forloop[2];
				BOOL isLocated = FALSE;
				//ACC_Locate_Reduction(&acc_loopinfo.acc_forloop[1]);
				//locate where is the reduction statement.
				//Scan Midder loop body
				if(pForInfo1->wn_prehand_nodes)
				{
					isLocated = ACC_Locate_Reduction(pForInfo1->wn_prehand_nodes, 
						old_st, reductionmap.ReductionOpr);
				}
				
				if(!isLocated && pForInfo1->wn_afterhand_nodes)
				{
					isLocated = ACC_Locate_Reduction(pForInfo1->wn_afterhand_nodes, 
						old_st, reductionmap.ReductionOpr);
				}
				if(isLocated)
					reductionmap.acc_stmt_location = ACC_MIDDER_LOOP;
				else 
					reductionmap.acc_stmt_location = ACC_INNER_LOOP;
				//if it is using shared memory on device, 
				//we do not have to create anything on host side
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					ST* st_reduction_buffer = acc_global_memory_for_reduction_host[mtypeID];

					if(!st_reduction_buffer)
					//////////////////////////////////////////////////////////////////
					{
						ST* st_num_vectors;
						PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */;
						WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumVectors());
						////////////////////////////////////////////////////////////////////////////////////
						ACC_Host_Create_Preg_or_Temp(MTYPE_I4, "_total_num_of_vectors", &st_num_vectors);
						ACC_GET_RETURN_PREGS(rreg1, rreg2, MTYPE_I4);
						WN* wn_return = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), rreg1, Return_Val_Preg, ST_type(st_num_vectors));
						WN* temp_node = WN_Stid(TY_mtype(ST_type(st_num_vectors)), 0, 
														st_num_vectors, ST_type(st_num_vectors), wn_return);
						WN_INSERT_BlockLast( wn_replace_block, temp_node);

						/////////////////////////////////////////////////////////////
						WN* vector_num = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), 
									0, st_num_vectors, ST_type(st_num_vectors));

						///////////////////////////////////////////////////////////////////////
						WN* alloc_size = WN_Binary(OPR_MPY, MTYPE_I4, WN_COPY_Tree(vector_num),
								WN_Intconst(MTYPE_I4, TY_size(ST_type(reductionmap.hostName))));
						//malloc device addr
									
						//reductionmap	
						//ST *old_st = reductionmap.hostName;	
					    TY_IDX ty = ST_type(old_st);
					    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
					    char* localname = (char *) alloca(strlen(ST_name(old_st))+128);
						sprintf ( localname, "__device_reduction_%s", ACC_Get_ScalarName_of_Reduction(mtypeID) );
						TY_IDX ty_p = Make_Pointer_Type(ty);
						ST *st_device = NULL;
						//WN *device_addr = NULL;
						st_device = New_ST( CURRENT_SYMTAB );
						ST_Init(st_device,
								Save_Str( localname ),
								CLASS_VAR,
								SCLASS_AUTO,
								EXPORT_LOCAL,
								ty_p);		
						reductionmap.deviceName = st_device;
						//call the acc malloc
						WN_INSERT_BlockLast( wn_replace_block, GenReductionMalloc(st_device, alloc_size));				
						acc_global_memory_for_reduction_host[mtypeID] = st_device;
					}
					else
						reductionmap.deviceName = st_reduction_buffer;
				}
				if(reductionmap.acc_stmt_location == ACC_MIDDER_LOOP)
				{
					if(acc_reduction_rolling == ACC_RD_UNROLLING)
						reductionmap.reduction_kenels = ACC_GenerateWorkerReduction_unrolling(&reductionmap);
					else
						reductionmap.reduction_kenels = ACC_GenerateWorkerReduction_rolling(&reductionmap);
				}
				else if(reductionmap.acc_stmt_location == ACC_INNER_LOOP)
				{
					if(acc_reduction_rolling == ACC_RD_UNROLLING)
						reductionmap.reduction_kenels = ACC_GenerateWorkerVectorReduction_unrolling(&reductionmap);
					else
						reductionmap.reduction_kenels = ACC_GenerateWorkerVectorReduction_rolling(&reductionmap);
				}
				acc_loopinfo.acc_forloop[1].reductionmap[iRdIdx] = reductionmap;
				acc_reduction_tab_map[reductionmap.hostName] = reductionmap;
				iRdIdx ++;
			}
			iRdIdx = 0;
			while(iRdIdx<acc_loopinfo.acc_forloop[2].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[2].reductionmap[iRdIdx];
				ST *old_st = reductionmap.hostName;
				TY_IDX ty = ST_type(old_st);
				TYPE_ID mtypeID = TY_mtype(ty);
				reductionmap.looptype = acc_loopinfo.acc_forloop[2].looptype;
					
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					ST* st_reduction_buffer = acc_global_memory_for_reduction_host[mtypeID];
					//////////////////////////////////////////////////////////////////
					if(!st_reduction_buffer)			
					{
						ST* st_num_vectors;
						PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */;
						WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumVectors());
						////////////////////////////////////////////////////////////////////////////////////
						ACC_Host_Create_Preg_or_Temp(MTYPE_I4, "_total_num_of_vectors", &st_num_vectors);
						ACC_GET_RETURN_PREGS(rreg1, rreg2, MTYPE_I4);
						WN* wn_return = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), rreg1, Return_Val_Preg, ST_type(st_num_vectors));
						WN* temp_node = WN_Stid(TY_mtype(ST_type(st_num_vectors)), 0, 
														st_num_vectors, ST_type(st_num_vectors), wn_return);
						WN_INSERT_BlockLast( wn_replace_block, temp_node);

						/////////////////////////////////////////////////////////////
						WN* vector_num = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), 
									0, st_num_vectors, ST_type(st_num_vectors));

						///////////////////////////////////////////////////////////////////////
						WN* alloc_size = WN_Binary(OPR_MPY, MTYPE_I4, WN_COPY_Tree(vector_num),
								WN_Intconst(MTYPE_I4, TY_size(ST_type(reductionmap.hostName))));
						//malloc device addr
									
						//reductionmap	
						//ST *old_st = reductionmap.hostName;	
					    TY_IDX ty = ST_type(old_st);
					    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
					    char* localname = (char *) alloca(strlen(ST_name(old_st))+128);
						sprintf ( localname, "__device_reduction_%s", ACC_Get_ScalarName_of_Reduction(mtypeID) );
						TY_IDX ty_p = Make_Pointer_Type(ty);
						ST *st_device = NULL;
						//WN *device_addr = NULL;
						st_device = New_ST( CURRENT_SYMTAB );
						ST_Init(st_device,
								Save_Str( localname ),
								CLASS_VAR,
								SCLASS_AUTO,
								EXPORT_LOCAL,
								ty_p);		
						reductionmap.deviceName = st_device;
						//call the acc malloc
						WN_INSERT_BlockLast( wn_replace_block, GenReductionMalloc(st_device, alloc_size));				
						acc_global_memory_for_reduction_host[mtypeID] = st_device;
					}
					else
						reductionmap.deviceName = st_reduction_buffer;
				}
				if(acc_reduction_rolling == ACC_RD_UNROLLING)
					reductionmap.reduction_kenels = ACC_GenerateVectorReduction_unrolling(&reductionmap);
				else
					reductionmap.reduction_kenels = ACC_GenerateVectorReduction_rolling(&reductionmap);
				acc_loopinfo.acc_forloop[2].reductionmap[iRdIdx] = reductionmap;
				acc_reduction_tab_map[reductionmap.hostName] = reductionmap;
				iRdIdx ++;
			}
		}
		else if(acc_loopinfo.loopnum == 2
				&& acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			iRdIdx = 0;
			while(iRdIdx<acc_loopinfo.acc_forloop[0].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[0].reductionmap[iRdIdx];
				ST *old_st = reductionmap.hostName;
				//Locate Reduction stmt
				FOR_LOOP_INFO* pForInfo0 = &acc_loopinfo.acc_forloop[0];
				FOR_LOOP_INFO* pForInfo1 = &acc_loopinfo.acc_forloop[1];
				BOOL isLocated = FALSE;
				reductionmap.looptype = pForInfo0->looptype;
				//ACC_Locate_Reduction(&acc_loopinfo.acc_forloop[0]);
				if(pForInfo0->wn_prehand_nodes)
				{
					isLocated = ACC_Locate_Reduction(pForInfo0->wn_prehand_nodes, 
						old_st, reductionmap.ReductionOpr);
				}
				
				if(!isLocated && pForInfo0->wn_afterhand_nodes)
				{
					isLocated = ACC_Locate_Reduction(pForInfo0->wn_afterhand_nodes, 
						old_st, reductionmap.ReductionOpr);
				}
				if(isLocated)
					reductionmap.acc_stmt_location = ACC_OUTTER_LOOP;
				else 
					reductionmap.acc_stmt_location = ACC_INNER_LOOP;
				
				ST* st_num_vectors;
				PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */;
				if(reductionmap.acc_stmt_location == ACC_INNER_LOOP)
				{				
					//call is present function to check whether it has already been created.
					//each thread carries one buffer unit
					WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumVectors());
					
				}
				else //if(reductionmap.acc_stmt_location == ACC_OUTTER_LOOP)
				{				
					if(pForInfo0->looptype == ACC_GANG)
						//each gang carries one buffer unit
						WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumGangs());
					else if(pForInfo0->looptype == ACC_GANG_WORKER)
						//each worker carries one buffer unit
						WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumGangsWorkers());
				}
				ACC_Host_Create_Preg_or_Temp(MTYPE_I4, "_total_num_of_vectors", &st_num_vectors);
				ACC_GET_RETURN_PREGS(rreg1, rreg2, MTYPE_I4);
				WN* wn_return = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), rreg1, Return_Val_Preg, ST_type(st_num_vectors));
				WN* temp_node = WN_Stid(TY_mtype(ST_type(st_num_vectors)), 0, 
												st_num_vectors, ST_type(st_num_vectors), wn_return);
				WN_INSERT_BlockLast( wn_replace_block, temp_node);

				/////////////////////////////////////////////////////////////
				WN* vector_num = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), 
							0, st_num_vectors, ST_type(st_num_vectors));

				///////////////////////////////////////////////////////////////////////
				WN* alloc_size = WN_Binary(OPR_MPY, MTYPE_I4, WN_COPY_Tree(vector_num),
						WN_Intconst(MTYPE_I4, TY_size(ST_type(reductionmap.hostName))));
				//malloc device addr
							
				//reductionmap	
				//ST *old_st = reductionmap.hostName;	
			    TY_IDX ty = ST_type(old_st);
			    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			    char* localname = (char *) alloca(strlen(ST_name(old_st))+128);
				sprintf ( localname, "__device_reduction_%s", ST_name(old_st) );
				TY_IDX ty_p = Make_Pointer_Type(ty);
				ST *st_device = NULL;
				//WN *device_addr = NULL;
				st_device = New_ST( CURRENT_SYMTAB );
				ST_Init(st_device,
						Save_Str( localname ),
						CLASS_VAR,
						SCLASS_AUTO,
						EXPORT_LOCAL,
						ty_p);		
				reductionmap.deviceName = st_device;
				reductionmap.st_num_of_element = st_num_vectors;
				//call the acc malloc
				WN_INSERT_BlockLast( wn_replace_block, GenReductionMalloc(st_device, alloc_size));				
		
				reductionmap.reduction_kenels = ACC_GenerateReduction_Kernels_TopLoop(&reductionmap);
				acc_loopinfo.acc_forloop[0].reductionmap[iRdIdx] = reductionmap;				
				acc_reduction_tab_map[reductionmap.hostName] = reductionmap;
				iRdIdx ++;
			}
			iRdIdx = 0;
			while(iRdIdx<acc_loopinfo.acc_forloop[1].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[1].reductionmap[iRdIdx];
				ST *old_st = reductionmap.hostName;
				TY_IDX ty = ST_type(old_st);
				TYPE_ID mtypeID = TY_mtype(ty);
				reductionmap.looptype = acc_loopinfo.acc_forloop[1].looptype;

				
				if(acc_reduction_mem == ACC_RD_GLOBAL_MEM)
				{
					ST* st_reduction_buffer = acc_global_memory_for_reduction_host[mtypeID];

					if(!st_reduction_buffer)
					//////////////////////////////////////////////////////////////////
					{
						ST* st_num_vectors;
						PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */;
						WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumVectors());
						////////////////////////////////////////////////////////////////////////////////////
						ACC_Host_Create_Preg_or_Temp(MTYPE_I4, "_total_num_of_vectors", &st_num_vectors);
						ACC_GET_RETURN_PREGS(rreg1, rreg2, MTYPE_I4);
						WN* wn_return = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), rreg1, Return_Val_Preg, ST_type(st_num_vectors));
						WN* temp_node = WN_Stid(TY_mtype(ST_type(st_num_vectors)), 0, 
														st_num_vectors, ST_type(st_num_vectors), wn_return);
						WN_INSERT_BlockLast( wn_replace_block, temp_node);

						/////////////////////////////////////////////////////////////
						WN* vector_num = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), 
									0, st_num_vectors, ST_type(st_num_vectors));

						///////////////////////////////////////////////////////////////////////
						WN* alloc_size = WN_Binary(OPR_MPY, MTYPE_I4, WN_COPY_Tree(vector_num),
								WN_Intconst(MTYPE_I4, TY_size(ST_type(reductionmap.hostName))));
						//malloc device addr
									
						//reductionmap	
						//ST *old_st = reductionmap.hostName;	
					    TY_IDX ty = ST_type(old_st);
					    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
					    char* localname = (char *) alloca(strlen(ST_name(old_st))+128);
						sprintf ( localname, "__device_reduction_%s", ACC_Get_ScalarName_of_Reduction(mtypeID) );
						TY_IDX ty_p = Make_Pointer_Type(ty);
						ST *st_device = NULL;
						//WN *device_addr = NULL;
						st_device = New_ST( CURRENT_SYMTAB );
						ST_Init(st_device,
								Save_Str( localname ),
								CLASS_VAR,
								SCLASS_AUTO,
								EXPORT_LOCAL,
								ty_p);		
						reductionmap.deviceName = st_device;
						//call the acc malloc
						WN_INSERT_BlockLast( wn_replace_block, GenReductionMalloc(st_device, alloc_size));				
						acc_global_memory_for_reduction_host[mtypeID] = st_device;
					}
					else
						reductionmap.deviceName = st_reduction_buffer;
				}
				//////////////////////////////////////////////////////////////////////////
				
				if(acc_reduction_rolling == ACC_RD_UNROLLING)
				{
					if(reductionmap.looptype == ACC_VECTOR)
						reductionmap.reduction_kenels = ACC_GenerateVectorReduction_unrolling(&reductionmap);
					else if(reductionmap.looptype == ACC_WORKER_VECTOR)
						reductionmap.reduction_kenels = ACC_GenerateWorkerVectorReduction_unrolling(&reductionmap);
				}
				else
				{
					if(reductionmap.looptype == ACC_VECTOR)
						reductionmap.reduction_kenels = ACC_GenerateVectorReduction_rolling(&reductionmap);
					else if(reductionmap.looptype == ACC_WORKER_VECTOR)
						reductionmap.reduction_kenels = ACC_GenerateWorkerVectorReduction_rolling(&reductionmap);
				}
				///
				acc_loopinfo.acc_forloop[1].reductionmap[iRdIdx] = reductionmap;				
				acc_reduction_tab_map[reductionmap.hostName] = reductionmap;
				iRdIdx ++;
			}
		}
		else if(acc_loopinfo.loopnum == 1 
				&& acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			iRdIdx = 0;
			while(iRdIdx<acc_loopinfo.acc_forloop[0].reductionmap.size())
			{
				//if(acc_loopinfo.acc_forloop[0].)			
				PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */;
				ST* st_num_vectors;
				//call is present function to check whether it has already been created.
				WN_INSERT_BlockLast( wn_replace_block, ACC_GenGetTotalNumVectors());
				ACC_Host_Create_Preg_or_Temp(MTYPE_I4, "_total_num_of_vectors", &st_num_vectors);
				ACC_GET_RETURN_PREGS(rreg1, rreg2, MTYPE_I4);
				WN* wn_return = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), rreg1, Return_Val_Preg, ST_type(st_num_vectors));
				WN* temp_node = WN_Stid(TY_mtype(ST_type(st_num_vectors)), 0, 
												st_num_vectors, ST_type(st_num_vectors), wn_return);
				WN_INSERT_BlockLast( wn_replace_block, temp_node);

				WN* vector_num = WN_Ldid(TY_mtype(ST_type(st_num_vectors)), 
						0, st_num_vectors, ST_type(st_num_vectors));

				ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[0].reductionmap[iRdIdx];
				WN* alloc_size = WN_Binary(OPR_MPY, MTYPE_I4, WN_COPY_Tree(vector_num),
							WN_Intconst(MTYPE_I4, TY_size(ST_type(reductionmap.hostName))));
				//malloc device addr
							
				//reductionmap	
				ST *old_st = reductionmap.hostName;	
			    TY_IDX ty = ST_type(old_st);
			    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			    char* localname = (char *) alloca(strlen(ST_name(old_st))+128);
				sprintf ( localname, "__device_reduction_%s", ST_name(old_st) );
				TY_IDX ty_p = Make_Pointer_Type(ty);
				ST *st_device = NULL;
				//WN *device_addr = NULL;
				st_device = New_ST( CURRENT_SYMTAB );
				ST_Init(st_device,
						Save_Str( localname ),
						CLASS_VAR,
						SCLASS_AUTO,
						EXPORT_LOCAL,
						ty_p);		
				reductionmap.deviceName = st_device;
				reductionmap.st_num_of_element = st_num_vectors;
				//call the acc malloc
				WN_INSERT_BlockLast( wn_replace_block, GenReductionMalloc(st_device, alloc_size));				
				reductionmap.looptype = acc_loopinfo.acc_forloop[0].looptype;
				reductionmap.reduction_kenels = ACC_GenerateReduction_Kernels_TopLoop(&reductionmap);
				acc_loopinfo.acc_forloop[0].reductionmap[iRdIdx] = reductionmap;				
				acc_reduction_tab_map[reductionmap.hostName] = reductionmap;
				iRdIdx ++;
			}			
		}
		
		/***************************************************************************/
		/***************************************************************************/
		/***************************************************************************/
		acc_parallel_loop_info.acc_loopinfo[i] = acc_loopinfo;
		i++;
	}
}

/*Including parallel construct region*/
static void 
Transform_ACC_Parallel_Block ( WN * tree, ParallelRegionInfo* pPRInfo, WN* wn_replace_block)
{
	INT32 i;
	WN *wn;
	WN *wn2;
	WN *wn3;
	WN *wn4;
	WN *cur_node;
	WN *prev_node;
	WN *next_node;
	WN *sp_block;
	ST *lock_st;

	INT32 num_criticals;
	BOOL is_region;
	WN_PRAGMA_ID cur_id, end_id;
	INT32 gate_construct_num;
	int iKernelsCount = 0;
	//PARALLEL_LOOP_INFO acc_parallel_loop_info;
	acc_parallel_loop_info.acc_loopinfo.clear();
	acc_parallel_loop_info.wn_prehand_nodes = NULL;
	acc_parallel_loop_info.loopnum = 0;
		
	//acc_reduction_mem = ACC_RD_GLOBAL_MEM;
	//acc_reduction_rolling = ACC_RD_UNROLLING;
	//acc_reduction_exemode = ACC_RD_LAUNCH_KERNEL;
	//Scan and translate the loop region. Attaching the general nodes as well.
	for (cur_node = WN_first(tree); cur_node; cur_node = next_node) 
	{
		prev_node = WN_prev(cur_node);
		next_node = WN_next(cur_node);

		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		//Process the loop region which is inside the kernel region
		if ((is_region = (WN_opcode(cur_node) == OPC_REGION &&
                             WN_first(WN_region_pragmas(cur_node)) &&
                             WN_opcode(WN_first(
			        WN_region_pragmas(cur_node))) == OPC_PRAGMA) ) &&
	       WN_pragma(WN_first(WN_region_pragmas(cur_node))) ==
						WN_PRAGMA_ACC_LOOP_BEGIN) 
		{
			acc_t = ACCP_LOOP_REGION;
			//++num_constructs;
			//generate kernel function
			//This should be the first ACC LOOP
			acc_loopinfo.loopnum = 0;
			acc_loopinfo.acc_forloop.clear();
			kernel_tmp_variable_count = 0;
			ACC_Extract_ACC_LoopNest_Info(cur_node);
			//verify the loop depth, all the depth should be the same.
			if(acc_parallel_loop_info.loopnum 
				&& acc_loopinfo.loopnum != acc_parallel_loop_info.acc_loopinfo[0].loopnum)
			{
				Fail_FmtAssertion ("ACC Loop depth should be exactly the same in parallel region. ");
			}
				
			acc_parallel_loop_info.acc_loopinfo.push_back(acc_loopinfo);
			acc_parallel_loop_info.loopnum ++;
			acc_t = (ACCP_process_type)0;
			
    	} 
		else if(WN_opcode(cur_node) != OPC_REGION_EXIT)
		{
			if(acc_parallel_loop_info.loopnum == 0)
			{
				if(acc_parallel_loop_info.wn_prehand_nodes == NULL)
					acc_parallel_loop_info.wn_prehand_nodes = WN_CreateBlock();
				WN_INSERT_BlockLast(acc_parallel_loop_info.wn_prehand_nodes, cur_node);
			}
			else
			{
				if(acc_loopinfo.wn_following_nodes == NULL)
					acc_loopinfo.wn_following_nodes = WN_CreateBlock();
				WN_INSERT_BlockLast(acc_loopinfo.wn_following_nodes, cur_node);
			}			
		}
	}
	//this function should be called before ACC_Create_MicroTask.
	acc_reduction_tab_map.clear();
	acc_additionalKernelLaunchParamList.clear();
	acc_shared_memory_for_reduction_block.clear();
	acc_global_memory_for_reduction_host.clear();		//used in host side
	acc_global_memory_for_reduction_device.clear();	//used in host side
	acc_global_memory_for_reduction_param.clear();	//used in kernel parameters
	acc_global_memory_for_reduction_block.clear();
	///////////////////////////////////////////////////////////////////////////////////////////////
	ACC_ProcessReduction_Parallel(pPRInfo, wn_replace_block);
	///////////////////////////////////////////////////////////////////////////////////////////////
	//create local var table map
    kernel_tmp_variable_count = 0;
	
	for (wn = pPRInfo->acc_param; wn; wn = WN_next(wn))
	{
	  kernel_parameters_count ++;
	}
	//reserve 256 slots for kernel tmp variables.
	//switch symtab
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	//outline
	ACC_Push_Some_Globals( );
	//create outline function
	acc_st_shared_memory = NULL;
    ACC_Create_MicroTask( PAR_FUNC_ACC_KERNEL, (void*)pPRInfo, TRUE);

	//st_shared_array_4parallelRegion = NULL;
	WN* wn_parallelBlock = WN_CreateBlock();
	//Create private data list
	int iPivateCount = 0;
	if(acc_private_nodes)
	{
		WN* wn_node = acc_private_nodes;
	    char szlocalname[256];	  
		for(; wn_node; wn_node=WN_next(wn_node))
		{
			ST* st_private = WN_st(wn_node);
			
			map<ST*, ACC_ReductionMap>::iterator itor = acc_reduction_tab_map.find(st_private);			
			if(itor != acc_reduction_tab_map.end())
				continue;
			
			TY_IDX index_ty = ST_type(st_private);
		    ST* st_new_private = New_ST( CURRENT_SYMTAB );
	    	sprintf ( szlocalname, "%s", ST_name(st_private));
			ST_Init(st_new_private,
		      Save_Str( szlocalname),
		      CLASS_VAR,
		      SCLASS_AUTO,
		      EXPORT_LOCAL,
		      index_ty);//Put this variables in local table
			ACC_VAR_TABLE var;
			var.has_offset = FALSE;
			var.orig_st = st_private;
			var.new_st = st_new_private;
		    acc_local_new_var_map[st_private] = var;
		}		
	}

	//Begin transform the loop
	if(acc_parallel_loop_info.wn_prehand_nodes)
	{
		WN_INSERT_BlockLast(wn_parallelBlock, acc_parallel_loop_info.wn_prehand_nodes);
		acc_parallel_loop_info.wn_prehand_nodes = NULL;
	}
	//Transformation
	i = 0;
	while(i < acc_parallel_loop_info.loopnum)
	{
		acc_loopinfo = acc_parallel_loop_info.acc_loopinfo[i];
		ACC_Transform_SingleForLoop(pPRInfo, wn_parallelBlock);
		if(acc_loopinfo.wn_following_nodes)
			WN_INSERT_BlockLast(wn_parallelBlock, acc_loopinfo.wn_following_nodes);
		i++;
	}

	///////////////////////////////////////////////////////////////////////////////
	ACC_Localize_Parent_Stack lps(FALSE, NULL);
	ACC_Walk_and_Localize(wn_parallelBlock);
	
	WN_INSERT_BlockLast(acc_parallel_func, wn_parallelBlock);
	
	//launch kernels
	//LaunchKernel(0, wn_replace_block, TRUE);
		
	ACC_Transfer_Maps ( acc_pmaptab, acc_cmaptab, acc_parallel_func, 
		  PU_Info_regions_ptr(Current_PU_Info) );

	/* Create a new dependence graph for the child  and move all the 
	 appropriate vertices from the parent to the child graph */

	Current_Map_Tab = acc_cmaptab;
	ACC_Fix_Dependence_Graph ( acc_ppuinfo, Current_PU_Info, acc_parallel_func ); 
	Current_Map_Tab = acc_pmaptab;

  
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );
	
	//setup the gangs and vectors
	//ACC_Setup_GPU_toplogy_parallel(pPRInfo, wn_replace_block);
	//launch kernels
	LaunchKernel(0, wn_replace_block, TRUE);
	//launch reduction
	
	
	i=0;
	while(i < acc_parallel_loop_info.loopnum)
	{
		acc_loopinfo = acc_parallel_loop_info.acc_loopinfo[i];
		/***************************************************************************/
		/*************************Check the reduction here*****************************/
		/***************************************************************************/
		int iRdIdx;
		//Only need to take care the top level reduction. Reduction operation in a single block will be handled
		//in ACC_Transform_SingleForLoop. 
		iRdIdx = 0;
		while(iRdIdx<acc_loopinfo.acc_forloop[0].reductionmap.size())
		{
			ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[0].reductionmap[iRdIdx];
			ST *st_old = reductionmap.hostName;
			ST *st_device = reductionmap.deviceName;
			ST *st_reductionKernel = reductionmap.reduction_kenels;			
			WN* wn_reduction_final_stmt = GenFinalReductionAlgorithm(st_device, st_old,
				st_reductionKernel, reductionmap.st_num_of_element, TY_size(ST_type(st_old)));
			WN_INSERT_BlockLast(wn_replace_block, wn_reduction_final_stmt);
			iRdIdx ++;
		}
		/***************************************************************************/
		/***************************************************************************/
		/***************************************************************************/
		i++;
	}
}


/*Including kernel construct region*/
static void 
Transform_ACC_Kernel_Block ( WN * tree, KernelsRegionInfo* pKRInfo, WN* wn_replace_block)
{
	INT32 i;
	WN *wn;
	WN *wn2;
	WN *wn3;
	WN *wn4;
	WN *cur_node;
	WN *prev_node;
	WN *next_node;
	WN *sp_block;
	ST *lock_st;

	INT32 num_criticals;
	BOOL is_omp, is_region;
	WN_PRAGMA_ID cur_id, end_id;
	INT32 gate_construct_num;
	int iKernelsCount = 0;
	
	for (wn = pKRInfo->acc_param; wn; wn = WN_next(wn))
	{
	  kernel_parameters_count ++;
	}
	//reserve 256 slots for kernel tmp variables.
	//this function should be called before ACC_Create_MicroTask.
	acc_reduction_tab_map.clear();

	for (cur_node = WN_first(tree); cur_node; cur_node = next_node) 
	{

		prev_node = WN_prev(cur_node);
		next_node = WN_next(cur_node);

		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////
		//Process the loop region which is inside the kernel region
		if ((is_region = (WN_opcode(cur_node) == OPC_REGION &&
                             WN_first(WN_region_pragmas(cur_node)) &&
                             WN_opcode(WN_first(
			        WN_region_pragmas(cur_node))) == OPC_PRAGMA) ) &&
	       WN_pragma(WN_first(WN_region_pragmas(cur_node))) ==
						WN_PRAGMA_ACC_LOOP_BEGIN) 
		{

			ACCP_process_type save_acct = acc_t;
			acc_t = ACCP_LOOP_REGION;

			//++num_constructs;
			//generate kernel function
			//This should be the first ACC LOOP
			acc_loopinfo.loopnum = 0;
			acc_loopinfo.acc_forloop.clear();
			iKernelsCount ++;
			ACC_Extract_ACC_LoopNest_Info(cur_node);
			//Setup gangs and vectors before kernel launch
			ACC_Setup_GPU_toplogy_kernels(wn_replace_block);
			//Reduction process here, if there is a reduction algorithm
			//Only the reduction across the whole gang and thread is supported at this time.
			//for(WN* wn_reduction = acc_loopinfo.acc_forloop[0].acc_reduction; wn_reduction; wn_reduction=WN_next(wn_reduction))
			//{
			//	ACC_Process_SingleReductionNode(wn_reduction, 
			//							wn_replace_block);
			//}
			
			////////////////////////////////////////////////
			ACC_Transform_MultiForLoop(pKRInfo);
			//launch kernels
			LaunchKernel(iKernelsCount-1, wn_replace_block, FALSE);
			//If there is some kind of reduction
			i = 0;
			/*while(i<acc_loopinfo.acc_forloop[0].reductionmap.size())
			{
				ACC_ReductionMap reductionmap = acc_loopinfo.acc_forloop[0].reductionmap[i];
				WN* wn_lauch_reduction = GenFinalReductionAlgorithm(reductionmap.deviceName, reductionmap.hostName,
												reductionmap.reduction_kenels, GetKernelParamType(reductionmap.hostName));
				WN_INSERT_BlockLast(wn_replace_block, wn_lauch_reduction);
				i++;
			}*/
			//OK. Let's begin analysis
			
			//ACC_Process_ForLoop_InKernels( cur_node );
			
			/*if (WN_first(wn)) {
			   if (prev_node)
			      WN_next(prev_node) = WN_first(wn);
			   else
			      WN_first(tree) = WN_first(wn);
			   WN_prev(WN_first(wn)) = prev_node;
			   WN_next(WN_last(wn)) = next_node;
			   if (next_node)
			      WN_prev(next_node) = WN_last(wn);
			   else
			      WN_last(tree) = WN_last(wn);
			   next_node = WN_first(wn);
			} else {
			   if (prev_node)
			      WN_next(prev_node) = next_node;
			   else
			      WN_first(tree) = next_node;
			   if (next_node)
			      WN_prev(next_node) = prev_node;
			   else
			      WN_last(tree) = prev_node;
			}*/

			WN_Delete ( WN_region_pragmas(cur_node) );
			WN_DELETE_Tree ( WN_region_exits(cur_node) );
			RID_Delete ( Current_Map_Tab, cur_node );
			WN_Delete ( cur_node );
			//WN_Delete ( wn );

			acc_t = save_acct;
    	} 
	}
}


/*  Walk the reduction, last_local, local, and firstprivate lists and add the
    contents to the VAR_TABLE table.  If firstprivate_blockp is non-NULL,
    then (*firstprivate_blockp) must be NULL, and the code to initialize
    values of FIRSTPRIVATE variables is returned in (*firstprivate_blockp);
    if the value of (*firstprivate_blockp) is NULL upon return, no such
    code was generated.  Accumulate code to allocate dynamic arrays in
    (*alloca_blockp).  */

/*static void 
ACC_Create_Local_VariableTab ( ACC_VAR_TABLE * vtab, ACC_VAR_TYPE vtype, ST* old_st,
						WN_OFFSET old_offset, ST* new_st)
{	
	WN		  *l;
	OPERATOR   opr;
	ACC_VAR_TABLE *v = vtab;	
	ST   *sym;
	TY_IDX ty;
		 
	v->vtype	      = vtype;
	v->orig_st	      = old_st;
	v->orig_offset      = old_offset;
	if (ST_class(old_st) != CLASS_PREG) 
	{
		ty = ST_type(old_st);
    	TY_KIND kind = TY_kind(ty);
	
		if (TY_kind(ty) == KIND_ARRAY) 
		{
	      v->mtype = TY_mtype(TY_etype(ty));
	      v->has_offset = FALSE;
	    } 
		else if (TY_kind(ty) == KIND_STRUCT)
	    {
	      v->mtype = TY_mtype(ty);
	      v->has_offset = FALSE;
	    } 
		else if (v->is_dynamic_array) 
	    {
	      v->mtype = TY_mtype(TY_AR_etype(TY_pointed(ty)));
	      v->has_offset = FALSE;
	    } 
		else 
	    {
	      v->mtype = TY_mtype(ty);
	      v->has_offset = FALSE;
	    }
	    v->ty         = ty;
	    v->new_st     = new_st;
	    v->new_offset = 0;
	}
	 
}*/

/****************************************************/
/*this following  two functions will be used in ACC_Walk_and_Localize*/
inline void ACC_WN_set_offsetx ( WN *wn, WN_OFFSET ofst )
{
  OPERATOR opr;
  opr = WN_operator(wn);
  if ((opr == OPR_PRAGMA) || (opr == OPR_XPRAGMA)) {
    WN_pragma_arg1(wn) = ofst;
  } else {
    WN_offset(wn) = ofst;
  }
}



static inline TYPE_ID Promote_Type(TYPE_ID mtype)
{
  switch (mtype) {
    case MTYPE_I1 : case MTYPE_I2: return(MTYPE_I4);
    case MTYPE_U1 : case MTYPE_U2: return(MTYPE_U4);
    default: return mtype;
  }
}


static void ACC_Dump_InOutAnalysis (ACC_VAR_Liveness * pHead)
{
	char* strVarType[9] = {\
					"UDEF",\
					"DEF",\
					"USE",\
					"USEDEF",\
					"DEFUSE",\
					"PRIVATE",\
					"IN",\
					"OUT",\
					"INOUT"};
	ACC_VAR_Liveness* pNext = pHead;
	while(pNext)
	{
		char szInfo[256];
		
		sprintf(szInfo, "variable:%s, status:%s", ST_name(pNext->var_st), strVarType[pNext->DU_Live_type]);
		printf("%s\n", szInfo);
		pNext = pNext->pNext;
	}
}


/**********************************************************************************/
//Generate GPU kernels parameters from acc kernels/parallel block
//isKernelRegion, if it is acc kernels region, should be set as TRUE.
// 				if it is the acc parallel region, should be set as FALSE.
/**********************************************************************************/

static BOOL ACC_Check_Pointer_Exist_inClause(ST* st_param, void* pRegionInfo, BOOL isKernelRegion)
{
	ParallelRegionInfo* pParallelRegionInfo;
	KernelsRegionInfo* pKernelsRegionInfo;
	WN* wn;
	if(isKernelRegion)
	{
		pKernelsRegionInfo = (KernelsRegionInfo*)pRegionInfo;
		//Get a pair list for kernel parameters.
		//for(wn=paramlist; wn; wn=WN_next(wn))
		{
			//ST* st_param = WN_st(wn);
			int i = 0;
			BOOL isFound = FALSE;
			int j = 0;
				
			TY_IDX ty = ST_type(st_param);
			TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			//if it is scalar var, skip it.
			//let's traverse all the tables
			while(j < acc_nested_dregion_info.Depth)
			{
				//acc_nested_dregion_info.DRegionInfo[j];
				//
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyinMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}				
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcreateMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].presentMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].dptrList.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].dptrList[i] == st_param)
					{
						return TRUE;
					}
					i ++;
				}				
				j++;
			}
			
			//Kernels Table
			while(i < pKernelsRegionInfo->pcopyMap.size())
			{
				if(pKernelsRegionInfo->pcopyMap[i]->hostName == st_param)
				{
					return TRUE;
				}
				i ++;
			}
			i = 0;
			while(i < pKernelsRegionInfo->pcopyinMap.size())
			{
				if(pKernelsRegionInfo->pcopyinMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			
			i = 0;
			while(i < pKernelsRegionInfo->pcopyoutMap.size())
			{
				if(pKernelsRegionInfo->pcopyoutMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			
			i = 0;
			while(i < pKernelsRegionInfo->pcreateMap.size())
			{
				if(pKernelsRegionInfo->pcreateMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			i = 0;
			while(i < pKernelsRegionInfo->presentMap.size())
			{
				if(pKernelsRegionInfo->presentMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}			
			
			i = 0;
			while(i < pKernelsRegionInfo->dptrList.size())
			{
				if(pKernelsRegionInfo->dptrList[i] == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			return FALSE;
		}
	}
	else
	{
		pParallelRegionInfo = (ParallelRegionInfo*)pRegionInfo;
		//Get a pair list for kernel parameters.
		//for(wn=paramlist; wn; wn=WN_next(wn))
		{
			//ST* st_param = WN_st(wn);
			int i = 0;
			int j = 0;
				
			TY_IDX ty = ST_type(st_param);
			TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			
			//let's traverse all the tables
			while(j < acc_nested_dregion_info.Depth)
			{
				//acc_nested_dregion_info.DRegionInfo[j];
				//
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}

				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyinMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				

				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				

				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcreateMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				

				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].presentMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->hostName == st_param)
					{
						return TRUE;
					}
					i ++;
				}
				

				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].dptrList.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].dptrList[i] == st_param)
					{
						return TRUE;
					}
					i ++;
				}
			
				
				j++;
			}


			//Kernels Table
			while(i < pParallelRegionInfo->pcopyMap.size())
			{
				if(pParallelRegionInfo->pcopyMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			i = 0;
			while(i < pParallelRegionInfo->pcopyinMap.size())
			{
				if(pParallelRegionInfo->pcopyinMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			i = 0;
			while(i < pParallelRegionInfo->pcopyoutMap.size())
			{
				if(pParallelRegionInfo->pcopyoutMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			i = 0;
			while(i < pParallelRegionInfo->pcreateMap.size())
			{
				if(pParallelRegionInfo->pcreateMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			
			i = 0;
			while(i < pParallelRegionInfo->presentMap.size())
			{
				if(pParallelRegionInfo->presentMap[i]->hostName == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			
			i = 0;
			while(i < pParallelRegionInfo->dptrList.size())
			{
				if(pParallelRegionInfo->dptrList[i] == st_param)
				{
						return TRUE;
				}
				i ++;
			}
			return FALSE;
		}
	}
}

static void ACC_Free_DefUse_Chain(ACC_VAR_Liveness ** pHead)
{
	ACC_VAR_Liveness* pNext;
	while(*pHead)
	{
		pNext = (*pHead)->pNext;
		delete (*pHead);
		(*pHead) = pNext;
	}
}


static void ACC_Finalize_Liveness_Chain(ACC_VAR_Liveness * pHead)
{
	while(pHead)
	{
		if(pHead->DU_Live_type == ACC_DU_DEF)
			pHead->DU_Live_type = ACC_LIVE_PRIVATE;
		
		else if(pHead->DU_Live_type == ACC_DU_USE)
			pHead->DU_Live_type = ACC_LIVE_IN;
		
		else if(pHead->DU_Live_type == ACC_DU_USEDEF)
			pHead->DU_Live_type = ACC_LIVE_IN;
		
		else if(pHead->DU_Live_type == ACC_DU_DEFUSE)
			pHead->DU_Live_type = ACC_LIVE_PRIVATE;
		//move to next
		pHead = pHead->pNext;
	}
}

static void ACC_Retrieve_Kernel_Param_From_Liveness(ACC_VAR_Liveness * pHead)
{
	while(pHead)
	{
		WN* wn;
		ST* stVar = pHead->var_st;		
		if(pHead->DU_Live_type == ACC_LIVE_PRIVATE)
		{
			//pHead->var_st;	
        	WN* newNode = WN_CreatePragma(WN_PRAGMA_ACC_CLAUSE_PRIVATE, stVar, 0, 0);
		   
		   for (wn = acc_private_nodes; wn; wn = WN_next(wn))
			 if (WN_st(wn) == WN_st(newNode))
		   		break;
		   if (wn == NULL) {
			 WN_next(newNode) = acc_private_nodes;
			 acc_private_nodes = newNode;
			 //++acc_private_count;
		   } else
			 WN_Delete ( newNode );
		   
		   
		}
		
		else if(pHead->DU_Live_type == ACC_LIVE_IN || ACC_LIVE_INOUT || ACC_LIVE_OUT)
		{		
           WN* newNode = WN_CreatePragma(WN_PRAGMA_ACC_CLAUSE_PARM, stVar, 0, 0);
			
		   for (wn = acc_parms_nodes; wn; wn = WN_next(wn))
			 if (WN_st(wn) == WN_st(newNode))
		   		break;
		   if (wn == NULL) 
		   {
			 WN_next(newNode) = acc_parms_nodes;
			 acc_parms_nodes = newNode;
			 //++acc_parms_count;
		   } 
		   else
			 WN_Delete (newNode);		   
		}
		
		//move to next
		pHead = pHead->pNext;
	}
}

/*To get the in/out liveness analysis for the ACC code region, 
Scan the code folllowing right behind the acc region
		def 			use
udef:	 /			 /
def:		private		out
use:		in			in
usedef:	in			inout
defuse:	private		out
*/
static void
ACC_Region_LivenessAnalysis (WN * tree, ACC_VAR_Liveness* pHead)
{
	
	OPCODE op;
	OPERATOR opr;
	INT32 i;
	WN *r;
	WN *temp;
	ST *old_sym;
	WN_OFFSET old_offset;
	ACC_VAR_Liveness *w;
	
	/* Ignore NULL subtrees. */
	
	if (tree == NULL)
	  return;
	
	/* Initialization. */
	
	op = WN_opcode(tree);
	opr = OPCODE_operator(op);
	
	/* Look for and replace any nodes referencing localized symbols */
	//use
	if (opr == OPR_LDID) 
	{
	  old_sym = WN_st(tree);
	  
	  BOOL isPreg = strncmp(".preg_", ST_name(old_sym), 6);
	  if(!isPreg)
		 return;
	  //old_offset = WN_offsetx(tree);
	  for (w=pHead; w; w=w->pNext) 
	  {
		if ((w->var_st == old_sym) && (w->DU_Live_type < ACC_LIVE_PRIVATE)) 
		{ 
			if((w->DU_Live_type < ACC_LIVE_PRIVATE))
			{
				if(w->DU_Live_type == ACC_DU_DEF)
				{
				  w->DU_Live_type = ACC_LIVE_OUT;
				}
				else if(w->DU_Live_type == ACC_DU_USE)
				{
				  w->DU_Live_type = ACC_LIVE_IN;
				}
				else if(w->DU_Live_type == ACC_DU_USEDEF)
				{
				  w->DU_Live_type = ACC_LIVE_INOUT;
				}
				else if(w->DU_Live_type == ACC_DU_DEFUSE)
				{
				  w->DU_Live_type = ACC_LIVE_OUT;
				}
			}
			break;
		}
	  }	
	}
	//def
	else if (opr == OPR_STID)
	{
	  old_sym = WN_st(tree);
	  //old_offset = WN_offsetx(tree);
	  
	  BOOL isPreg = strncmp(".preg_", ST_name(old_sym), 6);
	  if(!isPreg)
		 return;
	  for (w=pHead; w; w=w->pNext) 
	  {
		if ((w->var_st == old_sym)) 
		{ 
			if((w->DU_Live_type < ACC_LIVE_PRIVATE))
			{
				if(w->DU_Live_type == ACC_DU_DEF)
				{
				  w->DU_Live_type = ACC_LIVE_PRIVATE;
				}
				else if(w->DU_Live_type == ACC_DU_USE)
				{
				  w->DU_Live_type = ACC_LIVE_IN;
				}
				else if(w->DU_Live_type == ACC_DU_USEDEF)
				{
				  w->DU_Live_type = ACC_LIVE_IN;
				}
				else if(w->DU_Live_type == ACC_DU_DEFUSE)
				{
				  w->DU_Live_type = ACC_LIVE_PRIVATE;
				}
			}
			break;
		}
	  }
	}	
	
	
	/* Walk all children */
	
	//lps->Push(tree);
	if (op == OPC_BLOCK) 
	{
	  r = WN_first(tree);
	  while (r) 
	  { // localize each node in block
		ACC_Region_LivenessAnalysis ( r, pHead);
		if (WN_prev(r) == NULL)
		  WN_first(tree) = r;
		if (WN_next(r) == NULL)
		  WN_last(tree) = r;
		
		r = WN_next(r); 	 
	 }
	}
	else if(op == OPC_REGION)
	{
	  if ((WN_opcode(tree) == OPC_REGION) &&
			   WN_first(WN_region_pragmas(tree)) &&
			   ((WN_opcode(WN_first(WN_region_pragmas(tree))) == OPC_PRAGMA) ||
				(WN_opcode(WN_first(WN_region_pragmas(tree))) == OPC_XPRAGMA)))
	  {
		  WN *wtmp = WN_first(WN_region_pragmas(tree));
		  WN_PRAGMA_ID wid = (WN_PRAGMA_ID) WN_pragma(wtmp);	  
		  WN* wn_next_node = WN_next(wtmp); //this is the pragma things, ignored here
		  WN* wn_stmt_block = WN_region_body(tree);//region body
		  //scan the region body
		  ACC_Region_LivenessAnalysis ( wn_stmt_block, pHead);
		  /////////////////////////////////////////////////////
		  WN* wn_cont_nodes = WN_next(tree); //the whirl node folllowing this region
		  while(wn_cont_nodes)
		  {
			  ACC_Region_LivenessAnalysis ( wn_cont_nodes, pHead);
			  wn_cont_nodes = WN_next(wn_cont_nodes);
		  }
		  
	  }
	  else
	  {
		  Fail_FmtAssertion ("Wrong Region format cause the defuse analysis failed in ACC lower.");
	  }
	}
	else 
	{
	  for (i=0; i < WN_kid_count(tree); i++)
	  {
		ACC_Region_LivenessAnalysis ( WN_kid(tree, i), pHead);
	  }
	}
	//lps->Pop();


  return;
}   


static void
ACC_Region_LivenessAnalysis_cont (WN * tree, ACC_VAR_Liveness* pHead)
{
	WN* cont = tree;
	while(cont)
	{
		ACC_Region_LivenessAnalysis(cont, pHead);
		cont = WN_next(cont);
	}
}

/*Scan the block/region and generate the variable status: 
		def(store) 			use(load)
udef:	def			use
def:		def			defuse
use:		usedef		use
usedef:	usedef		usedef
defuse:	defuse		defuse
*/
static void
ACC_Region_DefUseAnalysis (WN * tree, ACC_VAR_Liveness ** pHead, void* pRegionInfo, BOOL isKernelRegion)
{
  OPCODE op;
  OPERATOR opr;
  INT32 i;
  WN *r;
  WN *temp;
  ST *old_sym;
  WN_OFFSET old_offset;
  ACC_VAR_Liveness *w;

  /* Ignore NULL subtrees. */

  if (tree == NULL)
    return;

  /* Initialization. */

  op = WN_opcode(tree);
  opr = OPCODE_operator(op);

  /* Look for and replace any nodes referencing localized symbols */
  //use
  if (opr == OPR_LDID) 
  {
  	BOOL isPreg = FALSE;
    old_sym = WN_st(tree);
	isPreg = strncmp(".preg_", ST_name(old_sym), 6);
	if(!isPreg)
		return;
    //old_offset = WN_offsetx(tree);
    for (w=*pHead; w; w=w->pNext) 
	{
      if ((w->var_st == old_sym)) 
	  {	
	  	  if(w->DU_Live_type == ACC_DU_DEF)
	  	  {
	  	  	w->DU_Live_type = ACC_DU_DEFUSE;
	  	  }
		  break;
      }
    }

	if(w == NULL)
	{
		//a new node which will be attached in the linklist becomes a new header.
		ACC_VAR_Liveness* pNewHead = new ACC_VAR_Liveness;
		pNewHead->DU_Live_type = ACC_DU_USE;
		pNewHead->var_st = old_sym;
		pNewHead->pNext = *pHead;
		pNewHead->pPrev = NULL;
		if(*pHead)
			(*pHead)->pPrev = pNewHead;
		*pHead = pNewHead;		
	}
  }
  //def
  else if (opr == OPR_STID)
  {
    old_sym = WN_st(tree);
    old_offset = WN_offsetx(tree);
	BOOL isPreg = strncmp(".preg_", ST_name(old_sym), 6);
    if(!isPreg)
		return;
    for (w=*pHead; w; w=w->pNext) 
	{
      if ((w->var_st == old_sym)) 
	  {	
	  	  if(w->DU_Live_type == ACC_DU_USE)
	  	  {
	  	  	w->DU_Live_type = ACC_DU_USEDEF;
	  	  }
		  break;
      }
    }

	if(w == NULL)
	{
		//a new node which will be attached in the linklist becomes a new header.
		ACC_VAR_Liveness* pNewHead = new ACC_VAR_Liveness;
		pNewHead->DU_Live_type = ACC_DU_DEF;
		pNewHead->var_st = old_sym;
		pNewHead->pNext = *pHead;
		pNewHead->pPrev = NULL;		
		if(*pHead)
			(*pHead)->pPrev = pNewHead;
		*pHead = pNewHead;		
	}
  }   
  else if (opr == OPR_LDA)
  {
  	//usually LDA operation cannot be IN/OUT/INOUT
  	//in Kernels region.
    old_sym = WN_st(tree);
	BOOL bExistInDClause = ACC_Check_Pointer_Exist_inClause(old_sym, pRegionInfo, isKernelRegion);
    old_offset = WN_offsetx(tree);
    for (w=*pHead; w; w=w->pNext) 
	{
      if ((w->var_st == old_sym)) 
	  {	
		  break;
      }
    }
	if(w == NULL)
	{
		//a new node which will be attached in the linklist becomes a new header.
		ACC_VAR_Liveness* pNewHead = new ACC_VAR_Liveness;
		if(bExistInDClause)
			pNewHead->DU_Live_type = ACC_DU_USE;
		else 
			pNewHead->DU_Live_type = ACC_DU_DEF;
		pNewHead->var_st = old_sym;
		pNewHead->pNext = *pHead;
		pNewHead->pPrev = NULL;		
		if(*pHead)
			(*pHead)->pPrev = pNewHead;
		*pHead = pNewHead;		
	}
  	
  }
  

  /* Walk all children */

  //lps->Push(tree);
  if (op == OPC_BLOCK) 
  {
    r = WN_first(tree);
    while (r) 
	{ // localize each node in block
      ACC_Region_DefUseAnalysis ( r, pHead, pRegionInfo, isKernelRegion);
      if (WN_prev(r) == NULL)
        WN_first(tree) = r;
      if (WN_next(r) == NULL)
        WN_last(tree) = r;
	  
      r = WN_next(r);      
   }
  }
  else if(op == OPC_REGION)
  {
	if ((WN_opcode(tree) == OPC_REGION) &&
			 WN_first(WN_region_pragmas(tree)) &&
			 ((WN_opcode(WN_first(WN_region_pragmas(tree))) == OPC_PRAGMA) ||
			  (WN_opcode(WN_first(WN_region_pragmas(tree))) == OPC_XPRAGMA)))
	{
	    WN *wtmp = WN_first(WN_region_pragmas(tree));
	    WN_PRAGMA_ID wid = (WN_PRAGMA_ID) WN_pragma(wtmp);		
	    WN* wn_next_node = WN_next(wtmp); //this is the pragma things, ignored here
	    WN* wn_stmt_block = WN_region_body(tree);//region body
	    //scan the region body
		ACC_Region_DefUseAnalysis ( wn_stmt_block, pHead, pRegionInfo, isKernelRegion);
		/////////////////////////////////////////////////////
	    WN* wn_cont_nodes = WN_next(tree); //the whirl node folllowing this region
	    while(wn_cont_nodes)
    	{
    		ACC_Region_DefUseAnalysis ( wn_cont_nodes, pHead, pRegionInfo, isKernelRegion);
			wn_cont_nodes = WN_next(wn_cont_nodes);
    	}
	    
	}
	else
	{
        Fail_FmtAssertion ("Wrong Region format cause the defuse analysis failed in ACC lower.");
	}
  }
  else 
  {
    for (i=0; i < WN_kid_count(tree); i++)
    {
      ACC_Region_DefUseAnalysis ( WN_kid(tree, i), pHead, pRegionInfo, isKernelRegion);
    }
  }
  //lps->Pop();

  return;
}   

static WN* ACC_Make_Array_ref(ST *base, WN* wn_offset, WN* wn_dim)
{
    WN *arr_ref = WN_Create( OPCODE_make_op(OPR_ARRAY,Pointer_Mtype,MTYPE_V),3);
	if(TY_kind(ST_type(base)) == KIND_POINTER)
    	WN_element_size(arr_ref) = TY_size(TY_pointed(ST_type(base)));
	else if(TY_kind(ST_type(base)) == KIND_ARRAY)
    	WN_element_size(arr_ref) = TY_size(TY_etype(ST_type(base)));
    WN_array_base(arr_ref) = WN_Lda(Pointer_type, 0, base);
    WN_array_index(arr_ref,0) = wn_offset;
    WN_array_dim(arr_ref,0) = wn_dim;
    return arr_ref;
} /* make_array_ref */


/*
Walk the tree, replacing global references with local ones.  Within
parallel regions, also translate label numbers from those of the parent PU
to those of the child, and generate new INITO/INITV structures (for e.g.
C++ exception handling blocks) for the child PU.

Argument is_par_region must be TRUE iff tree is an MP construct that's a
parallel region.

In a non-recursive call to this routine, output argument
non_pod_finalization must point to a NULL WN *. Upon return,
(*non_pod_finalization) points to the non-POD finalization IF node (if one
was found in the tree), and this IF node is removed from the tree; the IF
node cannot have been the "tree" argument in the non-recursive call.

Note that within orphaned worksharing constructs, non-POD variables have
been localized already by the frontend, so we don't rewrite references to
such variables that appear in vtab.

In a non-recursive call to this routine, it is guaranteed that if the root
node of tree is not a load or store (e.g. it's a DO_LOOP or block), that
root node will not be replaced.
*/
static WN *
ACC_Walk_and_Localize (WN * tree)
{
  OPCODE op;
  OPERATOR opr;
  INT32 i;
  WN *r;
  WN *temp;
  ST *old_sym;
  WN_OFFSET old_offset;

  /* Ignore NULL subtrees. */

  if (tree == NULL)
    return (tree);

  /* Initialization. */

  op = WN_opcode(tree);
  opr = OPCODE_operator(op);

  /* Look for and replace any nodes referencing localized symbols */

  if (opr == OPR_LDID) 
  {
    old_sym = WN_st(tree);
    old_offset = WN_offsetx(tree);
	map<ST*, ACC_VAR_TABLE>::iterator itor = acc_local_new_var_map.find(old_sym);
	if(itor != acc_local_new_var_map.end())	
    //for (w=vtab; w->orig_st; w++) 
	{
	  ACC_VAR_TABLE newVar = itor->second;
      if ((newVar.orig_st == old_sym) &&
	  (newVar.has_offset ? (newVar.orig_offset == old_offset) : TRUE )) 
	  {	
  		  WN_st_idx(tree) = ST_st_idx(newVar.new_st);
		  // for reduction of a field of STRUCT, the TY_kind would be different
		  // And, we need to fix the TY for the wn, the field_id, and offsetx
		  // As the local_xxx symbol is always .predef..., so field_id should be 0
		  if (TY_kind(ST_type(newVar.new_st)) != TY_kind(WN_ty(tree)))
		  {
	        WN_set_ty(tree, ST_type(newVar.new_st));
	    	WN_set_field_id(tree, 0);
		  }
		  if (newVar.has_offset)
		    ACC_WN_set_offsetx(tree, newVar.new_offset);
      }
    }
  }
  else if (opr == OPR_STID)
  {
    old_sym = WN_st(tree);
    old_offset = WN_offsetx(tree);
	map<ST*, ACC_VAR_TABLE>::iterator itor = acc_local_new_var_map.find(old_sym);
	if(itor != acc_local_new_var_map.end())	
    //for (w=vtab; w->orig_st; w++) 
	{
	  ACC_VAR_TABLE newVar = itor->second;
      if ((newVar.orig_st == old_sym) &&
	  		(newVar.has_offset ? (newVar.orig_offset == old_offset) : TRUE )) 
	  {
		WN_st_idx(tree) = ST_st_idx(newVar.new_st);
		if (TY_kind(ST_type(newVar.new_st)) != TY_kind(WN_ty(tree)))
		{
				WN_set_ty(tree, ST_type(newVar.new_st));
				WN_set_field_id(tree, 0);
		}
		if (newVar.has_offset)
				ACC_WN_set_offsetx(tree, newVar.new_offset);	
      }
    }
  } 
  /*else if (opr == OPR_ILOAD) 
  {
    for (w=vtab; w->orig_st; w++) 
	{
      if ((newVar.vtree && (WN_ACC_Compare_Trees(newVar.vtree, WN_kid0(tree)) == 0)) ||
	  (newVar.vtreex && (WN_ACC_Compare_Trees(newVar.vtreex, WN_kid0(tree)) == 0))) 
	  {
		temp = WN_RLdid ( Promote_Type(newVar.mtype), newVar.mtype, newVar.new_offset,
			  newVar.new_st, newVar.ty );
#ifdef KEY // bug 10707: honor the type in the original tree node
		if (WN_rtype(tree) == MTYPE_F8 && WN_rtype(temp) == MTYPE_C8 ||
		    WN_rtype(tree) == MTYPE_F4 && WN_rtype(temp) == MTYPE_C4) 
		{
		  WN_set_rtype(temp, WN_rtype(tree));
		  WN_set_desc(temp, WN_desc(tree));
		}
#endif
		WN_DELETE_Tree ( tree );
		tree = temp;
		op = WN_opcode(tree);
		opr = OPCODE_operator(op);
		break;
      }
    }
#ifdef KEY
    if (opr == OPR_ILOAD)  // There was no match above
      WN_kid0(tree) = ACC_Walk_and_Localize ( WN_kid0(tree));
#endif // KEY
  }*/
  /*else if (opr == OPR_ISTORE) 
  {
    for (w=vtab; w->orig_st; w++) 
	{
      if ((newVar.vtree && (WN_ACC_Compare_Trees(newVar.vtree, WN_kid1(tree)) == 0)) ||
	  (newVar.vtreex && (WN_ACC_Compare_Trees(newVar.vtreex, WN_kid1(tree)) == 0))) 
	  {
		temp = WN_Stid ( newVar.mtype, newVar.new_offset, newVar.new_st, newVar.ty,
			 WN_kid0(tree) );
#ifdef KEY // bug 10707: honor the type in the original tree node
		if (WN_desc(tree) == MTYPE_F8 && WN_desc(temp) == MTYPE_C8 ||
		    WN_desc(tree) == MTYPE_F4 && WN_desc(temp) == MTYPE_C4) 
		{
		  WN_set_desc(temp, WN_desc(tree));
		}
#endif
		WN_linenum(temp) = WN_linenum(tree);
		WN_prev(temp) = WN_prev(tree);
		if (WN_prev(temp))
		  WN_next(WN_prev(temp)) = temp;
		WN_next(temp) = WN_next(tree);
		if (WN_next(temp))
		  WN_prev(WN_next(temp)) = temp;
		WN_DELETE_Tree ( WN_kid1(tree) );
		WN_Delete ( tree );
		tree = temp;
		op = WN_opcode(tree);
		opr = OPCODE_operator(op);
		break;
      }
    }
    // PV 600983: don't translate labels (or exception regions) inside
    // orphaned constructs
  }*/
  else if (opr == OPR_ARRAY && WN_num_dim(tree)>1) 
  {
  	int idim = WN_num_dim(tree);
	int ii = 0;
	WN* wn_offset = NULL;
	WN* wn_base = WN_array_base(tree) ;
	WN* wn_dimInOne = NULL;;
    old_sym = WN_st(wn_base);
	ST* new_sym = NULL;
	UINT32 esize = WN_element_size(tree);
	TY_IDX ty = ACC_Get_ElementTYForMultiArray(WN_st(wn_base));
	
	map<ST*, ACC_VAR_TABLE>::iterator itor = acc_local_new_var_map.find(old_sym);
	
	if(itor != acc_local_new_var_map.end())	
    //for (w=vtab; w->orig_st; w++) 
	{
	  	ACC_VAR_TABLE newVar = itor->second;
		//WN_num_dim
		//WN_array_dim(x,i)
		//WN_array_index(x, i)
		if(new_sym)
		{
			for(ii=0; ii<idim-1; ii++)
			{
				WN* wn_index = WN_array_index(tree, ii);
				//TY_IDX idx_ty = WN_rtype(wn_index);
				//if(WN_has_sym(wn_index))
				//	idx_ty = TY_mtype(ST_type(WN_st(wn_index)));
				//else
				//	Is_True(WN_has_sym(wn_index), ("WN_st: wn doesn't have ST field"));
				wn_index = WN_COPY_Tree(wn_index);
				int iii = ii;
				while(iii<idim-1)
				{
					WN* wn_dim = WN_array_dim(tree, iii+1);
			   		wn_index = WN_Binary(OPR_MPY, 
			   						WN_rtype(wn_index), 
			   						wn_index, 
			   						WN_COPY_Tree(wn_dim));
					iii ++;
				}
				if(wn_offset)
		   			wn_offset = WN_Binary(OPR_ADD, 
			   					WN_rtype(wn_index), 
		   						wn_offset, 
		   						wn_index);
				else
					wn_offset = wn_index;
				if(wn_dimInOne == NULL)
					wn_dimInOne = WN_Binary(OPR_MPY, 
			   					WN_rtype(wn_index), 
			   						WN_COPY_Tree(WN_array_dim(tree, ii)), 
			   						WN_COPY_Tree(WN_array_dim(tree, ii+1)));
				else
					wn_dimInOne = WN_Binary(OPR_MPY, 
			   					WN_rtype(wn_index), 
			   						wn_dimInOne, 
			   						WN_COPY_Tree(WN_array_dim(tree, ii+1)));
			}
			WN* wn_index = WN_array_index(tree, ii);
			if(wn_offset)
				wn_offset = WN_Binary(OPR_ADD, 
			   				WN_rtype(wn_index), 
							WN_COPY_Tree(wn_index), 
							wn_offset);
			else
				wn_offset = WN_COPY_Tree(wn_index);
			wn_offset = WN_Binary(OPR_MPY, 
						MTYPE_U4, 
						wn_offset, 
						WN_Intconst(MTYPE_U4, TY_size(ty)));
			//wnx = WN_Lda( Pointer_type, 0, new_sym);
			//Set_TY_align(ST_type(new_sym), 4);
			WN* wn_ldidbase = WN_Ldid(Pointer_type, 0, new_sym, ST_type(new_sym));
			//WN_Lda( Pointer_type, 0, new_sym);
			//WN_Ldid(Pointer_type, 0, new_sym, ST_type(new_sym));
			WN* newtree = WN_Binary(OPR_ADD, 
						MTYPE_U4, 
						wn_offset, 
						wn_ldidbase);
			//WN_prev(newtree) = WN_prev(tree);
			//WN_next(newtree) = WN_next(tree);
			//WN* newtree = ACC_Make_Array_ref(new_sym, wn_offset, wn_dimInOne);
			WN_DELETE_Tree(tree);
			tree = (newtree);
		}
   }
  }
  else if (OPCODE_has_sym(op) && WN_st(tree)) 
  {
    old_sym = WN_st(tree);
    old_offset = OPCODE_has_offset(op) ? WN_offsetx(tree) : 0;
	
	map<ST*, ACC_VAR_TABLE>::iterator itor = acc_local_new_var_map.find(old_sym);
	
	if(itor != acc_local_new_var_map.end())	
	{
	  	ACC_VAR_TABLE newVar = itor->second;
	    //for (w=vtab; w->orig_st; w++) 
	      if ((newVar.orig_st == old_sym) &&
		  		(newVar.has_offset ? (newVar.orig_offset == old_offset) : TRUE )) 
		  {
			WN_st_idx(tree) = ST_st_idx(newVar.new_st);
			if (OPCODE_has_offset(op) && newVar.has_offset)
			  ACC_WN_set_offsetx(tree, newVar.new_offset);
	      }
	}
  }

  /* Walk all children */

  if (op == OPC_BLOCK) 
  {
    r = WN_first(tree);
    while (r) 
	{ // localize each node in block
      r = ACC_Walk_and_Localize ( r);
      if (WN_prev(r) == NULL)
        WN_first(tree) = r;
      if (WN_next(r) == NULL)
        WN_last(tree) = r;

      r = WN_next(r);
      
   }
  }
  else 
  {
    for (i=0; i < WN_kid_count(tree); i++)
    {
      WN_kid(tree, i) = ACC_Walk_and_Localize ( WN_kid(tree, i));
    }
  }
  return (tree);
}   


/***************************************************************/
/*  Process the contents of a parallel region.  
  *  single GPU kernel will be generated in this region */
/***************************************************************/
static void ACC_Handle_Parallel_Loops( WN* stmt_block, ParallelRegionInfo* pPRInfo, WN* wn_replace_block)
{  
  Transform_ACC_Parallel_Block(stmt_block, pPRInfo, wn_replace_block);
} // Process_ACC_Region()



/***************************************************************/
/*  Process the contents of a kernels region.  
  *  multi different kernels may include in this region */
/***************************************************************/
static void ACC_Handle_Kernels_Loops( WN* stmt_block, KernelsRegionInfo* pKRInfo, WN* wn_replace_block)
{
  Transform_ACC_Kernel_Block(stmt_block, pKRInfo, wn_replace_block);
} // Process_ACC_Region()

static WN *LaunchKernel (int index, WN* wn_replace_block, BOOL bParallel)
{	
	WN * wn;
	WN * wnx;
	WN * l;
	//int iParm = device_copyout.size() + device_copyin.size() + acc_parms_count;
	UINT32 i = 0;
	int parm_id = 0;
	//make the kernels parameters ready first
	Push_Kernel_Parameters(wn_replace_block, bParallel);

	//Then launch the kernel module
	//create whirl CALL
	wn = WN_Create(OPC_VCALL, 3 );
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_LAUNCHKERNEL);

	WN_Set_Call_IS_KERNEL_LAUNCH(wn);
	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
	WN_linenum(wn) = acc_line_number;
	//which kernel function
	char* kernelname = ST_name(acc_kernel_functions_st[index]);
	WN* wn_kernelname = WN_LdaString(kernelname,0, strlen(kernelname)+1);
	WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wn_kernelname, 
						 		WN_ty(wn_kernelname), WN_PARM_BY_VALUE);
	//which PTX file	
    char* srcfname = Last_Pathname_Component(Src_File_Name);   
    char* ptxfname = New_Extension ( srcfname, ".w2c.ptx");
	WN* wn_ptxname = WN_LdaString(ptxfname,0, strlen(ptxfname)+1);
	WN_kid(wn, 1) = WN_CreateParm(Pointer_type, wn_ptxname, 
						 		WN_ty(wn_ptxname), WN_PARM_BY_VALUE);
	if(acc_AsyncExpr)
	{
		WN_kid(wn, 2) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(acc_AsyncExpr), 
			  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
	}
	else 
		WN_kid(wn, 2) = WN_CreateParm(MTYPE_I4, WN_Intconst(MTYPE_I4, -2), 
			  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
	WN_INSERT_BlockLast(wn_replace_block, wn);
	return wn;
}

static ST * st_is_pcreate;
static WN_OFFSET ofst_st_is_pcreate;
static BOOL is_pcreate_tmp_created = FALSE;

extern UINT32 Enable_UHACCReductionFlag;


WN * 
lower_acc ( WN * block, WN * node, LOWER_ACTIONS actions )
{
  INT32 i;			/* Temporary index */
  INT32 vsize;			/* Var_table size */
  INT32 lsize;			/* label_info_table size */
  INT32 ssize;			/* Shared_table size */
  INT32 msize;			/* Mpnum_table size */
  BOOL cont;			/* Loop control */
  WN   *wn;			/* Temporary node */
  WN   *temp_node;		/* Temporary node */
  WN   *stmt1_block;		/* If true statement block */
  WN   *stmt2_block;		/* If false statement block */
  WN   *cur_node;		/* Current node within original nodes */
  WN   *next_node;		/* Next node in sequence */
  WN   *return_nodes;		/* Nodes to be returned */
  WN   *fp;			/* Frame pointer uplink */
  WN   *mpsched_wn;		/* Real wn for mpsched node */
  WN   *chunk_wn;		/* Real wn for chunk node */
  WN   *acc_call_wn;		/* Real wn for mp call */
  WN   *if_cond_wn;		/* Real wn for if condition */
  BOOL mp_if;			/* MP if transformation flag */
  ST   *lock_st;		/* ST for critical section lock */
  ST   *ntrip_st;		/* ST for loop trip count */
  ST   *return_st;		/* ST for mp status return */
  WN_OFFSET ntrip_ofst;		/* Offset for loop trip count */
  WN_OFFSET return_ofst;	/* Offset for mp status return */
  PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */
  INT32 num_criticals;		/* Number of nested critical sections */
  BOOL  while_seen;		/* While seen where do should be */
  WN   *task_fpsetup_blk; /* sets up firstprivate struct for task */
  
  WN* wn_cont_nodes = NULL;
  WN* wn_stmt_block = NULL;
  WN* wn_replace_block = NULL;  
  ACCP_process_type acc_process_type;


  /* Validate input arguments. */

  Is_True(actions & LOWER_ACC,
	  ("actions does not contain LOWER_ACC"));
  Is_True(((WN_opcode(node) == OPC_PRAGMA) ||
	   (WN_opcode(node) == OPC_XPRAGMA) ||
	   (WN_opcode(node) == OPC_IF) ||
	   (WN_opcode(node) == OPC_REGION)),
	  ("invalid acc node"));

	//Is_True(PU_Info_proc_sym(Current_PU_Info) == last_pu_proc_sym,
	//        ("LowerMP_PU_Init() not called for this PU"));
	/* Determine processing required based on first node. */
	acc_t = ACCP_UNKNOWN;
	UINT32 tmp_flag;
	tmp_flag = 2;
    acc_reduction_rolling = (ReductionRolling)((Enable_UHACCReductionFlag & tmp_flag)>>1);
	tmp_flag = 1;
    acc_reduction_mem = (ReductionUsingMem)(Enable_UHACCReductionFlag & tmp_flag);

	
	acc_stmt_block = NULL;	  /* Original statement nodes */
	acc_cont_nodes = NULL;	  /* Statement nodes after acc code */
	acc_if_node = NULL;	  /* Points to (optional) if node */
	acc_device_nodes = NULL;
	acc_host_nodes = NULL;
	acc_replace_block = NULL;
	acc_reduction_nodes = NULL; /* Points to (optional) reduction nodes */
	acc_copy_nodes = NULL;	  /* Points to (optional) shared nodes */
	acc_copyin_nodes = NULL;	  /* Points to (optional) copyin nodes */
	acc_copyout_nodes = NULL;   /* Points to (optional) copyout nodes */
	acc_wait_nodes = NULL;  /* Points to (optional) acc wait pragma nodes */
	acc_parms_nodes = NULL;
	acc_num_gangs_node = NULL;
	acc_num_workers_node = NULL;
	acc_vector_length_node = NULL;
	acc_collapse_node = NULL;
	acc_gang_node = NULL;
	acc_worker_node = NULL;
	acc_vector_node = NULL;
	acc_seq_node = NULL;
	acc_independent_node = NULL;


	acc_create_nodes = NULL;
	acc_present_nodes = NULL;
	acc_present_or_copy_nodes = NULL;
	acc_present_or_copyin_nodes = NULL;
	acc_present_or_copyout_nodes = NULL;
	acc_present_or_create_nodes = NULL;
	acc_deviceptr_nodes = NULL;
	acc_private_nodes = NULL;
	acc_pHeadLiveness = NULL;

	acc_dregion_pcreate.clear();
	acc_dregion_pcopy.clear();
	acc_dregion_pcopyin.clear();
	acc_dregion_pcopyout.clear();
	acc_dregion_present.clear();
	acc_dregion_host.clear();
	acc_dregion_device.clear();
	acc_dregion_private.clear();
	acc_dregion_fprivate.clear();
	/*acc_create_count = 0;
	acc_present_count = 0;
	acc_present_or_copy_count = 0;
	acc_present_or_copyin_count = 0;
	acc_present_or_copyout_count = 0;
	acc_present_or_create_count = 0;
	acc_deviceptr_count = 0;
	acc_private_count = 0;
	acc_firstprivate_count = 0;*/

	is_pcreate_tmp_created = FALSE;

  acc_collapse_count = 0;   /* collapse count */
  acc_clause_intnum = 0;	  /*Int expression, it's for integer expression, e.g, in wait pragma*/
  //acc_copyin_count = 0;  /* Count of copyins */
  //acc_copyout_count = 0; /* Count of copyouts */
  //acc_copy_count = 0;	  /* Count of copys */
  acc_async_nodes = 0;
  //acc_parms_count = 0;
  acc_reduction_count = 0;
  acc_kernel_functions_st.clear();
  //nested data region info reset
  acc_nested_dregion_info.Depth = 0;
  acc_nested_dregion_info.DRegionInfo.clear();
  
  ssize = 4096 * sizeof(ACC_SHARED_TABLE);
  acc_copyin_table = (ACC_SHARED_TABLE *) alloca ( ssize );
  BZERO ( acc_copyin_table, ssize );
  
  ssize = 4096 * sizeof(ACC_SHARED_TABLE);
  acc_copyout_table = (ACC_SHARED_TABLE *) alloca ( ssize );
  BZERO ( acc_copyout_table, ssize );
  
  ssize = 4096 * sizeof(ACC_SHARED_TABLE);
  acc_copy_table = (ACC_SHARED_TABLE *) alloca ( ssize );
  BZERO ( acc_copy_table, ssize );
  	
  ssize = 4096 * sizeof(ACC_SHARED_TABLE);
  acc_parm_table = (ACC_SHARED_TABLE *) alloca ( ssize );
  BZERO ( acc_parm_table, ssize );

  start_processing:

  if ((WN_opcode(node) == OPC_REGION) &&
	     WN_first(WN_region_pragmas(node)) &&
	     ((WN_opcode(WN_first(WN_region_pragmas(node))) == OPC_PRAGMA) ||
	      (WN_opcode(WN_first(WN_region_pragmas(node))) == OPC_XPRAGMA))) {

    WN *wtmp = WN_first(WN_region_pragmas(node));
    WN_PRAGMA_ID wid = (WN_PRAGMA_ID) WN_pragma(wtmp);


    switch (wid) {
      /* orphaned SINGLE region: process it now and return */
    case WN_PRAGMA_ACC_KERNELS_BEGIN:
      acc_process_type = ACCP_KERNEL_REGION;
      break;
    case WN_PRAGMA_ACC_PARALLEL_BEGIN:
      acc_process_type = ACCP_PARALLEL_REGION;
      break;
    case WN_PRAGMA_ACC_DATA_BEGIN:
      acc_process_type = ACCP_DATA_REGION;
      break;
    case WN_PRAGMA_ACC_HOSTDATA_BEGIN:
      acc_process_type = ACCP_HOST_DATA_REGION;
      break;	  
	case WN_PRAGMA_ACC_WAIT:
	  acc_process_type = ACCP_WAIT_REGION;
	  break;
    case WN_PRAGMA_ACC_UPDATE:
      acc_process_type = ACCP_UPDATE_REGION;
      break;
    case WN_PRAGMA_ACC_DECLARE:
      acc_process_type = ACCP_DECLARE_REGION;
      break;
    case WN_PRAGMA_ACC_CACHE:
      acc_process_type = ACCP_CACHE_REGION;
      break;
	  

    default:
      printf("pragma value = %d", (int)wid); /* for test. by jhs,02.9.3 */
      Fail_FmtAssertion (
		 "out of context pragma (%s) in acc {primary pragma} processing",
		 WN_pragmas[wid].name);
    }

    next_node = WN_next(wtmp);
    wn_cont_nodes = WN_next(node);
    wn_stmt_block = WN_region_body(node);

    //if (mpt != MPP_ORPHANED_PDO) 
    {
      WN_Delete ( wtmp );
      WN_Delete ( WN_region_pragmas(node) );
      WN_DELETE_Tree ( WN_region_exits(node) );
    }

  }

  
  ACC_Process_Clause_Pragma(next_node);

  if(wn_cont_nodes)
	WN_prev(wn_cont_nodes) = NULL;

	wn_replace_block = WN_CreateBlock();

	//Copy the data into device memory space
	device_copy.clear();
	device_copyin.clear();
	device_copyout.clear();
	device_create.clear();
	device_present.clear();
	device_present_or_copy.clear();
	device_present_or_copyin.clear();
	device_present_or_copyout.clear();
	device_present_or_create.clear();

	if (acc_process_type == ACCP_DATA_REGION)
	{
		//it will include any other constructs.
		//Get the information and move to the next stage
		SingleDRegionInfo sDRegionInfo;
		sDRegionInfo.acc_if_node = acc_if_node;
		//sDRegionInfo.acc_copy_nodes = acc_copy_nodes;		/* Points to (optional) copy nodes */
		//sDRegionInfo.acc_copyin_nodes = acc_copyin_nodes;	/* Points to (optional) copyin nodes */
		//sDRegionInfo.acc_copyout_nodes = acc_copyout_nodes;	/* Points to (optional) copyout nodes */	
		//sDRegionInfo.acc_create_nodes = acc_create_nodes;
		sDRegionInfo.acc_present_nodes = acc_present_nodes;
		sDRegionInfo.acc_present_or_copy_nodes = acc_present_or_copy_nodes;
		sDRegionInfo.acc_present_or_copyin_nodes = acc_present_or_copyin_nodes;
		sDRegionInfo.acc_present_or_copyout_nodes = acc_present_or_copyout_nodes;
		sDRegionInfo.acc_present_or_create_nodes = acc_present_or_create_nodes;
		sDRegionInfo.acc_deviceptr_nodes = acc_deviceptr_nodes;

		sDRegionInfo.acc_dregion_pcreate = acc_dregion_pcreate;		
		sDRegionInfo.acc_dregion_pcopy = acc_dregion_pcopy;		
		sDRegionInfo.acc_dregion_pcopyin = acc_dregion_pcopyin;		
		sDRegionInfo.acc_dregion_pcopyout = acc_dregion_pcopyout;		
		sDRegionInfo.acc_dregion_present = acc_dregion_present;		
		sDRegionInfo.acc_dregion_private = acc_dregion_private;		
		sDRegionInfo.acc_dregion_fprivate = acc_dregion_fprivate;
		
	    sDRegionInfo.wn_cont_nodes = wn_cont_nodes;
	    sDRegionInfo.wn_stmt_block = wn_stmt_block;
		
		  //////////////////////////////////////////////////////////////////////////
		  //Get def/use 
		  sDRegionInfo.acc_pHeadLiveness = NULL;
		  //ACC_Region_DefUseAnalysis(wn_stmt_block, &sDRegionInfo.acc_pHeadLiveness);
		  //Get in/out/private list
		  //ACC_Region_LivenessAnalysis(wn_cont_nodes, sDRegionInfo.acc_pHeadLiveness);
		  //ACC_Free_DefUse_Chain(&sDRegionInfo.acc_pHeadLiveness);
		  ///////////////////////////////////////////////////////////////////////////
		  
  		acc_nested_dregion_info.DRegionInfo.push_back(sDRegionInfo);
		acc_nested_dregion_info.Depth ++;
		
		//reset them
		acc_if_node = NULL;
		acc_copy_nodes = NULL;		/* Points to (optional) copy nodes */
		acc_copyin_nodes = NULL;	/* Points to (optional) copyin nodes */
		acc_copyout_nodes = NULL;	/* Points to (optional) copyout nodes */	
		acc_create_nodes = NULL;
		acc_present_nodes = NULL;
		acc_present_or_copy_nodes = NULL;
		acc_present_or_copyin_nodes = NULL;
		acc_present_or_copyout_nodes = NULL;
		acc_present_or_create_nodes = NULL;
		acc_deviceptr_nodes = NULL;
		
		acc_dregion_pcreate.clear();
		acc_dregion_pcopy.clear();
		acc_dregion_pcopyin.clear();
		acc_dregion_pcopyout.clear();
		acc_dregion_present.clear();
		acc_dregion_host.clear();
		acc_dregion_device.clear();
		acc_dregion_private.clear();
		acc_dregion_fprivate.clear();
		
		wn_replace_block = ACC_Process_DataRegion(wn_stmt_block);
  		acc_nested_dregion_info.DRegionInfo.pop_back();
		acc_nested_dregion_info.Depth --;
	}
	else if (acc_process_type == ACCP_KERNEL_REGION) 
	{
	  //generate kernel and launch the kernel
	  wn_replace_block = ACC_Process_KernelsRegion(wn_stmt_block, wn_cont_nodes);
	} 	 
	else if(acc_process_type == ACCP_HOST_DATA_REGION)
	{
	  //TODO for host construct region
	  wn_replace_block = ACC_Process_HostDataRegion(wn_stmt_block);
	}
	else if(acc_process_type == ACCP_PARALLEL_REGION)
	{
	  //It will include LOOP constructs
	  wn_replace_block = ACC_Process_ParallelRegion(wn_stmt_block, wn_cont_nodes);
	}		  	   	  
	else if (acc_process_type == ACCP_WAIT_REGION) 
	{
		  //Wait
	  wn_replace_block = ACC_Process_WaitRegion(wn_stmt_block);
	} 	  	  
	else if (acc_process_type == ACCP_DECLARE_REGION) 
	{
		  //Declare
	  wn_replace_block = ACC_Process_DeclareRegion(wn_stmt_block);
	} 	  	  
	else if (acc_process_type == ACCP_CACHE_REGION) 
	{
		  //Cache
	  wn_replace_block = ACC_Process_CacheRegion(wn_stmt_block);
	} 	  	  
	else if (acc_process_type == ACCP_UPDATE_REGION) 
	{
		  //Update
	  wn_replace_block = ACC_Process_UpdateRegion(wn_stmt_block);
	}  

  
  /* For all other parallel nodes return the replacement code chained with
       any code following the original parallel constructs. */

  if (WN_first(wn_replace_block)) 
  {
	  return_nodes = WN_first(wn_replace_block);
	  WN_next(WN_last(wn_replace_block)) = wn_cont_nodes;
	  if (wn_cont_nodes)
	  	WN_prev(wn_cont_nodes) = WN_last(wn_replace_block);
  } 
  else
	  return_nodes = wn_cont_nodes;

	WN_Delete (wn_replace_block );
    
    return (return_nodes);
}

static ST* ACC_GenSingleCreateAndMallocDeviceMem(ACC_DREGION__ENTRY dEntry, 
						vector<ACC_DATA_ST_MAP*>* pDMap, WN* ReplacementBlock)
{
	WN* dclause = dEntry.acc_data_clauses;
	ST *old_st = WN_st(dclause);
	WN_OFFSET old_offset = WN_offsetx(dclause);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__device_%s", ST_name(old_st) );
	
	if (kind == KIND_ARRAY|| kind == KIND_POINTER)
	{
		TY_IDX etype;
		if(kind == KIND_ARRAY)
			etype = ACC_Get_ElementTYForMultiArray(old_st);
		else
			etype = TY_pointed(ty);
		TY_IDX ty_p = Make_Pointer_Type(etype);
		ST *karg = NULL;
		WN *device_addr = NULL;
		WN* wnSize = ACC_GetArraySize(dEntry);
		WN* wnStart = ACC_GetArrayStart(dEntry);
		ACC_DATA_ST_MAP* pSTMap = new ACC_DATA_ST_MAP;
		karg = New_ST( CURRENT_SYMTAB );
		ST_Init(karg,
				Save_Str( localname ),
				CLASS_VAR,
				SCLASS_AUTO,
				EXPORT_LOCAL,
				ty_p);
		//Set_ST_is_value_parm( karg );
		pSTMap->deviceName = karg;
		pSTMap->hostName = old_st;
		pSTMap->wnSize = wnSize;
		pSTMap->wnStart = wnStart;
		pDMap->push_back(pSTMap);
		WN* WN_mallocall = Gen_DeviceMalloc(old_st, karg, WN_COPY_Tree(wnSize));
		WN_INSERT_BlockLast(ReplacementBlock, WN_mallocall);
		return karg;
	}
	else 
		Fail_FmtAssertion (
      "out of WN kind in ACC_GenDeviceCreateAndMalloc(...)");
	return NULL;
}

static WN* ACC_GenIsPCreate(ACC_DREGION__ENTRY dEntry)
{
	WN* wnSize = ACC_GetArraySize(dEntry);	
	WN* wnSizeInUnit = ACC_GetArraySizeInUnit(dEntry);
	WN* wnStart = ACC_GetArrayStart(dEntry);
	ST *old_st = WN_st(dEntry.acc_data_clauses);
	WN* wnAddr;
	if(TY_kind(ST_type(old_st)) == KIND_ARRAY)
		wnAddr = WN_Lda( Pointer_type, 0, old_st);
  	else
		wnAddr = WN_Ldid(Pointer_type, 0, old_st, ST_type(old_st));
	
	WN * wn;
	wn = WN_Create(OPC_I4CALL, 4 );
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_PRESENT_CREATE);

	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
	WN_linenum(wn) = acc_line_number;

	//wnx = WN_Lda( Pointer_type, 0, Src);
	WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnAddr, WN_ty(wnAddr), WN_PARM_BY_VALUE);  
	
	WN_kid(wn, 1) = WN_CreateParm(MTYPE_U4, wnStart, Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);

	WN_kid(wn, 2) = WN_CreateParm(MTYPE_U4, wnSizeInUnit, Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
	
	WN_kid(wn, 3) = WN_CreateParm(MTYPE_U4, wnSize, Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
	
	return wn;
}

//ACCR_GET_DEVICE_ADDR
static ST* ACC_GenDeclareSingleDeviceMem(ACC_DREGION__ENTRY dentry, 
						vector<ACC_DATA_ST_MAP*>* pDMap)
{
	WN* l = dentry.acc_data_clauses;
	ST *old_st = WN_st(l);
	WN_OFFSET old_offset = WN_offsetx(l);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__device_%s", ST_name(old_st) );
	
	if (kind == KIND_ARRAY|| kind == KIND_POINTER)
	{
		TY_IDX etype;
		if(kind == KIND_ARRAY)
			etype = TY_etype(ty);
		else
			etype = TY_pointed(ty);
		TY_IDX ty_p = Make_Pointer_Type(etype);
		ST *karg = NULL;
		WN *device_addr = NULL;
		WN* wnSize = ACC_GetArraySize(dentry);
		WN* wnStart = ACC_GetArrayStart(dentry);
		ACC_DATA_ST_MAP* pSTMap = new ACC_DATA_ST_MAP;
		karg = New_ST( CURRENT_SYMTAB );
		ST_Init(karg,
				Save_Str( localname ),
				CLASS_VAR,
				SCLASS_AUTO,
				EXPORT_LOCAL,
				ty_p);
		//Set_ST_is_value_parm( karg );
		pSTMap->deviceName = karg;
		pSTMap->hostName = old_st;
		pSTMap->wnSize = wnSize;
		pSTMap->wnStart = wnStart;
		pDMap->push_back(pSTMap);
		return karg;
		//return WN_mallocall;
		//Gen_DataH2D(old_st, karg, isize);
		//device_addr = WN_Lda( Pointer_type, 0, karg);
	}
	else 
		Fail_FmtAssertion (
      "out of WN kind in ACC_GenDeviceCreateAndMalloc(...)");
	return NULL;
}


static WN* ACC_Gen_GetDeviceAddr(ACC_DREGION__ENTRY dentry, ST* st_device)
{
	WN* wnSize = ACC_GetArraySize(dentry);
	WN* wnStart = ACC_GetArrayStart(dentry);
	ST *old_st = WN_st(dentry.acc_data_clauses);
	
	WN* wnAddr;
	if(TY_kind(ST_type(old_st)) == KIND_ARRAY)
		wnAddr = WN_Lda( Pointer_type, 0, old_st);
  	else
		wnAddr = WN_Ldid(Pointer_type, 0, old_st, ST_type(old_st));
	
	WN * wn;
	WN * wnx;
	wn = WN_Create(OPC_VCALL, 4 );
	WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_GET_DEVICE_ADDR);

	WN_Set_Call_Non_Data_Mod(wn);
	WN_Set_Call_Non_Data_Ref(wn);
	WN_Set_Call_Non_Parm_Mod(wn);
	WN_Set_Call_Non_Parm_Ref(wn);
	WN_Set_Call_Parm_Ref(wn);
	WN_linenum(wn) = acc_line_number;

	//wnx = WN_Lda( Pointer_type, 0, Src);
	WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnAddr, WN_ty(wnAddr), WN_PARM_BY_VALUE);  
	
	wnx = WN_Lda( Pointer_type, 0, st_device);
    WN_kid(wn, 1) = WN_CreateParm(Pointer_type, wnx, WN_ty(wnx), WN_PARM_BY_REFERENCE);
	
	WN_kid(wn, 2) = WN_CreateParm(MTYPE_U4, wnStart, Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);

	WN_kid(wn, 3) = WN_CreateParm(MTYPE_U4, wnSize, Be_Type_Tbl(MTYPE_U4), WN_PARM_BY_VALUE);
	
	return wn;
}

static void ACC_GenPresentNode(vector<ACC_DREGION__ENTRY>* pDREntries, vector<ACC_DATA_ST_MAP*>* pDMap, 
								WN* ReplacementBlock)
{
	//WN* l;	
	WN* dClause;
	WN* wnLength;
	int i=0;
	if(!is_pcreate_tmp_created)
	{
		ACC_Host_Create_Preg_or_Temp( MTYPE_I4, "_is_pcreate",
	                  &st_is_pcreate);
		is_pcreate_tmp_created = TRUE;
	}
	
	//////////////////////////////////////
	for (i=0; i<pDREntries->size(); i++) 
	//for (l = nodes; l; l = WN_next(l)) 
	{	  
	  ACC_DREGION__ENTRY dentry = (*pDREntries)[i];
	  dClause = dentry.acc_data_clauses;
	  wnLength = dentry.acc_data_length;
	  
	  WN* wnSize = ACC_GetArraySize(dentry);
	  WN* wnStart = ACC_GetArrayStart(dentry);
	  ST *old_st = WN_st(dClause);
	  WN_OFFSET old_offset = WN_offsetx(dClause);
	  ST* st_device = ACC_GenDeclareSingleDeviceMem(dentry, pDMap);

	  WN* wn_gen_device_addr = ACC_Gen_GetDeviceAddr(dentry, st_device);
	  WN_INSERT_BlockLast(ReplacementBlock, wn_gen_device_addr);
	}
}

static void ACC_GenDevicePtr(WN* nodes, vector<ST*>* pDMap)
{
	WN* l;		
	for (l = nodes; l; l = WN_next(l)) 
	{	  
	  //WN* wnSize = ACC_GetArraySize(l);
	  //WN* wnStart = ACC_GetArrayStart(l);
	  ST *dptr_st = WN_st(l);
	  pDMap->push_back(dptr_st);
	  //WN_OFFSET old_offset = WN_offsetx(l);
	  //ST* st_device = ACC_GenDeclareSingleDeviceMem(l, pDMap);

	  //WN* wn_gen_device_addr = ACC_Gen_GetDeviceAddr(l, st_device);
	  //WN_INSERT_BlockLast(ReplacementBlock, wn_gen_device_addr);
	}
}

//for create and copyout, MemIn should be false.
//for copy and copyin, MemIn should be true.
static void ACC_GenDeviceCreateCopyInOut(vector<ACC_DREGION__ENTRY>* pDREntries, 
								vector<ACC_DATA_ST_MAP*>* pDMap, 
								WN* ReplacementBlock, BOOL MemIn)
{
	WN* dClause;
	WN* wnLength;
	int i=0;
	if(!is_pcreate_tmp_created)
	{
		ACC_Host_Create_Preg_or_Temp( MTYPE_I4, "_is_pcreate",
	                  &st_is_pcreate);
		is_pcreate_tmp_created = TRUE;
	}
	
	//////////////////////////////////////
	for (i=0; i<pDREntries->size(); i++) 
	{
	  ACC_DREGION__ENTRY dentry = (*pDREntries)[i];
	  dClause = dentry.acc_data_clauses;
	  wnLength = dentry.acc_data_length;
	  
	  WN* wnSize = ACC_GetArraySize(dentry);
	  WN* wnStart = ACC_GetArrayStart(dentry);
	  ST *old_st = WN_st(dClause);
	  WN_OFFSET old_offset = WN_offsetx(dClause);
 	  WN* thenblock = WN_CreateBlock();
 	  WN* elseblock = WN_CreateBlock();
	  PREG_NUM rreg1, rreg2;	/* Pregs with I4 return values */;
	  //call is present function to check whether it has already been created.
      WN_INSERT_BlockLast( ReplacementBlock, ACC_GenIsPCreate(dentry));
      ACC_GET_RETURN_PREGS(rreg1, rreg2, MTYPE_I4);
      WN* wn_return = WN_Ldid(TY_mtype(ST_type(st_is_pcreate)), rreg1, Return_Val_Preg, ST_type(st_is_pcreate));
      WN* temp_node = WN_Stid(TY_mtype(ST_type(st_is_pcreate)), 0, 
	  								st_is_pcreate, ST_type(st_is_pcreate), wn_return);
	  
      //WN_linenum(temp_node) = acc_line_number;
	  
      WN_INSERT_BlockLast( ReplacementBlock, temp_node );
	  //if it is not exist create it
	  temp_node = WN_Ldid(TY_mtype(ST_type(st_is_pcreate)), 0, st_is_pcreate, ST_type(st_is_pcreate));	  	
	  WN* test = WN_Relational (OPR_EQ, TY_mtype(ST_type(st_is_pcreate)), 
	  								temp_node, WN_Intconst(MTYPE_I4, 0));
	  ST* st_dMem = ACC_GenSingleCreateAndMallocDeviceMem(dentry, pDMap, thenblock);
	  if(MemIn)
	  {
	  	WN* wn_h2d = Gen_DataH2D(old_st, st_dMem, WN_COPY_Tree(wnSize), WN_COPY_Tree(wnStart));
		WN_INSERT_BlockLast(thenblock, wn_h2d);		
	  }
	  WN* ifexp = WN_CreateIf(test, thenblock, elseblock);
	  
	  WN_INSERT_BlockLast(ReplacementBlock, ifexp);
	}
}

/*static void ACC_GenDataCopyIn(vector<ACC_DATA_ST_MAP*>* pDMap, WN* ReplacementBlock)
{	
	UINT32 i = 0;
	while(i<(*pDMap).size())
	{
		ACC_DATA_ST_MAP* pSTMap = (*pDMap)[i];
		WN* H2DCalls = Gen_DataH2D((*pDMap)[i]->hostName, 
									(*pDMap)[i]->deviceName, 
									WN_COPY_Tree((*pDMap)[i]->wnSize),
									WN_COPY_Tree((*pDMap)[i]->wnStart));
		WN_INSERT_BlockLast(ReplacementBlock, H2DCalls);
		i++;
	}
}*/
static WN* ACC_GenFreeDeviceMemory(ST* st_device_mem)
{
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 1);
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_FREE_ON_DEVICE);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  
  wnx = WN_Ldid(Pointer_type, 0, st_device_mem, ST_type(st_device_mem));

  //wnx = WN_Lda( Pointer_type, 0, Src);
  WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);
  
  return wn;
}

static void ACC_GenDeviceMemFreeInBatch(vector<ACC_DATA_ST_MAP*>* pDMap, WN* ReplacementBlock)
{	
	UINT32 i = 0;
	while(i<(*pDMap).size())
	{
		ACC_DATA_ST_MAP* pSTMap = (*pDMap)[i];
		WN* FeeDMemCalls = ACC_GenFreeDeviceMemory((*pDMap)[i]->deviceName);
		WN_INSERT_BlockLast(ReplacementBlock, FeeDMemCalls);
		i++;
	}
}


static void ACC_GenDataCopyOut(vector<ACC_DATA_ST_MAP*>* pDMap, WN* ReplacementBlock)
{	
	UINT32 i = 0;
	while(i<(*pDMap).size())
	{
		ACC_DATA_ST_MAP* pSTMap = (*pDMap)[i];
		WN* D2HCalls = Gen_DataD2H((*pDMap)[i]->deviceName, 
									(*pDMap)[i]->hostName, 
									WN_COPY_Tree((*pDMap)[i]->wnSize),
									WN_COPY_Tree((*pDMap)[i]->wnStart));
		WN_INSERT_BlockLast(ReplacementBlock, D2HCalls);
		i++;
	}
}

static WN* ACC_GenUpdateHostVar(ST* st_Var, WN* wn_start, WN* wn_length)
{	
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 4 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_UPDATE_HOST_VARIABLE);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  if(TY_kind(ST_type(st_Var)) == KIND_ARRAY)
	wnx = WN_Lda( Pointer_type, 0, st_Var);
  else
	wnx = WN_Ldid(Pointer_type, 0, st_Var, ST_type(st_Var));

  //wnx = WN_Lda( Pointer_type, 0, Src);
  WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);

  //wnx = WN_Lda( Pointer_type, 0, Dst);
  WN_kid(wn, 1) = WN_CreateParm(MTYPE_I4, wn_start, 
                       Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  
  WN_kid(wn, 2) = WN_CreateParm(MTYPE_I4, wn_length, 
		  			   Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  if(acc_AsyncExpr)
  {
  	WN_kid(wn, 3) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(acc_AsyncExpr), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  }
  else 
  	WN_kid(wn, 3) = WN_CreateParm(MTYPE_I4, WN_Intconst(MTYPE_I4, -2), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}


static WN* ACC_GenUpdateDeviceVar(ST* st_Var, WN* wn_start, WN* wn_length)
{	
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 4 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_UPDATE_DEVICE_VARIABLE);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  if(TY_kind(ST_type(st_Var)) == KIND_ARRAY)
	wnx = WN_Lda( Pointer_type, 0, st_Var);
  else
	wnx = WN_Ldid(Pointer_type, 0, st_Var, ST_type(st_Var));

  //wnx = WN_Lda( Pointer_type, 0, Src);
  WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);

  //wnx = WN_Lda( Pointer_type, 0, Dst);
  WN_kid(wn, 1) = WN_CreateParm(MTYPE_I4, wn_start, 
                       Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  
  WN_kid(wn, 2) = WN_CreateParm(MTYPE_I4, wn_length, 
		  			   Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  if(acc_AsyncExpr)
  {
  	WN_kid(wn, 3) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(acc_AsyncExpr), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);
  }
  else 
  	WN_kid(wn, 3) = WN_CreateParm(MTYPE_I4, WN_Intconst(MTYPE_I4, -2), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}

/*if wn_int_expr is "0", wait for all stream.
else, it will just wait for the specified stream.*/
static WN* ACC_GenWaitStream(WN* wn_int_expr)
{	
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 1 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_WAIT_SOME_OR_ALL_STREAM);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;
  
  WN_kid(wn, 0) = WN_CreateParm(MTYPE_I4, WN_COPY_Tree(wn_int_expr), 
  		  Be_Type_Tbl(MTYPE_I4), WN_PARM_BY_VALUE);

  return wn;
}


static void ACC_Process_Clause_Pragma(WN * tree)
{
	 // Process_PDO() parses clauses of orphanded PDO
	 //cont = (mpt != MPP_ORPHANED_PDO);
	 WN* wn_next_node = tree;
	 WN* wn_cur_node;
	 WN* wn;
	 while ((wn_cur_node = wn_next_node)) 
	 {
	
	   wn_next_node = WN_next(wn_cur_node);
	
	   if (((WN_opcode(wn_cur_node) == OPC_PRAGMA) ||
			(WN_opcode(wn_cur_node) == OPC_XPRAGMA)) &&
		   (WN_pragmas[WN_pragma(wn_cur_node)].users & PUSER_ACC)) 
	   {
	
		 {
	
	   switch (WN_pragma(wn_cur_node)) {
	
		 case WN_PRAGMA_ACC_CLAUSE_IF:
		   if (acc_if_node)
			 WN_DELETE_Tree ( acc_if_node );
		   acc_if_node = wn_cur_node;
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_REDUCTION:
		   for (wn = acc_reduction_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
			   break;
		   if (wn == NULL) 
		   {
				 if (WN_opcode(wn_cur_node) != OPC_PRAGMA &&
				   WN_operator(WN_kid0(wn_cur_node)) == OPR_ARRAY &&
				   OPCODE_has_sym(WN_opcode(WN_kid0(WN_kid0(wn_cur_node)))) == 0) 
				   {
					   WN_DELETE_Tree ( wn_cur_node );
				   } 
				   else 
				   {
					   WN_next(wn_cur_node) = acc_reduction_nodes;
					   acc_reduction_nodes = wn_cur_node;
					   ++acc_reduction_count;
				   }
		   } 
		   else
			 WN_DELETE_Tree ( wn_cur_node );
		   break;
		   
		 /*case WN_PRAGMA_ACC_CLAUSE_COPYOUT:
		   for (wn = acc_copyout_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
		   if (wn == NULL) {
			 WN_next(wn_cur_node) = acc_copyout_nodes;
			 acc_copyout_nodes = wn_cur_node;
			 ++acc_copyout_count;
			 if (TY_kind(ST_type(WN_st(wn_cur_node))) == KIND_SCALAR)
		   acc_copyout_table[acc_copyout_count] = WN_st(wn_cur_node);
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
	
		 case WN_PRAGMA_ACC_CLAUSE_COPYIN:
		   for (wn = acc_copyin_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
		   if (wn == NULL) {
			 WN_next(wn_cur_node) = acc_copyin_nodes;
			 acc_copyin_nodes = wn_cur_node;
			 ++acc_copyin_count;
			 if (TY_kind(ST_type(WN_st(wn_cur_node))) == KIND_SCALAR)
		   acc_copyin_table[acc_copyin_count] = WN_st(wn_cur_node);
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_CREATE:
		   for (wn = acc_create_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
		   if (wn == NULL) {
			 WN_next(wn_cur_node) = acc_create_nodes;
			 acc_create_nodes = wn_cur_node;
			 ++acc_create_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;*/
		 case WN_PRAGMA_ACC_CLAUSE_DATA_LENGTH:
		 	continue;
		   
		 case WN_PRAGMA_ACC_CLAUSE_PRESENT:
		   for (wn = acc_present_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
			 //the next node  must be node data length
		 //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
			 
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_present.push_back(entry);
			 /////////////////////////////////////////
			 WN_next(wn_cur_node) = acc_present_nodes;
			 acc_present_nodes = wn_cur_node;
			 //++acc_present_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_COPY:
		 case WN_PRAGMA_ACC_CLAUSE_PRESENT_OR_COPY:
		   for (wn = acc_present_or_copy_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
			 //the next node  must be node data length
		 //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_pcopy.push_back(entry);
			 ///////////////////////////////////////////
			 WN_next(wn_cur_node) = acc_present_or_copy_nodes;
			 acc_present_or_copy_nodes = wn_cur_node;
			 //++acc_present_or_copy_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_COPYIN:
		 case WN_PRAGMA_ACC_CLAUSE_PRESENT_OR_COPYIN:
		   for (wn = acc_present_or_copyin_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
			 //the next node  must be node data length
		 //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_pcopyin.push_back(entry);
			 ///////////////////////////////////////////
			 WN_next(wn_cur_node) = acc_present_or_copyin_nodes;
			 acc_present_or_copyin_nodes = wn_cur_node;
			 //++acc_present_or_copyin_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_COPYOUT:
		 case WN_PRAGMA_ACC_CLAUSE_PRESENT_OR_COPYOUT:
		   for (wn = acc_present_or_copyout_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
			 //the next node  must be node data length
		 //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_pcopyout.push_back(entry);
			 ///////////////////////////////////////////
			 WN_next(wn_cur_node) = acc_present_or_copyout_nodes;
			 acc_present_or_copyout_nodes = wn_cur_node;
			 //++acc_present_or_copyout_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_HOST:
		   for (wn = acc_host_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   		break;
		   //the next node  must be node data length
		   //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_host.push_back(entry);
			 ///////////////////////////////////////////
			 WN_next(wn_cur_node) = acc_host_nodes;
			 acc_host_nodes = wn_cur_node;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_DEVICE:
		   for (wn = acc_device_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   		break;
		   //the next node  must be node data length
		   //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_device.push_back(entry);
			 WN_next(wn_cur_node) = acc_device_nodes;
			 acc_device_nodes = wn_cur_node;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_CREATE:
		 case WN_PRAGMA_ACC_CLAUSE_PRESENT_OR_CREATE:
		   for (wn = acc_present_or_create_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
			 //the next node  must be node data length
		 //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_pcreate.push_back(entry);
			 //////////////////////////////////////
			 WN_next(wn_cur_node) = acc_present_or_create_nodes;
			 acc_present_or_create_nodes = wn_cur_node;
			 //++acc_present_or_create_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_DEVICEPTR:
		   for (wn = acc_deviceptr_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
		   if (wn == NULL) {
			 WN_next(wn_cur_node) = acc_deviceptr_nodes;
			 acc_deviceptr_nodes = wn_cur_node;
			 //++acc_deviceptr_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_PRIVATE:
		   for (wn = acc_private_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
		   //the next node  must be node data length
		   //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_private.push_back(entry);
			 //////////////////////////////////////
			 WN_next(wn_cur_node) = acc_private_nodes;
			 acc_private_nodes = wn_cur_node;
			 //++acc_private_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_FIRST_PRIVATE:
		   for (wn = acc_firstprivate_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
		   //the next node  must be node data length
		   //(WN_opcode(wn_next_node) == OPC_XPRAGMA)
		   acc_dlength_node = WN_COPY_Tree(WN_kid0(wn_next_node));
		   
		   if (wn == NULL) {
		   	 ACC_DREGION__ENTRY entry;
			 entry.acc_data_clauses = wn_cur_node;
			 entry.acc_data_length = acc_dlength_node;
			 acc_dregion_fprivate.push_back(entry);
			 //////////////////////////////////////
			 WN_next(wn_cur_node) = acc_firstprivate_nodes;
			 acc_firstprivate_nodes = wn_cur_node;
			 //++acc_firstprivate_count;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_NUM_GANGS:
		   wn = acc_num_gangs_node; //only one stmt allowed.
		   if (wn == NULL) {
			 WN_next(wn_cur_node) = acc_num_gangs_node;
			 acc_num_gangs_node = wn_cur_node;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_NUM_WORKERS:
		   wn = acc_num_workers_node; //only one stmt allowed.
		   if (wn == NULL) {
			 WN_next(wn_cur_node) = acc_num_workers_node;
			 acc_num_workers_node = wn_cur_node;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_VECTOR_LENGTH:
		   wn = acc_vector_length_node; //only one stmt allowed.
		   if (wn == NULL) {
			 WN_next(wn_cur_node) = acc_vector_length_node;
			 acc_vector_length_node = wn_cur_node;
		   } else
			 WN_Delete ( wn_cur_node );
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_PARM:
		   for (wn = acc_parms_nodes; wn; wn = WN_next(wn))
			 if (ACC_Identical_Pragmas(wn_cur_node, wn))
		   break;
		   if (wn == NULL) 
		   {
			 WN_next(wn_cur_node) = acc_parms_nodes;
			 acc_parms_nodes = wn_cur_node;
			 //++acc_parms_count;
		   } 
		   else
			 WN_Delete ( wn_cur_node );
		   break;
	
		 /*case WN_PRAGMA_NUMTHREADS:
		   if (numthreads_node)
			 WN_DELETE_Tree ( numthreads_node );
		   numthreads_node = cur_node;
		   break;*/
		 case WN_PRAGMA_ACC_CLAUSE_ASYNC:
		   acc_async_nodes = wn_cur_node;
		   break;
		   
		 case WN_PRAGMA_ACC_CLAUSE_INTEXP:
			acc_clause_intnum = wn_cur_node;
			break;
	
		 default:
			Fail_FmtAssertion ("out of context pragma (%s) in ACC {top-level pragma} processing",
								WN_pragmas[WN_pragma(wn_cur_node)].name);
	
	   }	
	  }	
	 } 	
	}
}

/**********************************************************************************/
//Generate GPU kernels parameters from acc kernels/parallel block
//isKernelRegion, if it is acc kernels region, should be set as TRUE.
// 				if it is the acc parallel region, should be set as FALSE.
/**********************************************************************************/

static WN* ACC_Generate_KernelParameters(WN* paramlist, void* pRegionInfo, BOOL isKernelRegion)
{
	ParallelRegionInfo* pParallelRegionInfo;
	KernelsRegionInfo* pKernelsRegionInfo;
	WN* wn;
	if(isKernelRegion)
	{
		pKernelsRegionInfo = (KernelsRegionInfo*)pRegionInfo;
		//Get a pair list for kernel parameters.
		for(wn=paramlist; wn; wn=WN_next(wn))
		{
			ST* st_param = WN_st(wn);
			int i = 0;
			BOOL isFound = FALSE;
			int j = 0;
				
			TY_IDX ty = ST_type(st_param);
			TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			if(kind == KIND_SCALAR)
			{
				KernelParameter kparam;
				isFound = TRUE;
				kparam.st_host = st_param;
				kparam.st_device = NULL;
				acc_kernelLaunchParamList.push_back(kparam);
				continue;
			}
			//if it is scalar var, skip it.
			//let's traverse all the tables
			while(j < acc_nested_dregion_info.Depth)
			{
				//acc_nested_dregion_info.DRegionInfo[j];
				//
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyinMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcreateMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].presentMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].dptrList.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].dptrList[i] == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = NULL;
						kparam.st_device = st_param;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				
				j++;
			}
			if(isFound == TRUE)
				continue;

			//Kernels Table
			while(i < pKernelsRegionInfo->pcopyMap.size())
			{
				if(pKernelsRegionInfo->pcopyMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pKernelsRegionInfo->pcopyMap[i]->hostName;
					kparam.st_device = pKernelsRegionInfo->pcopyMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pKernelsRegionInfo->pcopyinMap.size())
			{
				if(pKernelsRegionInfo->pcopyinMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pKernelsRegionInfo->pcopyinMap[i]->hostName;
					kparam.st_device = pKernelsRegionInfo->pcopyinMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pKernelsRegionInfo->pcopyoutMap.size())
			{
				if(pKernelsRegionInfo->pcopyoutMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pKernelsRegionInfo->pcopyoutMap[i]->hostName;
					kparam.st_device = pKernelsRegionInfo->pcopyoutMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pKernelsRegionInfo->pcreateMap.size())
			{
				if(pKernelsRegionInfo->pcreateMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pKernelsRegionInfo->pcreateMap[i]->hostName;
					kparam.st_device = pKernelsRegionInfo->pcreateMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pKernelsRegionInfo->presentMap.size())
			{
				if(pKernelsRegionInfo->presentMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pKernelsRegionInfo->presentMap[i]->hostName;
					kparam.st_device = pKernelsRegionInfo->presentMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == TRUE)
				continue;
			
			i = 0;
			while(i < pKernelsRegionInfo->dptrList.size())
			{
				if(pKernelsRegionInfo->dptrList[i] == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = NULL;
					kparam.st_device = st_param;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == FALSE)
			  Fail_FmtAssertion("ACC_Generate_KernelParameters: illegal parameters, kernels param:%s undefined in previous acc pragma.",
									ST_name(st_param));
		}
	}
	else
	{
		pParallelRegionInfo = (ParallelRegionInfo*)pRegionInfo;
		//Get a pair list for kernel parameters.
		for(wn=paramlist; wn; wn=WN_next(wn))
		{
			ST* st_param = WN_st(wn);
			int i = 0;
			BOOL isFound = FALSE;
			int j = 0;
				
			TY_IDX ty = ST_type(st_param);
			TY_KIND kind = TY_kind(ty);//ST_name(old_st)
			if(kind == KIND_SCALAR)
			{
				KernelParameter kparam;
				isFound = TRUE;
				kparam.st_host = st_param;
				kparam.st_device = NULL;
				acc_kernelLaunchParamList.push_back(kparam);
				continue;
			}
			
			//let's traverse all the tables
			while(j < acc_nested_dregion_info.Depth)
			{
				//acc_nested_dregion_info.DRegionInfo[j];
				//
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcopyMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyinMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcopyinMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcopyoutMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].pcreateMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].pcreateMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].presentMap.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->hostName == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->hostName;
						kparam.st_device = acc_nested_dregion_info.DRegionInfo[j].presentMap[i]->deviceName;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
				
				i = 0;
				while(i < acc_nested_dregion_info.DRegionInfo[j].dptrList.size())
				{
					if(acc_nested_dregion_info.DRegionInfo[j].dptrList[i] == st_param)
					{
						KernelParameter kparam;
						isFound = TRUE;
						kparam.st_host = NULL;
						kparam.st_device = st_param;
						acc_kernelLaunchParamList.push_back(kparam);
						break;
					}
					i ++;
				}
				
				if(isFound == TRUE)
					break;
			
				
				j++;
			}
			if(isFound == TRUE)
				continue;

			//Kernels Table
			while(i < pParallelRegionInfo->pcopyMap.size())
			{
				if(pParallelRegionInfo->pcopyMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pParallelRegionInfo->pcopyMap[i]->hostName;
					kparam.st_device = pParallelRegionInfo->pcopyMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pParallelRegionInfo->pcopyinMap.size())
			{
				if(pParallelRegionInfo->pcopyinMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pParallelRegionInfo->pcopyinMap[i]->hostName;
					kparam.st_device = pParallelRegionInfo->pcopyinMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pParallelRegionInfo->pcopyoutMap.size())
			{
				if(pParallelRegionInfo->pcopyoutMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pParallelRegionInfo->pcopyoutMap[i]->hostName;
					kparam.st_device = pParallelRegionInfo->pcopyoutMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pParallelRegionInfo->pcreateMap.size())
			{
				if(pParallelRegionInfo->pcreateMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pParallelRegionInfo->pcreateMap[i]->hostName;
					kparam.st_device = pParallelRegionInfo->pcreateMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pParallelRegionInfo->presentMap.size())
			{
				if(pParallelRegionInfo->presentMap[i]->hostName == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = pParallelRegionInfo->presentMap[i]->hostName;
					kparam.st_device = pParallelRegionInfo->presentMap[i]->deviceName;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
				
			if(isFound == TRUE)
				continue;
			i = 0;
			while(i < pParallelRegionInfo->dptrList.size())
			{
				if(pParallelRegionInfo->dptrList[i] == st_param)
				{
					KernelParameter kparam;
					isFound = TRUE;
					kparam.st_host = NULL;
					kparam.st_device = st_param;
					acc_kernelLaunchParamList.push_back(kparam);
					break;
				}
				i ++;
			}
			
			if(isFound == FALSE)
			  Fail_FmtAssertion("ACC_Generate_KernelParameters: illegal parameters, parallel param:%s undefined in previous acc pragma",
									ST_name(st_param));
		}
	}
}

static WN* ACC_Process_KernelsRegion( WN * tree, WN* wn_cont)
{
	KernelsRegionInfo kernelsRegionInfo;
	WN* wn;
	
	WN* kernelsBlock = WN_CreateBlock();
	acc_reduction_count = 0;
	//ignore if, present, deviceptr clauses
	kernelsRegionInfo.acc_if_node = acc_if_node;
	//sDRegionInfo.acc_copy_nodes = acc_copy_nodes; 	  /* Points to (optional) copy nodes */
	//sDRegionInfo.acc_copyin_nodes = acc_copyin_nodes;   /* Points to (optional) copyin nodes */
	//sDRegionInfo.acc_copyout_nodes = acc_copyout_nodes; /* Points to (optional) copyout nodes */	  
	//sDRegionInfo.acc_create_nodes = acc_create_nodes;
	kernelsRegionInfo.acc_present_nodes = acc_present_nodes;
	kernelsRegionInfo.acc_present_or_copy_nodes = acc_present_or_copy_nodes;
	kernelsRegionInfo.acc_present_or_copyin_nodes = acc_present_or_copyin_nodes;
	kernelsRegionInfo.acc_present_or_copyout_nodes = acc_present_or_copyout_nodes;
	kernelsRegionInfo.acc_present_or_create_nodes = acc_present_or_create_nodes;
	kernelsRegionInfo.acc_deviceptr_nodes = acc_deviceptr_nodes;
	kernelsRegionInfo.acc_async = acc_async_nodes;
	kernelsRegionInfo.acc_param = acc_parms_nodes;

	kernelsRegionInfo.acc_dregion_pcreate = acc_dregion_pcreate;		
	kernelsRegionInfo.acc_dregion_pcopy = acc_dregion_pcopy;		
	kernelsRegionInfo.acc_dregion_pcopyin = acc_dregion_pcopyin;		
	kernelsRegionInfo.acc_dregion_pcopyout = acc_dregion_pcopyout;		
	kernelsRegionInfo.acc_dregion_present = acc_dregion_present;
	kernelsRegionInfo.acc_dregion_private = acc_dregion_private;		
	kernelsRegionInfo.acc_dregion_fprivate = acc_dregion_fprivate;

	//async expr
	if(acc_async_nodes)
	{		
		acc_AsyncExpr = WN_COPY_Tree(WN_kid0(acc_async_nodes));
		WN_Delete(acc_async_nodes); 
	}
	//declare the data	
	if(acc_present_or_copy_nodes)
	{
		//declaration, cuda malloc 
		ACC_GenDeviceCreateCopyInOut(&kernelsRegionInfo.acc_dregion_pcopy, 
  										&kernelsRegionInfo.pcopyMap, 
  										kernelsBlock, TRUE);
	}
	if(acc_present_or_copyin_nodes)
	{
		//declaration, cuda malloc 
		ACC_GenDeviceCreateCopyInOut(&kernelsRegionInfo.acc_dregion_pcopyin, 
  										&kernelsRegionInfo.pcopyinMap, 
  										kernelsBlock, TRUE);
	}
	if(acc_present_or_copyout_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenDeviceCreateCopyInOut(&kernelsRegionInfo.acc_dregion_pcopyout, 
  										&kernelsRegionInfo.pcopyoutMap, 
  										kernelsBlock, FALSE);
	}
	if(acc_present_or_create_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenDeviceCreateCopyInOut(&kernelsRegionInfo.acc_dregion_pcreate, 
  										&kernelsRegionInfo.pcreateMap, 
  										kernelsBlock, FALSE);
	}
	if(acc_present_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenPresentNode(&kernelsRegionInfo.acc_dregion_present, 
  										&kernelsRegionInfo.presentMap, 
  										kernelsBlock);
	}
	if(acc_deviceptr_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenDevicePtr(acc_deviceptr_nodes, 
  										&kernelsRegionInfo.dptrList);
	}
	
	//reset them
	acc_if_node = NULL;
	acc_copy_nodes = NULL;	  /* Points to (optional) copy nodes */
	acc_copyin_nodes = NULL;	  /* Points to (optional) copyin nodes */
	acc_copyout_nodes = NULL;   /* Points to (optional) copyout nodes */	  
	acc_create_nodes = NULL;
	acc_present_nodes = NULL;
	acc_present_or_copy_nodes = NULL;
	acc_present_or_copyin_nodes = NULL;
	acc_present_or_copyout_nodes = NULL;
	acc_present_or_create_nodes = NULL;
	acc_deviceptr_nodes = NULL;
	acc_async_nodes = NULL;
			  
	acc_dregion_pcreate.clear();
	acc_dregion_pcopy.clear();
	acc_dregion_pcopyin.clear();
	acc_dregion_pcopyout.clear();
	acc_dregion_present.clear();
	acc_dregion_host.clear();
	acc_dregion_device.clear();
	acc_dregion_private.clear();
	acc_dregion_fprivate.clear();
	//Handling the data analysis	
	
	//Generating kernel parameter lists from in/out chain
	/*ACC_Region_DefUseAnalysis(wn_stmt_block, &acc_pHeadLiveness);
	ACC_Region_LivenessAnalysis_cont(wn_cont_nodes, acc_pHeadLiveness);
	i = acc_nested_dregion_info.Depth - 1;
	while(i>=0)
	{
		SingleDRegionInfo sDRegionInfo = 
		acc_nested_dregion_info.DRegionInfo[i];
	ACC_Region_LivenessAnalysis_cont(sDRegionInfo.wn_cont_nodes, acc_pHeadLiveness);
	i--;
	}
	ACC_Finalize_Liveness_Chain(acc_pHeadLiveness);
	ACC_Retrieve_Kernel_Param_From_Liveness(acc_pHeadLiveness);
	ACC_Dump_InOutAnalysis(acc_pHeadLiveness);
	ACC_Free_DefUse_Chain(&acc_pHeadLiveness);*/
	//Generating kernel parameter lists from in/out chain
	ACC_Region_DefUseAnalysis(tree, &acc_pHeadLiveness, &kernelsRegionInfo, TRUE);
	ACC_Region_LivenessAnalysis_cont(wn_cont, acc_pHeadLiveness);
	int i = acc_nested_dregion_info.Depth - 1;
	while(i>=0)
	{
	  	SingleDRegionInfo sDRegionInfo = 
			acc_nested_dregion_info.DRegionInfo[i];
		ACC_Region_LivenessAnalysis_cont(sDRegionInfo.wn_cont_nodes, acc_pHeadLiveness);
		i--;
	}

	ACC_Finalize_Liveness_Chain(acc_pHeadLiveness);
	ACC_Retrieve_Kernel_Param_From_Liveness(acc_pHeadLiveness);
	kernelsRegionInfo.acc_param = acc_parms_nodes;
	ACC_Dump_InOutAnalysis(acc_pHeadLiveness);
	ACC_Free_DefUse_Chain(&acc_pHeadLiveness);
	//Generate parameters
	acc_kernelLaunchParamList.clear();
	ACC_Generate_KernelParameters(acc_parms_nodes, (void*)&kernelsRegionInfo, TRUE);
    //multi kernels and multi loopnest process
    ACC_Handle_Kernels_Loops(tree, &kernelsRegionInfo, kernelsBlock);
	acc_kernel_functions_st.clear();
	//Generate the kernel launch function.
	//WN_INSERT_BlockLast(acc_replace_block, LauchKernel(0));	
	//Generate the data copy back function
	if(kernelsRegionInfo.acc_present_or_copyout_nodes)
		ACC_GenDataCopyOut(&kernelsRegionInfo.pcopyoutMap, kernelsBlock);
	if(kernelsRegionInfo.acc_present_or_copy_nodes)
		ACC_GenDataCopyOut(&kernelsRegionInfo.pcopyMap, kernelsBlock);
	
	if(acc_AsyncExpr)
		WN_Delete(acc_AsyncExpr);
	acc_AsyncExpr = NULL;
	/****************************************************************************/
	//Free device memory
	if(kernelsRegionInfo.acc_present_or_copyin_nodes)
		ACC_GenDeviceMemFreeInBatch(&kernelsRegionInfo.pcopyinMap, kernelsBlock);
	if(kernelsRegionInfo.acc_present_or_copyout_nodes)
		ACC_GenDeviceMemFreeInBatch(&kernelsRegionInfo.pcopyoutMap, kernelsBlock);
	if(kernelsRegionInfo.acc_present_or_copy_nodes)
		ACC_GenDeviceMemFreeInBatch(&kernelsRegionInfo.pcopyMap, kernelsBlock);
	if(kernelsRegionInfo.acc_present_or_create_nodes)
		ACC_GenDeviceMemFreeInBatch(&kernelsRegionInfo.pcreateMap, kernelsBlock);
	return kernelsBlock;
}

static WN* ACC_Process_Block_Transform(WN * tree )
{
  WN* node = tree;
  ACCP_process_type acc_process_type;
  
  WN* wn_next_node; // = WN_next(wtmp);
  WN* wn_cont_nodes;// = WN_next(node);
  WN* wn_stmt_block;// = WN_region_body(node);
  WN* cur_node, *next_node, *prev_node;
  WN* replacement_block = WN_CreateBlock();
  
  for (cur_node = WN_first(tree); cur_node; cur_node = next_node)  
  {
    prev_node = WN_prev(cur_node);
    next_node = WN_next(cur_node);
	if ((WN_opcode(cur_node) == OPC_REGION) &&
			 WN_first(WN_region_pragmas(cur_node)) &&
			 ((WN_opcode(WN_first(WN_region_pragmas(cur_node))) == OPC_PRAGMA) ||
			  (WN_opcode(WN_first(WN_region_pragmas(cur_node))) == OPC_XPRAGMA)))
	{
	    WN *wtmp = WN_first(WN_region_pragmas(cur_node));
	    WN_PRAGMA_ID wid = (WN_PRAGMA_ID) WN_pragma(wtmp);
		WN* subBlock;

	    switch (wid) {
	      /* orphaned SINGLE region: process it now and return */
	    case WN_PRAGMA_ACC_KERNELS_BEGIN:
	      acc_process_type = ACCP_KERNEL_REGION;
	      break;
	    case WN_PRAGMA_ACC_PARALLEL_BEGIN:
	      acc_process_type = ACCP_PARALLEL_REGION;
	      break;
	    case WN_PRAGMA_ACC_DATA_BEGIN:
	      acc_process_type = ACCP_DATA_REGION;
	      break;
	    case WN_PRAGMA_ACC_HOSTDATA_BEGIN:
	      acc_process_type = ACCP_HOST_DATA_REGION;
	      break;	  
		case WN_PRAGMA_ACC_WAIT:
		  acc_process_type = ACCP_WAIT_REGION;
		  break;
	    case WN_PRAGMA_ACC_UPDATE:
	      acc_process_type = ACCP_UPDATE_REGION;
	      break;
	    case WN_PRAGMA_ACC_DECLARE:
	      acc_process_type = ACCP_DECLARE_REGION;
	      break;
	    case WN_PRAGMA_ACC_CACHE:
	      acc_process_type = ACCP_CACHE_REGION;
	      break;		  

	    default:
	      printf("pragma value = %d", (int)wid); /* for test. by jhs,02.9.3 */
	      Fail_FmtAssertion (
			 "out of context pragma (%s) in acc {primary pragma} processing",
			 WN_pragmas[wid].name);
	    }
		
	    wn_next_node = WN_next(wtmp);
	    wn_cont_nodes = WN_next(cur_node);
	    wn_stmt_block = WN_region_body(cur_node);
		if (acc_process_type == WN_PRAGMA_ACC_WAIT
			|| acc_process_type == WN_PRAGMA_ACC_UPDATE
			|| acc_process_type == WN_PRAGMA_ACC_DECLARE
			|| acc_process_type == WN_PRAGMA_ACC_CACHE) 
	    {
	      WN_Delete ( wtmp );
	      WN_Delete ( WN_region_pragmas(cur_node) );
	      WN_DELETE_Tree ( WN_region_exits(cur_node) );
	    }
		
		///////////////////////////////////////////
		//process clauses
		ACC_Process_Clause_Pragma(wn_next_node);
		//Begin to process the body
		if (acc_process_type == ACCP_DATA_REGION)
		{
			  //it will include any other constructs.
			  //Get the information and move to the next stage
			  SingleDRegionInfo sDRegionInfo;
			  sDRegionInfo.acc_if_node = acc_if_node;
			  //sDRegionInfo.acc_copy_nodes = acc_copy_nodes; 	  /* Points to (optional) copy nodes */
			  //sDRegionInfo.acc_copyin_nodes = acc_copyin_nodes;   /* Points to (optional) copyin nodes */
			  //sDRegionInfo.acc_copyout_nodes = acc_copyout_nodes; /* Points to (optional) copyout nodes */	  
			  //sDRegionInfo.acc_create_nodes = acc_create_nodes;
			  sDRegionInfo.acc_present_nodes = acc_present_nodes;
			  sDRegionInfo.acc_present_or_copy_nodes = acc_present_or_copy_nodes;
			  sDRegionInfo.acc_present_or_copyin_nodes = acc_present_or_copyin_nodes;
			  sDRegionInfo.acc_present_or_copyout_nodes = acc_present_or_copyout_nodes;
			  sDRegionInfo.acc_present_or_create_nodes = acc_present_or_create_nodes;
			  sDRegionInfo.acc_deviceptr_nodes = acc_deviceptr_nodes;
			  acc_nested_dregion_info.DRegionInfo.push_back(sDRegionInfo);
			  acc_nested_dregion_info.Depth ++;
		
			  //reset them
			  acc_if_node = NULL;
			  acc_copy_nodes = NULL;	  /* Points to (optional) copy nodes */
			  acc_copyin_nodes = NULL;	  /* Points to (optional) copyin nodes */
			  acc_copyout_nodes = NULL;   /* Points to (optional) copyout nodes */	  
			  acc_create_nodes = NULL;
			  acc_present_nodes = NULL;
			  acc_present_or_copy_nodes = NULL;
			  acc_present_or_copyin_nodes = NULL;
			  acc_present_or_copyout_nodes = NULL;
			  acc_present_or_create_nodes = NULL;
			  acc_deviceptr_nodes = NULL;
			  //cotinue process the body
			  subBlock = ACC_Process_DataRegion(wn_stmt_block);
			  acc_nested_dregion_info.DRegionInfo.pop_back();
			  acc_nested_dregion_info.Depth --;
		  }
		  else if(acc_process_type == ACCP_HOST_DATA_REGION)
		  {
		      //TODO for host construct region
		      subBlock = ACC_Process_HostDataRegion(wn_stmt_block);
		  }
		  else if(acc_process_type == ACCP_PARALLEL_REGION)
		  {
			  //It will include LOOP constructs
		      subBlock = ACC_Process_ParallelRegion(wn_stmt_block, wn_cont_nodes);
		  }		  	  
		  else if (acc_process_type == ACCP_KERNEL_REGION) 
		  {
		  	  //generate kernel and launch the kernel
		      subBlock = ACC_Process_KernelsRegion(wn_stmt_block, wn_cont_nodes);
		  } 	  	  
		  else if (acc_process_type == ACCP_WAIT_REGION) 
		  {
		  	  //Wait
		      subBlock = ACC_Process_WaitRegion(wn_stmt_block);
		  }
		  else if (acc_process_type == ACCP_DECLARE_REGION) 
		  {
		  	  //Declare
		      subBlock = ACC_Process_DeclareRegion(wn_stmt_block);
		  } 	  	  
		  else if (acc_process_type == ACCP_CACHE_REGION) 
		  {
		  	  //Cache
		      subBlock = ACC_Process_CacheRegion(wn_stmt_block);
		  } 	  	  
		  else if (acc_process_type == ACCP_UPDATE_REGION) 
		  {
		  	  //Update
		      subBlock = ACC_Process_UpdateRegion(wn_stmt_block);
		  } 

		  WN_INSERT_BlockLast(replacement_block, subBlock);
		
	}
	else if(WN_opcode(cur_node) != OPC_REGION_EXIT)//normal statement
	{
		int ikid = 0;
		//in case of the ACC pragma in the while-do/if-stmt etc.
		for (ikid = 0; ikid < WN_kid_count(cur_node); ikid++)
        if (WN_kid(cur_node, ikid) &&
           (WN_opcode(WN_kid(cur_node, ikid)) == OPC_BLOCK))
              WN_kid(cur_node, ikid) = ACC_Process_Block_Transform( WN_kid(cur_node, ikid) );
		
		WN_INSERT_BlockLast(replacement_block, cur_node);
	}
  }
    
  return replacement_block;  
}


static WN* ACC_Process_ParallelRegion( WN * tree, WN* wn_cont)
{
	ParallelRegionInfo parallelRegionInfo;
	WN* wn;
	
	WN* parallelBlock = WN_CreateBlock();
	//ignore if, present, deviceptr clauses
	parallelRegionInfo.acc_if_node = acc_if_node;
	//sDRegionInfo.acc_copy_nodes = acc_copy_nodes; 	  /* Points to (optional) copy nodes */
	//sDRegionInfo.acc_copyin_nodes = acc_copyin_nodes;   /* Points to (optional) copyin nodes */
	//sDRegionInfo.acc_copyout_nodes = acc_copyout_nodes; /* Points to (optional) copyout nodes */	  
	//sDRegionInfo.acc_create_nodes = acc_create_nodes;
	parallelRegionInfo.acc_present_nodes = acc_present_nodes;
	parallelRegionInfo.acc_present_or_copy_nodes = acc_present_or_copy_nodes;
	parallelRegionInfo.acc_present_or_copyin_nodes = acc_present_or_copyin_nodes;
	parallelRegionInfo.acc_present_or_copyout_nodes = acc_present_or_copyout_nodes;
	parallelRegionInfo.acc_present_or_create_nodes = acc_present_or_create_nodes;
	parallelRegionInfo.acc_deviceptr_nodes = acc_deviceptr_nodes;
	parallelRegionInfo.acc_async = acc_async_nodes;
	parallelRegionInfo.acc_param = acc_parms_nodes;
	parallelRegionInfo.acc_private = acc_private_nodes;
	parallelRegionInfo.acc_firstprivate = acc_firstprivate_nodes;
	parallelRegionInfo.acc_num_gangs = acc_num_gangs_node;
	parallelRegionInfo.acc_num_workers = acc_num_workers_node;
	parallelRegionInfo.acc_vector_length = acc_vector_length_node;
	parallelRegionInfo.acc_reduction = acc_reduction_nodes;

	parallelRegionInfo.acc_dregion_pcreate = acc_dregion_pcreate;		
	parallelRegionInfo.acc_dregion_pcopy = acc_dregion_pcopy;		
	parallelRegionInfo.acc_dregion_pcopyin = acc_dregion_pcopyin;		
	parallelRegionInfo.acc_dregion_pcopyout = acc_dregion_pcopyout;		
	parallelRegionInfo.acc_dregion_present = acc_dregion_present;
	parallelRegionInfo.acc_dregion_private = acc_dregion_private;		
	parallelRegionInfo.acc_dregion_fprivate = acc_dregion_fprivate;

	//async expr
	if(acc_async_nodes)
	{		
		acc_AsyncExpr = WN_COPY_Tree(WN_kid0(acc_async_nodes));
		WN_Delete(acc_async_nodes); 
	}
	//declare the data	
	if(acc_present_or_copy_nodes)
	{
		//declaration, cuda malloc 
		ACC_GenDeviceCreateCopyInOut(&parallelRegionInfo.acc_dregion_pcopy, 
  										&parallelRegionInfo.pcopyMap, 
  										parallelBlock, TRUE);
	}
	if(acc_present_or_copyin_nodes)
	{
		//declaration, cuda malloc 
		ACC_GenDeviceCreateCopyInOut(&parallelRegionInfo.acc_dregion_pcopyin, 
  										&parallelRegionInfo.pcopyinMap, 
  										parallelBlock, TRUE);
	}
	if(acc_present_or_copyout_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenDeviceCreateCopyInOut(&parallelRegionInfo.acc_dregion_pcopyout, 
  										&parallelRegionInfo.pcopyoutMap, 
  										parallelBlock, FALSE);
	}
	if(acc_present_or_create_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenDeviceCreateCopyInOut(&parallelRegionInfo.acc_dregion_pcreate, 
  										&parallelRegionInfo.pcreateMap, 
  										parallelBlock, FALSE);
	}
	if(acc_present_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenPresentNode(&parallelRegionInfo.acc_dregion_present, 
  										&parallelRegionInfo.presentMap, 
  										parallelBlock);
	}
	if(acc_deviceptr_nodes)
	{
		//Only declaration and cuda malloc, no data movement
		ACC_GenDevicePtr(acc_deviceptr_nodes, 
  										&parallelRegionInfo.dptrList);
	}
		
	//reset them
	acc_if_node = NULL;
	acc_copy_nodes = NULL;	  /* Points to (optional) copy nodes */
	acc_copyin_nodes = NULL;	  /* Points to (optional) copyin nodes */
	acc_copyout_nodes = NULL;   /* Points to (optional) copyout nodes */	  
	acc_create_nodes = NULL;
	acc_present_nodes = NULL;
	acc_present_or_copy_nodes = NULL;
	acc_present_or_copyin_nodes = NULL;
	acc_present_or_copyout_nodes = NULL;
	acc_present_or_create_nodes = NULL;
	acc_deviceptr_nodes = NULL;
	acc_async_nodes = NULL;
	
			  
	acc_dregion_pcreate.clear();
	acc_dregion_pcopy.clear();
	acc_dregion_pcopyin.clear();
	acc_dregion_pcopyout.clear();
	acc_dregion_present.clear();
	acc_dregion_host.clear();
	acc_dregion_device.clear();
	acc_dregion_private.clear();
	acc_dregion_fprivate.clear();
	acc_local_new_var_map.clear();
	acc_reduction_count = 0;
	
	//Generating kernel parameter lists from in/out chain
	ACC_Region_DefUseAnalysis(tree, &acc_pHeadLiveness, &parallelRegionInfo, FALSE);
	ACC_Region_LivenessAnalysis_cont(wn_cont, acc_pHeadLiveness);
	int i = acc_nested_dregion_info.Depth - 1;
	while(i>=0)
	{
	  	SingleDRegionInfo sDRegionInfo = 
			acc_nested_dregion_info.DRegionInfo[i];
		ACC_Region_LivenessAnalysis_cont(sDRegionInfo.wn_cont_nodes, acc_pHeadLiveness);
		i--;
	}

	ACC_Finalize_Liveness_Chain(acc_pHeadLiveness);
	ACC_Retrieve_Kernel_Param_From_Liveness(acc_pHeadLiveness);	
	parallelRegionInfo.acc_param = acc_parms_nodes;
	parallelRegionInfo.acc_private = acc_private_nodes;
	ACC_Dump_InOutAnalysis(acc_pHeadLiveness);
	ACC_Free_DefUse_Chain(&acc_pHeadLiveness);
	//generate parameters
	acc_kernelLaunchParamList.clear();
	ACC_Generate_KernelParameters(acc_parms_nodes, (void*)&parallelRegionInfo, FALSE);
    //multi kernels and multi loopnest process
    ACC_Setup_GPU_toplogy_parallel(&parallelRegionInfo, parallelBlock);
    ACC_Handle_Parallel_Loops(tree, &parallelRegionInfo, parallelBlock);
	acc_kernel_functions_st.clear();
	//Generate the kernel launch function.
	//WN_INSERT_BlockLast(acc_replace_block, LauchKernel(0));	
	//Generate the data copy back function
	if(parallelRegionInfo.acc_present_or_copyout_nodes)
		ACC_GenDataCopyOut(&parallelRegionInfo.pcopyoutMap, parallelBlock);
	if(parallelRegionInfo.acc_present_or_copy_nodes)
		ACC_GenDataCopyOut(&parallelRegionInfo.pcopyMap, parallelBlock);
	
	if(acc_AsyncExpr)
		WN_Delete(acc_AsyncExpr);
	acc_AsyncExpr = NULL;
	/****************************************************************************/
	//Free device memory
	if(parallelRegionInfo.acc_present_or_copyin_nodes)
		ACC_GenDeviceMemFreeInBatch(&parallelRegionInfo.pcopyinMap, parallelBlock);
	if(parallelRegionInfo.acc_present_or_copyout_nodes)
		ACC_GenDeviceMemFreeInBatch(&parallelRegionInfo.pcopyoutMap, parallelBlock);
	if(parallelRegionInfo.acc_present_or_copy_nodes)
		ACC_GenDeviceMemFreeInBatch(&parallelRegionInfo.pcopyMap, parallelBlock);
	if(parallelRegionInfo.acc_present_or_create_nodes)
		ACC_GenDeviceMemFreeInBatch(&parallelRegionInfo.pcreateMap, parallelBlock);
	return parallelBlock;
}


static WN* ACC_Process_HostDataRegion( WN * tree )
{
	return NULL;
}


static WN* ACC_Process_WaitRegion( WN * tree )
{
	WN* acc_int_expr = NULL;
	
	acc_int_expr = WN_COPY_Tree(WN_kid0(acc_clause_intnum));
	WN_Delete(acc_clause_intnum); 
	
	WN* wn_waitBlock = WN_CreateBlock();

	WN_INSERT_BlockLast(wn_waitBlock, ACC_GenWaitStream(acc_int_expr));
	
		
	WN_Delete(acc_int_expr);
	return wn_waitBlock; 
	//return NULL;
}


static WN* ACC_Process_UpdateRegion( WN * tree )
{
	int i;
	WN* wn_updateBlock = WN_CreateBlock();
	WN* dClause;
	WN* wnLength;
	
	ACC_DREGION__ENTRY dentry;
	//async expr
	if(acc_async_nodes)
	{		
		acc_AsyncExpr = WN_COPY_Tree(WN_kid0(acc_async_nodes));
		WN_Delete(acc_async_nodes); 
		acc_async_nodes = NULL;
	}
	for (i=0; i<acc_dregion_host.size(); i++) 
	{
	  ACC_DREGION__ENTRY dentry = acc_dregion_host[i];
	  dClause = dentry.acc_data_clauses;
	  wnLength = dentry.acc_data_length;
	  
	  WN* wnSize = ACC_GetArraySize(dentry);
	  WN* wnStart = ACC_GetArrayStart(dentry);
	  ST *old_st = WN_st(dClause);
	  WN_OFFSET old_offset = WN_offsetx(dClause);
	  WN_INSERT_BlockLast(wn_updateBlock, 
	  			ACC_GenUpdateHostVar(old_st, wnStart, wnSize));
	}
	
	for (i=0; i<acc_dregion_device.size(); i++)  
	{	  
	  ACC_DREGION__ENTRY dentry = acc_dregion_device[i];
	  dClause = dentry.acc_data_clauses;
	  wnLength = dentry.acc_data_length;
	  WN* wnSize = ACC_GetArraySize(dentry);
	  WN* wnStart = ACC_GetArrayStart(dentry);
	  ST *old_st = WN_st(dClause);
	  WN_OFFSET old_offset = WN_offsetx(dClause);
	  WN_INSERT_BlockLast(wn_updateBlock, 
	  			ACC_GenUpdateDeviceVar(old_st, wnStart, wnSize));
	}

	if(acc_AsyncExpr)
		WN_Delete(acc_AsyncExpr);
	acc_AsyncExpr = NULL;
	
    for (i=0; i<acc_dregion_host.size(); i++) 
	{
	    ACC_DREGION__ENTRY dentry = acc_dregion_host[i];
	    WN_Delete (dentry.acc_data_clauses);
	    WN_Delete (dentry.acc_data_length);
    }
	
    for (i=0; i<acc_dregion_device.size(); i++) 
	{
	    ACC_DREGION__ENTRY dentry = acc_dregion_device[i];
	    WN_Delete ( dentry.acc_data_clauses );
	    WN_Delete ( dentry.acc_data_length );
    }
	acc_dregion_host.clear();
	acc_dregion_device.clear();
	if(acc_if_node)
	{
		WN_Delete(acc_if_node);
		acc_if_node = NULL;
	}
	
	return wn_updateBlock;
}


static WN* ACC_Process_CacheRegion( WN * tree )
{
	return NULL;
}


static WN* ACC_Process_DeclareRegion( WN * tree )
{
	return NULL;
}




static WN* ACC_Process_DataRegion( WN * tree )
{
  WN* node = tree;
  int i = 0;
  ACCP_process_type acc_process_type;
  
  WN* wn_next_node; // = WN_next(wtmp);
  WN* wn_cont_nodes;// = WN_next(node);
  WN* wn_stmt_block;// = WN_region_body(node);
  WN* cur_node, *next_node, *prev_node;
  WN* DRegion_replacement_block = WN_CreateBlock();
  //acc_loopinfo.acc_forloop
  SingleDRegionInfo sDRegionInfo = 
  					acc_nested_dregion_info.DRegionInfo[acc_nested_dregion_info.Depth - 1];
  
  if(sDRegionInfo.acc_present_or_copyin_nodes)
  	ACC_GenDeviceCreateCopyInOut(&sDRegionInfo.acc_dregion_pcopyin, 
  										&sDRegionInfo.pcopyinMap, DRegion_replacement_block, TRUE);
  if(sDRegionInfo.acc_present_or_copyout_nodes)
  	ACC_GenDeviceCreateCopyInOut(&sDRegionInfo.acc_dregion_pcopyout,
  										&sDRegionInfo.pcopyoutMap, DRegion_replacement_block, FALSE);
  if(sDRegionInfo.acc_present_or_copy_nodes)
  	ACC_GenDeviceCreateCopyInOut(&sDRegionInfo.acc_dregion_pcopy,
  										&sDRegionInfo.pcopyMap, DRegion_replacement_block, TRUE);
  if(sDRegionInfo.acc_present_or_create_nodes)
  	ACC_GenDeviceCreateCopyInOut(&sDRegionInfo.acc_dregion_pcreate,
  										&sDRegionInfo.pcreateMap, DRegion_replacement_block, FALSE);
  if(sDRegionInfo.acc_present_nodes)
  	ACC_GenPresentNode(&sDRegionInfo.acc_dregion_present,
  										&sDRegionInfo.presentMap, DRegion_replacement_block);
  if(sDRegionInfo.acc_deviceptr_nodes)
  	ACC_GenDevicePtr(sDRegionInfo.acc_deviceptr_nodes, &sDRegionInfo.dptrList);
  
  acc_nested_dregion_info.DRegionInfo[acc_nested_dregion_info.Depth - 1] = sDRegionInfo;
  
  for (cur_node = WN_first(tree); cur_node; cur_node = next_node)  
  {
    prev_node = WN_prev(cur_node);
    next_node = WN_next(cur_node);
	if ((WN_opcode(cur_node) == OPC_REGION) &&
			 WN_first(WN_region_pragmas(cur_node)) &&
			 ((WN_opcode(WN_first(WN_region_pragmas(cur_node))) == OPC_PRAGMA) ||
			  (WN_opcode(WN_first(WN_region_pragmas(cur_node))) == OPC_XPRAGMA)))
	{
	    WN *wtmp = WN_first(WN_region_pragmas(cur_node));
	    WN_PRAGMA_ID wid = (WN_PRAGMA_ID) WN_pragma(wtmp);
		WN* subBlock;

	    switch (wid) {
	      /* orphaned SINGLE region: process it now and return */
	    case WN_PRAGMA_ACC_KERNELS_BEGIN:
	      acc_process_type = ACCP_KERNEL_REGION;
	      break;
	    case WN_PRAGMA_ACC_PARALLEL_BEGIN:
	      acc_process_type = ACCP_PARALLEL_REGION;
	      break;
	    case WN_PRAGMA_ACC_DATA_BEGIN:
	      acc_process_type = ACCP_DATA_REGION;
	      break;
	    case WN_PRAGMA_ACC_HOSTDATA_BEGIN:
	      acc_process_type = ACCP_HOST_DATA_REGION;
	      break;	  
		case WN_PRAGMA_ACC_WAIT:
		  acc_process_type = ACCP_WAIT_REGION;
		  break;
	    case WN_PRAGMA_ACC_UPDATE:
	      acc_process_type = ACCP_UPDATE_REGION;
	      break;
	    case WN_PRAGMA_ACC_DECLARE:
	      acc_process_type = ACCP_DECLARE_REGION;
	      break;
	    case WN_PRAGMA_ACC_CACHE:
	      acc_process_type = ACCP_CACHE_REGION;
	      break;		  

	    default:
	      printf("pragma value = %d", (int)wid); /* for test. by jhs,02.9.3 */
	      Fail_FmtAssertion (
			 "out of context pragma (%s) in acc {primary pragma} processing",
			 WN_pragmas[wid].name);
	    }
		
	    wn_next_node = WN_next(wtmp);
	    wn_cont_nodes = WN_next(cur_node);
	    wn_stmt_block = WN_region_body(cur_node);
		if (acc_process_type == WN_PRAGMA_ACC_WAIT
			|| acc_process_type == WN_PRAGMA_ACC_UPDATE
			|| acc_process_type == WN_PRAGMA_ACC_DECLARE
			|| acc_process_type == WN_PRAGMA_ACC_CACHE) 
	    {
	      WN_Delete ( wtmp );
	      WN_Delete ( WN_region_pragmas(cur_node) );
	      WN_DELETE_Tree ( WN_region_exits(cur_node) );
	    }
		
		///////////////////////////////////////////
		//process clauses
		ACC_Process_Clause_Pragma(wn_next_node);
		//Begin to process the body
		if (acc_process_type == ACCP_DATA_REGION)
		{
			  //it will include any other constructs.
			  //Get the information and move to the next stage
			  SingleDRegionInfo sDRegionInfo;
			  sDRegionInfo.acc_if_node = acc_if_node;
			  //sDRegionInfo.acc_copy_nodes = acc_copy_nodes; 	  /* Points to (optional) copy nodes */
			  //sDRegionInfo.acc_copyin_nodes = acc_copyin_nodes;   /* Points to (optional) copyin nodes */
			  //sDRegionInfo.acc_copyout_nodes = acc_copyout_nodes; /* Points to (optional) copyout nodes */	  
			  //sDRegionInfo.acc_create_nodes = acc_create_nodes;
			  sDRegionInfo.acc_present_nodes = acc_present_nodes;
			  sDRegionInfo.acc_present_or_copy_nodes = acc_present_or_copy_nodes;
			  sDRegionInfo.acc_present_or_copyin_nodes = acc_present_or_copyin_nodes;
			  sDRegionInfo.acc_present_or_copyout_nodes = acc_present_or_copyout_nodes;
			  sDRegionInfo.acc_present_or_create_nodes = acc_present_or_create_nodes;
			  sDRegionInfo.acc_deviceptr_nodes = acc_deviceptr_nodes;
			  sDRegionInfo.wn_cont_nodes = wn_cont_nodes;
			  sDRegionInfo.wn_stmt_block = wn_stmt_block;

				sDRegionInfo.acc_dregion_pcreate = acc_dregion_pcreate;		
				sDRegionInfo.acc_dregion_pcopy = acc_dregion_pcopy;		
				sDRegionInfo.acc_dregion_pcopyin = acc_dregion_pcopyin;		
				sDRegionInfo.acc_dregion_pcopyout = acc_dregion_pcopyout;		
				sDRegionInfo.acc_dregion_present = acc_dregion_present;	
				sDRegionInfo.acc_dregion_private = acc_dregion_private;		
				sDRegionInfo.acc_dregion_fprivate = acc_dregion_fprivate;
			  //////////////////////////////////////////////////////////////////////////
			  //Get def/use 
		  	  // sDRegionInfo.acc_pHeadLiveness = NULL;
			  //ACC_Region_DefUseAnalysis(wn_stmt_block, &sDRegionInfo.acc_pHeadLiveness);
			  //Get in/out/private list
			  //ACC_Region_LivenessAnalysis(wn_cont_nodes, sDRegionInfo.acc_pHeadLiveness);
			  //ACC_Free_DefUse_Chain(&sDRegionInfo.acc_pHeadLiveness);
			  ///////////////////////////////////////////////////////////////////////////
			  
			  acc_nested_dregion_info.DRegionInfo.push_back(sDRegionInfo);
			  acc_nested_dregion_info.Depth ++;
		
			  //reset them
			  acc_if_node = NULL;
			  acc_copy_nodes = NULL;	  /* Points to (optional) copy nodes */
			  acc_copyin_nodes = NULL;	  /* Points to (optional) copyin nodes */
			  acc_copyout_nodes = NULL;   /* Points to (optional) copyout nodes */	  
			  acc_create_nodes = NULL;
			  acc_present_nodes = NULL;
			  acc_present_or_copy_nodes = NULL;
			  acc_present_or_copyin_nodes = NULL;
			  acc_present_or_copyout_nodes = NULL;
			  acc_present_or_create_nodes = NULL;
			  acc_deviceptr_nodes = NULL;

			  acc_dregion_pcreate.clear();
			  acc_dregion_pcopy.clear();
			  acc_dregion_pcopyin.clear();
			  acc_dregion_pcopyout.clear();
			  acc_dregion_present.clear();
			  acc_dregion_host.clear();
			  acc_dregion_device.clear();
			  acc_dregion_private.clear();
			  acc_dregion_fprivate.clear();
			  //cotinue process the body
	
			  subBlock = ACC_Process_DataRegion(wn_stmt_block);
			  acc_nested_dregion_info.DRegionInfo.pop_back();
			  acc_nested_dregion_info.Depth --;
		  }
		  else if(acc_process_type == ACCP_HOST_DATA_REGION)
		  {
		      //TODO for host construct region
		      subBlock = ACC_Process_HostDataRegion(wn_stmt_block);
		  }
		  else if(acc_process_type == ACCP_PARALLEL_REGION)
		  {
		  	  //Basically, parallel region is still a GPU kernel function.
			  //Generating kernel parameter lists from in/out chain.
		  	  /*ACC_Region_DefUseAnalysis(wn_stmt_block, &acc_pHeadLiveness);
			  ACC_Region_LivenessAnalysis_cont(wn_cont_nodes, acc_pHeadLiveness);
			  i = acc_nested_dregion_info.Depth - 1;
			  while(i>=0)
		  	  {
		  	  	SingleDRegionInfo sDRegionInfo = 
  					acc_nested_dregion_info.DRegionInfo[i];
				ACC_Region_LivenessAnalysis_cont(sDRegionInfo.wn_cont_nodes, acc_pHeadLiveness);
				i--;
		  	  }
			  ACC_Finalize_Liveness_Chain(acc_pHeadLiveness);
			  ACC_Retrieve_Kernel_Param_From_Liveness(acc_pHeadLiveness);
			  ACC_Dump_InOutAnalysis(acc_pHeadLiveness);
			  ACC_Free_DefUse_Chain(&acc_pHeadLiveness);*/
			  //It will include LOOP constructs
		      subBlock = ACC_Process_ParallelRegion(wn_stmt_block, wn_cont_nodes);
		  }		  	  
		  else if (acc_process_type == ACCP_KERNEL_REGION) 
		  {
			  /////////////////////////////////////////////////////////
		  	  //generate kernel and launch the kernel
		      subBlock = ACC_Process_KernelsRegion(wn_stmt_block, wn_cont_nodes);
		  } 	  	  
		  else if (acc_process_type == ACCP_WAIT_REGION) 
		  {
		  	  //Wait
		      subBlock = ACC_Process_WaitRegion(wn_stmt_block);
		  }
		  else if (acc_process_type == ACCP_DECLARE_REGION) 
		  {
		  	  //Declare
		      subBlock = ACC_Process_DeclareRegion(wn_stmt_block);
		  } 	  	  
		  else if (acc_process_type == ACCP_CACHE_REGION) 
		  {
		  	  //Cache
		      subBlock = ACC_Process_CacheRegion(wn_stmt_block);
		  } 	  	  
		  else if (acc_process_type == ACCP_UPDATE_REGION) 
		  {
		  	  //Update
		      subBlock = ACC_Process_UpdateRegion(wn_stmt_block);
		  } 

		  WN_INSERT_BlockLast(DRegion_replacement_block, subBlock);
		
	}
	else if(WN_opcode(cur_node) != OPC_REGION_EXIT)//normal statement
	{
		int ikid = 0;
		//in case of the ACC pragma in the while-do/if-stmt etc.
		for (ikid = 0; ikid < WN_kid_count(cur_node); ikid++)
        if (WN_kid(cur_node, ikid) &&
           (WN_opcode(WN_kid(cur_node, ikid)) == OPC_BLOCK))
              WN_kid(cur_node, ikid) = ACC_Process_Block_Transform( WN_kid(cur_node, ikid) );
		
		WN_INSERT_BlockLast(DRegion_replacement_block, cur_node);
	}
  }
  
  if(sDRegionInfo.acc_present_or_copyout_nodes)
  	ACC_GenDataCopyOut(&sDRegionInfo.pcopyoutMap, DRegion_replacement_block);
  if(sDRegionInfo.acc_present_or_copy_nodes)
  	ACC_GenDataCopyOut(&sDRegionInfo.pcopyMap, DRegion_replacement_block);
  /****************************************************************************/
  //Free device memory
  if(sDRegionInfo.acc_present_or_copyin_nodes)
  	ACC_GenDeviceMemFreeInBatch(&sDRegionInfo.pcopyinMap, DRegion_replacement_block);
  if(sDRegionInfo.acc_present_or_copyout_nodes)
  	ACC_GenDeviceMemFreeInBatch(&sDRegionInfo.pcopyoutMap, DRegion_replacement_block);
  if(sDRegionInfo.acc_present_or_copy_nodes)
  	ACC_GenDeviceMemFreeInBatch(&sDRegionInfo.pcopyMap, DRegion_replacement_block);
  if(sDRegionInfo.acc_present_or_create_nodes)
  	ACC_GenDeviceMemFreeInBatch(&sDRegionInfo.pcreateMap, DRegion_replacement_block);
  acc_region_num ++;
  acc_construct_num = 0;
  
  return DRegion_replacement_block;  
}


/*  Compare two PRAGMA nodes or XPRAGMA trees for equality.  */

static BOOL 
ACC_Identical_Pragmas ( WN * wn1, WN * wn2 )
{
  INT32 i;

  if ((WN_operator(wn1) != WN_operator(wn2)) ||
      (WN_pragma(wn1) != WN_pragma(wn2)) ||
      (WN_st(wn1) != WN_st(wn2)) ||
      (WN_pragma_flags(wn1) != WN_pragma_flags(wn2)) ||
      ((WN_operator(wn1) == OPR_PRAGMA) &&
       (WN_pragma_arg1(wn1) != WN_pragma_arg1(wn2))) || 
       (WN_pragma_arg2(wn1) != WN_pragma_arg2(wn2)) ||
      (WN_kid_count(wn1) != WN_kid_count(wn2)))
    return (FALSE);

  for (i = 0; i < WN_kid_count(wn1); i++)
    if (WN_ACC_Compare_Trees(WN_kid(wn1, i), WN_kid(wn2, i)) != 0)
      return (FALSE);

  return (TRUE);
}

/**
reduction kernel parameters
**/

static ST* st_input_data;
static ST* st_output_data;
static ST* st_num_elem;
static ST* st_blocksize;
//static ST* st_isPow2;
/*****************************************/
//This function is only called by ACC_Create_Reduction_Kernels  function.
static void Create_reduction_kernel_st_params(ST* st_reduction)
{	
	//reduction_kernel_param.clear();
	
	WN* wn;
	//this var may be deleted later.it is useless.
	ACC_VAR_TYPE vtype = ACC_VAR_COPYIN;			
	TY_IDX ty = ST_type(st_reduction);
	TY_KIND kind = TY_kind(ty);//ST_name(old_st)
	if (kind != KIND_SCALAR)
    	Fail_FmtAssertion("Create_reduction_kernel_st_params: invalid OpenACC reduction type. It must be scalar variables.");
	
	char* localname = (char *) alloca(strlen(ST_name(st_reduction))+20);

	//declare in buffer
	sprintf ( localname, "g_in_%s", ST_name(st_reduction) );

	//This is a pointer type
	//TY_IDX pty = TY_pointed(ty);
	TY_IDX ty_p = Make_Pointer_Type(ty);
	ST *karg = NULL;
	karg = New_ST( CURRENT_SYMTAB );
	ST_Init(karg,
			Save_Str( localname ),
			CLASS_VAR,
			SCLASS_FORMAL,
			EXPORT_LOCAL,
			ty_p);
	Set_ST_is_value_parm( karg );
	st_input_data = karg;
	
	//declare out buffer	
	sprintf ( localname, "g_out_%s", ST_name(st_reduction) );

	//This is a pointer type
	//TY_IDX pty = TY_pointed(ty);
	karg = New_ST( CURRENT_SYMTAB );
	ST_Init(karg,
			Save_Str( localname ),
			CLASS_VAR,
			SCLASS_FORMAL,
			EXPORT_LOCAL,
			ty_p);
	Set_ST_is_value_parm( karg );
	st_output_data = karg;

	//delcare "n", how many elements in this input buffer
	sprintf ( localname, "n_of_%s", ST_name(st_reduction) );
	karg = New_ST( CURRENT_SYMTAB );
	ST_Init(karg,
			Save_Str( localname ),
			CLASS_VAR,
			SCLASS_FORMAL,
			EXPORT_LOCAL,
			Be_Type_Tbl(MTYPE_U4));
	Set_ST_is_value_parm( karg );
	st_num_elem = karg;
	//delcare "blocksize", 
	sprintf ( localname, "blocksize");
	karg = New_ST( CURRENT_SYMTAB );
	ST_Init(karg,
			Save_Str( localname ),
			CLASS_VAR,
			SCLASS_FORMAL,
			EXPORT_LOCAL,
			Be_Type_Tbl(MTYPE_U4));
	Set_ST_is_value_parm( karg );
	st_blocksize = karg;

}


//This function is only called by ACC_Create_Reduction_vector/worker/workervector  function.
static void Create_reduction_device_st_params(ST* st_reduction)
{	
	//reduction_kernel_param.clear();
	
	WN* wn;
	//this var may be deleted later.it is useless.
	ACC_VAR_TYPE vtype = ACC_VAR_COPYIN;			
	TY_IDX ty = ST_type(st_reduction);
	TY_KIND kind = TY_kind(ty);//ST_name(old_st)
	if (kind != KIND_SCALAR)
    	Fail_FmtAssertion("Create_reduction_kernel_st_params: invalid OpenACC reduction type. It must be scalar variables.");
	
	char* localname = (char *) alloca(strlen(ST_name(st_reduction))+20);

	//declare in buffer
	sprintf ( localname, "sdata_%s", ST_name(st_reduction) );

	//This is a pointer type
	//TY_IDX pty = TY_pointed(ty);
	TY_IDX ty_p = Make_Pointer_Type(ty);
	ST *karg = NULL;
	karg = New_ST( CURRENT_SYMTAB );
	ST_Init(karg,
			Save_Str( localname ),
			CLASS_VAR,
			SCLASS_FORMAL,
			EXPORT_LOCAL,
			ty_p);
	Set_ST_is_value_parm( karg );
	st_input_data = karg;	

}



/*Init value of reduction*/
static char* ACC_Get_Name_of_Reduction(OPERATOR ReductionOpr)
{
	switch(ReductionOpr)
	{
	case OPR_ADD:
		return "ADD";
	case OPR_CIOR:
		return "CIOR";
	case OPR_BIOR:
		return "BIOR";
	case OPR_BXOR:
		return "BXOR";
	case OPR_MAX:
		return "MAX";
	case OPR_MIN:
		return "MIN";
	case OPR_BAND:
		return "BAND";
	case OPR_MPY:
		return "MPY";
	case OPR_CAND:
		return "CAND";
	default:
    	Fail_FmtAssertion("invalid reduction operator for OpenACC: ACC_Get_Name_of_Reduction");		
	}
}


/*Init value of reduction*/
static char* ACC_Get_ScalarName_of_Reduction(TYPE_ID typeID)
{
	switch(typeID)
	{
	case MTYPE_I1: 		 /*   8-bit integer */
		return "char";
	case MTYPE_I2:  		 /*  16-bit integer */
		return "short";
	case MTYPE_I4:  		 /*  32-bit integer */
		return "int32";
	case MTYPE_I8:  		 /*  64-bit integer */
		return "long";
	case MTYPE_U1:  		 /*   8-bit unsigned integer */
		return "uchar";
	case MTYPE_U2:  		 /*  16-bit unsigned integer */
		return "ushort";
	case MTYPE_U4:  		 /*  32-bit unsigned integer */
		return "uint32";
	case MTYPE_U8:  		 /*  64-bit unsigned integer */
		return "ulong";
	case MTYPE_F4: 		 /*  32-bit IEEE floating point */
		return "float";
	case MTYPE_F8: 		 /*  64-bit IEEE floating point */
		return "double";
	default:
		Is_True(FALSE, ("Wrong reduction data Type in ACC_Get_ScalarName_of_Reduction. It must be scalar data. "));
	}
}


/*Init value of reduction*/
static char* ACC_Get_LoopTypeName_of_Reduction(ACC_LOOP_TYPE looptype)
{
	switch(looptype)
	{
	case ACC_VECTOR: 			
		return "vector";
	case ACC_WORKER_VECTOR:  	
		return "worker_vector";
	case ACC_WORKER:  		 	
		return "worker";
	default:
		Is_True(FALSE, ("Wrong reduction Loop Type."));
	}
}

ST_IDX Make_ACC_CUDA_Runtime_ST ( ST* st_device_func)
{
  //Is_True(rop >= ACCRUNTIME_FIRST && rop <= ACCRUNTIME_LAST,
  //        ("Make_ACCRuntime_ST: bad rop == %d", (INT) rop));
  if(acc_reduction_device_reduction_call[st_device_func])
  	return ST_st_idx(*acc_reduction_device_reduction_call[st_device_func]);

    // If the global type doesn't exist, create it and its pointer type.
  if (accruntime_ty == TY_IDX_ZERO) {
    TY &mpr_ty = New_TY ( accruntime_ty );
    TY_Init(mpr_ty, 0, KIND_FUNCTION, MTYPE_UNKNOWN,
            Save_Str(".accruntime"));
    Set_TY_align(accruntime_ty, 1);

    TYLIST_IDX parm_idx;
    TYLIST& parm_list = New_TYLIST(parm_idx);
    Set_TY_tylist(mpr_ty, parm_idx);
    Set_TYLIST_type(parm_list, Be_Type_Tbl(MTYPE_I4));  // I4 return type
      // are there really no parameters? -- DRK
    Set_TYLIST_type(New_TYLIST(parm_idx), TY_IDX_ZERO); // end of parm list

    TY_IDX ty_idx;
    TY &ty = New_TY ( ty_idx );
    TY_Init(ty, Pointer_Size, KIND_POINTER, Pointer_Mtype,
      Save_Str ( ".accruntime_ptr" ));
    Set_TY_pointed(ty, accruntime_ty);

    Set_TY_align(ty_idx, Pointer_Size); // unnecessary? TY_Init does
                                        // not set alignment -- DRK
  }

  PU_IDX pu_idx;
  PU& pu = New_PU(pu_idx);
  PU_Init(pu, accruntime_ty, CURRENT_SYMTAB);

  /*  Create the ST, fill in all appropriate fields and enter into the */
  /*  global symbol table.  */

  ST *st = New_ST ( GLOBAL_SYMTAB );
  ST_Init(st, Save_Str ( ST_name(st_device_func) ), CLASS_FUNC, SCLASS_EXTERN,
    EXPORT_PREEMPTIBLE, pu_idx);

  Allocate_Object ( st );
  acc_reduction_device_reduction_call[st_device_func] = st;
  //accr_sts[rop] = ST_st_idx(*st);
  return ST_st_idx(*acc_reduction_device_reduction_call[st_device_func]);
}

/*static WN* ACC_Gen_Dynamic_LaunchReductionKernel(ST* st_kernel_func, ST* st_inputdata)
{
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 1);
  WN_st_idx(wn) = ST_st_idx(*st_kernel_func);
  ST* st_fun = WN_st(wn);
  //Set_ST_st_idx(st_fun, Save_Str(ST_name(st_device_func)));
  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  wnx = WN_Ldid(Pointer_type, 0, st_inputdata, ST_type(st_inputdata));


  //wnx = WN_Lda( Pointer_type, 0, Src);
  WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);
  return wn;
}*/

static WN* ACC_Gen_Call_Local_Reduction(ST* st_device_func, ST* st_inputdata)
{
  WN * wn;
  WN * wnx;
  wn = WN_Create(OPC_VCALL, 1);
  WN_st_idx(wn) = ST_st_idx(*st_device_func);
  ST* st_fun = WN_st(wn);
  //Set_ST_st_idx(st_fun, Save_Str(ST_name(st_device_func)));
  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  wnx = WN_Ldid(Pointer_type, 0, st_inputdata, ST_type(st_inputdata));


  //wnx = WN_Lda( Pointer_type, 0, Src);
  WN_kid(wn, 0) = WN_CreateParm(Pointer_type, wnx, 
                       WN_ty(wnx), WN_PARM_BY_VALUE);
  return wn;
}

//vector<acc_reduction_kernels_pair> acc_reduction_kernels_maps
static void ACC_Create_Reduction_Kernels ( PAR_FUNC_ACC_TYPE func_type, 
				ACC_ReductionMap* pReduction_map)
{
  // should be merged up after done. Currently reserved for Debug
  const char *construct_type_str = "accrg_reduction";
  char temp_str[64];
  char szReduction_name[64];
  char szReduction_datatype[64];
  OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
  ST* st_reduction_var = pReduction_map->hostName;
  char* looptype;

  sprintf(szReduction_name, "%s", ACC_Get_Name_of_Reduction(ReductionOpr));
  sprintf(szReduction_datatype, "%s", ACC_Get_ScalarName_of_Reduction(TY_mtype(ST_type(st_reduction_var))));
  // generate new name for nested function

  // should PAR regions and PAR DO's be numbered separately? -- DRK
  const char *old_st_name = ST_name(PU_Info_proc_sym(Current_PU_Info));
  char *st_name = (char *) alloca(strlen(old_st_name) + 64);
  if(func_type == PAR_FUNC_ACC_KERNEL)
  	looptype = "kernel";
  else
  	looptype = ACC_Get_LoopTypeName_of_Reduction(pReduction_map->looptype);
  sprintf ( st_name, "__%s_%s_%s_%s_%d_%d", construct_type_str, looptype, szReduction_name, szReduction_datatype,
	      acc_region_num, acc_construct_num );
  
  acc_construct_num ++;
  // get function prototype

  TY_IDX func_ty_idx = TY_IDX_ZERO;

  if  (func_ty_idx == TY_IDX_ZERO) 
  {
    // create new type for function, and type for pointer to function

    TY& ty = New_TY(func_ty_idx);
    sprintf(temp_str, ".%s", construct_type_str);
    TY_Init(ty, 0, KIND_FUNCTION, MTYPE_UNKNOWN, Save_Str(temp_str));
    Set_TY_align(func_ty_idx, 1);
	
    Set_TY_has_prototype(func_ty_idx);
	

    TYLIST_IDX parm_idx;
    TYLIST& parm_list = New_TYLIST(parm_idx);	
    Set_TY_tylist(ty, parm_idx);
    Set_TYLIST_type(parm_list, Be_Type_Tbl(MTYPE_V));  // return type

    /* turn this off if don't want to use taskargs struct */
    //else if (0)

    Set_TYLIST_type(New_TYLIST(parm_idx), TY_IDX_ZERO); // end of parm list

    // now create a type for a pointer to this function
    TY_IDX ptr_ty_idx;
    TY &ptr_ty = New_TY(ptr_ty_idx);
    sprintf(temp_str, ".%s_ptr", construct_type_str);
    TY_Init(ptr_ty, Pointer_Size, KIND_POINTER, Pointer_Mtype,
            Save_Str(temp_str));
    Set_TY_pointed(ptr_ty, func_ty_idx);
  }


  // create new PU and ST for nested function

  PU_IDX pu_idx;
  PU& pu = New_PU(pu_idx);
  PU_Init(pu, func_ty_idx, CURRENT_SYMTAB);

/*
Many questions of DRK's about flags:

is_pure and no_side_effects shouldn't be set due to output ref. parms?
does no_delete matter?
have no idea: needs_fill_align_lowering, needs_t9, put_in_elf_section,
  has_return_address, has_inlines, calls_{set,long}jmp, namelist
has_very_high_whirl and mp_needs_lno should have been handled already
is inheriting pu_recursive OK?
*/

  Set_PU_no_inline(pu);
  Set_PU_is_nested_func(pu);
  Set_PU_acc(pu);
  Set_PU_has_acc(pu);
  
#ifdef KEY
  Set_PU_acc_lower_generated(pu);
#endif // KEY
    // child PU inherits language flags from parent
  if (PU_c_lang(Current_PU_Info_pu()))
    Set_PU_c_lang(pu);
  if (PU_cxx_lang(Current_PU_Info_pu()))
    Set_PU_cxx_lang(pu);
  if (PU_f77_lang(Current_PU_Info_pu()))
    Set_PU_f77_lang(pu);
  if (PU_f90_lang(Current_PU_Info_pu()))
    Set_PU_f90_lang(pu);
  if (PU_java_lang(Current_PU_Info_pu()))
    Set_PU_java_lang(pu);

  Set_FILE_INFO_has_acc(File_info);  // is this true after acc lowerer?--DRK
  
  //TY_IDX	   funtype = ST_pu_type(st);
  //BOOL		   has_prototype = TY_has_prototype(funtype);
  //acc_reduction_proc

  acc_reduction_proc = New_ST(GLOBAL_SYMTAB);
  ST_Init(acc_reduction_proc,
          Save_Str (st_name),
          CLASS_FUNC,
          SCLASS_TEXT,
          EXPORT_LOCAL,
          TY_IDX (pu_idx));
  Set_ST_addr_passed(acc_reduction_proc);  
  Set_ST_sfname_idx(acc_reduction_proc, Save_Str(Src_File_Name));
  
  if(func_type == PAR_FUNC_ACC_KERNEL)
  	Set_ST_ACC_kernels_func(acc_reduction_proc);
  else if(func_type == PAR_FUNC_ACC_DEVICE)
  	Set_ST_ACC_device_func(acc_reduction_proc);

  Allocate_Object ( acc_reduction_proc);

  //
  acc_reduction_kernels_pair reduction_pairs;
  reduction_pairs.opr_ty = ST_type(st_reduction_var);
  reduction_pairs.ReductionOpr = ReductionOpr;
  reduction_pairs.st_kernels_fun = acc_reduction_proc;
  reduction_pairs.looptype = pReduction_map->looptype;
  if(func_type == PAR_FUNC_ACC_KERNEL)
  	acc_reduction_kernels_maps.push_back(reduction_pairs);
  else
  	acc_reduction_devices_maps.push_back(reduction_pairs);


  // create nested symbol table for parallel function

  New_Scope(CURRENT_SYMTAB + 1,
            Malloc_Mem_Pool,  // find something more appropriate--DRK
            TRUE);
  acc_csymtab = CURRENT_SYMTAB;
  acc_func_level = CURRENT_SYMTAB;
  Scope_tab[acc_csymtab].st = acc_reduction_proc;

  Set_PU_lexical_level(pu, CURRENT_SYMTAB);

  ACC_Create_Func_DST ( st_name );


  // pre-allocate in child as many pregs as there are in the parent

  for (UINT32 i = 1; i < PREG_Table_Size(acc_psymtab); i++) {
    PREG_IDX preg_idx;
    PREG &preg = New_PREG(acc_csymtab, preg_idx);
      // share name with corresponding parent preg
    Set_PREG_name_idx(preg,
      PREG_name_idx((*Scope_tab[acc_psymtab].preg_tab)[preg_idx]));
  }

    // create ST's for parameters

  ST *arg_gtid = NULL;
  ST *task_args = NULL;
  //Create the local ST for  kernels parameters
  if(func_type == PAR_FUNC_ACC_KERNEL)
  	Create_reduction_kernel_st_params(st_reduction_var);
  else
  	Create_reduction_device_st_params(st_reduction_var);
  //if(isParallelRegion)
  //	Create_kernel_parameters_ST(pPRInfo->acc_param);
  //else  	
  //	Create_kernel_parameters_ST(pKRInfo->acc_param);
  
  //acc_local_taskargs = NULL;
  //More parameters
  /*WN* l;
  for (l = acc_parms_nodes; l; l = WN_next(l)) 
  {
	ST* pST = WN_st(l);
	TY_IDX tmp_ty = ST_type(pST);    
	TYPE_ID tmp_tid = TY_mtype(tmp_ty);
	WN* param = WN_Ldid(tmp_tid, 0, pST, tmp_ty);
	WN_kid(wn, parm_id) = WN_CreateParm(tmp_tid, param, tmp_ty, WN_PARM_BY_VALUE);
	parm_id ++;
  }*/
  //////////////////////////////////////////////////////////////////////
  /* declare some global variables for threadIdx and blockIdx */
  
  glbl_threadIdx_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_threadIdx_x,
      Save_Str( "__nv50_threadIdx_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_threadIdx_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_threadIdx_y,
      Save_Str( "__nv50_threadIdx_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_threadIdx_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_threadIdx_z,
      Save_Str( "__nv50_threadIdx_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockIdx_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockIdx_x,
      Save_Str( "__nv50_blockIdx_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockIdx_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockIdx_y,
      Save_Str( "__nv50_blockIdx_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockIdx_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockIdx_z,
      Save_Str( "__nv50_blockIdx_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockDim_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockDim_x,
      Save_Str( "__nv50_blockdim_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockDim_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockDim_y,
      Save_Str( "__nv50_blockdim_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  glbl_blockDim_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_blockDim_z,
      Save_Str( "__nv50_blockdim_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));

	
  glbl_gridDim_x = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_gridDim_x,
      Save_Str( "__nv50_griddim_x"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  
  glbl_gridDim_y = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_gridDim_y,
      Save_Str( "__nv50_griddim_y"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  
  glbl_gridDim_z = New_ST(CURRENT_SYMTAB); 
  ST_Init(glbl_gridDim_z,
      Save_Str( "__nv50_griddim_z"),
      CLASS_VAR,
      SCLASS_FORMAL,
      EXPORT_LOCAL,
      Be_Type_Tbl(MTYPE_U4));
  

    // TODO: other procedure specific arguments should
    // be handled here.

    // create WHIRL tree for nested function

  acc_reduction_func = WN_CreateBlock ( );
  WN* reference_block = WN_CreateBlock ( );
  WN* pragma_block = WN_CreateBlock ( );
#ifdef KEY
  WN *current_pu_tree = PU_Info_tree_ptr(Current_PU_Info);
  //WN *thread_priv_prag = WN_first(WN_func_pragmas(PU_Info_tree_ptr(Current_PU_Info)));
  
#endif
  // Currently, don't pass data via arguments.
  //UINT arg_cnt = 1;
  /* turn this off if don't want to use taskargs struct */
  //if (is_task_region) arg_cnt = 2;

  //UINT slink_arg_pos = arg_cnt - 1;
  //acc_last_microtask = func_entry;

  //if (has_gtid) {
  //  WN_kid0(func_entry) = WN_CreateIdname ( 0, arg_gtid );
  //} 
  UINT ikid = 0;
  WN *func_entry;
  //vector<ST*>::iterator itor = kernel_param.begin();
  //while(ikid < reduction_kernel_param.size())
  if(func_type == PAR_FUNC_ACC_KERNEL)
  {
     func_entry = WN_CreateEntry ( 4, acc_reduction_proc,
                                    acc_reduction_func, pragma_block,
                                    reference_block );
     WN_kid(func_entry, 0) = WN_CreateIdname ( 0, ST_st_idx(st_input_data));
     WN_kid(func_entry, 1) = WN_CreateIdname ( 0, ST_st_idx(st_output_data));
     WN_kid(func_entry, 2) = WN_CreateIdname ( 0, ST_st_idx(st_num_elem));
     WN_kid(func_entry, 3) = WN_CreateIdname ( 0, ST_st_idx(st_blocksize));
     //WN_kid(func_entry, 4) = WN_CreateIdname ( 0, ST_st_idx(st_isPow2));
	 //ACC_Add_DST_variable ( reduction_kernel_param[ikid], acc_nested_dst, acc_line_number, DST_INVALID_IDX );
  	 //ikid ++;
  }
  else
  {
     func_entry = WN_CreateEntry ( 1, acc_reduction_proc,
                                    acc_reduction_func, pragma_block,
                                    reference_block );
     WN_kid(func_entry, 0) = WN_CreateIdname ( 0, ST_st_idx(st_input_data));
  }

     // TODO: variable arguments list should be added here.

  WN_linenum(func_entry) = acc_line_number;


  // create PU_Info for nested function
  
  PU_Info *reduction_kernels_pu = TYPE_MEM_POOL_ALLOC ( PU_Info, Malloc_Mem_Pool );
  PU_Info_init ( reduction_kernels_pu );
  Set_PU_Info_tree_ptr (reduction_kernels_pu, func_entry );

  PU_Info_proc_sym(reduction_kernels_pu) = ST_st_idx(acc_reduction_proc);
  PU_Info_maptab(reduction_kernels_pu) = acc_cmaptab = WN_MAP_TAB_Create(MEM_pu_pool_ptr);
  PU_Info_pu_dst(reduction_kernels_pu) = acc_nested_dst;
  Set_PU_Info_state(reduction_kernels_pu, WT_SYMTAB, Subsect_InMem);
  Set_PU_Info_state(reduction_kernels_pu, WT_TREE, Subsect_InMem);
  Set_PU_Info_state(reduction_kernels_pu, WT_PROC_SYM, Subsect_InMem);
  Set_PU_Info_flags(reduction_kernels_pu, PU_IS_COMPILER_GENERATED);

  // don't copy nystrom points to analysis, alias_tag map
  // mp function's points to analysis will be analyzed locally.
  //AliasAnalyzer *aa = AliasAnalyzer::aliasAnalyzer();
  //if (aa) 
  //{
  //  // Current_Map_Tab is update to PU_Info_maptab(parallel_pu) in PU_Info_maptab
  //  Is_True(PU_Info_maptab(reduction_kernels_pu) == Current_Map_Tab,
  //      ("parallel_pu's PU's maptab isn't parallel_pu\n"));
  //  Current_Map_Tab = acc_pmaptab;
  //  WN_MAP_Set_dont_copy(aa->aliasTagMap(), TRUE);
  //  WN_MAP_Set_dont_copy(WN_MAP_ALIAS_CGNODE, TRUE);
  //  Current_Map_Tab = PU_Info_maptab(reduction_kernels_pu);
  //}
  //else 
  //{
    Current_Map_Tab = acc_pmaptab;
    WN_MAP_Set_dont_copy(WN_MAP_ALIAS_CGNODE, TRUE);
    Current_Map_Tab = PU_Info_maptab(reduction_kernels_pu);
  //}

    // use hack to save csymtab using parallel_pu, so we can restore it
    // later when we lower parallel_pu; this is necessary because the
    // new symtab routines can't maintain more than one chain of symtabs
    // in memory at one time, and we lower the parent PU all the way to
    // CG before we lower any of the nested MP PUs
        // Save_Local_Symtab expects this
  Set_PU_Info_symtab_ptr(reduction_kernels_pu, NULL);
  Save_Local_Symtab(acc_csymtab, reduction_kernels_pu);

  Is_True(PU_Info_state(reduction_kernels_pu, WT_FEEDBACK) == Subsect_Missing,
          ("there should be no feedback for parallel_pu"));

  RID *root_rid = RID_Create ( 0, 0, func_entry );
  RID_type(root_rid) = RID_TYPE_func_entry;
  Set_PU_Info_regions_ptr ( reduction_kernels_pu, root_rid );
  Is_True(PU_Info_regions_ptr(reduction_kernels_pu) != NULL,
	 ("ACC_Create_Reduction_Kernels, NULL root RID"));

  PU_Info *tpu = PU_Info_child(Current_PU_Info);

    // add parallel_pu after last child MP PU_Info item in parent's list
  if (tpu && PU_Info_state(tpu, WT_SYMTAB) == Subsect_InMem &&
      PU_acc(PU_Info_pu(tpu)) ) 
  {
    PU_Info *npu;

    while ((npu = PU_Info_next(tpu)) &&
	   PU_Info_state(npu, WT_SYMTAB) == Subsect_InMem &&
	   PU_acc(PU_Info_pu(npu)) )
      tpu = npu;

    PU_Info_next(tpu) = reduction_kernels_pu;
    PU_Info_next(reduction_kernels_pu) = npu;
  } 
  else 
  {
    PU_Info_child(Current_PU_Info) = reduction_kernels_pu;
    PU_Info_next(reduction_kernels_pu) = tpu;
  }


  // change some global state; need to clean this up--DRK

  Current_PU_Info = reduction_kernels_pu;
  Current_pu = &Current_PU_Info_pu();
  Current_Map_Tab = acc_pmaptab;

  //if (has_gtid)
  //  Add_DST_variable ( arg_gtid, nested_dst, line_number, DST_INVALID_IDX );
  //Add_DST_variable ( arg_slink, nested_dst, line_number, DST_INVALID_IDX );

}


//kernel function for reductions
static ST* ACC_Get_Reduction_kernels(OPERATOR ReductionOpr, TY_IDX ty_reduction_opr)
{
	int i=0;
	while(i<acc_reduction_kernels_maps.size())
	{
		acc_reduction_kernels_pair kernelsPair = acc_reduction_kernels_maps[i];
		if(kernelsPair.opr_ty == ty_reduction_opr
				&& kernelsPair.ReductionOpr == ReductionOpr)
			{
				return kernelsPair.st_kernels_fun;
			}
		i++;
	}
	return NULL;
}

//device function for reductions
static ST* ACC_Get_Reduction_devices(ACC_ReductionMap* pReduction_map)
{
	int i=0;	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
    TY_IDX ty_reduction_opr = ST_type(pReduction_map->hostName);
	ACC_LOOP_TYPE looptype = pReduction_map->looptype;
	while(i<acc_reduction_devices_maps.size())
	{
		acc_reduction_kernels_pair kernelsPair = acc_reduction_devices_maps[i];
		if(kernelsPair.opr_ty == ty_reduction_opr
				&& kernelsPair.ReductionOpr == ReductionOpr
				&& kernelsPair.looptype == looptype)
			{
				return kernelsPair.st_kernels_fun;
			}
		i++;
	}
	return NULL;
}


static WN* ACC_LoadDeviceSharedArrayElem(WN* wn_array_offset, ST* st_array)
{
	
	//load shared array
	unsigned int isize;
	if(TY_kind(ST_type(st_array)) == KIND_ARRAY)
	{
		isize = TY_size(TY_etype(ST_type(st_array)));
	}
	else if(TY_kind(ST_type(st_array)) == KIND_POINTER)
	{
		isize = TY_size(TY_pointed(ST_type(st_array)));
	}
	WN* wn_index = WN_Binary(OPR_MPY, MTYPE_U4, wn_array_offset, WN_Intconst(MTYPE_U4, isize));
	WN* wn_LoadReductionArray = WN_Ldid(Pointer_type, 0, st_array, ST_type(st_array));
	WN* wn_ptrLoc = WN_Binary(OPR_ADD, MTYPE_U4, wn_index, wn_LoadReductionArray);

	return wn_ptrLoc;
	//WN* tree = WN_Istore(TY_mtype(TY_pointed(ST_type(w->new_st))), 0, ST_type(w->new_st), wn_ptrLoc, WN_kid(tree, 0));
	//WN* tree = WN_Iload(TY_mtype(TY_pointed(ST_type(w->new_st))), 0,  TY_pointed(ST_type(w->new_st)), wn_ptrLoc);
}

/*Init value of reduction*/
static WN* ACC_Get_Init_Value_of_Reduction(OPERATOR ReductionOpr)
{
	WN* wn_init = NULL;
	switch(ReductionOpr)
	{
	case OPR_ADD:
	case OPR_CIOR:
	case OPR_BIOR:
	case OPR_BXOR:
		//return 0
		wn_init = WN_Intconst(MTYPE_U4, 0);
		break;
	case OPR_MAX:
		//return min INT_MIN
		wn_init = WN_Intconst(MTYPE_U4, INT_MIN);
		break;
	case OPR_MIN:
		//return max INT_MAX
		wn_init = WN_Intconst(MTYPE_U4, INT_MAX);
		break;
	case OPR_BAND:
		//return ~0
		wn_init = WN_Intconst(MTYPE_U4, (~0));
		break;
	case OPR_MPY:
	case OPR_CAND:
		//return 1
		wn_init = WN_Intconst(MTYPE_U4, 1);
		break;
	default:
    	Fail_FmtAssertion("invalid reduction operator for OpenACC: ACC_Get_Init_Value_of_Reduction");		
	}
	return wn_init;
}

static WN* GenRightShiftAndOrOperations(WN* wn_op, WN* wn_bitpos)
{
	TYPE_ID rtype = TY_mtype(ST_type(WN_st(wn_op)));
	WN* wn_rightShift = WN_Binary(OPR_ASHR, rtype, WN_COPY_Tree(wn_op), wn_bitpos);
	WN* wn_OrOpr = WN_Binary(OPR_BIOR, rtype, WN_COPY_Tree(wn_op), wn_rightShift);
	
	wn_OrOpr = WN_Stid(rtype, 0, WN_st(wn_op), ST_type(WN_st(wn_op)), wn_OrOpr);
	return wn_OrOpr;
}

static WN* Gen_Next_Pow2DeviceStmt(WN* wn_op, WN* wn_replace_block)
{
	TYPE_ID rtype = TY_mtype(ST_type(WN_st(wn_op)));//
	WN* wn_ShrAndOr;
	WN* wn_minusOne = WN_Binary(OPR_SUB, rtype, WN_COPY_Tree(wn_op), WN_Intconst(rtype, 1));
	wn_minusOne = WN_Stid(TY_mtype(ST_type(WN_st(wn_op))), 0, WN_st(wn_op),
								ST_type(WN_st(wn_op)), wn_minusOne);
	WN_INSERT_BlockLast(wn_replace_block,  wn_minusOne);

	wn_ShrAndOr = GenRightShiftAndOrOperations(wn_op, WN_Intconst(rtype, 1));
	WN_INSERT_BlockLast(wn_replace_block,  wn_ShrAndOr);
	wn_ShrAndOr = GenRightShiftAndOrOperations(wn_op, WN_Intconst(rtype, 2));
	WN_INSERT_BlockLast(wn_replace_block,  wn_ShrAndOr);
	wn_ShrAndOr = GenRightShiftAndOrOperations(wn_op, WN_Intconst(rtype, 4));
	WN_INSERT_BlockLast(wn_replace_block,  wn_ShrAndOr);
	wn_ShrAndOr = GenRightShiftAndOrOperations(wn_op, WN_Intconst(rtype, 8));
	WN_INSERT_BlockLast(wn_replace_block,  wn_ShrAndOr);
	wn_ShrAndOr = GenRightShiftAndOrOperations(wn_op, WN_Intconst(rtype, 16));
	WN_INSERT_BlockLast(wn_replace_block,  wn_ShrAndOr);
	
	WN* wn_plusOne = WN_Binary(OPR_ADD, rtype, WN_COPY_Tree(wn_op), WN_Intconst(rtype, 1));
	wn_plusOne = WN_Stid(TY_mtype(ST_type(WN_st(wn_op))), 0, WN_st(wn_op),
								ST_type(WN_st(wn_op)), wn_plusOne);
	WN_INSERT_BlockLast(wn_replace_block,  wn_plusOne);
}

static WN* Gen_Sync_Threads()
{
  WN * wn;
  wn = WN_Create(OPC_VCALL, 0 );
  WN_st_idx(wn) = GET_ACCRUNTIME_ST(ACCR_SYNCTHREADS);

  WN_Set_Call_Non_Data_Mod(wn);
  WN_Set_Call_Non_Data_Ref(wn);
  WN_Set_Call_Non_Parm_Mod(wn);
  WN_Set_Call_Non_Parm_Ref(wn);
  WN_Set_Call_Parm_Ref(wn);
  WN_linenum(wn) = acc_line_number;

  return wn;
}

/*static WN* Gen_ReductionIfElseBlock1_Vector(WN* wn_blocksize, WN* wn_tid, WN* wn_btid, ST* st_shared_array,
						WN* wn_mySum, unsigned int ilimit, OPERATOR ReductionOpr)
{
	WN* wn_IfOuterTest = WN_Relational (OPR_GE, MTYPE_U4, 
								WN_COPY_Tree(wn_blocksize), 
								WN_Intconst(MTYPE_U4, ilimit));
	WN* wn_IfInnerTest = WN_Relational (OPR_LT, MTYPE_U4, 
								WN_COPY_Tree(wn_tid), 
								WN_Intconst(MTYPE_U4, ilimit/2));;
	WN* wn_OuterBlock = WN_CreateBlock();
	WN* wn_InnerBlock = WN_CreateBlock();
	WN* wn_CallSyncThreads = Gen_Sync_Threads();
	
	WN* Init0 = WN_Binary(OPR_ADD, MTYPE_U4, 
					WN_COPY_Tree(wn_tid), WN_Intconst(MTYPE_U4, ilimit/2));
	TY_IDX elem_ty;
	if(TY_kind(ST_type(st_shared_array)) == KIND_ARRAY)
		elem_ty = TY_etype(ST_type(st_shared_array));
	else if(TY_kind(ST_type(st_shared_array)) == KIND_POINTER)
		elem_ty = TY_pointed(ST_type(st_shared_array));
	else
    	Fail_FmtAssertion("invalid TY_kind. It must be ARRAY or POINTER in Gen_ReductionIfElseBlock1");	
			
	Init0 = ACC_LoadDeviceSharedArrayElem(Init0, st_shared_array);	
	Init0 = WN_Iload(TY_mtype(elem_ty), 0,  elem_ty, Init0);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ST_type(WN_st(wn_mySum))), WN_COPY_Tree(wn_mySum), Init0);
	Init0 = WN_Stid(TY_mtype(ST_type(WN_st(wn_mySum))), 0, WN_st(wn_mySum),
								ST_type(WN_st(wn_mySum)), Init0);	
	WN_INSERT_BlockLast(wn_InnerBlock,  Init0);


	Init0 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_btid), st_shared_array);	
	Init0 = WN_Istore(TY_mtype(elem_ty), 0, Make_Pointer_Type(elem_ty), Init0, WN_COPY_Tree(wn_mySum));	
	WN_INSERT_BlockLast(wn_InnerBlock,  Init0);
	Init0 = WN_CreateIf(wn_IfInnerTest, wn_InnerBlock, WN_CreateBlock());
	WN_INSERT_BlockLast(wn_OuterBlock,  Init0);
	if(ilimit > 64)//same warp, no necessary 
		WN_INSERT_BlockLast(wn_OuterBlock,  wn_CallSyncThreads);
	Init0 = WN_CreateIf(wn_IfOuterTest, wn_OuterBlock, WN_CreateBlock());
	return Init0;
}*/


static WN* Gen_ReductionIfElseBlock1(WN* wn_blocksize, WN* wn_tid, ST* st_shared_array,
						WN* wn_mySum, unsigned int ilimit, OPERATOR ReductionOpr)
{
	WN* wn_IfOuterTest = WN_Relational (OPR_GE, MTYPE_U4, 
								WN_COPY_Tree(wn_blocksize), 
								WN_Intconst(MTYPE_U4, ilimit));
	WN* wn_IfInnerTest = WN_Relational (OPR_LT, MTYPE_U4, 
								WN_COPY_Tree(wn_tid), 
								WN_Intconst(MTYPE_U4, ilimit/2));;
	WN* wn_OuterBlock = WN_CreateBlock();
	WN* wn_InnerBlock = WN_CreateBlock();
	WN* wn_CallSyncThreads = Gen_Sync_Threads();
	
	WN* Init0 = WN_Binary(OPR_ADD, MTYPE_U4, 
					WN_COPY_Tree(wn_tid), WN_Intconst(MTYPE_U4, ilimit/2));
	TY_IDX elem_ty;
	if(TY_kind(ST_type(st_shared_array)) == KIND_ARRAY)
		elem_ty = TY_etype(ST_type(st_shared_array));
	else if(TY_kind(ST_type(st_shared_array)) == KIND_POINTER)
		elem_ty = TY_pointed(ST_type(st_shared_array));
	else
    	Fail_FmtAssertion("invalid TY_kind. It must be ARRAY or POINTER in Gen_ReductionIfElseBlock1");	
			
	Init0 = ACC_LoadDeviceSharedArrayElem(Init0, st_shared_array);	
	Init0 = WN_Iload(TY_mtype(elem_ty), 0,  elem_ty, Init0);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ST_type(WN_st(wn_mySum))), WN_COPY_Tree(wn_mySum), Init0);
	Init0 = WN_Stid(TY_mtype(ST_type(WN_st(wn_mySum))), 0, WN_st(wn_mySum),
								ST_type(WN_st(wn_mySum)), Init0);	
	WN* Init1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	Init1 = WN_Istore(TY_mtype(elem_ty), 0, Make_Pointer_Type(elem_ty), 
								Init1, WN_COPY_Tree(wn_mySum));	
	
	if(ilimit > 64)
	{
		WN_INSERT_BlockLast(wn_InnerBlock,  Init0);
		WN_INSERT_BlockLast(wn_InnerBlock,  Init1);
		Init0 = WN_CreateIf(wn_IfInnerTest, wn_InnerBlock, WN_CreateBlock());
		WN_INSERT_BlockLast(wn_OuterBlock,  Init0);
		WN_INSERT_BlockLast(wn_OuterBlock,  wn_CallSyncThreads);
		Init0 = WN_CreateIf(wn_IfOuterTest, wn_OuterBlock, WN_CreateBlock());
	}
	else
	{
		WN_INSERT_BlockLast(wn_OuterBlock,  Init0);
		WN_INSERT_BlockLast(wn_OuterBlock,  Init1);
		Init0 = WN_CreateIf(wn_IfOuterTest, wn_OuterBlock, WN_CreateBlock());
	}

	
	return Init0;
}


static WN* Gen_ReductionIfElseBlock2(WN* wn_blocksize, WN* wn_tid, ST* st_shared_array,
						WN* wn_mySum, unsigned int ilimit, OPERATOR ReductionOpr)
{
	WN* wn_IfOuterTest = WN_Relational (OPR_GE, MTYPE_U4, 
								WN_COPY_Tree(wn_blocksize), 
								WN_Intconst(MTYPE_U4, ilimit));
	//WN* wn_IfInnerTest = WN_Relational (OPR_LT, TY_mtype(ST_type(WN_st(wn_tid))), 
	//							WN_COPY_Tree(wn_tid), 
	//							WN_Intconst(MTYPE_U4, ilimit/2));;
	WN* wn_OuterBlock = WN_CreateBlock();
	//WN* wn_InnerBlock = WN_CreateBlock();
	//WN* wn_CallSyncThreads = Gen_Sync_Threads();
	TY_IDX elem_ty;
	if(TY_kind(ST_type(st_shared_array)) == KIND_ARRAY)
		elem_ty = TY_etype(ST_type(st_shared_array));
	else if(TY_kind(ST_type(st_shared_array)) == KIND_POINTER)
		elem_ty = TY_pointed(ST_type(st_shared_array));
	else
    	Fail_FmtAssertion("invalid TY_kind. It must be ARRAY or POINTER in Gen_ReductionIfElseBlock2");	
	
	WN* Init0 = WN_Binary(OPR_ADD, MTYPE_U4, 
					WN_COPY_Tree(wn_tid), WN_Intconst(MTYPE_U4, ilimit/2));
	Init0 = ACC_LoadDeviceSharedArrayElem(Init0, st_shared_array);	
	Init0 = WN_Iload(TY_mtype(elem_ty), 0,  
									elem_ty, Init0);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ST_type(WN_st(wn_mySum))), WN_COPY_Tree(wn_mySum), Init0);
	Init0 = WN_Stid(TY_mtype(ST_type(WN_st(wn_mySum))), 0, WN_st(wn_mySum),
								ST_type(WN_st(wn_mySum)), Init0);	
	WN_INSERT_BlockLast(wn_OuterBlock,  Init0);


	Init0 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	Init0 = WN_Istore(TY_mtype(elem_ty), 0, 
					Make_Pointer_Type(elem_ty), Init0, WN_COPY_Tree(wn_mySum));	
	WN_INSERT_BlockLast(wn_OuterBlock,  Init0);
	//Init0 = WN_CreateIf(wn_IfInnerTest, wn_InnerBlock, WN_CreateBlock());
	//WN_INSERT_BlockLast(wn_OuterBlock,  Init0);
	//WN_INSERT_BlockLast(wn_OuterBlock,  wn_CallSyncThreads);
	Init0 = WN_CreateIf(wn_IfOuterTest, wn_OuterBlock, WN_CreateBlock());
	return Init0;
}

/*return the reduction device function name, every type of reduction will be return once.
For example,  if "+" was generated once, compiler won't generate another "+" kernel. 
It will just return the previous function name.
This function is only valid for Kernels outter Loop reduction.
*/
static ST* ACC_GenerateWorkerVectorReduction_unrolling(ACC_ReductionMap* pReduction_map)
{
	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
	WN* reduction_params = NULL;
	//reductionmap	
	ST *old_st = pReduction_map->hostName;
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	//WN_OFFSET old_offset = WN_offsetx(reduction_node);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    //char* localname; //= (char *) alloca(strlen(ST_name(old_st))+10);
	//sprintf ( localname, "__device_reduction_%s", ST_name(old_st));
	ST* st_kernel = ACC_Get_Reduction_devices(pReduction_map);
	if(st_kernel)
		return st_kernel;
	
	//generate new reuction kernels for this type and this operator
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	ACC_Push_Some_Globals( );
	ACC_Create_Reduction_Kernels(PAR_FUNC_ACC_DEVICE, pReduction_map);
	//////////////////////////////////////////////////////
	//make local declaress
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	ST* st_shared_array = st_input_data; //this is actually a shared memory buffer pointer 
	
	
	//WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
	//				0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	
	ST* st_tid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_tid,
	  Save_Str("tid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_tid = WN_Ldid(TY_mtype(ST_type(st_tid)), 
					0, st_tid, ST_type(st_tid));

	//my sum
	ST* st_mySum = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_mySum,
	  Save_Str("mySum"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty);
	WN* wn_mySum = WN_Ldid(TY_mtype(ST_type(st_mySum)), 
					0, st_mySum, ST_type(st_mySum));
	//blocksize
	ST* st_blockSize = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_blockSize,
	  Save_Str("blockSize"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_blockSize = WN_Ldid(TY_mtype(ST_type(st_blockSize)), 
					0, st_blockSize, ST_type(st_blockSize));
	//nextpow2
	ST* st_nextpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextpow2,
	  Save_Str("nextpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextpow2 = WN_Ldid(TY_mtype(ST_type(st_nextpow2)), 
					0, st_nextpow2, ST_type(st_nextpow2));

	
	//prevpow2
	ST* st_prevpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_prevpow2,
	  Save_Str("prevpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_prevpow2 = WN_Ldid(TY_mtype(ST_type(st_prevpow2)), 
					0, st_prevpow2, ST_type(st_prevpow2));
	
	
	//Set up predefined variable in CUDA
	WN* wn_threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* wn_threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* wn_threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* wn_blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* wn_blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* wn_blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* wn_blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* wn_blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* wn_blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* wn_griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* wn_griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* wn_griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));


	WN* Init0 = NULL;
	//pow2 alignment first
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_blockdimx), WN_COPY_Tree(wn_blockdimy));
	Init0 = WN_Stid(TY_mtype(ST_type(st_nextpow2)), 0, 
					st_nextpow2, ST_type(st_nextpow2), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//gen aligment stmt
	Gen_Next_Pow2DeviceStmt(wn_nextpow2, acc_reduction_func);

	//prevpow2 = nextpow2 >> 1
	Init0 = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_nextpow2), 
					WN_Intconst(TY_mtype(ST_type(st_nextpow2)), 1));
	Init0 = WN_Stid(TY_mtype(ST_type(st_prevpow2)), 0, 
					st_prevpow2, ST_type(st_prevpow2), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);


	//tid = threadIdx.y * blockdim.x + threadIdx.x
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_threadidy), WN_COPY_Tree(wn_blockdimx));
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						Init0, WN_COPY_Tree(wn_threadidx));
	Init0 = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);	
	//blocksize = blockdimx*blockdimy
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
									WN_COPY_Tree(wn_blockdimx), WN_COPY_Tree(wn_blockdimy));
	Init0 = WN_Stid(TY_mtype(ST_type(st_blockSize)), 0, st_blockSize, 
									ST_type(st_blockSize), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	/////////////////////////////////////////////////////////////////////////////////	
	WN* wn_IfTest1 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfBody1 = WN_CreateBlock();
	/////////////////////////////////////////////////////////////////////////////////
	WN* wn_nextIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
							WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfTest11 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_nextIndex), WN_COPY_Tree(wn_blockSize));
	WN* wn_IfBody11 = WN_CreateBlock();
	
	WN* wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextIndex), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	WN* wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);

	
	WN* wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);
	WN_INSERT_BlockLast( wn_IfBody11,  wn_shArr3);
	WN* wn_ifThenElse = WN_CreateIf(wn_IfTest11, wn_IfBody11, WN_CreateBlock());
	////////////////////////////////////////////////////////////////////////////////////////
	WN_INSERT_BlockLast( wn_IfBody1,  wn_ifThenElse);
	wn_ifThenElse = WN_CreateIf(wn_IfTest1, wn_IfBody1, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse);
	//init sum
	wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	//Init0 = ACC_Get_Init_Value_of_Reduction(ReductionOpr);
	Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), wn_shArr1);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Gen_Sync_Threads());
	//mySum += g_idata[btid];
	/*Init0 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_btid), st_input_data);
	Init0 = WN_Iload(TY_mtype(TY_pointed(ST_type(st_input_data))), 0,  
									TY_pointed(ST_type(st_input_data)), Init0);
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ST_type(st_mySum)), WN_COPY_Tree(wn_mySum), Init0);
	Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), Init0);*/


	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_shared_array, wn_mySum, 512, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_shared_array, wn_mySum, 256, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_shared_array, wn_mySum, 128, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	
	WN* wn_IfTest2 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_Intconst(MTYPE_U4, 32));
	WN* wn_IfBody2 = WN_CreateBlock();
	
    localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__smem_%s", ST_name(old_st));
	Set_TY_is_volatile(ty);
	TY_IDX ty_pointer = Make_Pointer_Type(ty);
	ST* st_smem_pointer = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_smem_pointer,
	  Save_Str(localname),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty_pointer);
	
    Init0 = WN_Ldid(Pointer_type, 0, st_shared_array, ST_type(st_shared_array));
	Init0 = WN_Stid(TY_mtype(ST_type(st_smem_pointer)), 0, 
					st_smem_pointer, ST_type(st_smem_pointer), Init0);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 64, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 32, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 16, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 8, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 4, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 2, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	WN* wn_ifThenElse2 = WN_CreateIf(wn_IfTest2, wn_IfBody2, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse2);
    WN_INSERT_BlockLast( acc_reduction_func,  Gen_Sync_Threads());

	
	
	
	//Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(WN_st(wn_tid))), 
	//				WN_COPY_Tree(wn_tid), WN_Intconst(MTYPE_U4, ilimit/2));
	/*Init0 = ACC_LoadDeviceSharedArrayElem(WN_Intconst(MTYPE_U4, 0), st_shared_array);	
	Init0 = WN_Iload(TY_mtype(TY_pointed(ST_type(st_shared_array))), 0,  
									TY_pointed(ST_type(st_shared_array)), Init0);
	
    WN* wn_return = WN_CreateReturn_Val (OPR_RETURN_VAL, TY_mtype(ST_type(old_st)), MTYPE_V, Init0);
	
	WN_INSERT_BlockLast( acc_reduction_func,  wn_return);//*/

	
	////////////////////////////////////////////////////////////////
	//restore info
	//ACC_Transfer_Maps ( acc_pmaptab, acc_cmaptab, acc_reduction_func, 
	//	  PU_Info_regions_ptr(Current_PU_Info) );

	/* Create a new dependence graph for the child  and move all the 
	 appropriate vertices from the parent to the child graph */

	//Current_Map_Tab = acc_cmaptab;
	//ACC_Fix_Dependence_Graph ( acc_ppuinfo, Current_PU_Info, acc_reduction_func); 
	//Current_Map_Tab = acc_pmaptab;

  
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );

	return acc_reduction_proc;
}


static ST* ACC_GenerateWorkerVectorReduction_rolling(ACC_ReductionMap* pReduction_map)
{
	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
	WN* reduction_params = NULL;
	//reductionmap	
	ST *old_st = pReduction_map->hostName;
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	//WN_OFFSET old_offset = WN_offsetx(reduction_node);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    //char* localname; //= (char *) alloca(strlen(ST_name(old_st))+10);
	//sprintf ( localname, "__device_reduction_%s", ST_name(old_st));
	ST* st_kernel = ACC_Get_Reduction_devices(pReduction_map);
	if(st_kernel)
		return st_kernel;
	
	//generate new reuction kernels for this type and this operator
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	ACC_Push_Some_Globals( );
	ACC_Create_Reduction_Kernels(PAR_FUNC_ACC_DEVICE, pReduction_map);
	//////////////////////////////////////////////////////
	//make local declaress
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	ST* st_shared_array = st_input_data; //this is actually a shared memory buffer pointer 
	
	
	//WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
	//				0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	
	ST* st_tid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_tid,
	  Save_Str("tid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_tid = WN_Ldid(TY_mtype(ST_type(st_tid)), 
					0, st_tid, ST_type(st_tid));

	//blocksize
	ST* st_blockSize = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_blockSize,
	  Save_Str("blockSize"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_blockSize = WN_Ldid(TY_mtype(ST_type(st_blockSize)), 
					0, st_blockSize, ST_type(st_blockSize));
	//nextpow2
	ST* st_nextpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextpow2,
	  Save_Str("nextpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextpow2 = WN_Ldid(TY_mtype(ST_type(st_nextpow2)), 
					0, st_nextpow2, ST_type(st_nextpow2));

	
	//prevpow2
	ST* st_prevpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_prevpow2,
	  Save_Str("prevpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_prevpow2 = WN_Ldid(TY_mtype(ST_type(st_prevpow2)), 
					0, st_prevpow2, ST_type(st_prevpow2));

	
	//next_index
	ST* st_nextIndex = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextIndex,
	  Save_Str("nextIndex"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextIndex = WN_Ldid(TY_mtype(ST_type(st_nextIndex)), 
					0, st_nextIndex, ST_type(st_nextIndex));
	//active thread id
	ST* st_active = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_active,
	  Save_Str("active"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_active = WN_Ldid(TY_mtype(ST_type(st_active)), 
					0, st_active, ST_type(st_active));
	
	
	//Set up predefined variable in CUDA
	WN* wn_threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* wn_threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* wn_threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* wn_blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* wn_blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* wn_blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* wn_blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* wn_blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* wn_blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* wn_griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* wn_griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* wn_griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));


	WN* Init0 = NULL;
	//pow2 alignment first
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_blockdimx), WN_COPY_Tree(wn_blockdimy));
	Init0 = WN_Stid(TY_mtype(ST_type(st_nextpow2)), 0, 
					st_nextpow2, ST_type(st_nextpow2), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//gen aligment stmt
	Gen_Next_Pow2DeviceStmt(wn_nextpow2, acc_reduction_func);

	//prevpow2 = nextpow2 >> 1
	Init0 = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_nextpow2), 
					WN_Intconst(TY_mtype(ST_type(st_nextpow2)), 1));
	Init0 = WN_Stid(TY_mtype(ST_type(st_prevpow2)), 0, 
					st_prevpow2, ST_type(st_prevpow2), Init0);


	//tid = threadIdx.y * blockdim.x + threadIdx.x
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_threadidy), WN_COPY_Tree(wn_blockdimx));
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						Init0, WN_COPY_Tree(wn_threadidx));
	Init0 = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);	
	/////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////	
	WN* wn_IfTest1 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfBody1 = WN_CreateBlock();
	/////////////////////////////////////////////////////////////////////////////////
	WN* wn_nextOprIdx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
							WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfTest11 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_nextOprIdx), WN_COPY_Tree(wn_blockSize));
	WN* wn_IfBody11 = WN_CreateBlock();
	
	WN* wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextOprIdx), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	WN* wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);

	
	WN* wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);
	WN_INSERT_BlockLast( wn_IfBody11,  wn_shArr3);
	WN* wn_ifThenElse = WN_CreateIf(wn_IfTest11, wn_IfBody11, WN_CreateBlock());
	////////////////////////////////////////////////////////////////////////////////////////
	WN_INSERT_BlockLast( wn_IfBody1,  wn_ifThenElse);
	wn_ifThenElse = WN_CreateIf(wn_IfTest1, wn_IfBody1, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse);
	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	//build INIT, TEST and Increment stmt for FORLOOP
	WN* wn_IndexInit = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_prevpow2)), 
					WN_COPY_Tree(wn_prevpow2), 
					WN_Intconst(TY_mtype(ST_type(st_prevpow2)), 1))	;
	wn_IndexInit = WN_Stid(TY_mtype(ST_type(st_active)), 0, 
					st_active, ST_type(st_active), wn_IndexInit);

	WN* wn_for_test = WN_Relational (OPR_GT, TY_mtype(ST_type(st_active)), 
								WN_COPY_Tree(wn_active), 
								WN_Intconst(TY_mtype(ST_type(st_active)), 0));
	
	WN* wn_IndexStep = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_active)), 
					WN_COPY_Tree(wn_active), 
					WN_Intconst(TY_mtype(ST_type(st_active)), 1));
	wn_IndexStep = WN_Stid(TY_mtype(ST_type(st_active)), 0, 
					st_active, ST_type(st_active), wn_IndexStep);

	//build body for FORLOOP
	WN* wn_forloopbody = WN_CreateBlock();
	WN* wn_nextIndexUpdate = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_active));
	wn_nextIndexUpdate = WN_Stid(TY_mtype(ST_type(st_nextIndex)), 0, 
					st_nextIndex, ST_type(st_nextIndex), wn_nextIndexUpdate);
	WN_INSERT_BlockLast( wn_forloopbody,  wn_nextIndexUpdate);	
	//if stmt
	WN* wn_if_test = WN_Relational (OPR_LT, TY_mtype(ST_type(st_active)), 
								WN_COPY_Tree(wn_tid), 
								WN_COPY_Tree(wn_active));
	WN* wn_if_thenbody = WN_CreateBlock();
	
	wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextIndex), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);
	
	wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);

	WN_INSERT_BlockLast( wn_if_thenbody,  wn_shArr3);
	WN* wn_if_stmt = WN_CreateIf(wn_if_test, wn_if_thenbody, WN_CreateBlock());
	
	WN_INSERT_BlockLast( wn_forloopbody,  wn_if_stmt);	
	WN_INSERT_BlockLast( wn_forloopbody,  Gen_Sync_Threads());	
	
	WN* wn_forloopidname = WN_CreateIdname(0,st_active);
	WN* wn_forloopstmt = WN_CreateDO(wn_forloopidname, wn_IndexInit, 
						wn_for_test, wn_IndexStep, wn_forloopbody, NULL);
	
	WN_INSERT_BlockLast( acc_reduction_func,  wn_forloopstmt);	
  
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );

	return acc_reduction_proc;
}

/*return the reduction device function name, every type of reduction will be return once.
For example,  if "+" was generated once, compiler won't generate another "+" kernel. 
It will just return the previous function name.
This function is only valid for Kernels outter Loop reduction.
*/
static ST* ACC_GenerateWorkerReduction_unrolling(ACC_ReductionMap* pReduction_map)
{
	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
	WN* reduction_params = NULL;
	//reductionmap	
	ST *old_st = pReduction_map->hostName;
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	//WN_OFFSET old_offset = WN_offsetx(reduction_node);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    //char* localname; //= (char *) alloca(strlen(ST_name(old_st))+10);
	//sprintf ( localname, "__device_reduction_%s", ST_name(old_st));
	ST* st_kernel = ACC_Get_Reduction_devices(pReduction_map);
	if(st_kernel)
		return st_kernel;
	
	//generate new reuction kernels for this type and this operator
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	ACC_Push_Some_Globals( );
	ACC_Create_Reduction_Kernels(PAR_FUNC_ACC_DEVICE, pReduction_map);
	//////////////////////////////////////////////////////
	//make local declaress
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	/*sprintf ( localname, "__sdata_%s", ST_name(old_st));
	TY_IDX ty_array = Make_Pointer_Type(ty);//Make_Array_Type(TY_mtype(ty), 1, 1024);
	ST* st_shared_array = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_shared_array,
	  Save_Str(localname),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty_array);
	Set_ST_ACC_shared_array(*st_shared_array);*/
	ST* st_shared_array = st_input_data;
	
	
	//WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
	//				0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	
	ST* st_tid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_tid,
	  Save_Str("tid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_tid = WN_Ldid(TY_mtype(ST_type(st_tid)), 
					0, st_tid, ST_type(st_tid));
	//i
	ST* st_loop_index = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_loop_index,
	  Save_Str("i"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_loop_index = WN_Ldid(TY_mtype(ST_type(st_loop_index)), 
					0, st_loop_index, ST_type(st_loop_index));
	//
	ST* st_gridSize = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_gridSize,
	  Save_Str("gridSize"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_gridSize = WN_Ldid(TY_mtype(ST_type(st_gridSize)), 
					0, st_gridSize, ST_type(st_gridSize));
	//my sum
	ST* st_mySum = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_mySum,
	  Save_Str("mySum"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty);
	WN* wn_mySum = WN_Ldid(TY_mtype(ST_type(st_mySum)), 
					0, st_mySum, ST_type(st_mySum));
	

	//nextpow2
	ST* st_nextpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextpow2,
	  Save_Str("nextpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextpow2 = WN_Ldid(TY_mtype(ST_type(st_nextpow2)), 
					0, st_nextpow2, ST_type(st_nextpow2));

	
	//prevpow2
	ST* st_prevpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_prevpow2,
	  Save_Str("prevpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_prevpow2 = WN_Ldid(TY_mtype(ST_type(st_prevpow2)), 
					0, st_prevpow2, ST_type(st_prevpow2));
	
	
	WN* wn_blocksize = WN_Ldid(TY_mtype(ST_type(st_gridSize)), 
					0, st_gridSize, ST_type(st_gridSize));
	//Set up predefined variable in CUDA
	WN* wn_threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* wn_threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* wn_threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* wn_blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* wn_blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* wn_blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* wn_blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* wn_blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* wn_blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* wn_griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* wn_griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* wn_griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));
	
	WN* Init0 = NULL;
	//pow2 alignment first
	//Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_nextpow2)), 
	//				WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockdimy));
	Init0 = WN_Stid(TY_mtype(ST_type(st_nextpow2)), 0, 
					st_nextpow2, ST_type(st_nextpow2), WN_COPY_Tree(wn_blockdimy));
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//gen aligment stmt
	Gen_Next_Pow2DeviceStmt(wn_nextpow2, acc_reduction_func);

	//prevpow2 = nextpow2 >> 1
	Init0 = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_nextpow2), 
					WN_Intconst(TY_mtype(ST_type(st_nextpow2)), 1));
	Init0 = WN_Stid(TY_mtype(ST_type(st_prevpow2)), 0, 
					st_prevpow2, ST_type(st_prevpow2), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);	


	//tid = threadIdx.y * blockdim.x + threadIdx.x
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_threadidy), WN_COPY_Tree(wn_blockdimx));
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						Init0, WN_COPY_Tree(wn_threadidx));
	Init0 = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);	
	
	//Init0 = ACC_Get_Init_Value_of_Reduction(ReductionOpr);
	//Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), Init0);
	//WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	//////////////////////////////////////////////////////////////////////////////////	
	WN* wn_IfTest1 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfBody1 = WN_CreateBlock();
	/////////////////////////////////////////////////////////////////////////////////
	WN* wn_nextIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
							WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfTest11 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_nextIndex), WN_COPY_Tree(wn_blockdimy));
	WN* wn_IfBody11 = WN_CreateBlock();
	
	WN* wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextIndex), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	WN* wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);

	
	WN* wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);
	WN_INSERT_BlockLast( wn_IfBody11,  wn_shArr3);
	WN* wn_ifThenElse = WN_CreateIf(wn_IfTest11, wn_IfBody11, WN_CreateBlock());
	////////////////////////////////////////////////////////////////////////////////////////
	WN_INSERT_BlockLast( wn_IfBody1,  wn_ifThenElse);
	wn_ifThenElse = WN_CreateIf(wn_IfTest1, wn_IfBody1, WN_CreateBlock());

	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse);
	////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\	
	//init sum
	wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), wn_shArr1);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Gen_Sync_Threads());	
	
	/////////////////////////////////////////////////////////////////////////////////
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_shared_array, wn_mySum, 512, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_shared_array, wn_mySum, 256, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_shared_array, wn_mySum, 128, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	
	WN* wn_IfTest2 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_Intconst(MTYPE_U4, 32));
	WN* wn_IfBody2 = WN_CreateBlock();
	
    localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__smem_%s", ST_name(old_st));
	Set_TY_is_volatile(ty);
	TY_IDX ty_pointer = Make_Pointer_Type(ty);
	ST* st_smem_pointer = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_smem_pointer,
	  Save_Str(localname),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty_pointer);
	
    Init0 = WN_Ldid(Pointer_type, 0, st_shared_array, ST_type(st_shared_array));
	Init0 = WN_Stid(TY_mtype(ST_type(st_smem_pointer)), 0, 
					st_smem_pointer, ST_type(st_smem_pointer), Init0);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 64, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 32, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 16, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 8, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 4, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 2, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	WN* wn_ifThenElse2 = WN_CreateIf(wn_IfTest2, wn_IfBody2, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse2);
    WN_INSERT_BlockLast( acc_reduction_func,  Gen_Sync_Threads());

	
	
	
	//Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(WN_st(wn_tid))), 
	//				WN_COPY_Tree(wn_tid), WN_Intconst(MTYPE_U4, ilimit/2));
	/*Init0 = ACC_LoadDeviceSharedArrayElem(WN_Intconst(MTYPE_U4, 0), st_shared_array);	
	Init0 = WN_Iload(TY_mtype(TY_pointed(ST_type(st_shared_array))), 0,  
									TY_pointed(ST_type(st_shared_array)), Init0);
	
    WN* wn_return = WN_CreateReturn_Val (OPR_RETURN_VAL, TY_mtype(ST_type(old_st)), MTYPE_V, Init0);
	
	WN_INSERT_BlockLast( acc_reduction_func,  wn_return);//*/
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );

	return acc_reduction_proc;
}

static ST* ACC_GenerateWorkerReduction_rolling(ACC_ReductionMap* pReduction_map)
{
	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
	WN* reduction_params = NULL;
	//reductionmap	
	ST *old_st = pReduction_map->hostName;
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	//WN_OFFSET old_offset = WN_offsetx(reduction_node);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    //char* localname; //= (char *) alloca(strlen(ST_name(old_st))+10);
	//sprintf ( localname, "__device_reduction_%s", ST_name(old_st));
	ST* st_kernel = ACC_Get_Reduction_devices(pReduction_map);
	if(st_kernel)
		return st_kernel;
	
	//generate new reuction kernels for this type and this operator
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	ACC_Push_Some_Globals( );
	ACC_Create_Reduction_Kernels(PAR_FUNC_ACC_DEVICE, pReduction_map);
	//////////////////////////////////////////////////////
	//make local declaress
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	ST* st_shared_array = st_input_data; //this is actually a shared memory buffer pointer 
	
	
	//WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
	//				0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	
	ST* st_tid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_tid,
	  Save_Str("tid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_tid = WN_Ldid(TY_mtype(ST_type(st_tid)), 
					0, st_tid, ST_type(st_tid));

	//blocksize
	ST* st_blockSize = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_blockSize,
	  Save_Str("blockSize"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_blockSize = WN_Ldid(TY_mtype(ST_type(st_blockSize)), 
					0, st_blockSize, ST_type(st_blockSize));
	//nextpow2
	ST* st_nextpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextpow2,
	  Save_Str("nextpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextpow2 = WN_Ldid(TY_mtype(ST_type(st_nextpow2)), 
					0, st_nextpow2, ST_type(st_nextpow2));

	
	//prevpow2
	ST* st_prevpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_prevpow2,
	  Save_Str("prevpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_prevpow2 = WN_Ldid(TY_mtype(ST_type(st_prevpow2)), 
					0, st_prevpow2, ST_type(st_prevpow2));

	
	//next_index
	ST* st_nextIndex = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextIndex,
	  Save_Str("nextIndex"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextIndex = WN_Ldid(TY_mtype(ST_type(st_nextIndex)), 
					0, st_nextIndex, ST_type(st_nextIndex));
	//active thread id
	ST* st_active = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_active,
	  Save_Str("active"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_active = WN_Ldid(TY_mtype(ST_type(st_active)), 
					0, st_active, ST_type(st_active));
	
	
	//Set up predefined variable in CUDA
	WN* wn_threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* wn_threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* wn_threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* wn_blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* wn_blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* wn_blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* wn_blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* wn_blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* wn_blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* wn_griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* wn_griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* wn_griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));


	WN* Init0 = NULL;
	//pow2 alignment first
	//Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_nextpow2)), 
	//				WN_COPY_Tree(wn_blockdimx), WN_COPY_Tree(wn_blockdimy));
	Init0 = WN_Stid(TY_mtype(ST_type(st_nextpow2)), 0, 
					st_nextpow2, ST_type(st_nextpow2), WN_COPY_Tree(wn_blockdimy));
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//gen aligment stmt
	Gen_Next_Pow2DeviceStmt(wn_nextpow2, acc_reduction_func);

	//prevpow2 = nextpow2 >> 1
	Init0 = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_nextpow2), 
					WN_Intconst(TY_mtype(ST_type(st_nextpow2)), 1));
	Init0 = WN_Stid(TY_mtype(ST_type(st_prevpow2)), 0, 
					st_prevpow2, ST_type(st_prevpow2), Init0);


	//tid = threadIdx.y * blockdim.x + threadIdx.x
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_threadidy), WN_COPY_Tree(wn_blockdimx));
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						Init0, WN_COPY_Tree(wn_threadidx));
	Init0 = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);	
	/////////////////////////////
	WN* wn_IfTest1 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfBody1 = WN_CreateBlock();
	/////////////////////////////////////////////////////////////////////////////////
	WN* wn_nextOprIdx = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
							WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfTest11 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_nextOprIdx), WN_COPY_Tree(wn_blockSize));
	WN* wn_IfBody11 = WN_CreateBlock();
	
	WN* wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextOprIdx), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	WN* wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);

	
	WN* wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);
	WN_INSERT_BlockLast( wn_IfBody11,  wn_shArr3);
	WN* wn_ifThenElse = WN_CreateIf(wn_IfTest11, wn_IfBody11, WN_CreateBlock());
	//////////////////////////////////////////////////////////////////////////////////
	WN_INSERT_BlockLast( wn_IfBody1,  wn_ifThenElse);
	wn_ifThenElse = WN_CreateIf(wn_IfTest1, wn_IfBody1, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse);

	/////////////////////////////////////////////////////////////////////////////////
	//build INIT, TEST and Increment stmt for FORLOOP
	WN* wn_IndexInit = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_prevpow2)), 
					WN_COPY_Tree(wn_prevpow2), 
					WN_Intconst(TY_mtype(ST_type(st_prevpow2)), 1))	;
	wn_IndexInit = WN_Stid(TY_mtype(ST_type(st_active)), 0, 
					st_active, ST_type(st_active), wn_IndexInit);

	WN* wn_for_test = WN_Relational (OPR_GT, TY_mtype(ST_type(st_active)), 
								WN_COPY_Tree(wn_active), 
								WN_Intconst(TY_mtype(ST_type(st_active)), 0));
	
	WN* wn_IndexStep = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_active)), 
					WN_COPY_Tree(wn_active), 
					WN_Intconst(TY_mtype(ST_type(st_active)), 1));
	wn_IndexStep = WN_Stid(TY_mtype(ST_type(st_active)), 0, 
					st_active, ST_type(st_active), wn_IndexStep);

	//build body for FORLOOP
	WN* wn_forloopbody = WN_CreateBlock();
	WN* wn_nextIndexUpdate = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_active));
	wn_nextIndexUpdate = WN_Stid(TY_mtype(ST_type(st_nextIndex)), 0, 
					st_nextIndex, ST_type(st_nextIndex), wn_nextIndexUpdate);
	WN_INSERT_BlockLast( wn_forloopbody,  wn_nextIndexUpdate);	
	//if stmt
	WN* wn_if_test = WN_Relational (OPR_LT, TY_mtype(ST_type(st_active)), 
								WN_COPY_Tree(wn_tid), 
								WN_COPY_Tree(wn_active));
	
	WN* wn_if_thenbody = WN_CreateBlock();
	
	wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextIndex), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);
	
	wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);

	WN_INSERT_BlockLast( wn_if_thenbody,  wn_shArr3);
	WN* wn_if_stmt = WN_CreateIf(wn_if_test, wn_if_thenbody, WN_CreateBlock());
	
	WN_INSERT_BlockLast( wn_forloopbody,  wn_if_stmt);	
	WN_INSERT_BlockLast( wn_forloopbody,  Gen_Sync_Threads());	
	
	WN* wn_forloopidname = WN_CreateIdname(0,st_active);
	WN* wn_forloopstmt = WN_CreateDO(wn_forloopidname, wn_IndexInit, 
						wn_for_test, wn_IndexStep, wn_forloopbody, NULL);
	
	WN_INSERT_BlockLast( acc_reduction_func,  wn_forloopstmt);	
  
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );

	return acc_reduction_proc;
}


/*return the reduction device function name, every type of reduction will be return once.
For example,  if "+" was generated once, compiler won't generate another "+" kernel. 
It will just return the previous function name.
This function is only valid for Kernels outter Loop reduction.
*/
static ST* ACC_GenerateVectorReduction_unrolling(ACC_ReductionMap* pReduction_map)
{
	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
	WN* reduction_params = NULL;
	//reductionmap	
	ST *old_st = pReduction_map->hostName;
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	//WN_OFFSET old_offset = WN_offsetx(reduction_node);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    //char* localname; //= (char *) alloca(strlen(ST_name(old_st))+10);
	//sprintf ( localname, "__device_reduction_%s", ST_name(old_st));
	ST* st_kernel = ACC_Get_Reduction_devices(pReduction_map);
	if(st_kernel)
		return st_kernel;
	
	
	//generate new reuction kernels for this type and this operator
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	ACC_Push_Some_Globals( );
	ACC_Create_Reduction_Kernels(PAR_FUNC_ACC_DEVICE, pReduction_map);
	//////////////////////////////////////////////////////
	//make local declaress
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	ST* st_shared_array = st_input_data; //this is actually a shared memory buffer pointer 
	
	
	//WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
	//				0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	
	ST* st_tid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_tid,
	  Save_Str("tid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_tid = WN_Ldid(TY_mtype(ST_type(st_tid)), 
					0, st_tid, ST_type(st_tid));

	//block tid, because blockdim.y may larger than 1.	
	/*ST* st_btid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_btid,
	  Save_Str("btid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_btid = WN_Ldid(TY_mtype(ST_type(st_btid)), 
					0, st_btid, ST_type(st_btid));*/
	
	//my sum
	ST* st_mySum = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_mySum,
	  Save_Str("mySum"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty);
	WN* wn_mySum = WN_Ldid(TY_mtype(ST_type(st_mySum)), 
					0, st_mySum, ST_type(st_mySum));
	//nextpow2
	ST* st_nextpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextpow2,
	  Save_Str("nextpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextpow2 = WN_Ldid(TY_mtype(ST_type(st_nextpow2)), 
					0, st_nextpow2, ST_type(st_nextpow2));

	
	//prevpow2
	ST* st_prevpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_prevpow2,
	  Save_Str("prevpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_prevpow2 = WN_Ldid(TY_mtype(ST_type(st_prevpow2)), 
					0, st_prevpow2, ST_type(st_prevpow2));
	
    localname = (char *) alloca(strlen(ST_name(old_st))+30);
	sprintf ( localname, "__smem_local_%s", ST_name(old_st));
	TY_IDX ty_pointer = Make_Pointer_Type(ty);
	ST* st_smemlocal_pointer = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_smemlocal_pointer,
	  Save_Str(localname),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty_pointer);
	WN* wn_localMem = WN_Ldid(Pointer_type, 0, st_smemlocal_pointer, 
				ST_type(st_smemlocal_pointer));
	
	//Set up predefined variable in CUDA
	WN* wn_threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* wn_threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* wn_threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* wn_blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* wn_blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* wn_blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* wn_blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* wn_blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* wn_blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* wn_griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* wn_griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* wn_griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));

	WN* wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
								WN_COPY_Tree(wn_threadidy), WN_COPY_Tree(wn_blockdimx));
	wn_offset = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)),
		wn_offset, WN_Intconst(TY_mtype(ST_type(glbl_blockDim_x)),	TY_size(ty)));
	WN* wn_base = WN_Ldid(Pointer_type, 0, st_shared_array, ST_type(st_shared_array));
	wn_offset = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockDim_x)),
		wn_offset, wn_base);
	wn_offset = WN_Stid(Pointer_type, 0, st_smemlocal_pointer, 
				ST_type(st_smemlocal_pointer), wn_offset); 
	WN_INSERT_BlockLast( acc_reduction_func,  wn_offset);

	
	WN* Init0 = NULL;
	//pow2 alignment first
	Init0 = WN_Stid(TY_mtype(ST_type(st_nextpow2)), 0, 
					st_nextpow2, ST_type(st_nextpow2), WN_COPY_Tree(wn_blockdimx));
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//gen aligment stmt
	Gen_Next_Pow2DeviceStmt(wn_nextpow2, acc_reduction_func);

	//prevpow2 = nextpow2 >> 1
	Init0 = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_nextpow2), 
					WN_Intconst(TY_mtype(ST_type(st_nextpow2)), 1));
	Init0 = WN_Stid(TY_mtype(ST_type(st_prevpow2)), 0, 
					st_prevpow2, ST_type(st_prevpow2), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);


	//tid in the block, btid = threadIdx.y * blockdim.x + threadIdx.x
	//Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
	//					WN_COPY_Tree(wn_threadidy), WN_COPY_Tree(wn_blockdimx));
	//Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
	//					Init0, WN_COPY_Tree(wn_threadidx));
	//Init0 = WN_Stid(TY_mtype(ST_type(st_btid)), 0, st_btid, ST_type(st_btid), Init0);
	//WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	
	//tid in single y direction	
	Init0 = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), WN_COPY_Tree(wn_threadidx));
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	
	//blocksize = blockdimx*blockdimy
	//Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
	//								WN_COPY_Tree(blockdimx), WN_COPY_Tree(blockdimy));
	//Init0 = WN_Stid(TY_mtype(ST_type(st_blockSize)), 0, st_blockSize, 
	//								ST_type(st_blockSize), Init0);
	//WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//
	//Init0 = ACC_Get_Init_Value_of_Reduction(ReductionOpr);
	//Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), Init0);
	//WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	//
	
	WN* wn_IfTest1 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfBody1 = WN_CreateBlock();
	/////////////////////////////////////////////////////////////////////////////////
	WN* wn_nextIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
							WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_prevpow2));
	//WN* wn_btidnextIndex = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
	//						WN_COPY_Tree(wn_btid), WN_COPY_Tree(wn_prevpow2));
	WN* wn_IfTest11 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_nextIndex), WN_COPY_Tree(wn_blockdimx));
	WN* wn_IfBody11 = WN_CreateBlock();
	
	WN* wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextIndex), st_smemlocal_pointer);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	WN* wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_smemlocal_pointer);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);

	
	WN* wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_smemlocal_pointer);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);
	WN_INSERT_BlockLast( wn_IfBody11,  wn_shArr3);
	WN* wn_ifThenElse = WN_CreateIf(wn_IfTest11, wn_IfBody11, WN_CreateBlock());
	////////////////////////////////////////////////////////////////////////////////////////
	WN_INSERT_BlockLast( wn_IfBody1,  wn_ifThenElse);
	wn_ifThenElse = WN_CreateIf(wn_IfTest1, wn_IfBody1, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse);
	////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\	
	////////////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\	
	//init sum
	wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_smemlocal_pointer);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), wn_shArr1);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Gen_Sync_Threads());
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smemlocal_pointer, wn_mySum, 512, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smemlocal_pointer, wn_mySum, 256, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smemlocal_pointer, wn_mySum, 128, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	
	WN* wn_IfTest2 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_Intconst(MTYPE_U4, 32));
	WN* wn_IfBody2 = WN_CreateBlock();
	
    localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__smem_%s", ST_name(old_st));
	Set_TY_is_volatile(ty);
	ty_pointer = Make_Pointer_Type(ty);
	ST* st_smem_pointer = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_smem_pointer,
	  Save_Str(localname),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty_pointer);
	
    Init0 = WN_Ldid(Pointer_type, 0, st_smemlocal_pointer, ST_type(st_smemlocal_pointer));
	Init0 = WN_Stid(TY_mtype(ST_type(st_smem_pointer)), 0, 
					st_smem_pointer, ST_type(st_smem_pointer), Init0);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 64, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 32, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 16, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 8, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 4, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_prevpow2, wn_tid, st_smem_pointer, wn_mySum, 2, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	WN* wn_ifThenElse2 = WN_CreateIf(wn_IfTest2, wn_IfBody2, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse2);
    WN_INSERT_BlockLast( acc_reduction_func,  Gen_Sync_Threads());
	
	 
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );

	return acc_reduction_proc;
}


static ST* ACC_GenerateVectorReduction_rolling(ACC_ReductionMap* pReduction_map)
{
	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
	WN* reduction_params = NULL;
	//reductionmap	
	ST *old_st = pReduction_map->hostName;
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	//WN_OFFSET old_offset = WN_offsetx(reduction_node);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    //char* localname; //= (char *) alloca(strlen(ST_name(old_st))+10);
	//sprintf ( localname, "__device_reduction_%s", ST_name(old_st));
	ST* st_kernel = ACC_Get_Reduction_devices(pReduction_map);
	if(st_kernel)
		return st_kernel;
	
	//generate new reuction kernels for this type and this operator
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	ACC_Push_Some_Globals( );
	ACC_Create_Reduction_Kernels(PAR_FUNC_ACC_DEVICE, pReduction_map);
	//////////////////////////////////////////////////////
	//make local declaress
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	ST* st_shared_array = st_input_data; //this is actually a shared memory buffer pointer 
	
	
	//WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
	//				0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	
	ST* st_tid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_tid,
	  Save_Str("tid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_tid = WN_Ldid(TY_mtype(ST_type(st_tid)), 
					0, st_tid, ST_type(st_tid));

	//blocksize
	ST* st_blockSize = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_blockSize,
	  Save_Str("blockSize"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_blockSize = WN_Ldid(TY_mtype(ST_type(st_blockSize)), 
					0, st_blockSize, ST_type(st_blockSize));
	//nextpow2
	ST* st_nextpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextpow2,
	  Save_Str("nextpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextpow2 = WN_Ldid(TY_mtype(ST_type(st_nextpow2)), 
					0, st_nextpow2, ST_type(st_nextpow2));

	
	//prevpow2
	ST* st_prevpow2 = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_prevpow2,
	  Save_Str("prevpow2"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_prevpow2 = WN_Ldid(TY_mtype(ST_type(st_prevpow2)), 
					0, st_prevpow2, ST_type(st_prevpow2));

	
	//next_index
	ST* st_nextIndex = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_nextIndex,
	  Save_Str("nextIndex"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_nextIndex = WN_Ldid(TY_mtype(ST_type(st_nextIndex)), 
					0, st_nextIndex, ST_type(st_nextIndex));
	//active thread id
	ST* st_active = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_active,
	  Save_Str("active"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));	
	WN* wn_active = WN_Ldid(TY_mtype(ST_type(st_active)), 
					0, st_active, ST_type(st_active));
	
	
	//Set up predefined variable in CUDA
	WN* wn_threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* wn_threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* wn_threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* wn_blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* wn_blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* wn_blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* wn_blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* wn_blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* wn_blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* wn_griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* wn_griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* wn_griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));


	WN* Init0 = NULL;
	//pow2 alignment first
	//Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(st_nextpow2)), 
	//				WN_COPY_Tree(wn_blockdimx), WN_COPY_Tree(wn_blockdimy));
	Init0 = WN_Stid(TY_mtype(ST_type(st_nextpow2)), 0, 
					st_nextpow2, ST_type(st_nextpow2), WN_COPY_Tree(wn_blockdimx));
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//gen aligment stmt
	Gen_Next_Pow2DeviceStmt(wn_nextpow2, acc_reduction_func);

	//prevpow2 = nextpow2 >> 1
	Init0 = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_nextpow2)), 
					WN_COPY_Tree(wn_nextpow2), 
					WN_Intconst(TY_mtype(ST_type(st_nextpow2)), 1));
	Init0 = WN_Stid(TY_mtype(ST_type(st_prevpow2)), 0, 
					st_prevpow2, ST_type(st_prevpow2), Init0);


	//tid = threadIdx.y * blockdim.x + threadIdx.x
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_threadidy), WN_COPY_Tree(wn_blockdimx));
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						Init0, WN_COPY_Tree(wn_threadidx));
	Init0 = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);	

	/////////////////////////////
	//build INIT, TEST and Increment stmt for FORLOOP
	WN* wn_IndexInit = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_prevpow2)), 
					WN_COPY_Tree(wn_prevpow2), 
					WN_Intconst(TY_mtype(ST_type(st_prevpow2)), 1))	;
	wn_IndexInit = WN_Stid(TY_mtype(ST_type(st_active)), 0, 
					st_active, ST_type(st_active), wn_IndexInit);

	WN* wn_for_test = WN_Relational (OPR_GT, TY_mtype(ST_type(st_active)), 
								WN_COPY_Tree(wn_active), 
								WN_Intconst(TY_mtype(ST_type(st_active)), 0));
	
	WN* wn_IndexStep = WN_Binary(OPR_ASHR, TY_mtype(ST_type(st_active)), 
					WN_COPY_Tree(wn_active), 
					WN_Intconst(TY_mtype(ST_type(st_active)), 1));
	wn_IndexStep = WN_Stid(TY_mtype(ST_type(st_active)), 0, 
					st_active, ST_type(st_active), wn_IndexStep);

	//build body for FORLOOP
	WN* wn_forloopbody = WN_CreateBlock();
	WN* wn_nextIndexUpdate = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), 
						WN_COPY_Tree(wn_tid), WN_COPY_Tree(wn_active));
	wn_nextIndexUpdate = WN_Stid(TY_mtype(ST_type(st_nextIndex)), 0, 
					st_nextIndex, ST_type(st_nextIndex), wn_nextIndexUpdate);
	WN_INSERT_BlockLast( wn_forloopbody,  wn_nextIndexUpdate);	
	//if stmt
	WN* wn_if_test = WN_Relational (OPR_LT, TY_mtype(ST_type(st_active)), 
								WN_COPY_Tree(wn_threadidx), 
								WN_COPY_Tree(wn_active));
	
	WN* wn_if_thenbody = WN_CreateBlock();
	
	WN* wn_shArr1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_nextIndex), st_shared_array);	
	wn_shArr1 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr1);
	
	WN* wn_shArr2 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);	
	wn_shArr2 = WN_Iload(TY_mtype(ty), 0,  ty, wn_shArr2);
	
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ty), wn_shArr2, wn_shArr1);
	
	WN* wn_shArr3 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	wn_shArr3 = WN_Istore(TY_mtype(ty), 0, Make_Pointer_Type(ty), wn_shArr3, Init0);

	WN_INSERT_BlockLast( wn_if_thenbody,  wn_shArr3);
	WN* wn_if_stmt = WN_CreateIf(wn_if_test, wn_if_thenbody, WN_CreateBlock());
	
	WN_INSERT_BlockLast( wn_forloopbody,  wn_if_stmt);	
	WN_INSERT_BlockLast( wn_forloopbody,  Gen_Sync_Threads());	
	
	WN* wn_forloopidname = WN_CreateIdname(0,st_active);
	WN* wn_forloopstmt = WN_CreateDO(wn_forloopidname, wn_IndexInit, 
						wn_for_test, wn_IndexStep, wn_forloopbody, NULL);
	
	WN_INSERT_BlockLast( acc_reduction_func,  wn_forloopstmt);	
  
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );

	return acc_reduction_proc;
}


/*return the reduction kernels name, every type of reduction will be return once.
For example,  if "+" was generated once, compiler won't generate another "+" kernel. 
It will just return the previous function name.
This function is only valid for Kernels outter Loop reduction.
*/
static ST* ACC_GenerateReduction_Kernels_TopLoop(ACC_ReductionMap* pReduction_map)
{
	
	OPERATOR ReductionOpr = pReduction_map->ReductionOpr;
	WN* reduction_params = NULL;
	//reductionmap	
	ST *old_st = pReduction_map->hostName;
	INT64 acc_dtype = 
			GetKernelParamType(old_st);
	//WN_OFFSET old_offset = WN_offsetx(reduction_node);		
    TY_IDX ty = ST_type(old_st);
    TY_KIND kind = TY_kind(ty);//ST_name(old_st)
    //char* localname; //= (char *) alloca(strlen(ST_name(old_st))+10);
	//sprintf ( localname, "__device_reduction_%s", ST_name(old_st));
	ST* st_kernel = ACC_Get_Reduction_kernels(ReductionOpr, ty);
	if(st_kernel)
		return st_kernel;
	
	//generate new reuction kernels for this type and this operator
	acc_psymtab = CURRENT_SYMTAB;
	acc_ppuinfo = Current_PU_Info;
	acc_pmaptab = Current_Map_Tab;
	ACC_Push_Some_Globals( );
	ACC_Create_Reduction_Kernels(PAR_FUNC_ACC_KERNEL, pReduction_map);
	//////////////////////////////////////////////////////
	//make local declaress
    char* localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__sdata_%s", ST_name(old_st));
	TY_IDX ty_array = Make_Pointer_Type(ty);//Make_Array_Type(TY_mtype(ty), 1, 1024);
	ST* st_shared_array = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_shared_array,
	  Save_Str(localname),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty_array);
	Set_ST_ACC_shared_array(*st_shared_array);
	
	
	//WN* threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
	//				0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	
	ST* st_tid = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_tid,
	  Save_Str("tid"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_tid = WN_Ldid(TY_mtype(ST_type(st_tid)), 
					0, st_tid, ST_type(st_tid));
	//i
	ST* st_loop_index = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_loop_index,
	  Save_Str("i"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_loop_index = WN_Ldid(TY_mtype(ST_type(st_loop_index)), 
					0, st_loop_index, ST_type(st_loop_index));
	//
	ST* st_gridSize = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_gridSize,
	  Save_Str("gridSize"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  Be_Type_Tbl(MTYPE_U4));
	WN* wn_gridSize = WN_Ldid(TY_mtype(ST_type(st_gridSize)), 
					0, st_gridSize, ST_type(st_gridSize));
	//my sum
	ST* st_mySum = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_mySum,
	  Save_Str("mySum"),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty);
	WN* wn_mySum = WN_Ldid(TY_mtype(ST_type(st_mySum)), 
					0, st_mySum, ST_type(st_mySum));

	
	sprintf ( localname, "is_power2");
	ST* st_IsPow2 = New_ST( CURRENT_SYMTAB );
	ST_Init(st_IsPow2,
			Save_Str("is_power2"),
			CLASS_VAR,
			SCLASS_FORMAL,
			EXPORT_LOCAL,
			Be_Type_Tbl(MTYPE_U4));
	WN* wn_IsPow2 = WN_Ldid(TY_mtype(ST_type(st_IsPow2)), 
					0, st_IsPow2, ST_type(st_IsPow2));
	
	
	//Set up predefined variable in CUDA
	WN* wn_threadidx = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_x)), 
					0, glbl_threadIdx_x, ST_type(glbl_threadIdx_x));
	WN* wn_threadidy = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_y)), 
					0, glbl_threadIdx_y, ST_type(glbl_threadIdx_y));
	WN* wn_threadidz = WN_Ldid(TY_mtype(ST_type(glbl_threadIdx_z)), 
					0, glbl_threadIdx_z, ST_type(glbl_threadIdx_z));
	
	WN* wn_blockidx = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_x)), 
					0, glbl_blockIdx_x, ST_type(glbl_blockIdx_x));
	WN* wn_blockidy = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_y)), 
					0, glbl_blockIdx_y, ST_type(glbl_blockIdx_y));
	WN* wn_blockidz = WN_Ldid(TY_mtype(ST_type(glbl_blockIdx_z)), 
					0, glbl_blockIdx_z, ST_type(glbl_blockIdx_z));
	
	WN* wn_blockdimx = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_x)), 
					0, glbl_blockDim_x, ST_type(glbl_blockDim_x));
	WN* wn_blockdimy = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_y)), 
					0, glbl_blockDim_y, ST_type(glbl_blockDim_y));
	WN* wn_blockdimz = WN_Ldid(TY_mtype(ST_type(glbl_blockDim_z)), 
					0, glbl_blockDim_z, ST_type(glbl_blockDim_z));
	
	WN* wn_griddimx = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_x)), 
					0, glbl_gridDim_x, ST_type(glbl_gridDim_x));
	WN* wn_griddimy = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_y)), 
					0, glbl_gridDim_y, ST_type(glbl_gridDim_y));
	WN* wn_griddimz = WN_Ldid(TY_mtype(ST_type(glbl_gridDim_z)), 
					0, glbl_gridDim_z, ST_type(glbl_gridDim_z));
	
	//load parameter: num of element
	WN* wn_num_elem = WN_Ldid(TY_mtype(ST_type(st_num_elem)), 0, st_num_elem, ST_type(st_num_elem));
	WN* InitIsPow2 = WN_Binary(OPR_SUB, TY_mtype(ST_type(st_num_elem)), WN_COPY_Tree(wn_num_elem), 
								WN_Intconst(TY_mtype(ST_type(st_num_elem)), 1));
	
	InitIsPow2 = WN_Binary(OPR_BAND, TY_mtype(ST_type(st_IsPow2)), 
								WN_COPY_Tree(wn_num_elem), InitIsPow2);
	InitIsPow2 = WN_Relational (OPR_EQ, TY_mtype(ST_type(st_IsPow2)), 
						InitIsPow2, WN_Intconst(TY_mtype(ST_type(st_IsPow2)),0));
	InitIsPow2 = WN_Stid(TY_mtype(ST_type(st_IsPow2)), 0, st_IsPow2, 
									ST_type(st_IsPow2), InitIsPow2);	
	WN_INSERT_BlockLast( acc_reduction_func,  InitIsPow2);

	
	WN* Init0 = WN_Stid(TY_mtype(ST_type(st_tid)), 0, st_tid, ST_type(st_tid), WN_COPY_Tree(wn_threadidx));
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	WN* wn_blocksize = WN_Ldid(TY_mtype(ST_type(st_blocksize)), 0, st_blocksize, ST_type(st_blocksize));
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), WN_COPY_Tree(wn_blockidx), WN_COPY_Tree(wn_blocksize));
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockIdx_x)), Init0,  WN_Intconst(MTYPE_U4, 2));
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(glbl_blockIdx_x)), Init0,  WN_COPY_Tree(wn_threadidx));
	Init0 = WN_Stid(TY_mtype(ST_type(st_loop_index)), 0, st_loop_index, ST_type(st_loop_index), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	
	//WN* wn_gridSizes = WN_Ldid(TY_mtype(ST_type(st_blocksize)), 0, st_blocksize, ST_type(st_blocksize));
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), WN_COPY_Tree(wn_griddimx), WN_COPY_Tree(wn_blocksize));
	Init0 = WN_Binary(OPR_MPY, TY_mtype(ST_type(glbl_blockDim_x)), Init0,  WN_Intconst(MTYPE_U4, 2));
	Init0 = WN_Stid(TY_mtype(ST_type(st_gridSize)), 0, st_gridSize, ST_type(st_gridSize), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	Init0 = ACC_Get_Init_Value_of_Reduction(ReductionOpr);
	Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), Init0);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	//Begin calculation
	WN* wn_loadWhileBlock = WN_CreateBlock();

	WN* wn_whileTest = WN_Relational (OPR_LT, TY_mtype(ST_type(st_loop_index)), 
								WN_COPY_Tree(wn_loop_index), 
								WN_COPY_Tree(wn_num_elem));
	//while body
	WN* wn_IfTest1 = WN_Relational (OPR_EQ, TY_mtype(ST_type(st_IsPow2)), 
								WN_COPY_Tree(wn_IsPow2), 
								WN_Intconst(MTYPE_U4, 1));
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_loop_index)), 
						WN_COPY_Tree(wn_loop_index), WN_COPY_Tree(wn_blocksize));
	WN* wn_IfTest2 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_loop_index)), 
						Init0, WN_COPY_Tree(wn_num_elem));
	WN* wn_IfCombinedTest = WN_Binary (OPR_CIOR, Boolean_type, wn_IfTest1, wn_IfTest2);
	//WN_Relational (OPR_CIOR, TY_mtype(ST_type(st_loop_index)), WN_COPY_Tree(wn_loop_index), WN_COPY_Tree(wn_num_elem));
	//WN* tree = WN_Istore(TY_mtype(TY_pointed(ST_type(w->new_st))), 0, ST_type(w->new_st), wn_ptrLoc, WN_kid(tree, 0));
	//WN* tree = WN_Iload(TY_mtype(TY_pointed(ST_type(w->new_st))), 0,  TY_pointed(ST_type(w->new_st)), wn_ptrLoc);
	WN* wn_ifThenBody = WN_CreateBlock();
	//mySum += g_idata[i];
	Init0 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_loop_index), st_input_data);
	Init0 = WN_Iload(TY_mtype(TY_pointed(ST_type(st_input_data))), 0,  
									TY_pointed(ST_type(st_input_data)), Init0);
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ST_type(st_mySum)), WN_COPY_Tree(wn_mySum), Init0);
	Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), Init0);
	WN_INSERT_BlockLast( wn_loadWhileBlock,  Init0);
	//mySum += g_idata[i+blockSize]; 
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_loop_index)), 
					WN_COPY_Tree(wn_loop_index), WN_COPY_Tree(wn_blocksize));
	Init0 = ACC_LoadDeviceSharedArrayElem(Init0, st_input_data);
	Init0 = WN_Iload(TY_mtype(TY_pointed(ST_type(st_input_data))), 0,  
									TY_pointed(ST_type(st_input_data)), Init0);
	Init0 = WN_Binary(ReductionOpr, TY_mtype(ST_type(st_mySum)), WN_COPY_Tree(wn_mySum), Init0);
	Init0 = WN_Stid(TY_mtype(ST_type(st_mySum)), 0, st_mySum, ST_type(st_mySum), Init0);
	WN_INSERT_BlockLast( wn_ifThenBody,  Init0);
	WN* wn_ifThenElse = WN_CreateIf(wn_IfCombinedTest, wn_ifThenBody, WN_CreateBlock());
	WN_INSERT_BlockLast( wn_loadWhileBlock,  wn_ifThenElse);
	//i += gridSize;
	Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(st_loop_index)), 
					WN_COPY_Tree(wn_loop_index), WN_COPY_Tree(wn_gridSize));
	Init0 = WN_Stid(TY_mtype(ST_type(st_loop_index)), 0, st_loop_index, ST_type(st_loop_index), Init0);
	WN_INSERT_BlockLast( wn_loadWhileBlock,  Init0);
    WN* wn_WhileDO = WN_CreateWhileDo(wn_whileTest, wn_loadWhileBlock);
	WN_INSERT_BlockLast( acc_reduction_func,  wn_WhileDO);

	//sdata[tid] = mySum;
	Init0 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_tid), st_shared_array);
	Init0 = WN_Istore(TY_mtype(TY_pointed(ST_type(st_shared_array))), 0, 
					Make_Pointer_Type(ty), Init0, WN_COPY_Tree(wn_mySum));
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	//__syncthreads();
	WN* wn_callSyncThreads = Gen_Sync_Threads();
	WN_INSERT_BlockLast( acc_reduction_func,  WN_COPY_Tree(wn_callSyncThreads));

	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_shared_array, wn_mySum, 512, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_shared_array, wn_mySum, 256, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_shared_array, wn_mySum, 128, ReductionOpr);
	WN_INSERT_BlockLast( acc_reduction_func,  Init0);

	
	wn_IfTest2 = WN_Relational (OPR_LT, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_Intconst(MTYPE_U4, 32));
	WN* wn_IfBody2 = WN_CreateBlock();
	
    localname = (char *) alloca(strlen(ST_name(old_st))+10);
	sprintf ( localname, "__smem_%s", ST_name(old_st));
	Set_TY_is_volatile(ty);
	TY_IDX ty_pointer = Make_Pointer_Type(ty);
	ST* st_smem_pointer = New_ST(CURRENT_SYMTAB); 
	ST_Init(st_smem_pointer,
	  Save_Str(localname),
	  CLASS_VAR,
	  SCLASS_AUTO,
	  EXPORT_LOCAL,
	  ty_pointer);
	
    Init0 = WN_Ldid(Pointer_type, 0, st_shared_array, ST_type(st_shared_array));
	Init0 = WN_Stid(TY_mtype(ST_type(st_smem_pointer)), 0, 
					st_smem_pointer, ST_type(st_smem_pointer), Init0);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_smem_pointer, wn_mySum, 64, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_smem_pointer, wn_mySum, 32, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_smem_pointer, wn_mySum, 16, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_smem_pointer, wn_mySum, 8, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_smem_pointer, wn_mySum, 4, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	Init0 = Gen_ReductionIfElseBlock1(wn_blocksize, wn_tid, st_smem_pointer, wn_mySum, 2, ReductionOpr);
	WN_INSERT_BlockLast( wn_IfBody2,  Init0);
	WN* wn_ifThenElse2 = WN_CreateIf(wn_IfTest2, wn_IfBody2, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse2);

	
	WN* wn_IfTest3 = WN_Relational (OPR_EQ, TY_mtype(ST_type(st_tid)), 
								WN_COPY_Tree(wn_tid), 
								WN_Intconst(MTYPE_U4, 0));
	WN* wn_IfBody3 = WN_CreateBlock();
	
	//Init0 = WN_Binary(OPR_ADD, TY_mtype(ST_type(WN_st(wn_tid))), 
	//				WN_COPY_Tree(wn_tid), WN_Intconst(MTYPE_U4, ilimit/2));
	Init0 = ACC_LoadDeviceSharedArrayElem(WN_Intconst(MTYPE_U4, 0), st_shared_array);	
	Init0 = WN_Iload(TY_mtype(TY_pointed(ST_type(st_shared_array))), 0,  
									TY_pointed(ST_type(st_shared_array)), Init0);
	
	WN* Init1 = ACC_LoadDeviceSharedArrayElem(WN_COPY_Tree(wn_blockidx), st_output_data);	
	Init1 = WN_Istore(TY_mtype(TY_pointed(ST_type(st_output_data))), 0, 
					ST_type(st_output_data), Init1, Init0);
	WN_INSERT_BlockLast( wn_IfBody3,  Init1);
	WN* wn_ifThenElse3 = WN_CreateIf(wn_IfTest3, wn_IfBody3, WN_CreateBlock());
	WN_INSERT_BlockLast( acc_reduction_func,  wn_ifThenElse3);//*/

	
	////////////////////////////////////////////////////////////////
	//restore info
	//ACC_Transfer_Maps ( acc_pmaptab, acc_cmaptab, acc_reduction_func, 
	//	  PU_Info_regions_ptr(Current_PU_Info) );

	/* Create a new dependence graph for the child  and move all the 
	 appropriate vertices from the parent to the child graph */

	//Current_Map_Tab = acc_cmaptab;
	//ACC_Fix_Dependence_Graph ( acc_ppuinfo, Current_PU_Info, acc_reduction_func); 
	//Current_Map_Tab = acc_pmaptab;

  
	/* Restore parent information. */

	CURRENT_SYMTAB = acc_psymtab;
	Current_PU_Info = acc_ppuinfo;
	Current_pu = &Current_PU_Info_pu();
	Current_Map_Tab = acc_pmaptab;
	ACC_Pop_Some_Globals( );

	return acc_reduction_proc;
}
/*
static BB_NODE* Find_region_start_bb(CFG* cfg)
{
	BB_NODE* bb = cfg->First_bb();
	while(bb && bb->Kind() != BB_REGIONSTART)
	{
		bb = bb->Next();
	}
	return bb;
}

void Check_XPragma_stmt_list_wn(CFG* cfg)
{
  BB_NODE *bb = Find_region_start_bb(cfg);
  if(bb == NULL)
  	return;
  //STMT_CONTAINER stmt_cont(bb->Firststmt(), bb->Laststmt());
  STMT_LIST *stmt_list = bb->Stmtlist();
  STMTREP_ITER stmt_iter(stmt_list);
  STMTREP  *tmp;
  FOR_ALL_NODE(tmp, stmt_iter, Init()) {
    OPERATOR crepOperator, wnOpr = tmp->Opr();
    CODEREP* crep;
    if(wnOpr == OPR_XPRAGMA)
    {
    	crep = tmp->Rhs();
    	CODEKIND ckind = crep->Kind();
    	//crepOperator = crep->Opr();
    	int test = 1;
    	test ++;
    }
  }
}
*/
static void ACC_Blank()
{
}
