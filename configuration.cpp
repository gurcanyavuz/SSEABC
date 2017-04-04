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

#include "configuration.h"
#include "rng.h"
#include <tclap/CmdLine.h>
#include <cfloat>
/***********************************************************************************************************************
 * Configuration sınıfı program için gerekli olan parametrelerin dışarıdan alınmasını ve bunların dışarıya sunulduğu
 * metodları içerir. 
 * *********************************************************************************************************************/
//TCLAP dışarıdan alınan parametrelerin belli bir formatta kolayca alınmasını sağlar
using namespace TCLAP;
using namespace std;

Configuration::Configuration(int argc, char** argv) {
    try {
        CmdLine cmd("Self Adaptive Artificial Bee Colony Algorithm", ' ', "1.0");

        //seed degeri: Gerekli
        ValueArg<unsigned long> seedArg("", "seed", "RNG seed", true, 0, "unsigned long"); //true parametresi gerekli oldugunu belirtir
        cmd.add(seedArg);

        //Max. fonksiyon cagirimi: Gerekli
        ValueArg<unsigned int> fesArg("", "fes", "Maximum number of function evaluations", true, 0, "unsigned int");
        cmd.add(fesArg);

        //Yarışma ID'si :Gerekli
        vector<unsigned int> allowedCompetitions;
        allowedCompetitions.push_back(CEC05);
        allowedCompetitions.push_back(CEC08);
        allowedCompetitions.push_back(SOFT_COMPUTING);
        allowedCompetitions.push_back(MIXTURE);
        allowedCompetitions.push_back(CEC14);
        ValuesConstraint<unsigned int> allowedCompetitionIDVals(allowedCompetitions);

        ValueArg<unsigned int> cIDArg("", "competitionID", "Competiton ID", true, 0, &allowedCompetitionIDVals);
        cmd.add(cIDArg);


        //problemID2 si: Gerekli
        vector<unsigned int> allowedProblems;
        for (int i = 0; i < 50; i++)
            allowedProblems.push_back(i);
        ValuesConstraint<unsigned int> allowedProblemIDVals(allowedProblems);

        ValueArg<unsigned int> pIDArg("", "problemID", "Problem ID", true, 0, &allowedProblemIDVals);
        cmd.add(pIDArg);


        //Problem boyutu: Gerekli
        ValueArg<unsigned int> dimArg("", "dimensions", "Dimensionality", true, 0, "unsigned int");
        cmd.add(dimArg);

        //Max. fonksiyon cagirimi: Gerekli
        ValueArg<unsigned int> seAArg("", "asize", "The size of the search equation archive", true, 0, "unsigned int");
        cmd.add(seAArg);


        //Problem aralığının minimum degeri: Gerekli 
        ValueArg<double> minRangeArg("", "minrange", "Minimum value of the search range", true, 0, "double");
        cmd.add(minRangeArg);

        //Problem aralığının maksimum degeri: Gerekli
        ValueArg<double> maxRangeArg("", "maxrange", "Maximum value of the search range", true, 0, "double");
        cmd.add(maxRangeArg);

        //Populasyon buyuklugu: Gerekli
        ValueArg<unsigned int> numFoodArg("", "nfoods", "Number of food sources", true, 1000, "unsigned int");
        cmd.add(numFoodArg);


        //limit faktoru: Gerekli
        ValueArg<double> limitFArg("", "limitF", "factor of limit", true, 1, "double");
        cmd.add(limitFArg);

        //Yerel arama iterasyon sayısı: Gerekli degil 
        ValueArg<unsigned int> lsItrArg("", "lsItr", "iteration number for Local Search procedures", false, 0, "unsigned int");
        cmd.add(lsItrArg);


        ValueArg<double> icmaesArg("","cmar","icmaes usage ratio",false,0,"double");
        cmd.add(icmaesArg);


        //Training Ratio: gerekli değil 
        ValueArg<double> trArg("","tr","Training ratio for local search procedures",false,0.0,"double");
        cmd.add(trArg);

        
        //Yerel arama hangisi: Gerekli 
        vector<unsigned int> allowedLS;
        allowedLS.push_back( NO_LOCAL_SEARCH );
        allowedLS.push_back( MTSLS1 );
        allowedLS.push_back( POWELL );
        allowedLS.push_back( CMAES );
        allowedLS.push_back( ADAPTIVE_LS );
        ValuesConstraint<unsigned int> allowedLSIDVals( allowedLS );

        ValueArg<unsigned int> lsIDArg("","lsID","local Search ID: 0 = no local search, 1 = MTSLS1, 2 = Powell's CDS, 3 = CMAES",true, 0,&allowedLSIDVals);
        cmd.add(lsIDArg);


        //Yerel arama stratejisi: Gerekli degil
        vector<unsigned int> allowedLSStrategies;
        allowedLSStrategies.push_back( DEFAULT_LS_STRATEGY );
        allowedLSStrategies.push_back( ILS_STRATEGY );
        ValuesConstraint<unsigned int> allowedLSStrVals( allowedLSStrategies );

        ValueArg<unsigned int> lsStrArg("","lsStrategy","local Search Invocation Strategy: 1 = default, 2 = Like as in ILS algorithms",false, 1,&allowedLSStrVals);
        cmd.add(lsStrArg);

        //Aynı noktaya maksimum yerel arama sayısı
        ValueArg<unsigned int> maxFaiArg("","maxfailures","Maximum number of failures of LS on a food source",false, 0,"unsigned int");
        cmd.add(maxFaiArg);			

        //fTol: Gerekli degil
        ValueArg<double> fTolArg("","ftol","Ftol value for Powell's CDS local Search",false,-1,"double");
        cmd.add(fTolArg);

        // Required parameters of gcmaes
        ValueArg<double> tuneaArg("","ttunea","ttunea",false,0,"double");
        cmd.add(tuneaArg);

        // Required parameters of gcmaes
        ValueArg<double> tunebArg("","ttuneb","ttuneb",false,0,"double");
        cmd.add(tunebArg);

        // Required parameters of gcmaes
        ValueArg<double> tunecArg("","ttunec","ttunec",false,0,"double");
        cmd.add(tunecArg);

        // Required parameters of gcmaes
        ValueArg<double> tunedArg("","ttuned","ttuned",false,0,"double");
        cmd.add(tunedArg);

        // Required parameters of gcmaes
        ValueArg<double> tuneeArg("","ttunee","ttunee",false,0,"double");
        cmd.add(tuneeArg);

        // Required parameters of gcmaes
        ValueArg<double> tunefArg("","ttunef","ttunef",false,0,"double");
        cmd.add(tunefArg);

        // Required parameters of gcmaes
        ValueArg<double> tunegArg("","ttuneg","ttuneg",false,0,"double");
        cmd.add(tunegArg);

 

        //Maksimum yiyecek sayısı (artırımlı versiyon için): Gerekli degil
        ValueArg<unsigned int> maxFoodArg("", "maxfoodsize", "Maximum food size", false, 1000, "unsigned int");
        cmd.add(maxFoodArg);

        //Kac iterasyonda bir populasyon artırım: Gerekli degil
        ValueArg<unsigned int> growthArg("", "growthperiod", "Growth period", false, 1, "unsigned int");
        cmd.add(growthArg);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        //Parses input
        cmd.parse(argc, argv);

        rngSeed = seedArg.getValue();
        maxFES = fesArg.getValue();
        competitionID = cIDArg.getValue();

        problemID = pIDArg.getValue();

        problemDimension = dimArg.getValue();
        minInitRange = minRangeArg.getValue();
        maxInitRange = maxRangeArg.getValue();

        numOfFoods = numFoodArg.getValue();
        limitF = limitFArg.getValue();
        seArchiveSize = seAArg.getValue();

        maxFoodSize = maxFoodArg.getValue();
        growthPeriod = growthArg.getValue();


        lsIterations = lsItrArg.getValue();

        fTol = fTolArg.getValue();

        lsID = lsIDArg.getValue();
        maxFailures = maxFaiArg.getValue();
        lsStrategy = lsStrArg.getValue();
        
        
        ttunea= tuneaArg.getValue();
        ttuneb= tunebArg.getValue();
        ttunec= tunecArg.getValue();
        ttuned= tunedArg.getValue();
        ttunee= tuneeArg.getValue();
        ttunef= tunefArg.getValue();
        ttuneg= tunegArg.getValue();
        
        lsEvals = icmaesArg.getValue()*maxFES;
	tr = trArg.getValue();
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////			

        if (competitionID == CEC08 && (problemID > 6 || problemID < 0)) {
            cout << "There is no problem available more than 7 in CEC08 competition" << endl;
            exit(0);
        }

        if (competitionID == CEC05 && (problemID > 24 || problemID < 0)) {
            cout << "There is no problem available more than 25 in CEC05 competition" << endl;
            exit(0);
        }


        if (competitionID == CEC08 && (problemID > 18 || problemID < 0)) {
            cout << "There is no problem available more than 19 in SOCO competition" << endl;
            exit(0);
        }

        if (competitionID == CEC14 && (problemID > 30 || problemID < 1)) {
            cout << "There is no problem available more than 30 in CEC2014 competition" << endl;
            exit(0);
        }


    } catch (TCLAP::ArgException &e) // catch any exceptions
    {
        std::cerr << "error: " << e.error() << " for arg " << e.argId() << std::endl;
    }
}

