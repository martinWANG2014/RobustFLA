
#include "../include/Solution.h"

double Position::EARTH_RADIUS = 6378.16;

std::ostream &operator<<(std::ostream &os, const Position &position) {
    os << "dLatitude: " << position.dLatitude << " dLongitude: " << position.dLongitude;
    return os;
}

double Route::K = 1.8520108;
double Route::MIN_SEPARATION_DISTANCE = 5.0;
double Route::A_BAR = 5;
double Route::MIN_ANGLE = cos(5.0 / 180.0 * M_PI);
double Flight::MIN_PROBA = 0.00001;
Time Point::PERIOD_UNIT = 30;
int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
        if (argc < 1) {
            std::cout
                    << "Usage mainProgram airportJsonFile beaconJsonFile flightJsonFile "
                    << "MethodMode RandomMode epsilon coefPi coefPij "
                    << "alpha beta gamma w1 w2 p" << std::endl
                    << "Method mode:" << std::endl
                    << "\t0\tHoeffding Inequalities" << std::endl
                    << "\t1\tMonte-Carlo Simulation" << std::endl
                    << "\t2\tGaussian Method" << std::endl
                    << "RandomMode:" << std::endl
                    << "\t0\t Normal distribution" << std::endl
                    << "\t1\t Folded Normal distribution" << std::endl
                    << "\t\2\t Hybrid Folded Normal distribution" << std::endl
                    << "epsilon\tinteger in [0, 100]" << std::endl
                    << "coefPi\tinteger in [0,100]" << std::endl
                    << "coefPij\tinteger in [0, 100]" << std::endl;
        }
        if (argc < 2) {
            throw invalid_argument("airport file required");
        } else if (argc < 3) {
            throw invalid_argument("beacon file required");
        } else if (argc < 4) {
            throw invalid_argument("flight file required");
        } else if (argc < 5) {
            throw invalid_argument("MethodMode required");
        } else if (argc < 6) {
            throw invalid_argument("RandomMode required");
        } else if (argc < 7) {
            throw invalid_argument("epsilon required");
        } else if (argc < 8) {
            throw invalid_argument("coefPi required");
        } else if (argc < 9) {
            throw invalid_argument("coefPij required");
        } else if (argc < 10) {
            throw invalid_argument("alpha required");
        } else if (argc < 11) {
            throw invalid_argument("beta required");
        } else if (argc < 12) {
            throw invalid_argument("gamma required");
        } else if (argc < 13) {
            throw invalid_argument("w1 required");
        } else if (argc < 14) {
            throw invalid_argument("w2 required");
        } else if (argc < 15) {
            throw invalid_argument("p required");
        }
        Network network;
        DoubleVector viParameter;
        viParameter.push_back(boost::lexical_cast<double>(argv[9]));
        viParameter.push_back(boost::lexical_cast<double>(argv[10]));
        viParameter.push_back(boost::lexical_cast<double>(argv[11]));
        viParameter.push_back(boost::lexical_cast<double>(argv[12]));
        viParameter.push_back(boost::lexical_cast<double>(argv[13]));
        viParameter.push_back(boost::lexical_cast<double>(argv[14]));
        Input input(argv[1], argv[2], argv[3]);
        input.initNetwork(&network, 0, viParameter);
        double dSumBenefits;
        int iMaxNbConflict;
        ApproximateFLA(&network, &dSumBenefits, &iMaxNbConflict, boost::lexical_cast<int>(argv[4]),
                       boost::lexical_cast<int>(argv[5]), boost::lexical_cast<double>(argv[6]) / 100.0,
                       boost::lexical_cast<double>(argv[7]) / 100.0, boost::lexical_cast<double>(argv[8]) / 100.0,
                       viParameter);
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}