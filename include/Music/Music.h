//
// Created by adven on 2024/3/27.
//

#ifndef FUCK_PTSD_MUSIC_H
#define FUCK_PTSD_MUSIC_H

#include "soloud.h"
#include "soloud_speech.h"
#include "soloud_thread.h"
#include "soloud_wav.h"

#include "BeatHeart.h"
#include "RhythmIndicator.h"
#include "Tempo.h"

namespace Music {
class Player final {
public:
    explicit Player();
    ~Player();

    void setEnable();

    void playMusic(const std::string &musicPath);

    void playMusic(const std::string &musicPath, bool setMusicLooping);

    // let music to this speed
    void setSpeed(float musicSpeed);

    // plus the speed to current speed, and play.
    void speedUp(float musicSpeed);

    void skipToTargetTime(float time);

    void clickEvent() { m_tempo.keyBoardClick(); };

    void readTempoFile(const std::string &path);

    bool TempoTrigger() { return m_tempo.canBeClick(); }

    std::shared_ptr<GameElement> getGameObject();

    void Update();

    void setTempoOffset(int16_t offset) { m_TempoOffset = offset; };
    void setTempoRange(std::size_t range) { m_TempoRange = range; };
    void setDisplayOffset(int16_t offset) { m_DisplayOffset = offset; };
    void setDisplayHeartBeatDurationTime(std::size_t time) {
        m_DisplayHeartBeatDurationTime = time;
    };

    std::size_t getTempoIndex();
    std::size_t getTempoIndex2() { return m_tempo.getTempoIndex2(); };
    std::size_t getTempoTime();

private:
    float m_currentSpeed = 1.0f;

    Tempo m_tempo;
    BeatHeart m_display;
    RhythmIndicator m_rhythmIndicator;

    /* ms */
    int16_t m_TempoOffset = 0;
    std::size_t m_TempoRange = 500;
    /* bool isShowHeartBeat = false; */
    bool isShowHeartBeat = true;
    int16_t m_DisplayOffset = -30;
    std::size_t m_DisplayRange = 100;
    std::size_t m_DisplayHeartBeatDurationTime = 80;

    SoLoud::handle m_musicHandle;

    SoLoud::Soloud m_soloud; // SoLoud engine core
    SoLoud::Speech m_speech; // A sound source (speech, in this case)
    std::shared_ptr<SoLoud::Wav> m_music;

    std::shared_ptr<GameElement> m_MusicObject =
        std::make_shared<GameElement>();
};
} // namespace Music

#endif // FUCK_PTSD_MUSIC_H
