//
// Created by chenghaowang on 22/05/17.
//

#ifndef NODE_H
#define NODE_H

#include "Position.h"
#include "Define.h"

/**
 * A Node is specified by an unique code, a name and a corresponding position.
 */
class Node {
public:
    Node(const Node &node){
        sCode = node.getCode();
        sName = node.getName();
        position = node.getPosition();
    }
    /**
     * Constructor with parameters.
     * @param sCode the point code.
     * @param sName  the point name.
     * @param position the point position.
     */
    Node(const String &sCode, const String &sName, const Position &position): sCode(sCode), sName(sName), position(position) {}

    /**
     * Constructor with parameters.
     * @param sCode the point code.
     * @param sName the point name.
     * @param dLatitude the point latitude.
     * @param dLongitude the point longitude.
     */
    Node(const String &sCode, const String &sName, double dLatitude, double dLongitude): sCode(sCode), sName(sName), position(dLatitude, dLongitude){}

    /**
     * Destructor.
     */
    virtual ~Node(){}

    /**
     * Getter for point code.
     * @return the code of corresponding point.
     */
    const String &getCode() const {
        return sCode;
    }

    /**
     * Getter for point name.
     * @return the name of corresponding point.
     */
    const String &getName() const {
        return sName;
    }

    /**
     * Getter for point position.
     * @return the position of corresponding point.
     */
    const Position &getPosition() const {
        return position;
    }

    /**
     * Override of == operand.
     * @param rhs the other compared Node.
     * @return true, if the two points have same code; false, otherwise.
     */
    bool operator==(const Node &rhs) const;

    /**
     * Override of != operand. see == operand override.
     * @param rhs the other compared Node.
     * @return true, if the two points don't have same code; false, otherwise.
     */
    bool operator!=(const Node &rhs) const;

    Node *clone(){
        return new Node(*this);
    }
private:
    /**
     * the point code.
     */
    String sCode;

    /**
     * the point name.
     */
    String sName;

    /**
     * the point position, see{@link Position}
     */
    Position position;
};
typedef Node WayPoint, Airport;
typedef std::vector<Node *> WayPointVector, AirportVector;
#endif //NODE_H
