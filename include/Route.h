//
// Created by chenghaowang on 23/05/17.
//

#ifndef ROUTE_H
#define ROUTE_H

#include "Define.h"
#include "NodePoint.h"

class Route {
public:
    Route(int iDefaultLevel, Airport *pAirOrigin, Time iDepartureTime): iDefaultLevel(iDefaultLevel), vpPointsList(){
        vpPointsList.push_back(new Point(pAirOrigin, iDepartureTime));
    }

    Route(Route &route): iDefaultLevel(route.getLevel()), vpPointsList(route.getPointsList()) {}

    /**
     * Destructor.
     */
    virtual ~Route() {
        for(auto&& point: vpPointsList){
            delete point;
        }
        vpPointsList.clear();
    }

    /**
     * Add a new point into route.
     * @param pPoint the new added point.
     */
    void addNewPoint(Point *pPoint){
        vpPointsList.push_back(pPoint);
    }

    const int getLevel(void) const{
        return iDefaultLevel;
    }

    /**
     * Getter for PointsList
     * @return the corresponding points that the route will pass.
     */
    const PointVector &getPointsList() const {
        return vpPointsList;
    }



    /**
     * Get the ith Point in the points list.
     * @param iIndex the index of queried Point.
     * @return the corresponding point.
     */
    Point *getPointAtI(int iIndex){
        return vpPointsList[iIndex];
    }

    void setArrivingTimeAtPoint(int iIndex, Time iArrivingTime){
        vpPointsList[iIndex]->setArrivingTime(iArrivingTime);
    }

    Time getArrivingTimeAtPoint(int iIndex){
        return vpPointsList[iIndex]->getArrivingTime();
    }

    const Position &getPositionAtPoint(int iIndex){
        return vpPointsList[iIndex]->getPosition();
    }

    int getPointListSize(){
        return (int)vpPointsList.size();
    }

    double getVelocityFromPoint(int iIndexPoint) {
        //Calculate the velocity, and it varies between -1% and 1%.
        if (iIndexPoint == 0) {
            return -1;
        }
        double dDeltaT = getArrivingTimeAtPoint(iIndexPoint) - getArrivingTimeAtPoint(iIndexPoint - 1);
        double dDistance = distanceBetween(getPositionAtPoint(iIndexPoint), getPositionAtPoint(iIndexPoint));
        if (dDeltaT != 0) {
            return dDistance / dDeltaT;
        }
        std::cerr << "[Fatal Error] the arriving time between two consecutive points is 0." << std::endl;
        return -1;
    }

    Time getFinalArrivingTime(){
        return getArrivingTimeAtPoint((int)vpPointsList.size()-1);
    }

    Route* update(double iDepartureTime){
        Route *newRoute = this->clone();
        double velocity = newRoute->getVelocityFromPoint(1);
        normal_dist NormalDistribution(velocity, velocity/300);
        velocity = NormalDistribution(generator);
        newRoute->setArrivingTimeAtPoint(0, (Time)ceil(iDepartureTime));
        for (int i=1; i < newRoute->getPointListSize(); i++){
            double dDistance = distanceBetween(newRoute->getPointAtI(i)->getPosition(), newRoute->getPointAtI(i-1)->getPosition());
            if (velocity > 0){
                double dDeltaT = dDistance / velocity;
                newRoute->setArrivingTimeAtPoint(i, (Time)ceil(getArrivingTimeAtPoint(i-1)+dDeltaT));
            }else{
                delete newRoute;
                return nullptr;
            }
        }
        return newRoute;
    }

    double probabilityConflict(int iIndex1, Route *pRoute2, int iIndex2);
    double probabilityConflict2(int iIndex1, Route *pRoute2, int iIndex2);
    double delayWithoutConflict(int iIndex1, Route *pRoute2, int iIndex2, int *ind);
    double delayWithoutConflict2(int iIndex1, Route *pRoute2, int iIndex2, int *ind);
    Route* clone(){
        return new Route(*this);
    }
private:
    /**
     * each route have a constant altitude.
     */
    int iDefaultLevel;

    /**
     * the point passed in the route.
     */
    PointVector vpPointsList;
};
//typedef std::vector<Route *> RouteVector;
#endif //ROUTE_H
