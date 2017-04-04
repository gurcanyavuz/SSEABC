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

#ifndef FOODSOURCE_H
#define FOODSOURCE_H

#include	"configuration.h"
#include	"problem.h"

class FoodSource {
public:
    FoodSource(Configuration * conf, Problem * problem);
    //       FoodSource(Configuration * conf, Problem * problem, unsigned int K);
    //       FoodSource(Configuration * conf, Problem * problem, double* loc);
    FoodSource(Configuration * conf, Problem * problem, FoodSource * bestFood);
    FoodSource(FoodSource * copy);
    ~FoodSource();

    void reInit();
    //       void reInit(const double * bestSoFarSolution);
    //       void reInit(double shake, const double * bestSoFarSolution);
    //       void chaoticReInit(unsigned int K);
    //       void inverseReInit(double * loc);

    long double getFitness() {
        return 1.0 / (objectiveValue + 1.0);
    }

    // This Fitness Function is for Rosenbrock ABC 

    long double getFitness(double sp, int order, unsigned int popSize) {
        return 2 - sp + 2 * (sp - 1) * (order - 1) / (popSize - 1);
    }

    long double getObjectiveValue() {
        return objectiveValue;
    }

    void setObjectiveValue(long double value) {
        objectiveValue = value;
    }

    void calculateObjectiveValue();

    double* getLocation() {
        return location;
    }
    void setLocation(const double * x);

    long double getProbability() {
        return prob;
    }

    void setProbability(long double value) {
        prob = value;
    }

    int getTrail() {
        return trail;
    }

    void initTrail() {
        trail = 0;
    }

    void increaseTrailVal() {
        trail++;
    }

    long double distanceTo(FoodSource * fsource);

    unsigned int getFailedAttempts() {
        return failedAttempts;
    }

    void resetFailedAttempts() {
        failedAttempts = 0;
    }

    void incrementFailedAttempts() {
        failedAttempts++;
    }

private:

    long double prob; // probability
    int trail;
    double * location;
    unsigned int failedAttempts;

    long double objectiveValue;
    Problem * p;
    Configuration * config;

};


#endif