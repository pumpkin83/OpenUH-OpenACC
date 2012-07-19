/*
  Copyright (C) 2010-2012 University of Houston.  All Rights Reserved.

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

  Contact information:
  http://ww.cs.uh.edu/~hpctools

*/


#include <stdint.h>
#include <sys/types.h>
#if ! defined(BUILD_OS_DARWIN)
#include <elf.h>		    /* for wn.h */
#endif /* ! defined(BUILD_OS_DARWIN) */

#include "defs.h"
#include "wn.h"
#include "erbe.h"

#include "symtab.h"

#include "mtypes.h"
#include "wn_util.h"
#include "wn_tree_util.h"
#include "config_targ.h"
#include "const.h"
#include "cxx_template.h"
#include "cxx_hash.h"
#include "pu_info.h"
#include "coarray_lower.h"
#include "srcpos.h"
#include "tracing.h"
#include "lnopt_main.h"
#include "wn_simp.h"
#include "strtab.h"
#include "region_util.h"
#include "config.h"
#include "prompf.h"
#include "anl_driver.h"
#include "cxx_memory.h"
#include "wb_buffer.h"
#include "wb_carray.h"
#include "wb_browser.h"
#include "wb.h"
#include "targ_const.h"
#include "dra_export.h"
#include "be_symtab.h"
#include "f90_utils.h"

#include <vector>
#include <map>

/***********************************************************************
 * Local constants, types, etc.
 ***********************************************************************/

typedef enum {
    READ_TO_LCB = 0,    /* read remote array, store data to LCB */
    WRITE_FROM_LCB = 1, /* load data from LCB, write remote array */
    READ_DIRECT = 2,    /* read remote array, store data directly to
                           local object */
    WRITE_DIRECT = 3    /* load data directly from local object, write
                           remote array */
} ACCESS_TYPE;


typedef enum  {
    NONE = 0,
    DV_BASE_PTR, DV_BASE_LEN, DV_ASSOC, DV_NUM_DIM, DV_TYPE_LEN,
    DV_ORIG_BASE , DV_ORIG_SIZE , DV_DIM1_LB   , DV_DIM1_EXT  ,
    DV_DIM1_STR  , DV_DIM2_LB   , DV_DIM2_EXT  , DV_DIM2_STR  ,
    DV_DIM3_LB   , DV_DIM3_EXT  , DV_DIM3_STR  , DV_DIM4_LB   ,
    DV_DIM4_EXT  , DV_DIM4_STR  , DV_DIM5_LB   , DV_DIM5_EXT  ,
    DV_DIM5_STR  , DV_DIM6_LB   , DV_DIM6_EXT  , DV_DIM6_STR  ,
    DV_DIM7_LB   , DV_DIM7_EXT  , DV_DIM7_STR  , DV_LAST
} DOPEVEC_FIELDS;

typedef struct {
  INT16 ofst32;
  INT16 type32;
  INT16 ofst64;
  INT16 type64;
  const char  *name;
} DOPEVEC_FIELD_INFO;


typedef struct {
  WN *stmt; /* a CAF statement node */
  WN *blk;  /* a block enclosing stmt */
} CAF_STMT_NODE;


/***********************************************************************
 * Local variable definitions
 ***********************************************************************/

static BOOL caf_prelower_initialized = FALSE;
static MEM_POOL caf_pool;
static WN_MAP Caf_Parent_Map;
static WN_MAP Caf_LCB_Map;
static WN_MAP Caf_Visited_Map;

static std::vector<CAF_STMT_NODE> caf_delete_list;
static std::map<ST *, ST *> save_coarray_symbol_map;
static std::map<ST *, ST *> common_save_coarray_symbol_map;

static ST *this_image_st = NULL;
static ST *num_images_st = NULL;

static DOPEVEC_FIELD_INFO dopevec_fldinfo[DV_LAST+1] = {
     0,         0,      0,         0,     "",
							/* FIOSTRUCT_NONE */
     0, MTYPE_U4,      0, MTYPE_U8,	 "base_addr",
							/* DV_BASE_PTR */
     4, MTYPE_I4,      8, MTYPE_I8,	 "base_len",
							/* DV_BASE_LEN */
#if 0
     8, MTYPE_U8,      16, MTYPE_U8,	 "flag_info",
							/* DV_FLAG_INFO */
#endif
     8, MTYPE_U4,      16, MTYPE_U4,     "assoc",
							/* DV_ASSOC */
     12, MTYPE_U4,     20, MTYPE_U4,     "num_dims",
							/* DV_NUM_DIM */
     16, MTYPE_U8,     24, MTYPE_U8,	 "type_len",
							/* DV_TYPE_LEN */
     24, MTYPE_U4,     32, MTYPE_U8,	 "orig_base",
							/* DV_ORIG_BASE */
     28, MTYPE_I4,     40, MTYPE_I8,	 "orig_size",
							/* DV_ORIG_SIZE */
     32, MTYPE_I4,     48, MTYPE_I8,	 "dim1_lb",
							/* DV_DIM1_LB */
     36, MTYPE_I4,     56, MTYPE_I8,	 "dim1_extent",
							/* DV_DIM1_EXTENT */
     40, MTYPE_I4,     64, MTYPE_I8,	 "dim1_stride",
							/* DV_DIM1_STRIDE */
     44, MTYPE_I4,     72, MTYPE_I8,	 "dim2_lb",
							/* DV_DIM2_LB */
     48, MTYPE_I4,     80, MTYPE_I8,	 "dim2_extent",
							/* DV_DIM2_EXT */
     52, MTYPE_I4,     88, MTYPE_I8,	 "dim2_stride",
							/* DV_DIM2_STR */
     56, MTYPE_I4,     96, MTYPE_I8,	 "dim3_lb",
							/* DV_DIM3_LB */
     60, MTYPE_I4,     104, MTYPE_I8,	 "dim3_extent",
							/* DV_DIM3_EXT */
     64, MTYPE_I4,     112, MTYPE_I8,	 "dim3_stride",
							/* DV_DIM3_STR */
     68, MTYPE_I4,     120, MTYPE_I8,	 "dim4_lb",
							/* DV_DIM4_LB */
     72, MTYPE_I4,     128, MTYPE_I8,	 "dim4_extent",
							/* DV_DIM4_EXT */
     76, MTYPE_I4,     136, MTYPE_I8,	 "dim4_stride",
							/* DV_DIM4_STR */
     80, MTYPE_I4,     144, MTYPE_I8,	 "dim5_lb",
							/* DV_DIM5_LB */
     84, MTYPE_I4,     152, MTYPE_I8,	 "dim5_extent",
							/* DV_DIM5_EXT */
     88, MTYPE_I4,     160, MTYPE_I8,	 "dim5_stride",
							/* DV_DIM5_STR */
     92, MTYPE_I4,     168, MTYPE_I8,	 "dim6_lb",
							/* DV_DIM6_LB */
     96, MTYPE_I4,     176, MTYPE_I8,	 "dim6_extent",
							/* DV_DIM6_EXT */
     100, MTYPE_I4,    184, MTYPE_I8,	 "dim6_stride",
							/* DV_DIM6_STR */
     104, MTYPE_I4,    192, MTYPE_I8,	 "dim7_lb",
							/* DV_DIM7_LB */
     108, MTYPE_I4,    200, MTYPE_I8,	 "dim7_extent",
							/* DV_DIM7_EXT */
     112, MTYPE_I4,    208, MTYPE_I8,	 "dim7_stride",
							/* DV_DIM7_STR */
};

/***********************************************************************
 * Local macros
 ***********************************************************************/

#define Set_LCB_Stmt(wn) (WN_MAP_Set(Caf_LCB_Map, wn, (void*)  1))
#define Is_LCB_Stmt(wn) ((WN*) WN_MAP_Get(Caf_LCB_Map, (WN*) wn))

#define Set_Parent(wn, p) (WN_MAP_Set(Caf_Parent_Map, wn, (void*)  p))
#define Get_Parent(wn) ((WN*) WN_MAP_Get(Caf_Parent_Map, (WN*) wn))

#define Set_Visited(wn) (WN_MAP_Set(Caf_Visited_Map, wn, (void*)  1))
#define Was_Visited(wn) ((WN*) WN_MAP_Get(Caf_Visited_Map, (WN*) wn))

#define NAME_IS( st, name ) \
        strlen( &Str_Table[(st)->u1.name_idx]) == strlen(name) \
        && !strncmp( &Str_Table[(st)->u1.name_idx], name, strlen(name))



/***********************************************************************
 * Local function declarations
 ***********************************************************************/

static BOOL is_load_operation(WN *node);
static BOOL is_convert_operation(WN *node);
static void gen_save_coarray_symbol(ST *sym);
static void gen_global_save_coarray_symbol(ST *sym);
static WN* gen_array1_ref( OPCODE op_array, TY_IDX array_type,
                               ST *array_st, INT8 index, INT8 dim);
static ST *get_lcb_sym(WN *access);
static WN *get_lcb_assignment(WN *coarray_deref, TY_IDX coarray_type,
                              ST *LCB_st);
static BOOL array_ref_on_LHS(WN *wn, TY_IDX *ty);
static BOOL array_ref_on_RHS(WN *wn, TY_IDX *ty);
static BOOL array_ref_in_parm(WN *wn);
static BOOL is_lvalue(WN *expr);
static WN* get_transfer_size(WN *arr_ref, TY_IDX elem_type);
static void replace_RHS_with_LCB( WN *stmt_node, ST *LCB_st);
static WN* get_enclosing_direct_arr_ref(WN *arr);
static TY_IDX get_assign_stmt_datatype(WN *stmt);
static WN* get_load_parent(WN *start);
static WN* get_store_parent(WN *start);
static TY_IDX get_array_type_from_tree(WN *array_ref);

static int subscript_is_strided(WN *array, INT8 i);
static WN* subscript_extent(WN *array, INT8 i);
static WN* subscript_stride(WN *array, INT8 i);
static int subscript_is_scalar(WN *array, INT8 i);
static inline WN* WN_array_subscript(WN *array, INT8 i);

static WN *find_outer_array(WN *start, WN *end);
static TY_IDX get_type_at_offset (TY_IDX ty, WN_OFFSET offset,
                                  BOOL stop_at_array = FALSE,
                                  BOOL stop_at_coarray = FALSE);
static void Coarray_Prelower_Init();
static void Parentize(WN *wn);
static void init_caf_extern_symbols();
static void handle_caf_call_stmts(WN_TREE_CONTAINER<PRE_ORDER>::iterator wipre,
                                  WN **wn_next_p);
static WN *expr_is_coindexed(WN *expr, WN **image, TY_IDX *coarray_type,
                             WN **direct_coarray_ref = NULL);
static WN *get_containing_arrayexp(WN *wn);
static BOOL is_dope(const TY_IDX tyi);
static BOOL currentpu_ismain();
static BOOL is_coarray_type(const TY_IDX tyi);
static void set_coarray_tsize(TY_IDX coarray_type);
static INT get_1darray_size(const TY_IDX tyi);
static INT get_coarray_rank(const TY_IDX tyi);
static INT get_array_rank(const TY_IDX tyi);
static INT get_coarray_corank(const TY_IDX tyi);
static TY_IDX get_array_type(const ST * array_st);
static BOOL is_contiguous_access(WN *remote_access);
static BOOL is_vector_access(WN *remote_access);
static TY_IDX create_arr1_type(TYPE_ID elem_type, INT16 ne);

static ST* gen_lcbptr_symbol(TY_IDX tyi, const char *rootname);

static WN* gen_coarray_access_stmt(WN *remote_access, WN *local_access,
                                   ST *lcbtemp, WN *xfer_size,
                                   ACCESS_TYPE access);
static void substitute_lcb(WN *remote_access, ST *lcbtemp,
                           WN *wn_arrayexp,  WN **replace_wn);

static BOOL stmt_rhs_is_addressable(WN *stmt_node);
static WN * array_ref_is_coindexed(WN *arr, TY_IDX ty);

static int add_caf_stmt_to_delete_list(WN *stmt, WN *blk);
static void delete_caf_stmts_in_delete_list();

static WN * Generate_Call_acquire_lcb(WN *, WN *);
static WN * Generate_Call_release_lcb(WN *);
static WN * Generate_Call_coarray_read(WN *image, WN *src, WN *dest,
                                       WN *nbytes);
static WN * Generate_Call_coarray_write(WN *image, WN *dest, WN *src,
                                        WN *nbytes);
static WN * Generate_Call_coarray_strided_read(WN *image, WN *src,
                                    WN *src_strides, WN *dest,
                                    WN *dest_strides, WN *count,
                                    WN *stride_levels);
static WN * Generate_Call_coarray_strided_write(WN *image, WN *dest,
                                    WN *dest_strides, WN *src,
                                    WN *src_strides, WN *count,
                                    WN *stride_levels);


// ====================================================================
//
// Utility functions to generate function calls
//
// TODO: move these to their own file in common/com
//
// ====================================================================

static void
My_Get_Return_Pregs(PREG_NUM *rreg1, PREG_NUM *rreg2, mTYPE_ID type,
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
} // My_Get_Return_Pregs()

#define MYGET_RETURN_PREGS(rreg1, rreg2, type) \
  My_Get_Return_Pregs(&rreg1, &rreg2, type, __FILE__, __LINE__)

ST *
Create_VarSym( char *symName, TYPE_ID vtype = MTYPE_I4,
                    SYMTAB_IDX symTab = CURRENT_SYMTAB )
{
  Is_True( symName != NULL, ("Create_VarSym: symName (arg1) is empty") );
  ST *newSym = New_ST ( symTab );
  ST_Init(newSym, Save_Str ( symName ), CLASS_VAR, SCLASS_AUTO,
          EXPORT_LOCAL, MTYPE_To_TY(vtype));

  return newSym;
}

WN *
Generate_Call_Shell( const char *name, TYPE_ID rtype, INT32 argc )
{
  TY_IDX  ty = Make_Function_Type( MTYPE_To_TY( rtype ) );
  ST     *st = Gen_Intrinsic_Function( ty, name );

  Clear_PU_no_side_effects( Pu_Table[ST_pu( st )] );
  Clear_PU_is_pure( Pu_Table[ST_pu( st )] );
  Set_PU_no_delete( Pu_Table[ST_pu( st )] );

  WN *wn_call = WN_Call( rtype, MTYPE_V, argc, st );

  WN_Set_Call_Default_Flags(  wn_call );
  // WN_Reset_Call_Non_Parm_Mod( wn_call );
  // WN_Reset_Call_Non_Parm_Ref( wn_call );

  return wn_call;
}


WN *
Generate_Call( const char *name, TYPE_ID rtype = MTYPE_V )
{
  WN *call = Generate_Call_Shell( name, rtype, 0 );
  return call;
}


inline WN *
Generate_Param( WN *arg, UINT32 flag )
{
  return WN_CreateParm( WN_rtype( arg ), arg,
			MTYPE_To_TY( WN_rtype( arg ) ), flag );
}


// ====================================================================
//
// Routines for carrying out coarray lowering.
//
// ====================================================================

/*
 * Coarray_Prelower
 *
 * The Coarray_Prelower phase is a preprocessing pass which handles certain CAF
 * instrincs and also standardizes statements containing remote coarray
 * accesses such that both the left and right hand sides contain l-values.
 *
 * For example, a statement such as this:
 *      y(:) = a(:)[i] + a(:)[j]
 * becomes:
 *      lcb1(:) = a(:)[i]
 *      lcb2(:) = a(:)[j]
 *      y(:) = lcb1(:) + lcb2(:)
 *
 * As another example:
 *      a(:)[i] = y(:) + z(:)
 * becomes:
 *      lcb3(:) = y(:) + z(:)
 *      a(:)[i] = lcb3(:)
 *
 * The prelowering phase will generate temporary buffers, referred to as Local
 * Communication Buffers (LCBs) from which data may be transmitted or to which
 * data may be received. If a statement with a remote coarray access already
 * contains l-values on both the left and right side, then no LCB will be
 * created in order to avoid an unnecessary extra copy.
 *
 * A subsequent back-end phase should handle lowering co-subscripted array
 * references into 1-sided GET/PUT calls. However, for the time being we will
 * handle this translation right here.
 */

