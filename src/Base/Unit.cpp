// SPDX-License-Identifier: LGPL-2.1-or-later
/****************************************************************************
 *                                                                          *
 *   This file is part of FreeCAD.                                          *
 *                                                                          *
 *   FreeCAD is free software: you can redistribute it and/or modify it     *
 *   under the terms of the GNU Lesser General Public License as            *
 *   published by the Free Software Foundation, either version 2.1 of the   *
 *   License, or (at your option) any later version.                        *
 *                                                                          *
 *   FreeCAD is distributed in the hope that it will be useful, but         *
 *   WITHOUT ANY WARRANTY; without even the implied warranty of             *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU       *
 *   Lesser General Public License for more details.                        *
 *                                                                          *
 *   You should have received a copy of the GNU Lesser General Public       *
 *   License along with FreeCAD. If not, see                                *
 *   <https://www.gnu.org/licenses/>.                                       *
 *                                                                          *
 ***************************************************************************/

#include <algorithm>
#include <array>
#include <cmath>
#include <cstring>
#include <limits>
#include <ranges>
#include <vector>

#include <fmt/format.h>
#include <fmt/ranges.h>

#include "Unit.h"

using namespace Base;

Unit::Unit(
    const int length,  // NOLINT
    const int mass,
    const int time,
    const int electricCurrent,
    const int thermodynamicTemperature,
    const int amountOfSubstance,
    const int luminousIntensity,
    const int angle
)
    : _name {""}
{
    auto cast = [](auto val) {
        return static_cast<int8_t>(std::clamp<decltype(val)>(
            val,
            std::numeric_limits<int8_t>::min(),
            std::numeric_limits<int8_t>::max()
        ));
    };

    _exps[0] = cast(length);
    _exps[1] = cast(mass);
    _exps[2] = cast(time);
    _exps[3] = cast(electricCurrent);
    _exps[4] = cast(thermodynamicTemperature);
    _exps[5] = cast(amountOfSubstance);
    _exps[6] = cast(luminousIntensity);
    _exps[7] = cast(angle);

    checkRange();
}

Unit& Unit::operator*=(const Unit& that)
{
    *this = *this * that;
    return *this;
}

Unit& Unit::operator/=(const Unit& that)
{
    *this = *this / that;
    return *this;
}

Unit Unit::operator*(const Unit& right) const
{
    auto mult = [&](auto leftExponent, auto rightExponent) {
        return leftExponent + rightExponent;
    };

    UnitExponents res {};
    std::transform(_exps.begin(), _exps.end(), right._exps.begin(), res.begin(), mult);

    return Unit {res};
}

Unit Unit::operator/(const Unit& right) const
{
    auto div = [&](auto leftExponent, auto rightExponent) {
        return leftExponent - rightExponent;
    };

    UnitExponents res {};
    std::transform(_exps.begin(), _exps.end(), right._exps.begin(), res.begin(), div);

    return Unit {res};
}

Unit Unit::root(const uint8_t num) const
{
    auto apply = [&](auto val) {
        if (val % num != 0) {
            throw UnitsMismatchError("unit values must be divisible by root");
        }
        return static_cast<decltype(val)>(val / num);
    };

    if (num < 1) {
        throw UnitsMismatchError("root must be > 0");
    }

    UnitExponents res {};
    std::transform(_exps.begin(), _exps.end(), res.begin(), apply);

    return Unit {res};
}

Unit Unit::pow(const double exp) const
{
    auto apply = [&](const auto val) {
        const auto num {val * exp};
        if (std::fabs(std::round(num) - num) >= std::numeric_limits<double>::epsilon()) {
            throw UnitsMismatchError("pow() of unit not possible");
        }

        return static_cast<decltype(val)>(val * exp);
    };

    UnitExponents res {};
    std::transform(_exps.begin(), _exps.end(), res.begin(), apply);

    return Unit {res};
}

UnitExponents Unit::exponents() const
{
    return _exps;
}

int Unit::length() const
{
    return _exps[0];
}

std::string Unit::getString() const
{
    auto buildSubStr = [&](auto index) {
        const std::string unitStrString {unitSymbols.at(index)};
        const auto absol {abs(_exps.at(index))};

        return absol <= 1 ? unitStrString
                          : fmt::format("{}^{}", unitStrString, std::to_string(absol));
    };

    auto buildStr = [&](auto indexes) {
        std::vector<std::string> subStrings {};
        std::transform(indexes.begin(), indexes.end(), std::back_inserter(subStrings), buildSubStr);

        return fmt::format("{}", fmt::join(subStrings, "*"));
    };

    //------------------------------------------------------------------------------

    auto [posValIndexes, negValIndexes] = nonZeroValsIndexes();
    auto numeratorStr = buildStr(posValIndexes);
    if (negValIndexes.empty()) {
        return numeratorStr;
    }

    auto denominatorStr = buildStr(negValIndexes);

    return fmt::format(
        "{}/{}",
        numeratorStr.empty() ? "1" : numeratorStr,
        negValIndexes.size() > 1 ? fmt::format("({})", denominatorStr) : denominatorStr
    );
}

std::string Unit::representation() const
{
    auto name = getTypeString();
    auto inParen = fmt::format("Unit: {} ({})", getString(), fmt::join(_exps, ","));
    return name.empty() ? inParen : fmt::format("{} [{}]", inParen, name);
}

std::string Unit::getTypeString() const
{
    if (_name.empty()) {
        const auto spec = std::ranges::find(unitSpecs, _exps, &UnitSpec::exps);
        return std::string(spec == unitSpecs.end() ? "" : spec->name);
    }

    return std::string {_name.data(), _name.size()};
}

std::pair<std::vector<size_t>, std::vector<size_t>> Unit::nonZeroValsIndexes() const
{
    std::vector<size_t> pos {};
    std::vector<size_t> neg {};

    auto posNeg = [&, index {0}](auto val) mutable {
        if (val != 0) {
            (val > 0 ? pos : neg).push_back(index);
        }
        ++index;
    };

    std::ranges::for_each(_exps, posNeg);

    return {pos, neg};
}
