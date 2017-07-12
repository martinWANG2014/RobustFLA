//
// Created by chenghaowang on 06/07/17.
//
#include "../include/Input.h"

static double MIN_SIGMA = 0.1;
static double PRECISION_SIGMA = 0.001;

double getSigma2Recursive(double alpha, double beta, double gamma, double sigma1, double sigma2) {
    double sigma = (sigma1 + sigma2) / 2.0;
    double gamma1 = 0.5 * (boost::math::erf(alpha / (sqrt(2) * sigma)) + boost::math::erf(beta / (sqrt(2) * sigma)));
    if (fabs(gamma1 - gamma) < PRECISION_SIGMA) {
        return gamma1;
    }
    if (gamma1 < gamma) {
        return getSigma2Recursive(alpha, beta, gamma, sigma, sigma2);
    } else {
        return getSigma2Recursive(alpha, beta, gamma, sigma1, sigma);
    }
}

double getSigma3Recursive(double alpha, double beta, double gamma, double w1, double w2, double p, double sigma1,
                          double sigma2) {
    double sigma = (sigma1 + sigma2) / 2.0;
    double gamma1 = 0.5 * p * (boost::math::erf((1 + w1 * alpha) / (sqrt(2) * sigma)) +
                               boost::math::erf((1 - w1 * alpha) / (sqrt(2) * sigma))) + 0.5 * (1 - p) *
                                                                                         (boost::math::erf(
                                                                                                 (1 + w2 * beta) /
                                                                                                 (sqrt(2) * sigma)) +
                                                                                          boost::math::erf(
                                                                                                  (1 - w2 * beta) /
                                                                                                  (sqrt(2) * sigma)));
    if (fabs(gamma1 - gamma) < PRECISION_SIGMA) {
        return gamma1;
    }
    if (gamma1 < gamma) {
        return getSigma3Recursive(alpha, beta, gamma, w1, w2, p, sigma, sigma2);
    } else {
        return getSigma3Recursive(alpha, beta, gamma, w1, w2, p, sigma1, sigma);
    }
}

double getSigmaRecursive(double alpha, double gamma, double w1, double sigma1, double sigma2) {
    double sigma = (sigma1 + sigma2) / 2.0;
    double gamma1 = 0.5 * (boost::math::erf((1 + w1 * alpha) / (sqrt(2) * sigma)) +
                           boost::math::erf((1 - w1 * alpha) / (sqrt(2) * sigma)));
    if (fabs(gamma1 - gamma) < PRECISION_SIGMA) {
        return gamma1;
    }
    if (gamma1 < gamma) {
        return getSigmaRecursive(alpha, gamma, w1, sigma, sigma2);
    } else {
        return getSigmaRecursive(alpha, gamma, w1, sigma1, sigma);
    }
}

double getSigma(double alpha, double gamma, double w1) {
    double sigma1 = alpha * (1 - w1) / (sqrt(2) * boost::math::erf_inv(gamma));
    double sigma2 = alpha * (1 + w1) / (sqrt(2) * boost::math::erf_inv(gamma));
    double dMaxSigma = getSigmaRecursive(alpha, gamma, w1, sigma1, sigma2);
    uni_dist UniformDistribution(MIN_SIGMA, dMaxSigma);
    std::default_random_engine generator;
    return UniformDistribution(generator);
}

double getSigma1(double alpha, double beta, double gamma) {
    double dMaxSigma = (beta + alpha) / (2 * sqrt(2) * boost::math::erf_inv(gamma));
    uni_dist UniformDistribution(MIN_SIGMA, dMaxSigma);
    std::default_random_engine generator;
    return UniformDistribution(generator);
}

double getSigma2(double alpha, double beta, double gamma) {
    double sigma1 = alpha / (sqrt(2) * boost::math::erf_inv(gamma));
    double sigma2 = beta / (sqrt(2) * boost::math::erf_inv(gamma));
    double dMaxSigma = getSigma2Recursive(alpha, beta, gamma, sigma1, sigma2);
    uni_dist UniformDistribution(MIN_SIGMA, dMaxSigma);
    std::default_random_engine generator;
    return UniformDistribution(generator);
}

