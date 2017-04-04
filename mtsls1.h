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

#ifndef MTSLS1_H
#define MTSLS1_H

#include "configuration.h"
#include "problem.h"
#include "localSearch.h"

class Mtsls1 : public LocalSearch {
public:


    Mtsls1(Configuration * conf, Problem * prob);

    ~Mtsls1() {
    }

    void apply(FoodSource * f, double stepSize, unsigned int itr);
    bool finished();
    //	long double getBestValue();
    //	double * getBestPosition();

private:


    int run(double * xk, double s, unsigned int maxitr);

    bool improve;
    int grade;
    int bonus1;
    int bonus2;
    //	int di;

    //	Configuration * config;
    //	Problem * p;
    //	long double bestVal;
    //	double * bestPos;

    //	long double addPenalty(const double *x);
    //	long double evaluateFunction(const double *x);

    //	void setBestPosition(const double * x);

};


#endif