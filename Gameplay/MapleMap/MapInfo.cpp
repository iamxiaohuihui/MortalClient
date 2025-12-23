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
#include "MapInfo.h"

#include "../../Constants.h"
#include "../../Util/Misc.h"

#include <string_view>

namespace jrc
{
MapInfo::MapInfo(WzNode src,
                 Range<std::int16_t> walls,
                 Range<std::int16_t> borders)
{
    WzNode info = src["info"];
    if (info["VRLeft"].getNodeType() == WzNode::NodeType::INTEGER) {
        map_walls = {info["VRLeft"], info["VRRight"]};
        map_borders = {info["VRTop"], info["VRBottom"]};
        map_borders = {map_borders.first() + Constants::VIEW_Y_OFFSET,
                       map_borders.second() - Constants::VIEW_Y_OFFSET};
    } else {
        map_walls = walls;
        map_borders = borders;
    }

    std::string bgm_path = info["bgm"];
    std::size_t split = bgm_path.find('/');
    std::string_view bgm_path_view = bgm_path;
    bgm = str::concat(bgm_path_view.substr(0, split),
                      ".img/",
                      bgm_path_view.substr(split + 1));

    cloud = info["cloud"].getBoolean();
    fieldlimit = info["fieldLimit"];
    hide_minimap = info["hideMinimap"].getBoolean();
    map_mark = info["mapMark"].getString();
    swim = info["swim"].getBoolean();
    town = info["town"].getBoolean();

    WzNode node_seat = src["seat"];
    for (auto  sub_seat = node_seat.begin() ; sub_seat != node_seat.end() ; ++sub_seat ){
        /* code */
        seats.push_back((*sub_seat).second);
    }
    
    // for (auto&& seat : src["seat"]) {
    //     seats.push_back(seat);
    // }

     WzNode node_ladderRope = src["ladderRope"];
    for (auto  sub_ladderRope = node_ladderRope.begin() ; sub_ladderRope != node_ladderRope.end() ; ++sub_ladderRope ){
        /* code */
        ladders.push_back((*sub_ladderRope).second);
    }
    // for (auto&& ladder : src["ladderRope"]) {
    //     ladders.push_back(ladder);
    // }
}

MapInfo::MapInfo() = default;

bool MapInfo::is_underwater() const
{
    return swim;
}

std::string MapInfo::get_bgm() const
{
    return bgm;
}

Range<std::int16_t> MapInfo::get_walls() const
{
    return map_walls;
}

Range<std::int16_t> MapInfo::get_borders() const
{
    return map_borders;
}

nullable_ptr<const Seat> MapInfo::find_seat(Point<std::int16_t> position) const
{
    for (auto& seat : seats) {
        if (seat.in_range(position)) {
            return seat;
        }
    }
    return nullptr;
}

nullable_ptr<const Ladder> MapInfo::find_ladder(Point<std::int16_t> position,
                                                bool upwards) const
{
    for (auto& ladder : ladders) {
        if (ladder.in_range(position, upwards)) {
            return ladder;
        }
    }
    return nullptr;
}

Seat::Seat(WzNode src)
{
    pos = src;
}

bool Seat::in_range(Point<std::int16_t> position) const
{
    auto hor = Range<std::int16_t>::symmetric(position.x(), 10);
    auto ver = Range<std::int16_t>::symmetric(position.y(), 10);
    return hor.contains(pos.x()) && ver.contains(pos.y());
}

Point<std::int16_t> Seat::get_pos() const
{
    return pos;
}

Ladder::Ladder(WzNode src)
{
    x = src["x"];
    y1 = src["y1"];
    y2 = src["y2"];
    ladder = src["l"].getBoolean();
}

bool Ladder::is_ladder() const
{
    return ladder;
}

bool Ladder::in_range(Point<std::int16_t> position, bool upwards) const
{
    auto hor = Range<std::int16_t>::symmetric(position.x(), 10);
    auto ver = Range<std::int16_t>(y1, y2);
    std::int16_t y = upwards ? position.y() - 5 : position.y() + 5;
    return hor.contains(x) && ver.contains(y);
}

bool Ladder::fell_off(std::int16_t y, bool downwards) const
{
    std::int16_t dy = downwards ? y + 5 : y - 5;
    return dy > y2 || y + 5 < y1;
}

std::int16_t Ladder::get_x() const
{
    return x;
}
} // namespace jrc
