//
// Created by chenghaowang on 22/05/17.
//

#ifndef NODE_H
#define NODE_H

#include "Position.h"
#include "Configuration.h"
/**
 * A Node is specified by an unique code, a name and a corresponding position.
 */
class Node {
public:
    /**
     * Constructor with parameters.
     * @param sCode         The point code
     * @param sName         The point name
     * @param position      The point position
     */
    Node(const String &sCode, const String &sName, const Position &position);

    /**
     * Constructor with parameters.
     * @param sCode             The point code
     * @param sName             The point name
     * @param dLatitude         The point latitude
     * @param dLongitude        The point longitude
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
     * Getter for point position.
     * @return the position of corresponding point.
     */
    const Position &getPosition() const;

    /**
     * Override of == operand.
     * @param rhs       The other compared Node
     * @return true, if the two points have same code; false, otherwise.
     */
    bool operator==(const Node &rhs) const;

    /**
     * Override of != operand. see == operand override
     * @param rhs       The other compared Node
     * @return true, if the two points don't have same code; false, otherwise.
     */
    bool operator!=(const Node &rhs) const;

private:
    /**
     * The point code.
     */
    String sCode;

    /**
     * The point name.
     */
    String sName;

    /**
     * The point position, see{@link Position}.
     */
    Position position;
};

typedef Node WayPoint, Airport;
typedef std::vector<Node *> WayPointVector, AirportVector;
#endif //NODE_H