Configuration::~Configuration() {
}

unsigned long Configuration::getRNGSeed() {
    return rngSeed;
}

unsigned int Configuration::getMaxFES() {
    return maxFES;
}

unsigned int Configuration::getCompetitionID() {
    return competitionID;
}

unsigned int Configuration::getProblemID() {
    return problemID;
}

unsigned int Configuration::getProblemDimension() {
    return problemDimension;
}

double Configuration::getMinInitRange() {
    if ((getProblemID() == 6 || getProblemID() == 24) && (getCompetitionID() == 0))
        return LDBL_MAX*-1.0;

    return minInitRange;
}

double Configuration::getMaxInitRange() {
    if ((getProblemID() == 6 || getProblemID() == 24) && (getCompetitionID() == 0))
        return LDBL_MAX;

    return maxInitRange;
}

double Configuration::getMinInitBound() {
    return minInitRange;
}

double Configuration::getMaxInitBound() {
    return maxInitRange;
}

unsigned int Configuration::numOfFoodSources() {
    return numOfFoods;
}
// changed by tianjun

unsigned int Configuration::getLimit() {
    return (int) (limitF * numOfFoods * problemDimension);
}

void Configuration::setStartTime(double stime) {
    startTime = stime;
}

double Configuration::getStartTime() {
    return startTime;
}