double getSigma3(double alpha, double beta, double gamma, double w1, double w2, double p) {
    double sigma1 = alpha / (sqrt(2) * boost::math::erf_inv(gamma));
    double sigma2 = beta / (sqrt(2) * boost::math::erf_inv(gamma));
    double dMaxSigma = getSigma3Recursive(alpha, beta, gamma, w1, w2, p, sigma1, sigma2);
    uni_dist UniformDistribution(MIN_SIGMA, dMaxSigma);
    std::default_random_engine generator;
    return UniformDistribution(generator);
}
void Input::parseWayPoints(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "[INFO] Parsing wayPoints file... " << std::flush;

    // Verify whether the data file exists or not: if not, pop up an error message.
    if (!exists(sWayPointPath)) {
        std::cerr << "[ERROR] Not exist " << sWayPointPath << std::endl;
        abort();
    }

    // Parse the json file with boost property tree.
    ptree root;
    // Read the json file by the read_json method offered by the boost library.
    read_json(sWayPointPath, root);

    int nbWayPoints = root.get<int>("BN");
    for (int i = 0; i < nbWayPoints; i++) {
        String sPrefixed(std::to_string(i));
        // Parse the wayPoint code.
        String sCode = root.get<String>(sPrefixed + ".Code");
        // Verify the wayPoint code, if it's empty, then pop up an error message.
        if (sCode.empty()) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th wayPoint's code is unknown, it is ignored automatically!" << std::endl;
        } else {
            // Parse the wayPoint name and its position.
            WayPoint *wayPoint = new WayPoint(sCode,
                                              root.get<String>(sPrefixed + ".Name", ""),
                                              root.get<double>(sPrefixed + ".Lat", 0.0),
                                              root.get<double>(sPrefixed + ".Lng", 0.0));

            // Verify whether the network has contained the wayPoint or not. If not then add it into network.
            if (!contains(pNetwork->getWayPointsList(), wayPoint)) {
                pNetwork->addNewWayPoint(wayPoint);
            }
        }
    }
    std::cout << "OK" << std::endl
              << "\tBeacon file data:" << std::endl
              << "\tWayPoints: " << nbWayPoints << std::endl
              << "\tValid WayPoints: " << pNetwork->getNbWayPoints() << std::endl;
}

void Input::parseAirports(Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;

    std::cout << "[INFO] Parsing airports file... " << std::flush;

    // Verify whether the data file exists or not: if not, pop up an error message.
    if (!exists(sAirportPath)) {
        std::cerr << "[ERROR] Not exist " << sAirportPath << std::endl;
        abort();
    }

    // Parse the json file with boost property tree.
    ptree root;
    // Read the json file by the read_json method offered by the boost library.
    read_json(sAirportPath, root);

    int nbAirports = root.get<int>("AN");
    for (int i = 0; i < nbAirports; i++) {
        String sPrefixed(std::to_string(i));
        // Parse the airport icao code.
        String sCode = root.get<String>(sPrefixed + ".ICAO");
        // Verify the airport icao code, if it's empty, then pop up an error message.
        if (sCode.empty()) {
            std::cout << "[Warning]: the " << i << "th airport's icao code is unknown, it is ignored automatically!"
                      << std::endl;
        } else {
            // Parse the airport name and its position.
            Airport *airport = new Airport(sCode,
                                           root.get<String>(sPrefixed + ".Name", ""),
                                           root.get<double>(sPrefixed + ".Lat", 0.0),
                                           root.get<double>(sPrefixed + ".Lng", 0.0));
            // Verify whether the network has contained the airport or not. If not, then add it into network.
            if (!contains(pNetwork->getAirportsList(), airport)) {
                pNetwork->addNewAirport(airport);
            }
        }
    }
    std::cout << "OK" << std::endl
              << "\tAirports file data:" << std::endl
              << "\tAirports: " << nbAirports << std::endl
              << "\tValid Airports: " << pNetwork->getNbAirports() << std::endl;
}

