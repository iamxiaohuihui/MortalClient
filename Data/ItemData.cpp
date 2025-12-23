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
#include "ItemData.h"

#include "Wz.h"

namespace jrc
{
ItemData::ItemData(std::int32_t id) : item_id(id)
{
    WzNode src;
    WzNode strsrc;

    std::string str_prefix = std::to_string(item_id / 10000);
    str_prefix.insert(0, "0", 1);
    str_prefix += ".img";

    std::string str_id = std::to_string(item_id);
    str_id.insert(0, "0", 1);

    switch (item_id / 1000000) {
    case 1:
        category = get_equip_category_str(item_id);
        src = WzFile::character[category][str_id + ".img"]["info"];
        strsrc = WzFile::string["Eqp.img"]["Eqp"][category]
                               [std::to_string(item_id)];
        break;
    case 2:
        category = "Consume";
        src = WzFile::item["Consume"][str_prefix][str_id]["info"];
        strsrc = WzFile::string["Consume.img"][std::to_string(item_id)];
        break;
    case 3:
        category = "Install";
        src = WzFile::item["Install"][str_prefix][str_id]["info"];
        strsrc = WzFile::string["Ins.img"][std::to_string(item_id)];
        break;
    case 4:
        category = "Etc";
        src = WzFile::item["Etc"][str_prefix][str_id]["info"];
        strsrc = WzFile::string["Etc.img"]["Etc"][std::to_string(item_id)];
        break;
    case 5:
        category = "Cash";
        src = WzFile::item["Cash"][str_prefix][str_id]["info"];
        strsrc = WzFile::string["Cash.img"][std::to_string(item_id)];
        break;
    }

    if (src.notNullObj()) {
        icons[false] = src["icon"];
        icons[true] = src["iconRaw"];
        price = src["price"];

        name = strsrc["name"].getString();
        desc = strsrc["desc"].getString();

        valid = true;
    } else {
        valid = false;
    }
}

std::string_view ItemData::get_equip_category_str(std::int32_t id) noexcept
{
    static constexpr std::string_view category_names[15] = {"Cap",
                                                            "Accessory",
                                                            "Accessory",
                                                            "Accessory",
                                                            "Coat",
                                                            "Longcoat",
                                                            "Pants",
                                                            "Shoes",
                                                            "Glove",
                                                            "Shield",
                                                            "Cape",
                                                            "Ring",
                                                            "Accessory",
                                                            "Accessory",
                                                            "Accessory"};

    std::size_t index = (static_cast<std::size_t>(id) / 10000ull) - 100ull;
    if (index < 15ull) {
        return category_names[index];
    } else if (index >= 30ull && index <= 70ull) {
        return "Weapon";
    } else {
        return {};
    }
}

bool ItemData::is_valid() const noexcept
{
    return valid;
}

ItemData::operator bool() const noexcept
{
    return is_valid();
}

std::int32_t ItemData::get_id() const noexcept
{
    return item_id;
}

std::int32_t ItemData::get_price() const noexcept
{
    return price;
}

std::string_view ItemData::get_name() const noexcept
{
    return name;
}

std::string_view ItemData::get_desc() const noexcept
{
    return desc;
}

std::string_view ItemData::get_category() const noexcept
{
    return category;
}

const Texture& ItemData::get_icon(bool raw) const noexcept
{
    return icons[raw];
}
} // namespace jrc
