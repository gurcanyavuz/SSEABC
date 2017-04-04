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


#ifndef ICMAESILS_H
#define ICMAESILS_H


#include "configuration.h"
#include "problem.h"
#include "localSearch.h"

using namespace std;
extern double tunec;

class ICMAESLS:public LocalSearch {

    protected:

    public:

	ICMAESLS(Configuration* config, Problem* problem);
	~ICMAESLS(void);
	bool finished();

	void apply(FoodSource * f, double stepSize, unsigned int itr);

    private:
	long double icmaes (unsigned int maxfevalsforicmaes, double* feedback);
	bool inbound(long double min, long double max, double *ind, int num);
	
	double *const*pop;
	int popsize;
	int lambda;
	double* stddev;
};


#endif
