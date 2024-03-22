#include <string>
#include <iostream>
#include <vector>

// Define the entity class

class entity
{
private:
    int health;
    bool isAlive;
    double position[2];
    char facing;
    bool isAlive;

public:
    bool getIsAlive() { return isAlive; }
    void takeDamage(int damage) {
        health -= damage;
        death();
    }
    void death() {
        if (health <= 0) {
            isAlive = false;
        }
    }
};