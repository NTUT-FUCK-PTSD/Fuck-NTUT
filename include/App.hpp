#ifndef APP_HPP
#define APP_HPP

#include "Background.hpp"
#include "Camera.h"
#include "Core/Context.hpp"
#include "Core/Drawable.hpp"
#include "Debug/DebugSystem.h"
#include "Dungeon/Map.h"
#include "Items/Tools.h"
#include "Music/Music.h"
#include "Player.h"
#include "Player/Equipment/Shovel.h"
#include "Player/Items/Coin.h"
#include "Player/Items/Diamond.h"
#include "Player/Items/Heart.h"
#include "SpriteSheet.hpp"
#include "Util/Text.hpp"
#include "pch.hpp" // IWYU pragma: export

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const { return m_CurrentState; }

    void Start(std::shared_ptr<Core::Context> context);

    void Update();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

    std::string Print(std::string s) { return s; }

private:
    void ValidTask();

private:
    // debug window
    std::shared_ptr<DebugSystem> m_DebugSystem;
        // music
        std::shared_ptr<MusicSystem> m_MusicSystem =
            std::make_shared<MusicSystem>();

    // settings
    glm::vec2 m_AniPlayerDestination = {0.0f, 0.0f};
    glm::vec2 m_AniCameraDestination = {0.0f, 0.0f};

    State m_CurrentState = State::START;

    Player::Direction m_PlayerMoveDirect = Player::Direction::NONE;

    std::shared_ptr<Player> m_MainCharacter;

    std::shared_ptr<Camera> m_Camera = std::make_shared<Camera>();
    std::shared_ptr<Camera> m_Window = std::make_shared<Camera>();

    glm::vec2 m_CameraPosition = {0, 0};

    std::shared_ptr<Dungeon::Map> m_DungeonMap;
};

#endif
