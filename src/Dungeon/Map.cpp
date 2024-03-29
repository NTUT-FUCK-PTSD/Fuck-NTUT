#include "Dungeon/Map.h"
#include "Dungeon/EnemyFactory.h"

namespace Dungeon {

Map::Map(const std::shared_ptr<MainCharacter> &mainCharacter,
         const std::string &path, const int &levelNum)
    : m_MainCharacter(mainCharacter) {
    m_Level = std::make_unique<Level>(path, levelNum);

    m_Size = m_Level->GetLevelIndexMax() - m_Level->GetLevelIndexMin() +
             3; // add 3 for the border
    m_MapData.resize(m_Size.x * m_Size.y);
    size_t mapIndex = 0, tmpMapIndex = 0;

    for (auto &tile : m_Level->GetTiles()) {
        mapIndex = GamePostion2MapIndex({tile.x, tile.y});
        if (tile.type == 23 || tile.type == 24 || tile.type == 103 ||
            tile.type == 106 || tile.type == 111 || tile.type == 118) {
            m_MapData[mapIndex].tiles.push_back(std::make_shared<Tile>(s_Tile{
                tile.x, tile.y, 0, tile.zone, tile.torch, tile.cracked}));
            m_Tiles.push_back(m_MapData[mapIndex].tiles.back());
        }
        m_MapData[mapIndex].tiles.push_back(std::make_shared<Tile>(tile));
        m_Tiles.push_back(m_MapData[mapIndex].tiles.back());
    }

    std::vector<glm::ivec2> direction = {{1, 0}, {-1, 0}, {0, 1},  {0, -1},
                                         {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (int i = 1; i < m_Size.y - 1; i++) {
        for (int j = 1; j < m_Size.x - 1; j++) {
            mapIndex = j + i * m_Size.x;
            if (m_MapData[mapIndex].tiles.empty()) {
                continue;
            }
            // door direction detection
            auto tmp = m_MapData[mapIndex].tiles.back();
            size_t doorCount = 0;
            if (tmp->GetTile().type == 103 || tmp->GetTile().type == 106 ||
                tmp->GetTile().type == 111 || tmp->GetTile().type == 118) {
                for (size_t k = 2; k < 4; k++) {
                    tmpMapIndex =
                        j + direction[k].x + (i + direction[k].y) * m_Size.x;
                    if (j + direction[k].x >= 0 &&
                        j + direction[k].x < m_Size.x &&
                        i + direction[k].y >= 0 &&
                        i + direction[k].y < m_Size.y &&
                        !m_MapData[tmpMapIndex].tiles.empty() &&
                        (m_MapData[tmpMapIndex].tiles.back()->IsWall())) {
                        doorCount++;
                    }
                }
                if (doorCount >= 2) {
                    m_MapData[mapIndex].tiles.pop_back();
                    m_Tiles.erase(
                        std::remove(m_Tiles.begin(), m_Tiles.end(), tmp),
                        m_Tiles.end()); // Remove Child
                    if (tmp->GetTile().type == 111) {
                        m_MapData[mapIndex].tiles.push_back(
                            std::make_shared<Tile>(s_Tile{
                                tmp->GetTile().x, tmp->GetTile().y, 52,
                                tmp->GetTile().zone, tmp->GetTile().torch,
                                tmp->GetTile().cracked}));
                    }
                    else {
                        if (tmp->GetTile().type == 103 ||
                            tmp->GetTile().type == 118) {
                            m_MapData[mapIndex].tiles.push_back(
                                std::make_shared<Tile>(s_Tile{
                                    tmp->GetTile().x, tmp->GetTile().y, 50,
                                    tmp->GetTile().zone, tmp->GetTile().torch,
                                    tmp->GetTile().cracked}));
                        }
                        else {
                            m_MapData[mapIndex].tiles.push_back(
                                std::make_shared<Tile>(s_Tile{
                                    tmp->GetTile().x, tmp->GetTile().y, 51,
                                    tmp->GetTile().zone, tmp->GetTile().torch,
                                    tmp->GetTile().cracked}));
                        }
                    }
                    m_Tiles.push_back(m_MapData[mapIndex].tiles.back());
                }
            }
            // generate border
            if (m_MapData[mapIndex].tiles.back()->GetTile().type != 102) {
                for (auto &dir : direction) {
                    tmpMapIndex = j + dir.x + (i + dir.y) * m_Size.x;
                    if (i + dir.y >= 0 && i + dir.y < m_Size.y &&
                        j + dir.x >= 0 && j + dir.x < m_Size.x &&
                        m_MapData[tmpMapIndex].tiles.empty()) {
                        m_MapData[tmpMapIndex].tiles.push_back(
                            std::make_shared<Tile>(s_Tile{
                                j + dir.x + m_Level->GetLevelIndexMin().x - 1,
                                i + dir.y + m_Level->GetLevelIndexMin().y - 1,
                                102, 0, 0, 0}));
                        m_Tiles.push_back(m_MapData[tmpMapIndex].tiles.back());
                    }
                }
            }
        }
    }

    // SetCloseDisplayb24
    for (int i = 0; i < m_Size.y; i++) {
        for (int j = 0; j < m_Size.x; j++) {
            mapIndex = j + i * m_Size.x;
            if (m_MapData[mapIndex].tiles.empty()) {
                continue;
            }
            if ((1 + i >= m_Size.y) ||
                (1 + i >= 0 && m_MapData[mapIndex + m_Size.x].tiles.empty())) {
                m_MapData[mapIndex].tiles.back()->SetCloseDisplayb24(true);
            }
        }
    }

    for (auto &enemy : m_Level->GetEnemies()) {
        mapIndex = GamePostion2MapIndex({enemy.x, enemy.y});
        m_MapData[mapIndex].enemies.push_back(EnemyFactory::CreateEnemy(enemy));
        m_Enemies.push_back(m_MapData[mapIndex].enemies.back());
    }

    // Add a Bat for testing
    m_MapData[GamePostion2MapIndex({1, 1})].enemies.push_back(
        std::make_shared<Enemies::OrangeSlime>(s_Enemy{1, 1, 0, 0, 0}));
    m_Enemies.push_back(m_MapData[GamePostion2MapIndex({1, 1})].enemies.back());

    for (auto &tile : m_Tiles) {
        m_Children.push_back(tile);
    }
    for (auto &enemy : m_Enemies) {
        m_Children.push_back(enemy);
    }
    CameraUpdate();
}

bool Map::CheckShowPosition(const glm::vec2 &position1,
                            const glm::vec2 &position2) {
    return (position1.x >= position2.x - (ALLOW_EXTRA_DRAW + HalfColNumber) &&
            position1.x <= position2.x + (ALLOW_EXTRA_DRAW + HalfColNumber) &&
            position1.y >= position2.y - (ALLOW_EXTRA_DRAW + HalfRowNumber) &&
            position1.y <= position2.y + (ALLOW_EXTRA_DRAW + HalfRowNumber));
}

void Map::CameraUpdate() {
    glm::vec2 cameraPos = m_MainCharacter->GetGamePosition();

    for (auto &tile : m_Tiles) {
        if (CheckShowPosition({tile->GetTile().x, tile->GetTile().y},
                              cameraPos)) {
            tile->SetVisible(true);
        }
        else {
            tile->SetVisible(false);
        }
    }
    for (auto &enemy : m_Enemies) {
        if (CheckShowPosition(enemy->GetGamePosition(), cameraPos)) {
            enemy->SetVisible(true);
        }
        else {
            enemy->SetVisible(false);
        }
    }
}

void Map::TempoUpdate() {
    for (auto &enemy : m_Enemies) {
        enemy->TempoMove();
    }
}

void Map::Update() {
    size_t mapIndex = 0;
    CameraUpdate();
    for (auto &enemy : m_Enemies) {
        if (!(enemy->GetGamePosition() == enemy->GetWillMovePosition()) &&
            isVaildMove(enemy->GetWillMovePosition())) {
            enemy->SetCanMove(true);

            mapIndex = GamePostion2MapIndex(enemy->GetGamePosition());
            m_MapData[mapIndex].enemies.erase(
                std::remove(m_MapData[mapIndex].enemies.begin(),
                            m_MapData[mapIndex].enemies.end(), enemy),
                m_MapData[mapIndex].enemies.end());
            m_MapData[GamePostion2MapIndex(enemy->GetWillMovePosition())]
                .enemies.push_back(enemy);
        }
        else {
            enemy->SetCanMove(false);
        }
        enemy->SetPlayerPosition(m_MainCharacter->GetGamePosition());
        enemy->Update();
    }
}

size_t Map::GamePostion2MapIndex(const glm::ivec2 &position) const {
    return (position.x - m_Level->GetLevelIndexMin().x + 1) +
           (position.y - m_Level->GetLevelIndexMin().y + 1) * m_Size.x;
}
bool Map::isVaildPosition(const glm::ivec2 &position) {
    if (position.x < m_Level->GetLevelIndexMin().x ||
        position.x > m_Level->GetLevelIndexMax().x ||
        position.y < m_Level->GetLevelIndexMin().y ||
        position.y > m_Level->GetLevelIndexMax().y) {
        return false;
    }
    return true;
}

bool Map::isVaildMove(const glm::ivec2 &position) {
    size_t mapIndex = GamePostion2MapIndex(position);
    if (m_MapData[mapIndex].tiles.empty()) {
        return false;
    }
    if (m_MapData[mapIndex].tiles.back()->IsWall()) {
        return false;
    }
    return true;
}

} // namespace Dungeon
