/* ---------------- */
/* --- simd2D.c --- */
/* ---------------- */

#include <math.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"


// --------------------------------------------------------
void avg3_reg_vf32matrix(vfloat32** vX, int n, vfloat32 **Y)
// --------------------------------------------------------
{
    int i, j;
    vfloat32 a0, b0, c0;
    vfloat32 a1, b1, c1;
    vfloat32 a2, b2, c2;

    vfloat32 aa0, cc0;
    vfloat32 aa1, cc1;
    vfloat32 aa2, cc2;

    for(i=0; i<n; i++)
        for(j = 0; j<n; j++) {
            a0 = _mm_load_ps((float32*) &vX[i][j]);
            b0 = _mm_load_ps((float32*) &vX[i][j+1]);
            DEBUG(display_vfloat32(a0, "%4.0f", "a0  =")); DEBUG(puts(""));
            DEBUG(display_vfloat32(b0, "%4.0f", "b0  =")); DEBUG(puts(""));
            DEBUG(display_vfloat32(vec_left3(a0,b0), "%4.0f", "ab  =")); DEBUG(puts(""));
        }

    for(i=-2; i<10; i++)
        for(j=-1; j<n+1; j++) {
            a0 = _mm_load_ps((float32*) &vX[i][j]);
            b0 = _mm_load_ps((float32*) &vX[i+1][j]);
            c0 = _mm_load_ps((float32*) &vX[i+2][j]);

            a1 = _mm_load_ps((float32*) &vX[i][j+1]);
            b1 = _mm_load_ps((float32*) &vX[i+1][j+1]);
            c1 = _mm_load_ps((float32*) &vX[i+2][j+1]);

            a2 = _mm_load_ps((float32*) &vX[i][j+2]);
            b2 = _mm_load_ps((float32*) &vX[i+1][j+2]);
            c2 = _mm_load_ps((float32*) &vX[i+2][j+2]);

            /* DEBUG(display_vfloat32(a0, "%4.0f", "a0  =")); DEBUG(puts("")); */
            /* DEBUG(display_vfloat32(b0, "%4.0f", "b0  =")); DEBUG(puts("")); */
            /* DEBUG(display_vfloat32(c0, "%4.0f", "c0  =")); DEBUG(puts("")); */

            /* DEBUG(display_vfloat32(a1, "%4.0f", "a1  =")); DEBUG(puts("")); */
            /* DEBUG(display_vfloat32(b1, "%4.0f", "b1  =")); DEBUG(puts("")); */
            /* DEBUG(display_vfloat32(c1, "%4.0f", "c1  =")); DEBUG(puts("")); */

            /* DEBUG(display_vfloat32(a2, "%4.0f", "a2  =")); DEBUG(puts("")); */
            /* DEBUG(display_vfloat32(b2, "%4.0f", "b2  =")); DEBUG(puts("")); */
            /* DEBUG(display_vfloat32(c2, "%4.0f", "c2  =")); DEBUG(puts("")); */
        }
    /*     y  = _mm_shuffle_ps(x0, x1, _MM_SHUFFLE(1, 0, 3, 2)); */
    /*     xx0  = _mm_shuffle_ps(y, x1 , _MM_SHUFFLE(2, 1, 2, 1)); */
    /*     DEBUG(display_vfloat32(xx0, "%4.0f", "xx0 =")); DEBUG(puts("")); */

    /*     y  = _mm_shuffle_ps(x1, x2, _MM_SHUFFLE(1, 0, 3, 2)); */
    /*     xx2  = _mm_shuffle_ps(x1, y , _MM_SHUFFLE(2, 1, 2, 1)); */
    /*     DEBUG(display_vfloat32(xx2, "%4.0f", "xx2 =")); DEBUG(puts("")); */

    /*     y = _mm_add_ps(xx0, xx2); */
    /*     y = _mm_add_ps(y, x1); */
    /*     y = _mm_div_ps(y, divider); */

    /*     _mm_store_ps((float*) &vY[i+1], y); */
    /* } */

    // CODE A COMPLETER
}
// --------------------------------------------------------
void avg3_rot_vf32matrix(vfloat32** X, int n, vfloat32 **Y)
// --------------------------------------------------------
{
    int i, j;
    vfloat32 a0, b0, c0;
    vfloat32 a1, b1, c1;
    vfloat32 a2, b2, c2;

    vfloat32 aa0, cc0;
    vfloat32 aa1, cc1;
    vfloat32 aa2, cc2;

    // CODE A COMPLETER
}
// --------------------------------------------------------
void avg3_red_vf32matrix(vfloat32** X, int n, vfloat32 **Y)
// --------------------------------------------------------
{
    int i, j;
    vfloat32 a0, b0, c0;
    vfloat32 a1, b1, c1;
    vfloat32 a2, b2, c2;

    vfloat32 aa0, cc0;
    vfloat32 aa1, cc1;
    vfloat32 aa2, cc2;

    // CODE A COMPLETER
}
// --------------------------------------------------------
void avg5_reg_vf32matrix(vfloat32** X, int n, vfloat32 **Y)
// --------------------------------------------------------
{
    int i, j;
    vfloat32 a0, b0, c0, d0, e0;
    vfloat32 a1, b1, c1, d1, e1;
    vfloat32 a2, b2, c2, d2, e2;
    vfloat32 a3, b3, c3, d3, e3;
    vfloat32 a4, b4, c4, d4, e4;

    // CODE A COMPLETER
}
// --------------------------------------------------------
void avg5_rot_vf32matrix(vfloat32** X, int n, vfloat32 **Y)
// --------------------------------------------------------
{
    int i, j;
    vfloat32 a0, b0, c0, d0, e0;
    vfloat32 a1, b1, c1, d1, e1;
    vfloat32 a2, b2, c2, d2, e2;
    vfloat32 a3, b3, c3, d3, e3;
    vfloat32 a4, b4, c4, d4, e4;

    // CODE A COMPLETER
}
// --------------------------------------------------------
void avg5_red_vf32matrix(vfloat32** X, int n, vfloat32 **Y)
// --------------------------------------------------------
{
    int i, j;
    vfloat32 a0, b0, c0, d0, e0;
    vfloat32 a1, b1, c1, d1, e1;
    vfloat32 a2, b2, c2, d2, e2;
    vfloat32 a3, b3, c3, d3, e3;
    vfloat32 a4, b4, c4, d4, e4;

    // CODE A COMPLETER
}
/* ============ */
void test2D(int n)
/* ============ */
{
    int card;

    int b; // border
    char *format = "%6.2f ";

    int si0, si1, sj0, sj1; // scalar indices
    int vi0, vi1, vj0, vj1; // vector indices
    int mi0, mi1, mj0, mj1; // memory (bounded) indices

    int si0b, si1b, sj0b, sj1b; // scalar indices with border
    int vi0b, vi1b, vj0b, vj1b; // vector indices with border
    int mi0b, mi1b, mj0b, mj1b; // memory (bounded) indices  with border

    float32  **sX, **sY3, **sY5;
    vfloat32 **vX, **vY3, **vY5;

    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    puts("===============");
    puts("=== test 2D ===");
    puts("===============");

    printf("n = %d\n\n", n);

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    b = 2; // 1 for 3x3, 2 for 5x5
    card = card = card_vfloat32();

    si0 = 0; si1 = n-1;
    sj0 = 0; sj1 = n-1;

    si0b = si0-b; si1b = si1+b;
    sj0b = sj0-b; sj1b = sj1+b;

    s2v(si0, si1, sj0, sj1, card, &vi0, &vi1, &vj0, &vj1);
    v2m(vi0, vi1, vj0, vj1, card, &mi0, &mi1, &mj0, &mj1);

    s2v(si0b, si1b, sj0b, sj1b, card, &vi0b, &vi1b, &vj0b, &vj1b);
    v2m(vi0b, vi1b, vj0b, vj1b, card, &mi0b, &mi1b, &mj0b, &mj1b);

    // allocation
    vX  = vf32matrix(vi0b, vi1b, vj0b, vj1b);
    vY3 = vf32matrix(vi0,  vi1,  vj0,  vj1);
    vY5 = vf32matrix(vi0,  vi1,  vj0,  vj1);

    printf("= %d\n", vi0b);
    printf("= %d\n", vi1b);
    printf("= %d\n", vj0b);
    printf("= %d\n", vj1b);

    printf("= %d\n", vi0);
    printf("= %d\n", vi1);
    printf("= %d\n", vj0);
    printf("= %d\n", vj1);



    // wrappers scalaires
    sX  = (float32**) vX;
    sY3 = (float32**) vY3;
    sY5 = (float32**) vY5;

    // ---------- //
    // -- init -- //
    // ---------- //

    zero_vf32matrix(vX,  vi0b, vi1b, vj0b, vj1b);
    zero_vf32matrix(vY3, vi0, vi1, vj0, vj1);
    zero_vf32matrix(vY5, vi0, vi1, vj0, vj1);

    init_vf32matrix_param(vX, vi0b, vi1b, vj0b, vj1b, 1,1,8);

    // display init data
    DEBUG(display_f32matrix(sX,  0, n-1, 0, n-1, format, "sX"));
    DEBUG(display_f32matrix(sY3, 0, n-1, 0, n-1, format, "sY3"));

    DEBUG(display_vf32matrix(vX,  vi0b, vi1b, vj0b, vj1b, format, "vX"));
    DEBUG(display_vf32matrix(vY3, vi0,  vi1,  vj0,  vj1,  format, "vY3"));
    DEBUG(display_vf32matrix(vY5, vi0,  vi1,  vj0,  vj1,  format, "vY5"));

    // ------------ //
    // -- calcul -- //
    // ------------ //
    CHRONO(avg3_reg_vf32matrix(vX, n/card, vY3),cycles); printf("avg 3x3 reg  "); DEBUG(display_vf32matrix(vY3, vi0, vi1, vj0, vj1, format, "Y3")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(avg3_rot_vf32matrix(vX, n/card, vY3),cycles); printf("avg 3x3 rot  "); DEBUG(display_vf32matrix(vY3, vi0, vi1, vj0, vj1, format, "Y3")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(avg3_red_vf32matrix(vX, n/card, vY3),cycles); printf("avg 3x3 red  "); DEBUG(display_vf32matrix(vY3, vi0, vi1, vj0, vj1, format, "Y3")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    BENCH(puts(""));

    CHRONO(avg5_reg_vf32matrix(vX, n/card, vY5),cycles); printf("avg 5x5 reg  "); DEBUG(display_vf32matrix(vY5, vi0, vi1, vj0, vj1, format, "Y5")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(avg5_rot_vf32matrix(vX, n/card, vY5),cycles); printf("avg 5x5 rot  "); DEBUG(display_vf32matrix(vY5, vi0, vi1, vj0, vj1, format, "Y5")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    CHRONO(avg5_red_vf32matrix(vX, n/card, vY5),cycles); printf("avg 5x5 red  "); DEBUG(display_vf32matrix(vY5, vi0, vi1, vj0, vj1, format, "Y5")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    //CHRONO(avg5_vf32matrix(vX, n/card, vY5),cycles); printf("avg 5x5   "); DEBUG(display_vf32matrix(vY5, vi0, vi1, vj0, vj1, format, "Y5")); BENCH(printf(format, cycles/(n*n))); BENCH(puts(""));
    BENCH(puts(""));

    // ---------- //
    // -- free -- //
    // ---------- //

    free_vf32matrix(vX, vi0b, vi1b, vj0b, vj1b);
    free_vf32matrix(vY3, vi0,  vi1,  vj0,  vj1);
    free_vf32matrix(vY5, vi0,  vi1,  vj0,  vj1);
}
// =================================
void main_2D(int argc, char * argv[])
// =================================
{

    DEBUG(test2D(8));

    BENCH(test2D(100));
    BENCH(test2D(1000));
    BENCH(test2D(2000));
}