WN * Coarray_Prelower(PU_Info *current_pu, WN *pu)
{
    BOOL is_main;
    WN *func_body;
    BOOL global_coarrays_processed = FALSE;

    if (!caf_prelower_initialized) {
      Coarray_Prelower_Init();
      caf_prelower_initialized = TRUE;
    }

    /* insert call to caf_init if this is the main PU */
    is_main  = currentpu_ismain();
    func_body = WN_func_body( pu );
    if ( is_main ) {
        WN *first_wn = WN_first(func_body);
        WN *call_caf_init = Generate_Call( CAF_INIT );
        WN_INSERT_BlockFirst( func_body, call_caf_init );
    }

    /* create extern symbols _this_image and _num_images. Should be
     * initialized by runtime library
     */
    init_caf_extern_symbols();

    /* Create Parent Map for WHIRL tree */
    Caf_Parent_Map = WN_MAP_Create(&caf_pool);
    Caf_LCB_Map = WN_MAP_Create(&caf_pool);
    Caf_Visited_Map = WN_MAP_Create(&caf_pool);

    Parentize(func_body);

    /* resize coarrays.
     * TODO: This should be fixed in front-end, actually.
     *       not tested on deferred-size / allocatables.
     * */
    ST *sym;
    UINT32 i;
    FOREACH_SYMBOL(CURRENT_SYMTAB, sym, i) {
        if (is_coarray_type(ST_type(sym))) {
            set_coarray_tsize(ST_type(sym));
            if (ST_sclass(sym) == SCLASS_PSTATIC) {
                gen_save_coarray_symbol(sym);
                /* don't allot space for this symbol in global memory */
                Set_TY_size(ST_type(sym), 0);
            }
        }
    }

    if (global_coarrays_processed == FALSE) {
        FOREACH_SYMBOL(GLOBAL_SYMTAB, sym, i) {
            if (is_coarray_type(ST_type(sym))) {
                set_coarray_tsize(ST_type(sym));
                if (ST_sclass(sym) == SCLASS_COMMON ||
                    ST_sclass(sym) == SCLASS_DGLOBAL) {
                    gen_global_save_coarray_symbol(sym);
                    /* don't allot space for this symbol in global memory */
                    Set_TY_size(ST_type(sym), 0);
                }
            }
        }
        global_coarrays_processed = TRUE;
    }

    /* for support for character coarrays
     **/
    WN *lhs_ref_param_wn = NULL;
    WN *rhs_ref_param_wn = NULL;

    /* Pass 1: Traverse PU, searching for:
     *   (1) CAF intrinsics (this_image, num_images, sync_images)
     *   (2) calls to STOP or _END, before which insert caf_finalize() call
     *   (3) co-subscripted array section references: "standardize" and, for now, go
     *       ahead and generate 1-sided GET/PUT calls
     */
    WN_TREE_CONTAINER<PRE_ORDER> wcpre(func_body);
    WN_TREE_CONTAINER<PRE_ORDER> ::iterator wipre, curr_wipre=NULL, temp_wipre = NULL;
    for (wipre = wcpre.begin(); wipre != wcpre.end(); ++wipre) {
        WN *insert_blk;
        WN *wn = wipre.Wn();
        WN *wn_next;
        WN *insert_wnx;
        WN *blk_node;
        WN *stmt_node;
        WN *parent;
        ST *func_st;
        INT8 rank, corank;
        ST *array_st;
        WN *wn_arrayexp;

        ST *lcbptr_st;
        WN *transfer_size_wn;
        ST *st1;
        TY_IDX ty1, ty2, ty3;
        TY_IDX coarray_type;
        TY_IDX elem_type;
        WN *replace_wn = NULL;
        WN *image;
        WN *coindexed_arr_ref;
        WN *direct_coarray_ref;

        parent = wipre.Get_parent_wn();

        /* if its a statement, set stmt_node and also set blk_node to
         * parent if the parent is a block */
        if ((OPCODE_is_stmt(WN_opcode(wn)) ||
            OPCODE_is_scf(WN_opcode(wn)) &&
             WN_operator(wn) != OPR_BLOCK &&
             WN_operator(wn) != OPR_FUNC_ENTRY)) {
          stmt_node = wn;
          wn_arrayexp = NULL;
          if (WN_operator(parent) == OPR_BLOCK) blk_node = parent;
        }

        /* stores most recently encountered ARRAYEXP in wn_arrayexp */
        if (WN_operator(wn) == OPR_ARRAYEXP)
          wn_arrayexp = wn;

        switch (WN_operator(wn)) {
            case OPR_CALL:
                handle_caf_call_stmts(wipre, &wn_next);

                /* check for call to _END */
                if ( NAME_IS(WN_st(wn), "_END") ) {
                    insert_wnx = Generate_Call( CAF_FINALIZE );
                    WN_INSERT_BlockBefore(blk_node, wn, insert_wnx);
                }
                break;
            case OPR_INTRINSIC_CALL:
                if (WN_opcode(wn) == OPC_VINTRINSIC_CALL) {
                  if (WN_intrinsic(wn) == INTRN_STOP_F90) {
                    insert_wnx = Generate_Call( CAF_FINALIZE );
                    WN_INSERT_BlockBefore(blk_node, wn, insert_wnx);

                  } else if (WN_intrinsic(wn) == INTRN_CASSIGNSTMT) {
                    /*for character coarrays support*/
                    lhs_ref_param_wn = WN_kid0(wn);
                    rhs_ref_param_wn = WN_kid1(wn);
                  }
                }

                break;

            /*case OPR_ARRAY:*/
            case OPR_ARRSECTION:

                coindexed_arr_ref = expr_is_coindexed(wn, &image, &coarray_type,
                                                      &direct_coarray_ref);
                if (image == NULL) break;
                if (is_vector_access(coindexed_arr_ref)) break;
                if (Was_Visited(coindexed_arr_ref)) break;

                temp_wipre = wipre;
                while ( temp_wipre.Wn() != coindexed_arr_ref )
                    temp_wipre++;

                Set_Visited(coindexed_arr_ref);

                if ( array_ref_on_LHS(coindexed_arr_ref, &elem_type) ) {
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    WN *RHS_wn = WN_kid0(stmt_node);
                    if ( !is_lvalue(RHS_wn)
                         || is_convert_operation(RHS_wn)
                         /* for now, use LCB for LDID, ILOAD, MLOAD as well */
                         || is_load_operation(RHS_wn)
                         || is_vector_access(RHS_wn)) {
                        WN *new_stmt_node;
                        ST *LCB_st;
                        WN *xfer_sz_node;

                        /* create LCB for RHS */
                        LCB_st = gen_lcbptr_symbol(
                               Make_Pointer_Type(elem_type,FALSE),
                               "LCB" );
                        xfer_sz_node = get_transfer_size(coindexed_arr_ref,
                                                         elem_type);
                        insert_wnx = Generate_Call_acquire_lcb(
                                xfer_sz_node,
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);

                        /* create "normalized" assignment to remote coarray */
                        new_stmt_node = WN_COPY_Tree(stmt_node);
                        replace_RHS_with_LCB(new_stmt_node,
                                             LCB_st);
                        Set_LCB_Stmt(new_stmt_node);
                        WN_INSERT_BlockAfter(blk_node, stmt_node, new_stmt_node);

                        /* call to release LCB */
                        insert_wnx = Generate_Call_release_lcb(
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockAfter(blk_node, new_stmt_node, insert_wnx);

                        /* replace LHS */
                        WN *store = get_store_parent(coindexed_arr_ref);
                        if (store) WN_offset(store) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st, wn_arrayexp,
                                       &replace_wn);
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;

                    }
                    /* handle indirection ... */
                } else if ( array_ref_on_RHS(coindexed_arr_ref, &elem_type) ) {
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    WN *RHS_wn = WN_kid0(stmt_node);
                    WN *LHS_img;
                    TY_IDX LHS_coarray_type;
                    BOOL LHS_is_coindexed  = FALSE;

                    if (WN_operator(stmt_node) == OPR_ISTORE ||
                        WN_operator(stmt_node) == OPR_MSTORE) {
                        WN *LHS_wn = WN_kid1(stmt_node);
                        if (expr_is_coindexed(LHS_wn, &LHS_img,
                            &LHS_coarray_type)) {
                            LHS_is_coindexed = TRUE;
                        }
                    }

                    if ( !is_lvalue(RHS_wn) || LHS_is_coindexed
                         || is_convert_operation(RHS_wn)
                         /* for now, use LCB for LDID, ILOAD, MLOAD as well */
                         || is_load_operation(RHS_wn)
                         || is_vector_access(WN_kid1(stmt_node)) ) {
                        WN *new_stmt_node;
                        ST *LCB_st;
                        WN *xfer_sz_node;

                        /* create LCB for coindexed array ref */
                        LCB_st = gen_lcbptr_symbol(
                               Make_Pointer_Type(elem_type,FALSE),
                               "LCB" );
                        xfer_sz_node = get_transfer_size(coindexed_arr_ref,
                                                         elem_type);
                        insert_wnx = Generate_Call_acquire_lcb(
                                xfer_sz_node,
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);

                        /* create "normalized" assignment from remote coarray */
                        new_stmt_node = get_lcb_assignment(
                                WN_COPY_Tree(Get_Parent(wn)),
                                coarray_type,
                                LCB_st);

                        WN_INSERT_BlockBefore(blk_node, stmt_node, new_stmt_node);

                        /* replace term in RHS */
                        WN *load = get_load_parent(coindexed_arr_ref);
                        if (load) WN_offset(load) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st, wn_arrayexp,
                                       &replace_wn);
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;

                        /* call to release LCB */
                        insert_wnx = Generate_Call_release_lcb(
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockAfter(blk_node, stmt_node, insert_wnx);

                    } else {
                    /* handle indirection ... */
                    }
                } else if ( array_ref_in_parm(coindexed_arr_ref) ) {
                    WN *new_stmt_node;
                    ST *LCB_st;
                    WN *xfer_sz_node;

                    elem_type = Ty_Table[coarray_type].u2.etype;

                    /* create LCB for coindexed array ref */
                    LCB_st = gen_lcbptr_symbol(
                            Make_Pointer_Type(elem_type,FALSE),
                            "LCB" );
                    xfer_sz_node = get_transfer_size(coindexed_arr_ref,
                            elem_type);
                    insert_wnx = Generate_Call_acquire_lcb(
                            xfer_sz_node,
                            WN_Lda(Pointer_type, 0, LCB_st));
                    WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);

                    if (Get_Parent(coindexed_arr_ref) == rhs_ref_param_wn) {
                        /*coarray read*/
                        insert_blk = gen_coarray_access_stmt( wn,
                                NULL, LCB_st, xfer_sz_node, READ_TO_LCB);
                        /* get LDID node in replace_wn that substitues wn
                         * pointed to by wipre*/
                        WN *load = get_load_parent(coindexed_arr_ref);
                        if (load) WN_offset(load) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st,
                                       wn_arrayexp, &replace_wn);
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;
                        insert_wnx = WN_first(insert_blk);
                        while (insert_wnx) {
                            insert_wnx = WN_EXTRACT_FromBlock(insert_blk,
                                    insert_wnx);
                            WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);
                            insert_wnx = WN_first(insert_blk);
                        }
                        WN_Delete(insert_blk);
                        insert_blk = NULL;
                        rhs_ref_param_wn = NULL;
                    } else if (Get_Parent(coindexed_arr_ref) == lhs_ref_param_wn) {
                        /*corray write*/

                        /* get LDID node in replace_wn that substitues wn
                         * pointed to by wipre*/
                        WN *store = get_store_parent(coindexed_arr_ref);
                        if (store) WN_offset(store) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st, wn_arrayexp,
                                       &replace_wn);

                        insert_blk = gen_coarray_access_stmt( wn, NULL,
                                LCB_st, xfer_sz_node, WRITE_FROM_LCB);
                        insert_wnx = WN_last(insert_blk);
                        while (insert_wnx) {
                            insert_wnx = WN_EXTRACT_FromBlock(insert_blk, insert_wnx);
                            WN_INSERT_BlockAfter(blk_node, stmt_node, insert_wnx);
                            insert_wnx = WN_last(insert_blk);
                        }
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;
                        WN_Delete(insert_blk);
                        insert_blk = NULL;
                        lhs_ref_param_wn = NULL;
                    }
                }

                break;
        }
    }

    /* Pass 2: Replace Save Coarray Symbols
     * TODO: Try to integrate this into the previous pass if possible
     */
    if (!save_coarray_symbol_map.empty() ||
        !common_save_coarray_symbol_map.empty() ) {
        WN_TREE_CONTAINER<POST_ORDER> wcpost(func_body);
        WN_TREE_CONTAINER<POST_ORDER> ::iterator wipost;
        for (wipost = wcpost.begin(); wipost != wcpost.end(); ++wipost) {
            WN *wn = wipost.Wn();
            ST *st1;
            switch (WN_operator(wn)) {
              case OPR_LDA:
                  st1 = WN_st(wn);
                  if (st1 && is_coarray_type(ST_type(st1))) {
                      ST *save_coarray_replace;
                      if (ST_sclass(st1) == SCLASS_COMMON ||
                          ST_sclass(st1) == SCLASS_DGLOBAL) {
                          save_coarray_replace =
                              common_save_coarray_symbol_map[st1];
                      } else if (ST_sclass(st1) == SCLASS_PSTATIC) {
                          save_coarray_replace =
                              save_coarray_symbol_map[st1];
                      } else {
                          continue;
                      }
                      Is_True(save_coarray_replace,
                        ("New symbol for save coarray was not created yet"));

                      wipost.Replace(
                              WN_Ldid(TY_mtype(ST_type(save_coarray_replace)),
                                  0, save_coarray_replace,
                                  ST_type(save_coarray_replace))
                              );
                      WN_Delete(wn);
                  }
                  break;

              case OPR_LDID:
                  st1 = WN_st(wn);
                  if (st1 && is_coarray_type(ST_type(st1))) {
                      ST *save_coarray_replace;
                      if (ST_sclass(st1) == SCLASS_COMMON ||
                          ST_sclass(st1) == SCLASS_DGLOBAL) {
                          save_coarray_replace =
                              common_save_coarray_symbol_map[st1];
                      } else if (ST_sclass(st1) == SCLASS_PSTATIC) {
                          save_coarray_replace =
                              save_coarray_symbol_map[st1];
                      } else {
                          continue;
                      }
                      Is_True(save_coarray_replace,
                        ("New symbol for save coarray was not created yet"));
                      wipost.Replace(
                              WN_IloadLdid(WN_desc(wn),
                                  0, ST_type(save_coarray_replace),
                                  save_coarray_replace, 0)
                              );
                      WN_Delete(wn);
                  }
                  break;

              case OPR_STID:
                  st1 = WN_st(wn);
                  if (st1 && is_coarray_type(ST_type(st1))) {
                      ST *save_coarray_replace;
                      if (ST_sclass(st1) == SCLASS_COMMON ||
                          ST_sclass(st1) == SCLASS_DGLOBAL) {
                          save_coarray_replace =
                              common_save_coarray_symbol_map[st1];
                      } else if (ST_sclass(st1) == SCLASS_PSTATIC) {
                          save_coarray_replace =
                              save_coarray_symbol_map[st1];
                      } else {
                          continue;
                      }
                      Is_True(save_coarray_replace,
                        ("New symbol for save coarray was not created yet"));
                      wipost.Replace(
                              WN_Istore(WN_rtype(WN_kid0(wn)),
                                  0, ST_type(save_coarray_replace),
                                  WN_Ldid(TY_mtype(ST_type(save_coarray_replace)),
                                      0, save_coarray_replace,
                                      ST_type(save_coarray_replace)),
                                  WN_COPY_Tree(WN_kid0(wn)), 0)
                              );
                      WN_Delete(wn);
                  }
                  break;

              case OPR_MLOAD:
                  st1 = WN_st(wn);
                  if (st1 && is_coarray_type(ST_type(st1))) {
                      ST *save_coarray_replace;
                      if (ST_sclass(st1) == SCLASS_COMMON ||
                          ST_sclass(st1) == SCLASS_DGLOBAL) {
                          save_coarray_replace =
                              common_save_coarray_symbol_map[st1];
                      } else if (ST_sclass(st1) == SCLASS_PSTATIC) {
                          save_coarray_replace =
                              save_coarray_symbol_map[st1];
                      } else {
                          continue;
                      }
                      Is_True(0,
                        ("MLOAD operation on save coarray not supported"));
                  }
                  break;

              case OPR_MSTORE:
                  st1 = WN_st(wn);
                  if (st1 && is_coarray_type(ST_type(st1))) {
                      ST *save_coarray_replace;
                      if (ST_sclass(st1) == SCLASS_COMMON ||
                          ST_sclass(st1) == SCLASS_DGLOBAL) {
                          save_coarray_replace =
                              common_save_coarray_symbol_map[st1];
                      } else if (ST_sclass(st1) == SCLASS_PSTATIC) {
                          save_coarray_replace =
                              save_coarray_symbol_map[st1];
                      } else {
                          continue;
                      }
                      Is_True(0,
                        ("MSTORE operation on save coarray not supported"));
                  }
                  break;

              default:
                  st1 = WN_has_sym(wn) ? WN_st(wn) :  NULL;
                  if (st1 && is_coarray_type(ST_type(st1))) {
                      ST *save_coarray_replace;
                      if (ST_sclass(st1) == SCLASS_COMMON ||
                          ST_sclass(st1) == SCLASS_DGLOBAL) {
                          save_coarray_replace =
                              common_save_coarray_symbol_map[st1];
                      } else if (ST_sclass(st1) == SCLASS_PSTATIC) {
                          save_coarray_replace =
                              save_coarray_symbol_map[st1];
                      } else {
                          continue;
                      }
                      Fail_FmtAssertion
                        ("Encountered unexpected save coarray symbol in whirl tree.");
                  }
            }
        }
    }

    /* Pass 3: Generate Communication for coindexed array section accesses
     * TODO: Move this to later back-end phase?
     */
    curr_wipre = NULL;
    temp_wipre = NULL;
    /* reconstruct parent map */
    Parentize(func_body);
    for (wipre = wcpre.begin(); wipre != wcpre.end(); ++wipre) {
        WN *insert_blk = NULL;
        WN *wn = wipre.Wn();
        WN *insert_wnx;
        WN *blk_node;
        WN *stmt_node;
        WN *parent;
        WN *wn_arrayexp;

        TY_IDX coarray_type;
        TY_IDX elem_type;
        WN *replace_wn = NULL;
        WN *image;
        WN *coindexed_arr_ref;
        WN *direct_coarray_ref;

        parent = wipre.Get_parent_wn();

        /* if its a statement, set stmt_node and also set blk_node to
         * parent if the parent is a block */
        if ((OPCODE_is_stmt(WN_opcode(wn)) ||
            OPCODE_is_scf(WN_opcode(wn)) &&
             WN_operator(wn) != OPR_BLOCK &&
             WN_operator(wn) != OPR_FUNC_ENTRY)) {
          stmt_node = wn;
          wn_arrayexp = NULL;
          if (WN_operator(parent) == OPR_BLOCK) blk_node = parent;
        }

        /* stores most recently encountered ARRAYEXP in wn_arrayexp */
        if (WN_operator(wn) == OPR_ARRAYEXP)
          wn_arrayexp = wn;

        switch (WN_operator(wn)) {
            /*case OPR_ARRAY:*/
            case OPR_ARRSECTION:
                coindexed_arr_ref = expr_is_coindexed(wn, &image, &coarray_type,
                                                      &direct_coarray_ref);
                if (image == NULL) break;

                if (is_vector_access(coindexed_arr_ref)) break;

                if ( array_ref_on_LHS(coindexed_arr_ref, &elem_type) ) {
                    WN *RHS_wn = WN_kid0(stmt_node);
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    Is_True(is_lvalue(RHS_wn),
                            ("Unexpected coarray ref found in RHS"));

                    if (Is_LCB_Stmt(stmt_node)) {
                        ST *LCB_st;
                        WN *xfer_sz_node;

                        LCB_st = get_lcb_sym(RHS_wn);
                        xfer_sz_node = get_transfer_size(RHS_wn, elem_type);
                        insert_blk = gen_coarray_access_stmt( wn,
                                            NULL, LCB_st,
                                            xfer_sz_node, WRITE_FROM_LCB);
                    } else {
                        WN *xfer_sz_node;
                        WN *local_access;

                        if (is_vector_access(RHS_wn)) break;

                        if (WN_operator(RHS_wn) == OPR_ARRAYEXP &&
                            WN_operator(WN_kid0(RHS_wn)) == OPR_ILOAD) {
                          local_access = WN_kid0(WN_kid0(RHS_wn));
                          Is_True(WN_operator(local_access) == OPR_ARRSECTION,
                              ("expecting the operator for local_access "
                               "to be ARRSECTION"));
                        } else {
                          /* should not reach */
                          Is_True(0, ("bad WHIRL node encountered in RHS"));
                        }

                        WN *LHS_wn = WN_kid1(stmt_node);
                        xfer_sz_node = get_transfer_size(RHS_wn, elem_type);

                        insert_blk = gen_coarray_access_stmt( wn,
                                            local_access, NULL,
                                            xfer_sz_node, WRITE_DIRECT);
                    }
                } else  if ( array_ref_on_RHS(coindexed_arr_ref, &elem_type) ) {
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    WN *RHS_wn = WN_kid0(stmt_node);
                    Is_True(is_lvalue(RHS_wn),
                            ("Unexpected coarray ref found in RHS"));

                    if (Is_LCB_Stmt(stmt_node)) {
                        ST *LCB_st;
                        WN *LHS_wn = WN_kid1(stmt_node);
                        WN *xfer_sz_node;

                        LCB_st = get_lcb_sym(LHS_wn);
                        xfer_sz_node = get_transfer_size(wn, elem_type);
                        insert_blk = gen_coarray_access_stmt( wn,
                                            NULL, LCB_st,
                                            xfer_sz_node, READ_TO_LCB);

                    } else {
                        WN *LHS_wn = WN_kid1(stmt_node);
                        WN *xfer_sz_node;

                        if (is_vector_access(LHS_wn)) break;

                        xfer_sz_node = get_transfer_size(wn, elem_type);
                        insert_blk = gen_coarray_access_stmt( wn,
                                            WN_kid0(LHS_wn), NULL,
                                            xfer_sz_node, READ_DIRECT);
                    }
                }

                /* replace with runtime call for remote coarray access */
                if (insert_blk) {
                    insert_wnx = WN_first(insert_blk);
                    while (insert_wnx) {
                        insert_wnx = WN_EXTRACT_FromBlock(insert_blk, insert_wnx);
                        WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);
                        insert_wnx = WN_first(insert_blk);
                    }
                    /* defer deletion of the old statment so that we can continue
                     * to traverse the tree */
                    add_caf_stmt_to_delete_list(stmt_node, blk_node);
                    WN_Delete(insert_blk);
                    insert_blk = NULL;
                }
        }
    }



    /* remove statements in caf_delete_list and clear the list */
    delete_caf_stmts_in_delete_list();

    save_coarray_symbol_map.clear();

    WN_MAP_Delete(Caf_Parent_Map);
    WN_MAP_Delete(Caf_LCB_Map);
    WN_MAP_Delete(Caf_Visited_Map);

    return pu;
} /* Coarray_Prelower */

