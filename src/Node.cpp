//
// Created by chenghaowang on 22/05/17.
//

#include "../include/Node.h"
bool Node::operator==(const Node &rhs) const {
    return sCode == rhs.getCode();
}

bool Node::operator!=(const Node &rhs) const {
    return !(rhs == *this);
}

Node::Node(const String &sCode, const String &sName, const Position &position) : sCode(sCode), sName(sName),
                                                                                 position(position) {}

Node::Node(const String &sCode, const String &sName, double dLatitude, double dLongitude) : sCode(sCode), sName(sName),
                                                                                            position(dLatitude,
                                                                                                     dLongitude) {}

Node::~Node() {}

const String &Node::getCode() const {
    return sCode;
}

const Position &Node::getPosition() const {
    return position;
}


