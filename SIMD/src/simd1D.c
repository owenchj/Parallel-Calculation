/* ---------------- */
/* --- simd1D.c --- */
/* ---------------- */

#include <stdio.h>
#include <stdlib.h>

#include "nrdef.h"
#include "nrutil.h"

#include "vnrdef.h"
#include "vnrutil.h"

#include "mutil.h"

#include "mymacro.h"
#include "simd_macro.h"

// -------------------------------------------------------------------
void add_vf32vector(vfloat32 *vX1, vfloat32 *vX2, int n, vfloat32 *vY)
// -------------------------------------------------------------------
{
    int i;
    vfloat32 x1, x2, y;

    for(i=0; i<n; i++) {

        x1 = _mm_load_ps((float32*) &vX1[i]);
        x2 = _mm_load_ps((float32*) &vX2[i]);

        y = _mm_add_ps(x1, x2);

        /* DEBUG(display_vfloat32(x1, "%4.0f", "x1 =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(x2, "%4.0f", "x2 =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(y,  "%4.0f", "y  =")); DEBUG(puts("")); */

        _mm_store_ps((float*) &vY[i], y);
    }
}
// ---------------------------------------------------------
vfloat32 dot_vf32vector(vfloat32 *vX1, vfloat32 *vX2, int n)
// ---------------------------------------------------------
{
    int i;
    vfloat32 x1, x2, p, s, s0;

    for(i=0; i<n; i++) {
        x1 = _mm_load_ps((float32*) &vX1[i]);
        x2 = _mm_load_ps((float32*) &vX2[i]);

        /* DEBUG(display_vfloat32(x1, "%4.0f", "x1 =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(x2, "%4.0f", "x2 =")); DEBUG(puts("")); */

        p   = _mm_mul_ps(x1, x2);
        s0  = _mm_shuffle_ps(p , p , _MM_SHUFFLE(2, 3, 0, 1));
        s0  = _mm_add_ps(p, s0);
        p   = _mm_shuffle_ps(s0, s0, _MM_SHUFFLE(1, 0, 3, 2));
        s  = vec_add3(p, s0, s);
    }
    // CODE A COMPLETER EN SSE1


    return s; // attention il faut retourner un registre SIMD et non un scalaire
}
// ----------------------------------------------------
void avg3_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// ----------------------------------------------------
{
    int i;
    vfloat32 x0, x1, x2; // registres alignes
    vfloat32 xx0, xx2; // registres non alignes

    for(i=-1; i<n-1; i++) {

        x0 = _mm_load_ps((float32*) &vX[i]);
        x1 = _mm_load_ps((float32*) &vX[i+1]);
        x2 = _mm_load_ps((float32*) &vX[i+2]);

        // vec_left1(1 2 3 4 | 5 6 7 8) => 4 5 6 7
        xx0 = vec_left1(x0, x1);
        // vec_left1(1 2 3 4 | 5 6 7 8) => 2 3 4 5
        xx2 = vec_right1(x1, x2);

        _mm_store_ps((float*) &vY[i+1], vec_div3(vec_add3(xx0, x1, xx2)));
    }


    // CODE A COMPLETER EN SSE1
}
// ----------------------------------------------------
void avg5_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// ----------------------------------------------------
{
    int i;
    vfloat32 x1, x2, x3; // registres alignes
    vfloat32 xx0, xx1, xx3, xx4; // registres non alignes

    for(i=-1; i<n-1; i++) {

        x1 = _mm_load_ps((float32*) &vX[i]);
        x2 = _mm_load_ps((float32*) &vX[i+1]);
        x3 = _mm_load_ps((float32*) &vX[i+2]);

        /* DEBUG(display_vfloat32(x1, "%4.0f", "x1  =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(x2, "%4.0f", "x2  =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(x3, "%4.0f", "x3  =")); DEBUG(puts("")); */

        xx0 = vec_left2(x1, x2);
        xx1 = vec_left1(x1, x2);
        // X2
        xx3 = vec_right1(x2, x3);
        xx4 = vec_right2(x2, x3);

        _mm_store_ps((float*) &vY[i+1], vec_div5(vec_add5(xx0, xx1, x2, xx3, xx4)));
    }

    // CODE A COMPLETER EN SSE1
}
// --------------------------------------------------------
void avg3_rot_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// --------------------------------------------------------
{
    int i, m, k;
    vfloat32 x0, x1, x2; // registres alignes
    vfloat32 xx0, xx2; // registres non alignes

    for(i=-1; i<n-1; i++) {

        if(i < 0)
            m = n + i;
        else
            m = i;

        x0 = _mm_load_ps((float32*) &vX[m % n]);
        x1 = _mm_load_ps((float32*) &vX[(i+1) % n]);
        x2 = _mm_load_ps((float32*) &vX[(i+2) % n]);

        // vec_left1(1 2 3 4 | 5 6 7 8) => 4 5 6 7
        xx0 = vec_left1(x0, x1);
        // vec_left1(1 2 3 4 | 5 6 7 8) => 2 3 4 5
        xx2 = vec_right1(x1, x2);

        _mm_store_ps((float*) &vY[i+1], vec_div3(vec_add3(xx0, x1, xx2)));
    }

    // CODE A COMPLETER EN SSE1
}
// --------------------------------------------------------
void avg5_rot_vf32vector(vfloat32 *vX, int n, vfloat32 *vY)
// --------------------------------------------------------
{
    int i, m;
    vfloat32 x1, x2, x3; // registres alignes
    vfloat32 xx0, xx1, xx3, xx4; // registres non alignes

    for(i=-1; i<n-1; i++) {

        if(i < 0)
            m = n + i;
        else
            m = i;

        x1 = _mm_load_ps((float32*) &vX[m % n]);
        x2 = _mm_load_ps((float32*) &vX[(i+1) % n]);
        x3 = _mm_load_ps((float32*) &vX[(i+2) % n]);

        /* DEBUG(display_vfloat32(x1, "%4.0f", "x1  =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(x2, "%4.0f", "x2  =")); DEBUG(puts("")); */
        /* DEBUG(display_vfloat32(x3, "%4.0f", "x3  =")); DEBUG(puts("")); */

        xx0 = vec_left2(x1, x2);
        xx1 = vec_left1(x1, x2);
        // X2
        xx3 = vec_right1(x2, x3);
        xx4 = vec_right2(x2, x3);

        _mm_store_ps((float*) &vY[i+1], vec_div5(vec_add5(xx0, xx1, x2, xx3, xx4)));
    }

    // CODE A COMPLETER EN SSE1
}
/* ========================== */
/* === Fonctions de tests === */
/* ========================== */


