#include "Dungeon/MiniMap.h"

namespace Dungeon {
MiniMap::MiniMap(std::shared_ptr<MapData> mapData)
    : m_MapData(mapData) {
    BuildMiniMap();
    Update();
}

void MiniMap::BuildMiniMap() {
    m_ColorCubes.clear();
    m_Children.clear();
    std::size_t mapIndex = 0;
    for (int i = 0; i < m_MapData->GetSize().y; i++) {
        for (int j = 0; j < m_MapData->GetSize().x; j++) {
            mapIndex = j + i * m_MapData->GetSize().x;
            m_ColorCubes.push_back(std::make_shared<ColorCube>());
            m_Children.push_back(std::make_shared<Util::GameObject>(
                m_ColorCubes.back(), 100 - (1e-1)));
            SetVisible(mapIndex, true);
            m_Children[mapIndex]->m_Transform.scale = {m_Scale, m_Scale};
            m_Children[mapIndex]->m_Transform.translation = {
                static_cast<int>(WINDOW_WIDTH) / 2 - DUNGEON_TILE_WIDTH -
                    (2 * (m_MapData->GetSize().x - (j + 1))) * m_Scale,
                -(static_cast<int>(WINDOW_HEIGHT) / 2) +
                    (52 * DUNGEON_SCALE + DUNGEON_TILE_WIDTH) +
                    (2 * (m_MapData->GetSize().y - (i + 1))) * m_Scale};
            UpdateTileColor(mapIndex);
            SetVisible(mapIndex, false);
        }
    }
}

void MiniMap::UpdateTileColor(const std::size_t mapIndex) {
    if (!m_MapData->IsTilesEmpty(mapIndex)) {
        if (m_MapData->GetTileBack(mapIndex)->IsWall()) {
            if (m_MapData->GetTileBack(mapIndex)->GetTile().type == 102) {
                SetColor(mapIndex, CubeColor::gray);
            }
            else if (m_MapData->GetTileBack(mapIndex)->GetTile().type == 107) {
                SetColor(mapIndex, CubeColor::stone);
            }
            else if (m_MapData->GetTileBack(mapIndex)->GetTile().type == 104 ||
                     m_MapData->GetTileBack(mapIndex)->GetTile().type == 105 ||
                     m_MapData->GetTileBack(mapIndex)->GetTile().type == 110) {
                SetColor(mapIndex, CubeColor::metrognome);
            }
            else {
                SetColor(mapIndex, CubeColor::wall);
            }
        }
        else if (m_MapData->GetTileBack(mapIndex)->IsDoor()) {
            SetColor(mapIndex, CubeColor::green);
        }
        else {
            SetColor(mapIndex, CubeColor::white);
        }
    }
}

void MiniMap::SetColor(const glm::vec2 &position, CubeColor color) {
    auto mapIndex = m_MapData->GamePosition2MapIndex(position);
    SetColor(mapIndex, color);
}

void MiniMap::SetColor(const std::size_t position, CubeColor color) {
    if (m_ColorCubes[position]->GetColor() == color) {
        return;
    }
    m_ColorCubes[position]->SetColor(color);
}

void MiniMap::SetVisible(const glm::vec2 &position, bool visible) {
    auto mapIndex = m_MapData->GamePosition2MapIndex(position);
    SetVisible(mapIndex, visible);
}

void MiniMap::SetVisible(const std::size_t position, bool visible) {
    m_Children[position]->SetVisible(visible);
}

void MiniMap::Update() {
    for (int i = 0; i < m_MapData->GetSize().y; i++) {
        for (int j = 0; j < m_MapData->GetSize().x; j++) {
            auto mapIndex = j + i * m_MapData->GetSize().x;
            if (!m_ColorCubes[mapIndex]->GetAvailable()) {
                continue;
            }
            // Update Enemy
            auto enemy = m_MapData->GetEnemy(mapIndex);
            if (enemy) {
                if (!enemy->GetSeen()) {
                    continue;
                }
                SetColor(mapIndex, CubeColor::red);
                SetVisible(mapIndex, true);
                continue;
            }
            // Update Tiles
            if (!m_MapData->IsTilesEmpty(mapIndex)) {
                if (!m_MapData->GetTileBack(mapIndex)->GetSeen()) {
                    continue;
                }
            }
            else {
                SetVisible(mapIndex, false);
                continue;
            }
            UpdateTileColor(mapIndex);
            SetVisible(mapIndex, true);
        }
    }

    // Update Player
    auto dTime = Util::Time::GetElapsedTimeMs() - m_LastPlayerChanged;
    if (dTime < 250) {
        auto mapIndex =
            m_MapData->GamePosition2MapIndex(m_MapData->GetPlayerPosition());
        SetColor(mapIndex, CubeColor::blue);
    }
    else if (dTime > 500) {
        m_PlayerStatus = !m_PlayerStatus;
        m_LastPlayerChanged = Util::Time::GetElapsedTimeMs();
    }
}

void MiniMap::SetScale(double scale) {
    m_Scale = scale;
    BuildMiniMap();
    Update();
}
} // namespace Dungeon
