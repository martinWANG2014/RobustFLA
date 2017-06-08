//
// Created by chenghaowang on 23/05/17.
//

#ifndef ROBUSTFLA_PREDEFINE_H
#define ROBUSTFLA_PREDEFINE_H

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <random>
#include <algorithm>

typedef std::string String;
typedef int Time;
typedef std::vector<int> IntVector;

template<typename T> T* findByCode(std::vector<T*> vpObjectsVector, const String &sCode){
    if (vpObjectsVector.size() < 1) return nullptr;
    for (auto &&item : vpObjectsVector) {
        if ( item->getCode() == sCode) {
            return item;
        }
    }
    return nullptr;
}

template<typename T> bool contains(std::vector<T*> vpObjectsVector, T* pObject){
    if (vpObjectsVector.size() < 1) return false;
    for (auto &&item : vpObjectsVector) {
        if (*item == *pObject) {
            return true;
        }
    }
    return false;
//    return std::find(vpObjectsVector.begin(), vpObjectsVector.end(), pObject) != vpObjectsVector.end();
}
template<typename T> bool contains(std::vector<T> vpObjectsVector, T object){
    if (vpObjectsVector.size() < 1) return false;
    for (auto &&item : vpObjectsVector) {
        if (item == object) {
            return true;
        }
    }
    return false;
//    return std::find(vpObjectsVector.begin(), vpObjectsVector.end(), pObject) != vpObjectsVector.end();
}
const double k = 1.8520108;              // constant : 1(nmi)=k*1(km)
const double min_separation = 5.0;
const double PIx = 3.141592653589793;
const double RADIO = 6378.16;
const double EPSILON = 1.0e-10; // precision of double variables
const int ITMAX = 1000;

#endif //ROBUSTFLA_PREDEFINE_H
