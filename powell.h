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

#ifndef POWELL_H
#define POWELL_H
/* $Id: powell.h,v 1.2 2006/02/19 23:59:12 hvengel Exp $ */
/* Powell multivariate minimiser */

/*
 * Copyright 2000 Graeme W. Gill
 * All rights reserved.
 *
 * This material is licenced under the GNU GENERAL PUBLIC LICENCE :-
 * see the Licence.txt file for licencing details.
 */

#include "numsup.h"
#include "configuration.h"
#include "problem.h"
#include "localSearch.h"

#define FTOL_REACHED -2
#define MAXEVAL_REACHED 1

/* Standard interface for powell function */
/* return err on sucess, -1.0 on failure */
/* Result will be in cp */

/* Arrays start at 0 */
class Powell : public LocalSearch {
public:

    Powell(Configuration * conf, Problem * prob);

    ~Powell() {
    }

    void apply(FoodSource * f, double stepSize, unsigned int itr);

    //	long double getBestValue();
    //	double * getBestPosition();

private:
    //	int di;
    //	Configuration * config;
    //	Problem * p;
    //	long double bestVal;
    //	double * bestPos;
    int run(
            double cp[], /* Initial starting point */
            double s, /* Size of initial search area */
            double ftol, /* Tollerance of error change to stop on */
            int maxit /* Maximum iterations allowed */
            );

    long double linmin(double cp[], /* Start point, and returned value */
            double xi[], /* Search vector */
            double ftol); /* Tolerance to stop on */

    //	long double addPenalty(const double *x);
    //	long double evaluateFunction(const double *x);

    //	void setBestPosition(const double * x);

};


#endif
