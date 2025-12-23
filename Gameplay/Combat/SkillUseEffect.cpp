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
#include "SkillUseEffect.h"

#include "../../Util/Misc.h"

namespace jrc
{
SingleUseEffect::SingleUseEffect(WzNode src) : effect(src["effect"])
{
}

void SingleUseEffect::apply(Char& target) const
{
    effect.apply(target);
}

TwoHUseEffect::TwoHUseEffect(WzNode src)
    : effects(src["effect"]["0"], src["effect"]["1"])
{
}

void TwoHUseEffect::apply(Char& target) const
{
    effects[target.is_two_handed()].apply(target);
}

MultiUseEffect::MultiUseEffect(WzNode src)
{
    std::int8_t no = -1;
    WzNode sub = src["effect"];
    for (auto  s_sub = sub.begin(); s_sub != sub.end(); ++s_sub)
    {
        /* code */
        effects.push_back((*s_sub).second);

        ++no;
        sub = src["effect" + std::to_string(no)];
    }
}

void MultiUseEffect::apply(Char& target) const
{
    for (const auto& effect : effects) {
        effect.apply(target);
    }
}

ByLevelUseEffect::ByLevelUseEffect(WzNode src)
{
    WzNode s_CharLevel = src["CharLevel"];
    for (auto s_sub = s_CharLevel.begin() ; s_sub != s_CharLevel.end(); ++s_sub) {
        auto sub = (*s_sub).second;
        auto level = string_conversion::or_zero<std::uint16_t>(sub.name());
        effects.emplace(level, sub["effect"]);
    }
}

void ByLevelUseEffect::apply(Char& target) const
{
    if (effects.empty()) {
        return;
    }

    auto iter = effects.upper_bound(target.get_level());
    if (iter != effects.begin()) {
        --iter;
    }

    iter->second.apply(target);
}

void IronBodyUseEffect::apply(Char& target) const
{
    target.show_iron_body();
}
} // namespace jrc
