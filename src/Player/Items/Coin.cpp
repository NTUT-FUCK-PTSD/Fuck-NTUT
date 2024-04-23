//
// Created by adven on 2024/3/22.
//
#include "Player/Items/Coin.h"
#include "Util/Color.hpp"
#include "Util/Image.hpp"
#include "Util/Logger.hpp"

//
Coin::Coin()
    : Animation({0, 0}) {
    m_text = std::make_shared<Util::Text>(
        m_TextStylePath,
        m_fontSize,
        m_showText,
        Util::Color(255, 255, 255)
    );
    const auto CoinImage = std::make_shared<Util::Image>(m_imagePath);

    m_CoinText->SetDrawable(m_text);
    m_CoinImage->SetDrawable(CoinImage);

    m_CoinText->SetZIndex((float)m_ZIndex);
    m_CoinImage->SetZIndex((float)m_ZIndex);

    m_CoinText->SetScale(m_FontScale);
    m_CoinImage->SetScale(m_ImageScale);

    m_CoinImage->SetPosition(m_Position);

    toLeftSideCoinText();
}

std::vector<std::shared_ptr<Util::GameObject>> Coin::getGameObject() {
    return {m_CoinText, m_CoinImage};
}

void Coin::toLeftSideCoinText() {
    // left point
    glm::vec2       initPos = {m_Position.x + 50, m_Position.y};
    const glm::vec2 modifyPos = {
      initPos.x + m_text->GetSize().x * 2.5,
      initPos.y
    };
    m_CoinText->SetPosition(modifyPos);
    //    LOG_DEBUG((glm::vec2){a.x + m_text->GetSize().x,
    //    m_text->GetSize().y});
}

void Coin::plusCoinNumber(const int number) {
    m_Number += number;
    const std::string str = '*' + std::to_string(m_Number);
    m_text->SetText(str);

    toLeftSideCoinText();
}

void Coin::SetPosition(const glm::vec2& position) {
    m_CoinText->SetPosition(position);
    m_CoinImage->SetPosition(position);
}

std::shared_ptr<GameElement> Coin::GetGameObject() {
    m_Coin->AddChild(m_CoinImage);
    m_Coin->AddChild(m_CoinText);

    m_Coin->SetVisible(false);
    return m_Coin;
}

void Coin::Update() {
    UpdateAnimation(true);
    if (m_IsAnimating || m_AnimationPosition == m_AnimationDestination) {
        m_GamePosition = ToolBoxs::PosToGamePos(m_AnimationDestination);
        m_Position = m_AnimationPosition;
    }
    //    SetZIndex(m_AnimationZIndex);
    SetPosition({m_Position.x + 585, m_Position.y + 350});
}
