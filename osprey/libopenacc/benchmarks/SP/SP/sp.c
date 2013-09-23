//-------------------------------------------------------------------------//
//                                                                         //
//  This benchmark is a serial C version of the NPB SP code. This C        //
//  version is developed by the Center for Manycore Programming at Seoul   //
//  National University and derived from the serial Fortran versions in    //
//  "NPB3.3-SER" developed by NAS.                                         //
//                                                                         //
//  Permission to use, copy, distribute and modify this software for any   //
//  purpose with or without fee is hereby granted. This software is        //
//  provided "as is" without express or implied warranty.                  //
//                                                                         //
//  Information on NPB 3.3, including the technical report, the original   //
//  specifications, source code, results and information on how to submit  //
//  new results, is available at:                                          //
//                                                                         //
//           http://www.nas.nasa.gov/Software/NPB/                         //
//                                                                         //
//  Send comments or suggestions for this C version to cmp@aces.snu.ac.kr  //
//                                                                         //
//          Center for Manycore Programming                                //
//          School of Computer Science and Engineering                     //
//          Seoul National University                                      //
//          Seoul 151-744, Korea                                           //
//                                                                         //
//          E-mail:  cmp@aces.snu.ac.kr                                    //
//                                                                         //
//-------------------------------------------------------------------------//

//-------------------------------------------------------------------------//
// Authors: Sangmin Seo, Jungwon Kim, Jun Lee, Jeongho Nah, Gangwon Jo,    //
//          and Jaejin Lee                                                 //
//-------------------------------------------------------------------------//

//---------------------------------------------------------------------
// program SP
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "header.h"
#include "type.h"
#include "timers.h"


#if defined(IBM)
#define wtime wtime
#elif defined(CRAY)
#define wtime WTIME
#else
#define wtime wtime_
#endif


/* common /global/ */
int grid_points[3], nx2, ny2, nz2;
logical timeron;

/* common /constants/ */
double tx1, tx2, tx3, ty1, ty2, ty3, tz1, tz2, tz3, 
       dx1, dx2, dx3, dx4, dx5, dy1, dy2, dy3, dy4, 
       dy5, dz1, dz2, dz3, dz4, dz5, dssp, dt, 
       ce[5][13], dxmax, dymax, dzmax, xxcon1, xxcon2, 
       xxcon3, xxcon4, xxcon5, dx1tx1, dx2tx1, dx3tx1,
       dx4tx1, dx5tx1, yycon1, yycon2, yycon3, yycon4,
       yycon5, dy1ty1, dy2ty1, dy3ty1, dy4ty1, dy5ty1,
       zzcon1, zzcon2, zzcon3, zzcon4, zzcon5, dz1tz1, 
       dz2tz1, dz3tz1, dz4tz1, dz5tz1, dnxm1, dnym1, 
       dnzm1, c1c2, c1c5, c3c4, c1345, conz1, c1, c2, 
       c3, c4, c5, c4dssp, c5dssp, dtdssp, dttx1, bt,
       dttx2, dtty1, dtty2, dttz1, dttz2, c2dttx1, 
       c2dtty1, c2dttz1, comz1, comz4, comz5, comz6, 
       c3c4tx3, c3c4ty3, c3c4tz3, c2iv, con43, con16;


/* common /fields/ */
double u[5][KMAX][JMAXP+1][IMAXP+1];
double us[KMAX][JMAXP+1][IMAXP+1];
double vs[KMAX][JMAXP+1][IMAXP+1];
double ws[KMAX][JMAXP+1][IMAXP+1];
double qs     [KMAX][JMAXP+1][IMAXP+1];
double rho_i  [KMAX][JMAXP+1][IMAXP+1];
double speed  [KMAX][JMAXP+1][IMAXP+1];
double square [KMAX][JMAXP+1][IMAXP+1];
double rhs[5][KMAX][JMAXP+1][IMAXP+1];
double forcing[JMAXP+1][5][KMAX][IMAXP+1];

/* common /work_1d/ */
double cv  [PROBLEM_SIZE];
double rhon[PROBLEM_SIZE];
double rhos[PROBLEM_SIZE];
double rhoq[PROBLEM_SIZE];
double cuf [PROBLEM_SIZE];
double q   [PROBLEM_SIZE];
double ue [PROBLEM_SIZE][5];
double buf[PROBLEM_SIZE][5];

/* common /work_lhs/ */
double lhs [IMAXP+1][IMAXP+1][5];
double lhsp[IMAXP+1][IMAXP+1][5];
double lhsm[IMAXP+1][IMAXP+1][5];

static double start[64], elapsed[64];


void wtime( double * );
void print_results(char *name, char class, int n1, int n2, int n3, int niter,
    double t, double mops, char *optype, logical verified, char *npbversion,
    char *compiletime, char *cs1, char *cs2, char *cs3, char *cs4, char *cs5,
    char *cs6, char *cs7);

int main(int argc, char *argv[])
{
  int i, niter, step, n3;
  double mflops, t, tmax, trecs[t_last+1];
  logical verified;
  char Class;
  char *t_names[t_last+1];

  //---------------------------------------------------------------------
  // Read input file (if it exists), else take
  // defaults from parameters
  //---------------------------------------------------------------------
  FILE *fp;
  if ((fp = fopen("timer.flag", "r")) != NULL) {
    timeron = true;
    t_names[t_total] = "total";
    t_names[t_rhsx] = "rhsx";
    t_names[t_rhsy] = "rhsy";
    t_names[t_rhsz] = "rhsz";
    t_names[t_rhs] = "rhs";
    t_names[t_xsolve] = "xsolve";
    t_names[t_ysolve] = "ysolve";
    t_names[t_zsolve] = "zsolve";
    t_names[t_rdis1] = "redist1";
    t_names[t_rdis2] = "redist2";
    t_names[t_tzetar] = "tzetar";
    t_names[t_ninvr] = "ninvr";
    t_names[t_pinvr] = "pinvr";
    t_names[t_txinvr] = "txinvr";
    t_names[t_add] = "add";
    fclose(fp);
  } else {
    timeron = false;
  }

  printf("\n\n NAS Parallel Benchmarks (NPB3.3-SER-C) - SP Benchmark\n\n");

  if ((fp = fopen("inputsp.data", "r")) != NULL) {
    int result;
    printf(" Reading from input file inputsp.data\n");
    result = fscanf(fp, "%d", &niter);
    while (fgetc(fp) != '\n');
    result = fscanf(fp, "%lf", &dt);
    while (fgetc(fp) != '\n');
    result = fscanf(fp, "%d%d%d", &grid_points[0], &grid_points[1], &grid_points[2]);
    fclose(fp);
  } else {
    printf(" No input file inputsp.data. Using compiled defaults\n");
    niter = NITER_DEFAULT;
    dt    = DT_DEFAULT;
    grid_points[0] = PROBLEM_SIZE;
    grid_points[1] = PROBLEM_SIZE;
    grid_points[2] = PROBLEM_SIZE;
  }

  printf(" Size: %4dx%4dx%4d\n", 
      grid_points[0], grid_points[1], grid_points[2]);
  printf(" Iterations: %4d    dt: %10.6f\n", niter, dt);
  printf("\n");

  if ((grid_points[0] > IMAX) ||
      (grid_points[1] > JMAX) ||
      (grid_points[2] > KMAX) ) {
    printf(" %d, %d, %d\n", grid_points[0], grid_points[1], grid_points[2]);
    printf(" Problem size too big for compiled array sizes\n");
    return 0;
  }
  nx2 = grid_points[0] - 2;
  ny2 = grid_points[1] - 2;
  nz2 = grid_points[2] - 2;

  set_constants();

  for (i = 1; i <= t_last; i++) {
    timer_clear(i);
  }
//pragma acc data create(u,us,vs,ws,qs,rho_i,speed,square,forcing,rhs)

#pragma acc data create(rhs, forcing, u, us, vs,ws,qs,rho_i,speed,square)
{
  exact_rhs();
#pragma acc update device(forcing)

  initialize();
#pragma acc update device(u)

  //---------------------------------------------------------------------
  // do one time step to touch all code, and reinitialize
  //---------------------------------------------------------------------
  adi();
  initialize();
#pragma acc update device(u)

  for (i = 1; i <= t_last; i++) {
    timer_clear(i);
  }
  timer_start(1);

  for (step = 1; step <= niter; step++) {
    if ((step % 20) == 0 || step == 1) {
      printf(" Time step %4d\n", step);
    }

    adi();
  }
#pragma acc update host(u)
}

  timer_stop(1);
  tmax = timer_read(1);

  verify(niter, &Class, &verified);

  if (tmax != 0.0) {
    n3 = grid_points[0]*grid_points[1]*grid_points[2];
    t = (grid_points[0]+grid_points[1]+grid_points[2])/3.0;
    mflops = (881.174 * (double)n3
             - 4683.91 * (t * t)
             + 11484.5 * t
             - 19272.4) * (double)niter / (tmax*1000000.0);
  } else {
    mflops = 0.0;
  }

  print_results("SP", Class, grid_points[0], 
                grid_points[1], grid_points[2], niter, 
                tmax, mflops, "          floating point", 
                verified, NPBVERSION,COMPILETIME, CS1, CS2, CS3, CS4, CS5, 
                CS6, "(none)");

  //---------------------------------------------------------------------
  // More timers
  //---------------------------------------------------------------------
  if (timeron) {
    for (i = 1; i <= t_last; i++) {
      trecs[i] = timer_read(i);
    }
    if (tmax == 0.0) tmax = 1.0;

    printf("  SECTION   Time (secs)\n");
    for (i = 1; i <= t_last; i++) {
      printf("  %-8s:%9.3f  (%6.2f%%)\n", 
          t_names[i], trecs[i], trecs[i]*100./tmax);
      if (i == t_rhs) {
        t = trecs[t_rhsx] + trecs[t_rhsy] + trecs[t_rhsz];
        printf("    --> %8s:%9.3f  (%6.2f%%)\n", "sub-rhs", t, t*100./tmax);
        t = trecs[t_rhs] - t;
        printf("    --> %8s:%9.3f  (%6.2f%%)\n", "rest-rhs", t, t*100./tmax);
      } else if (i == t_zsolve) {
        t = trecs[t_zsolve] - trecs[t_rdis1] - trecs[t_rdis2];
        printf("    --> %8s:%9.3f  (%6.2f%%)\n", "sub-zsol", t, t*100./tmax);
      } else if (i == t_rdis2) {
        t = trecs[t_rdis1] + trecs[t_rdis2];
        printf("    --> %8s:%9.3f  (%6.2f%%)\n", "redist", t, t*100./tmax);
      }
    }
  }

  return 0;
}

static double elapsed_time( void )
{
    double t;

    wtime( &t );
    return( t );
}

void timer_clear( int n )
{
    elapsed[n] = 0.0;
}

void timer_start( int n )
{
    start[n] = elapsed_time();
}

void timer_stop( int n )
{
    double t, now;

    now = elapsed_time();
    t = now - start[n];
    elapsed[n] += t;
}

double timer_read( int n )
{
	return( elapsed[n] );
}

void print_results(char *name, char class, int n1, int n2, int n3, int niter,
    double t, double mops, char *optype, logical verified, char *npbversion,
    char *compiletime, char *cs1, char *cs2, char *cs3, char *cs4, char *cs5,
    char *cs6, char *cs7) 
{
  char size[16];
  int j;

  printf( "\n\n %s Benchmark Completed.\n", name );
  printf( " Class           =             %12c\n", class );

  // If this is not a grid-based problem (EP, FT, CG), then
  // we only print n1, which contains some measure of the
  // problem size. In that case, n2 and n3 are both zero.
  // Otherwise, we print the grid size n1xn2xn3

  if ( ( n2 == 0 ) && ( n3 == 0 ) ) {
    if ( ( name[0] == 'E' ) && ( name[1] == 'P' ) ) {
      sprintf( size, "%15.0lf", pow(2.0, n1) );
      j = 14;
      if ( size[j] == '.' ) {
        size[j] = ' '; 
        j--;
      }
      size[j+1] = '\0';
      printf( " Size            =          %15s\n", size );
    } else {
      printf( " Size            =             %12d\n", n1 );
    }
  } else {
    printf( " Size            =           %4dx%4dx%4d\n", n1, n2, n3 );
  }

  printf( " Iterations      =             %12d\n", niter );
  printf( " Time in seconds =             %12.2lf\n", t );
  printf( " Mop/s total     =          %15.2lf\n", mops );
  printf( " Operation type  = %24s\n", optype );
  if ( verified ) 
    printf( " Verification    =             %12s\n", "SUCCESSFUL" );
  else
    printf( " Verification    =             %12s\n", "UNSUCCESSFUL" );
  printf( " Version         =             %12s\n", npbversion );
  printf( " Compile date    =             %12s\n", compiletime );
  
  printf( "\n Compile options:\n"
          "    CC           = %s\n", cs1 );
  printf( "    CLINK        = %s\n", cs2 );
  printf( "    C_LIB        = %s\n", cs3 );
  printf( "    C_INC        = %s\n", cs4 );
  printf( "    CFLAGS       = %s\n", cs5 );
  printf( "    CLINKFLAGS   = %s\n", cs6 );
  printf( "    RAND         = %s\n", cs7 );

  printf( "\n--------------------------------------\n"
          " Please send all errors/feedbacks to:\n"
          " Center for Manycore Programming\n"
          " cmp@aces.snu.ac.kr\n"
          " http://aces.snu.ac.kr\n"
          "--------------------------------------\n\n");
}

void c_print_results( char   *name,
                      char   class,
                      int    n1, 
                      int    n2,
                      int    n3,
                      int    niter,
                      double t,
                      double mops,
		      char   *optype,
                      int    passed_verification,
                      char   *npbversion,
                      char   *compiletime,
                      char   *cc,
                      char   *clink,
                      char   *c_lib,
                      char   *c_inc,
                      char   *cflags,
                      char   *clinkflags )
{
    printf( "\n\n %s Benchmark Completed\n", name ); 

    printf( " Class           =                        %c\n", class );

    if( n3 == 0 ) {
        long nn = n1;
        if ( n2 != 0 ) nn *= n2;
        printf( " Size            =             %12ld\n", nn );   /* as in IS */
    }
    else
        printf( " Size            =             %4dx%4dx%4d\n", n1,n2,n3 );

    printf( " Iterations      =             %12d\n", niter );
 
    printf( " Time in seconds =             %12.2f\n", t );

    printf( " Mop/s total     =             %12.2f\n", mops );

    printf( " Operation type  = %24s\n", optype);

    if( passed_verification < 0 )
        printf( " Verification    =            NOT PERFORMED\n" );
    else if( passed_verification )
        printf( " Verification    =               SUCCESSFUL\n" );
    else
        printf( " Verification    =             UNSUCCESSFUL\n" );

    printf( " Version         =             %12s\n", npbversion );

    printf( " Compile date    =             %12s\n", compiletime );

    printf( "\n Compile options:\n" );

    printf( "    CC           = %s\n", cc );

    printf( "    CLINK        = %s\n", clink );

    printf( "    C_LIB        = %s\n", c_lib );

    printf( "    C_INC        = %s\n", c_inc );

    printf( "    CFLAGS       = %s\n", cflags );

    printf( "    CLINKFLAGS   = %s\n", clinkflags );
#ifdef SMP
    evalue = getenv("MP_SET_NUMTHREADS");
    printf( "   MULTICPUS = %s\n", evalue );
#endif

    printf( "\n--------------------------------------\n");
    printf( " Please send all errors/feedbacks to:\n");
    printf( " Center for Manycore Programming\n");
    printf( " cmp@aces.snu.ac.kr\n");
    printf( " http://aces.snu.ac.kr\n");
    printf( "--------------------------------------\n");
}

