// cmp_sound_effect.h

#pragma once
#include "ecm.h"
#include <SFML/Audio.hpp>

class SoundEffectComponent : public Component {
private:
    sf::SoundBuffer _soundBuffer;
    sf::Sound _sound;

public:
    explicit SoundEffectComponent(Entity* p, const std::string& soundFile);
    void playSound();
    void stopSound();

    void update(double dt) override;

    void render() override;
};
