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


#ifndef ABC_H
#define ABC_H

#include	"configuration.h"
#include	"problem.h"
#include	"foodSource.h"
#include	"localSearch.h"

#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;

class ABC {
public:
    ABC(Configuration * conf, Problem * problem, LocalSearch * localS);
    virtual ~ABC();
    virtual void run() = 0;

    double* getSolution();
    void setSolution(const double * x);

    int getPopulationSize() {
        return NP;
    }

    void applyLocalSearch(FoodSource * bestfood);

protected:
    virtual void initFoodSources() = 0;
    virtual void sendEmployedBees() = 0;
    virtual void sendOnlookerBees() = 0;
    virtual void sendScoutBees() = 0;
    virtual void calculateProbabilities() = 0;

    bool notDone();
    double getStepSize();
    FoodSource * getBestFSource();
    int getBestFSourceIndex();

    vector<FoodSource*> foodSources; // yiyecek kaynaklarinin tutuldugu vektor
    int NP; // number of population
    double * solution, * tempSource; // cozum ve islemler icin gecici olarak kullanilan dizi
    long double solutionValue; // cozum degeri
    Configuration * config;
    Problem * p;
    int limit;
    LocalSearch * ls;

private:
    double getAdaptiveStepSize();
    void applyDefaultLSStrategy(FoodSource * bestfood);
    void applyILSStrategy(FoodSource * bestfood);

};


#endif