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

#ifndef ORIGINAL_ABC_H
#define ORIGINAL_ABC_H

#include	"configuration.h"
#include	"problem.h"
#include	"foodSource.h"
#include	"ABC.h"
#include	"localSearch.h"
#include	"sterms.h"
#include	"rng.h"

#include <vector>
#include <cstdlib>
#include <cmath>

using namespace std;

class OriginalABC : public ABC {
public:
    OriginalABC(Configuration * conf, Problem * problem, LocalSearch * ls);
    ~OriginalABC();
    void initFoodSources();
    void sendEmployedBees();
    void sendOnlookerBees();
    void sendScoutBees();
    void calculateProbabilities();
    void run();
private:
    bool nols;
    vector<SearchEquation*> searchEquations;
    int c; // archieve counter;
    void eliminateWorstCandidateSE();
    SearchEquation * createSE();
    //initialization strategies
    void initFoodSourcesDefault();
    //void initFoodSourcesChaotic();
    //void initFoodSourcesOpposition();

    void initializeSEArchive();
    void reinitializeSEArchieve();
    //strategies for employed bees and onlooker bees
    FoodSource * getSuitableReferencefor(FoodSource * fsource, unsigned int fID);
    double getFirstTerm(double xc, double gb, double r, unsigned int str);
    double getOtherTerms(double xc, double gb, double gbd, double r1, double r2, unsigned int str);
    //double getThirdTermEmp(int index, int p2c);
    //double getFourthTermEmp(int index, int p2c);

    //double getFirstTermOnl(int index, int p2c);
    //double getSecondTermOnl(int index, int p2c);
    //double getThirdTermOnl(int index, int p2c);
    //double getFourthTermOnl(int index, int p2c);

    //probability calculation strategies
    void calculateProbabilitiesDefault();
    //void calculateProbabilitiesWeighted();
    //void calculateProbabilitiesOrdered();
    //int * getOrder();

    //scout bees strategies
    void sendScoutBeesDefault();
    //void sendScoutBeesBABC();
    //void sendScoutBeesCABC();
    //void sendScoutBeesIABC();
    //void sendScoutBeesERABC();

    //void chaoticSearchCABC(FoodSource * fsource);
    //void chaoticSearchERABC(FoodSource * fsource);
    //void reinitFSourceBABC(FoodSource * fsource);
    void adaptivelySelectLS(FoodSource* bestf);
};


#endif