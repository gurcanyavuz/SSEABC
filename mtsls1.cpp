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

#include "mtsls1.h"
#include "rng.h"

#include <cfloat>
#include <vector>
#include <iostream>

using namespace std;

Mtsls1::Mtsls1(Configuration * conf, Problem * prob) : LocalSearch(conf, prob) {
    improve = true;
    grade = 0;
    bonus1 = 10;
    bonus2 = 1;
}


bool Mtsls1::finished(){
    if(problem->getFunctionEvaluations() >= config->getMaxFES()||problem->getBestSolutionValue()<=EPSILON)
                return true;
        return false;
}

int Mtsls1::run(double * xk, double s, unsigned int maxitr){
    double inits = s;
    int iter=0;
    vector<int> conbef;
    bool convergence = false;
    
    unsigned int countmaxevalsformtsls1 = maxitr + problem->getFunctionEvaluations();
    
    do
    {

      if(improve == false)
      {

	  s=s/RNG::randVal(1.5,2.5);
	  if(s<1e-20)
	  {

	    conbef.push_back(problem->getBestSolutionValue());
	    if(conbef.size()>=2)
	    {
		    if(abs(conbef.back()-conbef[conbef.size()-1])<1e-20)
		    {
			    convergence=true;
		    }

	    }

	    s=RNG::randVal(0.7,0.9)* inits;//(config->getMaxInitRange()-config->getMinInitRange());
	    //s=0.4*(config->getMaxInitRange()-config->getMinInitRange());
	  }
      }


      improve=false;
      
      for(int i = 0; i < config->getProblemDimension(); i++)
      {
	      long double before1 = evaluateFunction(xk);
	      
	      xk[i]=xk[i]-s;

	      long double after1 = evaluateFunction(xk);
	      // compared to the current best solution

	      if(after1 < problem->getBestSolutionValue())
	      {
		      grade=grade+bonus1;

	      }

	      if(abs(after1-before1)<=1e-20)
	      {
		      xk[i]=xk[i]+s;
	      }
	      else
	      {
		      if(after1-before1>1e-20)
		      {
			      xk[i]=xk[i]+s;
			      long double before2=before1;
			      xk[i]=xk[i]+0.5*s;

			      long double after2 = evaluateFunction(xk);

			      if(abs(after2 - problem->getBestSolutionValue()) > 1e-20 && after2 < problem->getBestSolutionValue())
			      {
				      grade=grade+bonus1;
			      }

			      if(after2>=before2||(0<before2-after2<=1e-20))
			      {
				      xk[i]=xk[i]-0.5*s;
			      }
			      else
			      {
				      grade=grade+bonus2;
				      improve=true;
			      }


		      }
		      else
		      {
			      grade=grade+bonus2;
			      improve=true;
		      }

	      }
      }
      iter++;
    }while((problem->getFunctionEvaluations() < countmaxevalsformtsls1) && (!finished()) && (iter < config->getLSIterations()) && convergence == false);


    return grade;

}

void Mtsls1::apply(FoodSource * f, double stepSize, unsigned int itr) {
   // cout << "HERE" << endl;
    const double* pos = f->getLocation();
    double x[config->getProblemDimension()];

    // create a temporary position vector for the particle position
    for (int i = 0; i < config->getProblemDimension(); i++)
        x[i] = pos[i];

    int ret = run(x, stepSize, itr);

    //update best position and value of the particle
    if (f->getObjectiveValue() > getBestValue()) {
        f->setObjectiveValue(getBestValue());
        f->setLocation(getBestPosition());
        f->initTrail();
    }

    if (ret < 0) {
        cout << "Error" << endl;
        exit(1);
    }

}