double randlc( double *x, double a )
{
  //--------------------------------------------------------------------
  //
  //  This routine returns a uniform pseudorandom double precision number in the
  //  range (0, 1) by using the linear congruential generator
  //
  //  x_{k+1} = a x_k  (mod 2^46)
  //
  //  where 0 < x_k < 2^46 and 0 < a < 2^46.  This scheme generates 2^44 numbers
  //  before repeating.  The argument A is the same as 'a' in the above formula,
  //  and X is the same as x_0.  A and X must be odd double precision integers
  //  in the range (1, 2^46).  The returned value RANDLC is normalized to be
  //  between 0 and 1, i.e. RANDLC = 2^(-46) * x_1.  X is updated to contain
  //  the new seed x_1, so that subsequent calls to RANDLC using the same
  //  arguments will generate a continuous sequence.
  //
  //  This routine should produce the same results on any computer with at least
  //  48 mantissa bits in double precision floating point data.  On 64 bit
  //  systems, double precision should be disabled.
  //
  //  David H. Bailey     October 26, 1990
  //
  //--------------------------------------------------------------------

  // r23 = pow(0.5, 23.0);
  ////  pow(0.5, 23.0) = 1.1920928955078125e-07
  // r46 = r23 * r23;
  // t23 = pow(2.0, 23.0);
  ////  pow(2.0, 23.0) = 8.388608e+06
  // t46 = t23 * t23;

  const double r23 = 1.1920928955078125e-07;
  const double r46 = r23 * r23;
  const double t23 = 8.388608e+06;
  const double t46 = t23 * t23;

  double t1, t2, t3, t4, a1, a2, x1, x2, z;
  double r;

  //--------------------------------------------------------------------
  //  Break A into two parts such that A = 2^23 * A1 + A2.
  //--------------------------------------------------------------------
  t1 = r23 * a;
  a1 = (int) t1;
  a2 = a - t23 * a1;

  //--------------------------------------------------------------------
  //  Break X into two parts such that X = 2^23 * X1 + X2, compute
  //  Z = A1 * X2 + A2 * X1  (mod 2^23), and then
  //  X = 2^23 * Z + A2 * X2  (mod 2^46).
  //--------------------------------------------------------------------
  t1 = r23 * (*x);
  x1 = (int) t1;
  x2 = *x - t23 * x1;
  t1 = a1 * x2 + a2 * x1;
  t2 = (int) (r23 * t1);
  z = t1 - t23 * t2;
  t3 = t23 * z + a2 * x2;
  t4 = (int) (r46 * t3);
  *x = t3 - t46 * t4;
  r = r46 * (*x);

  return r;
}


void vranlc( int n, double *x, double a, double y[] )
{
  //--------------------------------------------------------------------
  //
  //  This routine generates N uniform pseudorandom double precision numbers in
  //  the range (0, 1) by using the linear congruential generator
  //
  //  x_{k+1} = a x_k  (mod 2^46)
  //
  //  where 0 < x_k < 2^46 and 0 < a < 2^46.  This scheme generates 2^44 numbers
  //  before repeating.  The argument A is the same as 'a' in the above formula,
  //  and X is the same as x_0.  A and X must be odd double precision integers
  //  in the range (1, 2^46).  The N results are placed in Y and are normalized
  //  to be between 0 and 1.  X is updated to contain the new seed, so that
  //  subsequent calls to VRANLC using the same arguments will generate a
  //  continuous sequence.  If N is zero, only initialization is performed, and
  //  the variables X, A and Y are ignored.
  //
  //  This routine is the standard version designed for scalar or RISC systems.
  //  However, it should produce the same results on any single processor
  //  computer with at least 48 mantissa bits in double precision floating point
  //  data.  On 64 bit systems, double precision should be disabled.
  //
  //--------------------------------------------------------------------

  // r23 = pow(0.5, 23.0);
  ////  pow(0.5, 23.0) = 1.1920928955078125e-07
  // r46 = r23 * r23;
  // t23 = pow(2.0, 23.0);
  ////  pow(2.0, 23.0) = 8.388608e+06
  // t46 = t23 * t23;

  const double r23 = 1.1920928955078125e-07;
  const double r46 = r23 * r23;
  const double t23 = 8.388608e+06;
  const double t46 = t23 * t23;

  double t1, t2, t3, t4, a1, a2, x1, x2, z;

  int i;

  //--------------------------------------------------------------------
  //  Break A into two parts such that A = 2^23 * A1 + A2.
  //--------------------------------------------------------------------
  t1 = r23 * a;
  a1 = (int) t1;
  a2 = a - t23 * a1;

  //--------------------------------------------------------------------
  //  Generate N results.   This loop is not vectorizable.
  //--------------------------------------------------------------------
  for ( i = 0; i < n; i++ ) {
    //--------------------------------------------------------------------
    //  Break X into two parts such that X = 2^23 * X1 + X2, compute
    //  Z = A1 * X2 + A2 * X1  (mod 2^23), and then
    //  X = 2^23 * Z + A2 * X2  (mod 2^46).
    //--------------------------------------------------------------------
    t1 = r23 * (*x);
    x1 = (int) t1;
    x2 = *x - t23 * x1;
    t1 = a1 * x2 + a2 * x1;
    t2 = (int) (r23 * t1);
    z = t1 - t23 * t2;
    t3 = t23 * z + a2 * x2;
    t4 = (int) (r46 * t3) ;
    *x = t3 - t46 * t4;
    y[i] = r46 * (*x);
  }

  return;
}


// verification routine                         
//---------------------------------------------------------------------
void verify(int no_time_steps, char *Class, logical *verified)
{
  double xcrref[5], xceref[5], xcrdif[5], xcedif[5];
  double epsilon, xce[5], xcr[5], dtref = 0.0;
  int m;

  //---------------------------------------------------------------------
  // tolerance level
  //---------------------------------------------------------------------
  epsilon = 1.0e-08;


  //---------------------------------------------------------------------
  // compute the error norm and the residual norm, and exit if not printing
  //---------------------------------------------------------------------
  error_norm(xce);
  compute_rhs();

  rhs_norm(xcr);

  for (m = 0; m < 5; m++) {
    xcr[m] = xcr[m] / dt;
  }

  *Class = 'U';
  *verified = true;

  for (m = 0; m < 5; m++) {
    xcrref[m] = 1.0;
    xceref[m] = 1.0;
  }

  //---------------------------------------------------------------------
  // reference data for 12X12X12 grids after 100 time steps, 
  // with DT = 1.50e-02
  //---------------------------------------------------------------------
  if ( (grid_points[0] == 12) && (grid_points[1] == 12) &&
       (grid_points[2] == 12) && (no_time_steps == 100) ) {
    *Class = 'S';
    dtref = 1.5e-2;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of residual.
    //---------------------------------------------------------------------
    xcrref[0] = 2.7470315451339479e-02;
    xcrref[1] = 1.0360746705285417e-02;
    xcrref[2] = 1.6235745065095532e-02;
    xcrref[3] = 1.5840557224455615e-02;
    xcrref[4] = 3.4849040609362460e-02;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of solution error.
    //---------------------------------------------------------------------
    xceref[0] = 2.7289258557377227e-05;
    xceref[1] = 1.0364446640837285e-05;
    xceref[2] = 1.6154798287166471e-05;
    xceref[3] = 1.5750704994480102e-05;
    xceref[4] = 3.4177666183390531e-05;

    //---------------------------------------------------------------------
    // reference data for 36X36X36 grids after 400 time steps, 
    // with DT = 1.5e-03
    //---------------------------------------------------------------------
  } else if ( (grid_points[0] == 36) && (grid_points[1] == 36) &&
              (grid_points[2] == 36) && (no_time_steps == 400) ) {
    *Class = 'W';
    dtref = 1.5e-3;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of residual.
    //---------------------------------------------------------------------
    xcrref[0] = 0.1893253733584e-02;
    xcrref[1] = 0.1717075447775e-03;
    xcrref[2] = 0.2778153350936e-03;
    xcrref[3] = 0.2887475409984e-03;
    xcrref[4] = 0.3143611161242e-02;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of solution error.
    //---------------------------------------------------------------------
    xceref[0] = 0.7542088599534e-04;
    xceref[1] = 0.6512852253086e-05;
    xceref[2] = 0.1049092285688e-04;
    xceref[3] = 0.1128838671535e-04;
    xceref[4] = 0.1212845639773e-03;

    //---------------------------------------------------------------------
    // reference data for 64X64X64 grids after 400 time steps, 
    // with DT = 1.5e-03
    //---------------------------------------------------------------------
  } else if ( (grid_points[0] == 64) && (grid_points[1] == 64) &&
              (grid_points[2] == 64) && (no_time_steps == 400) ) {
    *Class = 'A';
    dtref = 1.5e-3;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of residual.
    //---------------------------------------------------------------------
    xcrref[0] = 2.4799822399300195;
    xcrref[1] = 1.1276337964368832;
    xcrref[2] = 1.5028977888770491;
    xcrref[3] = 1.4217816211695179;
    xcrref[4] = 2.1292113035138280;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of solution error.
    //---------------------------------------------------------------------
    xceref[0] = 1.0900140297820550e-04;
    xceref[1] = 3.7343951769282091e-05;
    xceref[2] = 5.0092785406541633e-05;
    xceref[3] = 4.7671093939528255e-05;
    xceref[4] = 1.3621613399213001e-04;

    //---------------------------------------------------------------------
    // reference data for 102X102X102 grids after 400 time steps,
    // with DT = 1.0e-03
    //---------------------------------------------------------------------
  } else if ( (grid_points[0] == 102) && (grid_points[1] == 102) &&
              (grid_points[2] == 102) && (no_time_steps == 400) ) {
    *Class = 'B';
    dtref = 1.0e-3;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of residual.
    //---------------------------------------------------------------------
    xcrref[0] = 0.6903293579998e+02;
    xcrref[1] = 0.3095134488084e+02;
    xcrref[2] = 0.4103336647017e+02;
    xcrref[3] = 0.3864769009604e+02;
    xcrref[4] = 0.5643482272596e+02;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of solution error.
    //---------------------------------------------------------------------
    xceref[0] = 0.9810006190188e-02;
    xceref[1] = 0.1022827905670e-02;
    xceref[2] = 0.1720597911692e-02;
    xceref[3] = 0.1694479428231e-02;
    xceref[4] = 0.1847456263981e-01;

    //---------------------------------------------------------------------
    // reference data for 162X162X162 grids after 400 time steps,
    // with DT = 0.67e-03
    //---------------------------------------------------------------------
  } else if ( (grid_points[0] == 162) && (grid_points[1] == 162) &&
              (grid_points[2] == 162) && (no_time_steps == 400) ) {
    *Class = 'C';
    dtref = 0.67e-3;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of residual.
    //---------------------------------------------------------------------
    xcrref[0] = 0.5881691581829e+03;
    xcrref[1] = 0.2454417603569e+03;
    xcrref[2] = 0.3293829191851e+03;
    xcrref[3] = 0.3081924971891e+03;
    xcrref[4] = 0.4597223799176e+03;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of solution error.
    //---------------------------------------------------------------------
    xceref[0] = 0.2598120500183e+00;
    xceref[1] = 0.2590888922315e-01;
    xceref[2] = 0.5132886416320e-01;
    xceref[3] = 0.4806073419454e-01;
    xceref[4] = 0.5483377491301e+00;

    //---------------------------------------------------------------------
    // reference data for 408X408X408 grids after 500 time steps,
    // with DT = 0.3e-03
    //---------------------------------------------------------------------
  } else if ( (grid_points[0] == 408) && (grid_points[1] == 408) &&
              (grid_points[2] == 408) && (no_time_steps == 500) ) {
    *Class = 'D';
    dtref = 0.30e-3;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of residual.
    //---------------------------------------------------------------------
    xcrref[0] = 0.1044696216887e+05;
    xcrref[1] = 0.3204427762578e+04;
    xcrref[2] = 0.4648680733032e+04;
    xcrref[3] = 0.4238923283697e+04;
    xcrref[4] = 0.7588412036136e+04;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of solution error.
    //---------------------------------------------------------------------
    xceref[0] = 0.5089471423669e+01;
    xceref[1] = 0.5323514855894e+00;
    xceref[2] = 0.1187051008971e+01;
    xceref[3] = 0.1083734951938e+01;
    xceref[4] = 0.1164108338568e+02;

    //---------------------------------------------------------------------
    // reference data for 1020X1020X1020 grids after 500 time steps,
    // with DT = 0.1e-03
    //---------------------------------------------------------------------
  } else if ( (grid_points[0] == 1020) && (grid_points[1] == 1020) &&
              (grid_points[2] == 1020) && (no_time_steps == 500) ) {
    *Class = 'E';
    dtref = 0.10e-3;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of residual.
    //---------------------------------------------------------------------
    xcrref[0] = 0.6255387422609e+05;
    xcrref[1] = 0.1495317020012e+05;
    xcrref[2] = 0.2347595750586e+05;
    xcrref[3] = 0.2091099783534e+05;
    xcrref[4] = 0.4770412841218e+05;

    //---------------------------------------------------------------------
    // Reference values of RMS-norms of solution error.
    //---------------------------------------------------------------------
    xceref[0] = 0.6742735164909e+02;
    xceref[1] = 0.5390656036938e+01;
    xceref[2] = 0.1680647196477e+02;
    xceref[3] = 0.1536963126457e+02;
    xceref[4] = 0.1575330146156e+03;
  } else {
    *verified = false;
  }

  //---------------------------------------------------------------------
  // verification test for residuals if gridsize is one of 
  // the defined grid sizes above (class .ne. 'U')
  //---------------------------------------------------------------------

  //---------------------------------------------------------------------
  // Compute the difference of solution values and the known reference values.
  //---------------------------------------------------------------------
  for (m = 0; m < 5; m++) {
    xcrdif[m] = fabs((xcr[m]-xcrref[m])/xcrref[m]);
    xcedif[m] = fabs((xce[m]-xceref[m])/xceref[m]);
  }

  //---------------------------------------------------------------------
  // Output the comparison of computed results to known cases.
  //---------------------------------------------------------------------
  if (*Class != 'U') {
    printf(" Verification being performed for class %c\n", *Class);
    printf(" accuracy setting for epsilon = %20.13E\n", epsilon);
    *verified = (fabs(dt-dtref) <= epsilon);
    if (!(*verified)) {  
      *Class = 'U';
      printf(" DT does not match the reference value of %15.8E\n", dtref);
    } 
  } else {
    printf(" Unknown class\n");
  }

  if (*Class != 'U') {
    printf(" Comparison of RMS-norms of residual\n");
  } else {
    printf(" RMS-norms of residual\n");
  }

  for (m = 0; m < 5; m++) {
    if (*Class == 'U') {
      printf("          %2d%20.13E\n", m+1, xcr[m]);
    } else if (xcrdif[m] <= epsilon) {
      printf("          %2d%20.13E%20.13E%20.13E\n",
          m+1, xcr[m], xcrref[m], xcrdif[m]);
    } else  {
      *verified = false;
      printf(" FAILURE: %2d%20.13E%20.13E%20.13E\n",
          m+1, xcr[m], xcrref[m], xcrdif[m]);
    }
  }

  if (*Class != 'U') {
    printf(" Comparison of RMS-norms of solution error\n");
  } else {
    printf(" RMS-norms of solution error\n");
  }
        
  for (m = 0; m < 5; m++) {
    if (*Class == 'U') {
      printf("          %2d%20.13E\n", m+1, xce[m]);
    } else if (xcedif[m] <= epsilon) {
      printf("          %2d%20.13E%20.13E%20.13E\n",
          m+1, xce[m], xceref[m], xcedif[m]);
    } else {
      *verified = false;
      printf(" FAILURE: %2d%20.13E%20.13E%20.13E\n",
          m+1, xce[m], xceref[m], xcedif[m]);
    }
  }
        
  if (*Class == 'U') {
    printf(" No reference values provided\n");
    printf(" No verification performed\n");
  } else if (*verified) {
    printf(" Verification Successful\n");
  } else {
    printf(" Verification failed\n");
  }
}


