/* Global headerfile of the OpenACC Testsuite */

/* This file was created with the accts_makeHeader.pl script using the following opions: */
/* -f=accts-c.conf -t=c  */


#ifndef ACC_TESTSUITE_H
#define ACC_TESTSUITE_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Version info                                           */
/**********************************************************/
#define ACCTS_VERSION "1.0a"


/* General                                                */
/**********************************************************/
#define LOOPCOUNT 	1000
#define REPETITIONS 50
/* following times are in seconds */
#define SLEEPTIME	 0.01
#define SLEEPTIME_LONG	 0.5


int test_acc_update_if(FILE * logfile);  /* Test for acc update device */
int crosstest_acc_update_if(FILE * logfile);  /* Crosstest for acc update device */
int test_acc_kernels_copyout(FILE * logfile);  /* Test for acc kernels copyout */
int crosstest_acc_kernels_copyout(FILE * logfile);  /* Crosstest for acc kernels copyout */
int test_acc_data_present(FILE * logfile);  /* Test for acc data present */
int crosstest_acc_data_present(FILE * logfile);  /* Crosstest for acc data present */
int test_ACC_DEVICE_NUM(FILE * logfile);  /* Test for ACC_DEVICE_NUM */
int crosstest_ACC_DEVICE_NUM(FILE * logfile);  /* Crosstest for ACC_DEVICE_NUM */
int test_acc_data_copyin(FILE * logfile);  /* Test for acc data copyin */
int crosstest_acc_data_copyin(FILE * logfile);  /* Crosstest for acc data copyin */
int test_acc_parallel_create(FILE * logfile);  /* Test for acc parallel copy */
int crosstest_acc_parallel_create(FILE * logfile);  /* Crosstest for acc parallel copy */
int test_acc_kernels_pcopyin(FILE * logfile);  /* Test for acc kernels pcopyin */
int crosstest_acc_kernels_pcopyin(FILE * logfile);  /* Crosstest for acc kernels pcopyin */
int test_acc_kernels_copyin(FILE * logfile);  /* Test for acc kernels copyin */
int crosstest_acc_kernels_copyin(FILE * logfile);  /* Crosstest for acc kernels copyin */
int test_acc_update_if(FILE * logfile);  /* Test for acc update if */
int crosstest_acc_update_if(FILE * logfile);  /* Crosstest for acc update if */
int test_acc_async_test(FILE * logfile);  /* Test for acc_async_test */
int crosstest_acc_async_test(FILE * logfile);  /* Crosstest for acc_async_test */
int test_acc_parallel_present(FILE * logfile);  /* Test for acc parallel present */
int crosstest_acc_parallel_present(FILE * logfile);  /* Crosstest for acc parallel present */
int test_acc_parallel_pcopyout(FILE * logfile);  /* Test for acc parallel pcopyout */
int crosstest_acc_parallel_pcopyout(FILE * logfile);  /* Crosstest for acc parallel pcopyout */
int test_acc_parallel_copy(FILE * logfile);  /* Test for acc parallel copy */
int crosstest_acc_parallel_copy(FILE * logfile);  /* Crosstest for acc parallel copy */
int test__OPENACC(FILE * logfile);  /* Test for _OPENACC */
int crosstest__OPENACC(FILE * logfile);  /* Crosstest for _OPENACC */
int test_acc_data_pcopy(FILE * logfile);  /* Test for acc data pcopy */
int crosstest_acc_data_pcopy(FILE * logfile);  /* Crosstest for acc data pcopy */
int test_acc_parallel_copyin(FILE * logfile);  /* Test for acc parallel copyin */
int crosstest_acc_parallel_copyin(FILE * logfile);  /* Crosstest for acc parallel copyin */
int test_acc_shutdown(FILE * logfile);  /* Test for acc_shutdown */
int crosstest_acc_shutdown(FILE * logfile);  /* Crosstest for acc_shutdown */
int test_acc_loop_collapse(FILE * logfile);  /* Test for acc loop collapse */
int crosstest_acc_loop_collapse(FILE * logfile);  /* Crosstest for acc loop collapse */
int test_acc_kernels_present(FILE * logfile);  /* Test for acc kernels present */
int crosstest_acc_kernels_present(FILE * logfile);  /* Crosstest for acc kernels present */
int test_acc_parallel_pcopyin(FILE * logfile);  /* Test for acc parallel pcopyin */
int crosstest_acc_parallel_pcopyin(FILE * logfile);  /* Crosstest for acc parallel pcopyin */
int test_acc_data_copy(FILE * logfile);  /* Test for acc data copy */
int crosstest_acc_data_copy(FILE * logfile);  /* Crosstest for acc data copy */
int test_acc_update_if(FILE * logfile);  /* Test for acc update device */
int crosstest_acc_update_if(FILE * logfile);  /* Crosstest for acc update device */
int test_acc_set_get_device_num(FILE * logfile);  /* Test for acc_set_get_device_num */
int crosstest_acc_set_get_device_num(FILE * logfile);  /* Crosstest for acc_set_get_device_num */
int test_acc_data_pcopyout(FILE * logfile);  /* Test for acc data pcopyout */
int crosstest_acc_data_pcopyout(FILE * logfile);  /* Crosstest for acc data pcopyout */
int test_acc_kernels_pcreate(FILE * logfile);  /* Test for acc_kernels_pcreate */
int crosstest_acc_kernels_pcreate(FILE * logfile);  /* Crosstest for acc_kernels_pcreate */
int test_acc_kernels_copy(FILE * logfile);  /* Test for acc kernels copy */
int crosstest_acc_kernels_copy(FILE * logfile);  /* Crosstest for acc kernels copy */
int test_test_acc_loop_seq (FILE * logfile);  /* Test for acc loop seq */
int crosstest_test_acc_loop_seq (FILE * logfile);  /* Crosstest for acc loop seq */
int test_acc_kernels_create(FILE * logfile);  /* Test for acc kernels create */
int crosstest_acc_kernels_create(FILE * logfile);  /* Crosstest for acc kernels create */
int test_acc_data_pcopyin(FILE * logfile);  /* Test for acc data pcopyin */
int crosstest_acc_data_pcopyin(FILE * logfile);  /* Crosstest for acc data pcopyin */
int test_acc_get_num_devices(FILE * logfile);  /* Test for acc_get_num_devices */
int crosstest_acc_get_num_devices(FILE * logfile);  /* Crosstest for acc_get_num_devices */
int test_acc_get_device_type(FILE * logfile);  /* Test for acc_get_device_type */
int crosstest_acc_get_device_type(FILE * logfile);  /* Crosstest for acc_get_device_type */
int test_acc_on_device(FILE * logfile);  /* Test for acc_on_device */
int crosstest_acc_on_device(FILE * logfile);  /* Crosstest for acc_on_device */
int test_acc_parallel_pcopy(FILE * logfile);  /* Test for acc parallel pcopy */
int crosstest_acc_parallel_pcopy(FILE * logfile);  /* Crosstest for acc parallel pcopy */
int test_acc_parallel_private(FILE * logfile);  /* Test for acc parallel private */
int crosstest_acc_parallel_private(FILE * logfile);  /* Crosstest for acc parallel private */
int test_acc_async_wait(FILE * logfile);  /* Test for acc_async_wait */
int crosstest_acc_async_wait(FILE * logfile);  /* Crosstest for acc_async_wait */
int test_acc_set_device_type(FILE * logfile);  /* Test for acc_set_device_type */
int crosstest_acc_set_device_type(FILE * logfile);  /* Crosstest for acc_set_device_type */
int test_acc_kernels_pcopyout(FILE * logfile);  /* Test for acc kernels pcopyout */
int crosstest_acc_kernels_pcopyout(FILE * logfile);  /* Crosstest for acc kernels pcopyout */
int test_acc_parallel_copyout(FILE * logfile);  /* Test for acc parallel copyout */
int crosstest_acc_parallel_copyout(FILE * logfile);  /* Crosstest for acc parallel copyout */
int test_acc_kernels_pcopy(FILE * logfile);  /* Test for acc kernels pcopy */
int crosstest_acc_kernels_pcopy(FILE * logfile);  /* Crosstest for acc kernels pcopy */
int test_acc_async_wait_all(FILE * logfile);  /* Test for acc_async_wait_all */
int crosstest_acc_async_wait_all(FILE * logfile);  /* Crosstest for acc_async_wait_all */
int test_acc_async_test_all(FILE * logfile);  /* Test for acc_async_test_all */
int crosstest_acc_async_test_all(FILE * logfile);  /* Crosstest for acc_async_test_all */
int test_acc_kernels_if(FILE * logfile);  /* Test for acc kernels if */
int crosstest_acc_kernels_if(FILE * logfile);  /* Crosstest for acc kernels if */
int test_test_acc_loop_reduction(FILE * logfile);  /* Test for acc loop reduction */
int crosstest_test_acc_loop_reduction(FILE * logfile);  /* Crosstest for acc loop reduction */
int test_acc_loop_independent(FILE * logfile);  /* Test for acc loop independent */
int crosstest_acc_loop_independent(FILE * logfile);  /* Crosstest for acc loop independent */
int test_acc_parallel_private(FILE * logfile);  /* Test for acc parallel private */
int crosstest_acc_parallel_private(FILE * logfile);  /* Crosstest for acc parallel private */
int test_acc_parallel_if(FILE * logfile);  /* Test for acc parallel if */
int crosstest_acc_parallel_if(FILE * logfile);  /* Crosstest for acc parallel if */
int test_acc_malloc(FILE * logfile);  /* Test for acc_malloc */
int crosstest_acc_malloc(FILE * logfile);  /* Crosstest for acc_malloc */
int test_ACC_DEVICE_TYPE(FILE * logfile);  /* Test for ACC_DEVICE_NUM */
int crosstest_ACC_DEVICE_TYPE(FILE * logfile);  /* Crosstest for ACC_DEVICE_NUM */
int test_acc_parallel_pcreate(FILE * logfile);  /* Test for acc parallel pcreate */
int crosstest_acc_parallel_pcreate(FILE * logfile);  /* Crosstest for acc parallel pcreate */
int test_acc_data_create(FILE * logfile);  /* Test for acc data create */
int crosstest_acc_data_create(FILE * logfile);  /* Crosstest for acc data create */
int test_acc_data_pcreate(FILE * logfile);  /* Test for acc data pcreate */
int crosstest_acc_data_pcreate(FILE * logfile);  /* Crosstest for acc data pcreate */
int test_acc_data_copyout(FILE * logfile);  /* Test for acc data copyout */
int crosstest_acc_data_copyout(FILE * logfile);  /* Crosstest for acc data copyout */
int test_acc_loop_private(FILE * logfile);  /* Test for acc loop private */
int crosstest_acc_loop_private(FILE * logfile);  /* Crosstest for acc loop private */
int test_test_acc_parallel_reduction(FILE * logfile);  /* Test for acc parallel reduction */
int crosstest_test_acc_parallel_reduction(FILE * logfile);  /* Crosstest for acc parallel reduction */

#endif
