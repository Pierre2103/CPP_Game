#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <iostream>
#include "../lib/map.hpp"

const int windowWidth = 1920;
const int windowHeight = 1080;
const int speed = 4;    // standard is 4
const int barWidth = 50; // Width of each bar
const int barHeight = 5; // Height of each bar

// Colors for the bars
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

struct Animal {
    sf::Sprite sprite;
    int frame = 0;
    int direction = -1; // Initialize to an invalid value to indicate no direction is currently chosen
    int iteration = 0;
    static const int animalSpeed = 2; // Consider using a float for smoother movement
    static const int moveDistance = 10; // Distance to move in one direction
};

std::vector<Animal> animals;
std::vector<sf::Texture> textures(6); // For 6 types of animals

// Inventory logic
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
    Tree1,
    Tree2,
    Tree3,
    Tree4,
    Firecamp,
    Workbench,
    House,
    Lava,
    Blackbox,
    ShelterWalls,
    AnimalSpawner,
    Wood,
    PlayerSpawn,
    Magma,
    Chest,
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

    for (size_t i = 0; i < inventory.size(); ++i)
    {
        text.setString(inventory[i].name + " : " + std::to_string(inventory[i].quantity));
        text.setPosition(std::round(x + 10), std::round(y + 10 + i * 30));
        window.draw(text);
    }

    // Display controls for eating and drinking
    text.setString("Press 'F' to eat, 'G' to drink");
    text.setPosition(x + 10, y + height - 60);
    window.draw(text);

    text.setString("Press 'I' to close inventory");
    text.setPosition(x + 10, y + height - 30);
    window.draw(text);

    window.setView(originalView);
}

