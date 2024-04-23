#include "Animation.h"

Animation::Animation(const glm::vec2& animationPosition)
    : m_AnimationPosition(animationPosition) {
    m_AnimationZIndex = ToolBoxs::PosToGamePos({0,
                                                m_AnimationPosition.y
                                                    - DUNGEON_TILE_WIDTH
                                                    - DUNGEON_TILE_WIDTH})
                            .y
                        / 1e2;
}

void Animation::MoveByTime(
    const unsigned long duringTimeMs,
    const glm::vec2&    destination,
    const uint16_t      direction
) {
    if (m_IsAnimating) {
        m_AnimationPosition = m_AnimationDestination;
        UpdateAnimation(true);
    }
    m_AnimationStartMs = Util::Time::GetElapsedTimeMs();
    m_AnimationDuringTimeMs = duringTimeMs;
    m_AnimationDestination = destination;
    m_IsAnimating = true;
    m_AnimationDirection = direction;
}

void Animation::MoveByTime(
    const unsigned long duringTimeMs,
    const glm::vec2&    destination
) {
    if (m_IsAnimating) {
        m_AnimationPosition = m_AnimationDestination;
        UpdateAnimation(false);
    }
    m_AnimationStartMs = Util::Time::GetElapsedTimeMs();
    m_AnimationDuringTimeMs = duringTimeMs;
    m_AnimationDestination = destination;
    m_IsAnimating = true;
}

void Animation::UpdateAnimation(const bool& isDirection) {
    if (m_IsAnimating) {
        unsigned long passTimeMs = Util::Time::GetElapsedTimeMs()
                                   - m_AnimationStartMs;

        if (passTimeMs > m_AnimationDuringTimeMs
            || ((m_AnimationPosition == m_AnimationDestination) && !isDirection
            )) {
            m_AnimationPosition = m_AnimationDestination;
            m_AnimationDestination = {0, 0};
            m_IsAnimating = false;
        } else {
            if (isDirection) {
                if (passTimeMs <= m_AnimationDuringTimeMs / 2.0f) {
                    float ratio = (float)passTimeMs
                                  / (m_AnimationDuringTimeMs / 2);
                    m_AnimationPosition += m_MoveAnimation[m_AnimationDirection]
                                           * ratio;
                } else {
                    float ratio = (float)(passTimeMs
                                          + (m_AnimationDuringTimeMs / 2))
                                  / (m_AnimationDuringTimeMs / 2);
                    m_AnimationPosition -= m_MoveAnimation[m_AnimationDirection]
                                           * (1.0f - ratio);
                }
            }
            float     ratio = (float)passTimeMs / m_AnimationDuringTimeMs;
            glm::vec2 move = m_AnimationDestination - m_AnimationPosition;
            m_AnimationPosition += move * ratio;
        }
    }
    m_AnimationZIndex = ToolBoxs::PosToGamePos({0,
                                                m_AnimationPosition.y
                                                    - DUNGEON_TILE_WIDTH
                                                    - DUNGEON_TILE_WIDTH})
                            .y
                        / 1e2;
}
