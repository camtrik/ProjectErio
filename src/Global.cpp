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