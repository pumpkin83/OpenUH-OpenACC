/**
 * Author: Rengan Xu
 * University of Houston
 */

#ifndef __KERNEL_H__
#define __KERNEL_H__
#include "acc_common.h"
#include "acc_data.h"
#include "acc_context.h"

extern int gangs[3];
extern int vectors[3];

extern void __accr_set_gangs(int x, int y, int z);

extern void __accr_set_vectors(int x, int y, int z);

extern void __accr_set_default_gang_vector(void);

extern void __accr_reset_default_gang_vector(void);

extern void __accr_set_gang_num_x(int x);

extern void __accr_set_gang_num_y(int y);

extern void __accr_set_gang_num_z(int z);

extern void __accr_set_vector_num_x(int x);

extern void __accr_set_vector_num_y(int y);

extern void __accr_set_vector_num_z(int z);

extern void __accr_launchkernel(char* szKernelName, char* szKernelLib, int async_expr);

#endif
