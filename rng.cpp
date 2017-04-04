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

#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

using namespace std;

gsl_rng* RNG::R = gsl_rng_alloc(gsl_rng_env_setup());
gsl_permutation* RNG::P = gsl_permutation_alloc(1);

void RNG::initializeRNG(unsigned long int rngSeed) {
    gsl_rng_set(RNG::R, rngSeed);
}

void RNG::initializePermutation(unsigned int size) {
    //Deallocate default
    gsl_permutation_free(RNG::P);

    //Allocate correct one	
    RNG::P = gsl_permutation_alloc(size);
    gsl_permutation_init(RNG::P);
}

double RNG::randVal(double min, double max) {
    return gsl_ran_flat(RNG::R, min, max);
}

double RNG::randGauss(double sd) {
    return gsl_ran_gaussian(RNG::R, sd);
}

void RNG::shufflePermutation() {
    gsl_ran_shuffle(RNG::R, RNG::P->data, gsl_permutation_size(RNG::P), sizeof (size_t));
}

int RNG::getPermutationElement(int index) {
    return gsl_permutation_get(RNG::P, index);
}

void RNG::deallocateRNG() {
    gsl_rng_free(RNG::R);
}

void RNG::deallocatePermutation() {
    gsl_permutation_free(RNG::P);
}
