#ifndef ANIMATION_H
#define ANIMATION_H

#include "Settings/Window.hpp"
#include "ToolBoxs.h"

#include "Util/Time.hpp"

class Animation {
public:
    Animation(const glm::vec2& animationPosition);

    virtual ~Animation() = default;

    virtual void MoveByTime(
        const unsigned long duringTimeMs,
        const glm::vec2&    destination,
        const uint16_t      direction
    );
    virtual void
    MoveByTime(const unsigned long duringTimeMs, const glm::vec2& destination);

protected:
    void UpdateAnimation(const bool& isDirection = false);

    const std::vector<glm::vec2> m_MoveAnimation = {
      {0, DUNGEON_TILE_WIDTH},
      {DUNGEON_TILE_WIDTH * 2 / 5, DUNGEON_TILE_WIDTH},
      {-DUNGEON_TILE_WIDTH / 4, DUNGEON_TILE_WIDTH},
      {-DUNGEON_TILE_WIDTH * 2 / 5, DUNGEON_TILE_WIDTH},
      {0, 0}
    };

    bool          m_IsAnimating = false;
    unsigned long m_AnimationStartMs = 0;
    unsigned long m_AnimationDuringTimeMs;
    glm::vec2     m_AnimationDestination = {1e9, 1e9};
    uint16_t      m_AnimationDirection;
    glm::vec2     m_AnimationPosition = {-1e9, -1e9};
    float         m_AnimationZIndex = 0.0f;
};

#endif
