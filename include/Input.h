//
// Created by martinwang on 27/04/17.
//

#ifndef INPUT_H
#define INPUT_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "Flight.h"
#include "Network.h"

////////////Definition of Flight Level/////////////////////////////////////////
typedef std::vector<int> IntVector;
typedef std::vector<double> DoubleVector;
#ifdef NRVSM
static IntVector const LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 330, 370, 410, 450, 490};
static IntVector const LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 310, 350, 390, 430, 470, 510};
#else
const IntVector LevelIFRA{10, 30, 50, 70, 90, 110, 130, 150, 170, 190, 210, 230, 250, 270, 290, 310, 330, 350, 370, 390,
                          410, 450, 490};
const IntVector LevelIFRB{20, 40, 60, 80, 100, 120, 140, 160, 180, 200, 220, 240, 260, 280, 300, 320, 340, 360, 380,
                          400, 430, 470, 510};
#endif
const IntVector LevelVFRA{35, 55, 75, 95, 115, 135, 155, 175, 195};
const IntVector LevelVFRB{45, 65, 85, 105, 125, 145, 165, 185};
////////////Definition of Flight Level/////////////////////////////////////////

////////////Method template////////////////////////////////////////////////////
/**
 * A template of the method to verify whether the a vector contains an element.
 * @tparam T                    the general type of data
 * @param vObjectsVector        the target vector
 * @param object                the queried object
 * @return true, if the vector contains the queried object; false, otherwise.
 */
template<typename T>
bool contains(std::vector<T> vObjectsVector, T object) {
    if (vObjectsVector.size() < 1) return false;
    for (auto &&item : vObjectsVector) {
        if (item == object) {
            return true;
        }
    }
    return false;
}

/**
 * A template of the method to verify whether the a vector contains an element, where the element is a pointer.
 * @tparam T                    the general type of data
 * @param vpObjectsVector       the target vector
 * @param pObject               the queried object
 * @return true, if the vector contains the queried object; false, otherwise.
 */
template<typename T>
bool contains(std::vector<T *> vpObjectsVector, T *pObject) {
    if (vpObjectsVector.size() < 1) return false;
    for (auto &&item : vpObjectsVector) {
        if (*item == *pObject) {
            return true;
        }
    }
    return false;
}
////////////Method template////////////////////////////////////////////////////

double getSigma2Recursive(double alpha, double beta, double gamma, double sigma1, double sigma2);

double getSigma3Recursive(double alpha, double beta, double gamma, double w1, double w2, double p, double sigma1,
                          double sigma2);

double getSigmaRecursive(double alpha, double gamma, double w1, double sigma1, double sigma2);

double getSigma(double alpha, double gamma, double w1);

double getSigma1(double alpha, double beta, double gamma);

double getSigma2(double alpha, double beta, double gamma);

double getSigma3(double alpha, double beta, double gamma, double w1, double w2, double p);
/**
 * The input class for a purpose to parse the data file, then to initialize the network object with these parsed data.
 */
class Input {
public:

    /**
     * Constructor with parameter
     * @param sAirportPath      The filename of airport
     * @param sWayPointPath     The filename of wayPoint
     * @param sFlightPath       The filename of flight
     */
    explicit Input(const String &sAirportPath, const String &sWayPointPath, const String &sFlightPath)
            : sAirportPath(sAirportPath), sWayPointPath(sWayPointPath), sFlightPath(sFlightPath){}

    /**
     * Destructor.
     */
    ~Input() {}

    /**
     * Initialize the air management network
     * @param pNetwork      A pointer of network
     */
    void initNetwork(Network *pNetwork, int iRandomMode, const DoubleVector &vdParameter) {
        parseAirports(pNetwork);
        parseWayPoints(pNetwork);
        parseFlights(pNetwork, iRandomMode, vdParameter);
        pNetwork->InitFlightLevelsList();
        std::cout << "\tFlight Level size: " << pNetwork->getNbLevels() << std::endl;
    }
private:
    /**
     * The airport filename.
     */
    String sAirportPath;

    /**
     * The wayPoint filename.
     */
    String sWayPointPath;

    /**
     * The flight filename.
     */
    String sFlightPath;

    /**
     * Parse the flight file.
     * @param pNetwork      A pointer of network
     */
    void parseFlights(Network *pNetwork, int iRandomMode, const DoubleVector &vdParameter);

    /**
     * Parse the airport file.
     * @param pNetwork      A pointer of network
     */
    void parseAirports(Network *pNetwork);

    /**
     * Parse the wayPoint file.
     * @param pNetwork      A pointer of network
     */
    void parseWayPoints(Network *pNetwork);

    /**
     * Verify whether the file exists.
     * @param name      The filename
     * @return true, if exists; false, otherwise.
     */
    bool exists(const String &name) {
        std::ifstream f(name.c_str());
        return f.good();
    }

    /**
     * Find the feasible flight level list.
     * @param iDefaultLevel     A given default flight level
     * @return A three elements list.
     */
    IntVector findFeasibleLevels(int iDefaultLevel);
};

#endif //INPUT_H
