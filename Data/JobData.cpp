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
#include "JobData.h"

#include "../Util/Misc.h"
#include "nlnx/node.hpp"
#include "nlnx/nx.hpp"

namespace jrc
{
JobData::JobData(std::int32_t id)
{
    std::string strid = string_format::extend_id(id, 3);
    nl::node src = nl::nx::skill[strid + ".img"];
    nl::node strsrc = nl::nx::string["Skill.img"][strid];

    icon = src["info"]["icon"];

    name = strsrc["bookName"].get_string();

    for (nl::node sub : src["skill"]) {
        std::int32_t skill_id
            = string_conversion::or_zero<std::int32_t>(sub.name());
        if (skill_id == 0)
            continue;

        skills.push_back(skill_id);
    }
}

const std::vector<std::int32_t>& JobData::get_skills() const
{
    return skills;
}

const std::string& JobData::get_name() const
{
    return name;
}

const Texture& JobData::get_icon() const
{
    return icon;
}
} // namespace jrc
