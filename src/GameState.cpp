#include "GameState.h"

/*--------------------------------------GamePlayingState------------------------------------------------*/

InGameState::InGameState() :
    backgroundColor(0, 219, 255)
{
    mapManager.converSketch(enemies, erio);
}

void InGameState::handleInput(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        else if (event.type == sf::Event::KeyPressed && erio.getBombCount() > 0) {
            if (event.key.code == sf::Keyboard::LControl) {
                erio.throwBomb();
            }
        }
    }
}

void InGameState::update(std::chrono::microseconds deltaTime) 
{
    viewX = std::clamp<int>(round(erio.getX() - SCREEN_WIDTH / 2), 0, CELL_SIZE * mapManager.getMapSize() - SCREEN_WIDTH);
    mapManager.update();
    erio.update(viewX, mapManager);

    for (auto& enemy : enemies) {
        enemy->update(viewX, mapManager, erio, enemies);
    }

    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const std::shared_ptr<Entity>& enemy) { return enemy->getDead(); }), enemies.end());

}

void InGameState::render(sf::RenderWindow& window, sf::View& view) 
{
    view.reset(sf::FloatRect(viewX, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
    window.setView(view);
    window.clear(backgroundColor);

    mapManager.drawMapBackground(viewX, window);
    erio.drawBombs(viewX, window);
    mapManager.drawMapBlocks(viewX, window);
    erio.draw(viewX, window);

    for (auto& enemy : enemies) {
        enemy->draw(viewX, window);
    }

    erio.displayMessage(viewX, window);
    window.display();

    if (erio.isWin()) {
        isWin = true;
    }
    if (erio.getDead()) {
        isOver = true;
    }

}

void InGameState::reset()
{
    erio.reset(true);  // 重置Erio
    enemies.clear();  // 清空敌人列表
    mapManager.converSketch(enemies, erio);  // 重新初始化地图
    isOver = false;
    isWin = false;
}


GameStartMenuState::GameStartMenuState() {
    // 加载背景图片
    backgroundTexture.loadFromFile("Resources/Images/bg.png");
    backgroundSprite.setTexture(backgroundTexture);

    // 设置文本
    font.loadFromFile(FONT_PATH);
    text.setFont(font);
    descriptionText.setFont(font);
    tutorialText.setFont(font);
    ruleText.setFont(font);

    text.setString("PRESS SPACE TO START");
    descriptionText.setString(GAME_DESCRIPTION);
    tutorialText.setString(HOW_TO_PLAY);
    ruleText.setString(SCORE_RULE);

    text.setCharacterSize(24);
    descriptionText.setCharacterSize(16);
    tutorialText.setCharacterSize(16);
    ruleText.setCharacterSize(16);

    text.setFillColor(sf::Color::White);
    descriptionText.setFillColor(sf::Color::White);
    tutorialText.setFillColor(sf::Color::White);
    ruleText.setFillColor(sf::Color::White);

    text.setPosition(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 3 * CELL_SIZE);  // 设置文本在屏幕中央
    descriptionText.setPosition(CELL_SIZE, SCREEN_HEIGHT / 2 + 50 - 3 * CELL_SIZE);
    tutorialText.setPosition(CELL_SIZE, SCREEN_HEIGHT / 2 + 100);
    ruleText.setPosition(CELL_SIZE, SCREEN_HEIGHT / 2 + 200);

}

/*--------------------------------------StartMenuState------------------------------------------------*/
void GameStartMenuState::handleInput(sf::RenderWindow& window) 
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
            isOver = true;
        }
    }
}

void GameStartMenuState::update(std::chrono::microseconds deltaTime) 
{
}

void GameStartMenuState::render(sf::RenderWindow& window, sf::View& view) 
{
    view.reset(sf::FloatRect(0, 0, SCREEN_WIDTH * SCREEN_RESIZE, SCREEN_HEIGHT * SCREEN_RESIZE));
    window.setView(view);
    window.clear();
    window.draw(backgroundSprite);
    window.draw(tutorialText);
    window.draw(descriptionText);
    window.draw(ruleText);
    window.draw(text);
    window.display();
}

