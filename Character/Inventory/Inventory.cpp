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
#include "Inventory.h"

#include "../../Console.h"
#include "../../Data/BulletData.h"
#include "../../Data/EquipData.h"
#include "../../Data/ItemData.h"

namespace jrc
{
Inventory::Inventory()
{
    bullet_slot = 0;
    meso = 0;
    running_uid = 0;
    slotmaxima[InventoryType::EQUIPPED] = Equipslot::LENGTH;
}

void Inventory::recalc_stats(Weapon::Type type)
{
    total_stats.clear();
    for (const auto& iter : inventories[InventoryType::EQUIPPED]) {
        auto equip_iter = equips.find(iter.second.unique_id);
        if (equip_iter != equips.end()) {
            const Equip& equip = equip_iter->second;
            for (auto stat_iter : total_stats) {
                stat_iter.second += equip.get_stat(stat_iter.first);
            }
        }
    }

    std::int32_t prefix = [](auto t) {
        switch (t) {
        case Weapon::BOW:
            return 2060;
        case Weapon::CROSSBOW:
            return 2061;
        case Weapon::CLAW:
            return 2070;
        case Weapon::GUN:
            return 2330;
        default:
            return 0;
        }
    }(type);

    bullet_slot = 0;
    if (prefix) {
        for (const auto& iter : inventories[InventoryType::USE]) {
            const Slot& slot = iter.second;
            if (slot.count && slot.item_id / 1000 == prefix) {
                bullet_slot = iter.first;
                break;
            }
        }
    }

    if (const auto bulletid = get_bullet_id()) {
        total_stats[Equipstat::WATK] += BulletData::get(bulletid).get_watk();
    }
}

void Inventory::set_meso(std::int64_t m)
{
    meso = m;
}

void Inventory::set_slotmax(InventoryType::Id type, std::uint8_t slotmax)
{
    slotmaxima[type] = slotmax;
}

void Inventory::add_item(InventoryType::Id invtype,
                         std::int16_t slot,
                         std::int32_t item_id,
                         bool cash,
                         std::int64_t expire,
                         std::uint16_t count,
                         const std::string& owner,
                         std::int16_t flags)
{
    items.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(add_slot(invtype, slot, item_id, count, cash)),
        std::forward_as_tuple(item_id, expire, owner, flags));
}

void Inventory::add_pet(InventoryType::Id invtype,
                        std::int16_t slot,
                        std::int32_t item_id,
                        bool cash,
                        std::int64_t expire,
                        const std::string& name,
                        std::int8_t level,
                        std::int16_t closeness,
                        std::int8_t fullness)
{
    pets.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(add_slot(invtype, slot, item_id, 1, cash)),
        std::forward_as_tuple(
            item_id, expire, name, level, closeness, fullness));
}

void Inventory::add_equip(InventoryType::Id invtype,
                          std::int16_t slot,
                          std::int32_t item_id,
                          bool cash,
                          std::int64_t expire,
                          std::uint8_t slots,
                          std::uint8_t level,
                          const EnumMap<Equipstat::Id, std::uint16_t>& stats,
                          const std::string& owner,
                          std::int16_t flag,
                          std::uint8_t ilevel,
                          std::uint16_t iexp,
                          std::int32_t vicious)
{
    equips.emplace(
        std::piecewise_construct,
        std::forward_as_tuple(add_slot(invtype, slot, item_id, 1, cash)),
        std::forward_as_tuple(item_id,
                              expire,
                              owner,
                              flag,
                              slots,
                              level,
                              stats,
                              ilevel,
                              iexp,
                              vicious));
}

void Inventory::remove(InventoryType::Id type, std::int16_t slot)
{
    auto iter = inventories[type].find(slot);
    if (iter == inventories[type].end()) {
        return;
    }

    std::int32_t unique_id = iter->second.unique_id;
    inventories[type].erase(iter);

    switch (type) {
    case InventoryType::EQUIPPED:
    case InventoryType::EQUIP:
        equips.erase(unique_id);
        break;
    case InventoryType::CASH:
        items.erase(unique_id);
        pets.erase(unique_id);
        break;
    default:
        items.erase(unique_id);
        break;
    }
}

void Inventory::swap(InventoryType::Id first_type,
                     std::int16_t first_slot,
                     InventoryType::Id second_type,
                     std::int16_t second_slot)
{
    Slot first = std::move(inventories[first_type][first_slot]);
    inventories[first_type][first_slot]
        = std::move(inventories[second_type][second_slot]);
    inventories[second_type][second_slot] = std::move(first);

    if (!inventories[first_type][first_slot].item_id) {
        remove(first_type, first_slot);
    }
    if (!inventories[second_type][second_slot].item_id) {
        remove(second_type, second_slot);
    }
}

std::int32_t Inventory::add_slot(InventoryType::Id type,
                                 std::int16_t slot,
                                 std::int32_t item_id,
                                 std::int16_t count,
                                 bool cash)
{
    ++running_uid;
    inventories[type][slot] = {running_uid, item_id, count, cash};
    return running_uid;
}

