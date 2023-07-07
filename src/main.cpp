#include <iostream>
#include <chrono>
#include <cmath>
#include <vector>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>


#include "Erio.h"
#include "Global.h"
#include "MapManager.h"
#include "Slime.h"
#include "Bomb.h"
#include "GameState.h"

enum GameStateType {
	GameStartMenu,
	InGame,
	GameOver,
	GameWin
};

int main() {
    sf::RenderWindow window(sf::VideoMode(SCREEN_RESIZE * SCREEN_WIDTH, SCREEN_RESIZE * SCREEN_HEIGHT), "Project Erio", sf::Style::Close);
    window.setPosition(sf::Vector2i(window.getPosition().x, window.getPosition().y - 90));
    /*-----------------Game Loop Objects-----------------------*/
    sf::Music bgm;
    // Camera
    sf::View view(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    // lag time since last update
    std::chrono::microseconds lag(0);
    // time of last update
    std::chrono::steady_clock::time_point previousTime = std::chrono::steady_clock::now();;

    GameStartMenuState startMenu;
    InGameState game;
    GameOverState overMenu;
    GameWinState winMenu;
    GameStateType currentState = GameStartMenu;
    
    while (window.isOpen()) {
        std::chrono::microseconds deltaTime =
            std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previousTime);
        previousTime += deltaTime;
        lag += deltaTime;
        switch (currentState) {
        case GameStartMenu: 
            playMusic(bgm, "Resources/Audio/startBGM.ogg");
            while (FRAME_DURATION <= lag) {
                // update game logic
                startMenu.handleInput(window);
                startMenu.update(deltaTime);
                lag -= FRAME_DURATION;
            }
            if (FRAME_DURATION > lag) {
                startMenu.render(window, view);
            }
            if (startMenu.switchToInGameState()) {
                stopMusic(bgm);
                game.reset();
                currentState = InGame;
            }

            break;
        case InGame:
            playMusic(bgm, "Resources/Audio/mainBGM.ogg");
            while (FRAME_DURATION <= lag) {
                // update game logic
                game.handleInput(window);
                game.update(deltaTime);
                lag -= FRAME_DURATION;
            }
            if (FRAME_DURATION > lag) {
                game.render(window, view);
            }
            if (game.switchToGameOver()) {
                stopMusic(bgm);
                overMenu.setOver(false);
                currentState = GameOver;
                std::this_thread::sleep_for(std::chrono::seconds(2));
                playMusic(bgm, "Resources/Audio/failSound.wav", false);
            }
            if (game.switchToGameWin()) {
                stopMusic(bgm);
                winMenu.setOver(false);
                currentState = GameWin;
                std::this_thread::sleep_for(std::chrono::seconds(1));
                playMusic(bgm, "Resources/Audio/winSound.wav", false);
            }
            break;
        case GameOver:
            while (FRAME_DURATION <= lag) {
                // update game logic
                overMenu.handleInput(window);
                overMenu.update(deltaTime);
                lag -= FRAME_DURATION;
            }
            if (FRAME_DURATION > lag) {
                overMenu.render(window, view);
            }
            if (overMenu.restartGame()) {
                startMenu.setOver(false);
                currentState = GameStartMenu;
            }
            break;
        case GameWin:
            while (FRAME_DURATION <= lag) {
                // update game logic
                winMenu.handleInput(window);
                winMenu.update(deltaTime);
                lag -= FRAME_DURATION;
            }
            if (FRAME_DURATION > lag) {
                winMenu.render(window, view);
            }
            if (winMenu.restartGame()) {
                startMenu.setOver(false);
                currentState = GameStartMenu;
            }
            break;
        }
    }
    return 0;
}
