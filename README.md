# Night 4 Life

![Night 4 Life](./assets/cover.webp)

## Description 📝

This is the repository for the Night 4 Life project. This is a C++ video game that was developed by @Pierre2103 and @AlexisLasselin for the C++ course at ALGOSUP.

This game is required by the course of Alan Méziane DAHMAM as an exercise to learn C++.

## Run 🏃

To run the game, you need to have the SFML library installed on your computer. You can find the installation instructions on the [SFML website](https://www.sfml-dev.org/tutorials/2.5/).

Once you have installed the library, you can run the game by running the following commands:

<!-- g++ src/game.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o output/game -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -->

```bash
g++ src/game.cpp -I/opt/homebrew/Cellar/sfml/2.6.1/include -o output/game -L/opt/homebrew/Cellar/sfml/2.6.1/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
./output/game
```

Enjoy the game! 👾

## Gameplay 🕹️

The game is a 2D game where you're the only survivor of a plane crash. You need to find a way to survive to this island and find a way to escape it. You will need to find food, water, and other resources to survive.

You win the game when you find the black box of the plane and you lose the game if you die.

## Features 🎯

- 2D Game
- Inventory System
- Interactions with the environment

## Controls 🎮

### QWERTY Keyboard 🇺🇸

- Move Up: W
- Move Down: S
- Move Left: A
- Move Right: D
- Interact: E
- Inventory: I
- Eat: F
- Drink: G
- Zoom In: P
- Zoom Out: M

### AZERTY Keyboard 🇫🇷

- Move Up: Z
- Move Down: S
- Move Left: Q
- Move Right: D
- Interact: E
- Inventory: I
- Eat: F
- Drink: G
- Zoom In: P
- Zoom Out: M
