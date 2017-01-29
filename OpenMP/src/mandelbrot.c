/* -------------------- */
/* --- mandelbrot.c --- */
/* -------------------- */

#include <math.h>

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <ctype.h> /* isdigit */
#include <string.h> /* memcpy */

#ifdef OPENMP
#include <omp.h>
#endif

#define  NB_THREADS 4

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"
#include "simd_macro.h"
#include "mymacro.h"
#include "cpu_count.h"

//#include "ia32intrin.h" // si compilateur Intel

// --------------------------------------------------
int mandelbrot_scalar(float a, float b, int max_iter)
// --------------------------------------------------
{

    // conseil: afficher le contenu des variables dans la boucles *et* valider via excel
    int iter = 0;

    float xn = a;
    float yn = b;

    while(iter<max_iter && xn*xn + yn*yn < 4.0f)
    {
        float x  = xn * xn - yn * yn;
        float y  = 2 * yn * xn ;

        xn = x  + a;
        yn = y  + b;
        iter++;
        //DEBUG(printf("(%4.2f) -> %3d\n", xn*xn + yn*yn, iter));
    }
    // COMPLETER ICI
    return iter;
}
// ------------------------------
void test_mandelbrot_scalar(void)
// ------------------------------
{
    int i;
    int iter, max_iter = 20;
    float x0, y0;
    float x1, y1;

    puts("------------------------------");
    puts("--- test_mandelbrot_scalar ---");
    puts("------------------------------");
    // tests unitaire pour valider

    x0 = -0.8; y0 = 0.3; iter = mandelbrot_scalar(x0, y0, max_iter); DEBUG(printf("(%4.2f %4.2f) -> %3d\n", x0, y0, iter));
    x0 = -0.7; y0 = 0.2; iter = mandelbrot_scalar(x0, y0, max_iter); DEBUG(printf("(%4.2f %4.2f) -> %3d\n", x0, y0, iter));
}
// --------------------------------------------------------------
vuint32 mandelbrot_SIMD_F32(vfloat32 a, vfloat32 b, int max_iter)
// --------------------------------------------------------------
{
    // version avec test de sortie en float
    vfloat32 x =a,y=b;
    vuint32 iter = _mm_set1_epi32(0);



    while(1){
        vfloat32 len = _mm_add_ps(_mm_mul_ps(x,x),_mm_mul_ps(y,y));
        /* DEBUG(display_vfloat32(len, "%8.4f", "len =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(x, "%8.4f", "x =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(y, "%8.4f", "y =")); DEBUG(puts("")); */


        vfloat32 mask = _mm_and_ps((vfloat32)(_mm_cmplt_epi32(iter,_mm_set1_epi32(max_iter))),(_mm_cmplt_ps(len,_mm_set1_ps(4.f))));
        if(_mm_movemask_ps(mask)==0)break;

        vfloat32 x2 = _mm_sub_ps(_mm_mul_ps(x,x),_mm_mul_ps(y,y));// x2 = x*x -y*y;
        vfloat32 xy = _mm_mul_ps(x,y);
        vfloat32 y2 = _mm_add_ps(xy, xy);// y2 =2*x*y;

        //Mask.i = _mm_srai_epi32( Mask.i, 31 );
        vfloat32 xn = _mm_add_ps(a,x2);
        xn = _mm_and_ps(xn, mask );
        x  = _mm_andnot_ps( mask, x );
        x  = _mm_or_ps( x,  xn);

        vfloat32 yn = _mm_add_ps(b,y2);
        yn = _mm_and_ps(yn, mask);
        y  = _mm_andnot_ps( mask, y );
        y  = _mm_or_ps( y,  yn);



        vuint32 in = _mm_add_epi32(iter,_mm_set1_epi32(1));
        vfloat32 inn = _mm_and_ps((vfloat32)in, mask);
        iter  = (vuint32)_mm_andnot_ps( mask, (vfloat32)iter );
        iter  = (vuint32)_mm_or_ps(inn,  (vfloat32)iter);
    }
    return iter;
    // COMPLETER ICI
}
// --------------------------------------------------------------
vuint32 mandelbrot_SIMD_I32(vfloat32 a, vfloat32 b, int max_iter)
// --------------------------------------------------------------
{
    // version avec test de sortie en int

    vuint32 iter = _mm_set1_epi32(0);

    return iter;
}
// ----------------------------
void test_mandelbrot_SIMD(void)
// ----------------------------
{
    int iter = 20;
    vuint32 i;
    vfloat32 a, b;
    double t0, t1, dt;

    puts("----------------------------");
    puts("--- test_mandelbrot_SIMD ---");
    puts("----------------------------");

    a = _mm_setr_ps(-0.8, -0.7, -0.8, -0.7);
    b = _mm_setr_ps(+0.3, +0.2, -0.3, -0.2);

    //iter = 20;
    puts("mandelbrot_SIMD_F32");
    a = _mm_setr_ps(-1.00, -0.90, -0.80, -0.70);
    b = _mm_setr_ps(+0.40, +0.30, +0.30, +0.10);

    DEBUG(display_vfloat32(a, "%8.4f", "a =")); DEBUG(puts(""));
    DEBUG(display_vfloat32(b, "%8.4f", "b =")); DEBUG(puts(""));

    i = mandelbrot_SIMD_F32(a, b, iter);
    display_vuint32(i, "%10d ", "iter"); puts("");

    puts("mandelbrot_SIMD_I32");
    a = _mm_setr_ps(-1.00, -0.90, -0.80, -0.70);
    b = _mm_setr_ps(+0.40, +0.30, +0.30, +0.10);
    i = mandelbrot_SIMD_I32(a, b, iter);
    display_vuint32(i, "%10d ", "iter"); puts("");
}

