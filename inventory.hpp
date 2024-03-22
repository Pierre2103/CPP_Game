#include "item.hpp"

class inventory {
private:
    item items[100];
    int numItems;

public:
    void addItem(item newItem) {
        items[numItems] = newItem;
        numItems++;
    }

    void consumeItem(item itemToUse) {
        if (itemToUse.getQuantity() > 0 && itemToUse.getIsUsable()) {
            itemToUse.useItem();
        }
    }

    void equipItem(item itemToEquip) {
        for (int i = 0; i < numItems; i++) {
            if (items[i].getItemName() == itemToEquip.getItemName()) {
                items[i].equipItem();
                break;
            }
        }
    }
};