unsigned int Configuration::getLSIterations() {
    return lsIterations;
}

double Configuration::getFTol() {
    return fTol;
}

unsigned int Configuration::getLocalSearchID() {
    return lsID;
}

void Configuration::setCurrentFoodSize(unsigned int newSize) {
    numOfFoods = newSize;
}

unsigned int Configuration::getMaxRepeatedFailures() {
    return maxFailures;
}

unsigned int Configuration::getLSStrategy() {
    return lsStrategy;
}

unsigned int Configuration::getMaximumFoodSize() {
    return maxFoodSize;
}

unsigned int Configuration::getGrowthPeriod() {
    return growthPeriod;
}


double Configuration::gettunea() {
    return ttunea;
}

double Configuration::gettuneb() {
    return ttuneb;
}

double Configuration::gettunec() {
    return ttunec;
}

double Configuration::gettuned() {
    return ttuned;
}

double Configuration::gettunee() {
    return ttunee;
}

double Configuration::gettunef() {
    return ttunef;
}

double Configuration::gettuneg() {
    return ttuneg;
}

void Configuration::print() {
    cout << "Parameters:" << endl;
    cout << endl;
    cout << "rngSeed = " << rngSeed << endl;
    cout << "maxFES = " << maxFES << endl;
    cout << "problemID = " << problemID << endl;
    cout << "problemDimension = " << problemDimension << endl;
    cout << "minInitRange = " << minInitRange << endl;
    cout << "maxInitRange = " << maxInitRange << endl;
    cout << "numberOfFoodSources = " << numOfFoods << endl;
}


