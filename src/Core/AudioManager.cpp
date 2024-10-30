#include "AudioManager.hpp"
#include <iostream>
#include "../Debug/Logger.hpp"
#include "Macros.hpp"

const std::unordered_map<const AudioSFXId, AudioConfig> AudioManager::audioConfigs =
{
    // Menu
    {AudioSFXId::MENU_SELECT, {"res/sfx/menu_select.wav"}},

    // Pistol
    {AudioSFXId::PISTOL_FIRE, {"res/sfx/pistol_fire.wav"}},
    {AudioSFXId::PISTOL_DRY_FIRE, {"res/sfx/pistol_dry_fire.wav"}},
    {AudioSFXId::PISTOL_RELOAD, {"res/sfx/pistol_reload.wav"}},

    // Shotgun
    {AudioSFXId::SHOTGUN_FIRE, {"res/sfx/shotgun_fire.wav"}},
    {AudioSFXId::SHOTGUN_DRY_FIRE, {"res/sfx/shotgun_dry_fire.wav"}},
    {AudioSFXId::SHOTGUN_PUMP, {"res/sfx/shotgun_pump.wav"}},
    {AudioSFXId::SHOTGUN_RELOAD, {"res/sfx/shotgun_reload.wav"}},

    // SMG
    {AudioSFXId::SMG_FIRE, {"res/sfx/smg_fire.wav"}},
    {AudioSFXId::SMG_DRY_FIRE, {"res/sfx/smg_dry_fire.wav"}},
    {AudioSFXId::SMG_RELOAD, {"res/sfx/smg_reload.wav"}},

    // Mobs
    {AudioSFXId::JUMP, {"res/sfx/jump.wav"}},
    {AudioSFXId::LAND_FROM_JUMP, {"res/sfx/land_from_jump.wav"}},
    {AudioSFXId::CRUSH, {"res/sfx/crush.wav"}},
    {AudioSFXId::PLAYER_HURT, {"res/sfx/player_hurt.wav"}},

    // Pickup
    {AudioSFXId::PICKUP, {"res/sfx/pickup.wav"}}
};

const std::unordered_map<const AudioMusicId, AudioConfig> AudioManager::musicConfigs =
{
    {AudioMusicId::MENUS, {"res/music/menus.ogg"}}
};

AudioManager::AudioManager() : 
    soundVolume(MIX_MAX_VOLUME), 
    musicVolume(MIX_MAX_VOLUME) {}

AudioManager::~AudioManager() 
{
    dispose();
}

void AudioManager::init() 
{
    if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 2048) < 0) 
    {
        logd("SDL_mixer could not initialize! SDL_mixer Error: %s", Mix_GetError());
    }
}

void AudioManager::dispose() {
    for (auto& sound : sounds) 
    {
        Mix_FreeChunk(sound.second);
    }
    sounds.clear();

    disposeAllLoadedMusic();

    Mix_CloseAudio();
}

bool AudioManager::loadSound(const AudioSFXId id) 
{
    if (sounds.find(id) != sounds.end())
        return true;
    Mix_Chunk* sound = Mix_LoadWAV(RPATH(audioConfigs.at(id).filename).c_str());
    if (!sound)
    {
        logd("Failed to load sound effect! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }
    sounds[id] = sound;
    return true;
}

bool AudioManager::loadMusic(const AudioMusicId id) 
{
    disposeAllLoadedMusic();
    Mix_Music* music = Mix_LoadMUS(RPATH(musicConfigs.at(id).filename).c_str());
    if (!music) 
    {
        logd("Failed to load music! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }
    predefinedMusic[id] = music;
    return true;
}

bool AudioManager::loadMusic(const std::string path)
{
    disposeAllLoadedMusic();
    Mix_Music* music = Mix_LoadMUS(RPATH(path).c_str());
    if (!music)
    {
        logd("Failed to load music! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }
    customMusic[path] = music;
    return true;
}

void AudioManager::playSound(const AudioSFXId id, int loops) 
{
    auto it = sounds.find(id);
    if (it != sounds.end()) 
    {
        Mix_PlayChannel(-1, it->second, loops);
    }
}

void AudioManager::playMusic(const AudioMusicId id, int loops) 
{
    auto it = predefinedMusic.find(id);
    if (it != predefinedMusic.end()) 
    {
        Mix_PlayMusic(it->second, loops);
    }
}

void AudioManager::playMusic(const std::string path, int loops)
{
    auto it = customMusic.find(path);
    if (it != customMusic.end())
    {
        Mix_PlayMusic(it->second, loops);
    }
}

void AudioManager::stopMusic() 
{
    Mix_HaltMusic();
}

void AudioManager::pauseMusic() 
{
    Mix_PauseMusic();
}

void AudioManager::resumeMusic() 
{
    Mix_ResumeMusic();
}

void AudioManager::setMusicVolume(int volume) 
{
    musicVolume = volume;
    Mix_VolumeMusic(musicVolume);
}

void AudioManager::setSoundVolume(int volume) 
{
    soundVolume = volume;
    Mix_Volume(-1, soundVolume);
}

bool AudioManager::loadSoundList(const std::vector<AudioSFXId>& soundList) 
{
    bool success = true;
    for (const auto& soundId : soundList) 
    {
        if (!loadSound(soundId)) 
        {
            success = false;
        }
    }
    return success;
}

void AudioManager::disposeAllLoadedMusic()
{
    for (auto& music : predefinedMusic) 
    {
        Mix_FreeMusic(music.second);
    }
    predefinedMusic.clear();

    for (auto& music : customMusic) 
    {
        Mix_FreeMusic(music.second);
    }
    customMusic.clear();
}
