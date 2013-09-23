/*********************************************************************
      This benchmark test program is measuring a cpu performance 
      of floating point operation and memory access speed.  
      
      Modification needed for testing turget computer!!
      Please adjust parameter : nn to take one minute to execute
      all calculation.  Original parameter set is for PC with 
      200 MHz MMX PENTIUM, whose score using this benchmark test
      is about 32.3 MFLOPS.

      If you have any question, please ask me via email.
      written by Ryutaro HIMENO, October 3, 1998.
      Version 2.0  
      ----------------------------------------------
         Ryutaro Himeno, Dr. of Eng.
         Head of Computer Information Center, 
         The Institute of Pysical and Chemical Research (RIKEN)
         Email : himeno@postman.riken.go.jp
      ---------------------------------------------------------------
      Version 2.1  June 30, 1999 
      Version 2.5  Nobenver 1, 2000

      You can adjust the size of this benchmark code to fit your target
      computer.  In that case, please chose following sets of 
      (mimax,mjmax,mkmax):
       small : 129,65,65
       midium: 257,129,129
       large : 513,257,257
       ext.large: 1025,513,513
      This program is to measure a computer performance in MFLOPS
      by using a kernel which appears in a linear solver of pressure 
      Poisson included in an incompressible Navier-Stokes solver. 
      A point-Jacobi method is employed in this solver.
      ------------------
      Finite-difference method, curvilinear coodinate system
      Vectorizable and parallelizable on each grid point
      No. of grid points : imax x jmax x kmax including boundaries
      ------------------
      A,B,C:coefficient matrix, wrk1: source term of Poisson equation
      wrk2 : working area, OMEGA : relaxation parameter
      BND:control variable for boundaries and objects ( = 0 or 1)
      P: pressure 
       -----------------
     Version 0.5
*********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"
#include "params.h"

#define NN               200

float jacobi(int, int);
void initmt(int);

static float  p[MIMAX][MJMAX][MKMAX];
static float  a[MIMAX][MJMAX][MKMAX][4],
              b[MIMAX][MJMAX][MKMAX][3],
              c[MIMAX][MJMAX][MKMAX][3];
static float  bnd[MIMAX][MJMAX][MKMAX];
static float  wrk1[MIMAX][MJMAX][MKMAX],
              wrk2[MIMAX][MJMAX][MKMAX];

static int imax, jmax, kmax;
static float omega;

int
main(int argc, char *argv[])
{
  int i, j, k, npe, myrank, blk, ist,left,right;
  float gosa;
  double cpu0, cpu1, nflop, xmflops2, score;

  MPI_Datatype  array_type;

  omega = 0.8;
  jmax = MJMAX-1;
  kmax = MKMAX-1;
  npe= NCPU;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &npe);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  if(npe!=1 && npe!=2 && npe!=4 && npe!=8 && npe!=16
     && npe!=32 && npe!=64){
    printf("Fatal Number of CPU\n");
    printf("2**n n= CPU\n");
    exit(1);
  }

  if(myrank==0)
    printf("%d node used. \n",npe);

  blk = (MI-1)/npe;

  left = myrank - 1;
  right = myrank + 1;

  if(myrank == 0){
    ist= 0;
    imax= blk + 1;
    left = MPI_PROC_NULL;
  } else if(myrank == npe-1) {
    ist = myrank*blk - 1;
    imax= blk + 1;
    right = MPI_PROC_NULL;
  } else {
    ist = myrank * blk - 1;
    imax= blk + 2;
  }

  /*
   *    Initializing matrixes
   */
  initmt(ist);

  if(myrank == 0){
    printf("mimax = %d mjmax = %d mkmax = %d\n",MIMAX, MJMAX, MKMAX);
    printf("imax = %d jmax = %d kmax =%d\n",imax,jmax,kmax);
  }

  /*
   *    Start measuring
   */
  cpu0 = MPI_Wtime();

  /*
   *    Jacobi iteration
   */

  gosa = jacobi(left,right);
  
  cpu1 = MPI_Wtime();

  if(myrank == 0){
    nflop = (MK-3)*(MJ-3)*(MI-3)*34;
    
    if(cpu1 != 0.0)
      xmflops2 = nflop/(cpu1-cpu0)*1.0e-6*(double)NN;
    
    score = xmflops2/32.27;
    
    printf("cpu : %f sec.\n", cpu1-cpu0);
    printf("Loop executed for %d times\n",NN);
    printf("Gosa : %e \n",gosa);
    printf("MFLOPS measured : %f\n",xmflops2);
    printf("Score based on MMX Pentium 200MHz : %f\n",score);
  }

  MPI_Finalize();
  return (0);
}

