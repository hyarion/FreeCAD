// SPDX-License-Identifier: LGPL-2.1-or-later

/***************************************************************************
 *   Copyright (c) 2013 Jürgen Riegel <juergen.riegel@web.de>              *
 *                                                                         *
 *   This file is part of the FreeCAD CAx development system.              *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Library General Public           *
 *   License as published by the Free Software Foundation; either          *
 *   version 2 of the License, or (at your option) any later version.      *
 *                                                                         *
 *   This library  is distributed in the hope that it will be useful,      *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Library General Public License for more details.                  *
 *                                                                         *
 *   You should have received a copy of the GNU Library General Public     *
 *   License along with this library; see the file COPYING.LIB. If not,    *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,         *
 *   Suite 330, Boston, MA  02111-1307, USA                                *
 *                                                                         *
 ***************************************************************************/

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <numbers>
#include <ranges>
#include <sstream>
#include <string>

#include <fmt/format.h>

#include "Exception.h"
#include "Quantity.h"
#include "QuantitySpecsData.h"
#include "Tools.h"
#include "UnitsApi.h"
#include "UnitsConvData.h"
#include "UnitsSchema.h"

/** \defgroup Units Units system
    \ingroup BASE
    \brief The quantities and units system enables FreeCAD to work transparently with many different
   units
*/

// suppress annoying warnings from generated source files
#ifdef _MSC_VER
# pragma warning(disable : 4003)
# pragma warning(disable : 4018)
# pragma warning(disable : 4065)
# pragma warning(disable : 4273)
# pragma warning(disable : 4335)  // disable MAC file format warning on VC
#endif

using Base::Quantity;
using Base::QuantityFormat;
using Base::UnitsSchema;

QuantityFormat::QuantityFormat(QuantityFormat::NumberFormat format, int decimals)
    : option(OmitGroupSeparator | RejectGroupSeparator)
    , format(format)
    , _precision(decimals)
    , _denominator(-1)
{}

int QuantityFormat::getPrecision() const
{
    return _precision < 0 ? UnitsApi::getDecimals() : _precision;
}

int QuantityFormat::getDenominator() const
{
    return _denominator < 0 ? UnitsApi::getDenominator() : _denominator;
}

// ----------------------------------------------------------------------------

Quantity::Quantity(double value, const std::string& unit)
{
    if (unit.empty()) {
        myValue = value;
        myUnit = Unit();
        return;
    }

    try {
        auto tmpQty = parse(unit);
        myValue = value * tmpQty.getValue();
        myUnit = tmpQty.getUnit();
    }
    catch (const Base::ParserError&) {
        myValue = 0.0;
        myUnit = Unit();
    }
}

double Quantity::getValueAs(const Quantity& other) const
{
    return myValue / other.getValue();
}

bool Quantity::operator==(const Quantity& that) const
{
    return (myValue == that.myValue) && (myUnit == that.myUnit);
}

bool Quantity::operator!=(const Quantity& that) const
{
    return !(*this == that);
}

bool Quantity::operator<(const Quantity& that) const
{
    if (myUnit != that.myUnit) {
        throw Base::UnitsMismatchError(
            "Quantity::operator <(): quantities need to have same unit to compare"
        );
    }

    return (myValue < that.myValue);
}

bool Quantity::operator>(const Quantity& that) const
{
    if (myUnit != that.myUnit) {
        throw Base::UnitsMismatchError(
            "Quantity::operator >(): quantities need to have same unit to compare"
        );
    }

    return (myValue > that.myValue);
}

bool Quantity::operator<=(const Quantity& that) const
{
    if (myUnit != that.myUnit) {
        throw Base::UnitsMismatchError(
            "Quantity::operator <=(): quantities need to have same unit to compare"
        );
    }

    return (myValue <= that.myValue);
}

bool Quantity::operator>=(const Quantity& that) const
{
    if (myUnit != that.myUnit) {
        throw Base::UnitsMismatchError(
            "Quantity::operator >=(): quantities need to have same unit to compare"
        );
    }

    return (myValue >= that.myValue);
}

Quantity Quantity::operator*(const Quantity& other) const
{
    return Quantity(myValue * other.myValue, myUnit * other.myUnit);
}

