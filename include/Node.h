//
// Created by chenghaowang on 22/05/17.
//

#ifndef NODE_H
#define NODE_H

#include "common.h"
#include "Position.h"

/**
 * A Node is specified by an unique code, a name and a corresponding position.
 */
class Node {
public:
    /**
     * Copy Constructor.
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
    Position &getPosition() const;

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

    /**
     * Clone the object itself.
     * @return the cloned object.
     */
    Node *clone();

    /**
     * Override the output stream to print the Node object.
     * @param os the standard output stream.
     * @param node the Node object.
     * @return redirected output stream.
     */
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
#endif //NODE_H
