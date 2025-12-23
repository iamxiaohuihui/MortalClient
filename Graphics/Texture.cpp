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
#include "Texture.h"

#include "../Configuration.h"
#include "GraphicsGL.h"
#include "Wz.h"

namespace jrc
{
Texture::Texture(WzNode src)
{
    if (src.getNodeType() == WzNode::NodeType::BITMAP) {
        WzNode src_origin = src["origin"];
        const Point<std::int16_t> original_origin = src_origin;
        const bool use_original_origin
            = src_origin.getNodeType() == WzNode::NodeType::VECTOR;

        const std::string link = src["source"];
        if (!link.empty()) {
            WzNode src_file = src;
            while (src_file != src_file.root()) {
                src_file = src_file.root();
            }
            src = src_file.resolve(
                std::string_view{link}.substr(link.find('/') + 1).data());
        }

        bitmap = src;
        origin = use_original_origin ? original_origin : src["origin"];
        dimensions = {bitmap.getWidth(), bitmap.getHeight()};

        GraphicsGL::get().add_bitmap(bitmap);
    }
}

Texture::Texture(WzBitmap bm, Point<std::int16_t> orig)
    : bitmap{bm}, origin{orig}, dimensions{bm.getWidth(), bm.getHeight()}
{
}

Texture::Texture() = default;

Texture::~Texture() = default;

void Texture::draw(const DrawArgument& args) const
{
    std::size_t id = bitmap.id();
    if (id == 0) {
        return;
    }

    GraphicsGL::get().draw(bitmap,
                           args.get_rectangle(origin, dimensions),
                           args.get_color(),
                           args.get_angle());
}

void Texture::shift(Point<std::int16_t> amount)
{
    origin -= amount;
}

bool Texture::is_valid() const
{
    return bitmap.id() > 0;
}

std::int16_t Texture::width() const
{
    return dimensions.x();
}

std::int16_t Texture::height() const
{
    return dimensions.y();
}

Point<std::int16_t> Texture::get_origin() const
{
    return origin;
}

Point<std::int16_t> Texture::get_dimensions() const
{
    return dimensions;
}
} // namespace jrc
