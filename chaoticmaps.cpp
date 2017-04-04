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

#include	"chaoticmaps.h"
#include	"configuration.h"

#include	<cmath>

using namespace std;

double ChaoticMaps::Logistic(double val) {
    return 4 * val * (1 - val);
}

double ChaoticMaps::Circle(double val) {
    return val + 1.2 - fmod((0.5 / (2 * PI)) * sin(2 * PI * val), 1.0);
}

double ChaoticMaps::Gauss(double val) {
    if (val == 0) return 0;
    return 1 / val - floor(1 / val);
}

double ChaoticMaps::Henon(double val, double prev) {
    return 1 - 1.4 * val * val + 0.3 * prev;
}

double ChaoticMaps::Sinusoidal(double val) {
    return sin(PI * val);
}

double ChaoticMaps::Sinus(double val) {
    return 2.3 * pow(val, 2 * sin(PI * val));
}

double ChaoticMaps::Tent(double val) {
    if (val < 0.7)
        return val / 0.7;
    return 10 / 3 * val * (1 - val);
}

double ChaoticMaps::ACFunc(double val, double mu) {
    return mu * (1 - val);
}


