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
    playerTexture.loadFromFile("assets/player_down_idle.png");
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

        //if the player move  

        // Handle input to move the map (and thus the player appears to move).
        sf::Vector2f movement(0.f, 0.f);
        // Handle input to move the map (and thus the player appears to move).
        // Move the player sprite and change its texture accordingly
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            movement.x -= speed;
            playerTexture.loadFromFile("assets/player_left_1.png");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            movement.x += speed;
            playerTexture.loadFromFile("assets/player_right_1.png");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            movement.y -= speed;
            playerTexture.loadFromFile("assets/player_up_1.png");
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            movement.y += speed;
            playerTexture.loadFromFile("assets/player_down_1.png");
        }

        // Update the player sprite texture
        playerSprite.setTexture(playerTexture);

        playerPosition += movement;
        playerSprite.setPosition(playerPosition);
        // Clear the window.
        window.clear();

        // Draw the tilemap, adjusted based on the player's "position".
        for (int y = 0; y < mapHeight; ++y) {
            for (int x = 0; x < mapWidth; ++x) {
                int terrainType = tilemap[y][x];
                if (terrainType >= 0 && terrainType < Terrain::NumTerrains) {
                    terrainSprites[terrainType].setPosition(x * tileSize - playerPosition.x + windowWidth / 2, 
                                                            y * tileSize - playerPosition.y + windowHeight / 2);
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
