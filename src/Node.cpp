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


