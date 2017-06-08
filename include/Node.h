//
// Created by chenghaowang on 22/05/17.
//

#ifndef ROBUSTFLA_POINT_H
#define ROBUSTFLA_POINT_H

#include <ostream>
#include "predefine.h"
#include "Position.h"
/**
 * A Node is specified by an unique code, a name and a corresponding position.
 */
class Node {
public:
    /**
     * Copy Contructor.
     * @param node see{@link Node}.
     */
    Node(const Node &node);

    /**
     * Constructor with parameters.
     * @param sCode the point code.
     * @param sName  the point name.
     * @param position the point position.
     */
    Node(const String &sCode, const String &sName, const Position &position);

    /**
     * Constructor with parameters.
     * @param sCode the point code.
     * @param sName the point name.
     * @param dLatitude the point latitude.
     * @param dLongitude the point longitude.
     */
    Node(const String &sCode, const String &sName, double dLatitude, double dLongitude);

    /**
     * Destructor.
     */
    virtual ~Node();

    /**
     * Getter for point code.
     * @return the code of corresponding point.
     */
    const String &getCode() const;

    /**
     * Getter for point name.
     * @return the name of corresponding point.
     */
    const String &getName() const;

    /**
     * Getter for point position.
     * @return the position of corresponding point.
     */
    const Position &getPosition() const;

    /**
     * override of == operand.
     * @param rhs the other compared Node.
     * @return true, if the two points have same code; false, otherwise.
     */
    bool operator==(const Node &rhs) const;

    /**
     * override of != operand. see == operand override.
     * @param rhs the other compared Node.
     * @return true, if the two points don't have same code; false, otherwise.
     */
    bool operator!=(const Node &rhs) const;

    /**
     * Clone the object itself.
     * @return the cloned object.
     */
    Node *clone();

    friend std::ostream &operator<<(std::ostream &os, const Node &node);

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
#endif //ROBUSTFLA_POINT_H
