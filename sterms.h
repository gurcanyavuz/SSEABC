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

#ifndef STERMS
#define STERMS

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

class SearchEquation {
public:

    SearchEquation(int first, int second, int third, int forth, int MR) {
        mr = MR;
        fv = first;
        sv = second;
        tv = third;
        fov = forth;
        success = 0;
    }

    ~SearchEquation() {
    }

    int getFirstTerm() {
        return fv;
    }

    int getSecondTerm() {
        return sv;
    }

    int getThirdTerm() {
        return tv;
    }

    int getForthTerm() {
        return fov;
    }

    int getMR() {
        return mr;
    }

    void setFirstTerm(int val) {
        fv = val;
    }

    void setSecondTerm(int val) {
        sv = val;
    }

    void setThirdTerm(int val) {
        tv = val;
    }

    void setForthTerm(int val) {
        fov = val;
    }

    void setMR(int val) {
        mr = val;
    }

    double getNofSuccess() {
        return success;
    }

    void initSuccess(double w) {
        success *= w;
    }

    void increaseSuccess() {
        success = success + 1.0;
    }

    /**
     * üretilen denklemleri string  olarak geriye döndürmektedir.
     * 
     * @return 
     */
    string getEquationString() {
        std::stringstream ss;

        // ss << mr << "\t" << fv << "\t" << sv << "\t" << tv << "\t" << fov << "\t" <<success << "\n";
        // succes olmadan yazdırma
        ss << mr << "\t" << fv << "\t" << sv << "\t" << tv << "\t" << fov << "\t" << "\n";
        std::string text;

        text = ss.str();

        //std::cout << text;

        return text;
    }

private:
    int fv;
    int sv;
    int tv;
    int fov;
    int mr;
    double success;

};

#endif