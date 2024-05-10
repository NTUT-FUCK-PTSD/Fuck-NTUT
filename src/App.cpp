#include "App.hpp"

#include "Util/Logger.hpp"
#include "eventpp/utilities/argumentadapter.h"
#include "eventpp/utilities/conditionalfunctor.h"

#include <Util/Input.hpp>
#include "Actions.h"
#include "Display/BeatHeart.h"
#include "Display/BeatIndicator.h"
#include "Dungeon/Enemy.h"
#include "Dungeon/MapHandler.h"
#include "Event/Event.h"
#include "HPIS.h"
#include "Helper.hpp"
#include "Music/Player.h"
#include "Music/Tempo.h"
#include "System.h"
#include "Systems/HEIS.h"
#include "Systems/HPIS.h"

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
        // m_MusicSystem->playMusic(ASSETS_DIR "/music/intro_onlyMusic.ogg",
        // true);
        Music::Player::Init();
        Music::Player::PlayMusic(
            ASSETS_DIR "/music/intro_onlyMusic.ogg",
            true,
            0.5f,
            1.0f
        );

        //    m_MusicSystem->skipToTargetTime(118.2f);
    }

    // Wait any key click
    if (ToolBoxs::IsAnyKeyPress()) {
        m_IsMainMenu = false;
    }

    // m_MusicSystem->Update();
    m_Camera->Update();

    if (Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }
    if (m_IsMainMenu) {
        return;
    }
    // play lobby music
    Music::Player::StopMusic();
    Music::Player::PlayMusic(ASSETS_DIR "/music/lobby.ogg", true);
    Music::Player::SetVolume(0.1f);

    Music::Tempo::ReadTempoFile(ASSETS_DIR "/music/lobby.txt");

    // play zone1 leve1
    // m_MusicSystem->playMusic(ASSETS_DIR "/music/zone1_1.ogg", true);
    // m_MusicSystem->readTempoFile(ASSETS_DIR "/music/zone1_1.txt");

    // remove background
    m_Camera->RemoveChild(m_Background->GetGameElement());
    m_Background.reset();

    // create Player
    m_MainCharacter = std::make_shared<Player>();
    m_MainCharacter->SetHeadImage(ASSETS_DIR "/entities/player1_heads.png");
    m_MainCharacter->SetBodyImage(ASSETS_DIR "/entities/player1_armor_body.png"
    );
    Event::EventQueue.appendListener(
        EventType::AttackPlayer,
        eventpp::conditionalFunctor(
            eventpp::argumentAdapter<
                void(const Object*, const AttackPlayerEventArgs&)>(
                [this](const Object*, const AttackPlayerEventArgs& e) {
                    if (Event::GetAttackPlayer()) {
                        m_MainCharacter->lostHP(e.GetDamage());
                    }
                }
            ),
            // This lambda is the condition. We use dynamic_cast to check if
            // the event is desired. This is for demonstration purpose, in
            // production you may use a better way than dynamic_cast.
            [](const Object*, const EventArgs& e) {
                return dynamic_cast<const AttackPlayerEventArgs*>(&e)
                       != nullptr;
            }
        )
    );
    Event::EventQueue.appendListener(
        EventType::ResetMap,
        [this](const Object*, const EventArgs&) {
            m_MainCharacter->SetGamePosition({0, 0});
        }
    );
    m_Camera->AddChild(m_MainCharacter->GetGameElement());
    m_Camera->AddUIChild(m_MainCharacter->GetWindowElement());

    // Test the Dungeon::Map
    m_DungeonMap = std::make_shared<Dungeon::Map>(
        m_Camera,
        m_MainCharacter,
        ASSETS_DIR "/dungeon/MY DUNGEON.xml",
        1
    );

    m_DungeonMap->SetDrawable(std::make_shared<Dungeon::MapHandler>(m_DungeonMap
    ));
    m_Camera->AddChild(m_DungeonMap);

    Display::BeatHeart::Init();
    m_Camera->AddUIChild(Display::BeatHeart::GetGameElement());

    Display::BeatIndicator::Init();
    m_Camera->AddUIChild(Display::BeatIndicator::GetGameElement());

    // helper
    Settings::Helper::Init(m_DungeonMap.get());

    // add tools throw system
    Game::System::Init(m_DungeonMap.get());
    Game::Systems::HPIS::Init(m_MainCharacter.get());
    Game::Systems::HEIS::Init(m_DungeonMap.get());

    // Music::Player::PauseMusic(true);
    // Music::Player::PauseMusic(false);
    // Music::Tempo::Pause(false);
    // Display::BeatIndicator::Pause(false);
    // Display::BeatHeart::Pause(false);

    m_CurrentState = State::UPDATE;
}

