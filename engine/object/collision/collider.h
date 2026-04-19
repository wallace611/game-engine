#include "object/object.h"

class Collider : public Object {
public:
    static bool IsCollided(Collider collider1, Collider collider2);
};