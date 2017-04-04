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


#include "foodSource.h"
#include "ABC.h"
#include "originalABC.h"
#include "configuration.h"
#include "rng.h"
#include "problem.h"
#include "localSearch.h"
#include "chaoticmaps.h" 
#include "sterms.h"
#include "icmaesils.h"
#include "mtsls1.h"
#include <cfloat> 
#include <iostream>
#include <fstream>

/**********************************************************************************************************************
 * UABC algoritmasının bulundugu sınıf
 * ********************************************************************************************************************/

//////////////////////////////////////////// CONSTRUCTOR AND DESTRUCTOR ////////////////////////////////////////////////

/**********************************************************************************************************
 * parametreler
 * ------------
 * conf: konfigurasyon nesnesi 
 * problem: problem nesnesi 
 * ls: yerel arama algoritmasi nesnesi
 * 
 * Yapılandırıcı içerisinde yiyecek kaynakları ilkleniyor ve her yiyecek kaynagina ait bilgiler ekleniyor 
 * ********************************************************************************************************/
OriginalABC::OriginalABC(Configuration * conf, Problem * problem, LocalSearch * ls) : ABC(conf, problem, ls) {

    initFoodSources();
    tempSource = new double[conf->getProblemDimension()];
    solution = new double[conf->getProblemDimension()];

    initializeSEArchive();
    nols = false;
}

// destructor

