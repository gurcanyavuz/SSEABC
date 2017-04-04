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

#include "rng.h"
#include "icmaesils.h"
#include "configuration.h"
#include "problem.h"

#include <string>
//#include <gsl/gsl_rng.h>
//#include <gsl/gsl_randist.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <gsl/gsl_math.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include <math.h>
#include <gsl/gsl_statistics.h>



#include <cassert>
#include <cmath>
#include <limits>
#include <cstdio>
#include <cstdlib>
#include <string.h>
#include <float.h>
#include <stdio.h>

#include <math.h>
#include <malloc.h>
//Timers
#include <sys/time.h>
#include <sys/resource.h>

extern "C" {
#include "cmaes_interface.h"
extern void   random_init( random_t *, long unsigned seed );
extern void   random_exit( random_t *);
extern double random_Gauss( random_t *);
extern double random_Uniform(random_t *);
double const * cmaes_SetMean(cmaes_t *, const double *xmean);
}

double tunec;

using namespace std;


ICMAESLS::ICMAESLS(Configuration* config, Problem* problem): LocalSearch(config, problem){
    tunea=config->gettunea();
    tuneb=config->gettuneb();
    tunec=config->gettunec();
    tuned=config->gettuned();
    tunee=config->gettunee();
    tunef=config->gettunef();
    tuneg=config->gettuneg();
    globleseed=config->getRNGSeed();
    lambda = 0;
    
    stddev= new double[di];
    for (int j = 0; j < di; j++) {
	stddev[j] = tunec*(config->getMaxInitBound()-config->getMinInitBound());
    }

}

ICMAESLS::~ICMAESLS(void) {
  //cout<<"cmaes silindi"<<endl;
  delete[] stddev;
  
}


bool ICMAESLS::finished(){
    if(problem->getFunctionEvaluations() >= config->getMaxFES()||problem->getBestSolutionValue()<=EPSILON)
                return true;
        return false;
}

void ICMAESLS::apply(FoodSource * f, double stepSize, unsigned int itr)
{
  
    for(int i = 0; i < di; i++){
	      bestPos[i]=f->getLocation()[i];
    }
    
    bestVal = icmaes(itr, bestPos);
    //cout<<"BEST VAL : "<<bestVal<<"  FVAL: "<<f->getObjectiveValue()<<endl;
    
    if(f->getObjectiveValue() > bestVal){
	f->setObjectiveValue(bestVal);
	f->setLocation(getBestPosition());
	f->initTrail();
    }

}

long double ICMAESLS::icmaes (unsigned int maxfevalsforicmaes, double* feedback)
{
	  cmaes_t evo;
	  double *fitvals;
	  double fbestever=0;
	  double *xbestever=NULL;
	  double fmean;
	  int i, irun, countevals = 0;
	  int maxevals;
	  char const * stop;


	  long double long_fitness;
	  double xinit[di];

	  maxevals=maxfevalsforicmaes;
	  int countmaxevalsforicmaes;

	  countmaxevalsforicmaes=maxevals+problem->getFunctionEvaluations();
	  
	  for (irun = 0; (problem->getFunctionEvaluations() < countmaxevalsforicmaes) && (!finished()); ++irun)
	  {
	      if(irun == 0)
	      {
		  
	    	  for (int j = 0; j < di; j++) {
				  xinit[j]=feedback[j];
			  }
		  }
	      else
	      {
		
	    	  for (int j = 0; j < di; j++) {
	    	  	 xinit[j]=RNG::randVal(config->getMinInitBound(),config->getMaxInitBound());
	    	  }
	      }
	   //   for (int j = 0; j < di; j++) {
	//	  stddev[j] = tunec*(config->getMaxInitBound()-config->getMinInitBound());
	 //     }

	      if(problem->getFunctionEvaluations() < countmaxevalsforicmaes)
	      {
	         problem->getFunctionValue(xinit);
	      }
	      else
	    	  break;
	      


	      string filename = "initials.par";
		  fitvals = cmaes_init(&evo, config->getProblemDimension(), xinit, stddev, 0, lambda, filename.c_str());
	      lambda = (int) cmaes_Get(&evo, "lambda");
	      setbuf(stdout, NULL);
	      
	     
	     while((!(stop=cmaes_TestForTermination(&evo))) && (problem->getFunctionEvaluations() < countmaxevalsforicmaes) && (!finished()))
	     {
			  pop = cmaes_SamplePopulation(&evo);
			  popsize = cmaes_Get(&evo, "popsize");
			  
			  for (i = 0; i < cmaes_Get(&evo, "popsize"); ++i) {


					 inbound(config->getMinInitRange(), config->getMaxInitRange(), pop[i], di);


					 if(problem->getFunctionEvaluations() < countmaxevalsforicmaes)
					 {
					    long_fitness=problem->getFunctionValue(pop[i]);
					 }
					 else
					 {
						break;
					 }

					 if (long_fitness <= LDBL_MAX ) {
						  fitvals[i] = long_fitness;
					 }
					 else {
							fitvals[i] = LDBL_MAX;
					 }

		       }

			  cmaes_UpdateDistribution(&evo, fitvals);
			  fflush(stdout);
		}

		lambda = tuned * cmaes_Get(&evo, "lambda");
		// cec13 competition
		if (lambda>200)
		{
		  lambda=200;
		}
        cmaes_exit(&evo);
	}


    for (int j = 0; j < di; j++)
    {
    	bestPos[j]=problem->getBestSoFarSolution()[j];
    }
    return problem->getBestSolutionValue();

}

bool ICMAESLS::inbound(long double min, long double max, double *ind, int num) {
	  bool check = true;
	  int i;
	  for (i = 0; i < num ; i++) {
		  if (ind[i] > max)
		  {
				 ind[i] = max;

		  }
		  else if (ind[i] < min)
		  {
			 ind[i] = min;

		  }
	  }
	  return check;
}