Quantity Quantity::operator*(double factor) const
{
    return Quantity(myValue * factor, myUnit);
}

Quantity Quantity::operator/(const Quantity& other) const
{
    return Quantity(myValue / other.myValue, myUnit / other.myUnit);
}

Quantity Quantity::operator/(double factor) const
{
    return Quantity(myValue / factor, myUnit);
}

Quantity Quantity::pow(const Quantity& other) const
{
    if (!other.isDimensionless()) {
        throw Base::UnitsMismatchError("Quantity::pow(): exponent must not have a unit");
    }

    return Quantity(
        std::pow(myValue, other.myValue),
        myUnit.pow(static_cast<signed char>(other.myValue))
    );
}

Quantity Quantity::pow(double exp) const
{
    return Quantity(std::pow(myValue, exp), myUnit.pow(exp));
}

Quantity Quantity::operator+(const Quantity& other) const
{
    if (myUnit != other.myUnit) {
        throw Base::UnitsMismatchError("Quantity::operator +(): Unit mismatch in plus operation");
    }

    return Quantity(myValue + other.myValue, myUnit);
}

Quantity& Quantity::operator+=(const Quantity& other)
{
    if (myUnit != other.myUnit) {
        throw Base::UnitsMismatchError("Quantity::operator +=(): Unit mismatch in plus operation");
    }

    myValue += other.myValue;

    return *this;
}

Quantity Quantity::operator-(const Quantity& other) const
{
    if (myUnit != other.myUnit) {
        throw Base::UnitsMismatchError("Quantity::operator -(): Unit mismatch in minus operation");
    }

    return Quantity(myValue - other.myValue, myUnit);
}

Quantity& Quantity::operator-=(const Quantity& other)
{
    if (myUnit != other.myUnit) {
        throw Base::UnitsMismatchError("Quantity::operator -=(): Unit mismatch in minus operation");
    }

    myValue -= other.myValue;

    return *this;
}

Quantity Quantity::operator-() const
{
    return Quantity(-myValue, myUnit);
}

std::string Quantity::toString(const QuantityFormat& format) const
{
    return fmt::format("'{} {}'", toNumber(format), myUnit.getString());
}

std::string Quantity::toNumber(const QuantityFormat& format) const
{
    std::stringstream ss;

    switch (format.format) {
        case QuantityFormat::Fixed:
            ss << std::fixed;
            break;
        case QuantityFormat::Scientific:
            ss << std::scientific;
            break;
        default:
            break;
    }
    ss << std::setprecision(format.getPrecision()) << myValue;

    return ss.str();
}

std::string Quantity::getUserString() const
{
    double dummy1 {};  // to satisfy GCC
    std::string dummy2 {};
    return getUserString(dummy1, dummy2);
}

std::string Quantity::getUserString(double& factor, std::string& unitString) const
{
    return Base::UnitsApi::schemaTranslate(*this, factor, unitString);
}

std::string Quantity::getUserString(UnitsSchema* schema, double& factor, std::string& unitString) const
{
    return schema->translate(*this, factor, unitString);
}

std::string Quantity::getSafeUserString() const
{
    auto userStr = getUserString();
    if (myValue != 0.0 && parse(userStr).getValue() == 0) {
        auto unitStr = getUnit().getString();
        userStr = fmt::format("{}{}{}", myValue, unitStr.empty() ? "" : " ", unitStr);
    }

    return Tools::escapeQuotesFromString(userStr);
}

/// true if unit equals to 1, therefore quantity has no dimension
bool Quantity::isDimensionless() const
{
    return myUnit == Unit::One;
}

/// true if it has a specific unit or no dimension.
bool Quantity::isDimensionlessOrUnit(const Unit& unit) const
{
    return isDimensionless() || myUnit == unit;
}

// true if it has a number with or without a unit
bool Quantity::isValid() const
{
    return !std::isnan(myValue);
}

void Quantity::setInvalid()
{
    myValue = std::numeric_limits<double>::quiet_NaN();
}

// === Predefined types (derived from QuantitySpecsData) ====================

namespace
{
constexpr Quantity fromSpec(std::string_view name)
{
    const auto& specs = Base::QuantitySpecsData::specs;
    auto it = std::ranges::find(specs, name, &Base::QuantitySpec::name);
    return Quantity {it->value, Base::Unit {it->exps}};
}
}  // namespace

