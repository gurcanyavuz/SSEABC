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

#ifndef PROBLEM_H
#define PROBLEM_H

#include "configuration.h"
#include <cmath>
#include <algorithm>

#include <stdio.h>
#include <math.h>
//#include <malloc.h>
#include <stdlib.h>


#ifdef __MACH__
#include "sys/malloc.h" // mac os x
#else
#include "malloc.h" // linux, windows
#endif

#define abss(a)     (a<0 ? (-a) : a)
#define INF 1.0e99
#define EPS 1.0e-14	
#define EPSILON 1.0e-20
#define E  2.7182818284590452353602874713526625

class Problem {
protected:
    Configuration* config;
    int dimension;
    unsigned int evaluations;
    unsigned int maxEvaluations;
    double optimum;
    long double oldResult;

    bool firstEvaluation;
    long double bestSolutionValue;
    double* bestSoFarSolution;

    double *OShift, *M, *y, *z, *x_bound;
    int ini_flag, n_flag, func_flag, *SS;

    void sphere_func(double *, double *, int, double *, double *, int, int); /* Sphere */ //
    void ellips_func(double *, double *, int, double *, double *, int, int); /* Ellipsoidal */ //
    void bent_cigar_func(double *, double *, int, double *, double *, int, int); /* Discus */ //
    void discus_func(double *, double *, int, double *, double *, int, int); /* Bent_Cigar */ //
    void dif_powers_func(double *, double *, int, double *, double *, int, int); /* Different Powers */ //
    void rosenbrock_func(double *, double *, int, double *, double *, int, int); /* Rosenbrock's */ //
    void schaffer_F7_func(double *, double *, int, double *, double *, int, int); /* Schwefel's F7 */ //
    void ackley_func(double *, double *, int, double *, double *, int, int); /* Ackley's */ //
    void rastrigin_func(double *, double *, int, double *, double *, int, int); /* Rastrigin's  */ //
    void weierstrass_func(double *, double *, int, double *, double *, int, int); /* Weierstrass's  */ ///
    void griewank_func(double *, double *, int, double *, double *, int, int); /* Griewank's  */ //
    void schwefel_func(double *, double *, int, double *, double *, int, int); /* Schwefel's */ //
    void katsuura_func(double *, double *, int, double *, double *, int, int); /* Katsuura */ //
    void bi_rastrigin_func(double *, double *, int, double *, double *, int, int); /* Lunacek Bi_rastrigin */ //
    void grie_rosen_func(double *, double *, int, double *, double *, int, int); /* Griewank-Rosenbrock  */ //
    void escaffer6_func(double *, double *, int, double *, double *, int, int); /* Expanded Scaffer??s F6  */ //
    void step_rastrigin_func(double *, double *, int, double *, double *, int, int); /* Noncontinuous Rastrigin's  */ //
    void happycat_func(double *, double *, int, double *, double *, int, int); /* HappyCat */ //
    void hgbat_func(double *, double *, int, double *, double *, int, int); /* HGBat  */ //

    void hf01(double *, double *, int, double *, double *, int *, int, int); /* Hybrid Function 1 */ //
    void hf02(double *, double *, int, double *, double *, int *, int, int); /* Hybrid Function 2 */ // 
    void hf03(double *, double *, int, double *, double *, int *, int, int); /* Hybrid Function 3 */ //
    void hf04(double *, double *, int, double *, double *, int *, int, int); /* Hybrid Function 4 */
    void hf05(double *, double *, int, double *, double *, int *, int, int); /* Hybrid Function 5 */
    void hf06(double *, double *, int, double *, double *, int *, int, int); /* Hybrid Function 6 */

    void cf01(double *, double *, int, double *, double *, int); /* Composition Function 1 */ //
    void cf02(double *, double *, int, double *, double *, int); /* Composition Function 2 */ //
    void cf03(double *, double *, int, double *, double *, int); /* Composition Function 3 */ //
    void cf04(double *, double *, int, double *, double *, int); /* Composition Function 4 */ //
    void cf05(double *, double *, int, double *, double *, int); /* Composition Function 5 */ //
    void cf06(double *, double *, int, double *, double *, int); /* Composition Function 6 */ //
    void cf07(double *, double *, int, double *, double *, int *, int); /* Composition Function 7 */ //
    void cf08(double *, double *, int, double *, double *, int *, int); /* Composition Function 8 */

    void shiftfunc(double*, double*, int, double*);
    void rotatefunc(double*, double*, int, double*);
    void sr_func(double *, double *, int, double*, double*, double, int, int); /* shift and rotate */
    void asyfunc(double *, double *x, int, double);
    void oszfunc(double *, double *, int);
    void cf_cal(double *, double *, int, double *, double *, double *, double *, int);

public:

    Problem(Configuration* config);
    virtual ~Problem();

    int getProblemDimension();
    //double getProblemOptimum();

    unsigned int getFunctionEvaluations();
    void setFunctionEvaluations(unsigned int evals);

    //Evaluation call
    long double evaluate(int nx, double* x);
    long double getFunctionValue(double* x);

    long double getBestSolutionValue();
    double* getBestSoFarSolution();
    void setBestSoFarSolution(const double* solution);

    void setBestSolutionValue(long double value);

    void printProgress();

};


#endif

