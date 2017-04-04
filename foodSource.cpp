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


#include	"rng.h"
#include	"configuration.h"
#include	"problem.h"
#include	"foodSource.h"
#include	"chaoticmaps.h"

#include	<iostream>
#include	<math.h>
using namespace std;

FoodSource::FoodSource(Configuration * conf, Problem * problem) {

    p = problem;
    config = conf;

    location = new double[config->getProblemDimension()];
    reInit();

}

FoodSource::~FoodSource() {
    delete[] location;
}


// FoodSource::FoodSource(Configuration * conf, Problem * problem, unsigned int K){
//   
//   p = problem;  
//   config = conf;
//   
//   location = new double[config->getProblemDimension()];  
//   chaoticReInit(K);
// }
// 
// 

FoodSource::FoodSource(Configuration * conf, Problem * problem, FoodSource * bestFood) {

    p = problem;
    config = conf;

    location = new double[config->getProblemDimension()];

    double r = RNG::randVal(0.0, 1.0); // the greater the lower limit, the closer to "copy" the strategy becomes

    //Initialize location according to best food location
    for (unsigned int i = 0; i < config->getProblemDimension(); i++) {
        double randompos = RNG::randVal(config->getMinInitBound(), config->getMaxInitBound());
        const double best = bestFood->getLocation()[i];
        location[i] = randompos + r * (best - randompos);
        if (location[i] < config->getMinInitBound())
            location[i] = config->getMinInitBound();
        if (location[i] > config->getMaxInitBound())
            location[i] = config->getMaxInitBound();
    }

    //Initialization
    calculateObjectiveValue();
    trail = prob = 0;

}


FoodSource::FoodSource(FoodSource * copy){
  
  p = copy->p;  
  config = copy->config;
  
  location = new double[config->getProblemDimension()];  
  
  //Initialize location according to best food location
  for(unsigned int i=0;i<config->getProblemDimension();i++){
	  const double best = copy->getLocation()[i];
	  location[i] = best;
  }
  
  //Initialization
  objectiveValue = copy->getObjectiveValue();
  trail = prob = 0;

}

// 
// 
// 
// FoodSource::FoodSource(Configuration * conf, Problem * problem, double * loc){
// 
//   p = problem;  
//   config = conf;
//   
//   location = new double[config->getProblemDimension()];  
//   inverseReInit(loc);
// 
// }
// 
// FoodSource::~FoodSource(){
//     delete[] location;
// }

void FoodSource::reInit() {

    //Initialize food source position 
    for (unsigned int i = 0; i < config->getProblemDimension(); i++) {
        location[i] = RNG::randVal(config->getMinInitBound(), config->getMaxInitBound());
    }

    //Initialization
    calculateObjectiveValue();
    trail = prob = 0;
    resetFailedAttempts();

}