// clang-format off
#define QUANTITY_BY_NAME(name) Quantity::name = fromSpec(#name)

constexpr Quantity QUANTITY_BY_NAME(NanoMetre);
constexpr Quantity QUANTITY_BY_NAME(MicroMetre);
constexpr Quantity QUANTITY_BY_NAME(MilliMetre);
constexpr Quantity QUANTITY_BY_NAME(CentiMetre);
constexpr Quantity QUANTITY_BY_NAME(DeciMetre);
constexpr Quantity QUANTITY_BY_NAME(Metre);
constexpr Quantity QUANTITY_BY_NAME(KiloMetre);

constexpr Quantity QUANTITY_BY_NAME(MilliLiter);
constexpr Quantity QUANTITY_BY_NAME(Liter);

constexpr Quantity QUANTITY_BY_NAME(Hertz);
constexpr Quantity QUANTITY_BY_NAME(KiloHertz);
constexpr Quantity QUANTITY_BY_NAME(MegaHertz);
constexpr Quantity QUANTITY_BY_NAME(GigaHertz);
constexpr Quantity QUANTITY_BY_NAME(TeraHertz);

constexpr Quantity QUANTITY_BY_NAME(MicroGram);
constexpr Quantity QUANTITY_BY_NAME(MilliGram);
constexpr Quantity QUANTITY_BY_NAME(Gram);
constexpr Quantity QUANTITY_BY_NAME(KiloGram);
constexpr Quantity QUANTITY_BY_NAME(Ton);

constexpr Quantity QUANTITY_BY_NAME(Second);
constexpr Quantity QUANTITY_BY_NAME(Minute);
constexpr Quantity QUANTITY_BY_NAME(Hour);

constexpr Quantity QUANTITY_BY_NAME(Ampere);
constexpr Quantity QUANTITY_BY_NAME(MilliAmpere);
constexpr Quantity QUANTITY_BY_NAME(KiloAmpere);
constexpr Quantity QUANTITY_BY_NAME(MegaAmpere);

constexpr Quantity QUANTITY_BY_NAME(Kelvin);
constexpr Quantity QUANTITY_BY_NAME(MilliKelvin);
constexpr Quantity QUANTITY_BY_NAME(MicroKelvin);

constexpr Quantity QUANTITY_BY_NAME(MilliMole);
constexpr Quantity QUANTITY_BY_NAME(Mole);

constexpr Quantity QUANTITY_BY_NAME(Candela);

constexpr Quantity QUANTITY_BY_NAME(Inch);
constexpr Quantity QUANTITY_BY_NAME(Foot);
constexpr Quantity QUANTITY_BY_NAME(Thou);
constexpr Quantity QUANTITY_BY_NAME(Yard);
constexpr Quantity QUANTITY_BY_NAME(Mile);

constexpr Quantity QUANTITY_BY_NAME(MilePerHour);

constexpr Quantity QUANTITY_BY_NAME(SquareFoot);
constexpr Quantity QUANTITY_BY_NAME(CubicFoot);

constexpr Quantity QUANTITY_BY_NAME(Pound);
constexpr Quantity QUANTITY_BY_NAME(Ounce);
constexpr Quantity QUANTITY_BY_NAME(Stone);
constexpr Quantity QUANTITY_BY_NAME(Hundredweights);

constexpr Quantity QUANTITY_BY_NAME(PoundForce);

constexpr Quantity QUANTITY_BY_NAME(Newton);
constexpr Quantity QUANTITY_BY_NAME(MilliNewton);
constexpr Quantity QUANTITY_BY_NAME(KiloNewton);
constexpr Quantity QUANTITY_BY_NAME(MegaNewton);

constexpr Quantity QUANTITY_BY_NAME(NewtonPerMeter);
constexpr Quantity QUANTITY_BY_NAME(MilliNewtonPerMeter);
constexpr Quantity QUANTITY_BY_NAME(KiloNewtonPerMeter);
constexpr Quantity QUANTITY_BY_NAME(MegaNewtonPerMeter);