OriginalABC::~OriginalABC() {

    delete [] tempSource;
    delete [] solution;

    int popsize = foodSources.size();
    for (unsigned int i = 0; i < popsize; i++)
        delete foodSources.at(i);
    foodSources.clear();

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////// INITIALIZATION STRATEGIES //////////////////////////////////////////////////

/*******************************************************************************************************************
 * yiyecek kaynaklarini olusturur. DEFAULT_INIT secilirse varsayılan sekilde rasgele dagilir. 
 * CHAOTIC_INIT secilirse kaotik rassal üreteclere gore yerlerine konumlandirilir.
 * Ayrıca istendiğinde opposition based ilkleme de olabilir.
 * 
 * Bu ilkleme mekanizmasi ile literatürdeki 4 ilkleme yöntemi birden elde edilmistir.
 * *****************************************************************************************************************/
void OriginalABC::initFoodSources() {

    initFoodSourcesDefault();
}

/******************************************************************************************************************
 * varsayilan ilkleme yapiliyor. Yiyecekler olusturularak foodSources adindaki vektore ekleniyor 
 * 
 * ***************************************************************************************************************/
void OriginalABC::initFoodSourcesDefault() {
    for (int i = 0; i < config->numOfFoodSources(); i++) {
        FoodSource * newFoodSource = new FoodSource(config, p);
        foodSources.push_back(newFoodSource);
    }
}

/**
 * Bir Searchequation sınıfı oluşturulmakta. Oluşturulurken random olarak 
 * termler belirlenmektedir. 
 * 
 * @return Geriye random oluşturulmuş bir searchEquation döndürülüyor
 */
SearchEquation * OriginalABC::createSE() {

    int first = floor(RNG::randVal(0, 3));
    int second = floor(RNG::randVal(0, 6));
    int third = floor(RNG::randVal(0, 6));
    int forth = floor(RNG::randVal(0, 6));

    int mr = floor(RNG::randVal(1, 3));
    if (mr == 2)
        mr = floor(RNG::randVal(1, 10));
    SearchEquation * se = new SearchEquation(first, second, third, forth, mr);
    return se;
}

void OriginalABC::initializeSEArchive() {

    std::stringstream ss;

    //    ss << config->getProblemDimension() << "_" << config->getProblemID()
    //            << "_" << "genEqus.dat";
    //    std::string fileName;
    //    fileName = ss.str();
    //
    //    std::ofstream ofs;
    //    ofs.open(fileName.c_str(), std::ofstream::out);


    for (int i = 0; i < config->getArchiveSize(); i++) {
        SearchEquation * se = createSE();
        searchEquations.push_back(se);

        // oluşturulan denklemler dosyaya yazılıyor
        //        ofs << se->getEquationString();
    }

    //    ofs.close();
    c = 0;
}

class SEComparator {
public:

    bool operator()(SearchEquation * se1, SearchEquation * se2) {
        return se1->getNofSuccess() > se2->getNofSuccess();
    }
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/////////////////////////////Search Equations////////////////////////////

/**
 * Bu metod uzakliga gore komsu secimi yapmak icin kullanilmaktadir. 
 * 
 * @param fsource komsulugu bulunacak yiyecek kaynagi, mevcut yiyecek kaynağı denilebilir.
 * @param fID komsuluk yiyecek kaynaginin indeksi, fsource indeks değeri
 * @return 
 */
FoodSource * OriginalABC::getSuitableReferencefor(FoodSource * fsource, unsigned int fID) {

    long double denum = 0.0;
    long double fProbs[foodSources.size()];

    // her bir yiyecek kaynaginin olasiligi hesaplaniyor
    // yiyecek kaynagina yakin olan yiyecek kaynaklarinin 
    // olasiligi daha yuksek oluyor
    for (unsigned int i = 0; i < foodSources.size(); i++) {
        if (i != fID) {
            FoodSource * refFood = foodSources.at(i);
            fProbs[i] = 1.0 / fsource->distanceTo(refFood); // yakın olanın olasılığı daha yüksek
            denum += fProbs[i];
        }
    }

    double r = RNG::randVal(0, 1);
    long double totalprobs = 0.0;
    unsigned int k = -1;

    // rulet benzeri bir secim ile olasilikli bir secim kurali ile bir yiyecek kaynagi seciliyor
    do {
        k++;
        if (k != fID) {
            totalprobs += fProbs[k];
        }
    } while (r * denum > totalprobs);

    return foodSources.at(k);
}

/******************************************************************************************************
 * index: referans yiyecek kaynağının indeksi
 * p2c: hangi parametrenin (boyutun) değişikliğe uğrayacağı
 * Employed bee aşamasindaki arama isleminin ilk teriminin belirlenmesi
 * 
 * ****************************************************************************************************/
double OriginalABC::getFirstTerm(double xc, double gb, double r, unsigned int str) {
    double value = 0.0;
    switch (str) {
        case 0: value = xc;
            break;
        case 1: value = gb;
            break;
        case 2: value = r;
            break;
    }

    return value;
}

double OriginalABC::getOtherTerms(double xc, double gb, double gbd, double r1, double r2, unsigned int str) {
    double value = 0;

    double randlr = -1.0; // phi sayısının varsayilan alt limiti 
    double randhr = 1.0; // phi sayisinin varsayilan ust limiti

    // hangi strateji secilmisse o stratejiye uygun ikinci terim isletilip sonuc donduruluyor
    switch (str) {
        case 0: value = RNG::randVal(randlr, randhr)*(gb - xc);
            break;
        case 1: value = RNG::randVal(randlr, randhr)*(gb - r1);
            break;
        case 2: value = RNG::randVal(randlr, randhr)*(xc - r1);
            break;
        case 3: value = RNG::randVal(randlr, randhr)*(r1 - r2);
            break;
        case 4: value = RNG::randVal(randlr, randhr)*(xc - gbd);
            break;
        case 5: value = 0;
            break;
    }

    return value;

}

////////////////////////////////////////////////////////////////////////////////

/*****************************************************************************************************************
 * Employed bee asamasinin isletildigi metod
 * 
 * ***************************************************************************************************************/
void OriginalABC::sendEmployedBees() {


    for (int i = 0; i < foodSources.size(); i++) { // Her bir yiyecek kaynagi icin

        FoodSource * currentFood = foodSources.at(i); // siradaki yiyecek kaynagina git
        double * currloc = currentFood->getLocation();

        // rastgele bir komşu alınıyor. mevcut yiyecek kaynağına eşit olmayan
        int neighbour = -1;
        do {
            neighbour = floor(RNG::randVal(0, foodSources.size()));
        } while (neighbour == i);

        // rastgele ikinci bir komşu alınıyor. mevcut yiyecek kaynağına eşit olmayan
        int neighbour2 = -1;
        do {
            neighbour2 = floor(RNG::randVal(0, foodSources.size()));
        } while (neighbour2 == i || neighbour2 == neighbour);

        // komşuların locationları alınıyor
        double * neighbourloc = (foodSources.at(neighbour))->getLocation();
        double * neighbour2loc = (foodSources.at(neighbour2))->getLocation();

        // en iyi location alınıyor.
        double * gbestloc = (getBestFSource())->getLocation();

        // seçim işlemi yapılıyor. uzaklık değerine göre.
        FoodSource * gbestneighbour = getSuitableReferencefor(currentFood, i);
        double * gbestdistloc = gbestneighbour->getLocation();

        // yiyecek kaynaginin pozisyonunu gecici bir yere ata
        for (int j = 0; j < config->getProblemDimension(); j++)
            tempSource[j] = currloc[j];

        SearchEquation * se = searchEquations.at(c);
        //        SearchEquation * se = searchEquations.at(floor(RNG::randVal(0, c)));


        // kac tane boyutta degisiklik yapilacaksa o kadar adet islemi tekrarla
        int param2change = floor(RNG::randVal(0, config->getProblemDimension()));

        // denklem oluşturulurken kullanılacak termler alınıyor
        double xc = currloc[param2change];
        double gb = gbestloc[param2change];
        double gbd = gbestdistloc[param2change];
        double r1 = neighbourloc[param2change];
        double r2 = neighbour2loc[param2change];

        // 4 terimli arama islemimizden gecirerek boyutun degerini degistir
        tempSource[param2change] = getFirstTerm(xc, gb, r1, se->getFirstTerm()) +
                getOtherTerms(xc, gb, gbd, r1, r2, se->getSecondTerm()) +
                getOtherTerms(xc, gb, gbd, r1, r2, se->getThirdTerm()) +
                getOtherTerms(xc, gb, gbd, r1, r2, se->getForthTerm());


        // eger boyutun degeri sinirlari tasiriyorsa, sinirlar icine cek 
        if (tempSource[param2change] < config->getMinInitRange())
            tempSource[param2change] = config->getMinInitRange();
        if (tempSource[param2change] > config->getMaxInitRange())
            tempSource[param2change] = config->getMaxInitRange();

        // guncellenen degerle yiyecek kaynagini sina. Yani amac fonksiyonuna sokarar amac degerini elde et
        long double objectiveValue = p->getFunctionValue(tempSource);

        // eger elde edilen degerin fitnes degeri daha iyiyse pozisyonu guncelle
        if (1.0 / (objectiveValue + 1.0) > currentFood->getFitness()) {
            se->increaseSuccess();
            currentFood->initTrail(); // trial degiskenini sifirla
            currentFood->setLocation(tempSource); // yiyecek kaynaginin pozisyonunu guncelle
            currentFood->setObjectiveValue(objectiveValue); // amac degerini guncelle

            //            cout << "(ES) used equation : " << se->getEquationString();
        } else { //eger elde edilen degerin fitnessi kucukse iyilestirme yapamamis demektir, trial degerini artir 
            currentFood->increaseTrailVal();
        }

    } // end-of-for
    /*end of employed bee phase*/

}



////////////////////////////////////////////////////////////// CALCULATE PROBABILITIES //////////////////////////////////////////////////////////////////////////////

/**********************************************************************************************************************
 * Bu metod onlooker bee asamasinda secilecek yiyecek kaynaklarinin secilme olasiginin belirlendigi metoddur.
 * Literaturdeki 3 farkli secim yolu denenmistir. Strateji disaridan alinan bilgiye gore secilmektedir.
 * ********************************************************************************************************************/
void OriginalABC::calculateProbabilities() {
    calculateProbabilitiesDefault();
}

/*******************************************************************************************************************
 * Orjinal ABC'deki olasilik hesabi yontemidir. Makalemde Eq. 4 'de nasil hesaplandigi gosterilmistir. 
 * Bu hesaplamanin dezavantaji, bir yiyecek kaynagi digerlerinden cok cok iyi durumdaysa olasiligi cok yuksek ve
 * digerlerinin cok az olacagindan secimde cesitliligin azalmasina sebeb olmasidir. Bu durum ozellikle yerel arama
 * kullanildiginda ortaya cikabilir.
 *******************************************************************************************************************/
void OriginalABC::calculateProbabilitiesDefault() {
    int i;
    double totalfit = 0.0;

    for (i = 0; i < foodSources.size(); i++) {
        totalfit += (foodSources.at(i))->getFitness();
    }

    for (i = 0; i < foodSources.size(); i++) {
        (foodSources.at(i))->setProbability((foodSources.at(i))->getFitness() / totalfit);
    }


}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////// ONLOOKER BEES //////////////////////////////////////////////////////////////////////////////////////

/***********************************************************************************************************************************
 * Onlooker bee asamasinin isletildigi bolumdur. Bu asamanin employed bee asamasindan tek farki, ziyaret edilen yani degisiklige 
 * ugrayan yiyecek kaynaginin olasiliga göre secilmesidir. Her bir yiyecegin ziyaret edilme olasiligi bir onceki metodlarda belirleniyordu
 * 
 * *********************************************************************************************************************************/
void OriginalABC::sendOnlookerBees() {

    int i, j, t;
    i = 0;
    t = 0;
    /*onlooker Bee Phase*/
    while (t < foodSources.size()) {
        FoodSource * currentFood = foodSources.at(i);

        double r = RNG::randVal(0, 1);

        if (r < currentFood->getProbability()) {

            int neighbour = -1;
            do {
                neighbour = floor(RNG::randVal(0, foodSources.size()));
            } while (neighbour == i);

            int neighbour2 = -1;
            do {
                neighbour2 = floor(RNG::randVal(0, foodSources.size()));
            } while (neighbour2 == i || neighbour2 == neighbour);

            double * neighbourloc = (foodSources.at(neighbour))->getLocation();
            double * neighbour2loc = (foodSources.at(neighbour2))->getLocation();

            double * gbestloc = (getBestFSource())->getLocation();

            FoodSource * gbestneighbour = getSuitableReferencefor(currentFood, i);
            double * gbestdistloc = gbestneighbour->getLocation();

            t++;

            double * currloc = currentFood->getLocation();
            for (j = 0; j < config->getProblemDimension(); j++)
                tempSource[j] = currloc[j];

            SearchEquation * se = searchEquations.at(c);
            //            SearchEquation * se = searchEquations.at(floor(RNG::randVal(0, c)));

            for (int m = 0; m < se->getMR(); m++) {
                // kac tane boyutta degisiklik yapilacaksa o kadar adet islemi tekrarla
                int param2change = floor(RNG::randVal(0, config->getProblemDimension()));

                double xc = currloc[param2change];
                double gb = gbestloc[param2change];
                double gbd = gbestdistloc[param2change];
                double r1 = neighbourloc[param2change];
                double r2 = neighbour2loc[param2change];
                // 4 terimli arama islemimizden gecirerek boyutun degerini degistir
                tempSource[param2change] = getFirstTerm(xc, gb, r1, se->getFirstTerm()) +
                        getOtherTerms(xc, gb, gbd, r1, r2, se->getSecondTerm()) +
                        getOtherTerms(xc, gb, gbd, r1, r2, se->getThirdTerm()) +
                        getOtherTerms(xc, gb, gbd, r1, r2, se->getForthTerm());

                /*if generated parameter value is out of boundaries, it is shifted onto the boundaries*/
                if (tempSource[param2change] < config->getMinInitRange())
                    tempSource[param2change] = config->getMinInitRange();
                if (tempSource[param2change] > config->getMaxInitRange())
                    tempSource[param2change] = config->getMaxInitRange();
            }

            long double objectiveValue = p->getFunctionValue(tempSource);


            /*a greedy selection is applied between the current solution i and its mutant*/
            if (1.0 / (objectiveValue + 1.0) > currentFood->getFitness()) {
                se->increaseSuccess();
                /*If the mutant solution is better than the current solution i, replace the solution with the mutant and reset the trial counter of solution i*/
                currentFood->initTrail();
                currentFood->setLocation(tempSource);
                currentFood->setObjectiveValue(objectiveValue);
                //                cout << "(OS) used equation : " << se->getEquationString();
            } else { /*if the solution i can not be improved, increase its trial counter*/
                currentFood->increaseTrailVal();
            }

        } /*if */

        i++;
        if (i == foodSources.size() - 1)
            i = 0;
    }/*while*/
    /*end of onlooker bee phase     */

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////// SCOUT BEE STEP //////////////////////////////

/*************************************************************************************************************
 * Scout bee asamasinin isletildigi metoddur. 5 tane farkli scout bee asamasi yontemi burada istenildiginde 
 * secilebilmektedir. 
 * ***********************************************************************************************************/
void OriginalABC::sendScoutBees() {
    sendScoutBeesDefault();
}

/**************************************************************************************************************
 * Original ABC'deki scout bee mekanizma
 **************************************************************************************************************/
void OriginalABC::sendScoutBeesDefault() {
    int maxtrialindex, i;
    maxtrialindex = 0;
    for (i = 1; i < foodSources.size(); i++) { // bir yiyecek kaynaginin limite gelip gelmedigini kontrol et
        if ((foodSources.at(i))->getTrail() > (foodSources.at(maxtrialindex))->getTrail()) {
            maxtrialindex = i;
        }
    }
    if ((foodSources.at(maxtrialindex))->getTrail() >= config->getLimit()) {
        (foodSources.at(maxtrialindex))->reInit(); // limit degerine ulasan yiyecek kaynagini bastan olustur
    }

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////7

void OriginalABC::reinitializeSEArchieve() {
    for (int i = 0; i < searchEquations.size(); i++)
        (searchEquations.at(i))->initSuccess((double) p->getFunctionEvaluations() / (double) config->getMaxFES());
}

void OriginalABC::eliminateWorstCandidateSE() {
    //sort foodsources according to their fitness
    SEComparator secomp;
    sort(searchEquations.begin(), searchEquations.end(), secomp);
    //sort(searchEquations.begin(), searchEquations.end());
    //
    //    cout << "sıralam sonrası vectorun hali \n";
    //    for (int i = 0; i < searchEquations.size(); i++) {
    //        SearchEquation* se = searchEquations.at(i);
    //        cout << "(" << i << ")" << se->getEquationString();
    //    }
    //    cout << "\n==================\n";

    //delete half of the worst search equations
    int s = config->getArchiveSize();
    double val = s / floor((config->getMaxFES() / (double) (2.0 * config->getMaximumFoodSize())) / (double) s);
    if (val >= c - 2) val = c - 2;
    //    cout << "val = " << val << endl;
    //  if(val < 1) val+=1; 
    if (c > 2)
        searchEquations.erase(searchEquations.end() - floor(val)/*+floor(c/(config->getMaxFES()/c))*/, searchEquations.end());
    //for(int i = 0; i < 2; i++){
    SearchEquation * se = createSE();
    searchEquations.push_back(se);
    //    cout << "search equation size : " << searchEquations.size() << endl;
    //}
}

void OriginalABC::adaptivelySelectLS(FoodSource* bestf) {

    LocalSearch* mtsls = new Mtsls1(config, p);
    LocalSearch* cmaes = new ICMAESLS(config, p);

    if (ls != NULL) delete ls;

    long double currsol = p->getBestSolutionValue();

    FoodSource* f1 = NULL;
    FoodSource* f2 = NULL;

    if (!nols) {
        f1 = new FoodSource(bestf);
        f2 = new FoodSource(bestf);
    } else {
        f1 = new FoodSource(config, p);
        f2 = new FoodSource(f1);
    }

    mtsls->apply(f1, (config->getMaxInitBound() - config->getMinInitBound()) / 4, config->getMaxFES() * config->trainingRatio());
    long double solutionmtsls = p->getBestSolutionValue();


    //FoodSource* f2 = new FoodSource(bestf);
    cmaes->apply(f1, (config->getMaxInitBound() - config->getMinInitBound()) / 4, config->getMaxFES() * config->trainingRatio()*2);
    long double solutioncmaes = p->getBestSolutionValue();
    delete f2;
    delete f1;
    if (currsol != solutioncmaes || currsol != solutionmtsls) {
        if (solutioncmaes < solutionmtsls) {

            ls = cmaes;
            //cout << "cmaes\n";

        } else {

            ls = mtsls;
            delete cmaes;
            //cout << "mtsls\n";
        }
    } else {

        delete cmaes;
        delete mtsls;
        //delete ls;
        ls = NULL;
        nols = true;
        //cout << "no-ls\n";
    }

    bestf->setObjectiveValue(p->getBestSolutionValue());
    bestf->setLocation(p->getBestSoFarSolution());

}

/********************************************************************************************************************************
 * Algoritmanin isletildiği metoddur.
 * 
 * *******************************************************************************************************************************/
void OriginalABC::run() {
    unsigned int iterations = 0;
    int count = 0;
    do {

        sendEmployedBees();
        calculateProbabilities();
        sendOnlookerBees();
        getBestFSource();
        sendScoutBees();


        FoodSource * bestfood = getBestFSource(); // en iyi yiyecek kaynagini al ve

        // local search algoritmaları yarıştırılıyor.
        if (p->getFunctionEvaluations() > (config->getMaxFES() * config->trainingRatio())) {
            if (config->getLocalSearchID() == ADAPTIVE_LS && !nols) {
                adaptivelySelectLS(bestfood);
            }
        }
        if (ls != NULL) {

            long double solbefore = p->getBestSolutionValue();

            applyLocalSearch(bestfood); // uzerinde yerel arama algoritmasi uygula (eğer yerel arama secilirse)  

            long double solafter = p->getBestSolutionValue();
            if (solbefore > solafter) nols = true;
            else nols = false;
        }
        if (iterations > 0 && iterations % config->getGrowthPeriod() == 0 && foodSources.size() < config->getMaximumFoodSize()) {
            foodSources.push_back(new FoodSource(config, p, getBestFSource())); // yiyecek kaynagini en iyi yiyeceğe göre ekle
        }

        iterations++;
        c++;

        if (c == searchEquations.size()) {


            //            if (c != 3) {
            //                cout << "pool Size = " << c << endl;
            //            }

            //cout << "HERE1 " << c << endl;
            if (c >= 2) {
                //cout<<" iterations = "<<iterations<<endl;
                eliminateWorstCandidateSE();
            }
            c = 0;
            //cout<<"HERE2"<<endl;
            reinitializeSEArchieve();
            //cout<<"HERE3"<<endl;
        }


    } while (notDone());

    //    if (ls == NULL) {
    //        cout << "no-ls\n";
    //    }


    //    cout << "pool size = " << searchEquations.size() << "\n";
    //
    //    //    cout << "sona kalan denklemler \n";
    //    for (int i = 0; i < searchEquations.size(); i++) {
    //        SearchEquation* se = searchEquations.at(i);
    //        cout << "(" << i << ")" << se->getEquationString();
    //    }
}

