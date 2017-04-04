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


//Configuration
#include "configuration.h"

//Problems
#include "problem.h"

//Algorithms
#include "ABC.h" 
#include "originalABC.h" // original ABC algorithm 

//local Search Algorithms
#include "localSearch.h"
#include "mtsls1.h"
#include "powell.h"
#include "icmaesils.h"

//Timers
#include <sys/time.h>
#include <sys/resource.h>

//C++ Libraries
#include <iostream>


using namespace std;

/**
 * local search seçimi yapılıyor.
 * @param config
 * @param p
 * @return 
 */
LocalSearch * initializeLocalSearch(Configuration * config, Problem * p) {
    LocalSearch * ls = NULL;
    switch (config->getLocalSearchID()) {
        case MTSLS1:
            ls = new Mtsls1(config, p);
            break;
        case POWELL:
            ls = new Powell(config, p);
            break;
        case CMAES:
            ls = new ICMAESLS(config, p);
            break;
    }

    return ls;
}

int main(int argc, char** argv) {

    double stime;
    static struct timeval tp;

    LocalSearch * ls;

    //Command line parsing
    Configuration* config = new Configuration(argc, argv);

    //Random number generator
    RNG::initializeRNG(config->getRNGSeed());

    //Initialize Problem
    Problem* problem = new Problem(config);

    //Initialize LocalSearch
    ls = initializeLocalSearch(config, problem);

    //Start timer
    gettimeofday(&tp, NULL);
    stime = (double) tp.tv_sec + (double) tp.tv_usec / 1000000.0;
    config->setStartTime(stime);

    //ABC intialization	
    ABC* abc = new OriginalABC(config, problem, ls);
    //    const clock_t begin_time = clock();
    //ABC execution
    abc->run();

    cout.precision(20);
    cout << "solution: " << scientific << problem->getBestSolutionValue() << endl;

    //    std::cout << "T2 : " << float( clock() - begin_time) / CLOCKS_PER_SEC;
    //Memory release
    RNG::deallocateRNG();
    RNG::deallocatePermutation();

    delete problem;
    delete abc;
    delete config;
    delete ls;

    //    const clock_t begin_time = clock();
    //
    //    double x = 0;
    //
    //    for (int i = 1; i < 1000000; i++) {
    //        x = 0.55 + (double) i;
    //
    //        x = x + x;
    //        x = x / 2;
    //        x = x*x;
    //
    //        x = sqrt(x);
    //        x = log(x);
    //        x = exp(x);
    //        x = x / (x + 2);
    //    }
    //    std::cout << "time " << float( clock() - begin_time) / CLOCKS_PER_SEC;

    return 0;
}
