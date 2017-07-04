#include "../include/Network.h"
#include "../include/Input.h"
#include "../include/Solution.h"

int main(int argc, char *argv[]) {
    using std::invalid_argument;
    try {
        if (argc < 2) {
            throw invalid_argument("airport file required");
        }
        else if (argc < 3) {
            throw invalid_argument("beacon file required");
        }
        else if (argc < 4) {
            throw invalid_argument("flight file required");
        }
        else if (argc < 5) {
            throw invalid_argument("configuration file required");
        }
        Network network;
        Input input(argv[1], argv[2], argv[3], argv[4]);
        input.initNetwork(&network);
        double *somme_objectif = new double(0);
        double *maxConflictCount = new double(0);
//        ApproximateFLA(&network, somme_objectif, maxConflictCount, 0);
        ApproximateFLA(&network, somme_objectif, maxConflictCount, 0, 0, 0, 0, 0, 0, 0);
    }
    catch (const invalid_argument &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}