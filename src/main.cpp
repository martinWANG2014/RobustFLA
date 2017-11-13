#include "../include/Solution.h"
#include <boost/filesystem.hpp>
void EchoUsage(){
    std::cout
            << "[USAGE] RobustFLA airport_json_File beacon_json_file flight_json_file pa period_length feasible_list_size method_mode epsilon coefficient_Pi nbIterations deterministicRule displayMode"
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
            ;
}

int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
        if (argc != 13 ) {
            EchoUsage();
            abort();
        }
        int percentileSup = boost::lexical_cast<int>(argv[4]);
        std::stringstream modifiedFlightFileName;
        modifiedFlightFileName << "../data/flight_" << percentileSup <<"_-.json";
        String flightName = modifiedFlightFileName.str();
        bool deterministicRule = boost::lexical_cast<int>(argv[11]) == 1;
        bool modeDisplay = boost::lexical_cast<int>(argv[12]) == 1;
        double epsilon = boost::lexical_cast<int>(argv[8]) / 100.0;
        int feasibleSize = boost::lexical_cast<int>(argv[6]);
        double coefPi = boost::lexical_cast<int>(argv[9]) / 100.0;
        int modeMethod = boost::lexical_cast<int>(argv[7]);
        int nbIterations = boost::lexical_cast<int>(argv[10]);
        if (nbIterations < 0) {
            nbIterations = 1000;
        }
        Network network;
        Input input(argv[1], argv[2], exists(flightName) ? flightName:argv[3]);
        input.initNetwork(&network, exists(flightName), modeDisplay);
        network.initialize(coefPi, feasibleSize, modeDisplay);
        String dataFilename = !exists(flightName) ? network.generateFlightsSup(percentileSup, TIMEOFFSETSUP) : flightName;
        Network::PERIOD_LENGTH = boost::lexical_cast<int>(argv[5]) / 1.00;
        if (modeMethod == 4) {
            ApproximateFLA(&network, dataFilename, epsilon, coefPi, feasibleSize, 0, percentileSup, modeDisplay, deterministicRule);
            ApproximateFLA(&network, dataFilename, epsilon, coefPi, feasibleSize, 1, percentileSup, modeDisplay, deterministicRule);
            ApproximateFLA(&network, dataFilename, epsilon, coefPi, feasibleSize, 2, percentileSup, modeDisplay, deterministicRule, nbIterations);
            ApproximateFLA(&network, dataFilename, epsilon, coefPi, feasibleSize, 3, percentileSup, modeDisplay, deterministicRule);
        } else {
            ApproximateFLA(&network, dataFilename, epsilon, coefPi, feasibleSize, modeMethod, percentileSup,  modeDisplay, deterministicRule, nbIterations);
        }
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}