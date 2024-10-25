#ifndef __SRC_CORE_AUDIOMANAGER_HPP_
#define __SRC_CORE_AUDIOMANAGER_HPP_

#include <SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <vector>

struct AudioConfig
{
    std::string filename;
};

enum class AudioSFXId
{
    // Menu
    MENU_SELECT,

    // Pistol
    PISTOL_FIRE,
    PISTOL_DRY_FIRE,
    PISTOL_RELOAD,

    // Shotgun
    SHOTGUN_FIRE,
    SHOTGUN_DRY_FIRE,
    SHOTGUN_PUMP,
    SHOTGUN_RELOAD,

    // SMG
    SMG_FIRE,
    SMG_DRY_FIRE,
    SMG_RELOAD,

    // Mobs
    JUMP,
    LAND_FROM_JUMP,
    CRUSH,

    // Pickup
    PICKUP
};

enum class AudioMusicId
{
    MAIN_MENU,
    GAME
};

class AudioManager 
{
public:
    AudioManager();
    ~AudioManager();

    static const std::unordered_map<const AudioSFXId, AudioConfig> audioConfigs;
    static const std::unordered_map<const AudioMusicId, AudioConfig> musicConfigs;

    void init();
    void dispose();

    bool loadSound(const AudioSFXId id);
    bool loadMusic(const AudioMusicId id);

    void playSound(const AudioSFXId id, int loops = 0);
    void playMusic(const AudioMusicId id, int loops = -1);

    void stopMusic();
    void pauseMusic();
    void resumeMusic();

    void setMusicVolume(int volume);
    void setSoundVolume(int volume);

    bool loadSoundList(const std::vector<AudioSFXId>& soundList);
    bool loadMusicList(const std::vector<AudioMusicId>& musicList);

private:
    std::unordered_map<AudioSFXId, Mix_Chunk*> sounds;
    std::unordered_map<AudioMusicId, Mix_Music*> music;

    int soundVolume;
    int musicVolume;
};

#endif // __SRC_CORE_AUDIOMANAGER_HPP_
