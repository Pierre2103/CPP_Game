#include "entity.hpp"

class animal : public entity
{
public:
    void dropLoot() {
        if (entity::getIsAlive() == false) {
            std::cout << "Dropped loot!" << std::endl;
            // TODO: Implement loot dropping
        }
    }
};