#include "Dungeon/SimpleMapData.h"

namespace Dungeon {
SimpleMapData::SimpleMapData(const glm::ivec2 &levelIndexMin,
                             const glm::ivec2 &levelIndexMax,
                             const glm::ivec2 &size)
    : m_LevelIndexMin(levelIndexMin),
      m_LevelIndexMax(levelIndexMax),
      m_Size(size) {
    m_Tiles.resize(m_Size.x * m_Size.y);
    m_HasEntity.resize(m_Size.x * m_Size.y, false);
}

void SimpleMapData::AddTile(const size_t &position,
                            const std::shared_ptr<Tile> &tile) {
    m_Tiles.at(position).push_back(tile);
}

void SimpleMapData::RemoveTile(const size_t &position,
                               const std::shared_ptr<Tile> &tile) {
    m_Tiles.at(position).erase(std::remove(m_Tiles.at(position).begin(),
                                           m_Tiles.at(position).end(), tile),
                               m_Tiles.at(position).end());
}

void SimpleMapData::PopbackTile(const size_t &position) {
    m_Tiles.at(position).pop_back();
}

bool SimpleMapData::IsTilesEmpty(const size_t &position) const {
    return m_Tiles.at(position).empty();
}

std::vector<std::shared_ptr<Tile>>
SimpleMapData::GetTiles(const size_t &position) const {
    return m_Tiles.at(position);
}

std::shared_ptr<Tile> SimpleMapData::GetTileBack(const size_t &position) const {
    return m_Tiles.at(position).back();
}

glm::ivec2 SimpleMapData::GetLevelIndexMax() const {
    return m_LevelIndexMax;
}

glm::ivec2 SimpleMapData::GetLevelIndexMin() const {
    return m_LevelIndexMin;
}

void SimpleMapData::SetLevelIndexMax(const glm::ivec2 &levelIndexMax) {
    m_LevelIndexMax = levelIndexMax;
}

void SimpleMapData::SetLevelIndexMin(const glm::ivec2 &levelIndexMin) {
    m_LevelIndexMin = levelIndexMin;
}

size_t SimpleMapData::GamePostion2MapIndex(const glm::ivec2 &position) const {
    return (position.x - GetLevelIndexMin().x + 1) +
           (position.y - GetLevelIndexMin().y + 1) * m_Size.x;
}

bool SimpleMapData::IsPositionValid(const glm::ivec2 &position) const {
    return position.x >= GetLevelIndexMin().x &&
           position.x <= GetLevelIndexMax().x &&
           position.y >= GetLevelIndexMin().y &&
           position.y <= GetLevelIndexMax().y;
}

bool SimpleMapData::IsHasEntity(const size_t &position) const {
    return m_HasEntity.at(position);
}

bool SimpleMapData::IsWalkable(const size_t &position) const {
    return !IsTilesEmpty(position) && !GetTileBack(position)->IsWall() &&
           !GetTileBack(position)->IsDoor() && !m_HasEntity.at(position);
}

bool SimpleMapData::IsPositionWalkable(const glm::ivec2 &position) const {
    return IsPositionValid(position) &&
           IsWalkable(GamePostion2MapIndex(position));
}

glm::ivec2 SimpleMapData::GetSize() const {
    return m_Size;
}

void SimpleMapData::SetSize(const glm::ivec2 &size) {
    m_Size = size;
}

void SimpleMapData::SetHasEntity(const size_t &position,
                                 const bool &hasEntity) {
    m_HasEntity.at(position) = hasEntity;
}

glm::vec2 SimpleMapData::GetPlayerPosition() const {
    return m_PlayerPosition;
}

void SimpleMapData::SetPlayerPosition(const glm::vec2 &playerPosition) {
    m_PlayerPosition = playerPosition;
}

} // namespace Dungeon
