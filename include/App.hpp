#ifndef APP_HPP
#define APP_HPP
#include "Background.hpp"
#include "Character.hpp"
#include "GlobalType.h"
#include "SpriteSheet.hpp"
#include "Util/Root.hpp"
#include "pch.hpp" // IWYU pragma: export

class App {
public:
    enum class State {
        START,
        UPDATE,
        END,
    };

    State GetCurrentState() const {
        return m_CurrentState;
    }

    void InitStart();
    void Start();
    void FinishStart();

    void InitUpdate();
    void Update();
    void FinishUpdate();

    void End(); // NOLINT(readability-convert-member-functions-to-static)

private:
    void ValidTask();

private:
    State m_CurrentState = State::START;

    // Background
    std::vector<std::shared_ptr<Background>> m_SaveBackground;
    std::shared_ptr<SpriteSheet>             m_SpriteSheet;

    std::shared_ptr<Character> m_Player1;
    std::shared_ptr<Character> m_Player2;
    std::shared_ptr<Character> m_Protagonist;
    Util::Root                 m_Root;
};

#endif
