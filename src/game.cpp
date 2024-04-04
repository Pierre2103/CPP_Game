#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <iostream>
#include "../lib/map.hpp"

const int windowWidth = 1920;
const int windowHeight = 1080;
const int speed = 3;
const int barWidth = 50; // Width of each bar
const int barHeight = 5; // Height of each bar

// Colors for the bars:
// Mix between pink and red for the health bar
// Light blue for thirst
// Orange for food
const sf::Color healthColor(136, 8, 8, 220);
const sf::Color thirstColor(36, 157, 159, 220);
const sf::Color foodColor(255, 165, 0, 220);

const sf::Color inventoryColor(0, 0, 0, 200);

int mapCenterX = mapWidth * tileSize / 2;
int mapCenterY = mapHeight * tileSize / 2;

int alternate = 0;

bool isInventoryOpen = false;

bool bridge1Crafted = false;
bool bridge2Crafted = false;
bool bridge3Crafted = false;
bool bridge4Crafted = false;

// inventory logic here
struct Item
{
    std::string name;
    int quantity;
};

std::vector<Item> inventory;

float inventoryX = 0;
float inventoryY = 0;
float inventoryWidth = 0;
float inventoryHeight = 0;

// Enumeration for the terrain types.
enum Terrain
{
    Grass,
    Water,
    Sand,
    Rock,
    Tree,
    Firecamp,
    Workbench,
    House,
    Plantation,
    AnimalEnclosures,
    Blackbox,
    ShelterWalls,
    AnimalSpawner,
    PlayerSpawn,
    Wood,
    NumTerrains
};

// Function to draw a colored bar
void drawBar(sf::RenderWindow &window, float x, float y, float width, float height, float fillRatio, sf::Color color)
{
    // Draw black outline
    sf::RectangleShape outline(sf::Vector2f(width, height));
    outline.setPosition(x, y);
    outline.setFillColor(sf::Color::Transparent);
    outline.setOutlineThickness(1); // Adjust outline thickness as needed
    outline.setOutlineColor(sf::Color::Black);
    window.draw(outline);

    // Draw filled bar
    sf::RectangleShape bar(sf::Vector2f(width * fillRatio, height));
    bar.setPosition(x, y);
    bar.setFillColor(color);
    window.draw(bar);
};

// draw a big rectangle for the inventory just for testing
void drawInventory(sf::RenderWindow &window, float x, float y, float width, float height, sf::Color color, sf::Font &font)
{
    sf::View originalView = window.getView();

    sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(uiView);

    sf::RectangleShape inventoryBackground(sf::Vector2f(width, height));
    inventoryBackground.setPosition(x, y);
    inventoryBackground.setFillColor(color);
    window.draw(inventoryBackground);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    // if item is wood, display wood icon (assets/item_wood.png) use a for loop to display icon

    for (size_t i = 0; i < inventory.size(); ++i)
    {
        // add the item icon here
        text.setString(inventory[i].name + " : " + std::to_string(inventory[i].quantity));
        text.setPosition(std::round(x + 10), std::round(y + 10 + i * 30));
        window.draw(text);

        switch (i)
        {
        case 0:
            sf::Texture itemTexture1;
            // set the size at 32x32
            itemTexture1.loadFromFile("assets/item_wood.png");
            sf::Sprite itemSprite1(itemTexture1);
            itemSprite1.setPosition(x + 10, y + 10 + i * 30);
            window.draw(itemSprite1);
            break;
        }
    }

    window.setView(originalView);
}

// for test only /!\ to remove
void fillInventory()
{
    Item item1;
    item1.name = "apple";
    item1.quantity = 5;
    inventory.push_back(item1);

    Item item2;
    item2.name = "wood";
    item2.quantity = 9;
    inventory.push_back(item2);

    Item item3;
    item3.name = "water bottle";
    item3.quantity = 1;
    inventory.push_back(item3);
};

void displayCaptionText(sf::RenderWindow &window, sf::Font &font, std::string captionText, sf::Vector2f position)
{
// put the text in a view
    sf::View originalView = window.getView();

    sf::View uiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    window.setView(uiView);

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);
    text.setString(captionText);
    text.setPosition(position);
    window.draw(text);

    window.setView(originalView);
};