void App::Update() {
    LOG_INFO(Music::Player::GetMusicTime());
    // LOG_INFO(Util::Time::GetElapsedTimeMs());
    //    LOG_INFO(1 / Util::Time::GetDeltaTime());

    auto musicTime =
        static_cast<std::size_t>(Music::Player::GetMusicTime() * 1000)
        % static_cast<std::size_t>(Music::Player::GetMusicLength() * 1000);

    if (Music::Tempo::IsSwitch()) {
        m_DungeonMap->TempoTrigger(Music::Tempo::GetBeatIdx());
        Display::BeatHeart::SwitchHeart(100);
        Event::SetAttackPlayer(true);
    }

    if (Util::Input::IsKeyDown(Util::Keycode::T)) {
        //        m_MainCharacter->GetToolMod()->RemoveTool("WEAPON", "Spear");
        Game::Systems::HPIS::ThrowOut(Player::Direction::NONE);
        //        const auto&  m = m_MainCharacter->GetGamePosition();
        //        const auto&& b = Settings::Helper::GamePosToMapIdx(m +
        //        glm::vec2{1, 0}); if
        //        (m_DungeonMap->GetMapData()->IsEnemyEmpty(b)) {
        //            LOG_INFO(m_DungeonMap->GetMapData()->GetEnemy(b)->GetHealth());
        //        }
    }

    if (Util::Input::IsKeyDown(Util::Keycode::N)) {
        m_DungeonMap->LoadLevel(
            m_DungeonMap->GetLevelNum() + 1,
            m_MainCharacter
        );
        m_AniCameraDestination = {0, 0};
        m_AniPlayerDestination = {0, 0};
    }

    if (Util::Input::IsKeyDown(Util::Keycode::UP)
        && Util::Input::IsKeyDown(Util::Keycode::DOWN)) {
        m_MainCharacter->PrepareThrowOut(true);
        LOG_INFO("Throw Mode");
        m_ThrowMode = true;
    }

    // player throw weapon
    if (m_ThrowMode
        && (Util::Input::IsKeyDown(Util::Keycode::W)
            || Util::Input::IsKeyDown(Util::Keycode::D)
            || Util::Input::IsKeyDown(Util::Keycode::S)
            || Util::Input::IsKeyDown(Util::Keycode::A))
        && Music::Tempo::IsTempoInRange(
            500,
            musicTime,
            Music::Player::LoopCounter()
        )) {
        m_MainCharacter->PrepareThrowOut(false);
        for (const auto& elem : m_MapTableCodeDire) {
            if (Util::Input::IsKeyDown(elem.first)) {
                //                Game::Actions::ThrowOutWeapon(m_DungeonMap.get(),
                //                elem.second);
                Game::Systems::HPIS::ThrowOut(elem.second);
            }
        }
        m_ThrowMode = false;

        // test
        m_MainCharacter
            ->MoveByTime(200, m_AniPlayerDestination, m_PlayerMoveDirection);
        m_MainCharacter->Update();
        m_Camera->MoveByTime(200, m_AniCameraDestination);
        m_DungeonMap->PlayerTrigger();
    }

    // player move
    else if (!m_ThrowMode
             && (Util::Input::IsKeyDown(Util::Keycode::W)
                 || Util::Input::IsKeyDown(Util::Keycode::D)
                 || Util::Input::IsKeyDown(Util::Keycode::S)
                 || Util::Input::IsKeyDown(Util::Keycode::A))
             && Music::Tempo::IsTempoInRange(
                 500,
                 musicTime,
                 Music::Player::LoopCounter()
             )) {
        glm::vec2 playerDestination = m_MainCharacter->GetGamePosition();

        if (m_PlayerMoveDirection != Player::NONE) {
            m_PlayerMoveDirection = Player::NONE;
        }
        const std::vector<Util::Keycode> key = {
          Util::Keycode::W,
          Util::Keycode::A,
          Util::Keycode::S,
          Util::Keycode::D
        };
        const std::vector<glm::vec2> direction =
            {{0, -1}, {-1, 0}, {0, 1}, {1, 0}};
        const std::vector<Player::Direction> playerDirection = {
          Player::Direction::UP,
          Player::Direction::LEFT,
          Player::Direction::DOWN,
          Player::Direction::RIGHT
        };
        const std::vector<glm::vec2> aniPlayerDirection = {
          {0, DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
          {-DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0},
          {0, -DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
          {DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0}
        };
        const std::vector<glm::vec2> aniCameraDirection = {
          {0, -DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
          {DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0},
          {0, DUNGEON_TILE_WIDTH * DUNGEON_SCALE},
          {-DUNGEON_TILE_WIDTH * DUNGEON_SCALE, 0}
        };

        auto mapIndex = m_DungeonMap->GamePostion2MapIndex(playerDestination);
        if (m_DungeonMap->GetMapData()->IsEnemyEmpty(mapIndex)) {
            m_DungeonMap->GetMapData()->GetEnemy(mapIndex)->Struck(2);
            m_Camera->Shake(150, 10);
            m_PlayerMoveDirection = Player::UP;

        } else {
            for (std::size_t i = 0; i < 4; i++) {
                if (Util::Input::IsKeyDown(key[i])
                    && m_DungeonMap->GetMapData()->IsPositionPlayerAct(
                        m_MainCharacter->GetGamePosition() + direction[i]
                    )) {
                    // origin mapdata actions
                    playerDestination = m_MainCharacter->GetGamePosition()
                                        + direction[i];
                    auto mapIndex = m_DungeonMap->GamePostion2MapIndex(
                        playerDestination
                    );
                    if (m_DungeonMap->GetMapData()->IsPositionInteractive(
                            playerDestination
                        )) {
                        if (m_DungeonMap->GetMapData()->IsEnemyEmpty(mapIndex
                            )) {
                            m_DungeonMap->GetMapData()
                                ->GetEnemy(mapIndex)
                                ->Struck(2);

                            // Game::Systems::HEIS::DetectHealth(mapIndex);

                            m_Camera->Shake(150, 10);
                        } else if (m_DungeonMap->GetMapData()->IsPositionWall(
                                       playerDestination
                                   )) {
                            m_DungeonMap->RemoveWall(
                                m_DungeonMap->GamePostion2MapIndex(
                                    playerDestination
                                )
                            );
                        } else if (m_DungeonMap->GetMapData()->IsPositionDoor(
                                       playerDestination
                                   )) {
                            m_DungeonMap->OpenDoor(
                                m_DungeonMap->GamePostion2MapIndex(
                                    playerDestination
                                )
                            );
                            m_Camera->Shake(100, 10);
                        }
                    } else {
                        m_PlayerMoveDirection = playerDirection[i];

                        m_AniPlayerDestination = {
                          m_AniPlayerDestination.x + aniPlayerDirection[i].x,
                          m_AniPlayerDestination.y + aniPlayerDirection[i].y
                        };
                        m_AniCameraDestination = {
                          m_AniCameraDestination.x + aniCameraDirection[i].x,
                          m_AniCameraDestination.y + aniCameraDirection[i].y
                        };
                    }
                }
            }
        }
        m_MainCharacter
            ->MoveByTime(200, m_AniPlayerDestination, m_PlayerMoveDirection);
        m_MainCharacter->Update();
        Event::EventQueue.dispatch(
            m_MainCharacter.get(),
            EventArgs(EventType::PlayerMove)
        );
        m_Camera->MoveByTime(200, m_AniCameraDestination);
        m_DungeonMap->PlayerTrigger();
    }

    // detect the player
    if (Util::Input::IsKeyDown(Util::Keycode::W)
        || Util::Input::IsKeyDown(Util::Keycode::D)
        || Util::Input::IsKeyDown(Util::Keycode::S)
        || Util::Input::IsKeyDown(Util::Keycode::A)) {
        // m_MusicSystem->clickEvent();
    }

    if (Util::Input::IsKeyUp(Util::Keycode::ESCAPE) || Util::Input::IfExit()) {
        m_CurrentState = State::END;
    }

    Game::System::Update();
    Display::BeatHeart::Update();
    Display::BeatIndicator::Update();
    Music::Tempo::Update(musicTime, 0u, Music::Player::LoopCounter());

    m_MainCharacter->Update();
    m_Camera->Update();
}

void App::End() {  // NOLINT(this method will mutate members in the future)
    LOG_TRACE("End");
}

std::map<Util::Keycode, Player::Direction> App::m_MapTableCodeDire = {
  {Util::Keycode::W, Player::Direction::UP},
  {Util::Keycode::D, Player::Direction::RIGHT},
  {Util::Keycode::S, Player::Direction::DOWN}
};
