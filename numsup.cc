/*
    Copyright 2016 Gurcan Yavuz, Dogan Aydin and Thomas Stuetzle
    
	Gurcan Yavuz	<gurcan.yavuz@dpu.edu.tr>
    Dogan Aydin		<dogan.aydin@dpu.edu.tr>
    Thomas Stuetzle	<stuetzle@ulb.ac.be>

    This file is part of SSEABC.

    SSEABC is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSEABC is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with SSEABC.  If not, see <http://www.gnu.org/licenses/>.
*/

/* $Id: numsup.cc,v 1.3 2006/02/19 23:59:12 hvengel Exp $ */
/* General Numerical routines support functions */

#include <cstdio>
#include <cstdlib>
#include <cmath>

#include "numsup.h"

/*
 * TODO: Should probably break the error handler out into
 *       a separate library, so that it can be ommitted.
 *       Or enhance it so that numerical callers of error()
 *       can get a callback on out of memory etc. ???
 *
 */

char error_program[] = "Numeric";
#define ERROR_OUT_DEFAULT stderr
#define WARN_OUT_DEFAULT stderr
#define VERBOSE_OUT_DEFAULT stdout

int verbose_level = 6; /* Current verbosity level */
/* 0 = none */
/* !0 = diagnostics */

#undef RETURN_NULL_ON_MALLOC	/* Else error out here */

/******************************************************************/
/* Error/debug output routines */
/******************************************************************/

/* Globals - can be changed on the fly */
FILE *error_out = NULL;
FILE *warn_out = NULL;
FILE *verbose_out = NULL;

/* Basic printf type error() and warning() routines */
void
error(char fmt[], ...) {
    va_list args;

    if (error_out == NULL)
        error_out = ERROR_OUT_DEFAULT;
    fprintf(error_out, "%s: Error - ", error_program);
    va_start(args, fmt);
    vfprintf(error_out, fmt, args);
    va_end(args);
    fprintf(error_out, "\n");
    fflush(error_out);
    exit(-1);
}

void
warning(char fmt[], ...) {
    va_list args;

    if (warn_out == NULL)
        warn_out = WARN_OUT_DEFAULT;
    fprintf(warn_out, "%s: Warning - ", error_program);
    va_start(args, fmt);
    vfprintf(warn_out, fmt, args);
    va_end(args);
    fprintf(warn_out, "\n");
    fflush(warn_out);
}

void
verbose(int level, char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    if (verbose_level >= level) {
        if (verbose_out == NULL)
            verbose_out = VERBOSE_OUT_DEFAULT;
        fprintf(verbose_out, "%s: ", error_program);
        vfprintf(verbose_out, fmt, args);
        fprintf(verbose_out, "\n");
        fflush(verbose_out);
    }
    va_end(args);
}

/******************************************************************/
/* Numerical Recipes Vector/Matrix Support functions              */
/******************************************************************/
/* Note the z suffix versions return zero'd vectors/matricies */

