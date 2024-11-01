#include "AudioManager.hpp"
#include <iostream>
#include "../Debug/Logger.hpp"
#include "Macros.hpp"
#include "Config.hpp"

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
    {AudioMusicId::MENUS, {std::string("res/music/menus.") + MUSIC_FILE_EXT}}
};

AudioManager::AudioManager() {}

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

    // Sync volume
    int currentSoundVolume = Mix_Volume(-1, -1);
    int currentMusicVolume = Mix_VolumeMusic(-1);
    soundVolumeUnitInterval = currentSoundVolume / MIX_MAX_VOLUME;
    musicVolumeUnitInterval = currentMusicVolume / MIX_MAX_VOLUME;
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

bool AudioManager::loadAndStartMusic(const AudioMusicId id) 
{
    return loadAndStartMusic(musicConfigs.at(id).filename);
}

bool AudioManager::loadAndStartMusic(const std::string path)
{
    if (music.find(path) != music.end())
        return true;
    disposeAllLoadedMusic();
    Mix_Music* loadedMusic = Mix_LoadMUS(RPATH(path).c_str());
    if (!loadedMusic)
    {
        logd("Failed to load music! SDL_mixer Error: %s", Mix_GetError());
        return false;
    }
    music[path] = loadedMusic;
    Mix_PlayMusic(loadedMusic, -1);
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

void AudioManager::setMusicVolume(float volumeUnitInterval) 
{
    musicVolumeUnitInterval = volumeUnitInterval;
    int musicVolume = floor(volumeUnitInterval * MIX_MAX_VOLUME);
    Mix_VolumeMusic(musicVolume);
}

void AudioManager::setSoundVolume(float volumeUnitInterval) 
{
    soundVolumeUnitInterval = volumeUnitInterval;
    int soundVolume = floor(volumeUnitInterval * MIX_MAX_VOLUME);
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
    for (auto& music : music) 
    {
        Mix_FreeMusic(music.second);
    }
    music.clear();
}