constexpr Quantity QUANTITY_BY_NAME(Pascal);
constexpr Quantity QUANTITY_BY_NAME(KiloPascal);
constexpr Quantity QUANTITY_BY_NAME(MegaPascal);
constexpr Quantity QUANTITY_BY_NAME(GigaPascal);

constexpr Quantity QUANTITY_BY_NAME(MilliBar);
constexpr Quantity QUANTITY_BY_NAME(Bar);

constexpr Quantity QUANTITY_BY_NAME(Torr);
constexpr Quantity QUANTITY_BY_NAME(mTorr);
constexpr Quantity QUANTITY_BY_NAME(yTorr);

constexpr Quantity QUANTITY_BY_NAME(PSI);
constexpr Quantity QUANTITY_BY_NAME(KSI);
constexpr Quantity QUANTITY_BY_NAME(MPSI);

constexpr Quantity QUANTITY_BY_NAME(Watt);
constexpr Quantity QUANTITY_BY_NAME(MilliWatt);
constexpr Quantity QUANTITY_BY_NAME(KiloWatt);
constexpr Quantity QUANTITY_BY_NAME(VoltAmpere);

constexpr Quantity QUANTITY_BY_NAME(Volt);
constexpr Quantity QUANTITY_BY_NAME(MilliVolt);
constexpr Quantity QUANTITY_BY_NAME(KiloVolt);

constexpr Quantity QUANTITY_BY_NAME(MegaSiemens);
constexpr Quantity QUANTITY_BY_NAME(KiloSiemens);
constexpr Quantity QUANTITY_BY_NAME(Siemens);
constexpr Quantity QUANTITY_BY_NAME(MilliSiemens);
constexpr Quantity QUANTITY_BY_NAME(MicroSiemens);

constexpr Quantity QUANTITY_BY_NAME(Ohm);
constexpr Quantity QUANTITY_BY_NAME(KiloOhm);
constexpr Quantity QUANTITY_BY_NAME(MegaOhm);

constexpr Quantity QUANTITY_BY_NAME(Coulomb);

constexpr Quantity QUANTITY_BY_NAME(Tesla);
constexpr Quantity QUANTITY_BY_NAME(MilliTesla);
constexpr Quantity QUANTITY_BY_NAME(Gauss);

constexpr Quantity QUANTITY_BY_NAME(Weber);

constexpr Quantity QUANTITY_BY_NAME(PicoFarad);
constexpr Quantity QUANTITY_BY_NAME(NanoFarad);
constexpr Quantity QUANTITY_BY_NAME(MicroFarad);
constexpr Quantity QUANTITY_BY_NAME(MilliFarad);
constexpr Quantity QUANTITY_BY_NAME(Farad);

constexpr Quantity QUANTITY_BY_NAME(NanoHenry);
constexpr Quantity QUANTITY_BY_NAME(MicroHenry);
constexpr Quantity QUANTITY_BY_NAME(MilliHenry);
constexpr Quantity QUANTITY_BY_NAME(Henry);

constexpr Quantity QUANTITY_BY_NAME(Joule);
constexpr Quantity QUANTITY_BY_NAME(MilliJoule);
constexpr Quantity QUANTITY_BY_NAME(KiloJoule);
constexpr Quantity QUANTITY_BY_NAME(VoltAmpereSecond);
constexpr Quantity QUANTITY_BY_NAME(WattSecond);
constexpr Quantity QUANTITY_BY_NAME(KiloWattHour);
constexpr Quantity QUANTITY_BY_NAME(ElectronVolt);
constexpr Quantity QUANTITY_BY_NAME(KiloElectronVolt);
constexpr Quantity QUANTITY_BY_NAME(MegaElectronVolt);
constexpr Quantity QUANTITY_BY_NAME(Calorie);
constexpr Quantity QUANTITY_BY_NAME(KiloCalorie);
constexpr Quantity QUANTITY_BY_NAME(NewtonMeter);

constexpr Quantity QUANTITY_BY_NAME(KMH);
constexpr Quantity QUANTITY_BY_NAME(MPH);

constexpr Quantity QUANTITY_BY_NAME(AngMinute);
constexpr Quantity QUANTITY_BY_NAME(AngSecond);
constexpr Quantity QUANTITY_BY_NAME(Degree);
constexpr Quantity QUANTITY_BY_NAME(Radian);
constexpr Quantity QUANTITY_BY_NAME(Gon);
// clang-format on