void Input::parseFlights(Network *pNetwork, int iRandomMode, const DoubleVector &vdParameter) {
    using boost::property_tree::ptree;
    using boost::property_tree::read_json;
    std::cout << "[INFO] Parsing flights file... " << std::flush;
    if (iRandomMode < 2 && (int) vdParameter.size() < 4) {
        std::cerr << "[ERROR] the parameter list for random method is not correct!" << std::endl;
        abort();
    }
    if (iRandomMode == 2 && (int) vdParameter.size() < 7) {
        std::cerr << "[ERROR] the parameter list for random method is not correct!" << std::endl;
        abort();
    }
    // Verify whether the data file exists or not: if not, pop up an error message.
    if (!exists(sFlightPath)) {
        std::cerr << "[ERROR] Not exist " << sFlightPath << std::endl;
        abort();
    }

    // Parse the json file with boost property tree.
    ptree root;
    // Read the json file by the read_json method offered by the boost library.
    read_json(sFlightPath, root);

    // Start to parse the content of json object.
    int nbFlights = root.get<int>("FN");
    for (int i = 0; i < nbFlights; i++) {
        String sPrefixed(std::to_string(i));
        // Parse the flight icao code.
        String sCode(root.get<String>(sPrefixed + ".FCode"));
        // Parse the flight departure airport.
        Airport *pAirportOrigin = pNetwork->findAirportByCode(root.get<String>(sPrefixed + ".Origin"));
        // Parse the flight destination airport.
        Airport *pAirportDestination = pNetwork->findAirportByCode(root.get<String>(sPrefixed + ".Dest"));
        // Parse the flight departure time.
        Time iDepartureTime = root.get<Time>(sPrefixed + ".DTime", -1);
        // Verify the icao code of a flight, if it is empty, then pop up an error message.
        if (sCode.empty()) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's code is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify the flight departure time, if it is not positive, then pop up an error message.
        else if (iDepartureTime < 0) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's departure time is not valid, it is ignored automatically!" << std::endl;
        }
            // Verify flight depart airport, if it is empty, then pop up an error message.
        else if (pAirportOrigin == nullptr) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's origin airport is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify flight destination airport, if it is empty, then pop up an error message.
        else if (pAirportDestination == nullptr) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's destination is unknown, it is ignored automatically!" << std::endl;
        }
            // Verify whether the departure airport is the same as the destination airport or not, if yes then pop up an error message.
        else if (*pAirportOrigin == *pAirportDestination) {
            std::cout << std::endl << "[Warning]: the " << i
                      << "th flight's destination is identical with the departure, it is ignored automatically!"
                      << std::endl;
        } else {
            // Parse the default flight level.
            Level iLevel = root.get<int>(sPrefixed + ".FLevel");
            Route *pRoute = new Route(iLevel, pAirportOrigin, iDepartureTime);
            int nbPoints = root.get<int>(sPrefixed + ".PointList.PLN");
            bool bValid = true;
            int iIndexPoint = 0;
            // Parse the flight default route.
            while (bValid && iIndexPoint < nbPoints) {
                String sPrefixedPath(sPrefixed + ".PointList." + std::to_string(iIndexPoint));
                // Parse the wayPoint in the route.
                WayPoint *pWayPoint = pNetwork->findWayPointByCode(root.get<String>(sPrefixedPath + ".Code"));
                // Verify the wayPoint, if it is empty, then pop up an error message.
                if (pWayPoint == nullptr) {
                    std::cout << std::endl << "[Warning]: the code of " << iIndexPoint << "th point in " << i
                              << "th flight's route is unknown, it is ignored automatically!"
                              << std::endl;
                    bValid = false;
                } else {
                    // Parse the arriving time at a corresponding wayPoint.
                    Point *point = new Point(pWayPoint, root.get<Time>(sPrefixedPath + ".Time"));
                    pRoute->addNewPoint(point);
                }
                iIndexPoint++;
            }
            // If the route is correct, then create a flight object.
            if (bValid) {
                Flight *pFlight = new Flight(sCode, pAirportOrigin, pAirportDestination, iDepartureTime, pRoute);
                // Verify whether the network has contained the flight or not. If not, then add it into network.
                bool bIsRouteValid = pFlight->selfCheck();
                if (bIsRouteValid && !contains(pNetwork->getFlightsList(), pFlight)) {
                    IntVector feasibleList = findFeasibleLevels(iLevel);
                    pFlight->setFeasibleLevelList(feasibleList);
                    double dSigma;
                    switch (iRandomMode) {
                        case 0:
                            dSigma = getSigma1(vdParameter[0], vdParameter[1], vdParameter[2]);
                            break;
                        case 1:
                            dSigma = getSigma2(vdParameter[0], vdParameter[1], vdParameter[2]);
                            break;
                        case 2:
                            dSigma = getSigma3(vdParameter[0], vdParameter[1], vdParameter[2], vdParameter[4],
                                               vdParameter[5], vdParameter[6]);
                            break;
                        default:
                            std::cout << std::endl
                                      << "[Warning]: the random mode is not supported, use the default one systemically!"
                                      << std::endl;
                            dSigma = getSigma1(vdParameter[0], vdParameter[1], vdParameter[2]);
                    }
                    pFlight->setSigma(dSigma);
                    pNetwork->addNewFlight(pFlight);
                } else if (bIsRouteValid) {
                    std::cout << std::endl
                              << "[Warning]: the route of " << i
                              << " flight is not correct, it is ignored automatically!"
                              << std::endl;
                } else {
                    std::cout << std::endl
                              << "[Warning]: the " << i << " flight was redundant, it is ignored automatically!"
                              << std::endl;
                }
            } else {
                std::cout << std::endl
                          << "[Warning]: the route of " << i << " flight is not complete, it is ignored automatically!"
                          << std::endl;
            }
        }
    }
    std::cout << "OK" << std::endl
              << "\tFlights file data:" << std::endl
              << "\tFlights: " << nbFlights << std::endl
              << "\tValid Flights: " << pNetwork->getNbFlights() << std::endl;
}

