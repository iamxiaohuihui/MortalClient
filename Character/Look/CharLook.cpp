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
#include "CharLook.h"

#include "../../Constants.h"
#include "../../Data/WeaponData.h"

#include <array>

namespace jrc
{
CharLook::CharLook(const LookEntry& entry)
{
    reset();

    set_body(entry.skin);
    set_hair(entry.hairid);
    set_face(entry.faceid);

    for (auto& equip : entry.equips) {
        add_equip(equip.second);
    }
}

CharLook::CharLook()
{
    reset();

    body = nullptr;
    hair = nullptr;
    face = nullptr;
}

void CharLook::reset()
{
    flip = true;

    action = nullptr;
    actionstr = "";
    actframe = 0;

    set_stance(Stance::STAND1);
    stframe.set(0);
    stelapsed = 0;

    set_expression(Expression::DEFAULT);
    expframe.set(0);
    expelapsed = 0;
}

void CharLook::draw(const DrawArgument& args,
                    Stance::Id interstance,
                    Expression::Id interexpression,
                    std::uint8_t interframe,
                    std::uint8_t interexpframe) const
{
    Point<std::int16_t> faceshift =
        drawinfo.getfacepos(interstance, interframe);
    DrawArgument faceargs =
        args + DrawArgument{faceshift, false, Point<std::int16_t>{}};

    if (Stance::is_climbing(interstance)) {
        body->draw(interstance, Body::BODY, interframe, args);
        equips.draw(
            Equipslot::GLOVES, interstance, Clothing::GLOVE, interframe, args);
        equips.draw(
            Equipslot::SHOES, interstance, Clothing::SHOES, interframe, args);
        equips.draw(
            Equipslot::PANTS, interstance, Clothing::PANTS, interframe, args);
        equips.draw(
            Equipslot::TOP, interstance, Clothing::TOP, interframe, args);
        equips.draw(
            Equipslot::TOP, interstance, Clothing::MAIL, interframe, args);
        equips.draw(
            Equipslot::CAPE, interstance, Clothing::CAPE, interframe, args);
        body->draw(interstance, Body::HEAD, interframe, args);
        equips.draw(Equipslot::EARRINGS,
                    interstance,
                    Clothing::EARRINGS,
                    interframe,
                    args);

        switch (equips.getcaptype()) {
        case CharEquips::NONE:
            hair->draw(interstance, Hair::BACK, interframe, args);
            break;
        case CharEquips::HEADBAND:
            equips.draw(
                Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
            hair->draw(interstance, Hair::BACK, interframe, args);
            break;
        case CharEquips::HALFCOVER:
            hair->draw(interstance, Hair::BELOWCAP, interframe, args);
            equips.draw(
                Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
            break;
        case CharEquips::FULLCOVER:
            equips.draw(
                Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
            break;
        case CharEquips::HAIRPIN:
            // TODO
            break;
        }

        equips.draw(Equipslot::SHIELD,
                    interstance,
                    Clothing::BACKSHIELD,
                    interframe,
                    args);
        equips.draw(Equipslot::WEAPON,
                    interstance,
                    Clothing::BACKWEAPON,
                    interframe,
                    args);
    } else {
        hair->draw(interstance, Hair::BELOWBODY, interframe, args);
        equips.draw(
            Equipslot::CAPE, interstance, Clothing::CAPE, interframe, args);
        equips.draw(Equipslot::SHIELD,
                    interstance,
                    Clothing::SHIELD_BELOW_BODY,
                    interframe,
                    args);
        equips.draw(Equipslot::WEAPON,
                    interstance,
                    Clothing::WEAPON_BELOW_BODY,
                    interframe,
                    args);
        equips.draw(Equipslot::CAP,
                    interstance,
                    Clothing::CAP_BELOW_BODY,
                    interframe,
                    args);
        body->draw(interstance, Body::BODY, interframe, args);
        equips.draw(Equipslot::GLOVES,
                    interstance,
                    Clothing::WRIST_OVER_BODY,
                    interframe,
                    args);
        equips.draw(Equipslot::GLOVES,
                    interstance,
                    Clothing::GLOVE_OVER_BODY,
                    interframe,
                    args);
        equips.draw(
            Equipslot::SHOES, interstance, Clothing::SHOES, interframe, args);
        body->draw(interstance, Body::ARM_BELOW_HEAD, interframe, args);

        if (equips.has_overall()) {
            equips.draw(
                Equipslot::TOP, interstance, Clothing::MAIL, interframe, args);
        } else {
            equips.draw(Equipslot::PANTS,
                        interstance,
                        Clothing::PANTS,
                        interframe,
                        args);
            equips.draw(
                Equipslot::TOP, interstance, Clothing::TOP, interframe, args);
        }

        body->draw(
            interstance, Body::ARM_BELOW_HEAD_OVER_MAIL, interframe, args);
        hair->draw(interstance, Hair::DEFAULT, interframe, args);
        equips.draw(Equipslot::SHIELD,
                    interstance,
                    Clothing::SHIELD_OVER_HAIR,
                    interframe,
                    args);
        equips.draw(Equipslot::EARRINGS,
                    interstance,
                    Clothing::EARRINGS,
                    interframe,
                    args);
        body->draw(interstance, Body::HEAD, interframe, args);
        hair->draw(interstance, Hair::SHADE, interframe, args);
        face->draw(interexpression, interexpframe, faceargs);
        equips.draw(
            Equipslot::FACEACC, interstance, Clothing::FACEACC, 0, faceargs);
        equips.draw(Equipslot::EYEACC,
                    interstance,
                    Clothing::EYEACC,
                    interframe,
                    args);
        equips.draw(Equipslot::SHIELD,
                    interstance,
                    Clothing::SHIELD,
                    interframe,
                    args);

        switch (equips.getcaptype()) {
        case CharEquips::NONE:
            hair->draw(interstance, Hair::OVERHEAD, interframe, args);
            break;
        case CharEquips::HEADBAND:
            equips.draw(
                Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
            hair->draw(interstance, Hair::DEFAULT, interframe, args);
            hair->draw(interstance, Hair::OVERHEAD, interframe, args);
            equips.draw(Equipslot::CAP,
                        interstance,
                        Clothing::CAP_OVER_HAIR,
                        interframe,
                        args);
            break;
        case CharEquips::HALFCOVER:
            hair->draw(interstance, Hair::DEFAULT, interframe, args);
            equips.draw(
                Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
            break;
        case CharEquips::FULLCOVER:
            equips.draw(
                Equipslot::CAP, interstance, Clothing::CAP, interframe, args);
            break;
        default:
            break;
        }

        equips.draw(Equipslot::WEAPON,
                    interstance,
                    Clothing::WEAPON_BELOW_ARM,
                    interframe,
                    args);
        bool twohanded = is_twohanded(interstance);
        if (twohanded) {
            equips.draw(Equipslot::TOP,
                        interstance,
                        Clothing::MAILARM,
                        interframe,
                        args);
            body->draw(interstance, Body::ARM, interframe, args);
            equips.draw(Equipslot::WEAPON,
                        interstance,
                        Clothing::WEAPON,
                        interframe,
                        args);
        } else {
            equips.draw(Equipslot::WEAPON,
                        interstance,
                        Clothing::WEAPON,
                        interframe,
                        args);
            body->draw(interstance, Body::ARM, interframe, args);
            equips.draw(Equipslot::TOP,
                        interstance,
                        Clothing::MAILARM,
                        interframe,
                        args);
        }

        equips.draw(
            Equipslot::GLOVES, interstance, Clothing::WRIST, interframe, args);
        equips.draw(
            Equipslot::GLOVES, interstance, Clothing::GLOVE, interframe, args);
        equips.draw(Equipslot::WEAPON,
                    interstance,
                    Clothing::WEAPON_OVER_GLOVE,
                    interframe,
                    args);

        body->draw(interstance, Body::HAND_BELOW_WEAPON, interframe, args);

        body->draw(interstance, Body::ARM_OVER_HAIR, interframe, args);
        body->draw(
            interstance, Body::ARM_OVER_HAIR_BELOW_WEAPON, interframe, args);
        equips.draw(Equipslot::WEAPON,
                    interstance,
                    Clothing::WEAPON_OVER_HAND,
                    interframe,
                    args);
        equips.draw(Equipslot::WEAPON,
                    interstance,
                    Clothing::WEAPON_OVER_BODY,
                    interframe,
                    args);
        body->draw(interstance, Body::HAND_OVER_HAIR, interframe, args);
        body->draw(interstance, Body::HAND_OVER_WEAPON, interframe, args);

        equips.draw(Equipslot::GLOVES,
                    interstance,
                    Clothing::WRIST_OVER_HAIR,
                    interframe,
                    args);
        equips.draw(Equipslot::GLOVES,
                    interstance,
                    Clothing::GLOVE_OVER_HAIR,
                    interframe,
                    args);
    }
}

void CharLook::draw(const DrawArgument& args, float alpha) const
{
    if (!body || !hair || !face) {
        return;
    }

    Point<std::int16_t> acmove;
    if (action) {
        acmove = action->get_move();
    }

    DrawArgument relargs = {acmove, flip};

    Stance::Id interstance = stance.get(alpha);
    Expression::Id interexpression = expression.get(alpha);
    std::uint8_t interframe = stframe.get(alpha);
    std::uint8_t interexpframe = expframe.get(alpha);

    switch (interstance) {
    case Stance::STAND1:
    case Stance::STAND2:
        if (alerted) {
            interstance = Stance::ALERT;
        }
        break;
    default:
        break;
    }

    draw(relargs + args,
         interstance,
         interexpression,
         interframe,
         interexpframe);
}

void CharLook::draw(Point<std::int16_t> position,
                    bool flipped,
                    Stance::Id interstance,
                    Expression::Id interexpression) const
{
    interstance = equips.adjust_stance(interstance);
    draw({position, flipped}, interstance, interexpression, 0, 0);
}

bool CharLook::update(std::uint16_t timestep)
{
    if (timestep == 0) {
        stance.normalize();
        stframe.normalize();
        expression.normalize();
        expframe.normalize();

        return false;
    }

    alerted.update();

    bool aniend = false;
    if (action == nullptr) {
        std::uint16_t delay = get_delay(stance.get(), stframe.get());
        std::uint16_t delta = delay - stelapsed;
        if (timestep >= delta) {
            stelapsed = timestep - delta;

            std::uint8_t nextframe = getnextframe(stance.get(), stframe.get());
            float threshold = static_cast<float>(delta) / timestep;
            stframe.next(nextframe, threshold);

            if (stframe == 0) {
                aniend = true;
            }
        } else {
            stance.normalize();
            stframe.normalize();

            stelapsed += timestep;
        }
    } else {
        std::uint16_t delay = action->get_delay();
        std::uint16_t delta = delay - stelapsed;
        if (timestep >= delta) {
            stelapsed = timestep - delta;

            actframe = drawinfo.next_actionframe(actionstr, actframe);
            if (actframe > 0) {
                action = drawinfo.get_action(actionstr, actframe);

                float threshold = static_cast<float>(delta) / timestep;
                stance.next(action->get_stance(), threshold);
                stframe.next(action->get_frame(), threshold);
            } else {
                aniend = true;
                action = nullptr;
                actionstr = "";
                set_stance(Stance::STAND1);
            }
        } else {
            stance.normalize();
            stframe.normalize();

            stelapsed += timestep;
        }
    }

    std::uint16_t expdelay = face->get_delay(expression.get(), expframe.get());
    std::uint16_t expdelta = expdelay - expelapsed;
    if (timestep >= expdelta) {
        expelapsed = timestep - expdelta;

        std::uint8_t nextexpframe =
            face->nextframe(expression.get(), expframe.get());
        float fcthreshold = static_cast<float>(expdelta) / timestep;
        expframe.next(nextexpframe, fcthreshold);

        if (expframe == 0) {
            if (expression == Expression::DEFAULT) {
                expression.next(Expression::BLINK, fcthreshold);
            } else {
                expression.next(Expression::DEFAULT, fcthreshold);
            }
        }
    } else {
        expression.normalize();
        expframe.normalize();

        expelapsed += timestep;
    }

    return aniend;
}

void CharLook::set_body(std::int32_t skin_id)
{
    auto iter = bodytypes.find(skin_id);
    if (iter == bodytypes.end()) {
        iter = bodytypes
                   .emplace(std::piecewise_construct,
                            std::forward_as_tuple(skin_id),
                            std::forward_as_tuple(skin_id, drawinfo))
                   .first;
    }
    body = &iter->second;
}

void CharLook::set_hair(std::int32_t hair_id)
{
    auto iter = hairstyles.find(hair_id);
    if (iter == hairstyles.end()) {
        iter = hairstyles
                   .emplace(std::piecewise_construct,
                            std::forward_as_tuple(hair_id),
                            std::forward_as_tuple(hair_id, drawinfo))
                   .first;
    }
    hair = &iter->second;
}

void CharLook::set_face(std::int32_t face_id)
{
    auto iter = facetypes.find(face_id);
    if (iter == facetypes.end()) {
        iter = facetypes.emplace(face_id, face_id).first;
    }
    face = &iter->second;
}

void CharLook::updatetwohanded()
{
    Stance::Id basestance = Stance::baseof(stance.get());
    set_stance(basestance);
}

void CharLook::add_equip(std::int32_t itemid)
{
    equips.add_equip(itemid, drawinfo);
    updatetwohanded();
}

void CharLook::remove_equip(Equipslot::Id slot)
{
    equips.remove_equip(slot);
    if (slot == Equipslot::WEAPON) {
        updatetwohanded();
    }
}

void CharLook::attack(bool degenerate)
{
    std::int32_t weapon_id = equips.get_weapon();
    if (weapon_id <= 0) {
        return;
    }

    const WeaponData& weapon = WeaponData::get(weapon_id);

    std::uint8_t attacktype = weapon.get_attack();
    if (attacktype == 9 && !degenerate) {
        stance.set(Stance::SHOT);
        set_action("handgun");
    } else {
        stance.set(getattackstance(attacktype, degenerate));
        stframe.set(0);
        stelapsed = 0;
    }

    weapon.get_usesound(degenerate).play();
}

void CharLook::attack(Stance::Id newstance)
{
    if (action || newstance == Stance::NONE) {
        return;
    }

    switch (newstance) {
    case Stance::SHOT:
        set_action("handgun");
        break;
    default:
        set_stance(newstance);
    }
}

void CharLook::set_stance(Stance::Id newstance)
{
    if (action || newstance == Stance::NONE) {
        return;
    }

    Stance::Id adjstance = equips.adjust_stance(newstance);
    if (stance != adjstance) {
        stance.set(adjstance);
        stframe.set(0);
        stelapsed = 0;
    }
}

Stance::Id CharLook::getattackstance(std::uint8_t attack,
                                     bool degenerate) const
{
    if (stance == Stance::PRONE) {
        return Stance::PRONESTAB;
    }

    enum Attack {
        NONE = 0,
        S1A1M1D = 1,
        SPEAR = 2,
        BOW = 3,
        CROSSBOW = 4,
        S2A2M2 = 5,
        WAND = 6,
        CLAW = 7,
        GUN = 9,
        NUM_ATTACKS
    };

    static const std::array<std::vector<Stance::Id>, NUM_ATTACKS>
        degen_stances = {{{Stance::NONE},
                          {Stance::NONE},
                          {Stance::NONE},
                          {Stance::SWINGT1, Stance::SWINGT3},
                          {Stance::SWINGT1, Stance::STABT1},
                          {Stance::NONE},
                          {Stance::NONE},
                          {Stance::SWINGT1, Stance::STABT1},
                          {Stance::NONE},
                          {Stance::SWINGP1, Stance::STABT2}}};

    static const std::array<std::vector<Stance::Id>, NUM_ATTACKS>
        attack_stances = {{{Stance::NONE},
                           {Stance::STABO1,
                            Stance::STABO2,
                            Stance::SWINGO1,
                            Stance::SWINGO2,
                            Stance::SWINGO3},
                           {Stance::STABT1, Stance::SWINGP1},
                           {Stance::SHOOT1},
                           {Stance::SHOOT2},
                           {Stance::STABO1,
                            Stance::STABO2,
                            Stance::SWINGT1,
                            Stance::SWINGT2,
                            Stance::SWINGT3},
                           {Stance::SWINGO1, Stance::SWINGO2},
                           {Stance::SWINGO1, Stance::SWINGO2},
                           {Stance::NONE},
                           {Stance::SHOT}}};

    if (attack <= NONE || attack >= NUM_ATTACKS) {
        return Stance::STAND1;
    }

    const auto& stances =
        degenerate ? degen_stances[attack] : attack_stances[attack];
    if (stances.empty()) {
        return Stance::STAND1;
    }

    std::size_t index = randomizer.next_int(stances.size());
    return stances[index];
}

std::uint16_t CharLook::get_delay(Stance::Id st, std::uint8_t fr) const
{
    return drawinfo.get_delay(st, fr);
}

std::uint8_t CharLook::getnextframe(Stance::Id st, std::uint8_t fr) const
{
    return drawinfo.nextframe(st, fr);
}

void CharLook::set_expression(Expression::Id newexpression)
{
    if (expression != newexpression) {
        expression.set(newexpression);
        expframe.set(0);

        expelapsed = 0;
    }
}

void CharLook::set_action(const std::string& acstr)
{
    if (acstr == actionstr || acstr.empty()) {
        return;
    }

    if (Stance::Id ac_stance = Stance::by_string(acstr)) {
        set_stance(ac_stance);
    } else {
        action = drawinfo.get_action(acstr, 0);

        if (action) {
            actframe = 0;
            stelapsed = 0;
            actionstr = acstr;

            stance.set(action->get_stance());
            stframe.set(action->get_frame());
        }
    }
}

void CharLook::set_direction(bool f)
{
    flip = f;
}

void CharLook::set_alerted(std::int64_t millis)
{
    alerted.set_for(millis);
}

bool CharLook::is_twohanded(Stance::Id st) const
{
    switch (st) {
    case Stance::STAND1:
    case Stance::WALK1:
        return false;
    case Stance::STAND2:
    case Stance::WALK2:
        return true;
    default:
        return equips.is_twohanded();
    }
}

std::uint16_t CharLook::get_attackdelay(std::size_t no,
                                        std::uint8_t first_frame) const
{
    if (action) {
        return drawinfo.get_attackdelay(actionstr, no);
    } else {
        std::uint16_t delay = 0;
        for (std::uint8_t frame = 0; frame < first_frame; frame++) {
            delay += get_delay(stance.get(), frame);
        }
        return delay;
    }
}

std::uint8_t CharLook::get_frame() const
{
    return stframe.get();
}

Stance::Id CharLook::get_stance() const
{
    return stance.get();
}

const Body* CharLook::get_body() const
{
    return body;
}

const Hair* CharLook::get_hair() const
{
    return hair;
}

const Face* CharLook::get_face() const
{
    return face;
}

const CharEquips& CharLook::get_equips() const
{
    return equips;
}

void CharLook::init()
{
    drawinfo.init();
}

BodyDrawinfo CharLook::drawinfo;
std::unordered_map<std::int32_t, Hair> CharLook::hairstyles;
std::unordered_map<std::int32_t, Face> CharLook::facetypes;
std::unordered_map<std::int32_t, Body> CharLook::bodytypes;
} // namespace jrc