// --------------------------------------------------------------------------------------------------------
void calc_mandelbrot_scalar(uint32 **M, int h, int w, float a0, float a1, float b0, float b1, int max_iter)
// --------------------------------------------------------------------------------------------------------
{
    // intervale de valeurs: [a0:a1]x[b0:b1]

    // la seule chose a modifier dans cette fonction est la ligne de pragma OpenMP

    int i, j;

    float da, db;
    float32 a, b;
    uint32 iter;

    da = (a1-a0)/w;
    db = (b1-b0)/h;

#ifdef OPENMP
    omp_set_num_threads(NB_THREADS);
#pragma omp parrallel for private(a, b, iter)
// COMPLETER ICI
#endif

    for(i=0; i<h; i++) {
        for(j=0; j<w; j++) {

            // conversion (i,j) -> (x,y)
            a = a0 + i * da;
            b = b0 + j * db;

            iter = mandelbrot_scalar(a, b, max_iter);

            M[i][j] = iter;
	}
    }
}
// -----------------------------------------------------------------------------------------------------------
void calc_mandelbrot_SIMD_F32(vuint32 **M, int h, int w, float a0, float a1, float b0, float b1, int max_iter)
// -----------------------------------------------------------------------------------------------------------
{
    int i, j;

    float da, db;
    float sa, sb;
    vfloat32 a, b;
    vuint32 iter;

    da = (a1-a0)/w;
    db = (b1-b0)/h;


#ifdef OPENMP
    // COMPLETER ICI
#endif

    for(i=0; i<h; i++) {
        for(j=0; j<w/4; j++) {

            // conversion (i,j) -> (x,y)
            sa = a0 + i * da;
            sb = b0 + j * db * 4;

            a = _mm_setr_ps(sa, sa+da, sa+2*da, sa+3*da);
            b = _mm_set1_ps(sb);

            iter = mandelbrot_SIMD_F32(a, b, max_iter);
            M[i][j] = iter;
        }
    }
}
// -----------------------------------------------------------------------------------------------------------
void calc_mandelbrot_SIMD_I32(vuint32 **M, int h, int w, float a0, float a1, float b0, float b1, int max_iter)
// -----------------------------------------------------------------------------------------------------------
{
    int i, j;

    float da, db;
    float sa, sb;
    vfloat32 a, b;
    vuint32 iter;

    da = (a1-a0)/w;
    db = (b1-b0)/h;

#ifdef OPENMP
    // COMPLETER ICI
#endif

    for(i=0; i<h; i++) {
        for(j=0; j<w/4; j++) {

            // conversion (i,j) -> (x,y)
            sa = a0 + i * da;
            sb = b0 + j * db * 4;

            a = _mm_setr_ps(sa, sa+da, sa+2*da, sa+3*da);
            b = _mm_set1_ps(sb);

            iter = mandelbrot_SIMD_I32(a, b, max_iter);
            M[i][j] = iter;
        }
    }
}
// -----------------------------------------------------------------------------------
convert_ui32matrix_ui8matrix(uint32 **m32, int i0, int i1, int j0, int j1, uint8 **m8)
// -----------------------------------------------------------------------------------
{
    int i, j;
    for(i=i0; i<=i1; i++) {
        for(j=j0; j<=j1; j++) {
            m8[i][j] = (uint8) m32[i][j];
        }
    }
}
// ----------------------------------------------
void bench_mandelbrot_scalar(int n, int max_iter)
// ----------------------------------------------
{
    // ne rien modifier dans cette fonction

    int h, w;
    int i0, i1, j0, j1;
    float a0, a1, b0, b1;
    uint32 **M32;
    uint8 **M8;


    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    puts("-----------------------------");
    puts("-- bench_mandelbrot_scalar --");
    puts("-----------------------------");

    h = w = n;
    M32 = ui32matrix(0, h-1, 0, w-1);
    M8  = ui8matrix(0, h-1, 0, w-1);

    a0 = -1.5; a1 = +0.5;
    b0 = -1.0; b1 = +1.0;

    CHRONO(calc_mandelbrot_scalar(M32, h, w, a0, a1, b0, b1, max_iter), cycles);  printf("scalar:   %10.2f\n", cycles/(n*n));

    DEBUG(convert_ui32matrix_ui8matrix(M32, 0, h-1, 0, w-1, M8));
    DEBUG(SavePGM_ui8matrix(M8, 0, h-1, 0, w-1, "M_scalar.pgm"));
    SavePGM_ui8matrix(M8, 0, h-1, 0, w-1, "M_scalar.pgm");

    free_ui32matrix(M32, 0, h-1, 0, w-1);
    free_ui8matrix(M8, 0, h-1, 0, w-1);
}
// --------------------------------------------
void bench_mandelbrot_SIMD(int n, int max_iter)
// --------------------------------------------
{
    // ne rien modifier dans cette fonction

    int h, w;
    float a0, a1, b0, b1;
    vuint32 **M32;
    uint32 **wM32;
    uint8 **M8;

    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    puts("---------------------------");
    puts("-- bench_mandelbrot_SIMD --");
    puts("---------------------------");
    h = w = n;

    M32 = vui32matrix(0, h-1, 0, w/4-1);
    M8  = ui8matrix(0, h-1, 0, w-1);
    wM32 = (uint32**) M32;

    // ne pas changer
    a0 = -1.5; a1 = +0.5;
    b0 = -1.0; b1 = +1.0;

    CHRONO(calc_mandelbrot_SIMD_F32(M32, h, w, a0, a1, b0, b1, max_iter), cycles);
    printf("SIMD F32: %10.2f\n", cycles/(n*n));

    /* CHRONO(calc_mandelbrot_SIMD_I32(M32, h, w, a0, a1, b0, b1, max_iter), cycles); // facultatif */
    /* printf("SIMD I32: %10.2f\n\n", cycles/(n*n)); */

    DEBUG(convert_ui32matrix_ui8matrix(wM32, 0, h-1, 0, w-1, M8));
    DEBUG(SavePGM_ui8matrix(M8, 0, h-1, 0, w-1, "M_v.pgm"));
    SavePGM_ui8matrix(M8, 0, h-1, 0, w-1, "M_v.pgm");

    free_vui32matrix(M32, 0, h-1, 0, w/4-1);
    free_ui8matrix(M8, 0, h-1, 0, w-1);
}

// =========================================
int main_mandelbrot(int argc, char * argv[])
// =========================================
{
    int n, max_iter; // pour avoir les meme param en scalar et SIMD ...

    test_mandelbrot_scalar();
    test_mandelbrot_SIMD();

    n = 1000; max_iter = 256;
    printf("n = %4d max_iter = %d\n", n, max_iter);
    bench_mandelbrot_scalar(n, max_iter);
    bench_mandelbrot_SIMD(n, max_iter);

    return 0;
}
