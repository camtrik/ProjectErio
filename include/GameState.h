#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Erio.h"
#include "Global.h"
#include "MapManager.h"
#include "Slime.h"
#include "Bomb.h"

class GameState {
public:
    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(std::chrono::microseconds deltaTime) = 0;
    virtual void render(sf::RenderWindow& window, sf::View& view) = 0;
};

class GameStateManager {
private:
    std::shared_ptr<GameState> currentState;

public:
    void setState(std::shared_ptr<GameState> state) {
        currentState = state;
    }

    void handleInput(sf::RenderWindow& window) {
        if (currentState)
            currentState->handleInput(window);
    }

    void update(std::chrono::microseconds deltaTime) {
        if (currentState)
            currentState->update(deltaTime);
    }

    void render(sf::RenderWindow& window, sf::View& view) {
        if (currentState)
            currentState->render(window, view);
    }
};

class InGameState : public GameState {
private:
    sf::View view;
    sf::Color backgroundColor;
    Erio erio;
    std::vector<std::shared_ptr<Entity>> enemies;
    MapManager mapManager;
    unsigned viewX;

public:
    InGameState() : backgroundColor(0, 219, 255) {
        view = sf::View(sf::FloatRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        mapManager.converSketch(enemies, erio);
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    void update(std::chrono::microseconds deltaTime) override {
        viewX = std::clamp<int>(round(erio.getX() - SCREEN_WIDTH / 2), 0, CELL_SIZE * mapManager.getMapSize() - SCREEN_WIDTH);
        mapManager.update();
        erio.update(viewX, mapManager);

        for (auto& enemy : enemies) {
            enemy->update(viewX, mapManager, erio, enemies);
        }

        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::shared_ptr<Entity>& enemy) { return enemy->getDead(); }), enemies.end());

    }

    void render(sf::RenderWindow& window, sf::View& view) override {
        view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
        window.setView(view);
        window.clear(backgroundColor);

        mapManager.drawMapBackground(viewX, window);
        erio.drawBombs(viewX, window);
        erio.draw(viewX, window);
        mapManager.drawMapBlocks(viewX, window);

        for (auto& enemy : enemies) {
            enemy->draw(viewX, window);
        }

        erio.displayMessage(viewX, window);
        window.display();
    }
};