#undef QUANTITY_BY_NAME

// === Predefined quantity registry queries ==================================

std::vector<Base::QuantitySpec const*> Quantity::predefinedQuantities(const Unit& unit)
{
    std::vector<QuantitySpec const*> result;
    for (const auto& spec : QuantitySpecsData::specs) {
        if (Unit {spec.exps} == unit) {
            result.push_back(&spec);
        }
    }
    return result;
}

std::span<const Base::QuantitySpec> Quantity::predefinedQuantities()
{
    return QuantitySpecsData::specs;
}

const Base::QuantitySpec* Quantity::findPredefined(std::string_view name)
{
    const auto& specs = QuantitySpecsData::specs;
    auto it = std::ranges::find(specs, name, &QuantitySpec::name);
    return it != specs.end() ? &(*it) : nullptr;
}

// === Parser & Scanner stuff ===============================================

// include the Scanner and the Parser for the 'Quantity's

// NOLINTNEXTLINE
Quantity QuantResult;

/* helper function for tuning number strings with groups in a locale agnostic way... */
// NOLINTBEGIN
double num_change(char* yytext, char dez_delim, char grp_delim)
{
    double ret_val {};
    const int num = 40;
    std::array<char, num> temp {};
    int iter = 0;
    for (char* ch = yytext; *ch != '\0'; ch++) {
        // skip group delimiter
        if (*ch == grp_delim) {
            continue;
        }
        // check for a dez delimiter other then dot
        if (*ch == dez_delim && dez_delim != '.') {
            temp[iter++] = '.';
        }
        else {
            temp[iter++] = *ch;
        }
        // check buffer overflow
        if (iter >= num) {
            return 0.0;
        }
    }

    temp[iter] = '\0';

    ret_val = atof(temp.data());
    return ret_val;
}
// NOLINTEND

#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wmissing-noreturn"
#endif

// error func
void Quantity_yyerror(const char* errorinfo)
{
    throw Base::ParserError(errorinfo);
}

#if defined(__clang__)
# pragma clang diagnostic pop
#endif


#if defined(__clang__)
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wsign-compare"
# pragma clang diagnostic ignored "-Wunneeded-internal-declaration"
#elif defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wsign-compare"
# pragma GCC diagnostic ignored "-Wfree-nonheap-object"
#endif

namespace QuantityParser
{

// NOLINTNEXTLINE
#define YYINITDEPTH 20
// show parser the lexer method
#define yylex QuantityLexer
int QuantityLexer();

// Parser, defined in Quantity.y
// NOLINTNEXTLINE
#include "Quantity.tab.c"

#ifndef DOXYGEN_SHOULD_SKIP_THIS
// Scanner, defined in Quantity.l
// NOLINTNEXTLINE
# include "Quantity.lex.c"
#endif  // DOXYGEN_SHOULD_SKIP_THIS

class StringBufferCleaner
{
public:
    explicit StringBufferCleaner(YY_BUFFER_STATE buffer)
        : my_string_buffer {buffer}
    {}
    ~StringBufferCleaner()
    {
        // free the scan buffer
        yy_delete_buffer(my_string_buffer);
    }

    StringBufferCleaner(const StringBufferCleaner&) = delete;
    StringBufferCleaner(StringBufferCleaner&&) = delete;
    StringBufferCleaner& operator=(const StringBufferCleaner&) = delete;
    StringBufferCleaner& operator=(StringBufferCleaner&&) = delete;

private:
    YY_BUFFER_STATE my_string_buffer;
};

}  // namespace QuantityParser

#if defined(__clang__)
# pragma clang diagnostic pop
#elif defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

Quantity Quantity::parse(const std::string& string)
{
    // parse from buffer
    QuantityParser::YY_BUFFER_STATE my_string_buffer = QuantityParser::yy_scan_string(string.c_str());
    QuantityParser::StringBufferCleaner cleaner(my_string_buffer);
    // set the global return variables
    QuantResult = Quantity(std::numeric_limits<double>::min());
    // run the parser
    QuantityParser::yyparse();

    return QuantResult;
}