void set_constants()
{
  ce[0][0]  = 2.0;
  ce[0][1]  = 0.0;
  ce[0][2]  = 0.0;
  ce[0][3]  = 4.0;
  ce[0][4]  = 5.0;
  ce[0][5]  = 3.0;
  ce[0][6]  = 0.5;
  ce[0][7]  = 0.02;
  ce[0][8]  = 0.01;
  ce[0][9]  = 0.03;
  ce[0][10] = 0.5;
  ce[0][11] = 0.4;
  ce[0][12] = 0.3;

  ce[1][0]  = 1.0;
  ce[1][1]  = 0.0;
  ce[1][2]  = 0.0;
  ce[1][3]  = 0.0;
  ce[1][4]  = 1.0;
  ce[1][5]  = 2.0;
  ce[1][6]  = 3.0;
  ce[1][7]  = 0.01;
  ce[1][8]  = 0.03;
  ce[1][9]  = 0.02;
  ce[1][10] = 0.4;
  ce[1][11] = 0.3;
  ce[1][12] = 0.5;

  ce[2][0]  = 2.0;
  ce[2][1]  = 2.0;
  ce[2][2]  = 0.0;
  ce[2][3]  = 0.0;
  ce[2][4]  = 0.0;
  ce[2][5]  = 2.0;
  ce[2][6]  = 3.0;
  ce[2][7]  = 0.04;
  ce[2][8]  = 0.03;
  ce[2][9]  = 0.05;
  ce[2][10] = 0.3;
  ce[2][11] = 0.5;
  ce[2][12] = 0.4;

  ce[3][0]  = 2.0;
  ce[3][1]  = 2.0;
  ce[3][2]  = 0.0;
  ce[3][3]  = 0.0;
  ce[3][4]  = 0.0;
  ce[3][5]  = 2.0;
  ce[3][6]  = 3.0;
  ce[3][7]  = 0.03;
  ce[3][8]  = 0.05;
  ce[3][9]  = 0.04;
  ce[3][10] = 0.2;
  ce[3][11] = 0.1;
  ce[3][12] = 0.3;

  ce[4][0]  = 5.0;
  ce[4][1]  = 4.0;
  ce[4][2]  = 3.0;
  ce[4][3]  = 2.0;
  ce[4][4]  = 0.1;
  ce[4][5]  = 0.4;
  ce[4][6]  = 0.3;
  ce[4][7]  = 0.05;
  ce[4][8]  = 0.04;
  ce[4][9]  = 0.03;
  ce[4][10] = 0.1;
  ce[4][11] = 0.3;
  ce[4][12] = 0.2;

  c1 = 1.4;
  c2 = 0.4;
  c3 = 0.1;
  c4 = 1.0;
  c5 = 1.4;

  bt = sqrt(0.5);

  dnxm1 = 1.0 / (double)(grid_points[0]-1);
  dnym1 = 1.0 / (double)(grid_points[1]-1);
  dnzm1 = 1.0 / (double)(grid_points[2]-1);

  c1c2 = c1 * c2;
  c1c5 = c1 * c5;
  c3c4 = c3 * c4;
  c1345 = c1c5 * c3c4;

  conz1 = (1.0-c1c5);

  tx1 = 1.0 / (dnxm1 * dnxm1);
  tx2 = 1.0 / (2.0 * dnxm1);
  tx3 = 1.0 / dnxm1;

  ty1 = 1.0 / (dnym1 * dnym1);
  ty2 = 1.0 / (2.0 * dnym1);
  ty3 = 1.0 / dnym1;

  tz1 = 1.0 / (dnzm1 * dnzm1);
  tz2 = 1.0 / (2.0 * dnzm1);
  tz3 = 1.0 / dnzm1;

  dx1 = 0.75;
  dx2 = 0.75;
  dx3 = 0.75;
  dx4 = 0.75;
  dx5 = 0.75;

  dy1 = 0.75;
  dy2 = 0.75;
  dy3 = 0.75;
  dy4 = 0.75;
  dy5 = 0.75;

  dz1 = 1.0;
  dz2 = 1.0;
  dz3 = 1.0;
  dz4 = 1.0;
  dz5 = 1.0;

  dxmax = max(dx3, dx4);
  dymax = max(dy2, dy4);
  dzmax = max(dz2, dz3);

  dssp = 0.25 * max(dx1, max(dy1, dz1));

  c4dssp = 4.0 * dssp;
  c5dssp = 5.0 * dssp;

  dttx1 = dt*tx1;
  dttx2 = dt*tx2;
  dtty1 = dt*ty1;
  dtty2 = dt*ty2;
  dttz1 = dt*tz1;
  dttz2 = dt*tz2;

  c2dttx1 = 2.0*dttx1;
  c2dtty1 = 2.0*dtty1;
  c2dttz1 = 2.0*dttz1;

  dtdssp = dt*dssp;

  comz1  = dtdssp;
  comz4  = 4.0*dtdssp;
  comz5  = 5.0*dtdssp;
  comz6  = 6.0*dtdssp;

  c3c4tx3 = c3c4*tx3;
  c3c4ty3 = c3c4*ty3;
  c3c4tz3 = c3c4*tz3;

  dx1tx1 = dx1*tx1;
  dx2tx1 = dx2*tx1;
  dx3tx1 = dx3*tx1;
  dx4tx1 = dx4*tx1;
  dx5tx1 = dx5*tx1;

  dy1ty1 = dy1*ty1;
  dy2ty1 = dy2*ty1;
  dy3ty1 = dy3*ty1;
  dy4ty1 = dy4*ty1;
  dy5ty1 = dy5*ty1;

  dz1tz1 = dz1*tz1;
  dz2tz1 = dz2*tz1;
  dz3tz1 = dz3*tz1;
  dz4tz1 = dz4*tz1;
  dz5tz1 = dz5*tz1;

  c2iv  = 2.5;
  con43 = 4.0/3.0;
  con16 = 1.0/6.0;

  xxcon1 = c3c4tx3*con43*tx3;
  xxcon2 = c3c4tx3*tx3;
  xxcon3 = c3c4tx3*conz1*tx3;
  xxcon4 = c3c4tx3*con16*tx3;
  xxcon5 = c3c4tx3*c1c5*tx3;

  yycon1 = c3c4ty3*con43*ty3;
  yycon2 = c3c4ty3*ty3;
  yycon3 = c3c4ty3*conz1*ty3;
  yycon4 = c3c4ty3*con16*ty3;
  yycon5 = c3c4ty3*c1c5*ty3;

  zzcon1 = c3c4tz3*con43*tz3;
  zzcon2 = c3c4tz3*tz3;
  zzcon3 = c3c4tz3*conz1*tz3;
  zzcon4 = c3c4tz3*con16*tz3;
  zzcon5 = c3c4tz3*c1c5*tz3;
}


// compute the right hand side based on exact solution
//---------------------------------------------------------------------
void exact_rhs()
{
  double dtemp[5], xi, eta, zeta, dtpp;
  int m, i, j, k, ip1, im1, jp1, jm1, km1, kp1;

  //---------------------------------------------------------------------
  // initialize                                  
  //---------------------------------------------------------------------
  for (k = 0; k <= grid_points[2]-1; k++) {
    for (j = 0; j <= grid_points[1]-1; j++) {
      for (i = 0; i <= grid_points[0]-1; i++) {
        for (m = 0; m < 5; m++) {
          forcing[j][m][k][i] = 0.0;
        }
      }
    }
  }

  //---------------------------------------------------------------------
  // xi-direction flux differences                      
  //---------------------------------------------------------------------

  for (k = 1; k <= grid_points[2]-2; k++) {
    zeta = (double)k * dnzm1;
    for (j = 1; j <= grid_points[1]-2; j++) {
      eta = (double)j * dnym1;

      for (i = 0; i <= grid_points[0]-1; i++) {
        xi = (double)i * dnxm1;

        exact_solution(xi, eta, zeta, dtemp);
        for (m = 0; m < 5; m++) {
          ue[i][m] = dtemp[m];
        }

        dtpp = 1.0 / dtemp[0];

        for (m = 1; m < 5; m++) {
          buf[i][m] = dtpp * dtemp[m];
        }

        cuf[i]    = buf[i][1] * buf[i][1];
        buf[i][0] = cuf[i] + buf[i][2] * buf[i][2] + buf[i][3] * buf[i][3]; 
        q[i] = 0.5*(buf[i][1]*ue[i][1] + buf[i][2]*ue[i][2] +
                    buf[i][3]*ue[i][3]);
      }

      for (i = 1; i <= grid_points[0]-2; i++) {
        im1 = i-1;
        ip1 = i+1;

        forcing[j][0][k][i] = forcing[j][0][k][i] -
          tx2*( ue[ip1][1]-ue[im1][1] )+
          dx1tx1*(ue[ip1][0]-2.0*ue[i][0]+ue[im1][0]);

        forcing[j][1][k][i] = forcing[j][1][k][i] - tx2 * (
            (ue[ip1][1]*buf[ip1][1]+c2*(ue[ip1][4]-q[ip1]))-
            (ue[im1][1]*buf[im1][1]+c2*(ue[im1][4]-q[im1])))+
          xxcon1*(buf[ip1][1]-2.0*buf[i][1]+buf[im1][1])+
          dx2tx1*( ue[ip1][1]-2.0* ue[i][1]+ue[im1][1]);

        forcing[j][2][k][i] = forcing[j][2][k][i] - tx2 * (
            ue[ip1][2]*buf[ip1][1]-ue[im1][2]*buf[im1][1])+
          xxcon2*(buf[ip1][2]-2.0*buf[i][2]+buf[im1][2])+
          dx3tx1*( ue[ip1][2]-2.0*ue[i][2] +ue[im1][2]);

        forcing[j][3][k][i] = forcing[j][3][k][i] - tx2*(
            ue[ip1][3]*buf[ip1][1]-ue[im1][3]*buf[im1][1])+
          xxcon2*(buf[ip1][3]-2.0*buf[i][3]+buf[im1][3])+
          dx4tx1*( ue[ip1][3]-2.0* ue[i][3]+ ue[im1][3]);

        forcing[j][4][k][i] = forcing[j][4][k][i] - tx2*(
            buf[ip1][1]*(c1*ue[ip1][4]-c2*q[ip1])-
            buf[im1][1]*(c1*ue[im1][4]-c2*q[im1]))+
          0.5*xxcon3*(buf[ip1][0]-2.0*buf[i][0]+buf[im1][0])+
          xxcon4*(cuf[ip1]-2.0*cuf[i]+cuf[im1])+
          xxcon5*(buf[ip1][4]-2.0*buf[i][4]+buf[im1][4])+
          dx5tx1*( ue[ip1][4]-2.0* ue[i][4]+ ue[im1][4]);
      }

      //---------------------------------------------------------------------
      // Fourth-order dissipation                         
      //---------------------------------------------------------------------
      for (m = 0; m < 5; m++) {
        i = 1;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (5.0*ue[i][m] - 4.0*ue[i+1][m] +ue[i+2][m]);
        i = 2;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (-4.0*ue[i-1][m] + 6.0*ue[i][m] -
            4.0*ue[i+1][m] +     ue[i+2][m]);
      }

      for (i = 3; i <= grid_points[0]-4; i++) {
        for (m = 0; m < 5; m++) {
          forcing[j][m][k][i] = forcing[j][m][k][i] - dssp*
            (ue[i-2][m] - 4.0*ue[i-1][m] +
             6.0*ue[i][m] - 4.0*ue[i+1][m] + ue[i+2][m]);
        }
      }

      for (m = 0; m < 5; m++) {
        i = grid_points[0]-3;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (ue[i-2][m] - 4.0*ue[i-1][m] +
           6.0*ue[i][m] - 4.0*ue[i+1][m]);
        i = grid_points[0]-2;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (ue[i-2][m] - 4.0*ue[i-1][m] + 5.0*ue[i][m]);
      }
    }
  }

  //---------------------------------------------------------------------
  // eta-direction flux differences             
  //---------------------------------------------------------------------
  for (k = 1; k <= grid_points[2]-2; k++) {
    zeta = (double)k * dnzm1;
    for (i = 1; i <= grid_points[0]-2; i++) {
      xi = (double)i * dnxm1;

      for (j = 0; j <= grid_points[1]-1; j++) {
        eta = (double)j * dnym1;

        exact_solution(xi, eta, zeta, dtemp);
        for (m = 0; m < 5; m++) {
          ue[j][m] = dtemp[m];
        }
        dtpp = 1.0/dtemp[0];

        for (m = 1; m < 5; m++) {
          buf[j][m] = dtpp * dtemp[m];
        }

        cuf[j]    = buf[j][2] * buf[j][2];
        buf[j][0] = cuf[j] + buf[j][1] * buf[j][1] + buf[j][3] * buf[j][3];
        q[j] = 0.5*(buf[j][1]*ue[j][1] + buf[j][2]*ue[j][2] +
                    buf[j][3]*ue[j][3]);
      }

      for (j = 1; j <= grid_points[1]-2; j++) {
        jm1 = j-1;
        jp1 = j+1;

        forcing[j][0][k][i] = forcing[j][0][k][i] -
          ty2*( ue[jp1][2]-ue[jm1][2] )+
          dy1ty1*(ue[jp1][0]-2.0*ue[j][0]+ue[jm1][0]);

        forcing[j][1][k][i] = forcing[j][1][k][i] - ty2*(
            ue[jp1][1]*buf[jp1][2]-ue[jm1][1]*buf[jm1][2])+
          yycon2*(buf[jp1][1]-2.0*buf[j][1]+buf[jm1][1])+
          dy2ty1*( ue[jp1][1]-2.0* ue[j][1]+ ue[jm1][1]);

        forcing[j][2][k][i] = forcing[j][2][k][i] - ty2*(
            (ue[jp1][2]*buf[jp1][2]+c2*(ue[jp1][4]-q[jp1]))-
            (ue[jm1][2]*buf[jm1][2]+c2*(ue[jm1][4]-q[jm1])))+
          yycon1*(buf[jp1][2]-2.0*buf[j][2]+buf[jm1][2])+
          dy3ty1*( ue[jp1][2]-2.0*ue[j][2] +ue[jm1][2]);

        forcing[j][3][k][i] = forcing[j][3][k][i] - ty2*(
            ue[jp1][3]*buf[jp1][2]-ue[jm1][3]*buf[jm1][2])+
          yycon2*(buf[jp1][3]-2.0*buf[j][3]+buf[jm1][3])+
          dy4ty1*( ue[jp1][3]-2.0*ue[j][3]+ ue[jm1][3]);

        forcing[j][4][k][i] = forcing[j][4][k][i] - ty2*(
            buf[jp1][2]*(c1*ue[jp1][4]-c2*q[jp1])-
            buf[jm1][2]*(c1*ue[jm1][4]-c2*q[jm1]))+
          0.5*yycon3*(buf[jp1][0]-2.0*buf[j][0]+
              buf[jm1][0])+
          yycon4*(cuf[jp1]-2.0*cuf[j]+cuf[jm1])+
          yycon5*(buf[jp1][4]-2.0*buf[j][4]+buf[jm1][4])+
          dy5ty1*(ue[jp1][4]-2.0*ue[j][4]+ue[jm1][4]);
      }

      //---------------------------------------------------------------------
      // Fourth-order dissipation                      
      //---------------------------------------------------------------------
      for (m = 0; m < 5; m++) {
        j = 1;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (5.0*ue[j][m] - 4.0*ue[j+1][m] +ue[j+2][m]);
        j = 2;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (-4.0*ue[j-1][m] + 6.0*ue[j][m] -
            4.0*ue[j+1][m] +     ue[j+2][m]);
      }

      for (j = 3; j <= grid_points[1]-4; j++) {
        for (m = 0; m < 5; m++) {
          forcing[j][m][k][i] = forcing[j][m][k][i] - dssp*
            (ue[j-2][m] - 4.0*ue[j-1][m] +
             6.0*ue[j][m] - 4.0*ue[j+1][m] + ue[j+2][m]);
        }
      }

      for (m = 0; m < 5; m++) {
        j = grid_points[1]-3;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (ue[j-2][m] - 4.0*ue[j-1][m] +
           6.0*ue[j][m] - 4.0*ue[j+1][m]);
        j = grid_points[1]-2;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (ue[j-2][m] - 4.0*ue[j-1][m] + 5.0*ue[j][m]);
      }
    }
  }

  //---------------------------------------------------------------------
  // zeta-direction flux differences                      
  //---------------------------------------------------------------------
  for (j = 1; j <= grid_points[1]-2; j++) {
    eta = (double)j * dnym1;
    for (i = 1; i <= grid_points[0]-2; i++) {
      xi = (double)i * dnxm1;

      for (k = 0; k <= grid_points[2]-1; k++) {
        zeta = (double)k * dnzm1;

        exact_solution(xi, eta, zeta, dtemp);
        for (m = 0; m < 5; m++) {
          ue[k][m] = dtemp[m];
        }

        dtpp = 1.0/dtemp[0];

        for (m = 1; m < 5; m++) {
          buf[k][m] = dtpp * dtemp[m];
        }

        cuf[k]    = buf[k][3] * buf[k][3];
        buf[k][0] = cuf[k] + buf[k][1] * buf[k][1] + buf[k][2] * buf[k][2];
        q[k] = 0.5*(buf[k][1]*ue[k][1] + buf[k][2]*ue[k][2] +
                    buf[k][3]*ue[k][3]);
      }

      for (k = 1; k <= grid_points[2]-2; k++) {
        km1 = k-1;
        kp1 = k+1;

        forcing[j][0][k][i] = forcing[j][0][k][i] -
          tz2*( ue[kp1][3]-ue[km1][3] )+
          dz1tz1*(ue[kp1][0]-2.0*ue[k][0]+ue[km1][0]);

        forcing[j][1][k][i] = forcing[j][1][k][i] - tz2 * (
            ue[kp1][1]*buf[kp1][3]-ue[km1][1]*buf[km1][3])+
          zzcon2*(buf[kp1][1]-2.0*buf[k][1]+buf[km1][1])+
          dz2tz1*( ue[kp1][1]-2.0* ue[k][1]+ ue[km1][1]);

        forcing[j][2][k][i] = forcing[j][2][k][i] - tz2 * (
            ue[kp1][2]*buf[kp1][3]-ue[km1][2]*buf[km1][3])+
          zzcon2*(buf[kp1][2]-2.0*buf[k][2]+buf[km1][2])+
          dz3tz1*(ue[kp1][2]-2.0*ue[k][2]+ue[km1][2]);

        forcing[j][3][k][i] = forcing[j][3][k][i] - tz2 * (
            (ue[kp1][3]*buf[kp1][3]+c2*(ue[kp1][4]-q[kp1]))-
            (ue[km1][3]*buf[km1][3]+c2*(ue[km1][4]-q[km1])))+
          zzcon1*(buf[kp1][3]-2.0*buf[k][3]+buf[km1][3])+
          dz4tz1*( ue[kp1][3]-2.0*ue[k][3] +ue[km1][3]);

        forcing[j][4][k][i] = forcing[j][4][k][i] - tz2 * (
            buf[kp1][3]*(c1*ue[kp1][4]-c2*q[kp1])-
            buf[km1][3]*(c1*ue[km1][4]-c2*q[km1]))+
          0.5*zzcon3*(buf[kp1][0]-2.0*buf[k][0]+buf[km1][0])+
          zzcon4*(cuf[kp1]-2.0*cuf[k]+cuf[km1])+
          zzcon5*(buf[kp1][4]-2.0*buf[k][4]+buf[km1][4])+
          dz5tz1*( ue[kp1][4]-2.0*ue[k][4]+ ue[km1][4]);
      }

      //---------------------------------------------------------------------
      // Fourth-order dissipation
      //---------------------------------------------------------------------
      for (m = 0; m < 5; m++) {
        k = 1;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (5.0*ue[k][m] - 4.0*ue[k+1][m] +ue[k+2][m]);
        k = 2;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (-4.0*ue[k-1][m] + 6.0*ue[k][m] -
            4.0*ue[k+1][m] +     ue[k+2][m]);
      }

      for (k = 3; k <= grid_points[2]-4; k++) {
        for (m = 0; m < 5; m++) {
          forcing[j][m][k][i] = forcing[j][m][k][i] - dssp*
            (ue[k-2][m] - 4.0*ue[k-1][m] +
             6.0*ue[k][m] - 4.0*ue[k+1][m] + ue[k+2][m]);
        }
      }

      for (m = 0; m < 5; m++) {
        k = grid_points[2]-3;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (ue[k-2][m] - 4.0*ue[k-1][m] +
           6.0*ue[k][m] - 4.0*ue[k+1][m]);
        k = grid_points[2]-2;
        forcing[j][m][k][i] = forcing[j][m][k][i] - dssp *
          (ue[k-2][m] - 4.0*ue[k-1][m] + 5.0*ue[k][m]);
      }

    }
  }

  //---------------------------------------------------------------------
  // now change the sign of the forcing function, 
  //---------------------------------------------------------------------
  for (k = 1; k <= grid_points[2]-2; k++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (i = 1; i <= grid_points[0]-2; i++) {
        for (m = 0; m < 5; m++) {
          forcing[j][m][k][i] = -1.0 * forcing[j][m][k][i];
        }
      }
    }
  }

}

