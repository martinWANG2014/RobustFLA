
#include "../include/Solution.h"

double Network::PERIOD_LENGTH = 30;
int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
//        if (argc < 15) {
//            std::cout
//                    << "Usage mainProgram airportJsonFile beaconJsonFile flightJsonFile "
//                    << "MethodMode RandomMode epsilon coefPi coefPij "
//                    << "alpha beta gamma w1 w2 p" << std::endl
//                    << "Method mode:" << std::endl
//                    << "\t0\tHoeffding Inequalities" << std::endl
//                    << "\t1\tMonte-Carlo Simulation" << std::endl
//                    << "\t2\tGaussian Method" << std::endl
//                    << "RandomMode:" << std::endl
//                    << "\t0\t Normal distribution" << std::endl
//                    << "\t1\t Folded Normal distribution" << std::endl
//                    << "\t\2\t Hybrid Folded Normal distribution" << std::endl
//                    << "epsilon\tinteger in [0, 100]" << std::endl
//                    << "coefPi\tinteger in [0,100]" << std::endl
//                    << "coefPij\tinteger in [0, 100]" << std::endl;
//        }
        Network::PERIOD_LENGTH = 40;
        Network network;
        std::cout << Network::PERIOD_LENGTH;
//        vdList viParameter;
//        viParameter.push_back(boost::lexical_cast<double>(argv[9]));
//        viParameter.push_back(boost::lexical_cast<double>(argv[10]));
//        viParameter.push_back(boost::lexical_cast<double>(argv[11])/100.0);
//        viParameter.push_back(boost::lexical_cast<double>(argv[12])/100.0);
//        viParameter.push_back(boost::lexical_cast<double>(argv[13])/100.0);
//        viParameter.push_back(boost::lexical_cast<double>(argv[14])/100.0);
//        Input input(argv[1], argv[2], argv[3]);
//        input.initNetwork(&network, boost::lexical_cast<int>(argv[5]), viParameter);
//        double dSumBenefits = 0;
//        int iMaxNbConflict = 0;
//        ApproximateFLA(&network, &dSumBenefits, &iMaxNbConflict, boost::lexical_cast<int>(argv[4]),
//                       boost::lexical_cast<int>(argv[5]), boost::lexical_cast<double>(argv[6]) / 100.0,
//                       boost::lexical_cast<double>(argv[7]) / 100.0, boost::lexical_cast<double>(argv[8]) / 100.0,
//                       viParameter);
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}