void
initmt(int ist)
{
	int i,j,k;

  for(i=0 ; i<imax ; ++i)
    for(j=0 ; j<jmax ; ++j)
      for(k=0 ; k<kmax ; ++k){
        a[i][j][k][0]=0.0;
        a[i][j][k][1]=0.0;
        a[i][j][k][2]=0.0;
        a[i][j][k][3]=0.0;
        b[i][j][k][0]=0.0;
        b[i][j][k][1]=0.0;
        b[i][j][k][2]=0.0;
        c[i][j][k][0]=0.0;
        c[i][j][k][1]=0.0;
        c[i][j][k][2]=0.0;
        p[i][j][k]=0.0;
        wrk1[i][j][k]=0.0;
        bnd[i][j][k]=0.0;
      }

  for(i=0 ; i<imax ; ++i)
    for(j=0 ; j<jmax ; ++j)
      for(k=0 ; k<kmax ; ++k){
        a[i][j][k][0]=1.0;
        a[i][j][k][1]=1.0;
        a[i][j][k][2]=1.0;
        a[i][j][k][3]=1.0/6.0;
        b[i][j][k][0]=0.0;
        b[i][j][k][1]=0.0;
        b[i][j][k][2]=0.0;
        c[i][j][k][0]=1.0;
        c[i][j][k][1]=1.0;
        c[i][j][k][2]=1.0;
        wrk1[i][j][k]=0.0;
        bnd[i][j][k]=1.0;
        p[i][j][k]=(float)((i+ist)*(i+ist))/
          (float)((MI-2)*(MI-2));
      }
}

float
jacobi(int left,int right)
{
  int  i,j,k,n;
  float  gosa, gosa0, s0, ss;

  MPI_Request  req[2];
  MPI_Status   st[2];

  for(n=0 ; n<NN ; ++n){
    gosa = 0.0;

    MPI_Irecv(&p[0][0][0],
              jmax*kmax,
              MPI_FLOAT,
              left,
              MPI_ANY_TAG,
              MPI_COMM_WORLD,
              &req[0]);
    MPI_Irecv(&p[imax-1][0][0],
              jmax*kmax,
              MPI_FLOAT,
              right,
              MPI_ANY_TAG,
              MPI_COMM_WORLD,
              &req[1]);

    MPI_Send(&p[1][0][0],
             jmax*kmax,
             MPI_FLOAT,
             left,
             0,
             MPI_COMM_WORLD);
    MPI_Send(&p[imax-2][0][0],
             jmax*kmax,
             MPI_FLOAT,
             right,
             0,
             MPI_COMM_WORLD);

    MPI_Wait(&req[0], &st[0]);
    MPI_Wait(&req[1], &st[1]);

    for(i=1 ; i<imax-1 ; ++i)
      for(j=1 ; j<jmax-1 ; ++j)
        for(k=1 ; k<kmax-1 ; ++k){
          s0 = a[i][j][k][0] * p[i+1][j  ][k  ]
             + a[i][j][k][1] * p[i  ][j+1][k  ]
             + a[i][j][k][2] * p[i  ][j  ][k+1]
             + b[i][j][k][0] * ( p[i+1][j+1][k  ] - p[i+1][j-1][k  ]
                               - p[i-1][j+1][k  ] + p[i-1][j-1][k  ] )
             + b[i][j][k][1] * ( p[i  ][j+1][k+1] - p[i  ][j-1][k+1]
                               - p[i  ][j+1][k-1] + p[i  ][j-1][k-1] )
             + b[i][j][k][2] * ( p[i+1][j  ][k+1] - p[i-1][j  ][k+1]
                               - p[i+1][j  ][k-1] + p[i-1][j  ][k-1] )
             + c[i][j][k][0] * p[i-1][j  ][k  ]
             + c[i][j][k][1] * p[i  ][j-1][k  ]
             + c[i][j][k][2] * p[i  ][j  ][k-1]
             + wrk1[i][j][k];

          ss = ( s0 * a[i][j][k][3] - p[i][j][k] ) * bnd[i][j][k];

          gosa = gosa + ss*ss;

          wrk2[i][j][k] = p[i][j][k] + omega * ss;

        }

    for(i=1 ; i<imax-1 ; ++i)
      for(j=1 ; j<jmax-1 ; ++j)
        for(k=1 ; k<kmax-1 ; ++k)
          p[i][j][k] = wrk2[i][j][k];

  } /* end n loop */

  gosa0 = 0.0;

  MPI_Allreduce(&gosa,
                &gosa0,
                1,
                MPI_FLOAT,
                MPI_SUM,
                MPI_COMM_WORLD);

  return(gosa0);
}
