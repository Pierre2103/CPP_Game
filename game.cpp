#include <SFML/Graphics.hpp>
#include <array>
#include <vector>
#include <iostream>
#include "map.hpp"

// Window dimensions.
const int windowWidth = 1920; // Example size, adjust as needed
const int windowHeight = 1080; // Example size, adjust as needed

int mapCenterX = mapWidth * tileSize / 2;
int mapCenterY = mapHeight * tileSize / 2;

const int speed = 4;

bool alternate = false;

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

    // std::cout << windowWidth/6 << ", " << windowHeight/6 << std::endl;


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
    textures[PlayerSpawn].loadFromFile("assets/no_texture.png");

    // Create sprites for each terrain type.
    std::vector<sf::Sprite> terrainSprites(Terrain::NumTerrains);
    for (int i = 0; i < Terrain::NumTerrains; ++i) {
        terrainSprites[i].setTexture(textures[i]);
    }

    window.setView(sf::View(sf::FloatRect(1584, 404, windowWidth/6 , windowHeight/6)));

    // Initialize player position at the center of the screen.
    sf::Vector2f playerPosition(1744, 494);
    // put the player sprite here
    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/player_down_0.png");
    sf::Sprite playerSprite(playerTexture);


    // Main game loop.
    while (window.isOpen()) {
    // std::cout << "Player position: " << playerPosition.x << ", " << playerPosition.y << std::endl;
        // Handle events.
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // Movement vector.
        sf::Vector2f movement(0, 0);

        // Handle input to move the map (and thus the player appears to move).
        // Move the player sprite and change its texture accordingly between 2 frames.
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            movement.y -= speed;
            if (alternate) {
                playerTexture.loadFromFile("assets/player_up_1.png");
            } else {
                playerTexture.loadFromFile("assets/player_up_2.png");
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            movement.x -= speed;
            if (alternate) {
                playerTexture.loadFromFile("assets/player_left_1.png");
            } else {
                playerTexture.loadFromFile("assets/player_left_2.png");
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            movement.y += speed;
            if (alternate) {
                playerTexture.loadFromFile("assets/player_down_1.png");
            } else {
                playerTexture.loadFromFile("assets/player_down_2.png");
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            movement.x += speed;
            if (alternate) {
                playerTexture.loadFromFile("assets/player_right_1.png");
            } else {
                playerTexture.loadFromFile("assets/player_right_2.png");
            }
        }

        alternate = !alternate;

        // Update the player sprite texture
        playerSprite.setTexture(playerTexture);

        playerPosition += 0.5f * movement;
        playerSprite.setPosition(playerPosition);

        // Update the view to follow the player sprite (centered).
        window.setView(sf::View(sf::FloatRect(playerPosition.x - 152, playerPosition.y - 80, windowWidth/6, windowHeight/6)));
        // Clear the window.
        window.clear();

        // Draw the tilemap, adjusted based on the player's "position".
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int terrainType = tilemap[y][x];
                if (terrainType >= 0 && terrainType < Terrain::NumTerrains) {
                    terrainSprites[terrainType].setPosition(x * tileSize - playerPosition.x + windowWidth / 2, y * tileSize - playerPosition.y + windowHeight / 2);
                    window.draw(terrainSprites[terrainType]);
                }
            }
        }

        // Draw the player.
        window.draw(playerSprite);

        // Update the window.
        window.display();
    }

    return 0;
}
