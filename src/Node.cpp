//
// Created by chenghaowang on 22/05/17.
//

#include "../include/Node.h"

Node::Node(const Node &node):sCode(node.getCode()), sName(node.getName()), position(node.getPosition()) {}

Node::Node(const String &sCode, const String &sName, const Position &position) :
        sCode(sCode), sName(sName), position(position) {}

Node::Node(const String &sCode, const String &sName, double dLatitude, double dLongitude):
        sCode(sCode), sName(sName), position(dLatitude, dLongitude){}

Node::~Node() {}

const String &Node::getCode() const {
    return sCode;
}

const String &Node::getName() const {
    return sName;
}

const Position &Node::getPosition() const {
    return position;
}

bool Node::operator==(const Node &rhs) const {
    return sCode == rhs.getCode();
}

bool Node::operator!=(const Node &rhs) const {
    return !(rhs == *this);
}

Node *Node::clone() {
    return new Node(*this);
}

std::ostream &operator<<(std::ostream &os, const Node &node) {
    os << "sCode: " << node.sCode << " sName: " << node.sName << " position: " << node.position << std::endl;
    return os;
}