/*
 * Lowering phase which must be called after VHO lowering
 */
WN * Coarray_Lower(PU_Info *current_pu, WN *pu)
{
    WN *func_body;

    /* Create Parent Map for WHIRL tree */
    Caf_Parent_Map = WN_MAP_Create(&caf_pool);
    Caf_LCB_Map = WN_MAP_Create(&caf_pool);
    Caf_Visited_Map = WN_MAP_Create(&caf_pool);

    func_body = WN_func_body( pu );


    /* for support for character coarrays
     **/
    WN *lhs_ref_param_wn = NULL;
    WN *rhs_ref_param_wn = NULL;


    /*
     *  Find co-subscripted array references, and generate LCBs.
     */
    WN_TREE_CONTAINER<PRE_ORDER> wcpre(func_body);
    WN_TREE_CONTAINER<PRE_ORDER> ::iterator wipre, curr_wipre=NULL, temp_wipre = NULL;
    Parentize(func_body);
    for (wipre = wcpre.begin(); wipre != wcpre.end(); ++wipre) {
        WN *insert_blk;
        WN *wn = wipre.Wn();
        WN *wn_next;
        WN *insert_wnx;
        WN *blk_node;
        WN *stmt_node;
        WN *parent;
        ST *func_st;
        INT8 rank, corank;
        ST *array_st;
        WN *wn_arrayexp;

        ST *lcbptr_st;
        WN *transfer_size_wn;
        ST *st1;
        TY_IDX ty1, ty2, ty3;
        TY_IDX coarray_type;
        TY_IDX elem_type;
        WN *replace_wn = NULL;
        WN *image;
        WN *coindexed_arr_ref;
        WN *direct_coarray_ref;

        parent = wipre.Get_parent_wn();

        /* if its a statement, set stmt_node and also set blk_node to
         * parent if the parent is a block */
        if ((OPCODE_is_stmt(WN_opcode(wn)) ||
            OPCODE_is_scf(WN_opcode(wn)) &&
             WN_operator(wn) != OPR_BLOCK &&
             WN_operator(wn) != OPR_FUNC_ENTRY)) {
          stmt_node = wn;
          wn_arrayexp = NULL;
          if (WN_operator(parent) == OPR_BLOCK) blk_node = parent;
        }

        /* stores most recently encountered ARRAYEXP in wn_arrayexp */
        if (WN_operator(wn) == OPR_ARRAYEXP)
          wn_arrayexp = wn;

        switch (WN_operator(wn)) {
            case OPR_ARRAY:

                coindexed_arr_ref = expr_is_coindexed(wn, &image, &coarray_type,
                                                      &direct_coarray_ref);
                if (image == NULL) break;
                if (is_vector_access(coindexed_arr_ref)) break;
                if (Was_Visited(coindexed_arr_ref)) break;

                temp_wipre = wipre;
                while ( temp_wipre.Wn() != coindexed_arr_ref )
                    temp_wipre++;

                Set_Visited(coindexed_arr_ref);

                if ( array_ref_on_LHS(coindexed_arr_ref, &elem_type) ) {
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    WN *RHS_wn = WN_kid0(stmt_node);
                    if ( !is_lvalue(RHS_wn)
                         || is_convert_operation(RHS_wn)
                         /* for now, use LCB for LDID, ILOAD, MLOAD as well */
                         || is_load_operation(RHS_wn)) {
                        WN *new_stmt_node;
                        ST *LCB_st;
                        WN *xfer_sz_node;

                        /* create LCB for RHS */
                        LCB_st = gen_lcbptr_symbol(
                               Make_Pointer_Type(elem_type,FALSE),
                               "LCB" );
                        xfer_sz_node = get_transfer_size(coindexed_arr_ref,
                                                         elem_type);
                        insert_wnx = Generate_Call_acquire_lcb(
                                xfer_sz_node,
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);

                        /* create "normalized" assignment to remote coarray */
                        new_stmt_node = WN_COPY_Tree(stmt_node);
                        replace_RHS_with_LCB(new_stmt_node,
                                             LCB_st);
                        Set_LCB_Stmt(new_stmt_node);
                        WN_INSERT_BlockAfter(blk_node, stmt_node, new_stmt_node);

                        /* call to release LCB */
                        insert_wnx = Generate_Call_release_lcb(
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockAfter(blk_node, new_stmt_node, insert_wnx);

                        /* replace LHS */
                        WN *store = get_store_parent(coindexed_arr_ref);
                        if (store) WN_offset(store) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st, wn_arrayexp,
                                       &replace_wn);
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;

                    }
                    /* handle indirection ... */
                } else if ( array_ref_on_RHS(coindexed_arr_ref, &elem_type) ) {
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    WN *RHS_wn = WN_kid0(stmt_node);
                    WN *LHS_img;
                    TY_IDX LHS_coarray_type;
                    BOOL LHS_is_coindexed  = FALSE;

                    if (WN_operator(stmt_node) == OPR_ISTORE ||
                        WN_operator(stmt_node) == OPR_MSTORE) {
                        WN *LHS_wn = WN_kid1(stmt_node);
                        if (expr_is_coindexed(LHS_wn, &LHS_img,
                            &LHS_coarray_type)) {
                            LHS_is_coindexed = TRUE;
                        }
                    }

                    if ( !is_lvalue(RHS_wn) || LHS_is_coindexed
                         || is_convert_operation(RHS_wn)
                         /* for now, use LCB for LDID, ILOAD, MLOAD as well */
                         || is_load_operation(RHS_wn) ) {
                        WN *new_stmt_node;
                        ST *LCB_st;
                        WN *xfer_sz_node;

                        /* create LCB for coindexed array ref */
                        LCB_st = gen_lcbptr_symbol(
                               Make_Pointer_Type(elem_type,FALSE),
                               "LCB" );
                        xfer_sz_node = get_transfer_size(coindexed_arr_ref,
                                                         elem_type);
                        insert_wnx = Generate_Call_acquire_lcb(
                                xfer_sz_node,
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);

                        /* create "normalized" assignment from remote coarray */
                        new_stmt_node = get_lcb_assignment(
                                WN_COPY_Tree(Get_Parent(wn)),
                                coarray_type,
                                LCB_st);

                        WN_INSERT_BlockBefore(blk_node, stmt_node, new_stmt_node);

                        /* replace term in RHS */
                        WN *load = get_load_parent(coindexed_arr_ref);
                        if (load) WN_offset(load) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st, wn_arrayexp,
                                       &replace_wn);
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;

                        /* call to release LCB */
                        insert_wnx = Generate_Call_release_lcb(
                                WN_Lda(Pointer_type, 0, LCB_st));
                        WN_INSERT_BlockAfter(blk_node, stmt_node, insert_wnx);

                    } else {
                    /* handle indirection ... */
                    }
                } else if ( array_ref_in_parm(coindexed_arr_ref) ) {
                    WN *new_stmt_node;
                    ST *LCB_st;
                    WN *xfer_sz_node;

                    elem_type = Ty_Table[coarray_type].u2.etype;

                    /* create LCB for coindexed array ref */
                    LCB_st = gen_lcbptr_symbol(
                            Make_Pointer_Type(elem_type,FALSE),
                            "LCB" );
                    xfer_sz_node = get_transfer_size(coindexed_arr_ref,
                            elem_type);
                    insert_wnx = Generate_Call_acquire_lcb(
                            xfer_sz_node,
                            WN_Lda(Pointer_type, 0, LCB_st));
                    WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);

                    if (Get_Parent(coindexed_arr_ref) == rhs_ref_param_wn) {
                        /*coarray read*/
                        insert_blk = gen_coarray_access_stmt( wn,
                                NULL, LCB_st, xfer_sz_node, READ_TO_LCB);
                        /* get LDID node in replace_wn that substitues wn
                         * pointed to by wipre*/
                        WN *load = get_load_parent(coindexed_arr_ref);
                        if (load) WN_offset(load) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st,
                                       wn_arrayexp, &replace_wn);
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;
                        insert_wnx = WN_first(insert_blk);
                        while (insert_wnx) {
                            insert_wnx = WN_EXTRACT_FromBlock(insert_blk,
                                    insert_wnx);
                            WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);
                            insert_wnx = WN_first(insert_blk);
                        }
                        WN_Delete(insert_blk);
                        insert_blk = NULL;
                        rhs_ref_param_wn = NULL;
                    } else if (Get_Parent(coindexed_arr_ref) == lhs_ref_param_wn) {
                        /*corray write*/

                        /* get LDID node in replace_wn that substitues wn
                         * pointed to by wipre*/
                        WN *store = get_store_parent(coindexed_arr_ref);
                        if (store) WN_offset(store) = 0;
                        substitute_lcb(direct_coarray_ref, LCB_st, wn_arrayexp,
                                       &replace_wn);

                        insert_blk = gen_coarray_access_stmt( wn, NULL,
                                LCB_st, xfer_sz_node, WRITE_FROM_LCB);
                        insert_wnx = WN_last(insert_blk);
                        while (insert_wnx) {
                            insert_wnx = WN_EXTRACT_FromBlock(insert_blk, insert_wnx);
                            WN_INSERT_BlockAfter(blk_node, stmt_node, insert_wnx);
                            insert_wnx = WN_last(insert_blk);
                        }
                        WN_Delete(coindexed_arr_ref);
                        temp_wipre.Replace(replace_wn);
                        wipre = temp_wipre;
                        WN_Delete(insert_blk);
                        insert_blk = NULL;
                        lhs_ref_param_wn = NULL;
                    }
                }

                break;
        }
    }


    /*
     * Lower to CAF Runtime calls for 1-sided communication.
     */
    Parentize(func_body);
    for (wipre = wcpre.begin(); wipre != wcpre.end(); ++wipre) {
        WN *insert_blk = NULL;
        WN *wn = wipre.Wn();
        WN *insert_wnx;
        WN *blk_node;
        WN *stmt_node;
        WN *parent;
        WN *wn_arrayexp;

        TY_IDX coarray_type;
        TY_IDX elem_type;
        WN *replace_wn = NULL;
        WN *image;
        WN *coindexed_arr_ref;
        WN *direct_coarray_ref;

        parent = wipre.Get_parent_wn();

        /* if its a statement, set stmt_node and also set blk_node to
         * parent if the parent is a block */
        if ((OPCODE_is_stmt(WN_opcode(wn)) ||
            OPCODE_is_scf(WN_opcode(wn)) &&
             WN_operator(wn) != OPR_BLOCK &&
             WN_operator(wn) != OPR_FUNC_ENTRY)) {
          stmt_node = wn;
          wn_arrayexp = NULL;
          if (WN_operator(parent) == OPR_BLOCK) blk_node = parent;
        }

        /* stores most recently encountered ARRAYEXP in wn_arrayexp */
        if (WN_operator(wn) == OPR_ARRAYEXP)
          wn_arrayexp = wn;

        switch (WN_operator(wn)) {
            case OPR_ARRAY:
                coindexed_arr_ref = expr_is_coindexed(wn, &image, &coarray_type,
                                                      &direct_coarray_ref);
                if (image == NULL) break;

                if (is_vector_access(coindexed_arr_ref)) break;

                Is_True( Is_LCB_Stmt(stmt_node),
                  ("Expected LCB statement for co-indexed array statement"));

                if ( array_ref_on_LHS(coindexed_arr_ref, &elem_type) ) {
                    WN *RHS_wn = WN_kid0(stmt_node);
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    Is_True(is_lvalue(RHS_wn),
                            ("Unexpected coarray ref found in RHS"));

                    ST *LCB_st;
                    WN *xfer_sz_node;

                    LCB_st = get_lcb_sym(RHS_wn);
                    xfer_sz_node = get_transfer_size(RHS_wn, elem_type);
                    insert_blk = gen_coarray_access_stmt( wn,
                            NULL, LCB_st,
                            xfer_sz_node, WRITE_FROM_LCB);

                } else  if ( array_ref_on_RHS(coindexed_arr_ref, &elem_type) ) {
                    if (elem_type == TY_IDX_ZERO) {
                        /* if elem_type being accessed still can't be resolved,
                         * just use the coarray etype */
                        elem_type = Ty_Table[coarray_type].u2.etype;
                    }
                    WN *RHS_wn = WN_kid0(stmt_node);
                    Is_True(is_lvalue(RHS_wn),
                            ("Unexpected coarray ref found in RHS"));

                    ST *LCB_st;
                    WN *LHS_wn = WN_kid1(stmt_node);
                    WN *xfer_sz_node;

                    LCB_st = get_lcb_sym(LHS_wn);
                    xfer_sz_node = get_transfer_size(wn, elem_type);
                    insert_blk = gen_coarray_access_stmt( wn,
                            NULL, LCB_st,
                            xfer_sz_node, READ_TO_LCB);
                }

                /* replace with runtime call for remote coarray access */
                if (insert_blk) {
                    insert_wnx = WN_first(insert_blk);
                    while (insert_wnx) {
                        insert_wnx = WN_EXTRACT_FromBlock(insert_blk, insert_wnx);
                        WN_INSERT_BlockBefore(blk_node, stmt_node, insert_wnx);
                        insert_wnx = WN_first(insert_blk);
                    }
                    /* defer deletion of the old statment so that we can continue
                     * to traverse the tree */
                    add_caf_stmt_to_delete_list(stmt_node, blk_node);
                    WN_Delete(insert_blk);
                    insert_blk = NULL;
                }
        }
    }

    /* remove statements in caf_delete_list and clear the list */
    delete_caf_stmts_in_delete_list();

    WN_MAP_Delete(Caf_Parent_Map);
    WN_MAP_Delete(Caf_LCB_Map);
    WN_MAP_Delete(Caf_Visited_Map);

    return pu;
} /* Coarray_Lower */


