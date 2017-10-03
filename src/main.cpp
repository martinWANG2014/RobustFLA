#include "../include/Solution.h"

void EchoUsage(){
    std::cout
            << "[USAGE] RobustFLA airport_json_File beacon_json_file flight_json_file method_mode period_length epsilon feasible_list_size coefficient_Pi alpha beta gamma w1 w2 p sigmaMode displayMode"
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
            << "modeDisplay \t\t\t A boolean, 1, show the details;  0, don't display the details.";
}
int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
        if (argc != 17) {
            EchoUsage();
            abort();
        }
        Network::PERIOD_LENGTH = boost::lexical_cast<double>(argv[5]);
        Network network;
        Input input(argv[1], argv[2], argv[3]);
        double dSumBenefits = 0;
        int iMaxNbConflict = 0;
        vdList vdSigmaParameters;
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[9]));
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[10]));
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[11]) / 10000.0);
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[12]) / 100.0);
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[13]) / 100.0);
        vdSigmaParameters.push_back(boost::lexical_cast<int>(argv[14]) / 100.0);


        input.initNetwork(&network, boost::lexical_cast<int>(argv[16]) == 1);
        network.initialize(boost::lexical_cast<double>(argv[8]) / 100.0, boost::lexical_cast<int>(argv[7]),
                           vdSigmaParameters, boost::lexical_cast<int>(argv[15]) == 1,
                           boost::lexical_cast<int>(argv[16]) == 1);
        if (boost::lexical_cast<int>(argv[4]) == 5) {
            ApproximateFLA(&network, vdSigmaParameters, boost::lexical_cast<double>(argv[6]) / 100.0,
                           boost::lexical_cast<double>(argv[8]) / 100.0, &dSumBenefits, &iMaxNbConflict, 0,
                           boost::lexical_cast<int>(argv[16]) == 1);
            ApproximateFLA(&network, vdSigmaParameters, boost::lexical_cast<double>(argv[6]) / 100.0,
                           boost::lexical_cast<double>(argv[8]) / 100.0, &dSumBenefits, &iMaxNbConflict, 1,
                           boost::lexical_cast<int>(argv[16]) == 1);
            ApproximateFLA(&network, vdSigmaParameters, boost::lexical_cast<double>(argv[6]) / 100.0,
                           boost::lexical_cast<double>(argv[8]) / 100.0, &dSumBenefits, &iMaxNbConflict, 3,
                           boost::lexical_cast<int>(argv[16]) == 1);
            ApproximateFLA(&network, vdSigmaParameters, boost::lexical_cast<double>(argv[6]) / 100.0,
                           boost::lexical_cast<double>(argv[8]) / 100.0, &dSumBenefits, &iMaxNbConflict, 2,
                           boost::lexical_cast<int>(argv[16]) == 1);
        } else {
            ApproximateFLA(&network, vdSigmaParameters, boost::lexical_cast<double>(argv[6]) / 100.0,
                           boost::lexical_cast<double>(argv[8]) / 100.0, &dSumBenefits, &iMaxNbConflict,
                           boost::lexical_cast<int>(argv[4]), boost::lexical_cast<int>(argv[16]) == 1);
        }
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}