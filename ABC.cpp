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


#include	"foodSource.h"
#include	"ABC.h"
#include	"configuration.h"
#include	"rng.h"
#include	"problem.h"
#include	"localSearch.h"

#include	<cfloat> 
#include	<iostream>

ABC::ABC(Configuration * conf, Problem * problem, LocalSearch * localS) {

    p = problem;
    config = conf;

    solutionValue = LDBL_MAX;
    ls = localS;

}

bool ABC::notDone() {
    //Check termination criterion
    if (p->getFunctionEvaluations() >= config->getMaxFES() || (p->getBestSolutionValue() <= EPSILON && config->getCompetitionID() != 1 && config->getProblemID() != 6))
        return false;
    return true;
}

ABC::~ABC() {
}

double* ABC::getSolution() {
    return solution;
} // en iyi cozumu getir

void ABC::setSolution(const double * x) { // en iyi cozumu guncelle
    for (int i = 0; i < config->getProblemDimension(); i++)
        solution[i] = x[i];
}

/**
 * en iyi yiyecek kaynagini getir
 */
FoodSource * ABC::getBestFSource() {

    int i, j;
    FoodSource * bestFood;
    solutionValue = LDBL_MAX;
    for (i = 0; i < foodSources.size(); i++) {
        if ((foodSources.at(i))->getObjectiveValue() < solutionValue) {
            solutionValue = (foodSources.at(i))->getObjectiveValue();
            setSolution((foodSources.at(i))->getLocation());
            bestFood = foodSources.at(i);
        }
    }

    return bestFood;
}

double ABC::getStepSize() {
    return getAdaptiveStepSize();
}

/*
 * En iyi yiyecek kaynaginin indeksini getir
 */
int ABC::getBestFSourceIndex() {

    int i, j;
    int bestFoodIndex;
    solutionValue = LDBL_MAX;
    for (i = 0; i < foodSources.size(); i++) {
        if ((foodSources.at(i))->getObjectiveValue() < solutionValue) {
            solutionValue = (foodSources.at(i))->getObjectiveValue();
            bestFoodIndex = i;
        }
    }

    return bestFoodIndex;

}

/*
 * yerel arama algoritmalarinin ne kadarlik bir alani tarayacaklari step degiskeni ile belirlenir. 
 * Bu genel olarak tüm arama uzayı olarak set edilir. Fakat adaptive step degiskeni kullanildiginda
 * daha iyi sonuclar vermektedir. Buradaki yontem IABC-LS de kullanilan yontemdir. Buna gore step
 * buyuklugu en iyi yiyecek kaynagi ile rasgele secilen bir yiyecek kaynagi arasinda bir birine en
 * uzak olan boyutlarin buyuklugu olarak belirlenmistir.
 */
double ABC::getAdaptiveStepSize() {

    int bestFoodIndex = getBestFSourceIndex();

    FoodSource * bestFood = foodSources.at(bestFoodIndex); // en iyi yiyecek kaynagini al

    FoodSource * reference;
    unsigned int foodID = -1;
    do {
        foodID = floor(RNG::randVal(0, foodSources.size()));
        reference = foodSources.at(foodID);
    } while (foodID == bestFoodIndex); // rastgele bir yiyecek kaynagi sec


    // determine the max vector distance between the best(or random) and reference food source
    double max = 0.0;
    for (int j = 0; j < config->getProblemDimension(); j++) {
        double temp = fabs(reference->getLocation()[j] - bestFood->getLocation()[j]);
        if (temp > max)
            max = temp;
    }
    // set step size as max vector distance 
    return max;

}

/*
 * yerel arama algoritmasinin en iyi yiyecek kaynagi ilk nokta alinarak uygulandigi yer
 * bestfood: en iyi yiyecek kaynagi. İki tur uygulama stratejisi vardir. Bunlardan birincisi
 * varsayilan yontem olan direk yerel arama algoritmasinin uygulanma yontemidir. Digeri
 * IABC-LS deki uygulama yontemidir.
 */
void ABC::applyLocalSearch(FoodSource * bestfood) {
    
    if (config->getLocalSearchID() != NO_LOCAL_SEARCH) {
        if (config->getLSStrategy() == DEFAULT_LS_STRATEGY)
            applyDefaultLSStrategy(bestfood);
        else if (config->getLSStrategy() == ILS_STRATEGY)
            applyILSStrategy(bestfood);
    }
}

void ABC::applyDefaultLSStrategy(FoodSource * bestfood) {
    ls->apply(bestfood, getStepSize(), config->getLSIterations());

}

/*
 * Bu yerel arama uygulama yonteminin mantigi soyledir: Yerel arama algoritmasi her zaman en iyi yiyecek
 * kaynagina uygulandigi icin yerel arama sonunda bir iyilesme olmaz ve algoritmalarin diger sureclerinde de
 * en iyi cozum degismezse yerel arama her zaman ayni noktaya uygulanacak ve basarisiz olacaktir. Bu basarizliklari
 * maxFailures degiskeni tutmaktadir. Eger maxFailures kadar bir denemede yerel algoritma basarisiz olursa bu durumda
 * yerel arama en iyi yiyecek kaynagina degil rasgele secilen bir yiyecek kaynagina uygulanmaktadir. 
 */
void ABC::applyILSStrategy(FoodSource * bestfood) {

    FoodSource * food = bestfood;

    int foodID = -1;
    int refFoodID = -1;

    //Check the best food source is in local optima
    if (food->getFailedAttempts() == config->getMaxRepeatedFailures()) {
        //if yes, apply local search to another (random) particle
        int fid = getBestFSourceIndex();
        do {
            foodID = floor(RNG::randVal(0, foodSources.size()));
        } while (foodID == fid && (foodSources.at(foodID))->getFailedAttempts() > 0);

        food = foodSources.at(foodID);
    }

    // select a reference food source for determining step size adaptively
    FoodSource * reference;

    do {
        refFoodID = floor(RNG::randVal(0, foodSources.size()));
    } while (foodID == refFoodID);

    reference = foodSources.at(refFoodID);


    // determine the max vector distance between the best(or random) and reference food source
    double max = 0.0;
    for (int j = 0; j < config->getProblemDimension(); j++) {
        double temp = fabs(reference->getLocation()[j] - food->getLocation()[j]);
        if (temp > max)
            max = temp;
    }


    long double before = food->getObjectiveValue();
    // apply local search
    ls->apply(food, max, config->getLSIterations());

    long double after = food->getObjectiveValue();

    //check the food source new location is better than the previous or not
    if (after < before)
        food->resetFailedAttempts();
    else
        food->incrementFailedAttempts();

}