void fillInventory()
{
    Item fruit;
    fruit.name = "Fruit";
    fruit.quantity = 0;
    inventory.push_back(fruit);

    Item wood;
    wood.name = "Wood";
    wood.quantity = 0;
    inventory.push_back(wood);

    Item water;
    water.name = "Water bottle";
    water.quantity = 0;
    inventory.push_back(water);
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

void loadAnimalsTextures() {
    textures[0].loadFromFile("assets/animal_chicken.png");
    textures[1].loadFromFile("assets/animal_pig.png");
    textures[2].loadFromFile("assets/animal_cow.png");
    textures[3].loadFromFile("assets/animal_sheep.png");
    textures[4].loadFromFile("assets/animal_goose.png");
    textures[5].loadFromFile("assets/animal_goat.png");
}

void initializeAnimals() {
    std::array<std::pair<int, int>, 60> spawnPoints = {
        std::make_pair(160, 30), std::make_pair(175, 35), std::make_pair(185, 40), std::make_pair(205, 35), std::make_pair(220, 20), std::make_pair(215, 70), std::make_pair(205, 75), std::make_pair(210, 95), std::make_pair(195, 95), std::make_pair(190, 85), std::make_pair(180, 80), std::make_pair(190, 75), std::make_pair(190, 65), std::make_pair(160, 60), std::make_pair(175, 80), std::make_pair(185, 95), std::make_pair(200, 80), std::make_pair(210, 70), std::make_pair(210, 90), std::make_pair(200, 105), std::make_pair(210, 140), std::make_pair(200, 150), std::make_pair(185, 135), std::make_pair(175, 140), std::make_pair(160, 120), std::make_pair(175, 115), std::make_pair(185, 125), std::make_pair(200, 120), std::make_pair(200, 200), std::make_pair(190, 215), std::make_pair(175, 200), std::make_pair(165, 200), std::make_pair(165, 190), std::make_pair(165, 175), std::make_pair(150, 165), std::make_pair(140, 160), std::make_pair(140, 145), std::make_pair(150, 130), std::make_pair(160, 120), std::make_pair(160, 140), std::make_pair(40, 30), std::make_pair(50, 35), std::make_pair(60, 40), std::make_pair(70, 45), std::make_pair(85, 40), std::make_pair(100, 40), std::make_pair(110, 30), std::make_pair(120, 40), std::make_pair(140, 50), std::make_pair(130, 80), std::make_pair(120, 100), std::make_pair(130, 100),
    };
    
    // Example loop to initialize animals
    for (int i = 0; i < spawnPoints.size(); ++i) {
        Animal animal;
        animal.sprite.setTexture(textures[i % 6]); // Cycle through 6 animal types
        animal.sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
        animal.sprite.setScale(0.7f, 0.7f);
        animal.sprite.setPosition(spawnPoints[i].first * tileSize, spawnPoints[i].second * tileSize);
        animals.push_back(animal);
    }
}

void updateAnimals() {
    static sf::Clock animalClock;
    if (animalClock.getElapsedTime().asSeconds() >= 0.25f) {
        for (auto& animal : animals) {
            // Update the animation frame
            animal.frame = (animal.frame + 1) % 4;
            animal.sprite.setTextureRect(sf::IntRect(animal.frame * 16, 0, 16, 16));

            // Assuming tileSize and tilemap are accessible here
            int x = static_cast<int>(animal.sprite.getPosition().x) / tileSize;
            int y = static_cast<int>(animal.sprite.getPosition().y) / tileSize;

            // Update direction and movement
            if (animal.iteration >= animal.moveDistance) {
                animal.direction = rand() % 4;
                animal.iteration = 0; // Reset for the next movement
            }

            if (animal.iteration < animal.moveDistance) {
                if (animal.direction == 0 && y > 0 && tilemap[y - 1][x] != Water) {
                    animal.sprite.move(0, -animal.animalSpeed);
                } else if (animal.direction == 1 && y < (mapHeight - 1) && tilemap[y + 1][x] != Water) {
                    animal.sprite.move(0, animal.animalSpeed);
                } else if (animal.direction == 2 && x > 0 && tilemap[y][x - 1] != Water) {
                    animal.sprite.move(-animal.animalSpeed, 0);
                } else if (animal.direction == 3 && x < (mapWidth - 1) && tilemap[y][x + 1] != Water) {
                    animal.sprite.move(animal.animalSpeed, 0);
                }
                animal.iteration++; // Increment the iteration each frame the animal moves
            }
        }
        animalClock.restart();
    }
}

void drawAnimals(sf::RenderWindow &window) {
    for (const auto& animal : animals) {
        window.draw(animal.sprite);
    }
}

// to call it:

int main()
{
    loadAnimalsTextures();
    initializeAnimals();
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
    textures[Tree1].loadFromFile("assets/tree_1.png");
    textures[Tree2].loadFromFile("assets/tree_2.png");
    textures[Tree3].loadFromFile("assets/tree_3.png");
    textures[Tree4].loadFromFile("assets/tree_4.png");
    textures[Firecamp].loadFromFile("assets/fire_1.png");
    textures[Workbench].loadFromFile("assets/no_texture.png");
    textures[House].loadFromFile("assets/no_texture.png");
    textures[Lava].loadFromFile("assets/lava.png");
    textures[Blackbox].loadFromFile("assets/no_texture.png");
    textures[ShelterWalls].loadFromFile("assets/fence.png");
    textures[AnimalSpawner].loadFromFile("assets/no_texture.png");
    textures[PlayerSpawn].loadFromFile("assets/PlayerSpawn.png");
    textures[Wood].loadFromFile("assets/wood.png");
    textures[Magma].loadFromFile("assets/lava.png");
    textures[Chest].loadFromFile("assets/chest.png");

    std::vector<sf::Sprite> terrainSprites(Terrain::NumTerrains);
    for (int i = 0; i < Terrain::NumTerrains; ++i)
    {
        terrainSprites[i].setTexture(textures[i]);
    };

    float zoomLevel = 8.0f;

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

    sf::Texture animalTexture;
    animalTexture.loadFromFile("assets/animal_chicken.png");
    sf::Sprite animalSprite(animalTexture);
    animalSprite.setTextureRect(sf::IntRect(0, 0, 16, 16)); // Set initial frame
    animalSprite.setScale(0.7f, 0.7f);
    animalSprite.setPosition(191 * tileSize, 86 * tileSize);

    // animate the lava tiles by changing the texture rect the texture dimensions are 16x320
    sf::Texture lavaTexture;
    lavaTexture.loadFromFile("assets/lava.png");
    sf::Sprite lavaSprite(lavaTexture);
    lavaSprite.setTextureRect(sf::IntRect(0, 0, 16, 320)); // Set initial frame

    bool hasKey1 = false;
    bool hasKey2 = false;
    bool hasKey3 = false;

    sf::Texture key1Texture;
    key1Texture.loadFromFile("assets/key1.png");
    sf::Sprite key1Sprite(key1Texture);
    key1Sprite.setScale(0.5f, 0.5f);
    key1Sprite.setPosition(200 * tileSize, 109 * tileSize);

    sf::Texture key2Texture;
    key2Texture.loadFromFile("assets/key2.png");
    sf::Sprite key2Sprite(key2Texture);
    key2Sprite.setScale(0.5f, 0.5f);
    key2Sprite.setPosition(231 * tileSize, 198 * tileSize);

    sf::Texture key3Texture;
    key3Texture.loadFromFile("assets/key3.png");
    sf::Sprite key3Sprite(key3Texture);
    key3Sprite.setScale(0.5f, 0.5f);
    key3Sprite.setPosition(30 * tileSize, 28 * tileSize);

    static sf::Clock animalClock;
    static int frame = 0;

    static sf::Clock lavaAnimationClock;
    static int tileFrame = 0;
    static bool direction = true; // Initially moving "forward" through frames

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
                    isInventoryOpen = !isInventoryOpen;
                };
            };
        };

        // Movement vector.
        sf::Vector2f movement(0, 0);

        int currentTileX = playerPosition.x / tileSize;
        int currentTileY = playerPosition.y / tileSize;
        int currentTileType = tilemap[currentTileY][currentTileX];

        // If the player is on a tree, allow the player to chop the tree
        if (currentTileType == Tree1 || currentTileType == Tree2 || currentTileType == Tree3 || currentTileType == Tree4)
        {
            static sf::Clock chopClock;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::E) && chopClock.getElapsedTime().asSeconds() >= 1.0f)
            {
                chopClock.restart();
                // Add wood to the player's inventory
                bool woodFound = false;
                for (auto &item : inventory)
                {
                    if (item.name == "Wood")
                    {
                        item.quantity += 1;
                        woodFound = true;
                        break;
                    }
                }
                if (!woodFound)
                {
                    Item wood;
                    wood.name = "Wood";
                    wood.quantity = 1;
                    inventory.push_back(wood);
                }

                // Add fruit to the player's inventory
                bool fruitFound = false;
                for (auto &item : inventory)
                {
                    if (item.name == "Fruit")
                    {
                        item.quantity += 2;
                        fruitFound = true;
                        break;
                    }
                }
                if (!fruitFound)
                {
                    Item fruit;
                    fruit.name = "Fruit";
                    fruit.quantity = 2;
                    inventory.push_back(fruit);
                }

                playerFood -= 20.0f;  // Decrease hunger when player chops a tree
                playerThirst -= 5.0f; // Decrease thirst when player chops a tree

                // Increment the current tile's value, and handle wrapping around if necessary
                tilemap[currentTileY][currentTileX]++; // Increment current tile

                if (tilemap[currentTileY][currentTileX] > Tree4) // If it's greater than the highest tree stage
                {
                    // Wrap around to the initial tree stage
                    tilemap[currentTileY][currentTileX] = Grass;
                }
            }
        }

        // If F is pressed; consume a fruit to gain hunger
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        {
            for (auto &item : inventory)
            {
                if (item.name == "Fruit" && item.quantity > 0)
                {
                    item.quantity -= 1;
                    playerFood += 30.0f; // Increase hunger when player consumes a fruit
                    if (playerFood >= maxFood)
                    {
                        playerFood = maxFood;
                    }
                    break;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        {
            for (auto &item : inventory)
            {
                if (item.name == "Water bottle" && item.quantity > 0)
                {
                    item.quantity -= 1;
                    playerThirst += 45.0f;
                    if (playerThirst >= maxThirst)
                    {
                        playerThirst = maxThirst;
                    }
                    break;
                }
            }
        }

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

        // Decrease life if the player walks on a firecamp or is just above a firecamp
        if (currentTileType == Firecamp || tilemap[currentTileY + 1][currentTileX] == Firecamp)
        {
            playerHealth -= 0.01f;
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
            // Check if the new position is on a water tile (assuming water tiles are represented by value 1) or a fence
            int tileX = newPosition.x / tileSize;
            int tileY = newPosition.y / tileSize;
            if (tilemap[tileY][tileX] != Water && tilemap[tileY][tileX] != ShelterWalls)
            {
            // Move the player to the new position
                playerPosition = newPosition;
                playerSprite.setPosition(playerPosition);
            };
        };

        sf::Vector2f viewSize(windowWidth / zoomLevel, windowHeight / zoomLevel);

        sf::View view(playerPosition, viewSize);

        window.setView(view);

        updateAnimals();

        window.clear();

        // Draw the terrain sprites with trees and fences layered over grass
        for (int y = 0; y < mapHeight; ++y)
        {
            for (int x = 0; x < mapWidth; ++x)
            {
                int terrainType = tilemap[y][x];
                if (terrainType >= 0 && terrainType < Terrain::NumTerrains)
                {
                    // If the terrain is a tree, a firecamp or a fence draw the grass first
                    if (terrainType == Firecamp || terrainType == Tree1 || terrainType == Tree2 || terrainType == Tree3 || terrainType == Tree4 || terrainType == ShelterWalls)
                    // if (terrainType == ShelterWalls)
                    {
                        int grassType = Grass; // Assuming grass is represented by 0
                        terrainSprites[grassType].setPosition(x * tileSize, y * tileSize);
                        window.draw(terrainSprites[grassType]);
                    };
                    // if it's lava, animate it by changing the texture rect
                    if (terrainType == Lava || terrainType == Magma)
                    {
                        if (lavaAnimationClock.getElapsedTime().asSeconds() >= 0.3f)
                        {
                            if (tileFrame == 19)
                            {
                                direction = false; // Change direction when reaching the last frame
                            }
                            else if (tileFrame == 0)
                            {
                                direction = true; // Change direction when reaching the first frame
                            }

                            tileFrame += direction ? 1 : -1; // Increment or decrement frame based on direction

                            lavaSprite.setTextureRect(sf::IntRect(0, tileFrame * 16, 16, 16));
                            lavaAnimationClock.restart();
                        }
                        lavaSprite.setPosition(x * tileSize, y * tileSize);
                        window.draw(lavaSprite);

                        // Draw the lava tiles in the surrounding area
                        if (y > 0)
                        {
                            lavaSprite.setPosition(x * tileSize, (y - 1) * tileSize);
                            window.draw(lavaSprite);
                        }
                        if (y < mapHeight - 1)
                        {
                            lavaSprite.setPosition(x * tileSize, (y + 1) * tileSize);
                            window.draw(lavaSprite);
                        }
                        if (x > 0)
                        {
                            lavaSprite.setPosition((x - 1) * tileSize, y * tileSize);
                            window.draw(lavaSprite);
                        }
                        if (x < mapWidth - 1)
                        {
                            lavaSprite.setPosition((x + 1) * tileSize, y * tileSize);
                            window.draw(lavaSprite);
                        }
                    }
                    if (terrainType == Water)
                    {
                    }

                    if (terrainType != Tree1 && terrainType != Tree2 && terrainType != Tree3 && terrainType != Tree4)
                    {
                        terrainSprites[terrainType].setPosition(x * tileSize, y * tileSize);
                        window.draw(terrainSprites[terrainType]);
                    }

                    // then draw trees and scale them up by 2
                    if (terrainType == Tree1 || terrainType == Tree2 || terrainType == Tree3 || terrainType == Tree4)
                    {

                        terrainSprites[terrainType].setScale(2.f, 2.f);
                        terrainSprites[terrainType].setPosition(x * tileSize - 16, y * tileSize - 16);
                        window.draw(terrainSprites[terrainType]);
                    }
                };
            };
        };

        // Update the positions of the bars and icons to follow the player
        float barX = playerPosition.x - windowWidth / (6 * zoomLevel) - 65;
        float barY = playerPosition.y - windowHeight / (6 * zoomLevel) - 40;
        float iconX = barX - 10; // Adjust the distance between the icon and the bar as needed

        // draw keys
        if (hasKey1 == false)
        {
            window.draw(key1Sprite);
        }
        else if (hasKey2 == false)
        {
            window.draw(key2Sprite);
        }
        else if (hasKey3 == false)
        {
            window.draw(key3Sprite);
        }

        drawAnimals(window);

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

        // display coordinates of the player
        displayCaptionText(window, font, "X: " + std::to_string(currentTileX) + " Y: " + std::to_string(currentTileY), sf::Vector2f(1275, 10));

        if (currentTileX >= 182 && currentTileX <= 187 && currentTileY >= 95 && currentTileY <= 99) // trigger zone
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
                        if (inventory[i].name == "Wood" && inventory[i].quantity >= 10)
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
                            if (inventory[i].name == "Wood")
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

        if (currentTileX >= 141 && currentTileX <= 145 && currentTileY >= 107 && currentTileY <= 110)
        {
            if (!bridge2Crafted)
            {
                displayCaptionText(window, font, "Press 'E' to craft the bridge to the third island", sf::Vector2f(475, 750));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    bool hasWood = false;
                    for (size_t i = 0; i < inventory.size(); ++i)
                    {
                        if (inventory[i].name == "Wood" && inventory[i].quantity >= 20)
                        {
                            hasWood = true;
                            break;
                        }
                    }

                    if (hasWood)
                    {
                        for (size_t i = 0; i < inventory.size(); ++i)
                        {
                            if (inventory[i].name == "Wood")
                            {
                                inventory[i].quantity -= 20;
                                if (inventory[i].quantity == 0)
                                {
                                    inventory.erase(inventory.begin() + i);
                                }
                                break;
                            }
                        }

                        for (int y = 102; y <= 110; ++y)
                        {
                            for (int x = 141; x <= 144; ++x)
                            {
                                tilemap[y][x] = Wood;
                            }
                        }

                        displayCaptionText(window, font, "Bridge has been crafted", sf::Vector2f(475, 750));
                        bridge2Crafted = true;
                    }
                    else
                    {
                        displayCaptionText(window, font, "You need 20 wood to craft a bridge", sf::Vector2f(515, 790));
                    }
                }
            }
            else
            {
                // std::cout << "Bridge 2 has already been crafted" << std::endl;
            }
        }

        if (currentTileX >= 150 && currentTileX <= 159 && currentTileY >= 61 && currentTileY <= 65)
        {
            if (!bridge3Crafted)
            {
                displayCaptionText(window, font, "Press 'E' to craft the bridge to the third island", sf::Vector2f(475, 750));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    bool hasWood = false;
                    for (size_t i = 0; i < inventory.size(); ++i)
                    {
                        if (inventory[i].name == "Wood" && inventory[i].quantity >= 25)
                        {
                            hasWood = true;
                            break;
                        }
                    }

                    if (hasWood)
                    {
                        for (size_t i = 0; i < inventory.size(); ++i)
                        {
                            if (inventory[i].name == "Wood")
                            {
                                inventory[i].quantity -= 25;
                                if (inventory[i].quantity == 0)
                                {
                                    inventory.erase(inventory.begin() + i);
                                }
                                break;
                            }
                        }

                        for (int y = 62; y <= 64; ++y)
                        {
                            for (int x = 152; x <= 157; ++x)
                            {
                                tilemap[y][x] = Wood;
                            }
                        }

                        displayCaptionText(window, font, "Bridge has been crafted", sf::Vector2f(475, 750));
                        bridge3Crafted = true;
                    }
                    else
                    {
                        displayCaptionText(window, font, "You need 25 wood to craft a bridge", sf::Vector2f(515, 790));
                    }
                }
            }
            else
            {
                // std::cout << "Bridge 3 has already been crafted" << std::endl;
            }
        }

        if (currentTileX >= 87 && currentTileX <= 91 && currentTileY >= 110 && currentTileY <= 112)
        {
            if (!bridge4Crafted)
            {
                displayCaptionText(window, font, "Press 'E' to craft the bridge to the fourth island", sf::Vector2f(475, 750));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    bool hasWood = false;
                    for (size_t i = 0; i < inventory.size(); ++i)
                    {
                        if (inventory[i].name == "Wood" && inventory[i].quantity >= 50)
                        {
                            hasWood = true;
                            break;
                        }
                    }

                    if (hasWood)
                    {
                        for (size_t i = 0; i < inventory.size(); ++i)
                        {
                            if (inventory[i].name == "Wood")
                            {
                                inventory[i].quantity -= 50;
                                if (inventory[i].quantity == 0)
                                {
                                    inventory.erase(inventory.begin() + i);
                                }
                                break;
                            }
                        }

                        for (int y = 112; y <= 117; ++y)
                        {
                            for (int x = 88; x <= 90; ++x)
                            {
                                tilemap[y][x] = Wood;
                            }
                        }

                        displayCaptionText(window, font, "Bridge has been crafted", sf::Vector2f(475, 750));
                        bridge4Crafted = true;
                    }
                    else
                    {
                        displayCaptionText(window, font, "You need 50 wood to craft a bridge", sf::Vector2f(515, 790));
                    }
                }
            }
            else
            {
                // std::cout << "Bridge 4 has already been crafted" << std::endl;
            }
        }


        // detect if the player is near the key1 (between 190, 85 and 192, 87) then display a message to press 'E' to pick up the key and pass the hasKey1 to true
        if (currentTileX >= 199 && currentTileX <= 201 && currentTileY >= 108 && currentTileY <= 110)
        {
            if (hasKey1 == false)
            {
                displayCaptionText(window, font, "Press 'E' to pick up the key", sf::Vector2f(475, 750));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    hasKey1 = true;
                    displayCaptionText(window, font, "Key 1 has been picked up", sf::Vector2f(475, 750));
                }
            }
        }

        // detect if the player is near the key2 (between 231, 198 and 233, 200) then display a message to press 'E' to pick up the key and pass the hasKey2 to true
        if (currentTileX >= 230 && currentTileX <= 232 && currentTileY >= 197 && currentTileY <= 199)
        {
            if (hasKey2 == false)
            {
                displayCaptionText(window, font, "Press 'E' to pick up the key", sf::Vector2f(475, 750));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    hasKey2 = true;
                    displayCaptionText(window, font, "Key 2 has been picked up", sf::Vector2f(475, 750));
                }
            }
        }

        // detect if the player is near the key3 (between 30, 28 and 32, 30) then display a message to press 'E' to pick up the key and pass the hasKey3 to true
        if (currentTileX >= 29 && currentTileX <= 31 && currentTileY >= 27 && currentTileY <= 29)
        {
            if (hasKey3 == false)
            {
                displayCaptionText(window, font, "Press 'E' to pick up the key", sf::Vector2f(475, 750));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    hasKey3 = true;
                    displayCaptionText(window, font, "Key 3 has been picked up", sf::Vector2f(475, 750));
                }
            }
        }

        // now if the player is on the area of the blackbox (between 25,222 and 30,227) and has all the keys, display a message to press 'E' to open the blackbox
        if (currentTileX >= 24 && currentTileX <= 31 && currentTileY >= 221 && currentTileY <= 228)
        {
            if (hasKey1 == true && hasKey2 == true && hasKey3 == true)
            {
                displayCaptionText(window, font, "Press 'E' to open the blackbox", sf::Vector2f(475, 750));

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
                {
                    displayCaptionText(window, font, "The blackbox has been opened", sf::Vector2f(500, 750));
                }
            }
            else
            {
                displayCaptionText(window, font, "You need all the keys to open the blackbox", sf::Vector2f(500, 750));
            }
        }

        // Display the contents of the window
        window.display();
    }

    return 0;
}
