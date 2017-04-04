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


#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <vector>
#include <cmath>

//CEC2005 problems
#define SHIFTED_SPHERE_CEC05				0	
#define SHIFTED_SCHWEFEL12_CEC05			1
#define SHIFTED_ROTATED_HIGH_CONDITIONED_ELLIPTIC_CEC05	2
#define NOISE_SHIFTED_SCHWEFEL12_CEC05			3
#define SCHWEFEL26_GLOBAL_OPTIMUM_ON_BOUNDS_CEC05	4
#define SHIFTED_ROSENBROCK_CEC05			5
#define	SHIFTED_ROTATED_GRIEWANK_CEC05			6
#define	SHIFTED_ROTATED_ACKLEY_GOOB_CEC05		7
#define	SHIFTED_RASTRIGIN_CEC05				8
#define	SHIFTED_ROTATED_RASTRIGIN_CEC05			9
#define	SHIFTED_ROTATED_WEIERSTRASS_CEC05		10
#define	BASIC_SCHWEFEL213_CEC05				11
#define	SHIFTED_EXPANDED_GRIEWANKROSENBROCK_CEC05	12
#define	SHIFTED_ROTATED_EXPANDED_SCHAFFERF6_CEC05	13
#define	BASIC_HYBRIDCOMPOSITION1			14
#define	ROTATED_HYBRIDCOMPOSITION1			15
#define	NOISE_ROTATED_HYBRIDCOMPOSITION1		16
#define	ROTATED_HYBRIDCOMPOSITION2			17
#define	ROTATED_HYBRIDCOMPOSITION2_NBGO			18
#define	ROTATED_HYBRIDCOMPOSITION2_GOOB			19
#define	ROTATED_HYBRIDCOMPOSITION3			20
#define	ROTATED_HYBRIDCOMPOSITION3_HCNM			21
#define	NONCONTINUOUS_ROTATED_HYBRIDCOMPOSITION3	22
#define	ROTATED_HYBRIDCOMPOSITION4			23
#define	ROTATED_HYBRIDCOMPOSITION4_NO_BOUNDS		24


//CEC'14 problems
#define	R_HC_ELLIPTIC               0
#define ROTATED_BENT_CIGER          1
#define ROTATED_DISCUS              2
#define	SHIFTED_ROTATED_ROSENBROCK  3
#define SHIFTED_ROTATED_ACKLEY      4
#define	SHIFTED_ROTATED_WEIERSTRASS 5
#define SHIFTED_ROTATED_GRIEWANK    6
#define SHIFTED_RASTRIGIN           7
#define	SHIFTED_ROTATED_RASTRIGIN   8
#define	SHIFTED_SCHWEFEL            9
#define SHIFTED_ROTATED_SCHWEFEL    10
#define SHIFTED_ROTATED_KATSUURA    11
#define	SHIFTED_ROTATED_HAPPYCAT    12
#define	SHIFTED_ROTATED_HGBAT       13
#define	SHIFTED__ROTATED_EXPANDED_GRIEWANKROSENBROCK 14
#define	SHIFTED_ROTATED_EXPANDED_SCHAFFERF6          15
#define HYBRID_1                    16
#define HYBRID_2                    17
#define HYBRID_3                    18
#define HYBRID_4                    19
#define HYBRID_5                    20
#define HYBRID_6                    21
#define COMPOSITION_1               22
#define COMPOSITION_2               23
#define COMPOSITION_3               24
#define COMPOSITION_4               25
#define COMPOSITION_5               26
#define COMPOSITION_6               27
#define COMPOSITION_7               28
#define COMPOSITION_8               29

//local search algorithms
#define 	NO_LOCAL_SEARCH		0
#define     	MTSLS1          	1
#define		POWELL			2
#define		CMAES			3
#define		ADAPTIVE_LS		4

//types of Local Search Invocation Strategies
#define		DEFAULT_LS_STRATEGY	1
#define		ILS_STRATEGY		2

//competitions
#define CEC05	 	0
#define CEC08		1
#define SOFT_COMPUTING 	2
#define MIXTURE		3
#define CEC14           4

#define PI acos(-1)

class Configuration {
private:

    //general parameters
    unsigned long rngSeed;
    unsigned int maxFES;

    //problem parameters
    unsigned int competitionID;
    unsigned int problemID;
    unsigned int problemDimension;
    double minInitRange;
    double maxInitRange;

    //---------------------------------------------ABC PARAMETERS-------------------------------------------------
    // tum abc çeşitleri için ortak parametreler
    unsigned int numOfFoods; //yiyecek kaynağı (popülasyon) sayısı
    double limitF; // limit faktörü (limit=limitF*numOfFoods*problemDimension))

    // YEREL ARAMA ILE ILGILI PARAMETRELER
    unsigned int lsIterations; // yerel arama algoritması iterasyon sayısı
    double fTol; // FTOL parameteresi yerel arama algoritması Powell secildiğinde kullanilir. Hata esik degeri
    unsigned int lsID; // Hangi yerel arama algoritmasi kullanilacak. 0 = yerel arama kullanma
    // 	
    unsigned int stepSizeType; // yerel arama algoritmalara ozgu adim buyuklugu parametresi. Tarıyacagi alanin genisligini belirler
    unsigned int maxFailures; // yerel arama ayni yiyecek kaynaginda defalarca basarisiz olabilir. Bu parametre bunun limit degerini
    // 				  // belirler
    unsigned int lsStrategy; // yerel aramanin nasil uygulanacaginin stratejisi belirlenir

    unsigned int lsEvals;
   
    double tr;

  
    
    //CMAES parametreleri
    double ttunea;
    double ttuneb;
    double ttunec;
    double ttuned;
    double ttunee;
    double ttunef;
    double ttuneg;

    //POPULASYON SAYISININ ARTIRILMASINA OZGU PARAMETRELER
    unsigned int maxFoodSize; // artacaksa kaca kadar artacak
    unsigned int growthPeriod; // kac iterasyonda bir artacak

    //Extra variables
    double startTime; // zamani tutmak ici
    unsigned int seArchiveSize;

public:

    Configuration(int argc, char** argv);
    ~Configuration();

    unsigned long getRNGSeed();
    unsigned int getMaxFES();

    unsigned int getCompetitionID();
    unsigned int getProblemID();
    unsigned int getProblemDimension();
    double getMinInitRange();
    double getMaxInitRange();

    double getMinInitBound();
    double getMaxInitBound();

    //paramaters for all ABC variants
    unsigned int numOfFoodSources();
    void setCurrentFoodSize(unsigned int newSize);

    unsigned int getLimit();

    unsigned int getLSIterations();
    unsigned int getLocalSearchID();


    double getFTol();

    unsigned int getMaxRepeatedFailures();

    unsigned int getLSStrategy();


    unsigned int getMaximumFoodSize();
    unsigned int getGrowthPeriod();

    unsigned int getArchiveSize() {
        return seArchiveSize;
    }
    void setStartTime(double stime);
    double getStartTime();

    void print();
    
      //CMAES parametreleri
    double gettunea();
    double gettuneb();
    double gettunec();
    double gettuned();
    double gettunee();
    double gettunef();
    double gettuneg();



    unsigned int getLSEvaluations() {
        return lsEvals;
    }

    double trainingRatio() {
        return tr;
    }

};

#endif