// This subroutine initializes the field variable u using 
// tri-linear transfinite interpolation of the boundary values     
//---------------------------------------------------------------------
void initialize()
{
  int i, j, k, m, ix, iy, iz;
  double xi, eta, zeta, Pface[2][3][5], Pxi, Peta, Pzeta, temp[5];

  //---------------------------------------------------------------------
  //  Later (in compute_rhs) we compute 1/u for every element. A few of 
  //  the corner elements are not used, but it convenient (and faster) 
  //  to compute the whole thing with a simple loop. Make sure those 
  //  values are nonzero by initializing the whole thing here. 
  //---------------------------------------------------------------------
  for (k = 0; k <= grid_points[2]-1; k++) {
    for (j = 0; j <= grid_points[1]-1; j++) {
      for (i = 0; i <= grid_points[0]-1; i++) {
        u[0][k][j][i] = 1.0;
        u[1][k][j][i] = 0.0;
        u[2][k][j][i] = 0.0;
        u[3][k][j][i] = 0.0;
        u[4][k][j][i] = 1.0;
      }
    }
  }

  //---------------------------------------------------------------------
  // first store the "interpolated" values everywhere on the grid    
  //---------------------------------------------------------------------
  for (k = 0; k <= grid_points[2]-1; k++) {
    zeta = (double)k * dnzm1;
    for (j = 0; j <= grid_points[1]-1; j++) {
      eta = (double)j * dnym1;
      for (i = 0; i <= grid_points[0]-1; i++) {
        xi = (double)i * dnxm1;

        for (ix = 0; ix < 2; ix++) {
          Pxi = (double)ix;
          exact_solution(Pxi, eta, zeta, &Pface[ix][0][0]);
        }

        for (iy = 0; iy < 2; iy++) {
          Peta = (double)iy;
          exact_solution(xi, Peta, zeta, &Pface[iy][1][0]);
        }

        for (iz = 0; iz < 2; iz++) {
          Pzeta = (double)iz;
          exact_solution(xi, eta, Pzeta, &Pface[iz][2][0]);
        }

        for (m = 0; m < 5; m++) {
          Pxi   = xi   * Pface[1][0][m] + (1.0-xi)   * Pface[0][0][m];
          Peta  = eta  * Pface[1][1][m] + (1.0-eta)  * Pface[0][1][m];
          Pzeta = zeta * Pface[1][2][m] + (1.0-zeta) * Pface[0][2][m];

          u[m][k][j][i] = Pxi + Peta + Pzeta - 
                          Pxi*Peta - Pxi*Pzeta - Peta*Pzeta + 
                          Pxi*Peta*Pzeta;
        }
      }
    }
  }


  //---------------------------------------------------------------------
  // now store the exact values on the boundaries        
  //---------------------------------------------------------------------

  //---------------------------------------------------------------------
  // west face                                                  
  //---------------------------------------------------------------------
  xi = 0.0;
  i  = 0;
  for (k = 0; k <= grid_points[2]-1; k++) {
    zeta = (double)k * dnzm1;
    for (j = 0; j <= grid_points[1]-1; j++) {
      eta = (double)j * dnym1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
        u[m][k][j][i] = temp[m];
      }
    }
  }

  //---------------------------------------------------------------------
  // east face                                                      
  //---------------------------------------------------------------------
  xi = 1.0;
  i  = grid_points[0]-1;
  for (k = 0; k <= grid_points[2]-1; k++) {
    zeta = (double)k * dnzm1;
    for (j = 0; j <= grid_points[1]-1; j++) {
      eta = (double)j * dnym1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
        u[m][k][j][i] = temp[m];
      }
    }
  }

  //---------------------------------------------------------------------
  // south face                                                 
  //---------------------------------------------------------------------
  eta = 0.0;
  j   = 0;
  for (k = 0; k <= grid_points[2]-1; k++) {
    zeta = (double)k * dnzm1;
    for (i = 0; i <= grid_points[0]-1; i++) {
      xi = (double)i * dnxm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
        u[m][k][j][i] = temp[m];
      }
    }
  }

  //---------------------------------------------------------------------
  // north face                                    
  //---------------------------------------------------------------------
  eta = 1.0;
  j   = grid_points[1]-1;
  for (k = 0; k <= grid_points[2]-1; k++) {
    zeta = (double)k * dnzm1;
    for (i = 0; i <= grid_points[0]-1; i++) {
      xi = (double)i * dnxm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
        u[m][k][j][i] = temp[m];
      }
    }
  }

  //---------------------------------------------------------------------
  // bottom face                                       
  //---------------------------------------------------------------------
  zeta = 0.0;
  k    = 0;
  for (j = 0; j <= grid_points[1]-1; j++) {
    eta = (double)j * dnym1;
    for (i =0; i <= grid_points[0]-1; i++) {
      xi = (double)i * dnxm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
        u[m][k][j][i] = temp[m];
      }
    }
  }

  //---------------------------------------------------------------------
  // top face     
  //---------------------------------------------------------------------
  zeta = 1.0;
  k    = grid_points[2]-1;
  for (j = 0; j <= grid_points[1]-1; j++) {
    eta = (double)j * dnym1;
    for (i =0; i <= grid_points[0]-1; i++) {
      xi = (double)i * dnxm1;
      exact_solution(xi, eta, zeta, temp);
      for (m = 0; m < 5; m++) {
        u[m][k][j][i] = temp[m];
      }
    }
  }
}

// this function returns the exact solution at point xi, eta, zeta  
//---------------------------------------------------------------------
void exact_solution(double xi, double eta, double zeta, double dtemp[5])
{
  int m;

  for (m = 0; m < 5; m++) {
    dtemp[m] = ce[m][0] +
      xi  *(ce[m][1] + xi  *(ce[m][4] + xi  *(ce[m][7] + xi  *ce[m][10]))) +
      eta *(ce[m][2] + eta *(ce[m][5] + eta *(ce[m][8] + eta *ce[m][11]))) +
      zeta*(ce[m][3] + zeta*(ce[m][6] + zeta*(ce[m][9] + zeta*ce[m][12])));
  }
}

void lhsinit(int ni, int nj)
{
  int j, m;

  //---------------------------------------------------------------------
  // zap the whole left hand side for starters
  // set all diagonal values to 1. This is overkill, but convenient
  //---------------------------------------------------------------------
  for (j = 1; j <= nj; j++) {
    for (m = 0; m < 5; m++) {
      lhs [j][0][m] = 0.0;
      lhsp[j][0][m] = 0.0;
      lhsm[j][0][m] = 0.0;
      lhs [j][ni][m] = 0.0;
      lhsp[j][ni][m] = 0.0;
      lhsm[j][ni][m] = 0.0;
    }
    lhs [j][0][2] = 1.0;
    lhsp[j][0][2] = 1.0;
    lhsm[j][0][2] = 1.0;
    lhs [j][ni][2] = 1.0;
    lhsp[j][ni][2] = 1.0;
    lhsm[j][ni][2] = 1.0;
  }
}


void lhsinitj(int nj, int ni)
{
  int i, m;

  //---------------------------------------------------------------------
  // zap the whole left hand side for starters
  // set all diagonal values to 1. This is overkill, but convenient
  //---------------------------------------------------------------------
  for (i = 1; i <= ni; i++) {
    for (m = 0; m < 5; m++) {
      lhs [0][i][m] = 0.0;
      lhsp[0][i][m] = 0.0;
      lhsm[0][i][m] = 0.0;
      lhs [nj][i][m] = 0.0;
      lhsp[nj][i][m] = 0.0;
      lhsm[nj][i][m] = 0.0;
    }
    lhs [0][i][2] = 1.0;
    lhsp[0][i][2] = 1.0;
    lhsm[0][i][2] = 1.0;
    lhs [nj][i][2] = 1.0;
    lhsp[nj][i][2] = 1.0;
    lhsm[nj][i][2] = 1.0;
  }
}

void add()
{
  int i, j, k, m;

  if (timeron) timer_start(t_add);
#pragma acc kernels loop gang pcopy(u) pcopyin(rhs)
  for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
    for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
      for (i = 1; i <= nx2; i++) {
        for (m = 0; m < 5; m++) {
          u[m][k][j][i] = u[m][k][j][i] + rhs[m][k][j][i];
        }
      }
    }
  }
  if (timeron) timer_stop(t_add);
}

void adi()
{
  compute_rhs();

  txinvr();

  x_solve();

  y_solve();

  z_solve();

  add();
}


void error_norm(double rms[5])
{
  int i, j, k, m, d;
  double xi, eta, zeta, u_exact[5], add;

  for (m = 0; m < 5; m++) {
    rms[m] = 0.0;
  }

  for (k = 0; k <= grid_points[2]-1; k++) {
    zeta = (double)k * dnzm1;
    for (j = 0; j <= grid_points[1]-1; j++) {
      eta = (double)j * dnym1;
      for (i = 0; i <= grid_points[0]-1; i++) {
        xi = (double)i * dnxm1;
        exact_solution(xi, eta, zeta, u_exact);

        for (m = 0; m < 5; m++) {
          add = u[m][k][j][i]-u_exact[m];
          rms[m] = rms[m] + add*add;
        }
      }
    }
  }

  for (m = 0; m < 5; m++) {
    for (d = 0; d < 3; d++) {
      rms[m] = rms[m] / (double)(grid_points[d]-2);
    }
    rms[m] = sqrt(rms[m]);
  }
}


void rhs_norm(double rms[5])
{
  int i, j, k, d, m;
  double add;

  for (m = 0; m < 5; m++) {
    rms[m] = 0.0;
  }

  for (k = 1; k <= nz2; k++) {
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
        for (m = 0; m < 5; m++) {
          add = rhs[m][k][j][i];
          rms[m] = rms[m] + add*add;
        } 
      } 
    } 
  } 

  for (m = 0; m < 5; m++) {
    for (d = 0; d < 3; d++) {
      rms[m] = rms[m] / (double)(grid_points[d]-2);
    }
    rms[m] = sqrt(rms[m]);
  }
}

void txinvr()
{
  int i, j, k;
  double t1, t2, t3, ac, ru1, uu, vv, ww, r1, r2, r3, r4, r5, ac2inv;

  if (timeron) timer_start(t_txinvr);
#pragma acc kernels pcopy(rhs), pcopyin(rho_i, us, vs, ws, speed, qs)
  {
  for (k = 1; k <= nz2; k++) {
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
        ru1 = rho_i[k][j][i];
        uu = us[k][j][i];
        vv = vs[k][j][i];
        ww = ws[k][j][i];
        ac = speed[k][j][i];
        ac2inv = ac*ac;

        r1 = rhs[0][k][j][i];
        r2 = rhs[1][k][j][i];
        r3 = rhs[2][k][j][i];
        r4 = rhs[3][k][j][i];
        r5 = rhs[4][k][j][i];

        t1 = c2 / ac2inv * ( qs[k][j][i]*r1 - uu*r2  - vv*r3 - ww*r4 + r5 );
        t2 = bt * ru1 * ( uu * r1 - r2 );
        t3 = ( bt * ru1 * ac ) * t1;

        rhs[0][k][j][i] = r1 - t1;
        rhs[1][k][j][i] = - ru1 * ( ww*r1 - r4 );
        rhs[2][k][j][i] =   ru1 * ( vv*r1 - r3 );
        rhs[3][k][j][i] = - t2 + t3;
        rhs[4][k][j][i] =   t2 + t3;
      }
    }
  }
  }
  if (timeron) timer_stop(t_txinvr);
}

void ninvr()
{
  int i, j, k;
  double r1, r2, r3, r4, r5, t1, t2;

  if (timeron) timer_start(t_ninvr);
#pragma acc kernels pcopy(rhs)
  {
  for (k = 1; k <= nz2; k++) {
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
        r1 = rhs[0][k][j][i];
        r2 = rhs[1][k][j][i];
        r3 = rhs[2][k][j][i];
        r4 = rhs[3][k][j][i];
        r5 = rhs[4][k][j][i];

        t1 = bt * r3;
        t2 = 0.5 * ( r4 + r5 );

        rhs[0][k][j][i] = -r2;
        rhs[1][k][j][i] =  r1;
        rhs[2][k][j][i] = bt * ( r4 - r5 );
        rhs[3][k][j][i] = -t1 + t2;
        rhs[4][k][j][i] =  t1 + t2;
      }
    }
  }
  }
  if (timeron) timer_stop(t_ninvr);
}

void pinvr()
{
  int i, j, k;
  double r1, r2, r3, r4, r5, t1, t2;

  if (timeron) timer_start(t_pinvr);

#pragma acc kernels pcopy(rhs)
  {
  for (k = 1; k <= nz2; k++) {
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
        r1 = rhs[0][k][j][i];
        r2 = rhs[1][k][j][i];
        r3 = rhs[2][k][j][i];
        r4 = rhs[3][k][j][i];
        r5 = rhs[4][k][j][i];

        t1 = bt * r1;
        t2 = 0.5 * ( r4 + r5 );

        rhs[0][k][j][i] =  bt * ( r4 - r5 );
        rhs[1][k][j][i] = -r3;
        rhs[2][k][j][i] =  r2;
        rhs[3][k][j][i] = -t1 + t2;
        rhs[4][k][j][i] =  t1 + t2;
      }
    }
  }
  }
  if (timeron) timer_stop(t_pinvr);
}

void tzetar()
{
  int i, j, k;
  double t1, t2, t3, ac, xvel, yvel, zvel, r1, r2, r3, r4, r5;
  double btuz, ac2u, uzik1;

  if (timeron) timer_start(t_tzetar);

#pragma acc kernels pcopyin(us,vs,ws,u) pcopy(rhs)
  {
  for (k = 1; k <= nz2; k++) {
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
        xvel = us[k][j][i];
        yvel = vs[k][j][i];
        zvel = ws[k][j][i];
        ac   = speed[k][j][i];

        ac2u = ac*ac;

        r1 = rhs[0][k][j][i];
        r2 = rhs[1][k][j][i];
        r3 = rhs[2][k][j][i];
        r4 = rhs[3][k][j][i];
        r5 = rhs[4][k][j][i];     

        uzik1 = u[0][k][j][i];
        btuz  = bt * uzik1;

        t1 = btuz/ac * (r4 + r5);
        t2 = r3 + t1;
        t3 = btuz * (r4 - r5);

        rhs[0][k][j][i] = t2;
        rhs[1][k][j][i] = -uzik1*r2 + xvel*t2;
        rhs[2][k][j][i] =  uzik1*r1 + yvel*t2;
        rhs[3][k][j][i] =  zvel*t2  + t3;
        rhs[4][k][j][i] =  uzik1*(-xvel*r2 + yvel*r1) + 
                           qs[k][j][i]*t2 + c2iv*ac2u*t1 + zvel*t3;
      }
    }
  }
  }
  if (timeron) timer_stop(t_tzetar);
}

