//////////////////////////////////////////////////////////////////////////////
// This file is part of the LibreMaple MMORPG client                        //
// Copyright © 2015-2016 Daniel Allendorf, 2018-2019 LibreMaple Team        //
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
#include "SkillHitEffect.h"

#include "../../Util/Misc.h"

namespace jrc
{
SingleHitEffect::SingleHitEffect(WzNode src) : effect(src["hit"]["0"])
{
}

void SingleHitEffect::apply(const AttackUser& user, Mob& target) const
{
    effect.apply(target, user.flip);
}

TwoHHitEffect::TwoHHitEffect(WzNode src)
    : effects(src["hit"]["0"], src["hit"]["1"])
{
}

void TwoHHitEffect::apply(const AttackUser& user, Mob& target) const
{
    effects[user.second_weapon].apply(target, user.flip);
}

ByLevelHitEffect::ByLevelHitEffect(WzNode src)
{
    WzNode node_CharLevel = src["CharLevel"];
    for (auto node_sub = node_CharLevel.begin()  ; node_sub != node_CharLevel.end() ; ++node_sub ) {
        std::uint16_t level
            = string_conversion::or_zero<std::uint16_t>((*node_sub).second.name());
        effects.emplace(level, (*node_sub).second["hit"]["0"]);
    }
}

void ByLevelHitEffect::apply(const AttackUser& user, Mob& target) const
{
    if (effects.empty()) {
        return;
    }

    auto iter = effects.upper_bound(user.level);
    if (iter != effects.begin()) {
        --iter;
    }

    iter->second.apply(target, user.flip);
}

ByLevelTwoHHitEffect::ByLevelTwoHHitEffect(WzNode src)
{
    WzNode node_CharLevel = src["CharLevel"];
    for (auto node_sub = node_CharLevel.begin()  ; node_sub != node_CharLevel.end() ; ++node_sub ) {
        auto level = string_conversion::or_zero<std::uint16_t>((*node_sub).second.name());
        effects.emplace(
            std::piecewise_construct,
            std::forward_as_tuple(level),
            std::forward_as_tuple((*node_sub).second["hit"]["0"], (*node_sub).second["hit"]["1"]));
    }
}

void ByLevelTwoHHitEffect::apply(const AttackUser& user, Mob& target) const
{
    if (effects.empty()) {
        return;
    }

    auto iter = effects.upper_bound(user.level);
    if (iter != effects.begin()) {
        --iter;
    }

    iter->second[user.second_weapon].apply(target, user.flip);
}

BySkillLevelHitEffect::BySkillLevelHitEffect(WzNode src)
{
    WzNode node_Level = src["level"];
    for (auto node_sub = node_Level.begin()  ; node_sub != node_Level.end() ; ++node_sub ) {
        auto level = string_conversion::or_zero<std::int32_t>((*node_sub).second.name());
        effects.emplace(level, (*node_sub).second["hit"]["0"]);
    }
}

void BySkillLevelHitEffect::apply(const AttackUser& user, Mob& target) const
{
    auto iter = effects.find(user.skill_level);
    if (iter != effects.end()) {
        iter->second.apply(target, user.flip);
    }
}
} // namespace jrc
