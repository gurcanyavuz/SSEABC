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

#include "utils.h"

//#include <iostream>
#include <string>
#include <sstream>
#include <fstream>


using namespace std;

void Utils::loadRowVector(ifstream & fs, int columns, double * row) {

    string aline;
    getline(fs, aline);
    istringstream lineStream(aline);

    for (int i = 0; i < columns; i++) {
        lineStream >> row[i];
    }

}

void Utils::loadRowVectorFromFile(string filename, int columns, double * row) {
    ifstream file(filename.c_str());
    loadColumnVector(file, columns, row);
    file.close();
}

void Utils::loadColumnVectorFromFile(string filename, int rows, double * column) {
    ifstream file(filename.c_str());
    loadColumnVector(file, rows, column);
    file.close();
}

void Utils::loadColumnVector(ifstream & fs, int rows, double * column) {
    string aline;
    getline(fs, aline);
    istringstream lineStream(aline);

    for (int i = 0; i < rows; i++) {
        lineStream >> column[i];
    }
}

void Utils::loadMatrixFromFile(string filename, int N, int rows, int columns, double *** matrix3D) {
    ifstream file(filename.c_str());
    for (int i = 0; i < N; i++) {
        loadMatrix(file, rows, columns, matrix3D[i]);
    }
    file.close();
}

void Utils::loadMatrixFromFile(string filename, int rows, int columns, double** matrix2D) {
    ifstream file(filename.c_str());
    loadMatrix(file, rows, columns, matrix2D);
    file.close();
}

void Utils::loadMatrix(ifstream & fs, int rows, int columns, double ** matrix) {
    for (int i = 0; i < rows; i++) {
        loadRowVector(fs, columns, matrix[i]);
    }
}