static const char * const dope_str_prefix = ".dope." ;
static const INT dope_str_prefix_len = 6;

static BOOL is_dope(const TY_IDX tyi)
{
  if (TY_kind(tyi) == KIND_STRUCT &&
      strncmp(TY_name(tyi), dope_str_prefix, dope_str_prefix_len) == 0)
    return TRUE;
  else
      return FALSE;
}


/*
 * currentpu_ismain
 *
 * Checks if the current PU is the main entry point of program
 */
static BOOL currentpu_ismain()
{
	ST *pu_st = Get_Current_PU_ST();
	char *pu_name = ST_name(pu_st);

	switch (PU_src_lang(Get_Current_PU())) {
		case PU_C_LANG:
		case PU_CXX_LANG:
			return strcmp( pu_name, "main") == 0;
			break;
		case PU_F90_LANG:
		case PU_F77_LANG:
			return strcmp( pu_name, "MAIN__") == 0;
			break;
		default:
			FmtAssert (FALSE, ("Unknown source language type"));
	}
} /* currentpu_ismain */


/*
 * is_coarray_type
 *
 * Checks if array type is coarray (has codimensions)
 */
static BOOL is_coarray_type(const TY_IDX tyi)
{
    if (TY_kind(tyi) == KIND_POINTER)
      return TY_is_coarray(TY_pointed(tyi));
    else
      return TY_is_coarray(tyi);
} /* is_coarray_type */

/*
 * set_coarray_tsize
 *
 * Determines size of (local) coarray. Set TY_size for coarray type
 * accordingly.
 *
 * TODO: not tested on deferred-size / allocatables.
 */
static void set_coarray_tsize(TY_IDX coarray_type)
{
    UINT16 i;
    TY_IDX real_coarray_type = coarray_type;
    TY_IDX base_type;
    if (TY_kind(coarray_type) == KIND_POINTER) {
        real_coarray_type = TY_pointed(coarray_type);
    }
    base_type = Ty_Table[real_coarray_type].u2.etype;
    ARB_HANDLE bounds(Ty_Table[real_coarray_type].Arb());
    INT ndim, co_dim, array_dim;
    ndim = ARB_dimension(bounds);
    co_dim = ARB_codimension(bounds);
    array_dim = ndim - co_dim;
    INT32 coarray_size =  TY_size(base_type);

    if (array_dim != 0)
        for (i = co_dim; i < ndim; i++) {
            INT16 ub = bounds[i].Entry()->u2.ubnd_val;
            INT16 lb = bounds[i].Entry()->u1.lbnd_val;
            coarray_size *= ub - lb + 1;
        }

    Set_TY_size(real_coarray_type, coarray_size);
} /* set_coarray_tsize */

static INT get_1darray_size(const TY_IDX tyi)
{
    ARB_HANDLE arb(Ty_Table[tyi].Arb());
    return (ARB_ubnd_val(arb) - ARB_lbnd_val(arb)+1);
}

static INT get_array_rank(const TY_IDX tyi)
{
    ARB_HANDLE arb(Ty_Table[tyi].Arb());
    return (ARB_dimension(arb));
}

static INT get_coarray_rank(const TY_IDX tyi)
{
    ARB_HANDLE arb(Ty_Table[tyi].Arb());
    return (ARB_dimension(arb) - ARB_codimension(arb));
}

static INT get_coarray_corank(const TY_IDX tyi)
{
    ARB_HANDLE arb(Ty_Table[tyi].Arb());
    return (ARB_codimension(arb));
}

static TY_IDX get_array_type(const ST *array_st)
{
    TY_IDX ty = ST_type(array_st);
    if (TY_kind(ty) == KIND_POINTER)
        return TY_pointed(ty);
    return ty;
}

static ST* gen_lcbptr_symbol(TY_IDX tyi, const char *rootname)
{
    ST *st = Gen_Temp_Named_Symbol(tyi, rootname, CLASS_VAR, SCLASS_AUTO);
    Set_ST_is_temp_var(st);
    Set_ST_is_lcb_ptr(st);
    return st;
}

/*
 * gen_coarray_access_stmt:
 *
 * Assumptions:
 *  - operator for remote_access is either OPR_ARRAY or OPR_ARRSECTION, and
 *    the offset of its parent ILOAD is 0
 *  - if access is READ_DIRECT or WRITE_DIRECT, local_ref will be non-NULL
 *    and LCB_st will be NULL
 *  - coarray_ref (and local_ref, if defined) are part of the PU tree
 *
 */
static WN* gen_coarray_access_stmt(WN *coarray_ref, WN *local_ref,
                                   ST *LCB_st, WN *xfer_size,
                                   ACCESS_TYPE access)
{
    WN *return_blk;
    TY_IDX coarray_type, elem_type;
    WN *image;
    WN *coarray_base_address;
    WN *local_base_address;
    vector<WN*> coarray_dim_sm;
    vector<WN*> coarray_subscript_strides;
    BOOL coarray_strided_first_subscript;
    INT8 coarray_rank, coarray_corank, coarray_totalrank;
    INT8 local_rank;
    vector<WN*> local_dim_sm;
    vector<WN*> local_subscript_strides;
    BOOL local_strided_first_subscript;
    WN_OFFSET offset = 0;
    WN *direct_coarray_ref;

    return_blk = WN_CreateBlock();

    coarray_ref = expr_is_coindexed(coarray_ref, &image, &coarray_type,
                                    &direct_coarray_ref);

    /* TODO: handle nested array expressions eventually, but for now we'll
     * just use direct_coarray_ref
     */
    coarray_ref = direct_coarray_ref;

    /* get enclosing assignment stmt node */
    WN *stmt_node = coarray_ref;
    while (stmt_node && !OPCODE_is_stmt(WN_opcode(stmt_node)))
        stmt_node = Get_Parent(stmt_node);

    Is_True( stmt_node != NULL,
            ("couldn't find enclosing statement for coarray reference!"));

    elem_type = get_assign_stmt_datatype(stmt_node);
    if (array_ref_on_LHS(coarray_ref, &elem_type)) ;
    else if (array_ref_on_RHS(coarray_ref, &elem_type)) ;


    /* get dimension info for the coarray */
    coarray_rank = get_coarray_rank(coarray_type);
    coarray_corank = get_coarray_corank(coarray_type);
    coarray_totalrank = coarray_rank + coarray_corank;

    /* get the dimension stride multipliers for coarray */
    coarray_dim_sm.resize(coarray_rank);
    if (coarray_rank > 0)
        coarray_dim_sm[0] = WN_Intconst(Integer_type, TY_size(elem_type));
    BOOL coarray_ref_noncontig = (WN_element_size(coarray_ref) < 0);
    if (coarray_ref_noncontig) {
        for (int i = 1; i < coarray_rank; i++) {
            coarray_dim_sm[i] =
                WN_COPY_Tree(WN_kid(coarray_ref, coarray_totalrank-i));
        }
    } else {
        for (int i = 1; i < coarray_rank; i++) {
            coarray_dim_sm[i] = WN_Mpy( MTYPE_U8,
                    WN_COPY_Tree(coarray_dim_sm[i-1]),
                    WN_COPY_Tree(WN_kid(coarray_ref, coarray_totalrank+1-i)) );
        }
    }

    /* compute base address for coarray_ref */
    WN *coarray_ref_offset = WN_Intconst(Integer_type, 0);
    /* offset for coarray_ref */
    if (access == READ_TO_LCB or access == READ_DIRECT) {
        WN *parent_load = coarray_ref;
        while (parent_load &&
               WN_operator(parent_load) != OPR_LDID  &&
               WN_operator(parent_load) != OPR_ILOAD &&
               WN_operator(parent_load) != OPR_MLOAD) {
            parent_load = Get_Parent(parent_load);
            if (parent_load && WN_operator(parent_load) == OPR_ADD) {
                coarray_ref_offset = WN_Add(Integer_type,
                        coarray_ref_offset,
                        WN_COPY_Tree(WN_kid1(parent_load)));
            }
        }
        if (parent_load) {
            offset = WN_offset(parent_load);
            if (offset != 0) {
                coarray_ref_offset = WN_Add(Integer_type,
                        coarray_ref_offset,
                        WN_Intconst(Integer_type, offset));
            }
        }
    } else {
        WN *parent_store = coarray_ref;
        while (parent_store &&
               WN_operator(parent_store) != OPR_STID &&
               WN_operator(parent_store) != OPR_ISTORE) {
            parent_store = Get_Parent(parent_store);
            if (parent_store && WN_operator(parent_store) == OPR_ADD) {
                coarray_ref_offset = WN_Add(Integer_type,
                        coarray_ref_offset,
                        WN_COPY_Tree(WN_kid1(parent_store)));
            }
        }
        if (parent_store) {
            offset = WN_offset(parent_store);
            if (offset != 0) {
                coarray_ref_offset = WN_Add(Integer_type,
                        coarray_ref_offset,
                        WN_Intconst(Integer_type, offset));
            }
        }
    }

    for (int i = 0; i < coarray_rank; i++) {
        WN *dim_offset;
        WN *sub = WN_array_index(coarray_ref, coarray_totalrank-i-1);
        if (WN_operator(sub) == OPR_TRIPLET) {
            dim_offset = WN_COPY_Tree(WN_kid0(sub));
        } else if (WN_operator(sub) == OPR_ARRAYEXP) {
            /* TODO: add support for this */
            dim_offset = NULL;
        } else {
            dim_offset = WN_COPY_Tree(sub);
        }

        coarray_ref_offset = WN_Add(MTYPE_U8,
                WN_Mpy(MTYPE_U8, dim_offset,
                        WN_COPY_Tree(coarray_dim_sm[i])),
                        coarray_ref_offset);
    }

    coarray_base_address = WN_Add(Pointer_type,
                          WN_COPY_Tree(WN_kid0(direct_coarray_ref)),
                          coarray_ref_offset);


    coarray_strided_first_subscript = FALSE;
    coarray_subscript_strides.resize(coarray_rank);
    if (WN_operator(coarray_ref) != OPR_ARRSECTION) {
        for (int i = 0; i < coarray_rank; i++)
            coarray_subscript_strides[i] = WN_Intconst(Integer_type, 1);
    } else {
        for (int i = 0; i < coarray_rank; i++)  {
            WN *sub = WN_array_index(coarray_ref, coarray_totalrank-i-1);
            if (WN_operator(sub) == OPR_TRIPLET) {
                coarray_subscript_strides[i] = WN_COPY_Tree(WN_kid1(sub));
                BOOL stride_is_const =
                   (WN_operator(WN_kid1(sub)) == OPR_INTCONST);
                if (i == 0 && (!stride_is_const ||
                    stride_is_const && WN_const_val(WN_kid1(sub)) > 1)) {
                    coarray_strided_first_subscript = TRUE;
                }
            } else {
                coarray_subscript_strides[i] = WN_Intconst(Integer_type, 1);
            }

        }
    }

    /* compute base address for local ref */

    local_strided_first_subscript = FALSE;
    ST *local_array_st;
    if (access == READ_TO_LCB || access == WRITE_FROM_LCB) {
        /* local_ref == NULL */
        local_base_address = WN_Ldid(Pointer_type, 0, LCB_st,
                                     ST_type(LCB_st));
    } else {
        /* local_ref != NULL */

        /* READ_DIRECT or WRITE_DIRECT access:
         * assume the local_ref operator is OPR_ARRSECTION if in
         * pre-lowering phase. */

        Is_True(local_ref != NULL,
                ("local_ref is NULL for DIRECT access type"));

        TY_IDX etype;
        local_array_st = WN_st(WN_kid0(local_ref));
        TY_IDX arr_type = get_array_type(local_array_st);
        if (is_dope(arr_type) || TY_kind(arr_type) == KIND_POINTER)
            arr_type = TY_pointed(FLD_type(TY_fld(arr_type)));


        if (TY_kind(arr_type) == KIND_ARRAY) {
            etype  = Ty_Table[arr_type].u2.etype;
            if (is_coarray_type(arr_type))
                local_rank = get_coarray_rank(arr_type);
            else
                local_rank = get_array_rank(arr_type);
        } else {
            local_rank = WN_num_dim(local_ref);
        }

        local_dim_sm.resize(local_rank);
        local_dim_sm[0] = WN_Intconst(Integer_type, TY_size(elem_type));

        BOOL local_ref_noncontig = (WN_element_size(local_ref) < 0);

        if (local_ref_noncontig) {
            for (int i = 1; i < local_rank; i++) {
                local_dim_sm[i] = WN_COPY_Tree( WN_kid(local_ref,
                                                       local_rank-i));
            }
        } else {
            for (int i = 1; i < local_rank; i++) {
                local_dim_sm[i] = WN_Mpy(MTYPE_U8,
                        WN_COPY_Tree(local_dim_sm[i-1]),
                        WN_COPY_Tree(WN_kid(local_ref, local_rank+1-i)));
            }
        }

        WN *local_ref_offset = WN_Intconst(Integer_type, 0);

        for (int i = 0; i < local_rank; i++) {
            WN *dim_offset;
            WN *sub = WN_array_index(local_ref, local_rank-i-1);
            if (WN_operator(sub) == OPR_TRIPLET) {
                dim_offset = WN_COPY_Tree(WN_kid0(sub));
            } else if (WN_operator(sub) == OPR_ARRAYEXP) {
                /* TODO: add support for this */
                dim_offset = NULL;
            } else {
                dim_offset = WN_COPY_Tree(sub);
            }

            local_ref_offset = WN_Add(MTYPE_U8,
                    WN_Mpy(MTYPE_U8, dim_offset,
                            WN_COPY_Tree(local_dim_sm[i])),
                            local_ref_offset);
        }

        local_base_address = WN_Add(Pointer_type,
                              WN_COPY_Tree(WN_kid0(local_ref)),
                              local_ref_offset);

        local_subscript_strides.resize(local_rank);
        if (WN_operator(local_ref) != OPR_ARRSECTION) {
            for (int i = 0; i < local_rank; i++)
                local_subscript_strides[i] = WN_Intconst(Integer_type, 1);
        } else {
            for (int i = 0; i < local_rank; i++)  {
                WN *sub = WN_array_index(local_ref, local_rank-i-1);
                if (WN_operator(sub) == OPR_TRIPLET) {
                    local_subscript_strides[i] = WN_COPY_Tree(WN_kid1(sub));
                    BOOL stride_is_const =
                        (WN_operator(WN_kid1(sub)) == OPR_INTCONST);
                    if (i == 0 && (!stride_is_const ||
                        stride_is_const && WN_const_val(WN_kid1(sub)) > 1)) {
                        local_strided_first_subscript = TRUE;
                    }
                } else {
                    local_subscript_strides[i] = WN_Intconst(Integer_type, 1);
                }
            }
        }
    }

    BOOL coarray_ref_is_contig = is_contiguous_access(coarray_ref);
    BOOL local_ref_is_contig = access == READ_TO_LCB ||
                               access == WRITE_FROM_LCB ||
                               ST_is_lcb_ptr(local_array_st) ||
                               is_contiguous_access(local_ref);

    if (coarray_ref_is_contig &&
        (access == READ_TO_LCB ||
        (local_ref_is_contig && access == READ_DIRECT))) {
        WN * call = Generate_Call_coarray_read(image,
                                          coarray_base_address,
                                          local_base_address,
                                          WN_COPY_Tree(xfer_size));
        WN_INSERT_BlockFirst( return_blk, call);
    } else if (coarray_ref_is_contig &&
        (access == WRITE_FROM_LCB ||
        (local_ref_is_contig && access == WRITE_DIRECT))) {
        WN * call = Generate_Call_coarray_write(image,
                                          coarray_base_address,
                                          local_base_address,
                                          WN_COPY_Tree(xfer_size));
        WN_INSERT_BlockFirst( return_blk, call);
    } else {

        /* generate a strided access
         * TODO: vector access for ARRAYEXP subscripts */

        TY_IDX dim_array_type = create_arr1_type(MTYPE_U8, coarray_rank+1);
        ST *count_st = Gen_Temp_Named_Symbol(
                dim_array_type, "count", CLASS_VAR, SCLASS_AUTO);
        Set_ST_is_temp_var(count_st);
        ST *coarray_strides_st = Gen_Temp_Named_Symbol(
                dim_array_type, "coarray_strides", CLASS_VAR, SCLASS_AUTO);
        Set_ST_is_temp_var(coarray_strides_st);

        ST *local_strides_st = Gen_Temp_Named_Symbol(
                dim_array_type, "local_strides", CLASS_VAR, SCLASS_AUTO);
        Set_ST_is_temp_var(local_strides_st);

        OPCODE op_array = OPCODE_make_op( OPR_ARRAY, Pointer_type, MTYPE_V );

        WN *count_store = NULL;
        WN *stride_store = NULL;


        int count_idx = 0;
        int coarray_stride_idx = 0;
        int local_stride_idx = 0;
        int coarray_ref_idx = 0;
        int local_ref_idx = 0;

        /* set count to elem_size for first dimension if necessary
         * NOTE: currently assuming coarray_dim_sm[0] to be elem_size
         */
        WN *elem_size = coarray_dim_sm[0]; /* will copy later when used */
        WN *count_array;


        WN *count_val = NULL;
        WN *coarray_stride_val = NULL;
        WN *local_stride_val = NULL;
        int extra_count = 0;
        if (subscript_is_scalar(coarray_ref, coarray_ref_idx) ) {
            count_array = gen_array1_ref(op_array, dim_array_type,
                                count_st, count_idx, coarray_rank+1);
            count_val = WN_COPY_Tree(elem_size);
            count_idx++;
            coarray_ref_idx++;
            count_store = WN_Istore( MTYPE_U8, 0,
                    Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                    count_array, count_val );
        } else if ( subscript_is_strided(coarray_ref, coarray_ref_idx) ) {
            count_array = gen_array1_ref(op_array, dim_array_type,
                                count_st, count_idx, coarray_rank+1);
            count_val = WN_COPY_Tree(elem_size);
            count_idx++;
            count_store = WN_Istore( MTYPE_U8, 0,
                    Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                    count_array, count_val );
        } else if (local_ref != NULL ) {
            /* so first dimension of coarray reference is neither scalar or
             * strided. Check if the same is true for the local reference (not
             * an LCB). */
            if (subscript_is_scalar(local_ref, local_ref_idx) ) {
                count_array = gen_array1_ref(op_array, dim_array_type,
                                    count_st, count_idx, coarray_rank+1);
                count_val = WN_COPY_Tree(elem_size);
                count_idx++;
                local_ref_idx++;
                count_store = WN_Istore( MTYPE_U8, 0,
                        Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                        count_array, count_val );
            } else if ( subscript_is_strided(local_ref, local_ref_idx) ) {
                count_array = gen_array1_ref(op_array, dim_array_type,
                                    count_st, count_idx, coarray_rank+1);
                count_val = WN_COPY_Tree(elem_size);
                count_idx++;
                count_store = WN_Istore( MTYPE_U8, 0,
                        Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                        count_array, count_val );
            }
        }
        if (count_store) {
            extra_count = 1;
            WN_INSERT_BlockLast(return_blk, count_store);
        }

        /* set counts and coarray strides */
        while (coarray_ref_idx < coarray_rank) {
            WN *coarray_stride_array;
            WN *stride_factor = NULL;
            count_val = NULL;
            coarray_stride_val = NULL;

            count_store = NULL;
            stride_store = NULL;


            if ( subscript_is_scalar(coarray_ref, coarray_ref_idx) ) {
                coarray_ref_idx++;
            } else if ( subscript_is_strided(coarray_ref, coarray_ref_idx) ) {
                /* setup count and coarray stride array reference */
                count_array = gen_array1_ref(op_array, dim_array_type,
                                    count_st, count_idx, coarray_rank+1);

                count_val = subscript_extent(coarray_ref, coarray_ref_idx);
                if (count_idx == 0) {
                    count_val = WN_Mpy(MTYPE_U8, WN_COPY_Tree(elem_size),
                                       count_val);
                }
                count_store = WN_Istore( MTYPE_U8, 0,
                        Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                        count_array, count_val );

                if (count_idx > 0) {
                    coarray_stride_array = gen_array1_ref(op_array, dim_array_type,
                                        coarray_strides_st, coarray_stride_idx,
                                        coarray_rank);
                    stride_factor = subscript_stride(coarray_ref, coarray_ref_idx);
                    coarray_stride_val = WN_Mpy( Integer_type,
                            WN_COPY_Tree(coarray_dim_sm[coarray_ref_idx]),
                            stride_factor);

                    stride_store = WN_Istore( MTYPE_U8, 0,
                            Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                            coarray_stride_array, coarray_stride_val );
                    coarray_stride_idx++;
                }

                count_idx++;
                coarray_ref_idx++;
            } else {
                /* setup count and coarray stride array reference */
                count_array = gen_array1_ref(op_array, dim_array_type,
                                    count_st, count_idx, coarray_rank+1);
                coarray_stride_array = gen_array1_ref(op_array, dim_array_type,
                                    coarray_strides_st, coarray_stride_idx,
                                    coarray_rank);

                count_val = subscript_extent(coarray_ref, coarray_ref_idx);
                if (count_idx == 0) {
                    count_val = WN_Mpy(MTYPE_U8, WN_COPY_Tree(elem_size),
                                       count_val);
                }
                count_store = WN_Istore( MTYPE_U8, 0,
                        Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                        count_array, count_val );

                if (count_idx > 0) {
                    coarray_stride_val =
                        WN_COPY_Tree(coarray_dim_sm[coarray_ref_idx]);


                    stride_store = WN_Istore( MTYPE_U8, 0,
                            Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                            coarray_stride_array, coarray_stride_val );
                    coarray_stride_idx++;
                }

                count_idx++;
                coarray_ref_idx++;
            }

            if (count_store) {
                WN_INSERT_BlockLast(return_blk, count_store);
                if (stride_store)
                    WN_INSERT_BlockLast(return_blk, stride_store);
            }
        }


        if (local_ref == NULL)
            local_rank = coarray_ref_idx;

        coarray_ref_idx = 0;
        count_idx = extra_count;
        while (local_ref_idx < local_rank) {
            WN *local_stride_array;
            WN *stride_factor = NULL;
            local_stride_val = NULL;

            stride_store = NULL;

            if (local_ref != NULL) {
                if ( subscript_is_scalar(local_ref, local_ref_idx) ) {
                    local_ref_idx++;
                } else if ( subscript_is_strided(local_ref, local_ref_idx) ) {
                    if (count_idx > 0) {
                        local_stride_array = gen_array1_ref(op_array,
                                        dim_array_type, local_strides_st,
                                        local_stride_idx, coarray_rank);

                        stride_factor = subscript_stride(local_ref, local_ref_idx);
                        local_stride_val = WN_Mpy( Integer_type,
                                WN_COPY_Tree(local_dim_sm[local_ref_idx]),
                                stride_factor);

                        stride_store = WN_Istore( MTYPE_U8, 0,
                                Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                                local_stride_array, local_stride_val );

                        local_stride_idx++;
                    }

                    count_idx++;
                    local_ref_idx++;
                } else {
                    if (count_idx > 0) {
                        local_stride_array = gen_array1_ref(op_array,
                                dim_array_type, local_strides_st,
                                local_stride_idx, coarray_rank);

                        local_stride_val =
                            WN_COPY_Tree(local_dim_sm[local_ref_idx]);

                        stride_store = WN_Istore( MTYPE_U8, 0,
                                Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                                local_stride_array, local_stride_val );

                        local_stride_idx++;
                    }

                    count_idx++;
                    local_ref_idx++;
                }
            } else {
                /* using LCB, so strides are based on the extents of the
                 * coarray reference */
                if ( subscript_is_scalar(coarray_ref, local_ref_idx) ) {
                    local_ref_idx++;
                } else {
                    if (count_idx > 0) {
                        local_stride_array = gen_array1_ref(op_array,
                                        dim_array_type, local_strides_st,
                                        local_stride_idx, coarray_rank);

                        if (local_stride_idx == 0) {
                            local_stride_val = WN_COPY_Tree(elem_size);
                        } else {
                            local_stride_val =
                                subscript_extent(coarray_ref, local_ref_idx-1);
                        }

                        stride_store = WN_Istore( MTYPE_U8, 0,
                                Make_Pointer_Type(MTYPE_To_TY(MTYPE_U8), FALSE),
                                local_stride_array, local_stride_val );

                        local_stride_idx++;
                    }

                    count_idx++;
                    local_ref_idx++;
                }
            }

            if (stride_store) {
                WN_INSERT_BlockLast(return_blk, stride_store);
            }
        }


        if (access == READ_TO_LCB || access == READ_DIRECT) {
            WN *call = Generate_Call_coarray_strided_read( image,
                    coarray_base_address,
                    WN_Lda(Pointer_type, 0, coarray_strides_st),
                    local_base_address,
                    WN_Lda(Pointer_type, 0, local_strides_st),
                    WN_Lda(Pointer_type, 0, count_st),
                    WN_Intconst(Integer_type, count_idx-1)
                   );
            WN_INSERT_BlockLast( return_blk, call );
        } else if (access == WRITE_FROM_LCB || access == WRITE_DIRECT) {
            WN *call = Generate_Call_coarray_strided_write( image,
                    coarray_base_address,
                    WN_Lda(Pointer_type, 0, coarray_strides_st),
                    local_base_address,
                    WN_Lda(Pointer_type, 0, local_strides_st),
                    WN_Lda(Pointer_type, 0, count_st),
                    WN_Intconst(Integer_type, count_idx-1)
                   );
            WN_INSERT_BlockLast( return_blk, call );
        } else {
            /* shouldn't reach */
        }
    }

    for (int i = 0; i < coarray_dim_sm.size(); i++) {
        WN_Delete(coarray_dim_sm[i]);
    }
    coarray_dim_sm.clear();

    for (int i = 0; i < coarray_subscript_strides.size(); i++) {
        WN_Delete(coarray_subscript_strides[i]);
    }
    coarray_subscript_strides.clear();

    if (local_ref) {
        for (int i = 0; i < local_dim_sm.size(); i++) {
            WN_Delete(local_dim_sm[i]);
        }
        local_dim_sm.clear();

        for (int i = 0; i < local_subscript_strides.size(); i++) {
            WN_Delete(local_subscript_strides[i]);
        }
        local_subscript_strides.clear();
    }

    return return_blk;
} /* gen_coarray_access_stmt */