/* Float Vector malloc/free */
float *fvector(
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    float *v;
    char error_msg[] = "Malloc failure in dvector()";
    if ((v = (float *) malloc((nh - nl + 1) * sizeof (float))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else

        error(error_msg);
#endif
    return v - nl;
}

float *fvectorz(
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    float *v;
    char error_msg[] = "Malloc failure in dvector()";
    if ((v = (float *) calloc(nh - nl + 1, sizeof (float))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg);
#endif
    return v - nl;
}

void free_fvector(
        float *v,
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    free((char *) (v + nl));
}

/* --------------------- */

/* 2D Float vector malloc/free */
float **fmatrix(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i;
    int rows, cols;
    float **m;
    char error_msg_p[] = "Malloc failure in dmatrix(), pointers";
    char error_msg_a[] = "Malloc failure in dmatrix(), array";
    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if ((m = (float **) malloc((rows + 1) * sizeof (float *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_p);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (float *) malloc(rows * cols * sizeof (float))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_a);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1; i <= nrh; i++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + cols;

    return m;
}

float **fmatrixz(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i;
    int rows, cols;
    float **m;
    char error_msg_p[] = "Malloc failure in dmatrix(), pointers";
    char error_msg_a[] = "Malloc failure in dmatrix(), array";

    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if ((m = (float **) malloc((rows + 1) * sizeof (float *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_p);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (float *) calloc(rows * cols, sizeof (float))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_a);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1; i <= nrh; i++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + cols;

    return m;
}

void free_fmatrix(
        float **m,
        int nrl,
        int nrh,
        int ncl,
        int nch
        ) {
    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    free((char *) (m[nrl - 1]));
    free((char *) (m + nrl - 1));
}

/* -------------------------- */

/* Double Vector malloc/free */
double *dvector(
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    double *v;
    char error_msg[] = "Malloc failure in dvector()";

    if ((v = (double *) malloc((nh - nl + 1) * sizeof (double))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg);
#endif
    return v - nl;
}

double *dvectorz(
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    double *v;
    char error_msg[] = "Malloc failure in dvector()";

    if ((v = (double *) calloc(nh - nl + 1, sizeof (double))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg);
#endif
    return v - nl;
}

void free_dvector(
        double *v,
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    free((char *) (v + nl));
}

/* --------------------- */

/* 2D Double vector malloc/free */
double **dmatrix(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i;
    int rows, cols;
    double **m;
    char error_msg_p[] = "Malloc failure in dmatrix(), pointers";
    char error_msg_a[] = "Malloc failure in dmatrix(), array";

    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if ((m = (double **) malloc((rows + 1) * sizeof (double *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_p);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (double *) malloc(rows * cols * sizeof (double))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_a);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1; i <= nrh; i++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + cols;

    return m;
}

double **dmatrixz(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i;
    int rows, cols;
    double **m;
    char error_msg_p[] = "Malloc failure in dmatrix(), pointers";
    char error_msg_a[] = "Malloc failure in dmatrix(), array";

    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if ((m = (double **) malloc((rows + 1) * sizeof (double *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_p);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (double *) calloc(rows * cols, sizeof (double))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_a);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1; i <= nrh; i++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + cols;

    return m;
}

void free_dmatrix(
        double **m,
        int nrl,
        int nrh,
        int ncl,
        int nch
        ) {
    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    free((char *) (m[nrl - 1]));
    free((char *) (m + nrl - 1));
}

/* --------------------- */
/* 2D diagonal half matrix vector malloc/free */
/* A half matrix must have equal rows and columns, */

/* and the column address must always be >= than the row. */
double **dhmatrix(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i, j;
    int rows, cols; /* , nn; */
    double **m;
    char error_msg1[] = "dhmatrix() given unequal rows and columns";
    char error_msg2[] = "Malloc failure in dhmatrix(), pointers";
    char error_msg3[] = "Malloc failure in dhmatrix(), array";

    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if (rows != cols)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg1);
#endif

    if ((m = (double **) malloc((rows + 1) * sizeof (double *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg2);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (double *) malloc((rows * rows + rows) / 2 * sizeof (double))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg3);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1, j = 1; i <= nrh; i++, j++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + j; /* Start with 1 entry and increment */

    return m;
}

double **dhmatrixz(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i, j;
    int rows, cols; /* , nn; */
    double **m;
    char error_msg1[] = "dhmatrix() given unequal rows and columns";
    char error_msg2[] = "Malloc failure in dhmatrix(), pointers";
    char error_msg3[] = "Malloc failure in dhmatrix(), array";

    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if (rows != cols)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg1);
#endif

    if ((m = (double **) malloc((rows + 1) * sizeof (double *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg2);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (double *) calloc((rows * rows + rows) / 2, sizeof (double))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg3);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1, j = 1; i <= nrh; i++, j++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + j; /* Start with 1 entry and increment */

    return m;
}

void free_dhmatrix(
        double **m,
        int nrl,
        int nrh,
        int ncl,
        int nch
        ) {
    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    free((char *) (m[nrl - 1]));
    free((char *) (m + nrl - 1));
}

/* --------------------- */

/* 2D vector copy */
void copy_dmatrix(
        double **dst,
        double **src,
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i, j;
    for (j = nrl; j <= nrh; j++)
        for (i = ncl; i <= nch; i++)
            dst[j][i] = src[j][i];
}

/* -------------------------------------------------------------- */

/* From an indirect array reference to a standard C type 2D array */
double **convert_dmatrix(
        double *a, /* base address of normal C array, ie &a[0][0] */
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i, j, nrow = nrh - nrl + 1, ncol = nch - ncl + 1;
    double **m;
    char error_msg[] = "Malloc failure in convert_dmatrix()";

    /* Allocate pointers to rows */
    if ((m = (double **) malloc(nrow * sizeof (double*))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg);
#endif

    m -= nrl;

    m[nrl] = a - ncl;
    for (i = 1, j = nrl + 1; i < nrow; i++, j++)
        m[j] = m[j - 1] + ncol;
    /* return pointer to array of pointers */
    return m;
}

/* Free the indirect array reference (but not array) */
void free_convert_dmatrix(
        double **m,
        int nrl,
        int nrh,
        int ncl,
        int nch
        ) {
    free((char*) (m + nrl));
}

/* ------------------ */

/* Integer vector malloc/free */
int *ivector(
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    int *v;
    char error_msg[] = "Malloc failure in dvector()";
    if ((v = (int *) malloc((nh - nl + 1) * sizeof (int))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg);
#endif
    return v - nl;
}

int *ivectorz(
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    int *v;
    char error_msg[] = "Malloc failure in dvector()";
    if ((v = (int *) calloc(nh - nl + 1, sizeof (int))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg);
#endif
    return v - nl;
}

void free_ivector(
        int *v,
        int nl, /* Lowest index */
        int nh /* Highest index */
        ) {
    free((char *) (v + nl));
}


/* ------------------------------ */

/* 2D integer vector malloc/free */

int **imatrix(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i;
    int rows, cols;
    int **m;
    char error_msg_p[] = "Malloc failure in imatrix(), pointers";
    char error_msg_a[] = "Malloc failure in imatrix(), array";

    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if ((m = (int **) malloc((rows + 1) * sizeof (int *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_p);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (int *) malloc(rows * cols * sizeof (int))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_a);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1; i <= nrh; i++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + cols;

    return m;
}

int **imatrixz(
        int nrl, /* Row low index */
        int nrh, /* Row high index */
        int ncl, /* Col low index */
        int nch /* Col high index */
        ) {
    int i;
    int rows, cols;
    int **m;
    char error_msg_p[] = "Malloc failure in imatrix(), pointers";
    char error_msg_a[] = "Malloc failure in imatrix(), array";

    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    rows = nrh - nrl + 1;
    cols = nch - ncl + 1;

    if ((m = (int **) malloc((rows + 1) * sizeof (int *))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_p);
#endif
    m -= nrl; /* Offset to nrl */
    m += 1; /* Make nrl-1 pointer to main allocation, in case rows get swaped */

    if ((m[nrl - 1] = (int *) calloc(rows * cols, sizeof (int))) == NULL)
#ifdef RETURN_NULL_ON_MALLOC
        return NULL;
#else
        error(error_msg_a);
#endif

    m[nrl] = m[nrl - 1] - ncl; /* Set first row address, offset to ncl */
    for (i = nrl + 1; i <= nrh; i++) /* Set subsequent row addresses */
        m[i] = m[i - 1] + cols;

    return m;
}

void free_imatrix(
        int **m,
        int nrl,
        int nrh,
        int ncl,
        int nch
        ) {
    if (nrh < nrl) /* Prevent failure for 0 dimension */
        nrh = nrl;
    if (nch < ncl)
        nch = ncl;

    free((char *) (m[nrl - 1]));
    free((char *) (m + nrl - 1));
}