// this function performs the solution of the approximate factorization
// step in the x-direction for all five matrix components
// simultaneously. The Thomas algorithm is employed to solve the
// systems for the x-lines. Boundary conditions are non-periodic
void x_solve()
{
  int i, j, k, i1, i2, m;
  double ru1, fac1, fac2;
  int ni = nx2+1;
  int gp0;

  double lhsX[5][KMAX][IMAXP+1][JMAXP+1];
  double lhspX[5][KMAX][IMAXP+1][JMAXP+1];
  double lhsmX[5][KMAX][IMAXP+1][JMAXP+1];
  double rhonX[KMAX][PROBLEM_SIZE][JMAXP+1];
  double rhsX[5][nz2+1][IMAXP+1][JMAXP+1];
  
  gp0 = grid_points[0];

  if (timeron) timer_start(t_xsolve);

#pragma acc data pcopy(rhs) pcopyin(us, speed, rho_i) create(lhsX,lhsmX,lhspX,rhonX,rhsX)
  {

#pragma acc kernels loop
  for (k = 0; k <= nz2; k++) {
    for (j = 0; j <= JMAXP; j++) {
      for (i = 0; i <= IMAXP; i++) {
          rhsX[0][k][i][j] = rhs[0][k][j][i];
          rhsX[1][k][i][j] = rhs[1][k][j][i];
          rhsX[2][k][i][j] = rhs[2][k][j][i];
          rhsX[3][k][i][j] = rhs[3][k][j][i];
          rhsX[4][k][i][j] = rhs[4][k][j][i];
      }
    }
  }

#pragma acc kernels
  {
  for (k = 1; k <= nz2; k++) {
    //lhsinit(nx2+1, ny2);
    for (j = 1; j <= ny2; j++) {
      for (m = 0; m < 5; m++) {
	lhsX[m][k][0][j] = 0.0;
	lhspX[m][k][0][j] = 0.0;
	lhsmX[m][k][0][j] = 0.0;
	lhsX[m][k][ni][j] = 0.0;
	lhspX[m][k][ni][j] = 0.0;
	lhsmX[m][k][ni][j] = 0.0;
      }
      lhsX[2][k][0][j] = 1.0;
      lhspX[2][k][0][j] = 1.0;
      lhsmX[2][k][0][j] = 1.0;
      lhsX[2][k][ni][j] = 1.0;
      lhspX[2][k][ni][j] = 1.0;
      lhsmX[2][k][ni][j] = 1.0;
    }
  

    //---------------------------------------------------------------------
    // Computes the left hand side for the three x-factors  
    //---------------------------------------------------------------------

    //---------------------------------------------------------------------
    // first fill the lhs for the u-eigenvalue                   
    //---------------------------------------------------------------------

    for (j = 1; j <= ny2; j++) {
      for (i = 0; i <= gp0-1; i++) {
        ru1 = c3c4*rho_i[k][j][i];
        //cv[i] = us[k][j][i];
        rhonX[k][i][j] = max(max(dx2+con43*ru1,dx5+c1c5*ru1), max(dxmax+ru1,dx1));
      }

      for (i = 1; i <= nx2; i++) {
        lhsX[0][k][i][j] =  0.0;
        //lhsX[1][k][i][j] = -dttx2 * cv[i-1] - dttx1 * rhonX[k][i-1][j];
        //lhsX[2][k][i][j] =  1.0 + c2dttx1 * rhonX[k][i][j];
        //lhsX[3][k][i][j] =  dttx2 * cv[i+1] - dttx1 * rhonX[k][i+1][j];
        lhsX[1][k][i][j] = -dttx2 * us[k][j][i-1] - dttx1 * rhonX[k][i-1][j];
        lhsX[2][k][i][j] =  1.0 + c2dttx1 * rhonX[k][i][j];
        lhsX[3][k][i][j] =  dttx2 * us[k][j][i+1] - dttx1 * rhonX[k][i+1][j];
        lhsX[4][k][i][j] =  0.0;
      }
    }

    //---------------------------------------------------------------------
    // add fourth order dissipation                             
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      i = 1;
      lhsX[2][k][i][j] = lhsX[2][k][i][j] + comz5;
      lhsX[3][k][i][j] = lhsX[3][k][i][j] - comz4;
      lhsX[4][k][i][j] = lhsX[4][k][i][j] + comz1;

      lhsX[1][k][i+1][j] = lhsX[1][k][i+1][j] - comz4;
      lhsX[2][k][i+1][j] = lhsX[2][k][i+1][j] + comz6;
      lhsX[3][k][i+1][j] = lhsX[3][k][i+1][j] - comz4;
      lhsX[4][k][i+1][j] = lhsX[4][k][i+1][j] + comz1;
    }

    for (j = 1; j <= ny2; j++) {
      for (i = 3; i <= gp0-4; i++) {
        lhsX[0][k][i][j] = lhsX[0][k][i][j] + comz1;
        lhsX[1][k][i][j] = lhsX[1][k][i][j] - comz4;
        lhsX[2][k][i][j] = lhsX[2][k][i][j] + comz6;
        lhsX[3][k][i][j] = lhsX[3][k][i][j] - comz4;
        lhsX[4][k][i][j] = lhsX[4][k][i][j] + comz1;
      }
    }

    for (j = 1; j <= ny2; j++) {
      i = gp0-3;
      lhsX[0][k][i][j] = lhsX[0][k][i][j] + comz1;
      lhsX[1][k][i][j] = lhsX[1][k][i][j] - comz4;
      lhsX[2][k][i][j] = lhsX[2][k][i][j] + comz6;
      lhsX[3][k][i][j] = lhsX[3][k][i][j] - comz4;

      lhsX[0][k][i+1][j] = lhsX[0][k][i+1][j] + comz1;
      lhsX[1][k][i+1][j] = lhsX[1][k][i+1][j] - comz4;
      lhsX[2][k][i+1][j] = lhsX[2][k][i+1][j] + comz5;
    }

    //---------------------------------------------------------------------
    // subsequently, fill the other factors (u+c), (u-c) by adding to 
    // the first  
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
        lhspX[0][k][i][j] = lhsX[0][k][i][j];
        lhspX[1][k][i][j] = lhsX[1][k][i][j] - dttx2 * speed[k][j][i-1];
        lhspX[2][k][i][j] = lhsX[2][k][i][j];
        lhspX[3][k][i][j] = lhsX[3][k][i][j] + dttx2 * speed[k][j][i+1];
        lhspX[4][k][i][j] = lhsX[4][k][i][j];
        lhsmX[0][k][i][j] = lhsX[0][k][i][j];
        lhsmX[1][k][i][j] = lhsX[1][k][i][j] + dttx2 * speed[k][j][i-1];
        lhsmX[2][k][i][j] = lhsX[2][k][i][j];
        lhsmX[3][k][i][j] = lhsX[3][k][i][j] - dttx2 * speed[k][j][i+1];
        lhsmX[4][k][i][j] = lhsX[4][k][i][j];
      }
    }

    //---------------------------------------------------------------------
    // FORWARD ELIMINATION  
    //---------------------------------------------------------------------

    //---------------------------------------------------------------------
    // perform the Thomas algorithm; first, FORWARD ELIMINATION     
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      for (i = 0; i <= gp0-3; i++) {
        i1 = i + 1;
        i2 = i + 2;
        fac1 = 1.0/lhsX[2][k][i][j];
        lhsX[3][k][i][j] = fac1*lhsX[3][k][i][j];
        lhsX[4][k][i][j] = fac1*lhsX[4][k][i][j];
        for (m = 0; m < 3; m++) {
          rhsX[m][k][i][j] = fac1*rhsX[m][k][i][j];
        }
        lhsX[2][k][i1][j] = lhsX[2][k][i1][j] - lhsX[1][k][i1][j]*lhsX[3][k][i][j];
        lhsX[3][k][i1][j] = lhsX[3][k][i1][j] - lhsX[1][k][i1][j]*lhsX[4][k][i][j];
        for (m = 0; m < 3; m++) {
          rhsX[m][k][i1][j] = rhsX[m][k][i1][j] - lhsX[1][k][i1][j]*rhsX[m][k][i][j];
        }
        lhsX[1][k][i2][j] = lhsX[1][k][i2][j] - lhsX[0][k][i2][j]*lhsX[3][k][i][j];
        lhsX[2][k][i2][j] = lhsX[2][k][i2][j] - lhsX[0][k][i2][j]*lhsX[4][k][i][j];
        for (m = 0; m < 3; m++) {
          rhsX[m][k][i2][j] = rhsX[m][k][i2][j] - lhsX[0][k][i2][j]*rhsX[m][k][i][j];
        }
      }
    }

    //---------------------------------------------------------------------
    // The last two rows in this grid block are a bit different, 
    // since they for (not have two more rows available for the
    // elimination of off-diagonal entries
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      i  = gp0-2;
      i1 = gp0-1;
      fac1 = 1.0/lhsX[2][k][i][j];
      lhsX[3][k][i][j] = fac1*lhsX[3][k][i][j];
      lhsX[4][k][i][j] = fac1*lhsX[4][k][i][j];
      for (m = 0; m < 3; m++) {
        rhsX[m][k][i][j] = fac1*rhsX[m][k][i][j];
      }
      lhsX[2][k][i1][j] = lhsX[2][k][i1][j] - lhsX[1][k][i1][j]*lhsX[3][k][i][j];
      lhsX[3][k][i1][j] = lhsX[3][k][i1][j] - lhsX[1][k][i1][j]*lhsX[4][k][i][j];
      for (m = 0; m < 3; m++) {
        rhsX[m][k][i1][j] = rhsX[m][k][i1][j] - lhsX[1][k][i1][j]*rhsX[m][k][i][j];
      }

      //---------------------------------------------------------------------
      // scale the last row immediately 
      //---------------------------------------------------------------------
      fac2 = 1.0/lhsX[2][k][i1][j];
      for (m = 0; m < 3; m++) {
        rhsX[m][k][i1][j] = fac2*rhsX[m][k][i1][j];
      }
    }

    //---------------------------------------------------------------------
    // for (the u+c and the u-c factors                 
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      for (i = 0; i <= gp0-3; i++) {
        i1 = i + 1;
        i2 = i + 2;

        m = 3;
        fac1 = 1.0/lhspX[2][k][i][j];
        lhspX[3][k][i][j]    = fac1*lhspX[3][k][i][j];
        lhspX[4][k][i][j]    = fac1*lhspX[4][k][i][j];
        rhsX[m][k][i][j]  = fac1*rhsX[m][k][i][j];
        lhspX[2][k][i1][j]   = lhspX[2][k][i1][j] - lhspX[1][k][i1][j]*lhspX[3][k][i][j];
        lhspX[3][k][i1][j]   = lhspX[3][k][i1][j] - lhspX[1][k][i1][j]*lhspX[4][k][i][j];
        rhsX[m][k][i1][j] = rhsX[m][k][i1][j] - lhspX[1][k][i1][j]*rhsX[m][k][i][j];
        lhspX[1][k][i2][j]   = lhspX[1][k][i2][j] - lhspX[0][k][i2][j]*lhspX[3][k][i][j];
        lhspX[2][k][i2][j]   = lhspX[2][k][i2][j] - lhspX[0][k][i2][j]*lhspX[4][k][i][j];
        rhsX[m][k][i2][j] = rhsX[m][k][i2][j] - lhspX[0][k][i2][j]*rhsX[m][k][i][j];

        m = 4;
        fac1 = 1.0/lhsmX[2][k][i][j];
        lhsmX[3][k][i][j]    = fac1*lhsmX[3][k][i][j];
        lhsmX[4][k][i][j]    = fac1*lhsmX[4][k][i][j];
        rhsX[m][k][i][j]  = fac1*rhsX[m][k][i][j];
        lhsmX[2][k][i1][j]   = lhsmX[2][k][i1][j] - lhsmX[1][k][i1][j]*lhsmX[3][k][i][j];
        lhsmX[3][k][i1][j]   = lhsmX[3][k][i1][j] - lhsmX[1][k][i1][j]*lhsmX[4][k][i][j];
        rhsX[m][k][i1][j] = rhsX[m][k][i1][j] - lhsmX[1][k][i1][j]*rhsX[m][k][i][j];
        lhsmX[1][k][i2][j]   = lhsmX[1][k][i2][j] - lhsmX[0][k][i2][j]*lhsmX[3][k][i][j];
        lhsmX[2][k][i2][j]   = lhsmX[2][k][i2][j] - lhsmX[0][k][i2][j]*lhsmX[4][k][i][j];
        rhsX[m][k][i2][j] = rhsX[m][k][i2][j] - lhsmX[0][k][i2][j]*rhsX[m][k][i][j];
      }
    }

    //---------------------------------------------------------------------
    // And again the last two rows separately
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      i  = gp0-2;
      i1 = gp0-1;

      m = 3;
      fac1 = 1.0/lhspX[2][k][i][j];
      lhspX[3][k][i][j]    = fac1*lhspX[3][k][i][j];
      lhspX[4][k][i][j]    = fac1*lhspX[4][k][i][j];
      rhsX[m][k][i][j]  = fac1*rhsX[m][k][i][j];
      lhspX[2][k][i1][j]   = lhspX[2][k][i1][j] - lhspX[1][k][i1][j]*lhspX[3][k][i][j];
      lhspX[3][k][i1][j]   = lhspX[3][k][i1][j] - lhspX[1][k][i1][j]*lhspX[4][k][i][j];
      rhsX[m][k][i1][j] = rhsX[m][k][i1][j] - lhspX[1][k][i1][j]*rhsX[m][k][i][j];

      m = 4;
      fac1 = 1.0/lhsmX[2][k][i][j];
      lhsmX[3][k][i][j]    = fac1*lhsmX[3][k][i][j];
      lhsmX[4][k][i][j]    = fac1*lhsmX[4][k][i][j];
      rhsX[m][k][i][j]  = fac1*rhsX[m][k][i][j];
      lhsmX[2][k][i1][j]   = lhsmX[2][k][i1][j] - lhsmX[1][k][i1][j]*lhsmX[3][k][i][j];
      lhsmX[3][k][i1][j]   = lhsmX[3][k][i1][j] - lhsmX[1][k][i1][j]*lhsmX[4][k][i][j];
      rhsX[m][k][i1][j] = rhsX[m][k][i1][j] - lhsmX[1][k][i1][j]*rhsX[m][k][i][j];

      //---------------------------------------------------------------------
      // Scale the last row immediately
      //---------------------------------------------------------------------
      rhsX[3][k][i1][j] = rhsX[3][k][i1][j]/lhspX[2][k][i1][j];
      rhsX[4][k][i1][j] = rhsX[4][k][i1][j]/lhsmX[2][k][i1][j];
    }

    //---------------------------------------------------------------------
    // BACKSUBSTITUTION 
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      i  = gp0-2;
      i1 = gp0-1;
      for (m = 0; m < 3; m++) {
        rhsX[m][k][i][j] = rhsX[m][k][i][j] - lhsX[3][k][i][j]*rhsX[m][k][i1][j];
      }

      rhsX[3][k][i][j] = rhsX[3][k][i][j] - lhspX[3][k][i][j]*rhsX[3][k][i1][j];
      rhsX[4][k][i][j] = rhsX[4][k][i][j] - lhsmX[3][k][i][j]*rhsX[4][k][i1][j];
    }

    //---------------------------------------------------------------------
    // The first three factors
    //---------------------------------------------------------------------
    for (j = 1; j <= ny2; j++) {
      for (i = gp0-3; i >= 0; i--) {
        i1 = i + 1;
        i2 = i + 2;
        for (m = 0; m < 3; m++) {
          rhsX[m][k][i][j] = rhsX[m][k][i][j] - 
                            lhsX[3][k][i][j]*rhsX[m][k][i1][j] -
                            lhsX[4][k][i][j]*rhsX[m][k][i2][j];
        }

        //-------------------------------------------------------------------
        // And the remaining two
        //-------------------------------------------------------------------
        rhsX[3][k][i][j] = rhsX[3][k][i][j] - 
                          lhspX[3][k][i][j]*rhsX[3][k][i1][j] -
                          lhspX[4][k][i][j]*rhsX[3][k][i2][j];
        rhsX[4][k][i][j] = rhsX[4][k][i][j] - 
                          lhsmX[3][k][i][j]*rhsX[4][k][i1][j] -
                          lhsmX[4][k][i][j]*rhsX[4][k][i2][j];
      }
    }
  }

  }  // end openacc parallel region