/*
 * substitute_lcb:
 *
 *   replaces a remote access reference with dereferenced or indexed reference
 *   to a temporary local coarray buffer.
 *
 *   returns replaced node in replace_wn. Also may modify wn_arrayexp if its
 *   an OPR_ARRSECTION operations.
 *
 */
static void substitute_lcb(WN *remote_access, ST *lcbtemp, WN *wn_arrayexp,
                           WN **replace_wn)
{
    TY_IDX ty1, ty2;
    TY_IDX elem_type;
    INT elem_size;
    ty1 = get_array_type_from_tree(remote_access);
    if ( is_dope(ty1) )
        ty1 = TY_pointed(FLD_type(TY_fld(ty1)));

    elem_type = TY_pointed(ST_type(lcbtemp));
    elem_size = TY_size(elem_type);

    /* resolve to a base element type */
    while ( TY_kind(elem_type) == KIND_ARRAY ) {
        elem_type = TY_etype(elem_type);
        elem_size = TY_size(elem_type);
        if (TY_kind(elem_type) == KIND_STRUCT) {
            elem_type = MTYPE_To_TY(MTYPE_U1);
            break;
        }
    }


    INT8 rank = get_coarray_rank(ty1);
    INT8 corank = get_coarray_corank(ty1);
    INT8 totalrank = rank +  corank;

    /* substitute lcbtemp into original remote_access node.
     * TODO: assuming OPR_ARRAY for simplicity right now ... */

    if (WN_operator(remote_access) == OPR_ARRAY) {
        *replace_wn =   WN_Ldid(Pointer_type, 0,
                                lcbtemp, ST_type(lcbtemp));
    } else if (WN_operator(remote_access) == OPR_ARRSECTION) {

        *replace_wn  = WN_Create( OPCODE_make_op(OPR_ARRSECTION,
                    WN_rtype(remote_access),
                    WN_desc(remote_access)), 1+2*rank);

        /* assume contiguous access in LCB */
        WN_element_size(*replace_wn) = elem_size;
           /* abs(WN_element_size(remote_access)); */

        WN_array_base(*replace_wn) =  WN_Ldid(Pointer_type, 0, lcbtemp,
                                              ST_type(lcbtemp));

        /* set sizes and index for each dimension of tmp buff */
        INT8 j = 1;
        for (INT8 i = 1; i <= rank; i++) {
            WN *wn_ext;
            /* TODO: handle other aggregate operators as well */
            if (WN_operator(WN_array_index(remote_access, corank+i-1)) == OPR_TRIPLET) {
              wn_ext = WN_COPY_Tree(WN_kid2(WN_array_index(remote_access,corank+i-1)));
            } else  {
              wn_ext = WN_Intconst(Integer_type, 1);
            }

            WN_array_dim(*replace_wn,i-1) = wn_ext;

            /* set subscript for dimension i */
            WN_array_index(*replace_wn,i-1) =
                WN_COPY_Tree(WN_array_index(remote_access, corank+i-1));

            if (WN_operator(WN_array_index(*replace_wn,i-1)) == OPR_TRIPLET) {
                WN_Delete(WN_kid0(WN_array_index(*replace_wn,i-1)));
                WN_kid0(WN_array_index(*replace_wn,i-1)) =
                    WN_Intconst(Integer_type, 0);
                WN_Delete(WN_kid1(WN_array_index(*replace_wn,i-1)));
                WN_kid1(WN_array_index(*replace_wn,i-1)) =
                    WN_Intconst(Integer_type, 1);
                WN_Delete(WN_kid2(WN_array_index(*replace_wn,i-1)));
                WN_kid2(WN_array_index(*replace_wn,i-1)) =
                    WN_COPY_Tree(WN_array_dim(*replace_wn, i-1));

                /* adjust enclosing arrayexp node */
                /*
                WN_Delete(WN_kid( wn_arrayexp, j));
                WN_kid( wn_arrayexp, j) =
                    WN_COPY_Tree(WN_array_dim(*replace_wn, i-1));
                    */

                j++;
            } else {
                WN_Delete(WN_array_index(*replace_wn,i-1));
                WN_array_index(*replace_wn,i-1) =
                    WN_Intconst(Integer_type,0);
            }
        }

    }

} /* substitute_lcb */



/*
 * is_contiguous_access : checks if an array access operation is accessing a
 * contiguous slice of the array.
 *
 * Algorithm:
 * requires_complete_access = FALSE
 * for d = last dim to first dim
 *    if access_range(d) < extent(d) && requires_complete_access
 *       return false
 *    else if access_range(d) == 1
 *       continue
 *    else if access_range(d) > 1
 *       requires_complete_access = TRUE
 *    fi
 *
 * return TRUE
 *
 */
static BOOL is_contiguous_access(WN *array_ref)
{
    INT8 num_dim, rank;
    FmtAssert(WN_operator(array_ref) != OPR_ARRAY ||
            WN_operator(array_ref) != OPR_ARRSECTION,
            ("Unexpected operator for array reference"));

    //ST *array_st = get_array_sym(array_ref);
    //TY_IDX arr_type = get_array_type(array_st);

    TY_IDX arr_type = get_array_type_from_tree(array_ref);
    if ( is_dope(arr_type) || TY_kind(arr_type) == KIND_POINTER)
        arr_type = TY_pointed(FLD_type(TY_fld(arr_type)));

    if (TY_kind(arr_type) != KIND_ARRAY)
        return TRUE;

    Is_True(TY_kind(arr_type) == KIND_ARRAY,
       ("arr_type for local_ref is not KIND_ARRAY for DIRECT access type"));

    if (is_coarray_type(arr_type))
        rank = get_coarray_rank(arr_type);
    else
        rank = get_array_rank(arr_type);

    num_dim = WN_num_dim(array_ref);

    INT* access_range = (INT *)malloc(num_dim*sizeof(INT));
    INT* extent = (INT *)malloc(num_dim*sizeof(INT));

    BOOL requires_complete_access = FALSE;

    if (WN_operator(array_ref) == OPR_ARRSECTION) {
        for (INT8 i = rank-1; i >= 0; i--) {
            WN *sub = WN_array_index(array_ref, num_dim-i-1);
            /* TODO: handle other aggregate operators as well */
            if (WN_operator(sub) == OPR_TRIPLET) {
                /* no static bounds, assume non-contiguous */
                if (WN_operator(WN_kid2(sub)) != OPR_INTCONST ||
                    WN_operator(WN_kid1(sub)) != OPR_INTCONST)
                    return FALSE;

                if (WN_operator(WN_kid1(sub)) == OPR_INTCONST &&
                    WN_const_val(WN_kid1(sub)) != 1)
                    return FALSE;

                access_range[i] = WN_const_val(WN_kid2(sub));
            } else {
                access_range[i] = 1;
            }

            /* calculate extents */
            WN *size = WN_array_dim(array_ref, num_dim-i-1);
            if (WN_operator(size) == OPR_INTCONST) {
                extent[i] = WN_const_val(size);
            } else {
                /* assume non-contiguous if array shape is not statically
                 * known */
                return FALSE;
            }

            if ((access_range[i] < extent[i]) && requires_complete_access)
                return FALSE;
            else if (access_range[i] > 1)  {
                requires_complete_access = TRUE;
            }
            else continue;
        }
    }

    free(access_range);
    free(extent);

    return TRUE;
}

