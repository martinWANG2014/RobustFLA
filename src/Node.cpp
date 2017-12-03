//
// Created by chenghaowang on 22/05/17.
//

#include "../include/Node.h"
bool Node::operator==(const Node &rhs) const {
//    std::cout << sCode << ":" << rhs.getCode() << std::endl;
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

Node *Node::clone() {
    return new Node(sCode, sName, position);
}

std::ostream &operator<<(std::ostream &os, const Node &node) {
    os << "sCode: " << node.sCode;
    return os;
}