#pragma acc kernels loop gang
  for (k = 0; k <= nz2; k++) {
#pragma acc loop gang vector
    for (j = 0; j <= JMAXP; j++) {
#pragma acc loop vector
      for (i = 0; i <= IMAXP; i++) {
          rhs[0][k][j][i] = rhsX[0][k][i][j];
          rhs[1][k][j][i] = rhsX[1][k][i][j];
          rhs[2][k][j][i] = rhsX[2][k][i][j];
          rhs[3][k][j][i] = rhsX[3][k][i][j];
          rhs[4][k][j][i] = rhsX[4][k][i][j];
      }
    }
  }
  if (timeron) timer_stop(t_xsolve);

  

  //---------------------------------------------------------------------
  // Do the block-diagonal inversion          
  //---------------------------------------------------------------------
  ninvr();

  }
}

// this function performs the solution of the approximate factorization
// step in the y-direction for all five matrix components
// simultaneously. The Thomas algorithm is employed to solve the
// systems for the y-lines. Boundary conditions are non-periodic
void y_solve()
{
  int i, j, k, j1, j2, m, ni, nj;
  double ru1, fac1, fac2;

  int gp0, gp1, gp2;
  double lhsY[5][KMAX][JMAXP+1][IMAXP+1];
  double lhspY[5][KMAX][JMAXP+1][IMAXP+1];
  double lhsmY[5][KMAX][JMAXP+1][IMAXP+1];
  double rhoqY[KMAX][JMAXP+1][PROBLEM_SIZE];
  
  gp0 = grid_points[0];
  gp1 = grid_points[1];
  gp2 = grid_points[2];

  nj = ny2+1;
  ni = nx2;

  if (timeron) timer_start(t_ysolve);

#pragma acc data pcopy(rhs) pcopyin(rho_i,vs,speed) create(lhsY,lhspY,lhsmY,rhoqY)
  {
#pragma acc kernels
  {
  for (k = 1; k <= gp2-2; k++) {
    //lhsinitj(ny2+1, nx2);
    for (i = 1; i <= ni; i++) {
      for (m = 0; m < 5; m++) {
	lhsY[m][k][0][i] = 0.0;
	lhspY[m][k][0][i] = 0.0;
	lhsmY[m][k][0][i] = 0.0;
	lhsY[m][k][nj][i] = 0.0;
	lhspY[m][k][nj][i] = 0.0;
	lhsmY[m][k][nj][i] = 0.0;
      }
      lhsY[2][k][0][i] = 1.0;
      lhspY[2][k][0][i] = 1.0;
      lhsmY[2][k][0][i] = 1.0;
      lhsY[2][k][nj][i] = 1.0;
      lhspY[2][k][nj][i] = 1.0;
      lhsmY[2][k][nj][i] = 1.0;
    }

    //---------------------------------------------------------------------
    // Computes the left hand side for the three y-factors   
    //---------------------------------------------------------------------

    //---------------------------------------------------------------------
    // first fill the lhs for the u-eigenvalue         
    //---------------------------------------------------------------------
    for (i = 1; i <= gp0-2; i++) {
      for (j = 0; j <= gp1-1; j++) {
        ru1 = c3c4*rho_i[k][j][i];
        //cv[j] = vs[k][j][i];
        rhoqY[k][i][j] = max(max(dy3+con43*ru1, dy5+c1c5*ru1), max(dymax+ru1, dy1));
      }

      for (j = 1; j <= gp1-2; j++) {
        lhsY[0][k][j][i] =  0.0;
        //lhsY[1][k][j][i] = -dtty2 * cv[j-1] - dtty1 * rhoqY[k][i][j-1];
        //lhsY[2][k][j][i] =  1.0 + c2dtty1 * rhoqY[k][i][j];
        //lhsY[3][k][j][i] =  dtty2 * cv[j+1] - dtty1 * rhoqY[k][i][j+1];
        lhsY[1][k][j][i] = -dtty2 * vs[k][j-1][i] - dtty1 * rhoqY[k][i][j-1];
        lhsY[2][k][j][i] =  1.0 + c2dtty1 * rhoqY[k][i][j];
        lhsY[3][k][j][i] =  dtty2 * vs[k][j+1][i] - dtty1 * rhoqY[k][i][j+1];
        lhsY[4][k][j][i] =  0.0;
      }
    }

    //---------------------------------------------------------------------
    // add fourth order dissipation                             
    //---------------------------------------------------------------------
    for (i = 1; i <= gp0-2; i++) {
      j = 1;
      lhsY[2][k][j][i] = lhsY[2][k][j][i] + comz5;
      lhsY[3][k][j][i] = lhsY[3][k][j][i] - comz4;
      lhsY[4][k][j][i] = lhsY[4][k][j][i] + comz1;

      lhsY[1][k][j+1][i] = lhsY[1][k][j+1][i] - comz4;
      lhsY[2][k][j+1][i] = lhsY[2][k][j+1][i] + comz6;
      lhsY[3][k][j+1][i] = lhsY[3][k][j+1][i] - comz4;
      lhsY[4][k][j+1][i] = lhsY[4][k][j+1][i] + comz1;
    }

    for (j = 3; j <= gp1-4; j++) {
      for (i = 1; i <= gp0-2; i++) {
        lhsY[0][k][j][i] = lhsY[0][k][j][i] + comz1;
        lhsY[1][k][j][i] = lhsY[1][k][j][i] - comz4;
        lhsY[2][k][j][i] = lhsY[2][k][j][i] + comz6;
        lhsY[3][k][j][i] = lhsY[3][k][j][i] - comz4;
        lhsY[4][k][j][i] = lhsY[4][k][j][i] + comz1;
      }
    }

    for (i = 1; i <= gp0-2; i++) {
      j = gp1-3;
      lhsY[0][k][j][i] = lhsY[0][k][j][i] + comz1;
      lhsY[1][k][j][i] = lhsY[1][k][j][i] - comz4;
      lhsY[2][k][j][i] = lhsY[2][k][j][i] + comz6;
      lhsY[3][k][j][i] = lhsY[3][k][j][i] - comz4;

      lhsY[0][k][j+1][i] = lhsY[0][k][j+1][i] + comz1;
      lhsY[1][k][j+1][i] = lhsY[1][k][j+1][i] - comz4;
      lhsY[2][k][j+1][i] = lhsY[2][k][j+1][i] + comz5;
    }

    //---------------------------------------------------------------------
    // subsequently, for (the other two factors                    
    //---------------------------------------------------------------------
    for (j = 1; j <= gp1-2; j++) {
      for (i = 1; i <= gp0-2; i++) {
        lhspY[0][k][j][i] = lhsY[0][k][j][i];
        lhspY[1][k][j][i] = lhsY[1][k][j][i] - dtty2 * speed[k][j-1][i];
        lhspY[2][k][j][i] = lhsY[2][k][j][i];
        lhspY[3][k][j][i] = lhsY[3][k][j][i] + dtty2 * speed[k][j+1][i];
        lhspY[4][k][j][i] = lhsY[4][k][j][i];
        lhsmY[0][k][j][i] = lhsY[0][k][j][i];
        lhsmY[1][k][j][i] = lhsY[1][k][j][i] + dtty2 * speed[k][j-1][i];
        lhsmY[2][k][j][i] = lhsY[2][k][j][i];
        lhsmY[3][k][j][i] = lhsY[3][k][j][i] - dtty2 * speed[k][j+1][i];
        lhsmY[4][k][j][i] = lhsY[4][k][j][i];
      }
    }


    //---------------------------------------------------------------------
    // FORWARD ELIMINATION  
    //---------------------------------------------------------------------
    for (j = 0; j <= gp1-3; j++) {
      j1 = j + 1;
      j2 = j + 2;
      for (i = 1; i <= gp0-2; i++) {
        fac1 = 1.0/lhsY[2][k][j][i];
        lhsY[3][k][j][i] = fac1*lhsY[3][k][j][i];
        lhsY[4][k][j][i] = fac1*lhsY[4][k][j][i];
        for (m = 0; m < 3; m++) {
          rhs[m][k][j][i] = fac1*rhs[m][k][j][i];
        }
        lhsY[2][k][j1][i] = lhsY[2][k][j1][i] - lhsY[1][k][j1][i]*lhsY[3][k][j][i];
        lhsY[3][k][j1][i] = lhsY[3][k][j1][i] - lhsY[1][k][j1][i]*lhsY[4][k][j][i];
        for (m = 0; m < 3; m++) {
          rhs[m][k][j1][i] = rhs[m][k][j1][i] - lhsY[1][k][j1][i]*rhs[m][k][j][i];
        }
        lhsY[1][k][j2][i] = lhsY[1][k][j2][i] - lhsY[0][k][j2][i]*lhsY[3][k][j][i];
        lhsY[2][k][j2][i] = lhsY[2][k][j2][i] - lhsY[0][k][j2][i]*lhsY[4][k][j][i];
        for (m = 0; m < 3; m++) {
          rhs[m][k][j2][i] = rhs[m][k][j2][i] - lhsY[0][k][j2][i]*rhs[m][k][j][i];
        }
      }
    }

    //---------------------------------------------------------------------
    // The last two rows in this grid block are a bit different, 
    // since they for (not have two more rows available for the
    // elimination of off-diagonal entries
    //---------------------------------------------------------------------
    j  = gp1-2;
    j1 = gp1-1;
    for (i = 1; i <= gp0-2; i++) {
      fac1 = 1.0/lhsY[2][k][j][i];
      lhsY[3][k][j][i] = fac1*lhsY[3][k][j][i];
      lhsY[4][k][j][i] = fac1*lhsY[4][k][j][i];
      for (m = 0; m < 3; m++) {
        rhs[m][k][j][i] = fac1*rhs[m][k][j][i];
      }
      lhsY[2][k][j1][i] = lhsY[2][k][j1][i] - lhsY[1][k][j1][i]*lhsY[3][k][j][i];
      lhsY[3][k][j1][i] = lhsY[3][k][j1][i] - lhsY[1][k][j1][i]*lhsY[4][k][j][i];
      for (m = 0; m < 3; m++) {
        rhs[m][k][j1][i] = rhs[m][k][j1][i] - lhsY[1][k][j1][i]*rhs[m][k][j][i];
      }
      //---------------------------------------------------------------------
      // scale the last row immediately 
      //---------------------------------------------------------------------
      fac2 = 1.0/lhsY[2][k][j1][i];
      for (m = 0; m < 3; m++) {
        rhs[m][k][j1][i] = fac2*rhs[m][k][j1][i];
      }
    }

    //---------------------------------------------------------------------
    // for (the u+c and the u-c factors                 
    //---------------------------------------------------------------------
    for (j = 0; j <= gp1-3; j++) {
      j1 = j + 1;
      j2 = j + 2;
      for (i = 1; i <= gp0-2; i++) {
        m = 3;
        fac1 = 1.0/lhspY[2][k][j][i];
        lhspY[3][k][j][i]    = fac1*lhspY[3][k][j][i];
        lhspY[4][k][j][i]    = fac1*lhspY[4][k][j][i];
        rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
        lhspY[2][k][j1][i]   = lhspY[2][k][j1][i] - lhspY[1][k][j1][i]*lhspY[3][k][j][i];
        lhspY[3][k][j1][i]   = lhspY[3][k][j1][i] - lhspY[1][k][j1][i]*lhspY[4][k][j][i];
        rhs[m][k][j1][i] = rhs[m][k][j1][i] - lhspY[1][k][j1][i]*rhs[m][k][j][i];
        lhspY[1][k][j2][i]   = lhspY[1][k][j2][i] - lhspY[0][k][j2][i]*lhspY[3][k][j][i];
        lhspY[2][k][j2][i]   = lhspY[2][k][j2][i] - lhspY[0][k][j2][i]*lhspY[4][k][j][i];
        rhs[m][k][j2][i] = rhs[m][k][j2][i] - lhspY[0][k][j2][i]*rhs[m][k][j][i];

        m = 4;
        fac1 = 1.0/lhsmY[2][k][j][i];
        lhsmY[3][k][j][i]    = fac1*lhsmY[3][k][j][i];
        lhsmY[4][k][j][i]    = fac1*lhsmY[4][k][j][i];
        rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
        lhsmY[2][k][j1][i]   = lhsmY[2][k][j1][i] - lhsmY[1][k][j1][i]*lhsmY[3][k][j][i];
        lhsmY[3][k][j1][i]   = lhsmY[3][k][j1][i] - lhsmY[1][k][j1][i]*lhsmY[4][k][j][i];
        rhs[m][k][j1][i] = rhs[m][k][j1][i] - lhsmY[1][k][j1][i]*rhs[m][k][j][i];
        lhsmY[1][k][j2][i]   = lhsmY[1][k][j2][i] - lhsmY[0][k][j2][i]*lhsmY[3][k][j][i];
        lhsmY[2][k][j2][i]   = lhsmY[2][k][j2][i] - lhsmY[0][k][j2][i]*lhsmY[4][k][j][i];
        rhs[m][k][j2][i] = rhs[m][k][j2][i] - lhsmY[0][k][j2][i]*rhs[m][k][j][i];
      }
    }

    //---------------------------------------------------------------------
    // And again the last two rows separately
    //---------------------------------------------------------------------
    j  = gp1-2;
    j1 = gp1-1;
    for (i = 1; i <= gp0-2; i++) {
      m = 3;
      fac1 = 1.0/lhspY[2][k][j][i];
      lhspY[3][k][j][i]    = fac1*lhspY[3][k][j][i];
      lhspY[4][k][j][i]    = fac1*lhspY[4][k][j][i];
      rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
      lhspY[2][k][j1][i]   = lhspY[2][k][j1][i] - lhspY[1][k][j1][i]*lhspY[3][k][j][i];
      lhspY[3][k][j1][i]   = lhspY[3][k][j1][i] - lhspY[1][k][j1][i]*lhspY[4][k][j][i];
      rhs[m][k][j1][i] = rhs[m][k][j1][i] - lhspY[1][k][j1][i]*rhs[m][k][j][i];

      m = 4;
      fac1 = 1.0/lhsmY[2][k][j][i];
      lhsmY[3][k][j][i]    = fac1*lhsmY[3][k][j][i];
      lhsmY[4][k][j][i]    = fac1*lhsmY[4][k][j][i];
      rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
      lhsmY[2][k][j1][i]   = lhsmY[2][k][j1][i] - lhsmY[1][k][j1][i]*lhsmY[3][k][j][i];
      lhsmY[3][k][j1][i]   = lhsmY[3][k][j1][i] - lhsmY[1][k][j1][i]*lhsmY[4][k][j][i];
      rhs[m][k][j1][i] = rhs[m][k][j1][i] - lhsmY[1][k][j1][i]*rhs[m][k][j][i];

      //---------------------------------------------------------------------
      // Scale the last row immediately 
      //---------------------------------------------------------------------
      rhs[3][k][j1][i]   = rhs[3][k][j1][i]/lhspY[2][k][j1][i];
      rhs[4][k][j1][i]   = rhs[4][k][j1][i]/lhsmY[2][k][j1][i];
    }


    //---------------------------------------------------------------------
    // BACKSUBSTITUTION 
    //---------------------------------------------------------------------
    j  = gp1-2;
    j1 = gp1-1;
    for (i = 1; i <= gp0-2; i++) {
      for (m = 0; m < 3; m++) {
        rhs[m][k][j][i] = rhs[m][k][j][i] - lhsY[3][k][j][i]*rhs[m][k][j1][i];
      }

      rhs[3][k][j][i] = rhs[3][k][j][i] - lhspY[3][k][j][i]*rhs[3][k][j1][i];
      rhs[4][k][j][i] = rhs[4][k][j][i] - lhsmY[3][k][j][i]*rhs[4][k][j1][i];
    }

    //---------------------------------------------------------------------
    // The first three factors
    //---------------------------------------------------------------------
    for (j = gp1-3; j >= 0; j--) {
      j1 = j + 1;
      j2 = j + 2;
      for (i = 1; i <= gp0-2; i++) {
        for (m = 0; m < 3; m++) {
          rhs[m][k][j][i] = rhs[m][k][j][i] - 
                            lhsY[3][k][j][i]*rhs[m][k][j1][i] -
                            lhsY[4][k][j][i]*rhs[m][k][j2][i];
        }

        //-------------------------------------------------------------------
        // And the remaining two
        //-------------------------------------------------------------------
        rhs[3][k][j][i] = rhs[3][k][j][i] - 
                          lhspY[3][k][j][i]*rhs[3][k][j1][i] -
                          lhspY[4][k][j][i]*rhs[3][k][j2][i];
        rhs[4][k][j][i] = rhs[4][k][j][i] - 
                          lhsmY[3][k][j][i]*rhs[4][k][j1][i] -
                          lhsmY[4][k][j][i]*rhs[4][k][j2][i];
      }
    }
  }
  }
  if (timeron) timer_stop(t_ysolve);

  pinvr();

  }
}

