#ifndef ENEMIESFACTORY_H
#define ENEMIESFACTORY_H

#include "Dungeon/Enemies/Bat.h"
#include "Dungeon/Enemies/BlueSlime.h"
#include "Dungeon/Enemies/GreenSlime.h"
#include "Dungeon/Enemies/OrangeSlime.h"
#include "Dungeon/Enemies/RedBat.h"

namespace Dungeon {
class EnemyFactory {
public:
    static std::shared_ptr<Enemy> CreateEnemy(const s_Enemy &u_Enemy) {
        // switch (u_Enemy.type) {
        // case 0:
        //     return std::make_shared<Enemies::GreenSlime>(u_Enemy);
        // case 1:
        //     return std::make_shared<Enemies::BlueSlime>(u_Enemy);
        // case 2:
        //     return std::make_shared<Enemies::OrangeSlime>(u_Enemy);
        // case 3:
        // case 4:
        // case 5:
        // case 6:
        //     return std::make_shared<Enemies::Bat>(u_Enemy);
        // case 7:
        //     return std::make_shared<Enemies::RedBat>(u_Enemy);

        // default:
        //     return nullptr;
        // }
        return std::make_shared<Enemies::GreenSlime>(u_Enemy);
    }
};
} // namespace Dungeon

#endif // ENEMIESFACTORY_H
