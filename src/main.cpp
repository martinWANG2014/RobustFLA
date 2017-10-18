#include "../include/Solution.h"
#include <boost/filesystem.hpp>
void EchoUsage(){
    std::cout
            << "[USAGE] RobustFLA airport_json_File beacon_json_file flight_json_file alpha beta gamma w1 w2 p pa sigmaMode generateFlight period_length feasible_list_size method_mode epsilon coefficient_Pi nbIterations deterministicRule displayMode"
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

int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
        if (argc != 21) {
            EchoUsage();
            abort();
        }
        bool generatedFlightData = boost::lexical_cast<int>(argv[12]) == 1;
        vdList vdDataParameters;
        vdDataParameters.push_back(boost::lexical_cast<int>(argv[4]));
        vdDataParameters.push_back(boost::lexical_cast<int>(argv[5]));
        vdDataParameters.push_back(boost::lexical_cast<int>(argv[6]) / 10000.0);
        vdDataParameters.push_back(boost::lexical_cast<int>(argv[7]) / 100.0);
        vdDataParameters.push_back(boost::lexical_cast<int>(argv[8]) / 100.0);
        vdDataParameters.push_back(boost::lexical_cast<int>(argv[9]) / 100.0);
        vdDataParameters.push_back(boost::lexical_cast<int>(argv[10]) / 100.0);

        std::stringstream modifiedFlightFileName;
        if (generatedFlightData && vdDataParameters[6] > 0) {
            modifiedFlightFileName << "../data/flight_" << vdDataParameters[0] << "_" << vdDataParameters[1] << "_"
                                   << vdDataParameters[2] << "_" << vdDataParameters[3] << "_" << vdDataParameters[4]
                                   << "_" << vdDataParameters[5]
                                   << "_0_-.json";
        } else {
            modifiedFlightFileName << "../data/flight_" << vdDataParameters[0] << "_" << vdDataParameters[1] << "_"
                                   << vdDataParameters[2] << "_" << vdDataParameters[3] << "_" << vdDataParameters[4]
                                   << "_" << vdDataParameters[5]
                                   << "_"
                                   << vdDataParameters[6] << "_-.json";
        }
        String flightName = modifiedFlightFileName.str();
        bool deterministicRule = boost::lexical_cast<int>(argv[19]) == 1;
        bool modeDisplay = boost::lexical_cast<int>(argv[20]) == 1;
        double epsilon = boost::lexical_cast<int>(argv[16]) / 100.0;
        int feasibleSize = boost::lexical_cast<int>(argv[14]);
        double coefPi = boost::lexical_cast<int>(argv[17]) / 100.0;
        bool modeSigma = boost::lexical_cast<int>(argv[11]) == 1;
        int modeMethod = boost::lexical_cast<int>(argv[15]);
        double offsetAdditionRoute = 300;
        int nbIterations = boost::lexical_cast<int>(argv[18]);
        if (nbIterations < 0) {
            nbIterations = 1000;
        }
        Network network;
        Input input(argv[1], argv[2], generatedFlightData && vdDataParameters[6] == 0 ? argv[3] : flightName);
        input.initNetwork(&network, modeDisplay, !generatedFlightData || vdDataParameters[6] > 0);
        network.initialize(vdDataParameters, coefPi, feasibleSize, modeSigma, modeDisplay, generatedFlightData);
//        network.writeFlightsJsonData(&network, vdDataParameters);
        String dataFilename = (generatedFlightData || !exists(flightName)) ? network.generateFlights(vdDataParameters,
                                                                                                     offsetAdditionRoute)
                                                                           : flightName;
        Network::PERIOD_LENGTH = boost::lexical_cast<int>(argv[13]) / 1.00;
        if (modeMethod == 5) {
            ApproximateFLA(&network, vdDataParameters, dataFilename, epsilon, coefPi, feasibleSize, 0, modeSigma,
                           modeDisplay, deterministicRule);
            ApproximateFLA(&network, vdDataParameters, dataFilename, epsilon, coefPi, feasibleSize, 1, modeSigma,
                           modeDisplay, deterministicRule);
            ApproximateFLA(&network, vdDataParameters, dataFilename, epsilon, coefPi, feasibleSize, 2, modeSigma,
                           modeDisplay, deterministicRule, nbIterations);
            ApproximateFLA(&network, vdDataParameters, dataFilename, epsilon, coefPi, feasibleSize, 3, modeSigma,
                           modeDisplay, deterministicRule);

        } else {
            ApproximateFLA(&network, vdDataParameters, dataFilename, epsilon, coefPi, feasibleSize, modeMethod,
                           modeSigma, modeDisplay, deterministicRule, nbIterations);
        }
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}