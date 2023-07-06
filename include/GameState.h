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

public:
    InGameState() : 
        backgroundColor(0, 219, 255)
    {
        mapManager.converSketch(enemies, erio);
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (event.type == sf::Event::KeyPressed /*&& erio.getBombCount() > 0*/) {
                if (event.key.code == sf::Keyboard::LControl) {
                    erio.throwBomb();
                }
            }
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

        if (erio.getDead()) {
            isOver = true;
        }
    }

    void reset()
    {
        erio.reset(true);  // ����Erio
        enemies.clear();  // ��յ����б�
        mapManager.converSketch(enemies, erio);  // ���³�ʼ����ͼ
        isOver = false;
    }

    bool switchToGameOver() { return isOver; }
};


/*--------------------------------------StartMenuState------------------------------------------------*/
class GameStartMenuState : public GameState {
private:
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Text text;
    sf::Font font;

public:
    GameStartMenuState() {
        // ���ر���ͼƬ
        backgroundTexture.loadFromFile("Resources/Images/bg.png");
        backgroundSprite.setTexture(backgroundTexture);

        // �����ı�
        font.loadFromFile("Resources/Fonts/slkscr.ttf");
        text.setFont(font);
        text.setString("PRESS SPACE TO START");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);  // �����ı�����Ļ����
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                // �����������������л�����Ϸ����״̬
                isOver = true;
            }
        }
    }

    void update(std::chrono::microseconds deltaTime) override {
        // �����״̬��û����Ҫ���µ���Ϸ�߼�
    }

    void render(sf::RenderWindow& window, sf::View& view) override {
        view.reset(sf::FloatRect(0, 0, SCREEN_WIDTH * SCREEN_RESIZE, SCREEN_HEIGHT * SCREEN_RESIZE));
        window.setView(view);
        window.clear();
        window.draw(backgroundSprite);
        window.draw(text);
        window.display();
    }

    bool switchToInGameState() { return isOver; }
};


/*--------------------------------------GameOverState------------------------------------------------*/
class GameOverState : public GameState {
private:
    sf::Text overText;
    sf::Text retryText;
    sf::Font font;

public:
    GameOverState() {
        // �����ı�
        font.loadFromFile("Resources/Fonts/slkscr.ttf");
        retryText.setFont(font);
        retryText.setString("PRESS RETURN TO RESTART");
        retryText.setCharacterSize(24);
        retryText.setFillColor(sf::Color::White);
        retryText.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT - CELL_SIZE * 8);  // �����ı�����Ļ����
    }

    void handleInput(sf::RenderWindow& window) override {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
                // �����������������л�����Ϸ��ʼ�˵�״̬
                isOver = true;
            }
        }
    }

    void update(std::chrono::microseconds deltaTime) override {
        // �����״̬��û����Ҫ���µ���Ϸ�߼�
        
    }

    void render(sf::RenderWindow& window, sf::View& view) override {
        view.reset(sf::FloatRect(0, 0, SCREEN_WIDTH * SCREEN_RESIZE, SCREEN_HEIGHT * SCREEN_RESIZE));
        window.setView(view);
        window.clear();
        window.draw(retryText);
        window.display();
    }

    bool restartGame() { return isOver; }
};
