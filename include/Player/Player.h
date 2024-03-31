//
// Created b adven on 2024/3/4.
//

#ifndef FUCK_PTSD_PLAYER_H
#define FUCK_PTSD_PLAYER_H

#include "Animation.h"
#include "GameElement.h"
#include "Items/Coin.h"
#include "Items/Diamond.h"
#include "Items/Heart.h"
#include "Items/Tools.h"
#include "SpriteSheet.hpp"
#include "ToolBoxs.h"

class Player : public Animation {
public:
    enum Direction { UP = 0, RIGHT, DOWN, LEFT, NONE };

    explicit Player(const std::string &headImagePath,
                    const std::string &bodyImagePath);

    explicit Player();

    virtual ~Player() = default;

    std::shared_ptr<GameElement> GetGameElement();

    [[nodiscard]] glm::vec2 GetGamePosition();

    std::shared_ptr<GameElement> GetWindowElement();

    void SetHeadImage(const std::string &path);
    void SetHeadImage(std::shared_ptr<SpriteSheet> image);
    void SetBodyImage(const std::string &path);
    void SetBodyImage(std::shared_ptr<SpriteSheet> image);

    void gainCoin(std::size_t number);
    void lostCoin(std::size_t number);
    void gainDiamond(std::size_t number);
    void lostDiamond(std::size_t number);

    void SetGamePosition(const glm::vec2 &position);

    void SetBodyImagePath(const std::string &Path) { m_BodyImagePath = Path; };

    void SetHeadImagePath(const std::string &Path) { m_HeadImagePath = Path; }

    void SetFaceTo(Direction direction);

    void SetZIndex(float index);

    void UpdateCoin(const unsigned long &duringTimeMs,
                    const glm::vec2 &destination, const uint16_t &direction);

    void Update();

private:
    void rendererPlayer(const std::string &headImagePath,
                        const std::string &bodyImagePath);

    std::string m_HeadImagePath;
    std::string m_BodyImagePath;

    glm::vec2 HeadSize;
    glm::vec2 BodySize;
    std::shared_ptr<SpriteSheet> HeadImage;
    std::shared_ptr<SpriteSheet> BodyImage;

    std::shared_ptr<GameElement> m_Body = std::make_shared<GameElement>();
    std::shared_ptr<GameElement> m_Head = std::make_shared<GameElement>();
    std::shared_ptr<GameElement> m_Player = std::make_shared<GameElement>();

    std::unique_ptr<Coin> m_Coin = std::make_unique<Coin>();
    std::unique_ptr<Heart> m_Heart = std::make_unique<Heart>();
    std::unique_ptr<Tools> m_Tools = std::make_unique<Tools>();
    std::unique_ptr<Diamond> m_Diamond = std::make_unique<Diamond>();
    std::shared_ptr<GameElement> m_Window = std::make_shared<GameElement>();

    glm::vec2 m_Position = {0, 0};
    glm::vec2 m_GamePosition = {0, 0};

    float m_ZIndex = 0.5;

    void SetPosition(const glm::vec2 &position);
};

#endif // FUCK_PTSD_PLAYER_H