static BOOL is_vector_access(WN *array_ref)
{
    WN_TREE_CONTAINER<POST_ORDER> wcpost(array_ref);
    WN *wn;
    INT8 num_dim, rank;
    BOOL is_vector = FALSE;


    wn = wcpost.begin().Wn();
    while (wn && WN_operator(wn) != OPR_ARRSECTION) {
        wn = Get_Parent(wn);
    }

    if (wn == NULL) return FALSE;
    array_ref = wn;

    if (WN_operator(array_ref) != OPR_ARRAY ||
            WN_operator(array_ref) != OPR_ARRSECTION)  {
    }


    TY_IDX arr_type = get_array_type_from_tree(array_ref);
    if ( is_dope(arr_type) || TY_kind(arr_type) == KIND_POINTER)
        arr_type = TY_pointed(FLD_type(TY_fld(arr_type)));

    if (TY_kind(arr_type) != KIND_ARRAY)
        return FALSE;

    Is_True(TY_kind(arr_type) == KIND_ARRAY,
       ("arr_type for local_ref is not KIND_ARRAY for DIRECT access type"));

    if (is_coarray_type(arr_type))
        rank = get_coarray_rank(arr_type);
    else
        rank = get_array_rank(arr_type);

    num_dim = WN_num_dim(array_ref);

    if (WN_operator(array_ref) == OPR_ARRSECTION) {
        for (INT8 i = rank-1; i >= 0; i--) {
            WN *sub = WN_array_index(array_ref, num_dim-i-1);
            if (WN_operator(sub) == OPR_ARRAYEXP) {
                return TRUE;
            }
        }
    }

    return FALSE;
}


static TY_IDX create_arr1_type(TYPE_ID elem_type, INT16 ne)
{
    TY_IDX arr_ty = TY_IDX_ZERO;
    TY_IDX elem_ty_idx= Be_Type_Tbl(elem_type);
    TY& ty = New_TY (arr_ty);
    TY_Init (ty, TY_size(elem_ty_idx)*ne, KIND_ARRAY, MTYPE_UNKNOWN,
            Save_Str2i("arrtype.", TY_name(elem_type), ne ));
    Set_TY_etype(ty, Be_Type_Tbl(elem_type));

    ARB_HANDLE arb = New_ARB ();
    ARB_Init (arb, 0, ne-1, TY_size(elem_ty_idx));
    Set_ARB_dimension(arb, 1);
    Set_ARB_first_dimen(arb);
    Set_ARB_last_dimen(arb);
    Set_ARB_const_lbnd(arb);
    Set_ARB_const_ubnd(arb);
    Set_ARB_const_stride(arb);

    Set_TY_arb(ty, arb);
    Set_TY_align (arr_ty, TY_size(elem_ty_idx));

    return arr_ty;
}

/*
 * stmt_rhs_is_addressable
 *
 * Returns TRUE if the RHS is identified as addressable. This includes:
 *     - LDID  (loading of a scalar value)
 *     - ILOAD (derefereced pointer, or loading of an array element
 *     - ARRAYEXP where kid0 is ILOAD
 *
 * TODO: For simplicity, currently only handles 3rd case from above.
 *       What about non-unit stride?
 */
static BOOL stmt_rhs_is_addressable(WN *stmt_node)
{
  WN *rhs;
  Is_True(stmt_node && WN_kid0(stmt_node),
          ("stmt_node not a value statement with kid 0"));

  rhs = WN_kid0(stmt_node);

  ///////// DEBUG ////////////////
  //fdump_tree(stdout, stmt_node);
  ////////////////////////////////

  return (WN_operator(rhs) == OPR_ARRAYEXP &&
          WN_operator(WN_kid0(rhs)) == OPR_ILOAD);
}

static WN * array_ref_is_coindexed(WN *arr, TY_IDX ty)
{
  WN *base_addr;
  ST *array_st;
  INT8 rank, corank, totalrank;

  if ((WN_operator(arr) != OPR_ARRAY) &&
      (WN_operator(arr) != OPR_ARRSECTION))
    return 0;

  if ( is_dope(ty) ) {
    ty = TY_pointed(FLD_type(TY_fld(ty)));
    if (!is_coarray_type(ty))
      return 0;
    rank = get_coarray_rank(ty);
    corank = get_coarray_corank(ty);
  } else  {
    /* break if not coarray */
    if (!is_coarray_type(ty))
      return 0;

    rank = get_coarray_rank(ty);
    corank = get_coarray_corank(ty);
  }

  /* check array depth matches in case its a coscalar of array type */
  if (rank == 0)  {
      TY_IDX ty1 = Ty_Table[ty].u2.etype;
      WN *p = Get_Parent(arr);
      while (TY_kind(ty1) == KIND_ARRAY) {
          ty1 = Ty_Table[ty1].u2.etype;
          if (!p || (WN_operator(p) != OPR_ARRAY &&
              WN_operator(p) != OPR_ARRSECTION)) {
              return 0;
          }
          p = Get_Parent(p);
      }
  }


  /* break if not cosubscripted */
  if (WN_kid_count(arr) == (1+2*rank))
    return 0;

  /* this is a image-selecting array reference. return expression that
   * computes selected image */

  totalrank = rank + corank;

  WN **wn_costr_m = (WN **)malloc(corank*sizeof(WN*));
  wn_costr_m[0] = WN_Intconst(Integer_type, 1);
  BOOL noncontig = (WN_element_size(arr) < 0);
  for (INT8 i=1; i < corank; i++) {
      if (noncontig)
          wn_costr_m[i] = WN_COPY_Tree(WN_kid(arr,corank-i));
      else
          wn_costr_m[i] = WN_Mpy( MTYPE_I8,
                  WN_COPY_Tree(wn_costr_m[i-1]),
                  WN_COPY_Tree(WN_kid(arr,corank+1-i)));
  }

  WN *image = WN_Intconst(MTYPE_U8, 1);
  for (INT8 i=0; i < corank; i++) {
      image = WN_Add(MTYPE_U8,
              WN_Mpy(MTYPE_I8,
                  WN_COPY_Tree(WN_kid(arr, 2*totalrank-rank-i)),
                  WN_COPY_Tree(wn_costr_m[i]) ),
              image);
  }


  free(wn_costr_m);
  return image;
}


/*
 * adds the <stmt, blk> pair to the delete list
 *   return 1 if stmt was not already in delete list
 *   return 0 if stmt was already in delete list (no add)
 */
static int add_caf_stmt_to_delete_list(WN *stmt, WN *blk)
{
  int i;
  CAF_STMT_NODE stmt_to_delete;
  int ok_to_delete = 1;

  /* check if statement is already in the delete list */
  for (i = 0; i != caf_delete_list.size(); ++i) {
    if (caf_delete_list[i].stmt == stmt) {
      ok_to_delete = 0;
      break;
    }
  }

  if (ok_to_delete) {
    stmt_to_delete.stmt = stmt;
    stmt_to_delete.blk = blk;
    caf_delete_list.push_back(stmt_to_delete);
    return 1;
  } else {
    return 0;
  }
}

static void delete_caf_stmts_in_delete_list()
{
  for (int i = 0; i != caf_delete_list.size(); ++i) {
    CAF_STMT_NODE stmt_to_delete = caf_delete_list[i];
    Is_True(stmt_to_delete.stmt && stmt_to_delete.blk,
        ("invalid CAF_STMT_NODE object in caf_delete_list"));
    WN *stmt = WN_EXTRACT_FromBlock(stmt_to_delete.blk, stmt_to_delete.stmt);
    WN_DELETE_Tree(stmt);
  }

  caf_delete_list.clear();
}



/* routines for generating specific runtime calls
 */

static WN *
Generate_Call_acquire_lcb(WN *xfer_size, WN *lcb_ptr)
{
    WN *call = Generate_Call_Shell( ACQUIRE_LCB, MTYPE_V, 2);
    WN_actual( call, 0 ) =
        Generate_Param( xfer_size, WN_PARM_BY_VALUE);
    WN_actual( call, 1 ) =
        Generate_Param( lcb_ptr, WN_PARM_BY_REFERENCE);

    return call;
}

static WN *
Generate_Call_release_lcb(WN *lcb_ptr)
{
    WN *call = Generate_Call_Shell( RELEASE_LCB, MTYPE_V, 1);
    WN_actual( call, 0 ) =
        Generate_Param( lcb_ptr, WN_PARM_BY_REFERENCE);

    return call;
}

static WN *
Generate_Call_coarray_read(WN *image, WN *src, WN *dest, WN *nbytes)
{
    WN *call = Generate_Call_Shell( COARRAY_READ, MTYPE_V, 4);
    WN_actual( call, 0 ) =
        Generate_Param( image, WN_PARM_BY_VALUE);
    WN_actual( call, 1 ) =
        Generate_Param( src, WN_PARM_BY_REFERENCE);
    WN_actual( call, 2 ) =
        Generate_Param( dest, WN_PARM_BY_REFERENCE);
    WN_actual( call, 3 ) =
        Generate_Param( nbytes, WN_PARM_BY_VALUE);

    return call;
}

static WN *
Generate_Call_coarray_write(WN *image, WN *dest, WN *src, WN *nbytes)
{
    WN *call = Generate_Call_Shell( COARRAY_WRITE, MTYPE_V, 4);
    WN_actual( call, 0 ) =
        Generate_Param( image, WN_PARM_BY_VALUE);
    WN_actual( call, 1 ) =
        Generate_Param( dest, WN_PARM_BY_REFERENCE);
    WN_actual( call, 2 ) =
        Generate_Param( src, WN_PARM_BY_REFERENCE);
    WN_actual( call, 3 ) =
        Generate_Param( nbytes, WN_PARM_BY_VALUE);

    return call;
}

static WN * Generate_Call_coarray_strided_read(WN *image, WN *src,
                                    WN *src_strides, WN *dest,
                                    WN *dest_strides, WN *count,
                                    WN *stride_levels)
{
    WN *call = Generate_Call_Shell( COARRAY_STRIDED_READ, MTYPE_V, 7);
    WN_actual( call, 0 ) =
        Generate_Param( image, WN_PARM_BY_VALUE);
    WN_actual( call, 1 ) =
        Generate_Param( src, WN_PARM_BY_REFERENCE);
    WN_actual( call, 2 ) =
        Generate_Param( src_strides, WN_PARM_BY_REFERENCE);
    WN_actual( call, 3 ) =
        Generate_Param( dest, WN_PARM_BY_REFERENCE);
    WN_actual( call, 4 ) =
        Generate_Param( dest_strides, WN_PARM_BY_REFERENCE);
    WN_actual( call, 5 ) =
        Generate_Param( count, WN_PARM_BY_REFERENCE);
    WN_actual( call, 6 ) =
        Generate_Param( stride_levels, WN_PARM_BY_VALUE);

    return call;
}

static WN * Generate_Call_coarray_strided_write(WN *image, WN *dest,
                                    WN *dest_strides, WN *src,
                                    WN *src_strides, WN *count,
                                    WN *stride_levels)
{
    WN *call = Generate_Call_Shell( COARRAY_STRIDED_WRITE, MTYPE_V, 7);
    WN_actual( call, 0 ) =
        Generate_Param( image, WN_PARM_BY_VALUE);
    WN_actual( call, 1 ) =
        Generate_Param( dest, WN_PARM_BY_REFERENCE);
    WN_actual( call, 2 ) =
        Generate_Param( dest_strides, WN_PARM_BY_REFERENCE);
    WN_actual( call, 3 ) =
        Generate_Param( src, WN_PARM_BY_REFERENCE);
    WN_actual( call, 4 ) =
        Generate_Param( src_strides, WN_PARM_BY_REFERENCE);
    WN_actual( call, 5 ) =
        Generate_Param( count, WN_PARM_BY_REFERENCE);
    WN_actual( call, 6 ) =
        Generate_Param( stride_levels, WN_PARM_BY_VALUE);

    return call;
}

/*
 * init_caf_extern_symbols
 *
 * Generates global extern symbols for:
 *    _this_image (this_image_st)
 *    _num_images (num_images_st)
 *
 * if they have not already been created.
 */
void init_caf_extern_symbols()
{

    if (this_image_st == NULL) {
        this_image_st = New_ST( GLOBAL_SYMTAB );
        ST_Init( this_image_st, Save_Str( "_this_image" ),
                CLASS_VAR, SCLASS_EXTERN, EXPORT_PREEMPTIBLE,
                MTYPE_To_TY(MTYPE_U8));
    }

    if (num_images_st == NULL) {
        num_images_st = New_ST( GLOBAL_SYMTAB );
        ST_Init( num_images_st, Save_Str( "_num_images" ),
                CLASS_VAR, SCLASS_EXTERN, EXPORT_PREEMPTIBLE,
                MTYPE_To_TY(MTYPE_U8));
    }
}

/*
 * handle_caf_call_stmts
 *
 * Certain call statements generated in front-end need to be further processed
 * in back-end. This includes, currently:
 *
 *    _THIS_IMAGE0: replace with global symbol _this_image
 *    _NUM_IMAGES: replace with global symbol _num_images
 *    _SYNC_IMAGES: replace arguments with (array-list, #array-list)
 *
 */
static void handle_caf_call_stmts(
    WN_TREE_CONTAINER<PRE_ORDER>::iterator
    wipre, WN **wn_next_p)
{
    ST *func_st;
    WN *wn_next, *wn;

    wn_next = *wn_next_p;
    wn = wipre.Wn();

    func_st = WN_st(wn);

    if ( NAME_IS(func_st, "_THIS_IMAGE0") ) {
        /* IR looks like:
         *   wn:          CALL _THIS_IMAGE0
         *   WN_next(wn): STID t$n
         *                   LDID .preg_return_val
         * We replace with:
         *   WN_next(wn): STID t$n
         *                   LDID _this_image
         */
        wn_next = WN_next(wn);
        wipre.Delete();
        Is_True( WN_operator(wn_next) == OPR_STID,
                ("Expected STID in IR after _THIS_IMAGE0() call"));
        WN_Delete( WN_kid0(wn_next) );
        WN_kid0(wn_next) = WN_Ldid(MTYPE_U8, 0,
                this_image_st, ST_type(this_image_st));
    } else if ( NAME_IS(func_st, "_NUM_IMAGES") ) {
        wn_next = WN_next(wn);
        wipre.Delete();
        Is_True( WN_operator(wn_next) == OPR_STID,
                ("Expected STID in IR after _num_images() call"));
        WN_Delete( WN_kid0(wn_next) );
        WN_kid0(wn_next) = WN_Ldid(MTYPE_U8, 0,
                num_images_st, ST_type(num_images_st));
    } else if ( NAME_IS(func_st, "_SYNC_IMAGES") ) {
        wn_next = WN_next(wn);

        Is_True( WN_num_actuals(wn) == 2,
                ("Expected 2 args for sync_images from FE"));

        ST *st1 = WN_st(WN_kid0(WN_actual(wn,0)));
        TY_IDX ty1 = ST_type(st1);
        if (TY_kind(ty1) == KIND_POINTER)
            ty1 = TY_pointed(ty1);
        Is_True( is_dope(ty1),
                ("Expected sync_images arg 1 to be a dope from FE"));

        /* args: DV, 1 -> array_list, #array_list */
        WN_Delete(WN_kid0(WN_actual(wn,0)));
        WN_Delete(WN_kid0(WN_actual(wn,1)));
        WN_kid0(WN_actual(wn,0)) =
            WN_Ldid(Pointer_type, 0 /* DV_BASE_PTR ofst */,
                    st1, MTYPE_To_TY(Pointer_type));
        if (TY_size(ty1) > ( Pointer_Size == 4?
                    dopevec_fldinfo[DV_DIM1_LB].ofst32 :
                    dopevec_fldinfo[DV_DIM1_LB].ofst64)) {
            WN_kid0(WN_actual(wn,1)) =
                WN_Ldid(Integer_type,
                        Pointer_Size == 4 ?
                        dopevec_fldinfo[DV_DIM1_EXT].ofst32 :
                        dopevec_fldinfo[DV_DIM1_EXT].ofst64,
                        WN_st(WN_kid0(WN_actual(wn,0))),
                        MTYPE_To_TY(Integer_type));
        } else {
            WN_kid0(WN_actual(wn,1)) =
                WN_Intconst(Integer_type, 1);
        }
    }
}

static void
dump_ty (TY_IDX ty_idx)
{
	TY& ty = Ty_Table[ty_idx];
	ty.Print(stdout);
}