// this function performs the solution of the approximate factorization
// step in the z-direction for all five matrix components
// simultaneously. The Thomas algorithm is employed to solve the
// systems for the z-lines. Boundary conditions are non-periodic
void z_solve()
{
  int i, j, k, k1, k2, m, nj,ni;
  double ru1, fac1, fac2;

  int gp0, gp1, gp2;
  double lhsZ[5][JMAXP+1][IMAXP+1][KMAX];
  double lhspZ[5][JMAXP+1][IMAXP+1][KMAX];
  double lhsmZ[5][JMAXP+1][IMAXP+1][KMAX];
  double rhosZ[JMAXP+1][IMAXP+1][PROBLEM_SIZE];
  
  gp0 = grid_points[0];
  gp1 = grid_points[1];
  gp2 = grid_points[2];

  nj = nz2+1;
  ni = nx2;

  if (timeron) timer_start(t_zsolve);

#pragma acc data copy(rhs) copyin(speed,ws,rho_i) create(lhsZ,lhspZ,lhsmZ,rhosZ)
  {

#pragma acc kernels
  {
  for (j = 1; j <= ny2; j++) {
    //lhsinitj(nz2+1, nx2);
    for (i = 1; i <= ni; i++) {
      for (m = 0; m < 5; m++) {
	lhsZ[m][j][0][i] = 0.0;
	lhspZ[m][j][0][i] = 0.0;
	lhsmZ[m][j][0][i] = 0.0;
	lhsZ[m][j][nj][i] = 0.0;
	lhspZ[m][j][nj][i] = 0.0;
	lhsmZ[m][j][nj][i] = 0.0;
      }
      lhsZ[2][j][0][i] = 1.0;
      lhspZ[2][j][0][i] = 1.0;
      lhsmZ[2][j][0][i] = 1.0;
      lhsZ[2][j][nj][i] = 1.0;
      lhspZ[2][j][nj][i] = 1.0;
      lhsmZ[2][j][nj][i] = 1.0;
    }
  }
  
  for (j = 1; j <= ny2; j++) {
    //---------------------------------------------------------------------
    // Computes the left hand side for the three z-factors   
    //---------------------------------------------------------------------

    //---------------------------------------------------------------------
    // first fill the lhs for the u-eigenvalue                          
    //---------------------------------------------------------------------
    for (i = 1; i <= nx2; i++) {
      for (k = 0; k <= nz2+1; k++) {
        ru1 = c3c4*rho_i[k][j][i];
        //cv[k] = ws[k][j][i];
        rhosZ[j][i][k] = max(max(dz4+con43*ru1, dz5+c1c5*ru1), max(dzmax+ru1, dz1));
      }

      for (k = 1; k <= nz2; k++) {
        lhsZ[0][j][k][i] =  0.0;
        //lhsZ[1][j][k][i] = -dttz2 * cv[k-1] - dttz1 * rhosZ[j][i][k-1];
        //lhsZ[2][j][k][i] =  1.0 + c2dttz1 * rhosZ[j][i][k];
        //lhsZ[3][j][k][i] =  dttz2 * cv[k+1] - dttz1 * rhosZ[j][i][k+1];
        lhsZ[1][j][k][i] = -dttz2 * ws[k-1][j][i] - dttz1 * rhosZ[j][i][k-1];
        lhsZ[2][j][k][i] =  1.0 + c2dttz1 * rhosZ[j][i][k];
        lhsZ[3][j][k][i] =  dttz2 * ws[k+1][j][i] - dttz1 * rhosZ[j][i][k+1];
        lhsZ[4][j][k][i] =  0.0;
      }
    }

  }
  
  for (j = 1; j <= ny2; j++) {
    //---------------------------------------------------------------------
    // add fourth order dissipation                                  
    //---------------------------------------------------------------------
    for (i = 1; i <= nx2; i++) {
      k = 1;
      lhsZ[2][j][k][i] = lhsZ[2][j][k][i] + comz5;
      lhsZ[3][j][k][i] = lhsZ[3][j][k][i] - comz4;
      lhsZ[4][j][k][i] = lhsZ[4][j][k][i] + comz1;

      k = 2;
      lhsZ[1][j][k][i] = lhsZ[1][j][k][i] - comz4;
      lhsZ[2][j][k][i] = lhsZ[2][j][k][i] + comz6;
      lhsZ[3][j][k][i] = lhsZ[3][j][k][i] - comz4;
      lhsZ[4][j][k][i] = lhsZ[4][j][k][i] + comz1;
    }

    for (k = 3; k <= nz2-2; k++) {
      for (i = 1; i <= nx2; i++) {
        lhsZ[0][j][k][i] = lhsZ[0][j][k][i] + comz1;
        lhsZ[1][j][k][i] = lhsZ[1][j][k][i] - comz4;
        lhsZ[2][j][k][i] = lhsZ[2][j][k][i] + comz6;
        lhsZ[3][j][k][i] = lhsZ[3][j][k][i] - comz4;
        lhsZ[4][j][k][i] = lhsZ[4][j][k][i] + comz1;
      }
    }

    for (i = 1; i <= nx2; i++) {
      k = nz2-1;
      lhsZ[0][j][k][i] = lhsZ[0][j][k][i] + comz1;
      lhsZ[1][j][k][i] = lhsZ[1][j][k][i] - comz4;
      lhsZ[2][j][k][i] = lhsZ[2][j][k][i] + comz6;
      lhsZ[3][j][k][i] = lhsZ[3][j][k][i] - comz4;

      k = nz2;
      lhsZ[0][j][k][i] = lhsZ[0][j][k][i] + comz1;
      lhsZ[1][j][k][i] = lhsZ[1][j][k][i] - comz4;
      lhsZ[2][j][k][i] = lhsZ[2][j][k][i] + comz5;
    }

    //---------------------------------------------------------------------
    // subsequently, fill the other factors (u+c), (u-c) 
    //---------------------------------------------------------------------
    for (k = 1; k <= nz2; k++) {
      for (i = 1; i <= nx2; i++) {
        lhspZ[0][j][k][i] = lhsZ[0][j][k][i];
        lhspZ[1][j][k][i] = lhsZ[1][j][k][i] - dttz2 * speed[k-1][j][i];
        lhspZ[2][j][k][i] = lhsZ[2][j][k][i];
        lhspZ[3][j][k][i] = lhsZ[3][j][k][i] + dttz2 * speed[k+1][j][i];
        lhspZ[4][j][k][i] = lhsZ[4][j][k][i];
        lhsmZ[0][j][k][i] = lhsZ[0][j][k][i];
        lhsmZ[1][j][k][i] = lhsZ[1][j][k][i] + dttz2 * speed[k-1][j][i];
        lhsmZ[2][j][k][i] = lhsZ[2][j][k][i];
        lhsmZ[3][j][k][i] = lhsZ[3][j][k][i] - dttz2 * speed[k+1][j][i];
        lhsmZ[4][j][k][i] = lhsZ[4][j][k][i];
      }
    }


  }
  
  for (j = 1; j <= ny2; j++) {
    //---------------------------------------------------------------------
    // FORWARD ELIMINATION  
    //---------------------------------------------------------------------
    for (k = 0; k <= gp2-3; k++) {
      k1 = k + 1;
      k2 = k + 2;
      for (i = 1; i <= nx2; i++) {
        fac1 = 1.0/lhsZ[2][j][k][i];
        lhsZ[3][j][k][i] = fac1*lhsZ[3][j][k][i];
        lhsZ[4][j][k][i] = fac1*lhsZ[4][j][k][i];
        for (m = 0; m < 3; m++) {
          rhs[m][k][j][i] = fac1*rhs[m][k][j][i];
        }
        lhsZ[2][j][k1][i] = lhsZ[2][j][k1][i] - lhsZ[1][j][k1][i]*lhsZ[3][j][k][i];
        lhsZ[3][j][k1][i] = lhsZ[3][j][k1][i] - lhsZ[1][j][k1][i]*lhsZ[4][j][k][i];
        for (m = 0; m < 3; m++) {
          rhs[m][k1][j][i] = rhs[m][k1][j][i] - lhsZ[1][j][k1][i]*rhs[m][k][j][i];
        }
        lhsZ[1][j][k2][i] = lhsZ[1][j][k2][i] - lhsZ[0][j][k2][i]*lhsZ[3][j][k][i];
        lhsZ[2][j][k2][i] = lhsZ[2][j][k2][i] - lhsZ[0][j][k2][i]*lhsZ[4][j][k][i];
        for (m = 0; m < 3; m++) {
          rhs[m][k2][j][i] = rhs[m][k2][j][i] - lhsZ[0][j][k2][i]*rhs[m][k][j][i];
        }
      }
    }

    //---------------------------------------------------------------------
    // The last two rows in this grid block are a bit different, 
    // since they for (not have two more rows available for the
    // elimination of off-diagonal entries
    //---------------------------------------------------------------------
    k  = gp2-2;
    k1 = gp2-1;
    for (i = 1; i <= nx2; i++) {
      fac1 = 1.0/lhsZ[2][j][k][i];
      lhsZ[3][j][k][i] = fac1*lhsZ[3][j][k][i];
      lhsZ[4][j][k][i] = fac1*lhsZ[4][j][k][i];
      for (m = 0; m < 3; m++) {
        rhs[m][k][j][i] = fac1*rhs[m][k][j][i];
      }
      lhsZ[2][j][k1][i] = lhsZ[2][j][k1][i] - lhsZ[1][j][k1][i]*lhsZ[3][j][k][i];
      lhsZ[3][j][k1][i] = lhsZ[3][j][k1][i] - lhsZ[1][j][k1][i]*lhsZ[4][j][k][i];
      for (m = 0; m < 3; m++) {
        rhs[m][k1][j][i] = rhs[m][k1][j][i] - lhsZ[1][j][k1][i]*rhs[m][k][j][i];
      }

      //---------------------------------------------------------------------
      // scale the last row immediately
      //---------------------------------------------------------------------
      fac2 = 1.0/lhsZ[2][j][k1][i];
      for (m = 0; m < 3; m++) {
        rhs[m][k1][j][i] = fac2*rhs[m][k1][j][i];
      }
    }

    //---------------------------------------------------------------------
    // for (the u+c and the u-c factors               
    //---------------------------------------------------------------------
    for (k = 0; k <= gp2-3; k++) {
      k1 = k + 1;
      k2 = k + 2;
      for (i = 1; i <= nx2; i++) {
        m = 3;
        fac1 = 1.0/lhspZ[2][j][k][i];
        lhspZ[3][j][k][i]    = fac1*lhspZ[3][j][k][i];
        lhspZ[4][j][k][i]    = fac1*lhspZ[4][j][k][i];
        rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
        lhspZ[2][j][k1][i]   = lhspZ[2][j][k1][i] - lhspZ[1][j][k1][i]*lhspZ[3][j][k][i];
        lhspZ[3][j][k1][i]   = lhspZ[3][j][k1][i] - lhspZ[1][j][k1][i]*lhspZ[4][j][k][i];
        rhs[m][k1][j][i] = rhs[m][k1][j][i] - lhspZ[1][j][k1][i]*rhs[m][k][j][i];
        lhspZ[1][j][k2][i]   = lhspZ[1][j][k2][i] - lhspZ[0][j][k2][i]*lhspZ[3][j][k][i];
        lhspZ[2][j][k2][i]   = lhspZ[2][j][k2][i] - lhspZ[0][j][k2][i]*lhspZ[4][j][k][i];
        rhs[m][k2][j][i] = rhs[m][k2][j][i] - lhspZ[0][j][k2][i]*rhs[m][k][j][i];

        m = 4;
        fac1 = 1.0/lhsmZ[2][j][k][i];
        lhsmZ[3][j][k][i]    = fac1*lhsmZ[3][j][k][i];
        lhsmZ[4][j][k][i]    = fac1*lhsmZ[4][j][k][i];
        rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
        lhsmZ[2][j][k1][i]   = lhsmZ[2][j][k1][i] - lhsmZ[1][j][k1][i]*lhsmZ[3][j][k][i];
        lhsmZ[3][j][k1][i]   = lhsmZ[3][j][k1][i] - lhsmZ[1][j][k1][i]*lhsmZ[4][j][k][i];
        rhs[m][k1][j][i] = rhs[m][k1][j][i] - lhsmZ[1][j][k1][i]*rhs[m][k][j][i];
        lhsmZ[1][j][k2][i]   = lhsmZ[1][j][k2][i] - lhsmZ[0][j][k2][i]*lhsmZ[3][j][k][i];
        lhsmZ[2][j][k2][i]   = lhsmZ[2][j][k2][i] - lhsmZ[0][j][k2][i]*lhsmZ[4][j][k][i];
        rhs[m][k2][j][i] = rhs[m][k2][j][i] - lhsmZ[0][j][k2][i]*rhs[m][k][j][i];
      }
    }

    //---------------------------------------------------------------------
    // And again the last two rows separately
    //---------------------------------------------------------------------
    k  = gp2-2;
    k1 = gp2-1;
    for (i = 1; i <= nx2; i++) {
      m = 3;
      fac1 = 1.0/lhspZ[2][j][k][i];
      lhspZ[3][j][k][i]    = fac1*lhspZ[3][j][k][i];
      lhspZ[4][j][k][i]    = fac1*lhspZ[4][j][k][i];
      rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
      lhspZ[2][j][k1][i]   = lhspZ[2][j][k1][i] - lhspZ[1][j][k1][i]*lhspZ[3][j][k][i];
      lhspZ[3][j][k1][i]   = lhspZ[3][j][k1][i] - lhspZ[1][j][k1][i]*lhspZ[4][j][k][i];
      rhs[m][k1][j][i] = rhs[m][k1][j][i] - lhspZ[1][j][k1][i]*rhs[m][k][j][i];

      m = 4;
      fac1 = 1.0/lhsmZ[2][j][k][i];
      lhsmZ[3][j][k][i]    = fac1*lhsmZ[3][j][k][i];
      lhsmZ[4][j][k][i]    = fac1*lhsmZ[4][j][k][i];
      rhs[m][k][j][i]  = fac1*rhs[m][k][j][i];
      lhsmZ[2][j][k1][i]   = lhsmZ[2][j][k1][i] - lhsmZ[1][j][k1][i]*lhsmZ[3][j][k][i];
      lhsmZ[3][j][k1][i]   = lhsmZ[3][j][k1][i] - lhsmZ[1][j][k1][i]*lhsmZ[4][j][k][i];
      rhs[m][k1][j][i] = rhs[m][k1][j][i] - lhsmZ[1][j][k1][i]*rhs[m][k][j][i];

      //---------------------------------------------------------------------
      // Scale the last row immediately (some of this is overkill
      // if this is the last cell)
      //---------------------------------------------------------------------
      rhs[3][k1][j][i] = rhs[3][k1][j][i]/lhspZ[2][j][k1][i];
      rhs[4][k1][j][i] = rhs[4][k1][j][i]/lhsmZ[2][j][k1][i];
    }


    //---------------------------------------------------------------------
    // BACKSUBSTITUTION 
    //---------------------------------------------------------------------
    k  = gp2-2;
    k1 = gp2-1;
    for (i = 1; i <= nx2; i++) {
      for (m = 0; m < 3; m++) {
        rhs[m][k][j][i] = rhs[m][k][j][i] - lhsZ[3][j][k][i]*rhs[m][k1][j][i];
      }

      rhs[3][k][j][i] = rhs[3][k][j][i] - lhspZ[3][j][k][i]*rhs[3][k1][j][i];
      rhs[4][k][j][i] = rhs[4][k][j][i] - lhsmZ[3][j][k][i]*rhs[4][k1][j][i];
    }

    //---------------------------------------------------------------------
    // Whether or not this is the last processor, we always have
    // to complete the back-substitution 
    //---------------------------------------------------------------------

    //---------------------------------------------------------------------
    // The first three factors
    //---------------------------------------------------------------------
    for (k = gp2-3; k >= 0; k--) {
      k1 = k + 1;
      k2 = k + 2;
      for (i = 1; i <= nx2; i++) {
        for (m = 0; m < 3; m++) {
          rhs[m][k][j][i] = rhs[m][k][j][i] - 
                            lhsZ[3][j][k][i]*rhs[m][k1][j][i] -
                            lhsZ[4][j][k][i]*rhs[m][k2][j][i];
        }

        //-------------------------------------------------------------------
        // And the remaining two
        //-------------------------------------------------------------------
        rhs[3][k][j][i] = rhs[3][k][j][i] - 
                          lhspZ[3][j][k][i]*rhs[3][k1][j][i] -
                          lhspZ[4][j][k][i]*rhs[3][k2][j][i];
        rhs[4][k][j][i] = rhs[4][k][j][i] - 
                          lhsmZ[3][j][k][i]*rhs[4][k1][j][i] -
                          lhsmZ[4][j][k][i]*rhs[4][k2][j][i];
      }
    }
  }
  }
  if (timeron) timer_stop(t_zsolve);

  tzetar();
  }
}

