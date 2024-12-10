// cmp_sound_effect.cpp

#include "cmp_sound_effect.h"

SoundEffectComponent::SoundEffectComponent(Entity* p, const std::string& soundFile) : Component(p) {
    if (!_soundBuffer.loadFromFile(soundFile)) {
        throw std::runtime_error("Failed to load sound effect");
    }
    _sound.setBuffer(_soundBuffer);
    _sound.setLoop(true);
    _sound.setVolume(30.0f);
}

void SoundEffectComponent::playSound() {
    if (_sound.getStatus() != sf::Sound::Playing) {
        _sound.play();
    }
}

void SoundEffectComponent::stopSound() {
    if (_sound.getStatus() == sf::Sound::Playing) {
        _sound.stop();
    }
}

void SoundEffectComponent::setVolume(const float volume) {
    _sound.setVolume(volume);
}

void SoundEffectComponent::update(double dt) {
    // No update logic needed
}

void SoundEffectComponent::render() {
    // No rendering logic needed
}