//to call it: 

int main()
{
    fillInventory();

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Night 4 Life");

    // Load icons
    sf::Texture hungerIconTexture;
    hungerIconTexture.loadFromFile("assets/hunger.png");
    sf::Sprite hungerIcon(hungerIconTexture);
    hungerIcon.setScale(0.5f, 0.5f); // Reduce icon size

    sf::Texture lifeIconTexture;
    lifeIconTexture.loadFromFile("assets/life.png");
    sf::Sprite lifeIcon(lifeIconTexture);
    lifeIcon.setScale(0.5f, 0.5f); // Reduce icon size

    sf::Texture thirstIconTexture;
    thirstIconTexture.loadFromFile("assets/thirst.png");
    sf::Sprite thirstIcon(thirstIconTexture);
    thirstIcon.setScale(0.5f, 0.5f); // Reduce icon size

    std::vector<sf::Texture> textures(Terrain::NumTerrains);
    textures[Grass].loadFromFile("assets/grass.png");
    textures[Water].loadFromFile("assets/water.png");
    textures[Sand].loadFromFile("assets/sand.png");
    textures[Rock].loadFromFile("assets/stone.png");
    textures[Tree].loadFromFile("assets/tree_1.png");
    textures[Firecamp].loadFromFile("assets/fire_1.png");
    textures[Workbench].loadFromFile("assets/no_texture.png");
    textures[House].loadFromFile("assets/no_texture.png");
    textures[Plantation].loadFromFile("assets/no_texture.png");
    textures[AnimalEnclosures].loadFromFile("assets/no_texture.png");
    textures[Blackbox].loadFromFile("assets/no_texture.png");
    textures[ShelterWalls].loadFromFile("assets/no_texture.png");
    textures[AnimalSpawner].loadFromFile("assets/no_texture.png");
    textures[PlayerSpawn].loadFromFile("assets/PlayerSpawn.png");
    textures[Wood].loadFromFile("assets/wood.png");

    std::vector<sf::Sprite> terrainSprites(Terrain::NumTerrains);
    for (int i = 0; i < Terrain::NumTerrains; ++i)
    {
        terrainSprites[i].setTexture(textures[i]);
    };

    float zoomLevel = 8.0f;

    // sf::Vector2f playerPosition(158 * tileSize, 28 * tileSize);
    sf::Vector2f playerPosition(158 * tileSize, 28 * tileSize);

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/player_down_0.png");
    sf::Sprite playerSprite(playerTexture);

    // Variables for player stats
    int playerHealth = 1000;
    int playerThirst = 2000;
    int playerFood = 4000;
    int maxHealth = 1000;
    int maxThirst = 2000;
    int maxFood = 4000;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            };

            // handle key press event
            if (event.type == sf::Event::KeyPressed)
            {
                // Check if the key pressed is 'I'
                if (event.key.code == sf::Keyboard::I)
                {
                    // Toggle inventory only on key press
                    isInventoryOpen = !isInventoryOpen;
                };
            };
        };

        // Movement vector.
        sf::Vector2f movement(0, 0);

        int currentTileX = playerPosition.x / tileSize;
        int currentTileY = playerPosition.y / tileSize;
        int currentTileType = tilemap[currentTileY][currentTileX];

        // Check if the key pressed is 'Z', 'W', or 'Up'
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) // Move up
        {
            movement.y -= speed;
            switch (alternate)
            {
            case 0:
                playerTexture.loadFromFile("assets/player_up_1.png");
                break;
            case 3:
                playerTexture.loadFromFile("assets/player_up_2.png");
                break;
            default:
                break;
            };
        }

        // Check if the key pressed is 'Q', 'A', or 'Left'
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) // Move Left
        {
            movement.x -= speed;
            switch (alternate)
            {
            case 0:
                playerTexture.loadFromFile("assets/player_left_1.png");
                break;
            case 3:
                playerTexture.loadFromFile("assets/player_left_2.png");
                break;
            default:
                break;
            };
        }

        // Check if the key pressed is 'S', or 'Down'
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) // Move Down
        {
            movement.y += speed;
            switch (alternate)
            {
            case 0:
                playerTexture.loadFromFile("assets/player_down_1.png");
                break;
            case 3:
                playerTexture.loadFromFile("assets/player_down_2.png");
                break;
            default:
                break;
            };
        }

        // Check if the key pressed is 'D', or 'Right'
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) // Move Right
        {
            movement.x += speed;
            switch (alternate)
            {
            case 0:
                playerTexture.loadFromFile("assets/player_right_1.png");
                break;
            case 3:
                playerTexture.loadFromFile("assets/player_right_2.png");
                break;
            default:
                break;
            };
        };

        // Check if the current tile is sand and adjust speed accordingly.
        if (currentTileType == Sand)
        {
            // Reduce the speed on sand tiles.
            movement *= 0.65f; // You can adjust the factor as needed.
        };

        // Decrease thirst and hunger when player walks
        if (movement.x != 0 || movement.y != 0)
        {
            playerThirst -= 0.0001f; // Decrease thirst by a smaller value
            playerFood -= 0.001f;    // Decrease hunger by a larger value
        };

        // Decrease health if either thirst or hunger is zero
        if (playerThirst <= 0 || playerFood <= 0)
        {
            playerHealth -= 0.001f;
        };

        // End the game if player health is zero by closing the window and displaying a game over message in ASCII art
        if (playerHealth <= 0)
        {
            window.close();
            std::cout << "  _____          __  __ ______    ______      ________ _____  " << std::endl;
            std::cout << " / ____|   /\\   |  \\/  |  ____|  / __ \\ \\    / /  ____|  __ \\ " << std::endl;
            std::cout << "| |  __   /  \\  | \\  / | |__    | |  | \\ \\  / /| |__  | |__) |" << std::endl;
            std::cout << "| | |_ | / /\\ \\ | |\\/| |  __|   | |  | |\\ \\/ / |  __| |  _  / " << std::endl;
            std::cout << "| |__| |/ ____ \\| |  | | |____  | |__| | \\  /  | |____| | \\ \\ " << std::endl;
            std::cout << " \\_____/_/    \\_\\_|  |_|______|  \\____/   \\/   |______|_|  \\_\\ " << std::endl;
        }

        // Ensure that playerThirst, playerFood, and playerHealth don't go below 0
        if (playerThirst < 0)
            playerThirst = 0;
        if (playerFood < 0)
            playerFood = 0;
        if (playerHealth < 0)
            playerHealth = 0;

        alternate = (alternate + 1) % 6;

        playerSprite.setTexture(playerTexture);

        sf::Vector2f newPosition = playerPosition + movement;

        if (newPosition.x >= 0 && newPosition.x < mapWidth * tileSize &&
            newPosition.y >= 0 && newPosition.y < mapHeight * tileSize)
        {
            // Check if the new position is on a water tile (assuming water tiles are represented by value 1)
            int tileX = newPosition.x / tileSize;
            int tileY = newPosition.y / tileSize;
            // if (tilemap[tileY][tileX] != Water)
            // {
            // Move the player to the new position
            playerPosition = newPosition;
            playerSprite.setPosition(playerPosition);
            // };
        };

        sf::Vector2f viewSize(windowWidth / zoomLevel, windowHeight / zoomLevel);

        sf::View view(playerPosition, viewSize);

        window.setView(view);

        window.clear();

        // Draw the terrain sprites with trees layered over grass
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int terrainType = tilemap[y][x];
                if (terrainType >= 0 && terrainType < Terrain::NumTerrains)
                {
                    // If the terrain is a tree or a firecamp, draw the grass first
                    if (terrainType == Tree || terrainType == Firecamp)
                    {
                        int grassType = Grass; // Assuming grass is represented by 0
                        terrainSprites[grassType].setPosition(x * tileSize, y * tileSize);
                        window.draw(terrainSprites[grassType]);
                    };
                    terrainSprites[terrainType].setPosition(x * tileSize, y * tileSize);
                    window.draw(terrainSprites[terrainType]);
                };
            };
        };

        // Update the positions of the bars and icons to follow the player
        float barX = playerPosition.x - windowWidth / (6 * zoomLevel) - 65;
        float barY = playerPosition.y - windowHeight / (6 * zoomLevel) - 40;
        float iconX = barX - 10; // Adjust the distance between the icon and the bar as needed

        // Draw player sprite, icons, and bars
        window.draw(playerSprite);

        // Draw icons
        lifeIcon.setPosition(iconX, barY - 2);
        window.draw(lifeIcon);

        thirstIcon.setPosition(iconX, barY - 2 + barHeight + 5);
        window.draw(thirstIcon);

        hungerIcon.setPosition(iconX, barY - 2 + 2 * (barHeight + 5));
        window.draw(hungerIcon);

        // Draw bars
        drawBar(window, barX, barY, barWidth, barHeight, static_cast<float>(playerHealth) / maxHealth, healthColor);
        drawBar(window, barX, barY + barHeight + 5, barWidth, barHeight, static_cast<float>(playerThirst) / maxThirst, thirstColor);
        drawBar(window, barX, barY + 2 * (barHeight + 5), barWidth, barHeight, static_cast<float>(playerFood) / maxFood, foodColor);

        // float inventoryX_ = playerPosition.x - windowWidth / (6 * zoomLevel) - 66;
        // float inventoryY_ = playerPosition.y - windowHeight / (6 * zoomLevel) - 10;

        float inventoryX_ = 37;
        float inventoryY_ = 206;
        float inventoryWidth_ = 360;
        float inventoryHeight_ = 625;

        inventoryX = inventoryX_;
        inventoryY = inventoryY_;
        inventoryWidth = inventoryWidth_;
        inventoryHeight = inventoryHeight_;

        sf::Font font;
        if (!font.loadFromFile("assets/REM-Medium.ttf"))
        {
            std::cout << "Error loading font" << std::endl;
            return -1;
        }

        // Draw the inventory
        if (isInventoryOpen)
        {
            drawInventory(window, inventoryX, inventoryY, inventoryWidth, inventoryHeight, inventoryColor, font);
        }
        else
        {
            // std::cout << "Inventory is closed" << std::endl;
        };

        // function to display the tiles coordinates
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setString("X: " + std::to_string(currentTileX) + " Y: " + std::to_string(currentTileY));
        text.setPosition(playerPosition.x - 50, playerPosition.y - 50);
        window.draw(text);

        // ======
        // Beginning of bridge 1 crafting function
        // ======

        if (currentTileX >= 182 && currentTileX <= 187 && currentTileY >= 95 && currentTileY <= 99) //trigger zone
        {
            if (!bridge1Crafted)
            {
                displayCaptionText(window, font, "Press 'E' to craft the bridge to the second island", sf::Vector2f(475, 750));

                // if the player presses 'E' and has 10 wood in their inventory
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    // check if the player has 10 wood in their inventory
                    bool hasWood = false;
                    for (size_t i = 0; i < inventory.size(); ++i)
                    {
                        if (inventory[i].name == "wood" && inventory[i].quantity >= 10)
                        {
                            hasWood = true;
                            break;
                        }
                    }

                    // if the player has 10 wood in their inventory
                    if (hasWood)
                    {
                        // remove 10 wood from the inventory
                        for (size_t i = 0; i < inventory.size(); ++i)
                        {
                            if (inventory[i].name == "wood")
                            {
                                inventory[i].quantity -= 10;
                                if (inventory[i].quantity == 0)
                                {
                                    inventory.erase(inventory.begin() + i);
                                }
                                break;
                            }
                        }

                        // replace the water tiles with wood tiles
                        for (int y = 98; y <= 103; ++y)
                        {
                            for (int x = 184; x <= 186; ++x)
                            {
                                tilemap[y][x] = Wood;
                            }
                        }

                        displayCaptionText(window, font, "Bridge has been crafted", sf::Vector2f(475, 750));
                        bridge1Crafted = true;
                    }
                    else
                    {
                        // std::cout << "You need 10 wood to craft a bridge" << std::endl;
                        // display this text to the screen
                        displayCaptionText(window, font, "You need 10 wood to craft a bridge", sf::Vector2f(515, 790));

                    }
                }
            }
            else
            {
                // std::cout << "Bridge 1 has already been crafted" << std::endl;
            }
        };
        // ======
        // End of bridge 1 crafting function
        // ======

        // Display the contents of the window
        window.display();
    }

    return 0;
}