/*
 * expr_is_coindexed
 *
 * input: expr - a WHIRL expression tree
 * output:
 *        image - WHIRL node for selected image
 *        coarray_type - type for coarray, if found
 *        direct_coarray_ref (optional) - the ARRAY/ARRSECTION
 *                            node for the coarray (symbol at
 *                            kid0 will be the coarray symbol)
 *        return value: coindexed array expression within input parameter
 *                      expr
 */
static WN *expr_is_coindexed(WN *expr, WN **image, TY_IDX *coarray_type,
                             WN **direct_coarray_ref)
{
    WN_TREE_CONTAINER<POST_ORDER> wcpost(expr);
    WN *wn;
    TY_IDX type;
    WN_OFFSET ofst = 0;
    WN *img = NULL;

    *image = NULL;
    *coarray_type = TY_IDX_ZERO;

    /* start from left most leaf and search up the tree for a coarray access
     * */
    wn = wcpost.begin().Wn();
    type = TY_IDX_ZERO;
    while (wn) {
        WN *parent = Get_Parent(wn);

        switch (WN_operator(wn)) {
            case OPR_ARRAY:
            case OPR_ARRSECTION:
                if (type == TY_IDX_ZERO)
                    break;

                //printf("(A) Before Type:\n");
                //dump_ty(type);
                type = get_type_at_offset(type, ofst, FALSE, TRUE);

                while (TY_kind(type) == KIND_POINTER)
                    type = TY_pointed(type);

                //printf("(A) After Type (%d):\n", ofst);
                //dump_ty(type);

                if ( is_coarray_type(type) ) {
                    *image = array_ref_is_coindexed(wn, type);
                    /* TODO: double check that we want to return if wn is not
                     * coindexed */
                    if (*image) {
                        WN *enclosing_ref = get_enclosing_direct_arr_ref(wn);
                        *coarray_type = type;
                        if (direct_coarray_ref != NULL)
                            *direct_coarray_ref = wn;
                        return enclosing_ref;
                    } else {
                        return NULL;
                    }
                }

                break;

            case OPR_LDA:
            case OPR_LDID:
                type = WN_ty(wn);
                while (TY_kind(type) == KIND_POINTER)
                    type = TY_pointed(type);
                ofst += WN_offset(wn);

                break;

            case OPR_ILOAD:
                //printf("(I) Before Type:\n");
                //dump_ty(type);
                type = get_type_at_offset(type, ofst, FALSE, FALSE);
                while (TY_kind(type) == KIND_POINTER)
                    type = TY_pointed(type);
                //printf("(I) After Type (%d):\n", ofst);
                //dump_ty(type);

                ofst = WN_offset(wn);

                break;

            case OPR_ADD:
                ofst += WN_const_val(WN_kid1(wn));
                break;
        }
        /* if back at original expression node, break */
        if (wn == expr) break;

        wn = parent;
    }


    return NULL;
}

static void Parentize(WN *wn)
{
  if (!OPCODE_is_leaf (WN_opcode (wn))) {
    if (WN_opcode(wn) == OPC_BLOCK) {
      WN* kid = WN_first (wn);
      while (kid) {
        Set_Parent (kid, wn);
        Parentize (kid);
        kid = WN_next (kid);
      }
    }
    else {
      INT kidno;
      WN* kid;
      for (kidno=0; kidno<WN_kid_count(wn); kidno++) {
        kid = WN_kid (wn, kidno);
        if (kid) {
          Set_Parent (kid, wn);
          Parentize (kid);
        }
      }
    }
  }
}

static void Coarray_Prelower_Init()
{
    MEM_POOL_Initialize(&caf_pool, "CAF Prelowering Pool", FALSE);
}

static TY_IDX get_type_at_offset (TY_IDX ty, WN_OFFSET offset, BOOL stop_at_array,
                                  BOOL stop_at_coarray)
{
  switch (TY_kind(ty)) {
    case KIND_STRUCT:
      {
        // return type of field
        FLD_ITER fld_iter = Make_fld_iter(TY_fld(ty));
        do {
          FLD_HANDLE fld(fld_iter);
          if (Is_Composite_Type(FLD_type(fld))
              && offset > FLD_ofst(fld)
              && offset < FLD_ofst(fld) + TY_size(FLD_type(fld)))
          {
            return get_type_at_offset (FLD_type(fld), offset - FLD_ofst(fld));
          }
          if (FLD_ofst(fld) == offset)
            return FLD_type(fld);
        } while (!FLD_last_field(fld_iter++));
        FmtAssert(FALSE, ("couldn't find matching field"));
      }
    case KIND_ARRAY:
      // return type of elements, recursing in case array of structs
      if (stop_at_coarray && is_coarray_type(ty)) {
          return ty;
      } else if (stop_at_array) {
          return ty;
      } else  {
          return get_type_at_offset (TY_etype(ty),
                  offset % TY_size(TY_etype(ty)), FALSE);
      }
    default:
      return ty;
  }
}

static WN *find_outer_array(WN *start, WN *end)
{
    WN *wn;
    wn = start;
    while (wn != end) {
        if (WN_operator(wn) == OPR_ARRAY || WN_operator(wn) == OPR_ARRSECTION)
            return wn;

        wn = Get_Parent(wn);
    }

    if (WN_operator(wn) == OPR_ARRAY || WN_operator(wn) == OPR_ARRSECTION)
        return wn;
    else
        return NULL;
}

/*
 * array_ref_on_LHS
 *
 * Input: wn, an ARRAY or ARRSECTION node.
 * Output: return value says if the input wn is on LHS of its parent
 * which is an assignment statement.
 */
static BOOL array_ref_on_LHS(WN *wn, TY_IDX *ty)
{
    WN *parent;
    Is_True(WN_operator(wn) != OPR_ARRAY ||
            WN_operator(wn) != OPR_ARRSECTION,
            ("Unexpected operator for array_ref_on_LHS arg"));

    parent = Get_Parent(wn);

    if (WN_operator(parent) == OPR_ISTORE) {
        TYPE_ID desc = WN_desc(parent);
        if (desc == MTYPE_M)
            *ty = TY_IDX_ZERO;
        else
            *ty = MTYPE_To_TY(desc);
        return TRUE;
    } else if (WN_operator(parent) == OPR_MSTORE) {
        *ty = TY_IDX_ZERO;
        return TRUE;
    } else if (WN_operator(parent) == OPR_ARRAYEXP) {
        WN *node = Get_Parent(parent);
        if (WN_operator(node) == OPR_ISTORE) {
            TYPE_ID desc = WN_desc(node);
            if (desc == MTYPE_M)
                *ty = TY_IDX_ZERO;
            else
                *ty = MTYPE_To_TY(desc);
        } else if (WN_operator(node) == OPR_MSTORE) {
            *ty = TY_IDX_ZERO;
        }
        return TRUE;
    }

    return FALSE;
}

/*
 * array_ref_on_RHS
 *
 * Input: wn, an ARRAY or ARRSECTION node.
 * Output: return value says if the input wn is on RHS of its parent
 * statement.
 */
static BOOL array_ref_on_RHS(WN *wn, TY_IDX *ty)
{
    WN *parent;
    Is_True(WN_operator(wn) != OPR_ARRAY ||
            WN_operator(wn) != OPR_ARRSECTION,
            ("Unexpected operator for array_ref_on_RHS arg"));

    parent = Get_Parent(wn);

    if (WN_operator(parent) == OPR_ILOAD) {
        TYPE_ID desc = WN_desc(parent);
        if (desc == MTYPE_M)
            *ty = TY_IDX_ZERO;
        else
            *ty = MTYPE_To_TY(desc);
        return TRUE;
    } else if (WN_operator(parent) == OPR_MLOAD) {
        *ty = TY_IDX_ZERO;
        return TRUE;
    }

    return FALSE;
}

/*
 * array_ref_in_parm
 *
 * Input: wn, an ARRAY or ARRSECTION node.
 * Output: return value says if the input wn is in a PARM node of its parent
 * statement.
 */
static BOOL array_ref_in_parm(WN *wn)
{
    WN *parent;
    Is_True(WN_operator(wn) != OPR_ARRAY ||
            WN_operator(wn) != OPR_ARRSECTION,
            ("Unexpected operator for array_ref_in_parm arg"));

    parent = Get_Parent(wn);

    return WN_operator(parent) == OPR_PARM;
}

/*
 * is_lvalue
 *
 * Input: wn, a whirl expression node
 * Output: return value says if the input wn is an lvalue (i.e. has an
 * address)
 */
static BOOL is_lvalue(WN *expr)
{
    BOOL ret;

    if (WN_operator(expr) == OPR_CVT) {
        ret = is_lvalue(WN_kid0(expr));
    } else {
        ret = (WN_operator(expr) == OPR_ARRAYEXP &&
                (WN_operator(WN_kid0(expr)) == OPR_MLOAD ||
                 WN_operator(WN_kid0(expr)) == OPR_ILOAD)) ||
            WN_operator(expr) == OPR_LDID  ||
            WN_operator(expr) == OPR_ILOAD ||
            WN_operator(expr) == OPR_MLOAD;
    }

    return ret;
}

/*
 * get_transfer_size
 *
 * Input: arr_ref, WN for an array reference
 *        elem_type, type for base element
 * Output: returns the size in bytes for the array reference arr_ref
 */
static WN* get_transfer_size(WN *arr_ref, TY_IDX elem_type)
{
    WN *size_wn = WN_Intconst(MTYPE_U8,
                              TY_size( elem_type) );

    if (WN_operator(arr_ref) == OPR_ARRSECTION) {
        WN *arrayexp = get_containing_arrayexp(arr_ref);

        if (arrayexp != NULL) {
            /* case 1 */
            for (INT8 i = 1; i < WN_kid_count(arrayexp); i++) {
                size_wn = WN_Mpy(MTYPE_U8, WN_COPY_Tree(WN_kid(arrayexp,i)),
                                 size_wn);
            }
        } else {
            /* case 2 */

            /* no ARRAYEXP found. So determine full size of its kids */
            INT8 rank = (WN_kid_count(arr_ref) - 1) / 2;
            for (INT8 i = 0; i < rank; i++) {
                WN* sub = WN_array_index(arr_ref, i);

                if (WN_operator(sub) == OPR_TRIPLET) {
                    /* multiply by number of values in progression */
                    size_wn = WN_Mpy(MTYPE_U8, WN_COPY_Tree(WN_kid(sub,2)),
                            size_wn);
                } else if (WN_operator(sub) == OPR_ARRAYEXP) {
                    /* multiply by size of first (and only) dimension */
                    size_wn = WN_Mpy(MTYPE_U8, WN_COPY_Tree(WN_kid(sub,1)),
                            size_wn);
                }
            }
        }
    } else if (WN_operator(arr_ref) == OPR_ARRAYEXP) {
        /* case 3 */
        for (INT8 i = 1; i < WN_kid_count(arr_ref); i++) {
            size_wn = WN_Mpy(MTYPE_U8, WN_COPY_Tree(WN_kid(arr_ref,i)),
                             size_wn);
        }
    } else if (WN_operator(arr_ref) == OPR_ARRAY) {
        WN *arrayexp = get_containing_arrayexp(arr_ref);

        if (arrayexp != NULL) {
            /* case 4 */
            for (INT8 i = 1; i < WN_kid_count(arrayexp); i++) {
                size_wn = WN_Mpy(MTYPE_U8, WN_COPY_Tree(WN_kid(arrayexp,i)),
                                 size_wn);
            }
        }
    }

    return size_wn;
}

/*
 * replace_RHS_with_LCB
 *
 * Input: stmt_node, an assignment statement of the form LHS = RHS
 *        LCB_st, a symbol for a generated LCB (local communication buffer)
 *
 * Action:
 *    This routine will substitute the LCB into RHS of assignment statement,
 *    so that it becomes:
 *          LHS = LCB(...)
 */
static void replace_RHS_with_LCB( WN *stmt_node, ST *LCB_st)
{
    WN *old_RHS;
    WN *new_RHS;
    WN *new_RHS_addr;
    TY_IDX elem_type;
    INT elem_size;
    TYPE_ID desc, rtype;

    Is_True(WN_operator(stmt_node) == OPR_ISTORE,
           ("Unexpected operator for replace_RHS_with_LCB"));

    elem_type = TY_pointed(ST_type(LCB_st));
    elem_size = TY_size(elem_type);

    /* resolve to a base element type */
    while ( TY_kind(elem_type) == KIND_ARRAY ) {
        elem_type = TY_etype(elem_type);
        elem_size = TY_size(elem_type);
        if (TY_kind(elem_type) == KIND_STRUCT) {
            elem_type = MTYPE_To_TY(MTYPE_U1);
            break;
        }
    }

    rtype = TY_mtype(elem_type);
    desc = TY_mtype(elem_type);
    if (desc == MTYPE_I1 || desc == MTYPE_I2)
        rtype = MTYPE_I4;
    else if (desc == MTYPE_U1 || desc == MTYPE_U2)
        rtype = MTYPE_U4;

    old_RHS = WN_kid0(stmt_node);

    /* check if RHS or LHS is an array expression, otherwise its just a simple
     * indirect load of the LCB pointer */
    if (WN_operator(old_RHS) == OPR_ARRAYEXP) {
        WN *LHS_arrayexp = WN_kid1(stmt_node);
        WN *LHS_arrsection = WN_kid0(LHS_arrayexp);
        INT LHS_section_ndim, LHS_corank;

        new_RHS_addr = WN_Create( OPCODE_make_op(OPR_ARRSECTION,
                    WN_rtype(LHS_arrsection),
                    WN_desc(LHS_arrsection)),
                    1+2*(WN_kid_count(old_RHS)-1));

        /* use elem_size since WN_element_size may not be true element size
         * when original array ref was non-contiguous (e.g. for assume shape
         * array of 8-byte integers, WN_element_size will be -4)
         */
        WN_element_size(new_RHS_addr) = elem_size;
            /* abs(WN_element_size(LHS_arrsection)); */

        WN_array_base(new_RHS_addr) =
            WN_CreateLdid(OPR_LDID, TY_mtype(ST_type(LCB_st)),
                        TY_mtype(ST_type(LCB_st)), 0, LCB_st,
                        ST_type(LCB_st));

        INT ndim = WN_kid_count(old_RHS) - 1;
        for (INT8 i = 1; i <= ndim; i++) {
            WN_array_dim(new_RHS_addr, i-1) =
                WN_COPY_Tree(WN_kid(old_RHS,i));

            OPCODE opc_triplet =
                Pointer_Size == 4 ? OPC_I4TRIPLET : OPC_I8TRIPLET;
            WN_array_index(new_RHS_addr, i-1) =
                WN_Create(opc_triplet, 3);
            /* lb */
            WN_kid0(WN_array_index(new_RHS_addr,i-1)) =
                WN_Intconst(Integer_type, 0);
            /* extent */
            WN_kid2(WN_array_index(new_RHS_addr,i-1)) =
                WN_COPY_Tree(WN_kid(old_RHS,i));
            /* stride */
            WN_kid1(WN_array_index(new_RHS_addr,i-1)) =
                WN_Intconst(Integer_type, 1);
        }
    } else if (WN_operator(stmt_node) == OPR_ISTORE &&
           WN_operator(WN_kid1(stmt_node)) == OPR_ARRAYEXP) {
        /* LHS is an array expression, so RHS should be one also */
        WN *LHS_arrayexp = WN_kid1(stmt_node);
        WN *LHS_arrsection = WN_kid0(LHS_arrayexp);
        INT LHS_section_ndim, LHS_corank;

        new_RHS_addr = WN_Create( OPCODE_make_op(OPR_ARRSECTION,
                    WN_rtype(LHS_arrsection),
                    WN_desc(LHS_arrsection)),
                    1+2*(WN_kid_count(LHS_arrayexp)-1));

        /* use elem_size since WN_element_size may not be true element size
         * when original array ref was non-contiguous (e.g. for assume shape
         * array of 8-byte integers, WN_element_size will be -4)
         */
        WN_element_size(new_RHS_addr) = elem_size;
           /* abs(WN_element_size(LHS_arrsection)); */

        WN_array_base(new_RHS_addr) =
            WN_CreateLdid(OPR_LDID, TY_mtype(ST_type(LCB_st)),
                        TY_mtype(ST_type(LCB_st)), 0, LCB_st,
                        ST_type(LCB_st));

        INT ndim = WN_kid_count(LHS_arrayexp)-1;
        for (INT8 i = 1; i <= ndim; i++) {
            WN_array_dim(new_RHS_addr, i-1) =
                WN_COPY_Tree(WN_kid(LHS_arrayexp,i));

            OPCODE opc_triplet =
                Pointer_Size == 4 ? OPC_I4TRIPLET : OPC_I8TRIPLET;
            WN_array_index(new_RHS_addr, i-1) =
                WN_Create(opc_triplet, 3);
            /* lb */
            WN_kid0(WN_array_index(new_RHS_addr,i-1)) =
                WN_Intconst(Integer_type, 0);
            /* extent */
            WN_kid2(WN_array_index(new_RHS_addr,i-1)) =
                WN_COPY_Tree(WN_kid(LHS_arrayexp,i));
            /* stride */
            WN_kid1(WN_array_index(new_RHS_addr,i-1)) =
                WN_Intconst(Integer_type, 1);
        }
    } else {
        new_RHS_addr =
            WN_CreateLdid(OPR_LDID, TY_mtype(ST_type(LCB_st)),
                        TY_mtype(ST_type(LCB_st)), 0, LCB_st,
                        ST_type(LCB_st));
    }

    switch (desc) {
        case MTYPE_I1:
            new_RHS_addr = WN_CreateCvtl(OPC_I4CVTL, 8, new_RHS_addr);
            break;
        case MTYPE_I2:
            new_RHS_addr = WN_CreateCvtl(OPC_I4CVTL, 16, new_RHS_addr);
            break;
        case MTYPE_U1:
            new_RHS_addr = WN_CreateCvtl(OPC_U4CVTL, 8, new_RHS_addr);
            break;
        case MTYPE_U2:
            new_RHS_addr = WN_CreateCvtl(OPC_U4CVTL, 16, new_RHS_addr);
            break;
    }

    new_RHS = WN_CreateIload(OPR_ILOAD, rtype, desc, 0,
            elem_type, ST_type(LCB_st), new_RHS_addr);

    WN_Delete(old_RHS);

    WN_kid0(stmt_node) = F90_Wrap_ARREXP(new_RHS);
}

