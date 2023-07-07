#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>

#include "Erio.h"
#include "Global.h"
#include "MapManager.h"
#include "Slime.h"
#include "Bomb.h"

class GameState {
protected:
    bool isOver{ false };
public:
    virtual void handleInput(sf::RenderWindow& window) = 0;
    virtual void update(std::chrono::microseconds deltaTime) = 0;
    virtual void render(sf::RenderWindow& window, sf::View& view) = 0;
    void setOver(bool over) { isOver = over; }  
};

/*--------------------------------------GamePlayingState------------------------------------------------*/
class InGameState : public GameState {
private:
    sf::Music bgm;
    sf::Color backgroundColor;
    Erio erio;
    std::vector<std::shared_ptr<Entity>> enemies;
    MapManager mapManager;
    unsigned viewX;
    bool isWin{ false };

public:
    InGameState();
    void handleInput(sf::RenderWindow& window) override;

    void update(std::chrono::microseconds deltaTime) override;

    void render(sf::RenderWindow& window, sf::View& view) override;

    void reset();

    bool switchToGameOver() { return isOver; }
    bool switchToGameWin() { return isWin; }
};


/*--------------------------------------StartMenuState------------------------------------------------*/
class GameStartMenuState : public GameState {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text text;
    sf::Text descriptionText;
    sf::Text tutorialText;
    sf::Text ruleText;
   
    sf::Font font;


public:
    GameStartMenuState();

    void handleInput(sf::RenderWindow& window);
    void update(std::chrono::microseconds deltaTime);
    void render(sf::RenderWindow& window, sf::View& view);
    bool switchToInGameState() { return isOver; }
};


/*--------------------------------------GameOverState------------------------------------------------*/
class GameOverState : public GameState {
private:
    sf::Text overText;
    sf::Text retryText;
    sf::Text tipText;
    sf::Font font;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;

public:
    GameOverState();
    void handleInput(sf::RenderWindow& window) override;
    void update(std::chrono::microseconds deltaTime);
    void render(sf::RenderWindow& window, sf::View& view) override;
    bool restartGame() { return isOver; }
};

/*--------------------------------------GameWinState------------------------------------------------*/
class GameWinState : public GameState {
private:
    sf::Text winText;
    sf::Font font;
    sf::Color backgroundColor;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Texture congraTexture;
    sf::Sprite congraSprite;

public:
    GameWinState();
    void handleInput(sf::RenderWindow& window) override;
    void update(std::chrono::microseconds deltaTime) override;
    void render(sf::RenderWindow& window, sf::View& view) override;
    bool restartGame() { return isOver; }
};
