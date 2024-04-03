#include "global.hpp"

using namespace std;

class item {
private:
    bool isUsable;
    bool isEquippable;
    int quantity;
    string itemName;

public:
    void equipItem() {
        if (isEquippable) {
            // Equip the item
        }
    }

    void useItem() {
        if (isUsable) {
            quantity--;
            // Use the item
        }
    }

    bool getIsUsable() {
        return isUsable;
    }

    string getItemName() {
        return itemName;
    }

    int getQuantity() {
        return quantity;
    }
};