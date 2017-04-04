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

#ifndef LOCALSEARCH_H_INCLUDED
#define LOCALSEARCH_H_INCLUDED

#include "problem.h"
#include "configuration.h"
#include "foodSource.h"

class LocalSearch {

    public:
        LocalSearch(Configuration* config, Problem* problem );
        virtual ~LocalSearch();


	virtual void apply(FoodSource * f, double stepSize, unsigned int itr) = 0;

	long double getBestValue();
	double * getBestPosition();
	
    protected:
        Problem* problem;
        Configuration* config;
	int di;
	
	long double bestVal;
	double * bestPos;
	
	long double addPenalty(const double *x);
	long double evaluateFunction(double *x);
	
	void setBestPosition(const double * x);
};

#endif // LOCALSEARCH_H_INCLUDED