/*--------------------------------------GameOverState------------------------------------------------*/
GameOverState::GameOverState() {
    // 设置文本
    font.loadFromFile(FONT_PATH);
    overText.setFont(font);
    retryText.setFont(font);
    tipText.setFont(font);

    overText.setString("GAME OVER !");
    retryText.setString("PRESS RETURN TO RESTART");
    tipText.setString(FAIL_TIP);

    overText.setCharacterSize(24);
    retryText.setCharacterSize(24);
    tipText.setCharacterSize(36);

    retryText.setFillColor(sf::Color::White);
    overText.setFillColor(sf::Color::White);
    tipText.setFillColor(sf::Color::Blue);

    overText.setPosition(SCREEN_WIDTH / 2 + CELL_SIZE * 2, CELL_SIZE * 2);
    retryText.setPosition(SCREEN_WIDTH / 2 - CELL_SIZE, CELL_SIZE * 4);
    tipText.setPosition(CELL_SIZE * 4, CELL_SIZE * 8);

    backgroundTexture.loadFromFile("Resources/Images/naiteiFail.png");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setPosition(2 * CELL_SIZE, SCREEN_HEIGHT / 2 + 8 * CELL_SIZE);
}

void GameOverState::handleInput(sf::RenderWindow& window)
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
            isOver = true;
        }
    }
}

void GameOverState::update(std::chrono::microseconds deltaTime)
{
}

void GameOverState::render(sf::RenderWindow& window, sf::View& view) 
{
    view.reset(sf::FloatRect(0, 0, SCREEN_WIDTH * SCREEN_RESIZE, SCREEN_HEIGHT * SCREEN_RESIZE));
    window.setView(view);
    window.clear();
    window.draw(retryText);
    //window.draw(overText);
    window.draw(tipText);
    window.draw(backgroundSprite);
    window.display();
}

/*--------------------------------------GameWinState------------------------------------------------*/
GameWinState::GameWinState() :
    backgroundColor(sf::Color::White)
{
    // 设置文本
    font.loadFromFile(FONT_PATH);
    winText.setFont(font);
    winText.setString(WIN_MESSAGE);
    winText.setCharacterSize(24);
    winText.setFillColor(sf::Color::Black);
    winText.setPosition(CELL_SIZE * 2, SCREEN_HEIGHT - CELL_SIZE * 8);  // 设置文本在屏幕中央

    backgroundTexture.loadFromFile("Resources/Images/naitei.png");
    congraTexture.loadFromFile("Resources/Images/congrat.png");
    backgroundSprite.setTexture(backgroundTexture);
    congraSprite.setTexture(congraTexture);
    backgroundSprite.setPosition(SCREEN_WIDTH / 2 - 5 * CELL_SIZE, SCREEN_HEIGHT / 2 + 5 * CELL_SIZE);
    // congraTexture 顺时针旋转30度
    congraSprite.setPosition(SCREEN_WIDTH / 2 + CELL_SIZE * 12, SCREEN_HEIGHT / 2 + 4 * CELL_SIZE);
    congraSprite.setRotation(15);
}

void GameWinState::handleInput(sf::RenderWindow& window) 
{
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Return) {
            isOver = true;
        }
    }
}

void GameWinState::update(std::chrono::microseconds deltaTime) 
{
}

void GameWinState::render(sf::RenderWindow& window, sf::View& view)
{
    view.reset(sf::FloatRect(0, 0, SCREEN_WIDTH * SCREEN_RESIZE, SCREEN_HEIGHT * SCREEN_RESIZE));
    window.setView(view);
    window.clear(backgroundColor);
    window.draw(winText);
    window.draw(backgroundSprite);
    window.draw(congraSprite);
    window.display();
}
