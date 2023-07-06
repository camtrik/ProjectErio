#pragma once
#include <memory>
#include "GameState.h"
class GameState;

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
