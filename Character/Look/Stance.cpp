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
#include "Stance.h"

#include "../../Console.h"

namespace jrc
{
Stance::Id Stance::by_state(std::int8_t state)
{
    std::int8_t index = state / 2 - 1;
    if (index < 0 || index > 10) {
        return WALK1;
    }

    static constexpr Id state_values[10]
        = {WALK1, STAND1, JUMP, ALERT, PRONE, FLY, LADDER, ROPE, DEAD, SIT};
    return state_values[index];
}

Stance::Id Stance::by_id(std::uint8_t id)
{
    if (id <= NONE || id >= LENGTH) {
        return NONE;
    }

    return static_cast<Stance::Id>(id);
}

Stance::Id Stance::by_string(std::string_view name)
{
    for (const auto& iter : names) {
        if (iter.second == name) {
            return iter.first;
        }
    }

    Console::get().print(str::concat("Unhandled stance: ", name));
    return NONE;
}

bool Stance::is_climbing(Id value)
{
    return value == LADDER || value == ROPE;
}

Stance::Id Stance::base_of(Id value)
{
    switch (value) {
    case STAND2:
        return STAND1;
    case WALK2:
        return WALK1;
    default:
        return value;
    }
}

Stance::Id Stance::second_of(Id value)
{
    switch (value) {
    case STAND1:
        return STAND2;
    case WALK1:
        return WALK2;
    default:
        return value;
    }
}

const EnumMap<Stance::Id, std::string> Stance::names
    = {"",        "alert",   "dead",      "fly",     "heal",    "jump",
       "ladder",  "prone",   "proneStab", "rope",    "shot",    "shoot1",
       "shoot2",  "shootF",  "sit",       "stabO1",  "stabO2",  "stabOF",
       "stabT1",  "stabT2",  "stabTF",    "stand1",  "stand2",  "swingO1",
       "swingO2", "swingO3", "swingOF",   "swingP1", "swingP2", "swingPF",
       "swingT1", "swingT2", "swingT3",   "swingTF", "walk1",   "walk2"};
} // namespace jrc