// ---------------
void test1D(int n)
// ---------------
{
    int b = 2; // border
    int card; // cardinal of vector type

    int si0, si1; // scalar indices
    int vi0, vi1; // vector indices
    int mi0, mi1; // memory (bounded) indices

    int si0b, si1b; // scalar indices with border
    int vi0b, vi1b; // vector indices with border
    int mi0b, mi1b; // memory (bounded) indices  with border


    vfloat32 *vX1, *vX2, *vY, *vY3, *vY5;
    vfloat32 d;

    char* format = "%6.2f ";

    // chronometrie
    int iter, niter = 4;
    int run, nrun = 5;
    double t0, t1, dt, tmin, t;
    double cycles;

    puts("===============");
    puts("=== test 1D ===");
    puts("===============");

    // ------------------------- //
    // -- calculs des indices -- //
    // ------------------------- //

    printf("n = %d\n", n);

    // 4
    card = card_vfloat32();

    si0 = 0;
    si1 = n-1;

    s2v1D(si0, si1, card, &vi0, &vi1);
    v2m1D(vi0, vi1, card, &mi0, &mi1);

    si0b = si0-b;
    si1b = si1+b;

    s2v1D(si0b, si1b, card, &vi0b, &vi1b);
    v2m1D(vi0b, vi1b, card, &mi0b, &mi1b);

    // ------------------------------------------- //
    // -- allocation des tableaux 1D vectoriels -- //
    // ------------------------------------------- //

    vX1 = vf32vector(vi0b, vi1b);
    vX2 = vf32vector(vi0b, vi1b);

    vY  = vf32vector(vi0, vi1);
    vY3 = vf32vector(vi0, vi1);
    vY5 = vf32vector(vi0, vi1);

    // ---------- //
    // -- init -- //
    // ---------- //

    zero_vf32vector(vX1, vi0b, vi1b);
    zero_vf32vector(vX2, vi0b, vi1b);
    zero_vf32vector(vY,  vi0, vi1);
    zero_vf32vector(vY3, vi0, vi1);
    zero_vf32vector(vY5, vi0, vi1);

    init_vf32vector_param(vX1, vi0, vi1, 1, 1);
    init_vf32vector_param(vX2, vi0, vi1, 1, 2);

    // --------------- //
    // -- affichage -- //
    // --------------- //

    // affichage classique sur une ligne: appel de la fonction scalaire
    DEBUG(display_f32vector((float32*) vX1, si0, si1, "%4.0f", "sX1"));

    // affichage par bloc SIMD: appel de la fonction SIMD
    DEBUG(display_vf32vector(vX1, vi0, vi1, "%4.0f", "vX1"));
    DEBUG(puts(""));

    // affichage classique sur une ligne: appel de la fonction scalaire
    DEBUG(display_f32vector((float32*) vX2, si0, si1, "%4.0f", "sX2"));

    // affichage par bloc SIMD: appel de la fonction SIMD
    DEBUG(display_vf32vector(vX2, vi0, vi1, "%4.0f", "vX2"));
    DEBUG(puts(""));

    // ------------ //
    // -- calcul -- //
    // ------------ //

    puts("----------------");
    puts("--- addition ---");
    puts("----------------");

    CHRONO(add_vf32vector(vX1, vX2, n/card, vY),cycles); printf("add: "); DEBUG(display_vf32vector(vY, vi0, vi1, format, "Y")); BENCH(printf(format, cycles/n)); BENCH(puts(""));

    // affichage classique sur une ligne: appel de la fonction scalaire
    DEBUG(display_f32vector((float32*) vY, si0, si1, "%4.0f", "sY"));
    DEBUG(puts(""));

    puts("-------------------");
    puts("--- dot product ---");
    puts("-------------------");

    CHRONO(d = dot_vf32vector(vX1, vX2, n/card),cycles); printf("dot product: "); DEBUG(display_vfloat32(d, "%6.0f ", "d")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    display_vfloat32(d, "%6.0f ", "d"); puts(""); // laisser sinon dead-code-elimination enleve tout le calcul ...

    puts("-----------");
    puts("--- avg ---");
    puts("-----------");

    CHRONO(avg3_vf32vector(vX1, n/card, vY3),cycles); printf("avg 3   "); DEBUG(display_vf32vector(vY3, vi0, vi1, format, "Y3")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    CHRONO(avg5_vf32vector(vX1, n/card, vY5),cycles); printf("avg 5   "); DEBUG(display_vf32vector(vY5, vi0, vi1, format, "Y5")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    BENCH(puts(""));

    CHRONO(avg3_rot_vf32vector(vX1, n/card, vY3),cycles); printf("avg 3 rot"); DEBUG(display_vf32vector(vY3, vi0, vi1, format, "Y3")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    CHRONO(avg5_rot_vf32vector(vX1, n/card, vY5),cycles); printf("avg 5 rot"); DEBUG(display_vf32vector(vY5, vi0, vi1, format, "Y5")); BENCH(printf(format, cycles/n)); BENCH(puts(""));
    BENCH(puts(""));


    // ---------- //
    // -- free -- //
    // ---------- //

    free_vf32vector(vX1, vi0b, vi1b);
    free_vf32vector(vX2, vi0b, vi1b);

    free_vf32vector(vY,  vi0, vi1);
    free_vf32vector(vY3, vi0, vi1);
    free_vf32vector(vY5, vi0, vi1);


}
// ================================
void main_1D(int argc, char *argv[])
// ================================
{
    DEBUG(test1D(12));

    BENCH(test1D(100 * 100));
    BENCH(test1D(1000 * 1000));
}
