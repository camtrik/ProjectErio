#include "Global.h"





void loadAudioFile(const std::string path, sf::SoundBuffer& buffer, sf::Sound& sound)
{
	if (!buffer.loadFromFile(path)) {
		std::cout << "Load audio file failed: " << path << std::endl;
	}
	sound.setBuffer(buffer);
}

void loadMusicFile(const std::string path, sf::Music& music)
{
	if (!music.openFromFile(path)) {
		std::cout << "Load music file failed: " << path << std::endl;
	}
}

void playMusic(sf::Music& bgm, std::string fileName, bool loop) 
{
	if (bgm.getStatus() != sf::Music::Playing) {
		loadMusicFile(fileName, bgm);
		bgm.setLoop(loop);
		bgm.play();
	}
}

void stopMusic(sf::Music& music)
{
	if (music.getStatus() == sf::Music::Playing) {
		music.stop();
	}
}