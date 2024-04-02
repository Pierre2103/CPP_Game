#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>

bool isInventoryOpen = false;

void drawInventory(sf::RenderWindow& window) {
    sf::RectangleShape inventory(sf::Vector2f(200, 200));
    inventory.setFillColor(sf::Color::White);
    inventory.setOutlineColor(sf::Color::Black);
    inventory.setOutlineThickness(2);
    inventory.setPosition(300, 200);

    window.draw(inventory);
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Inventory Toggle Example");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // Check for key press event
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::I) {
                    // Toggle inventory only on key press
                    isInventoryOpen = !isInventoryOpen;

                    if (isInventoryOpen) {
                        std::cout << "Inventory is open" << std::endl;
                    } else {
                        std::cout << "Inventory is closed" << std::endl;
                    }
                }
            }
        }

        window.clear();
        // Draw the game scene here...

        if (isInventoryOpen) {
            drawInventory(window); // Call your inventory drawing function here
        }

        window.display();
    }

    return 0;
}
