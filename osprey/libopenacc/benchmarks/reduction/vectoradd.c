#define N 10000

int main( int argc, char* argv[] )
{
 
    // Size of vectors
    int n = 10000;
 
    // Input vectors
    double* a;
    double* b;
    // Output vector
    double* c;
 
    // Size, in bytes, of each vector
    //int bytes = n*4;
    int i; 
    double diff;
    // Allocate memory for each vector
    /*a = (double*)malloc(bytes);
    b = (double*)malloc(bytes);
    c = (double*)malloc(bytes);
 
    // Initialize content of input vectors, vector a[i] = sin(i)^2 vector b[i]
    // = cos(i)^2
    for(i=0; i<n; i++) {
        a[i] = sin(i)*sin(i);
        b[i] = cos(i)*cos(i);
    } */  
 
    // sum component wise and save result into vector c
#pragma acc data copyin(a[0:n], b[0: n]), copyout(c[0: n])
  {
    #pragma acc kernels param(a, b, c, n)
    {
      #pragma acc loop  reduction(+: diff)
      for(i=0; i<n; i++) 
      {
        c[i] = a[i] + b[i];
        diff += c[i];
      } 
    }
  }
    //#pragma acc wait (20) 

    // Sum up vector c and print result divided by n, this should equal 1
    // within error
    double sum = 0.0;
    for(i=0; i<n; i++) {
        sum += c[i];
    }
    sum = sum/n;
    printf("final result: %f\n", sum);
 
    // Release memory
    /*free(a);
    free(b);
    free(c);*/
 
    return 0;
}
