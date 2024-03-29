#include "Dungeon/Enemies/OrangeSlime.h"

namespace Dungeon {
Enemies::OrangeSlime::OrangeSlime(const s_Enemy &u_Enemy)
    : Enemy(u_Enemy),
      Animation(ToolBoxs::GamePostoPos(GetGamePosition())),
      m_RandomGenerator(m_RandomDevice()),
      m_Distribution(0, 3) {
    m_NormalFrames = {0, 1, 2, 3};
    m_ShadowFrames = {4, 5, 6, 7};
    m_SpriteSheet = std::make_shared<SpriteSheet>(
        ASSETS_DIR "/entities/slime_orange.png", m_FrameSize, m_NormalFrames,
        true, 100, true, 100);
    m_Drawable = m_SpriteSheet;

    SetHealth(2); // 1 heart
    SetDamage(1); // 0.5 heart
    SetCoin(2);
    m_StartIdx = m_Distribution(m_RandomGenerator);
    m_State = -1; // Start from -1 to make the first move
    m_Movement[(m_StartIdx) % 4] += GetGamePosition();
    for (size_t i = 1; i < 4; i++) {
        m_Movement[(m_StartIdx + i) % 4] +=
            m_Movement[(m_StartIdx + i - 1) % 4];
    }
}
} // namespace Dungeon

namespace Dungeon::Enemies {
void OrangeSlime::Move() {
    if (m_IsAnimating) {
        m_AnimationPosition = m_AnimationDestination;
        Update();
    }
    m_NeedToMove = false;
    if (m_State > 3) {
        m_State = 0;
    }
    if (m_State == 0) {
        m_WillMovePosition = m_Movement[(m_StartIdx + m_State) % 4];
        m_NeedToMove = true;
    }
    else if (m_State == 1) {
        m_WillMovePosition = m_Movement[(m_StartIdx + m_State) % 4];
        m_NeedToMove = true;
    }
    else if (m_State == 2) {
        m_WillMovePosition = m_Movement[(m_StartIdx + m_State) % 4];
        m_NeedToMove = true;
    }
    else if (m_State == 3) {
        m_WillMovePosition = m_Movement[(m_StartIdx + m_State) % 4];
        m_NeedToMove = true;
    }
    m_State++;
}
void OrangeSlime::Update() {
    if (m_CanMove && !m_IsAnimating) {
        MoveByTime(200, ToolBoxs::GamePostoPos(m_WillMovePosition),
                   (m_StartIdx + m_State - 1) % 4);
        m_GamePosition = m_WillMovePosition;
        m_NeedToMove = false;
    }
    else if (!m_CanMove && m_NeedToMove) {
        m_State -= 1;
    }

    UpdateAnimation(true);
    if (m_IsAnimating || m_AnimationPosition == m_AnimationDestination) {
        m_Transform.translation = m_AnimationPosition;
    }
    SetZIndex(m_AnimationZIndex);
}
} // namespace Dungeon::Enemies
