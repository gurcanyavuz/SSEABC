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

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>

using namespace std;

class Utils {
public:
    static void loadRowVectorFromFile(string filename, int columns, double * row);
    static void loadRowVector(ifstream & fs, int columns, double * row);
    static void loadColumnVectorFromFile(string filename, int columns, double * column);
    static void loadColumnVector(ifstream & fs, int columns, double * column);
    static void loadMatrixFromFile(string filename, int N, int rows, int columns, double *** matrix3D);
    static void loadMatrixFromFile(string filename, int rows, int columns, double** matrix2D);
    static void loadMatrix(ifstream & fs, int rows, int columns, double ** matrix);

};
#endif