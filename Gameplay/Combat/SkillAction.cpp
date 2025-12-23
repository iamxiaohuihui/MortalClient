// Copyright © 2015-2016 Daniel Allendorf                                   //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <https://www.gnu.org/licenses/>.   //
//////////////////////////////////////////////////////////////////////////////
#include "SkillAction.h"

#include "../../Util/Misc.h"

namespace jrc
{
void RegularAction::apply(Char& target, Attack::Type atk_type) const
{
    Weapon::Type weapon_type = target.get_weapon_type();
    bool degenerate;
    switch (weapon_type) {
    case Weapon::BOW:
    case Weapon::CROSSBOW:
    case Weapon::CLAW:
    case Weapon::GUN:
        degenerate = atk_type != Attack::RANGED;
        break;
    default:
        degenerate = false;
    }

    target.attack(degenerate);
}

SingleAction::SingleAction(WzNode src)
{
    action = src["action"]["0"].getString();
}

void SingleAction::apply(Char& target, Attack::Type) const
{
    target.attack(action);
}

TwoHAction::TwoHAction(WzNode src)
{
    actions[false] = src["action"]["0"].getString();
    actions[true] = src["action"]["1"].getString();
}

void TwoHAction::apply(Char& target, Attack::Type) const
{
    bool twohanded = target.is_two_handed();
    std::string action = actions[twohanded];

    target.attack(action);
}

ByLevelAction::ByLevelAction(WzNode src, std::int32_t id)
{
    WzNode s_level = src["level"];
    for (auto s_sub = s_level.begin(); s_sub != s_level.end(); ++s_sub) {
        std::int32_t level
            = string_conversion::or_zero<std::int32_t>((*s_sub).second.name());
        actions[level] = (*s_sub).second["action"].getString();
    }

    skillid = id;
}

void ByLevelAction::apply(Char& target, Attack::Type) const
{
    std::int32_t level = target.get_skill_level(skillid);
    auto iter = actions.find(level);
    if (iter != actions.end()) {
        target.attack(iter->second);
    }
}
} // namespace jrc
