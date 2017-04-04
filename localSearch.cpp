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

#include <iostream>
#include <cfloat>


#include "localSearch.h"
#include "foodSource.h"
#include "rng.h"


using namespace std;

LocalSearch::LocalSearch( Configuration* config, Problem* problem)
{
  
    this->config = config;
    this->problem = problem;
    di = config->getProblemDimension();
    bestPos = new double[di];
    bestVal = LDBL_MAX;
}

LocalSearch::~LocalSearch(){
    delete [] bestPos;
}

long double LocalSearch::evaluateFunction(double * x){
      	long double result; int i;

	if(problem->getFunctionEvaluations() < config->getMaxFES()){
	//	if(problem->hasBoundConstraint()){
		    result = problem->evaluate(di, x) + addPenalty(x);
	//	}
	//	else 
	//	    result = problem->evaluate(di, x);
		
		problem->setFunctionEvaluations(problem->getFunctionEvaluations()+1);
		
		//Update and print best solution value
		if( problem->getBestSolutionValue() > result){
			problem->setBestSolutionValue(result);
			problem->setBestSoFarSolution(x);
			problem->printProgress();
		}
		
		if(bestVal > result){
			bestVal = result;
			setBestPosition(x);
		}
	
		return result;
	}
	else
		return problem->getBestSolutionValue();
	

}

void LocalSearch::setBestPosition(const double * x){
    for(int i = 0; i < di; i++)
      bestPos[i] = x[i]; 

}

double * LocalSearch::getBestPosition(){ return bestPos; }

long double LocalSearch::getBestValue(){ return bestVal; }

long double LocalSearch::addPenalty(const double * x){

	  int i;  
	  long double penalty = 0.0;
	  
	  double minr = config->getMinInitRange();
	  double maxr = config->getMaxInitRange();
	  
	  for(i = 0; i < di; i++){
	    if(x[i] < minr)
	      penalty+= fabs(x[i] - minr) * fabs(x[i] - minr);
	    if(x[i] > maxr)
	      penalty+= fabs(x[i] - maxr) * fabs(x[i] - maxr);
	  }
	  
	  return penalty * problem->getFunctionEvaluations();
}