// void FoodSource::reInit(const double * bestSoFarSolution){
//   
//   double r = RNG::randVal(0.0,1.0); // the greater the lower limit, the closer to "copy" the strategy becomes
//   
//   //Initialize food source position 
//   for(unsigned int i=0;i<config->getProblemDimension();i++){
// 	  double randompos = RNG::randVal(config->getMinInitBound(),config->getMaxInitBound());
// 	  const double best = bestSoFarSolution[i];
// 	  location[i] =   randompos + r*(best - randompos);
// 	  
// 	  if(location[i] < config->getMinInitBound()) 
// 	    location[i] = config->getMinInitBound();
// 	  if(location[i] > config->getMaxInitBound()) 
// 	    location[i] = config->getMaxInitBound();
// 
//   }
//   
//   //Initialization
//   calculateObjectiveValue();
//   trail = prob = 0;
//   resetFailedAttempts();
// 
//   
// }
// 
// void FoodSource::reInit(double shake, const double * bestSoFarSolution){
//   
//   //Initialize food source position 
//   for(unsigned int i=0;i<config->getProblemDimension();i++){
// 	  double randompos = RNG::randVal(config->getMinInitBound(),config->getMaxInitBound());
// 	  const double best = bestSoFarSolution[i];
// 	  location[i] =   best + shake*(best - randompos);
// 	  
//   	  if(location[i] < config->getMinInitBound()) 
// 	    location[i] = config->getMinInitBound();
// 	  if(location[i] > config->getMaxInitBound()) 
// 	    location[i] = config->getMaxInitBound();
// 
//   }
//   
//   //Initialization
//   calculateObjectiveValue();
//   trail = prob = 0;
//   resetFailedAttempts();
//   
// }
// 
// 
//     
// void FoodSource::chaoticReInit(unsigned int K){
//   	double ch = RNG::randVal(0,1);
// 	double prev = RNG::randVal(0,1); 
//   //Initialize food source position 
//   for(unsigned int i=0;i<config->getProblemDimension();i++){
// 	
// 	double temp = prev;
// 	for (unsigned k = 0; k < K; k++){
// 	    switch(config->getChaoticMapID()){
// 	      case LOGISTIC:
// 		ch = ChaoticMaps::Logistic(ch);
// 		break;
// 	      case CIRCLE:
// 		ch = ChaoticMaps::Circle(ch);
// 		break;
// 	      case GAUSS:
// 		ch = ChaoticMaps::Gauss(ch);
// 		break;
// 	      case SINUSOIDAL:
// 		ch = ChaoticMaps::Sinusoidal(ch);
// 		break;
// 	      case SINUS:
// 		ch = ChaoticMaps::Sinus(ch);
// 		break;
// 	      case TEND:
// 		ch = ChaoticMaps::Tent(ch);
// 		break;
// 	      case HENON:
// 		prev=ch;
// 		ch = ChaoticMaps::Henon(ch,temp);
// 		break;
// 	      case ACFUNC:
// 		ch = ChaoticMaps::ACFunc(ch,config->getMu());
// 		break;
// 	    }
// 	}
// 	location[i]=config->getMinInitBound() + ch * (config->getMaxInitBound() - config->getMinInitBound());
// 	
// 	/*if generated parameter value is out of boundaries, it is shifted onto the boundaries*/ 
// 	if (location[i] < config->getMinInitBound()) 
// 	    location[i] = config->getMinInitBound(); 
// 	if (location[i] > config->getMaxInitBound()) 
// 	    location[i] = config->getMaxInitBound(); 
// 	 
//   }
//    
//   //Initialization
//   calculateObjectiveValue();
//   trail = prob = 0;
//   resetFailedAttempts();
//   
//   if(isnan(objectiveValue)){
// 	for(unsigned int i=0;i<config->getProblemDimension();i++){
// 	//  cout<<"  loc "<<i<<": "<<location[i];
// 	}
// 	cout<<endl;
//    exit(0);
//   }
//   
// } 
// 
// 
// void FoodSource::inverseReInit(double * loc){
//   
//   //Initialize food source position 
//   for(unsigned int i=0;i<config->getProblemDimension();i++){
// 	location[i]=config->getMinInitBound() + (config->getMaxInitBound() - loc[i]);
//   }
//    
//   //Initialization
//   calculateObjectiveValue();
//   trail = prob = 0;
//   resetFailedAttempts();
//   
// } 
//  

void FoodSource::calculateObjectiveValue() {
    // evaluate objective function
    objectiveValue = p->getFunctionValue(location);

}

void FoodSource::setLocation(const double * x) {

    for (unsigned int i = 0; i < config->getProblemDimension(); i++)
        location[i] = x[i];

}

/**
 * Mevcut yiyecek kaynağı sınıfı bu metodu çağırarak, parametre olarak göderilen
 * yiyecek kaynağına olan uzaklığı hesaplanmaktadır. öklid hesabı yapmaktır. 
 * 
 * @param fsource uzaklık hesabı yağılacak olan yiyecek kaynağı
 * @return uzaklık değeri
 */
long double FoodSource::distanceTo(FoodSource * fsource) {

    double * foodloc = fsource->getLocation();

    long double dist = 0.0;

    // (x - y)^2 bunun kökü alınırak uzaklık hessabı yapılıyor. 
    // öklid hesabı yapılıyor.
    for (unsigned int i = 0; i < config->getProblemDimension(); i++) {
        dist += (location[i] - foodloc[i]) * (location[i] - foodloc[i]);
    }

    return sqrt(dist);

}
