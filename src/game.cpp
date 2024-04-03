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
const sf::Color healthColor(136, 8, 8);
const sf::Color thirstColor(36, 157, 159);
const sf::Color foodColor(255, 165, 0);

int mapCenterX = mapWidth * tileSize / 2;
int mapCenterY = mapHeight * tileSize / 2;

int alternate = 0;

bool isInventoryOpen = false;

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
void drawInventory(sf::RenderWindow &window)
{
    sf::RectangleShape inventory(sf::Vector2f(300, 300));
    inventory.setPosition(100, 100);
    inventory.setFillColor(sf::Color::White);
    inventory.setOutlineThickness(2);
    inventory.setOutlineColor(sf::Color::Black);
    window.draw(inventory);
};

int main()
{
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
                std::cout << "Key pressed: " << event.key.code << std::endl;

                // Check if the key pressed is 'I'
                if (event.key.code == sf::Keyboard::I)
                {
                    // Toggle inventory only on key press
                    isInventoryOpen = !isInventoryOpen;

                    // if (isInventoryOpen)
                    // {
                    //     std::cout << "Inventory is open" << std::endl;
                    // }
                    // else
                    // {
                    //     std::cout << "Inventory is closed" << std::endl;
                    // };
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
            if (tilemap[tileY][tileX] != Water)
            {
                // Move the player to the new position
                playerPosition = newPosition;
                playerSprite.setPosition(playerPosition);
            };
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

        drawInventory(window);

        if (isInventoryOpen)
        {
            std::cout << "Inventory is open" << std::endl;
            // drawInventory(window);
        }
        else
        {
            std::cout << "Inventory is closed" << std::endl;
        };

        // Display the contents of the window
        window.display();
    }

    return 0;
}