//
// Created by adven on 2024/3/24.
//

#ifndef FUCK_PTSD_WEAPON_H
#define FUCK_PTSD_WEAPON_H

#include "pch_extream.h"

#include "Equipment.h"
#include "ToolBoxs.h"

class Weapon final : public Equipment {
public:
    explicit Weapon();

    [[nodiscard]] std::shared_ptr<GameElement> getGameElement() override;

    void setPosition(const glm::vec2 position) override;

    std::vector<std::shared_ptr<Util::GameObject>> getGameObjects() override;


private:
    std::size_t m_ZIndex = 50;
    glm::vec2 m_Scale = {3,3 };
    glm::vec2 m_Position = {0, 0};

    //    ASSETS_DIR "/gui/diamond.png"

    std::string m_ImagePathWindow = ASSETS_DIR "/gui/hud_slot_2.png";
    std::string m_ImagePathItem = ASSETS_DIR "/items/weapon_dagger.png";

    std::shared_ptr<GameElement> m_Window = std::make_shared<GameElement>();
    std::shared_ptr<GameElement> m_Item = std::make_shared<GameElement>();
    std::shared_ptr<GameElement> m_Attack = std::make_shared<GameElement>();
};

#endif // FUCK_PTSD_WEAPON_H
