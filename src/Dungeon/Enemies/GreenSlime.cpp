#include "Dungeon/Enemies/GreenSlime.h"

#include "Game/ToolBoxs.h"

namespace Dungeon {
Enemies::GreenSlime::GreenSlime(
    const s_Enemy&                       u_Enemy,
    const std::shared_ptr<SimpleMapData> simpleMapData
)
    : Enemy(u_Enemy, simpleMapData) {
    m_Animation = std::make_unique<Animation>(
        ToolBoxs::GamePostoPos(GetGamePosition())
    );
    m_NormalFrames = {0, 1, 2, 3};
    m_ShadowFrames = {4, 5, 6, 7};
    m_SpriteSheet = std::make_shared<SpriteSheet>(
        ASSETS_DIR "/entities/slime_green.png",
        m_FrameSize,
        m_NormalFrames,
        true,
        100,
        true,
        100
    );
    m_Drawable = m_SpriteSheet;
    m_WillMovePosition = GetGamePosition();

    SetHealth(2);    // 1 heart
    SetDamage(100);  // 50 hearts
    SetCoin(1);
}
}  // namespace Dungeon
namespace Dungeon::Enemies {
void GreenSlime::Move() {
    m_Animation->MoveByTime(200, ToolBoxs::GamePostoPos(GetGamePosition()), 0);
}  // Green_Slime does not move
void GreenSlime::Update() {
    if (m_Animation->IsAnimating()) {
        m_Transform.translation = m_Animation->GetAnimationPosition();
    }
}
}  // namespace Dungeon::Enemies
