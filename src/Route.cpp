//
// Created by chenghaowang on 23/05/17.
//

#include "../include/Route.h"

Route::Route(void){}

Route::Route(const Route &route): vpPointsList(route.getPointsList()){}

Route::~Route() {

}

void Route::addNewPoint(Point *pPoint) {
    if(pPoint == nullptr) {
        std::cout << "at: void Route::addNewPoint(Point *pPoint)" << std::endl
                  << "warning: Point is null, the insertion is ignored automatically." << std::endl;
    }
//    else if(findByCode<Point>(vpPointsList, pPoint->getCode())) {
//        std::cout << "at: void Route::addNewPoint(Point *pPoint)" << std::endl
//                  << "warning: Point has been in the list, the insertion is ignored automatically." << std::endl;
//    }
    else {
        vpPointsList.push_back(pPoint);
    }
}

const PointVector &Route::getPointsList() const {
    return vpPointsList;
}

Point *Route::getPointAtI(int index) {
    if (index < 0 || index > (int)vpPointsList.size()){
        std::cerr << "fatal error! the index of queried point not valid!" << std::endl;
        abort();
    }
    return vpPointsList[index];
}

bool Route::operator==(const Route &rhs) const {
    return vpPointsList == rhs.vpPointsList;
}

bool Route::operator!=(const Route &rhs) const {
    return !(rhs == *this);
}

Route *Route::clone() {
    return new Route(*this);
}
std::ostream &operator<<(std::ostream &os, const PointVector &vpPointsList) {
    for(auto &&item : vpPointsList){
        os << *item << " ";
    }
    return os;
}


std::ostream &operator<<(std::ostream &os, const Route &route) {
    os << " vpPointsList(NodeCode, ArrivingTime): " << route.vpPointsList << std::endl;
    return os;
}


