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

#ifndef CHAOTIC_MAPS
#define CHAOTIC_MAPS

class ChaoticMaps {
public:

    static double Logistic(double val);
    static double Circle(double val);
    static double Gauss(double val);
    static double Henon(double val, double prev);
    static double Sinusoidal(double val);
    static double Sinus(double val);
    static double Tent(double val);
    static double ACFunc(double val, double mu);

};

#endif 