void compute_rhs()
{
  int i, j, k, m;
  double aux, rho_inv, uijk, up1, um1, vijk, vp1, vm1, wijk, wp1, wm1;


  if (timeron) timer_start(t_rhs);

#pragma acc data pcopy(rhs, u,rho_i,us,vs,ws,square,qs,forcing,speed)
  {
    //---------------------------------------------------------------------
    // compute the reciprocal of density, and the kinetic energy, 
    // and the speed of sound. 
    //---------------------------------------------------------------------
#pragma acc kernels loop gang
    for (k = 0; k <= grid_points[2]-1; k++) {
#pragma acc loop gang vector
      for (j = 0; j <= grid_points[1]-1; j++) {
#pragma acc loop vector
	for (i = 0; i <= grid_points[0]-1; i++) {
	  rho_inv = 1.0/u[0][k][j][i];
	  rho_i[k][j][i] = rho_inv;
	  us[k][j][i] = u[1][k][j][i] * rho_inv;
	  vs[k][j][i] = u[2][k][j][i] * rho_inv;
	  ws[k][j][i] = u[3][k][j][i] * rho_inv;
	  square[k][j][i] = 0.5* (
				  u[1][k][j][i]*u[1][k][j][i] + 
				  u[2][k][j][i]*u[2][k][j][i] +
				  u[3][k][j][i]*u[3][k][j][i] ) * rho_inv;
	  qs[k][j][i] = square[k][j][i] * rho_inv;
	  //-------------------------------------------------------------------
	  // (don't need speed and ainx until the lhs computation)
	  //-------------------------------------------------------------------
	  aux = c1c2*rho_inv* (u[4][k][j][i] - square[k][j][i]);
	  speed[k][j][i] = sqrt(aux);
	}
      }
    }

    //---------------------------------------------------------------------
    // copy the exact forcing term to the right hand side;  because 
    // this forcing term is known, we can store it on the whole grid
    // including the boundary                   
    //---------------------------------------------------------------------
#pragma acc kernels loop gang
    for (k = 0; k <= grid_points[2]-1; k++) {
#pragma acc loop gang vector
      for (j = 0; j <= grid_points[1]-1; j++) {
#pragma acc loop vector
	for (i = 0; i <= grid_points[0]-1; i++) {
	  for (m = 0; m < 5; m++) {
	    rhs[m][k][j][i] = forcing[j][m][k][i];
	  }
	}
      }
    }

    //---------------------------------------------------------------------
    // compute xi-direction fluxes 
    //---------------------------------------------------------------------
    if (timeron) timer_start(t_rhsx);
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
	for (i = 1; i <= nx2; i++) {
	  uijk = us[k][j][i];
	  up1  = us[k][j][i+1];
	  um1  = us[k][j][i-1];

	  rhs[0][k][j][i] = rhs[0][k][j][i] + dx1tx1 * 
	    (u[0][k][j][i+1] - 2.0*u[0][k][j][i] + u[0][k][j][i-1]) -
	    tx2 * (u[1][k][j][i+1] - u[1][k][j][i-1]);

	  rhs[1][k][j][i] = rhs[1][k][j][i] + dx2tx1 * 
	    (u[1][k][j][i+1] - 2.0*u[1][k][j][i] + u[1][k][j][i-1]) +
	    xxcon2*con43 * (up1 - 2.0*uijk + um1) -
	    tx2 * (u[1][k][j][i+1]*up1 - u[1][k][j][i-1]*um1 +
		   (u[4][k][j][i+1] - square[k][j][i+1] -
		    u[4][k][j][i-1] + square[k][j][i-1]) * c2);

	  rhs[2][k][j][i] = rhs[2][k][j][i] + dx3tx1 * 
	    (u[2][k][j][i+1] - 2.0*u[2][k][j][i] + u[2][k][j][i-1]) +
	    xxcon2 * (vs[k][j][i+1] - 2.0*vs[k][j][i] + vs[k][j][i-1]) -
	    tx2 * (u[2][k][j][i+1]*up1 - u[2][k][j][i-1]*um1);

	  rhs[3][k][j][i] = rhs[3][k][j][i] + dx4tx1 * 
	    (u[3][k][j][i+1] - 2.0*u[3][k][j][i] + u[3][k][j][i-1]) +
	    xxcon2 * (ws[k][j][i+1] - 2.0*ws[k][j][i] + ws[k][j][i-1]) -
	    tx2 * (u[3][k][j][i+1]*up1 - u[3][k][j][i-1]*um1);

	  rhs[4][k][j][i] = rhs[4][k][j][i] + dx5tx1 * 
	    (u[4][k][j][i+1] - 2.0*u[4][k][j][i] + u[4][k][j][i-1]) +
	    xxcon3 * (qs[k][j][i+1] - 2.0*qs[k][j][i] + qs[k][j][i-1]) +
	    xxcon4 * (up1*up1 -       2.0*uijk*uijk + um1*um1) +
	    xxcon5 * (u[4][k][j][i+1]*rho_i[k][j][i+1] - 
		      2.0*u[4][k][j][i]*rho_i[k][j][i] +
		      u[4][k][j][i-1]*rho_i[k][j][i-1]) -
	    tx2 * ( (c1*u[4][k][j][i+1] - c2*square[k][j][i+1])*up1 -
		    (c1*u[4][k][j][i-1] - c2*square[k][j][i-1])*um1 );
	}
      }
    }
    //---------------------------------------------------------------------
    // add fourth order xi-direction dissipation               
    //---------------------------------------------------------------------
    i = 1;
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i]- dssp * 
	    (5.0*u[m][k][j][i] - 4.0*u[m][k][j][i+1] + u[m][k][j][i+2]);
	}
      }
    }

    i = 2;
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp * 
	    (-4.0*u[m][k][j][i-1] + 6.0*u[m][k][j][i] -
	     4.0*u[m][k][j][i+1] + u[m][k][j][i+2]);
	}
      }
    }

#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang 
      for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
	for (i = 3; i <= nx2-2; i++) {
	  for (m = 0; m < 5; m++) {
	    rhs[m][k][j][i] = rhs[m][k][j][i] - dssp * 
	      ( u[m][k][j][i-2] - 4.0*u[m][k][j][i-1] + 
		6.0*u[m][k][j][i] - 4.0*u[m][k][j][i+1] + 
		u[m][k][j][i+2] );
	  }
	}
      }
    }

    i = nx2-1;
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp *
	    ( u[m][k][j][i-2] - 4.0*u[m][k][j][i-1] + 
	      6.0*u[m][k][j][i] - 4.0*u[m][k][j][i+1] );
	}
      }
    }
    i = nx2;
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp *
	    ( u[m][k][j][i-2] - 4.0*u[m][k][j][i-1] + 5.0*u[m][k][j][i] );
	}
      }
    }
    if (timeron) timer_stop(t_rhsx);

    //---------------------------------------------------------------------
    // compute eta-direction fluxes 
    //---------------------------------------------------------------------
    if (timeron) timer_start(t_rhsy);
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
	for (i = 1; i <= nx2; i++) {
	  vijk = vs[k][j][i];
	  vp1  = vs[k][j+1][i];
	  vm1  = vs[k][j-1][i];

	  rhs[0][k][j][i] = rhs[0][k][j][i] + dy1ty1 * 
	    (u[0][k][j+1][i] - 2.0*u[0][k][j][i] + u[0][k][j-1][i]) -
	    ty2 * (u[2][k][j+1][i] - u[2][k][j-1][i]);

	  rhs[1][k][j][i] = rhs[1][k][j][i] + dy2ty1 * 
	    (u[1][k][j+1][i] - 2.0*u[1][k][j][i] + u[1][k][j-1][i]) +
	    yycon2 * (us[k][j+1][i] - 2.0*us[k][j][i] + us[k][j-1][i]) -
	    ty2 * (u[1][k][j+1][i]*vp1 - u[1][k][j-1][i]*vm1);

	  rhs[2][k][j][i] = rhs[2][k][j][i] + dy3ty1 * 
	    (u[2][k][j+1][i] - 2.0*u[2][k][j][i] + u[2][k][j-1][i]) +
	    yycon2*con43 * (vp1 - 2.0*vijk + vm1) -
	    ty2 * (u[2][k][j+1][i]*vp1 - u[2][k][j-1][i]*vm1 +
		   (u[4][k][j+1][i] - square[k][j+1][i] - 
		    u[4][k][j-1][i] + square[k][j-1][i]) * c2);

	  rhs[3][k][j][i] = rhs[3][k][j][i] + dy4ty1 * 
	    (u[3][k][j+1][i] - 2.0*u[3][k][j][i] + u[3][k][j-1][i]) +
	    yycon2 * (ws[k][j+1][i] - 2.0*ws[k][j][i] + ws[k][j-1][i]) -
	    ty2 * (u[3][k][j+1][i]*vp1 - u[3][k][j-1][i]*vm1);

	  rhs[4][k][j][i] = rhs[4][k][j][i] + dy5ty1 * 
	    (u[4][k][j+1][i] - 2.0*u[4][k][j][i] + u[4][k][j-1][i]) +
	    yycon3 * (qs[k][j+1][i] - 2.0*qs[k][j][i] + qs[k][j-1][i]) +
	    yycon4 * (vp1*vp1       - 2.0*vijk*vijk + vm1*vm1) +
	    yycon5 * (u[4][k][j+1][i]*rho_i[k][j+1][i] - 
		      2.0*u[4][k][j][i]*rho_i[k][j][i] +
		      u[4][k][j-1][i]*rho_i[k][j-1][i]) -
	    ty2 * ((c1*u[4][k][j+1][i] - c2*square[k][j+1][i]) * vp1 -
		   (c1*u[4][k][j-1][i] - c2*square[k][j-1][i]) * vm1);
	}
      }

      //---------------------------------------------------------------------
      // add fourth order eta-direction dissipation         
      //---------------------------------------------------------------------
    }
    j = 1;
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (i = 1; i <= nx2; i++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i]- dssp * 
	    ( 5.0*u[m][k][j][i] - 4.0*u[m][k][j+1][i] + u[m][k][j+2][i]);
	}
      }
    }

      j = 2;

#pragma acc kernels loop gang
      for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
	for (i = 1; i <= nx2; i++) {
	  for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp * 
	    (-4.0*u[m][k][j-1][i] + 6.0*u[m][k][j][i] -
	     4.0*u[m][k][j+1][i] + u[m][k][j+2][i]);
	}
      }
      }
#pragma acc kernels loop gang
      for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 3; j <= ny2-2; j++) {
#pragma acc loop vector
	for (i = 1; i <= nx2; i++) {
	  for (m = 0; m < 5; m++) {
	    rhs[m][k][j][i] = rhs[m][k][j][i] - dssp * 
	      ( u[m][k][j-2][i] - 4.0*u[m][k][j-1][i] + 
		6.0*u[m][k][j][i] - 4.0*u[m][k][j+1][i] + 
		u[m][k][j+2][i] );
	  }
	}
      }
      }
      j = ny2-1;
#pragma acc kernels loop gang 
      for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (i = 1; i <= nx2; i++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp *
	    ( u[m][k][j-2][i] - 4.0*u[m][k][j-1][i] + 
	      6.0*u[m][k][j][i] - 4.0*u[m][k][j+1][i] );
	}
      }
      }
      j = ny2;
#pragma acc kernels loop gang
      for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (i = 1; i <= nx2; i++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp *
	    ( u[m][k][j-2][i] - 4.0*u[m][k][j-1][i] + 5.0*u[m][k][j][i] );
	}
      }
    }
    if (timeron) timer_stop(t_rhsy);

    //---------------------------------------------------------------------
    // compute zeta-direction fluxes 
    //---------------------------------------------------------------------
    if (timeron) timer_start(t_rhsz);
#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
	for (i = 1; i <= nx2; i++) {
	  wijk = ws[k][j][i];
	  wp1  = ws[k+1][j][i];
	  wm1  = ws[k-1][j][i];

	  rhs[0][k][j][i] = rhs[0][k][j][i] + dz1tz1 * 
	    (u[0][k+1][j][i] - 2.0*u[0][k][j][i] + u[0][k-1][j][i]) -
	    tz2 * (u[3][k+1][j][i] - u[3][k-1][j][i]);

	  rhs[1][k][j][i] = rhs[1][k][j][i] + dz2tz1 * 
	    (u[1][k+1][j][i] - 2.0*u[1][k][j][i] + u[1][k-1][j][i]) +
	    zzcon2 * (us[k+1][j][i] - 2.0*us[k][j][i] + us[k-1][j][i]) -
	    tz2 * (u[1][k+1][j][i]*wp1 - u[1][k-1][j][i]*wm1);

	  rhs[2][k][j][i] = rhs[2][k][j][i] + dz3tz1 * 
	    (u[2][k+1][j][i] - 2.0*u[2][k][j][i] + u[2][k-1][j][i]) +
	    zzcon2 * (vs[k+1][j][i] - 2.0*vs[k][j][i] + vs[k-1][j][i]) -
	    tz2 * (u[2][k+1][j][i]*wp1 - u[2][k-1][j][i]*wm1);

	  rhs[3][k][j][i] = rhs[3][k][j][i] + dz4tz1 * 
	    (u[3][k+1][j][i] - 2.0*u[3][k][j][i] + u[3][k-1][j][i]) +
	    zzcon2*con43 * (wp1 - 2.0*wijk + wm1) -
	    tz2 * (u[3][k+1][j][i]*wp1 - u[3][k-1][j][i]*wm1 +
		   (u[4][k+1][j][i] - square[k+1][j][i] - 
		    u[4][k-1][j][i] + square[k-1][j][i]) * c2);

	  rhs[4][k][j][i] = rhs[4][k][j][i] + dz5tz1 * 
	    (u[4][k+1][j][i] - 2.0*u[4][k][j][i] + u[4][k-1][j][i]) +
	    zzcon3 * (qs[k+1][j][i] - 2.0*qs[k][j][i] + qs[k-1][j][i]) +
	    zzcon4 * (wp1*wp1 - 2.0*wijk*wijk + wm1*wm1) +
	    zzcon5 * (u[4][k+1][j][i]*rho_i[k+1][j][i] - 
		      2.0*u[4][k][j][i]*rho_i[k][j][i] +
		      u[4][k-1][j][i]*rho_i[k-1][j][i]) -
	    tz2 * ((c1*u[4][k+1][j][i] - c2*square[k+1][j][i])*wp1 -
		   (c1*u[4][k-1][j][i] - c2*square[k-1][j][i])*wm1);
	}
      }
    }

    //---------------------------------------------------------------------
    // add fourth order zeta-direction dissipation                
    //---------------------------------------------------------------------
    k = 1;
#pragma acc kernels loop
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i]- dssp * 
	    (5.0*u[m][k][j][i] - 4.0*u[m][k+1][j][i] + u[m][k+2][j][i]);
	}
      }
    }

    k = 2;
#pragma acc kernels loop gang 
    for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
      for (i = 1; i <= nx2; i++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp * 
	    (-4.0*u[m][k-1][j][i] + 6.0*u[m][k][j][i] -
	     4.0*u[m][k+1][j][i] + u[m][k+2][j][i]);
	}
      }
    }

#pragma acc kernels loop gang
    for (k = 3; k <= nz2-2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
	for (i = 1; i <= nx2; i++) {
	  for (m = 0; m < 5; m++) {
	    rhs[m][k][j][i] = rhs[m][k][j][i] - dssp * 
	      ( u[m][k-2][j][i] - 4.0*u[m][k-1][j][i] + 
		6.0*u[m][k][j][i] - 4.0*u[m][k+1][j][i] + 
		u[m][k+2][j][i] );
	  }
	}
      }
    }

    k = nz2-1;
#pragma acc kernels loop gang vector
    for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
      for (i = 1; i <= nx2; i++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp *
	    ( u[m][k-2][j][i] - 4.0*u[m][k-1][j][i] + 
	      6.0*u[m][k][j][i] - 4.0*u[m][k+1][j][i] );
	}
      }
    }

    k = nz2;
#pragma acc kernels loop
    for (j = 1; j <= ny2; j++) {
      for (i = 1; i <= nx2; i++) {
	for (m = 0; m < 5; m++) {
	  rhs[m][k][j][i] = rhs[m][k][j][i] - dssp *
	    ( u[m][k-2][j][i] - 4.0*u[m][k-1][j][i] + 5.0*u[m][k][j][i] );
	}
      }
    }
    if (timeron) timer_stop(t_rhsz);

#pragma acc kernels loop gang
    for (k = 1; k <= nz2; k++) {
#pragma acc loop gang vector
      for (j = 1; j <= ny2; j++) {
#pragma acc loop vector
	for (i = 1; i <= nx2; i++) {
	  for (m = 0; m < 5; m++) {
	    rhs[m][k][j][i] = rhs[m][k][j][i] * dt;
	  }
	}
      }
    }
  }
  if (timeron) timer_stop(t_rhs);
}
