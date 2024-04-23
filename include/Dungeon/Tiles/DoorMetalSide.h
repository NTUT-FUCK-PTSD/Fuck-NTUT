#ifndef DOORMETALSIDE_H
#define DOORMETALSIDE_H

#include "Dungeon/Tiles/GeneralDoorSide.h"

namespace Dungeon {
namespace Tiles {
class DoorMetalSide final : public GeneralDoorSide {
public:
    explicit DoorMetalSide(const s_Tile& u_Tile);
    virtual ~DoorMetalSide() override = default;

    void UpdateDrawable() override;

private:
    void UpdateDoorMetalSideDrawable();
};
}  // namespace Tiles
}  // namespace Dungeon

#endif  // DOORMETALSIDE_H
