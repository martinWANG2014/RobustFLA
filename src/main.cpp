#include "../include/Network.h"
#include "../include/Input.h"
#include "../include/Solution.h"
double Position::EARTH_RADIUS = 6378.16;
double Route::K = 1.8520108;
double Route::MIN_SEPARATION_DISTANCE = 5.0;
double Route::A_BAR = 5;
double Route::MIN_ANGLE = sin(5 / 180 * M_PI);
double Flight::MIN_PROBA = 0.00001;
double Flight::ALPHA = 15;
double Flight::BETA = 60;
double Flight::GAMMA = 0.95;
double Flight::W1 = 1.0/3;
double Flight::W2 = 1.0/3;
double Flight::RATIO = 0.5;
double Flight::mu_1_1 = -Flight::W1 * Flight::ALPHA;
double Flight::mu_1_2 = Flight::W2 * Flight::BETA;
double Flight::sigma_1_1 = (Flight::ALPHA) / (sqrt(2) * boost::math::erf_inv(Flight::GAMMA));
double Flight::sigma_1_2 = (Flight::BETA) / (sqrt(2) * boost::math::erf_inv(Flight::GAMMA));
double Flight:: mu_1 = (Flight::sigma_1_1 * sqrt(2.0 / M_PI) * exp(-pow(Flight::mu_1_1 / (sqrt(2.0) * Flight::sigma_1_1), 2)) +
        Flight::mu_1_1 * boost::math::erf(Flight::mu_1_1 / (sqrt(2.0) * Flight::sigma_1_1)));
double Flight::mu_2 = (Flight::sigma_1_2 * sqrt(2.0 / M_PI) * exp(-pow(Flight::mu_1_2 / (sqrt(2.0) * Flight::sigma_1_2), 2)) +
        Flight::mu_1_2 * boost::math::erf(Flight::mu_1_2 / (sqrt(2.0) * Flight::sigma_1_2)));
double Flight::sigma_1 = pow(Flight::mu_1_1, 2) + pow(Flight::sigma_1_1, 2) - pow(Flight::mu_1, 2);
double Flight::sigma_2 = pow(Flight::mu_1_2, 2) + pow(Flight::sigma_1_2, 2) - pow(Flight::mu_2, 2);
double Flight::HYBRID_MU = Flight::RATIO * Flight::mu_1 + (1 - Flight::RATIO) * Flight::mu_2;
double Flight::HYBRID_SIGMA = sqrt(Flight::RATIO * Flight::sigma_1 + (1 - Flight::RATIO) * Flight::sigma_2);
Time Input::PERIOD_UNIT = 30;
int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
//        if (argc < 2) {
//            throw invalid_argument("airport file required");
//        }
//        else if (argc < 3) {
//            throw invalid_argument("beacon file required");
//        }
//        else if (argc < 4) {
//            throw invalid_argument("flight file required");
//        }
//        else if (argc < 5) {
//            throw invalid_argument("PeriodUnit required");
//        }
//        else if (argc < 6) {
//            throw invalid_argument("MethodMode required");
//        }
        Network network;
        //Input input(argv[1], argv[2], argv[3], boost::lexical_cast<Time>(argv[4]));
        Input input("airport.json","beacon.json", "flight.json");
        input.initNetwork(&network);
        double *somme_objectif = new double(0);
        double *maxConflictCount = new double(0);
        ApproximateFLA(&network, somme_objectif, maxConflictCount, true, 0,0);
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}