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

/* $Id: powell.cc,v 1.3 2006/02/19 23:59:12 hvengel Exp $ */
/* Multi-dimentional minizer using Powell or Conjugate Gradient methods */
/* This is good for smoother, well behaved functions. */

/* Code is an expression of the algorithms decsribed in */
/* "Numerical Recipes in C", by W.H.Press, B.P.Flannery, */
/* S.A.Teukolsky & W.T.Vetterling. */

/*
 * Copyright 2000 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

/* TTBD:
   Fix error handling to return status (malloc, excessive itters)
   Create to "safe" library ?
   Make standalone - ie remove numsup ?
 */

#include <cfloat>
#include <iostream>

#include "powell.h"

#undef SLOPE_SANITY_CHECK		/* exermental */

using namespace std;

Powell::Powell(Configuration * conf, Problem * prob) : LocalSearch(conf, prob) {
}

/* Standard interface for powell function */
/* return err on sucess, -1.0 on failure */

/* Result will be in cp */
int Powell::run(
        double cp[], /* Initial starting point */
        double s, /* Size of initial search area */
        double ftol, /* Tollerance of error change to stop on */
        int maxit) /* Maximum iterations allowed */ {
    int i;
    double **dmtx; /* Direction vector */
    double *spt; /* Starting point before exploring all the directions */
    double *xpt; /* Extrapolated point */
    double *svec; /* Search vector */
    int iter;
    long double retv; /* Returned function value at p */

    dmtx = dmatrixz(0, di - 1, 0, di - 1); /* Zero filled */
    spt = dvector(0, di - 1);
    xpt = dvector(0, di - 1);
    svec = dvector(0, di - 1);

    /* Create initial direction matrix by */
    /* placing search start on diagonal */
    for (i = 0; i < di; i++)
        dmtx[i][i] = s;

    /* Save the starting point */
    for (i = 0; i < di; i++) {
        spt[i] = cp[i];
    }

    /* Initial function evaluation */
    retv = evaluateFunction(cp);

    /* Iterate untill we converge on a solution, or give up. */
    for (iter = 1; iter < maxit; iter++) {
        int j;
        long double lretv; /* Last function return value */
        int ibig = 0; /* Index of biggest delta */
        double del = 0.0; /* Biggest function value decrease */
        long double pretv; /* Previous function return value */

        pretv = retv; /* Save return value at top of itteration */

        /* Loop over all directions in the set */
        for (i = 0; i < di; i++) {

            for (j = 0; j < di; j++) /* Extract this direction to make search vector */
                svec[j] = dmtx[j][i];

            /* Minimize in that direction */
            lretv = retv;
            retv = linmin(cp, svec, ftol);

            /* Record bigest function decrease, and dimension it occured on */
            if (fabs(lretv - retv) > del) {
                del = fabs(lretv - retv);
                ibig = i;
            }
        }

        /* If we have reached a suitable tollerance, then finish */
        if (2.0 * fabs(pretv - retv) <= ftol * (fabs(pretv) + fabs(retv))) {
            break;
        }

        for (i = 0; i < di; i++) {
            svec[i] = cp[i] - spt[i]; /* Average direction moved after minimization round */
            xpt[i] = cp[i] + svec[i]; /* Extrapolated point after round of minimization */
            spt[i] = cp[i]; /* New start point for next round */
        }


        /* Function value at extrapolated point */
        lretv = evaluateFunction(xpt);

        if (lretv < pretv) { /* Extrapolation keeps improving */
            long double t, t1, t2;

            t1 = pretv - retv - del;
            t2 = pretv - lretv;
            t = 2.0 * (pretv - 2.0 * retv + lretv) * t1 * t1 - del * t2 * t2;
            if (t < 0.0) {

                /* Move to the minimum of the new direction */
                retv = linmin(cp, svec, ftol);

                for (i = 0; i < di; i++) /* Save the new direction */
                    dmtx[i][ibig] = svec[i];
            }
        }
    }
    /* Free up all the temporary vectors and matrix */
    free_dvector(svec, 0, di - 1);
    free_dvector(xpt, 0, di - 1);
    free_dvector(spt, 0, di - 1);
    free_dmatrix(dmtx, 0, di - 1, 0, di - 1);

    if (iter < maxit)
        return FTOL_REACHED;

    return MAXEVAL_REACHED; /* Failed due to execessive itterations */
}

/*------------------------------*/
#define POWELL_GOLD 1.618034
#define POWELL_CGOLD 0.3819660
#define POWELL_MAXIT 100

/* Line bracketing and minimisation routine. */