void Inventory::change_count(InventoryType::Id type,
                             std::int16_t slot,
                             std::int16_t count)
{
    auto iter = inventories[type].find(slot);
    if (iter != inventories[type].end()) {
        iter->second.count = count;
    }
}

void Inventory::modify(InventoryType::Id type,
                       std::int16_t slot,
                       std::int8_t mode,
                       std::int16_t arg,
                       Movement move)
{
    if (slot < 0) {
        slot = -slot;
        type = InventoryType::EQUIPPED;
    }
    arg = arg < 0 ? -arg : arg;

    switch (mode) {
    case CHANGE_COUNT:
        change_count(type, slot, arg);
        break;
    case SWAP:
        switch (move) {
        case MOVE_INTERNAL:
            swap(type, slot, type, arg);
            break;
        case MOVE_UNEQUIP:
            swap(InventoryType::EQUIPPED, slot, InventoryType::EQUIP, arg);
            break;
        case MOVE_EQUIP:
            swap(InventoryType::EQUIP, slot, InventoryType::EQUIPPED, arg);
            break;
        case MOVE_NONE:
        default:
            break;
        }
        break;
    case REMOVE:
        remove(type, slot);
        break;
    default:
        break;
    }
}

std::uint8_t Inventory::get_slotmax(InventoryType::Id type) const
{
    return slotmaxima[type];
}

std::uint16_t Inventory::get_stat(Equipstat::Id type) const
{
    return total_stats[type];
}

std::int64_t Inventory::get_meso() const
{
    return meso;
}

bool Inventory::has_projectile() const
{
    return bullet_slot > 0;
}

bool Inventory::has_equipped(Equipslot::Id slot) const
{
    return inventories[InventoryType::EQUIPPED].count(slot) > 0;
}

std::int16_t Inventory::get_bullet_slot() const
{
    return bullet_slot;
}

std::uint16_t Inventory::get_bullet_count() const
{
    return static_cast<std::uint16_t>(
        get_item_count(InventoryType::USE, bullet_slot));
}

std::int32_t Inventory::get_bullet_id() const
{
    return get_item_id(InventoryType::USE, bullet_slot);
}

Equipslot::Id Inventory::find_equip_slot(std::int32_t itemid) const
{
    const EquipData& cloth = EquipData::get(itemid);
    if (!cloth.is_valid()) {
        return Equipslot::NONE;
    }

    Equipslot::Id eqslot = cloth.get_eq_slot();
    if (eqslot == Equipslot::RING) {
        if (!has_equipped(Equipslot::RING2)) {
            return Equipslot::RING2;
        }

        if (!has_equipped(Equipslot::RING3)) {
            return Equipslot::RING3;
        }

        if (!has_equipped(Equipslot::RING4)) {
            return Equipslot::RING4;
        }

        return Equipslot::RING;
    } else {
        return eqslot;
    }
}

std::int16_t Inventory::find_free_slot(InventoryType::Id type) const
{
    std::int16_t counter = 1;
    for (const auto& iter : inventories[type]) {
        if (iter.first != counter) {
            return counter;
        }

        ++counter;
    }
    return counter < slotmaxima[type] ? counter : static_cast<std::int16_t>(0);
}

std::int16_t Inventory::find_item(InventoryType::Id type,
                                  std::int32_t itemid) const
{
    for (auto& iter : inventories[type]) {
        if (iter.second.item_id == itemid) {
            return iter.first;
        }
    }
    return 0;
}

std::int16_t Inventory::get_item_count(InventoryType::Id type,
                                       std::int16_t slot) const
{
    auto iter = inventories[type].find(slot);
    if (iter != inventories[type].end()) {
        return iter->second.count;
    } else {
        return 0;
    }
}

std::int32_t Inventory::get_item_id(InventoryType::Id type,
                                    std::int16_t slot) const
{
    auto iter = inventories[type].find(slot);
    if (iter != inventories[type].end()) {
        return iter->second.item_id;
    } else {
        return 0;
    }
}

nullable_ptr<const Equip> Inventory::get_equip(InventoryType::Id type,
                                               std::int16_t slot) const
{
    if (type != InventoryType::EQUIPPED && type != InventoryType::EQUIP) {
        return {};
    }

    auto slot_iter = inventories[type].find(slot);
    if (slot_iter == inventories[type].end()) {
        return {};
    }

    auto equip_iter = equips.find(slot_iter->second.unique_id);
    if (equip_iter == equips.end()) {
        return {};
    }

    return equip_iter->second;
}

Inventory::Movement Inventory::movement_by_value(std::int8_t value)
{
    if (value >= MOVE_INTERNAL && value <= MOVE_EQUIP) {
        return static_cast<Movement>(value);
    }

    Console::get().print("Unknown move type: " + std::to_string(value));
    return MOVE_NONE;
}
} // namespace jrc