LevelVector Input::findFeasibleLevels(Level iDefaultLevel) {
    LevelVector feasibleList;
    feasibleList.push_back(iDefaultLevel);
#ifdef NRVSM
    if (iDefaultLevel < 290){
#else
    if (iDefaultLevel < 410) {
#endif
        auto position = LevelIFRB.end();
        switch (iDefaultLevel % 4) {
            // in IFR B group
            case 0:
                position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
                if (position== LevelIFRB.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in VFR B group
            case 1:
                position = std::find(LevelVFRB.begin(), LevelVFRB.end(), iDefaultLevel);
                if (position == LevelVFRB.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else if (position == LevelVFRB.end() - 1) {
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in IFR A group
            case 2:
                position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
                if (position == LevelIFRA.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
                // in VFR A group
            default:
                position = std::find(LevelVFRA.begin(), LevelVFRA.end(), iDefaultLevel);
                if (position == LevelVFRA.begin()) {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position + 2));
                } else if (position== LevelVFRA.end() - 1) {
                    feasibleList.push_back(*(position - 1));
                    feasibleList.push_back(*(position - 2));
                } else {
                    feasibleList.push_back(*(position + 1));
                    feasibleList.push_back(*(position - 1));
                }
                break;
        }
    } else if (iDefaultLevel % 40 == 10) {
        // in IFR A group
        auto position = std::find(LevelIFRA.begin(), LevelIFRA.end(), iDefaultLevel);
        if (position == LevelIFRA.end() - 1) {
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        } else {
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    } else {
        // in IFR B group
        auto position = std::find(LevelIFRB.begin(), LevelIFRB.end(), iDefaultLevel);
        if (position == LevelIFRB.end() - 1) {
            feasibleList.push_back(*(position - 1));
            feasibleList.push_back(*(position - 2));
        } else {
            feasibleList.push_back(*(position + 1));
            feasibleList.push_back(*(position - 1));
        }
    }
    return feasibleList;
}