/*
 * get_lcb_assignment
 *
 * Input: coarray_ref, a remote coarray reference
 *
 * Action:
 *    This routine will generate an assignment statement of the form:
 *         LCB(...) = coarray_ref(...)
 */
static WN *get_lcb_assignment(WN *coarray_deref, TY_IDX coarray_type,
                              ST *LCB_st)
{
    WN *new_stmt;
    WN *addr = NULL;
    TY_IDX elem_type;
    INT elem_size;
    TYPE_ID desc, rtype;
    WN *coarray_ref = WN_kid0(coarray_deref);

    Is_True(WN_operator(coarray_ref) == OPR_ARRSECTION ||
            WN_operator(coarray_ref) == OPR_ARRAY,
           ("Unexpected operator for get_lcb_assignment"));

    elem_type = TY_pointed(ST_type(LCB_st));
    elem_size = TY_size(elem_type);

    /* resolve to a base element type */
    while ( TY_kind(elem_type) == KIND_ARRAY ) {
        elem_type = TY_etype(elem_type);
        elem_size = TY_size(elem_type);
        if (TY_kind(elem_type) == KIND_STRUCT) {
            elem_type = MTYPE_To_TY(MTYPE_U1);
            break;
        }
    }

    rtype = TY_mtype(elem_type);
    desc = TY_mtype(elem_type);
    if (desc == MTYPE_I1 || desc == MTYPE_I2)
        rtype = MTYPE_I4;
    else if (desc == MTYPE_U1 || desc == MTYPE_U2)
        rtype = MTYPE_U4;

    if (WN_operator(coarray_ref) == OPR_ARRSECTION) {
        WN *new_arrsection;
        INT ndim, section_ndim;
        INT corank = get_coarray_corank(coarray_type);
        WN *new_arrexp = F90_Wrap_ARREXP(WN_COPY_Tree(coarray_ref));

        section_ndim = (WN_kid_count(coarray_ref) - 1) / 2 - corank;
        ndim = WN_kid_count(new_arrexp) - 1;

        new_arrsection = WN_Create( OPCODE_make_op(OPR_ARRSECTION,
                             WN_rtype(coarray_ref),
                             WN_desc(coarray_ref)), 1+2*ndim);

        /* use elem_size since WN_element_size may not be true element size
         * when original array ref was non-contiguous (e.g. for assume shape
         * array of 8-byte integers, WN_element_size will be -4)
         */
        WN_element_size(new_arrsection) = elem_size;
          /*  abs(WN_element_size(coarray_ref)); */

        WN_array_base(new_arrsection) =
            WN_CreateLdid(OPR_LDID, TY_mtype(ST_type(LCB_st)),
                        TY_mtype(ST_type(LCB_st)), 0, LCB_st,
                        ST_type(LCB_st));

        INT8 j = 1;
        for (INT8 i = 1; i <= section_ndim; i++) {
            WN *orig_index = WN_array_index(coarray_ref, corank+i-1);
            if ( (WN_operator(orig_index) == OPR_TRIPLET) ||
                 (WN_operator(orig_index) == OPR_ARRAYEXP) )  {
              WN_array_dim(new_arrsection, j-1) =
                  WN_COPY_Tree(WN_kid(new_arrexp, j));

              WN_array_index(new_arrsection, j-1) =
                  WN_Create( OPCODE_make_op(OPR_TRIPLET,
                    WN_rtype(orig_index), WN_desc(orig_index)), 3);
              /* lb */
              WN_kid0(WN_array_index(new_arrsection,j-1)) =
                  WN_Intconst(Integer_type, 0);
              /* extent */
              WN_kid2(WN_array_index(new_arrsection,j-1)) =
                  WN_COPY_Tree(WN_kid(new_arrexp, j));
              /* stride */
              WN_kid1(WN_array_index(new_arrsection,j-1)) =
                  WN_Intconst(Integer_type, 1);
              j++;
            }
        }

        /* replace old ARRSECTION node with new one  */
        WN_Delete(WN_kid0(new_arrexp));
        WN_kid0(new_arrexp) = new_arrsection;
        addr = new_arrexp;
    } else {
        addr =
            WN_CreateLdid(OPR_LDID, TY_mtype(ST_type(LCB_st)),
                        TY_mtype(ST_type(LCB_st)), 0, LCB_st,
                        ST_type(LCB_st));
    }

    switch (desc) {
        case MTYPE_I1:
            addr = WN_CreateCvtl(OPC_I4CVTL, 8, addr);
            break;
        case MTYPE_I2:
            addr = WN_CreateCvtl(OPC_I4CVTL, 16, addr);
            break;
        case MTYPE_U1:
            addr = WN_CreateCvtl(OPC_U4CVTL, 8, addr);
            break;
        case MTYPE_U2:
            addr = WN_CreateCvtl(OPC_U4CVTL, 16, addr);
            break;
    }

    WN *val = F90_Wrap_ARREXP(coarray_deref);
    TYPE_ID val_desc = WN_rtype(val);

    if (val_desc != rtype) {
        val = WN_Cvt(val_desc, rtype, val);
    }

    new_stmt = WN_Istore(rtype, 0, ST_type(LCB_st),
                       addr, val);

    Set_LCB_Stmt(new_stmt);

    return new_stmt;
}

/*
 * get_lcb_sym
 *
 * input: access, WHIRL node for a memory access. It is assumed that the base
 * address for the access is given by an LCB pointer
 *
 * output: return the LCB sym
 */
static ST *get_lcb_sym(WN *access)
{
    ST *LCB_st;
    WN *base;
    Is_True( access, ("get_lcb_sym called for NULL access"));
    if (access == NULL) return NULL;

    base = access;
    while (WN_kid0(base) != NULL)
        base = WN_kid0(base);

    LCB_st = WN_st(base);

    if (! ST_is_lcb_ptr(LCB_st) )
        return NULL;
    else
        return LCB_st;
}

/*
 * get_containing_arrayexp
 *
 * input: wn, a WHIRL node
 * output: containing array expression node
 *
 * action: search parent until we find a node with operator OPR_ARRAYEXP
 */
static WN *get_containing_arrayexp(WN *wn)
{
    WN *parent;

    parent = Get_Parent(wn);
    while (parent && WN_operator(parent) != OPR_ARRAYEXP)
        parent = Get_Parent(parent);

    return parent;
}

/*
 * get_assign_stmt_datatype
 *
 * input: a stmt WHIRL node
 * output: the type of the object being assigned
 *
 */
static TY_IDX get_assign_stmt_datatype(WN *stmt)
{
    if (WN_desc(stmt) != MTYPE_M &&
        (WN_operator(stmt) == OPR_STID ||
        WN_operator(stmt) == OPR_ISTORE)) {
        return MTYPE_To_TY(WN_desc(stmt));
    } else if (WN_operator(stmt) == OPR_ISTORE) {
        return TY_pointed(WN_ty(stmt));
    } else if ( WN_operator(stmt) == OPR_STID ||
                WN_operator(stmt) == OPR_MSTORE) {
        return WN_ty(stmt);
    }
    return TY_IDX_ZERO;
}

static WN* get_load_parent(WN *start)
{
    WN *load = start;
    while (load &&
            WN_operator(load) != OPR_LDID  &&
            WN_operator(load) != OPR_ILOAD &&
            WN_operator(load) != OPR_MLOAD) {
        load = Get_Parent(load);
    }

    return load;
}

static WN* get_store_parent(WN *start)
{
    WN *store = start;
    while (store &&
            WN_operator(store) != OPR_STID  &&
            WN_operator(store) != OPR_ISTORE) {
        store = Get_Parent(store);
    }

    return store;
}

/*
 * get_enclosing_direct_arr_ref
 *
 * input: an array reference node
 * output: enclosing array node that does not contain any indirection (i.e. no
 * ILOAD or MLOAD.
 */
static WN* get_enclosing_direct_arr_ref(WN *arr)
{
    WN *enclosing_arr;
    Is_True(WN_operator(arr) == OPR_ARRAY ||
            WN_operator(arr) == OPR_ARRSECTION,
          ("expected an array reference whirl node"));

    enclosing_arr = arr;

    WN *p = Get_Parent(enclosing_arr);
    while (p && !OPCODE_is_stmt(WN_opcode(p)) && WN_operator(p) != OPR_ILOAD &&
          WN_operator(p) != OPR_MLOAD) {
        if (WN_operator(p) == OPR_ARRAY || WN_operator(p) == OPR_ARRSECTION)
            enclosing_arr = p;
        p = Get_Parent(p);
    }

    return enclosing_arr;
}

/*
 * get_array_type_from_tree
 *
 * input: an WHIRL tree that is OPR_ARRAY or OPR_ARRSECTION
 * output: the TY_IDX for the array type accessed
 *
 * Action: start at left-most leaf
 */
static TY_IDX get_array_type_from_tree(WN *tree)
{
    WN_TREE_CONTAINER<POST_ORDER> wcpost(tree);
    WN *wn;
    TY_IDX type;
    WN_OFFSET ofst = 0;
    WN *img = NULL;

    /* start from left most leaf and search up the tree for a coarray access
     * */
    wn = wcpost.begin().Wn();
    type = TY_IDX_ZERO;
    while (wn) {
        WN *parent = Get_Parent(wn);

        switch (WN_operator(wn)) {
            case OPR_ARRAY:
            case OPR_ARRSECTION:
                if (type == TY_IDX_ZERO)
                    break;

                //printf("(A) Before Type:\n");
                //dump_ty(type);

                if (wn == tree)
                    type = get_type_at_offset(type, ofst, TRUE, TRUE);
                else
                    type = get_type_at_offset(type, ofst, FALSE, FALSE);

                while (TY_kind(type) == KIND_POINTER)
                    type = TY_pointed(type);

                //printf("(A) After Type (%d):\n", ofst);
                //dump_ty(type);

                if (wn == tree) return type;

                break;

            case OPR_LDA:
            case OPR_LDID:
                type = WN_ty(wn);
                ofst += WN_offset(wn);

                break;

            case OPR_ILOAD:
                //printf("(I) Before Type:\n");
                //dump_ty(type);
                type = get_type_at_offset(type, ofst, FALSE, FALSE);
                while (TY_kind(type) == KIND_POINTER)
                    type = TY_pointed(type);
                //printf("(I) After Type (%d):\n", ofst);
                //dump_ty(type);

                ofst = WN_offset(wn);

                break;

            case OPR_ADD:
                ofst += WN_const_val(WN_kid1(wn));
                break;
        }

        wn = parent;
    }

    /* should not reach */

    return TY_IDX_ZERO;
}


/* Fortran is column-major, meaning array subscripts in Fortran should be
 * ordered from the opposite end.
 */
static inline WN* WN_array_subscript(WN *array, INT8 i)
{
    return WN_array_index( array, WN_num_dim(array)-i-1 );
}

static int subscript_is_scalar(WN *array, INT8 i)
{
    WN *subscript = WN_array_subscript(array, i);

    return ( WN_operator(subscript) != OPR_TRIPLET &&
             WN_operator(subscript) != OPR_ARRAYEXP );
}

static int subscript_is_strided(WN *array, INT8 i)
{
    int is_strided = 0;
    WN *subscript = WN_array_subscript(array, i);

    if (WN_operator(subscript) == OPR_TRIPLET) {
        if (WN_operator(WN_kid1(subscript)) != OPR_INTCONST ||
            WN_const_val(WN_kid1(subscript)) != 1) {
            is_strided = 1;
        }
    } else if (WN_operator(subscript) == OPR_ARRAYEXP) {
        is_strided = 1;
    }

    return is_strided;
}

static WN* subscript_extent(WN *array, INT8 i)
{
    WN *extent;
    WN* subscript = WN_array_subscript(array, i);

    if (WN_operator(subscript) == OPR_TRIPLET) {
        extent = WN_COPY_Tree(WN_kid2(subscript));
    } else if (WN_operator(subscript) == OPR_ARRAYEXP) {
        /* TODO: not handled currently */
        extent = NULL;
    } else {
        extent = WN_COPY_Tree(subscript);
    }

    return extent;
}

static WN* subscript_stride(WN *array, INT8 i)
{
    WN *stride;
    WN* subscript = WN_array_subscript(array, i);

    if (WN_operator(subscript) == OPR_TRIPLET) {
        stride = WN_COPY_Tree(WN_kid1(subscript));
    } else if (WN_operator(subscript) == OPR_ARRAYEXP) {
        /* TODO: not handled currently */
        stride = WN_Intconst(Integer_type, 1);
    } else {
        stride = WN_Intconst(Integer_type, 1);
    }

    return stride;
}

static WN* gen_array1_ref( OPCODE op_array, TY_IDX array_type,
                               ST *array_st, INT8 index, INT8 dim)
{
    WN *array = WN_Create( op_array, 3 );
    WN_element_size(array) = TY_size(TY_AR_etype(array_type));
    WN_array_base(array) = WN_Lda(Pointer_type, 0, array_st);
    WN_array_index(array,0) = WN_Intconst(MTYPE_U8, index);
    WN_array_dim(array,0) = WN_Intconst(MTYPE_U8, dim);

    return array;
}

static void gen_save_coarray_symbol(ST *sym)
{
    Is_True( ST_sclass(sym) == SCLASS_PSTATIC,
            ("sym storage class should be SCLASS_PSTATIC"));

    char *new_sym_str = (char *) alloca(strlen("__SAVE_COARRAY_") +
                strlen(ST_name(sym)) + strlen(ST_name(Get_Current_PU_ST()))
                + 20);

    sprintf( new_sym_str, "__SAVE_COARRAY_%s_%s_%lu",
            ST_name(Get_Current_PU_ST()), ST_name(sym),
            (unsigned long) TY_size(ST_type(sym)));

    /* make symbol name a legal variable identifier */
    char *s = new_sym_str;
    for (int i = 0; i < strlen(new_sym_str); i++) {
        if (s[i] == '.') s[i] = '_';
    }

    ST *new_sym = New_ST( GLOBAL_SYMTAB );
    ST_Init( new_sym, Save_Str(new_sym_str), CLASS_VAR, SCLASS_EXTERN,
            EXPORT_PREEMPTIBLE, Make_Pointer_Type(ST_type(sym)) );

    save_coarray_symbol_map[sym] = new_sym;
}

static void gen_global_save_coarray_symbol(ST *sym)
{
    Is_True( ST_sclass(sym) == SCLASS_COMMON ||
             ST_sclass(sym) == SCLASS_DGLOBAL,
            ("sym storage class should be SCLASS_COMMON or SCLASS_DGLOBAL"));

    char *new_sym_str = (char *) alloca(strlen("__SAVE_COARRAY_") +
            strlen(ST_name(sym)) + strlen(ST_name(ST_base(sym)))
            + 20);

    sprintf( new_sym_str, "__SAVE_COARRAY_%s_%s_%lu",
            ST_name(ST_base(sym)), ST_name(sym),
            (unsigned long) TY_size(ST_type(sym)));

    /* make symbol name a legal variable identifier */
    char *s = new_sym_str;
    for (int i = 0; i < strlen(new_sym_str); i++) {
        if (s[i] == '.') s[i] = '_';
    }

    ST *new_sym = New_ST( GLOBAL_SYMTAB );
    ST_Init( new_sym, Save_Str(new_sym_str), CLASS_VAR, SCLASS_EXTERN,
            EXPORT_PREEMPTIBLE, Make_Pointer_Type(ST_type(sym)) );

    common_save_coarray_symbol_map[sym] = new_sym;
}

/*
 * is_load_operation
 *
 * simply return TRUE if its an ldid, iload, or mload operation
 */
static BOOL is_load_operation(WN *node)
{
    if (WN_operator(node) == OPR_CVT)
        return is_load_operation(WN_kid0(node));

    return WN_operator(node) == OPR_LDID ||
           WN_operator(node) == OPR_ILOAD ||
           WN_operator(node) == OPR_MLOAD;
}

/*
 * is_convert_operation
 *
 * simply return TRUE if its a type conversion operation.
 */
static BOOL is_convert_operation(WN *node)
{
    return WN_operator(node) == OPR_CVT;
}
