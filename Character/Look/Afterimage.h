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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////
#pragma once
#include "../../Graphics/Animation.h"
#include "../../Template/Rectangle.h"

namespace jrc
{
class Afterimage
{
public:
    Afterimage(std::int32_t skill_id,
               const std::string& name,
               const std::string& stance,
               std::int16_t level);
    Afterimage();

    void
    draw(std::uint8_t stframe, const DrawArgument& args, float alpha) const;
    void update(std::uint8_t stframe, std::uint16_t timestep);

    std::uint8_t get_first_frame() const;
    Rectangle<std::int16_t> get_range() const;

private:
    Animation animation;
    Rectangle<std::int16_t> range;
    std::uint8_t firstframe;
    bool displayed;
};
} // namespace jrc
