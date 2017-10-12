#include "../include/Solution.h"
#include <boost/filesystem.hpp>
void EchoUsage(){
    std::cout
            << "[USAGE] RobustFLA airport_json_File beacon_json_file flight_json_file method_mode period_length epsilon feasible_list_size coefficient_Pi alpha beta gamma w1 w2 p sigmaMode displayMode [nbIterations]"
            << std::endl
            << "method_mode 0 \t\t\t Deterministic Method" << std::endl
            << "method_mode 1 \t\t\t Hoeffding Inequalities Method" << std::endl
            << "method_mode 2 \t\t\t Monte-Carlo Simulation Method" << std::endl
            << "method_mode 3 \t\t\t Gaussian Method" << std::endl
            << "period_length \t\t\t An integer, the length of period that consider en-route conflict occurs at a wayPoint."
            << std::endl
            << "epsilon \t\t\t An integer in [0, 100],  the tolerance of infeasibility of separated constraints in FLA problem."
            << std::endl
            << "feasible_list_size \t\t\t An integer, the size of feasible flight level list." << std::endl
            << "coefficient_Pi \t\t\t An integer in [0, 100], the percentage of aircraft's flight time to induce an admissible penal cost"
            << std::endl
            << "alpha \t\t\t An integer, the lower bound for the interval [t-alpha, t+beta]" << std::endl
            << "beta \t\t\t An integer, the upper bound for the interval [t-alpha, t+beta]" << std::endl
            << "gamma \t\t\t An integer in [0, 100], Pr(t-alpha<=x<=t+beta)>=gamma%, the confident that generated departure time fall in an interval [t-alpha, t+beta]"
            << std::endl
            << "w1 \t\t\t An integer in [0, 100], the mean u=t-w1*alpha of the left part x<t" << std::endl
            << "w2 \t\t\t An integer in [0, 100], the mean u=t+w2*beta of the right part x>=t" << std::endl
            << "p \t\t\t An integer in [0, 100], the probability that Pr(x<t)=p%" << std::endl
            << "modeSigma \t\t\t A boolean, 1, set the dynamic sigma value;  0, set the statistic sigma value."
            << std::endl
            << "modeDisplay \t\t\t A boolean, 1, show the details;  0, don't display the details."
            << "nbIterations \t\t\t An integer, it's required for Monte-Carlo method";
}

String writeJsonData(String SolutionDir, double percentAdditionalFlights, int mode, double epsilon, double coefPi,
                     const Network *pNetwork) {
    using boost::property_tree::ptree;
    using boost::property_tree::write_json;
    // check the solution directory.

    // Write the json file with boost property tree.
    ptree root;

    // get the target filename.
    std::stringstream outFilename;
    outFilename << "flight_" << mode << "_" << doubleToString2(epsilon) << "_" << doubleToString2(coefPi) << "_"
                << percentAdditionalFlights << ".json";

    //write the flight list in a json format.
    root.put("FN", pNetwork->getNbFlights());
    for (int i = 0; i < pNetwork->getNbFlights(); i++) {
        Flight *flight = pNetwork->getFlightAtI(i);
        ptree rootFlight;
        rootFlight.put("Origin", flight->getOrigAirportName());
        rootFlight.put("Dest", flight->getDestAirportName());
        rootFlight.put("FCode", flight->getCode());
        rootFlight.put("DTime", doubleToString(flight->getDepartureTime()));
        rootFlight.put("FLevel", flight->getDefaultLevel());
        ptree rootPointsList;
        for (int j = 0; j < flight->getRoute()->getPointListSize(); j++) {
            ptree point;
            point.put("Code", flight->getPointAtI(j)->getPointName());
            point.put("Time", doubleToString(flight->getPointAtI(j)->getArrivingTime()));
            rootPointsList.put_child(std::to_string(j), point);
        }
        rootPointsList.put("PLN", flight->getNbPoints());
        rootFlight.put_child("PointList", rootPointsList);
        root.put_child(std::to_string(i), rootFlight);
    }
    // write to the target file.
    write_json(SolutionDir + "/" + outFilename.str(), root);
    return outFilename.str();
}



int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
        if (argc < 19 || argc > 20 ||
            (argc == 20 && (boost::lexical_cast<int>(argv[4]) != 2 || boost::lexical_cast<int>(argv[4]) != 5)) ||
            (argc == 19 && (boost::lexical_cast<int>(argv[4]) == 2 || boost::lexical_cast<int>(argv[4]) == 5))) {
            EchoUsage();
            abort();
        }
        Network::PERIOD_LENGTH = boost::lexical_cast<double>(argv[5]);
        Network network;
        String solutionDir = argv[18];
        Input input(argv[1], argv[2], argv[3]);
        double offsetAdditionRoute = 300;
        double dSumBenefits = 0;
        int iMaxNbConflict = 0;
        vdList vdSigmaParameters;
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[9]));
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[10]));
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[11]) / 10000.0);
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[12]) / 100.0);
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[13]) / 100.0);
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[14]) / 100.0);

        bool modeDisplay = boost::lexical_cast<int>(argv[17]) == 1;
        input.initNetwork(&network, modeDisplay);

        double epsilon = boost::lexical_cast<double>(argv[6]) / 100.0;
        int feasibleSize = boost::lexical_cast<int>(argv[7]);
        double coefPi = boost::lexical_cast<double>(argv[8]) / 100.0;
        double percentAdditionalFlights = boost::lexical_cast<double>(argv[15]) / 100.0;
        bool modeSigma = boost::lexical_cast<int>(argv[16]) == 1;
        int modeMethod = boost::lexical_cast<int>(argv[4]);
        int nbIterations = (modeMethod == 2 || modeMethod == 5) ? boost::lexical_cast<int>(argv[19]) : 0;
        network.initialize(coefPi, feasibleSize, vdSigmaParameters, modeSigma, modeDisplay);
        network.update(boost::lexical_cast<int>(argv[15]) / 100.0, offsetAdditionRoute);
        String dataFilename = writeJsonData(solutionDir, boost::lexical_cast<int>(argv[15]) / 100.0, modeMethod,
                                            epsilon, coefPi, &network);
        if (boost::lexical_cast<int>(argv[4]) == 5) {
            ApproximateFLA(&network, vdSigmaParameters, solutionDir, dataFilename, epsilon, coefPi, feasibleSize,
                           percentAdditionalFlights, &dSumBenefits, &iMaxNbConflict, 0, modeSigma, modeDisplay);
            ApproximateFLA(&network, vdSigmaParameters, solutionDir, dataFilename, epsilon, coefPi, feasibleSize,
                           percentAdditionalFlights, &dSumBenefits, &iMaxNbConflict, 1, modeSigma, modeDisplay);
            ApproximateFLA(&network, vdSigmaParameters, solutionDir, dataFilename, epsilon, coefPi, feasibleSize,
                           percentAdditionalFlights, &dSumBenefits, &iMaxNbConflict, 2, modeSigma, modeDisplay,
                           nbIterations);
            ApproximateFLA(&network, vdSigmaParameters, solutionDir, dataFilename, epsilon, coefPi, feasibleSize,
                           percentAdditionalFlights, &dSumBenefits, &iMaxNbConflict, 3, modeSigma, modeDisplay);
        } else {
            ApproximateFLA(&network, vdSigmaParameters, solutionDir, dataFilename, epsilon, coefPi, feasibleSize,
                           percentAdditionalFlights, &dSumBenefits, &iMaxNbConflict, modeMethod, modeSigma, modeDisplay,
                           nbIterations);
        }
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}