/* Return value at minimum. */
long double Powell::linmin(
        double cp[], /* Start point, and returned value */
        double xi[], /* Search vector */
        double ftol) /* Tolerance to stop on */ {
    int i;
    long double ax, xx, bx; /* Search vector multipliers */
    long double af, xf, bf; /* Function values at those points */
    double *xt, XT[10]; /* Trial point */

    if (di <= 10)
        xt = XT;
    else
        xt = dvector(0, di - 1); /* Vector for trial point */

    /* -------------------------- */
    /* First bracket the solution */

    /* The line is measured as startpoint + offset * search vector */
    /* Start with ax being vector offset 0.0 */
    ax = 0.0;
    for (i = 0; i < di; i++)
        xt[i] = cp[i] + ax * xi[i];
    af = evaluateFunction(xt);

    /* xx being vector offset 0.618 */
    xx = 1.0 / POWELL_GOLD;
    for (i = 0; i < di; i++)
        xt[i] = cp[i] + xx * xi[i];
    xf = evaluateFunction(xt);

    /* Fix it so that we are decreasing from point a -> x */
    if (xf > af) {
        long double tt;
        tt = ax;
        ax = xx;
        xx = tt;
        tt = af;
        af = xf;
        xf = tt;
    }

    bx = xx + POWELL_GOLD * (xx - ax); /* Guess b beyond a -> x */
    for (i = 0; i < di; i++)
        xt[i] = cp[i] + bx * xi[i];
    bf = evaluateFunction(xt);

#ifdef SLOPE_SANITY_CHECK
    /* If we're not seeing a slope indicitive of progress */
    /* of order ftol, give up straight away */
    if (2000.0 * fabs(xf - bf) <= ftol * (fabs(xf) + fabs(bf))
            && 2000.0 * fabs(af - xf) <= ftol * (fabs(af) + fabs(xf))) {

        if (xt != XT)
            free_dvector(xt, 0, di - 1);

        if (bf < xf) {
            xf = bf;
            xx = bx;
        }

        /* Compute solution vector */
        for (i = 0; i < di; i++)
            cp[i] += xx * xi[i];
        return xf;
    }
#endif /* SLOPE_SANITY_CHECK */

    /* While not bracketed */
    while (xf > bf) {
        long double ulim, ux, uf;
        long double tt, r, q;

        /* Compute ux by parabolic interpolation from a, x & b */
        q = (xx - bx) * (xf - af);
        r = (xx - ax) * (xf - bf);
        tt = q - r;
        if (tt >= 0.0 && tt < 1e-20) /* If +ve too small */
            tt = 1e-20;
        else if (tt <= 0.0 && tt > -1e-20) /* If -ve too small */
            tt = -1e-20;
        ux = xx - ((xx - bx) * q - (xx - ax) * r) / (2.0 * tt);
        ulim = xx + 100.0 * (bx - xx); /* Extrapolation limit */


        if ((xx - ux) * (ux - bx) > 0.0) { /* u is between x and b */

            for (i = 0; i < di; i++) /* Evaluate u */
                xt[i] = cp[i] + ux * xi[i];
            uf = evaluateFunction(xt);

            if (uf < bf) { /* Minimum is between x and b */
                ax = xx;
                af = xf;
                xx = ux;
                xf = uf;
                break;
            } else if (uf > xf) { /* Minimum is between a and u */
                bx = ux;
                bf = uf;
                break;
            }

            /* Parabolic fit didn't work, look further out in direction of b */
            ux = bx + POWELL_GOLD * (bx - xx);

        } else if ((bx - ux) * (ux - ulim) > 0.0) { /* u is between b and limit */
            for (i = 0; i < di; i++) /* Evaluate u */
                xt[i] = cp[i] + ux * xi[i];
            uf = evaluateFunction(xt);

            if (uf > bf) { /* Minimum is between x and u */
                ax = xx;
                af = xf;
                xx = bx;
                xf = bf;
                bx = ux;
                bf = uf;
                break;
            }
            xx = bx;
            xf = bf; /* Continue looking */
            bx = ux;
            bf = uf;
            ux = bx + POWELL_GOLD * (bx - xx); /* Test beyond b */

        } else if ((ux - ulim) * (ulim - bx) >= 0.0) { /* u is beyond limit */
            ux = ulim;
        } else { /* u is to left side of x ? */
            ux = bx + POWELL_GOLD * (bx - xx);
        }
        /* Evaluate u, and move into place at b */
        for (i = 0; i < di; i++)
            xt[i] = cp[i] + ux * xi[i];
        uf = evaluateFunction(xt);
        ax = xx;
        af = xf;
        xx = bx;
        xf = bf;
        bx = ux;
        bf = uf;
    }
    /* Got bracketed minimum between a -> x -> b */

    /* --------------------------------------- */
    /* Now use brent minimiser bewteen a and b */
    {
        /* a and b bracket solution */
        /* x is best function value so far */
        /* w is second best function value so far */
        /* v is previous second best, or third best */
        /* u is most recently tested point */
        double wx, vx, ux; /* Search vector multipliers */
        long double wf, vf, uf; /* Function values at those points */
        int iter;
        double de = 0.0; /* Distance moved on previous step */
        double e = 0.0; /* Distance moved on 2nd previous step */

        /* Make sure a and b are in ascending order */
        if (ax > bx) {
            long double tt;
            tt = ax;
            ax = bx;
            bx = tt;
            tt = af;
            af = bf;
            bf = tt;
        }

        wx = vx = xx; /* Initial values of other center points */
        wf = xf = xf;

        for (iter = 1; iter <= POWELL_MAXIT; iter++) {
            long double mx = 0.5 * (ax + bx); /* m is center of bracket values */
            double tol1 = ftol * fabs(xx) + 1e-10;
            double tol2 = 2.0 * tol1;

            if (fabs(xx - mx) <= (tol2 - 0.5 * (bx - ax))) {
                break;
            }

            if (fabs(e) > tol1) { /* Do a trial parabolic fit */
                long double te, p, q, r;
                r = (xx - wx) * (xf - vf);
                q = (xx - vx) * (xf - wf);
                p = (xx - vx) * q - (xx - wx) * r;
                q = 2.0 * (q - r);
                if (q > 0.0)
                    p = -p;
                else
                    q = -q;
                te = e; /* Save previous e value */
                e = de; /* Previous steps distance moved */

                if (fabs(p) >= fabs(0.5 * q * te) || p <= q * (ax - xx) || p >= q * (bx - xx)) {
                    /* Give up on the parabolic fit, and use the golden section search */
                    e = ((xx >= mx) ? ax - xx : bx - xx); /* Override previous distance moved */
                    de = POWELL_CGOLD * e;

                } else { /* Use parabolic fit */
                    de = p / q; /* Change in xb */
                    ux = xx + de; /* Trial point according to parabolic fit */
                    if ((ux - ax) < tol2 || (bx - ux) < tol2) {
                        if ((mx - xx) > 0.0) /* Don't use parabolic, use tol1 */
                            de = tol1; /* tol1 is +ve */
                        else
                            de = -tol1;
                    }
                }
            } else { /* Keep using the golden section search */
                e = ((xx >= mx) ? ax - xx : bx - xx); /* Override previous distance moved */
                de = POWELL_CGOLD * e;
            }

            if (fabs(de) >= tol1) /* If de moves as much as tol1 would */
                ux = xx + de; /* use it */
            else { /* else move by tol1 in direction de */
                if (de > 0.0)
                    ux = xx + tol1;
                else
                    ux = xx - tol1;
            }

            /* Evaluate function */
            for (i = 0; i < di; i++)
                xt[i] = cp[i] + ux * xi[i];
            uf = evaluateFunction(xt);

            if (uf <= xf) { /* Found new best solution */
                if (ux >= xx) {
                    ax = xx;
                    af = xf; /* New lower bracket */
                } else {
                    bx = xx;
                    bf = xf; /* New upper bracket */
                }
                vx = wx;
                vf = wf; /* New previous 2nd best solution */
                wx = xx;
                wf = xf; /* New 2nd best solution from previous best */
                xx = ux;
                xf = uf; /* New best solution from latest */
            } else { /* Found a worse solution */
                if (ux < xx) {
                    ax = ux;
                    af = uf; /* New lower bracket */
                } else {
                    bx = ux;
                    bf = uf; /* New upper bracket */
                }
                if (uf <= wf || wx == xx) { /* New 2nd best solution, or equal best */
                    vx = wx;
                    vf = wf; /* New previous 2nd best solution */
                    wx = ux;
                    wf = uf; /* New 2nd best from latest */
                } else if (uf <= vf || vx == xx || vx == wx) { /* New 3rd best, or equal 1st & 2nd */
                    vx = ux;
                    vf = uf; /* New previous 2nd best from latest */
                }
            }
        }
        /* !!! should do something if iter > POWELL_MAXIT !!!! */
        /* Solution is at xx, xf */

        /* Compute solution vector */
        for (i = 0; i < di; i++)
            cp[i] += xx * xi[i];
    }

    if (xt != XT)
        free_dvector(xt, 0, di - 1);
    return xf;
}

#undef POWELL_GOLD
#undef POWELL_CGOLD
#undef POWELL_MAXIT

/**************************************************/


void Powell::apply(FoodSource * f, double stepSize, unsigned int itr) {

    const double* pos = f->getLocation();
    double x[config->getProblemDimension()];

    double ftol = config->getFTol();
    // create a temporary position vector for the particle position
    for (int i = 0; i < config->getProblemDimension(); i++)
        x[i] = pos[i];

    int ret = 0;
    // apply local search, returns the termination code
    ret = run(x, stepSize, ftol, itr); //config->getLsEvaluations());

    //update best position and value of the particle
    if (f->getObjectiveValue() > getBestValue()) {
        f->setObjectiveValue(getBestValue());
        f->setLocation(getBestPosition());
        f->initTrail();
    }

    switch (ret) {
        case FTOL_REACHED:
        {

        }
            break;
        case MAXEVAL_REACHED:
        {
        }
            break;
        default:
        {
            cout << "Error" << endl;
            exit(1);
        }
    }
}
