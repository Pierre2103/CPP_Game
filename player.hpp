#include "entity.hpp"

class player : public entity
{
private:
    int hunger;
    int thirst;
    int level;
    bool isMoving;

public:
    void checkThirstAndHunger() {
        if (hunger <= 0 || thirst <= 0) {
            player::takeDamage(2);
        }
    }

    // TODO: Implement the following functions
    void openInventory() {
    }

    void move() {
    }

    void checkQuests() {
    }

    void levelUp() {
    }

    void interact() {
    }
};