#include "App.hpp"
#include "Background.hpp"
#include "Dungeon/MapHandler.h"
#include "Player.h"
#include "ToolBoxs.h"
#include "Util/Input.hpp"
#include "Util/Keycode.hpp"
#include "Util/Logger.hpp"
#include "Util/Time.hpp"
#include <thread>

using namespace tinyxml2;

extern "C" {
int32_t rusty_extern_c_integer();
}

// show the start background and listen the keypress
void App::Start() {
    LOG_TRACE("Start");
    if (m_FirstTime) {
        m_FirstTime = false;
        // create background
        m_Background = std::make_shared<Background>();
        m_Camera->AddChild(m_Background->GetGameElement());

        // play main background music
        m_MusicSystem->playMusic(ASSETS_DIR "/music/intro_onlyMusic.ogg", true);
        //    m_MusicSystem->skipToTargetTime(118.2f);
    }

    // Wait any key click
    if (ToolBoxs::IsAnyKeyPress()) {
        m_IsMainMenu = false;
    }

    m_MusicSystem->Update();
    m_Camera->Update();

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    if (m_IsMainMenu) {
        return;
    }
    // play lobby music
    //    m_MusicSystem->playMusic(ASSETS_DIR"/music/lobby.ogg", true);
    //    m_MusicSystem->readTempoFile(ASSETS_DIR"/music/lobby.txt");
    //    m_MusicSystem->setSpeed(1.2);

    // play zone1 leve1
    m_MusicSystem->playMusic(ASSETS_DIR "/music/zone1_1.ogg", true);
    m_MusicSystem->readTempoFile(ASSETS_DIR "/music/zone1_1.txt");

    // remove background
    m_Camera->RemoveChild(m_Background->GetGameElement());
    m_Background.reset();

    // create Player
    m_MainCharacter = std::make_shared<Player>();
    m_MainCharacter->SetHeadImage(ASSETS_DIR "/entities/player1_heads.png");
    m_MainCharacter->SetBodyImage(ASSETS_DIR
                                  "/entities/player1_armor_body.png");
    m_Camera->AddChild(m_MainCharacter->GetGameElement());
    m_Camera->AddUIChild(m_MainCharacter->GetWindowElement());

    // Test the Dungeon::Map
    m_DungeonMap = std::make_shared<Dungeon::Map>(
        m_Camera, m_MainCharacter, ASSETS_DIR "/dungeon/MY DUNGEON.xml", 1);
    m_DungeonMap->SetDrawable(
        std::make_shared<Dungeon::MapHandler>(m_DungeonMap));
    m_Camera->AddChild(m_DungeonMap);

    // display the tempo heart in music System
    m_Camera->AddUIChild(m_MusicSystem->getGameObject());

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    //    LOG_INFO(1 / Util::Time::GetDeltaTime());

    // add coin
    //    if (Util::Input::IsKeyDown(Util::Keycode::B)) {
    //        m_Coin->plusCoinNumber(10);
    //        m_Diamond->plusDiamondNumber(10);
    //    }
    auto tempoIndex = m_MusicSystem->getTempoIndex();
    if (m_BeforeTempoIndex != tempoIndex) {
        m_BeforeTempoIndex = tempoIndex;
        m_DungeonMap->TempoTrigger(tempoIndex);
    }

    if (Util::Input::IsKeyDown(Util::Keycode::N)) {
        m_DungeonMap->LoadLevel(m_DungeonMap->GetLevelNum() + 1);
        m_AniCameraDestination = {0, 0};
        m_AniPlayerDestination = {0, 0};
    }

    // player move
    if ((Util::Input::IsKeyDown(Util::Keycode::W) ||
         Util::Input::IsKeyDown(Util::Keycode::D) ||
         Util::Input::IsKeyDown(Util::Keycode::S) ||
         Util::Input::IsKeyDown(Util::Keycode::A)) &&
        m_MusicSystem->TempoTrigger()) {
        glm::vec2 playerDestination = m_MainCharacter->GetGamePosition();

        if (m_PlayerMoveDirect != Player::NONE) {
            m_PlayerMoveDirect = Player::NONE;
        }
        const std::vector<Util::Keycode> key = {
            Util::Keycode::W, Util::Keycode::A, Util::Keycode::S,
            Util::Keycode::D};
        const std::vector<glm::vec2> direction = {
            {0, -1}, {-1, 0}, {0, 1}, {1, 0}};
        const std::vector<Player::Direction> playerDirection = {
            Player::Direction::UP, Player::Direction::LEFT,
            Player::Direction::DOWN, Player::Direction::RIGHT};
        const std::vector<glm::vec2> aniPlayerDirection = {
            {0, DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
            {-DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0},
            {0, -DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
            {DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0}};
        const std::vector<glm::vec2> aniCameraDirection = {
            {0, -DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
            {DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0},
            {0, DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
            {-DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0}};

        for (std::size_t i = 0; i < 4; i++) {
            if (Util::Input::IsKeyDown(key[i]) &&
                m_DungeonMap->GetMapData()->IsPositionPlayerAct(
                    m_MainCharacter->GetGamePosition() + direction[i])) {
                playerDestination =
                    m_MainCharacter->GetGamePosition() + direction[i];
                m_MainCharacter->SetFaceTo(playerDirection[i]);
                auto mapIndex =
                    m_DungeonMap->GamePostion2MapIndex(playerDestination);
                if (m_DungeonMap->GetMapData()->IsPositionInteractive(
                        playerDestination)) {
                    if (m_DungeonMap->GetMapData()->IsHasEntity(mapIndex)) {
                        m_DungeonMap->RemoveEnemy(
                            m_DungeonMap->GamePostion2MapIndex(
                                playerDestination));
                        m_Camera->Shake(150, 10);
                    }
                    else if (m_DungeonMap->GetMapData()->IsPositionWall(
                                 playerDestination)) {
                        m_DungeonMap->RemoveWall(
                            m_DungeonMap->GamePostion2MapIndex(
                                playerDestination));
                    }
                    else if (m_DungeonMap->GetMapData()->IsPositionDoor(
                                 playerDestination)) {
                        m_DungeonMap->OpenDoor(
                            m_DungeonMap->GamePostion2MapIndex(
                                playerDestination));
                        m_Camera->Shake(100, 10);
                    }
                }
                else {
                    m_PlayerMoveDirect = playerDirection[i];

                    m_AniPlayerDestination = {
                        m_AniPlayerDestination.x + aniPlayerDirection[i].x,
                        m_AniPlayerDestination.y + aniPlayerDirection[i].y};
                    m_AniCameraDestination = {
                        m_AniCameraDestination.x + aniCameraDirection[i].x,
                        m_AniCameraDestination.y + aniCameraDirection[i].y};
                }
            }
        }
        m_MainCharacter->MoveByTime(200, m_AniPlayerDestination,
                                    m_PlayerMoveDirect);
        m_MainCharacter->Update();
        m_Camera->MoveByTime(200, m_AniCameraDestination);
        m_DungeonMap->PlayerTrigger();
    }

    // detect the player
    if (Util::Input::IsKeyDown(Util::Keycode::W) ||
        Util::Input::IsKeyDown(Util::Keycode::D) ||
        Util::Input::IsKeyDown(Util::Keycode::S) ||
        Util::Input::IsKeyDown(Util::Keycode::A)) {
        m_MusicSystem->clickEvent();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    //    LOG_INFO(rusty_extern_c_integer());

    // LOG_INFO("Music's tempo index: {}", m_MusicSystem->getTempoIndex());
    // LOG_INFO("Music's tempo time: {}ms", m_MusicSystem->getTempoTime());

    m_MusicSystem->Update();
    m_MainCharacter->Update();
    m_Camera->Update();
}

void App::End() { // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}
