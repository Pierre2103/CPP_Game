#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <iostream>
#include "map.hpp"

const int windowWidth = 1920;
const int windowHeight = 1080;
const int speed = 3;

int mapCenterX = mapWidth * tileSize / 2;
int mapCenterY = mapHeight * tileSize / 2;

int alternate = 0;

// Enumeration for the terrain types.
enum Terrain {
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
    NumTerrains // This will give us the number of terrains.
};

int main() {
    // Create the main window.
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Night 4 Life");

    // Load textures.
    std::vector<sf::Texture> textures(Terrain::NumTerrains);
    // Load textures for each terrain type...
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

    // Create sprites for each terrain type.
    std::vector<sf::Sprite> terrainSprites(Terrain::NumTerrains);
    for (int i = 0; i < Terrain::NumTerrains; ++i) {
        terrainSprites[i].setTexture(textures[i]);
    }

    // Set the initial zoom level
    float zoomLevel = 8.0f;

    // Initialize player position at the center of the screen.
    sf::Vector2f playerPosition(158 * tileSize, 28 * tileSize);

    // put the player sprite here
    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/player_down_0.png");
    sf::Sprite playerSprite(playerTexture);

    // Main game loop.
    while (window.isOpen()) {
        // Handle events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // printf("The player is on the tile (%d, %d)\n", (int)playerPosition.x / tileSize, (int)playerPosition.y / tileSize);

        // Movement vector.
        sf::Vector2f movement(0, 0);

        // Determine the tile the player is standing on.
        int currentTileX = playerPosition.x / tileSize;
        int currentTileY = playerPosition.y / tileSize;
        int currentTileType = tilemap[currentTileY][currentTileX];

        // Handle input to move the map (and thus the player appears to move).
        // Move the player sprite and change its texture accordingly between 2 frames.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            movement.y -= speed;
            switch (alternate) {
                case 0:
                    playerTexture.loadFromFile("assets/player_up_1.png");
                    break;
                case 1:
                    playerTexture.loadFromFile("assets/player_up_2.png");
                    break;
                default:
                    playerTexture.loadFromFile("assets/player_up_0.png");
                    break;
            }
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            movement.x -= speed;
            switch (alternate) {
                case 0:
                    playerTexture.loadFromFile("assets/player_left_1.png");
                    break;
                case 1:
                    playerTexture.loadFromFile("assets/player_left_2.png");
                    break;
                default:
                    playerTexture.loadFromFile("assets/player_left_0.png");
                    break;
            }
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            movement.y += speed;
            switch (alternate) {
                case 0:
                    playerTexture.loadFromFile("assets/player_down_1.png");
                    break;
                case 1:
                    playerTexture.loadFromFile("assets/player_down_2.png");
                    break;
                default:
                    playerTexture.loadFromFile("assets/player_down_0.png");
                    break;
            }
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            movement.x += speed;
            switch (alternate) {
                case 0:
                    playerTexture.loadFromFile("assets/player_right_1.png");
                    break;
                case 1:
                    playerTexture.loadFromFile("assets/player_right_2.png");
                    break;
                default:
                    playerTexture.loadFromFile("assets/player_right_0.png");
                    break;
            }
        }

        // Check if the current tile is sand and adjust speed accordingly.
        if (currentTileType == Sand) {
            // Reduce the speed on sand tiles.
            movement *= 0.65f; // You can adjust the factor as needed.
        }

        alternate = (alternate + 1) % 3; // Cycle through 0, 1, 2

        playerSprite.setTexture(playerTexture);

        // Calculate the potential new position of the player
        sf::Vector2f newPosition = playerPosition + movement;

        // Check if the new position is within the map boundaries
        if (newPosition.x >= 0 && newPosition.x < mapWidth * tileSize &&
            newPosition.y >= 0 && newPosition.y < mapHeight * tileSize) {
            // Check if the new position is on a water tile (assuming water tiles are represented by value 1)
            int tileX = newPosition.x / tileSize;
            int tileY = newPosition.y / tileSize;
            if (tilemap[tileY][tileX] != Water && tilemap[tileY][tileX] != Tree) {
                // Move the player to the new position
                playerPosition = newPosition;
                playerSprite.setPosition(playerPosition);
            }
        }

        // Calculate the view size based on the window size and the zoom level
        sf::Vector2f viewSize(windowWidth / zoomLevel, windowHeight / zoomLevel);

        // Create the view centered on the player
        sf::View view(playerPosition, viewSize);

        // Set the view in the window
        window.setView(view);

        // Clear the window
        window.clear();

        // Draw the terrain sprites with trees layered over grass
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int terrainType = tilemap[y][x];
                if (terrainType >= 0 && terrainType < Terrain::NumTerrains) {
                    // If the terrain is a tree or a firecamp, draw the grass first
                    if (terrainType == Tree || terrainType == Firecamp) {
                        int grassType = Grass; // Assuming grass is represented by 0
                        terrainSprites[grassType].setPosition(x * tileSize, y * tileSize);
                        window.draw(terrainSprites[grassType]);
                    }
                    // Draw the terrain sprite
                    terrainSprites[terrainType].setPosition(x * tileSize, y * tileSize);
                    window.draw(terrainSprites[terrainType]);
                }
            }
        }

        // Draw the player sprite
        window.draw(playerSprite);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
