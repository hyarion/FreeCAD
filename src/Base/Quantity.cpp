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

#include <array>
#include <cmath>
#include <limits>
#include <numbers>
#include <sstream>
#include <string>

#include <fmt/format.h>

#include "Exception.h"
#include "Quantity.h"
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

// === Predefined types =====================================================
// clang-format off
using namespace Base::UnitsConvData;

constexpr Quantity Quantity::NanoMetre           ( 1.0e-6                , Unit::Length                  );
constexpr Quantity Quantity::MicroMetre          ( 1.0e-3                , Unit::Length                  );
constexpr Quantity Quantity::MilliMetre          ( 1.0                   , Unit::Length                  );
constexpr Quantity Quantity::CentiMetre          ( 10.0                  , Unit::Length                  );
constexpr Quantity Quantity::DeciMetre           ( 100.0                 , Unit::Length                  );
constexpr Quantity Quantity::Metre               ( 1.0e3                 , Unit::Length                  );
constexpr Quantity Quantity::KiloMetre           ( 1.0e6                 , Unit::Length                  );

constexpr Quantity Quantity::MilliLiter          ( 1000.0                , Unit::Volume                  );
constexpr Quantity Quantity::Liter               ( 1.0e6                 , Unit::Volume                  );

constexpr Quantity Quantity::Hertz               ( 1.0                   , Unit::Frequency               );
constexpr Quantity Quantity::KiloHertz           ( 1.0e3                 , Unit::Frequency               );
constexpr Quantity Quantity::MegaHertz           ( 1.0e6                 , Unit::Frequency               );
constexpr Quantity Quantity::GigaHertz           ( 1.0e9                 , Unit::Frequency               );
constexpr Quantity Quantity::TeraHertz           ( 1.0e12                , Unit::Frequency               );

constexpr Quantity Quantity::MicroGram           ( 1.0e-9                , Unit::Mass                    );
constexpr Quantity Quantity::MilliGram           ( 1.0e-6                , Unit::Mass                    );
constexpr Quantity Quantity::Gram                ( 1.0e-3                , Unit::Mass                    );
constexpr Quantity Quantity::KiloGram            ( 1.0                   , Unit::Mass                    );
constexpr Quantity Quantity::Ton                 ( 1.0e3                 , Unit::Mass                    );

constexpr Quantity Quantity::Second              ( 1.0                   , Unit::TimeSpan                );
constexpr Quantity Quantity::Minute              ( 60.0                  , Unit::TimeSpan                );
constexpr Quantity Quantity::Hour                ( 3600.0                , Unit::TimeSpan                );

constexpr Quantity Quantity::Ampere              ( 1.0                   , Unit::ElectricCurrent         );
constexpr Quantity Quantity::MilliAmpere         ( 0.001                 , Unit::ElectricCurrent         );
constexpr Quantity Quantity::KiloAmpere          ( 1000.0                , Unit::ElectricCurrent         );
constexpr Quantity Quantity::MegaAmpere          ( 1.0e6                 , Unit::ElectricCurrent         );

constexpr Quantity Quantity::Kelvin              ( 1.0                   , Unit::Temperature             );
constexpr Quantity Quantity::MilliKelvin         ( 0.001                 , Unit::Temperature             );
constexpr Quantity Quantity::MicroKelvin         ( 0.000001              , Unit::Temperature             );

constexpr Quantity Quantity::MilliMole           ( 0.001                 , Unit::AmountOfSubstance       );
constexpr Quantity Quantity::Mole                ( 1.0                   , Unit::AmountOfSubstance       );

constexpr Quantity Quantity::Candela             ( 1.0                   , Unit::LuminousIntensity       );

constexpr Quantity Quantity::Inch                ( in                    , Unit::Length                  );
constexpr Quantity Quantity::Foot                ( ft                    , Unit::Length                  );
constexpr Quantity Quantity::Thou                ( in / 1000             , Unit::Length                  );
constexpr Quantity Quantity::Yard                ( yd                    , Unit::Length                  );
constexpr Quantity Quantity::Mile                ( mi                    , Unit::Length                  );

constexpr Quantity Quantity::MilePerHour         ( mi / 3600             , Unit::Velocity                );

constexpr Quantity Quantity::SquareFoot          ( ft * ft               , Unit::Area                    );
constexpr Quantity Quantity::CubicFoot           ( ft * ft * ft          , Unit::Volume                  );

constexpr Quantity Quantity::Pound               ( lb                    , Unit::Mass                    );
constexpr Quantity Quantity::Ounce               ( lb / 16               , Unit::Mass                    );
constexpr Quantity Quantity::Stone               ( lb * 14               , Unit::Mass                    );
constexpr Quantity Quantity::Hundredweights      ( lb * 112              , Unit::Mass                    );

constexpr Quantity Quantity::PoundForce          ( 1000 * lbf            , Unit::Force                   );

constexpr Quantity Quantity::Newton              ( 1000.0                , Unit::Force                   );
constexpr Quantity Quantity::MilliNewton         ( 1.0                   , Unit::Force                   );
constexpr Quantity Quantity::KiloNewton          ( 1e+6                  , Unit::Force                   );
constexpr Quantity Quantity::MegaNewton          ( 1e+9                  , Unit::Force                   );

constexpr Quantity Quantity::NewtonPerMeter      ( 1.00                  , Unit::Stiffness               );
constexpr Quantity Quantity::MilliNewtonPerMeter ( 1e-3                  , Unit::Stiffness               );
constexpr Quantity Quantity::KiloNewtonPerMeter  ( 1e3                   , Unit::Stiffness               );
constexpr Quantity Quantity::MegaNewtonPerMeter  ( 1e6                   , Unit::Stiffness               );

constexpr Quantity Quantity::Pascal              ( 0.001                 , Unit::Pressure                );
constexpr Quantity Quantity::KiloPascal          ( 1.00                  , Unit::Pressure                );
constexpr Quantity Quantity::MegaPascal          ( 1000.0                , Unit::Pressure                );
constexpr Quantity Quantity::GigaPascal          ( 1e+6                  , Unit::Pressure                );

constexpr Quantity Quantity::MilliBar            ( 0.1                   , Unit::Pressure                );
constexpr Quantity Quantity::Bar                 ( 100.0                 , Unit::Pressure                );

constexpr Quantity Quantity::Torr                ( 101.325 / 760.0       , Unit::Pressure                );
constexpr Quantity Quantity::mTorr               ( 101.325 / 760.0 / 1e3 , Unit::Pressure                );
constexpr Quantity Quantity::yTorr               ( 101.325 / 760.0 / 1e6 , Unit::Pressure                );

constexpr Quantity Quantity::PSI                 ( psi                   , Unit::Pressure                );
constexpr Quantity Quantity::KSI                 ( psi * 1000            , Unit::Pressure                );
constexpr Quantity Quantity::MPSI                ( psi * 1000000         , Unit::Pressure                );

constexpr Quantity Quantity::Watt                ( 1e+6                  , Unit::Power                   );
constexpr Quantity Quantity::MilliWatt           ( 1e+3                  , Unit::Power                   );
constexpr Quantity Quantity::KiloWatt            ( 1e+9                  , Unit::Power                   );
constexpr Quantity Quantity::VoltAmpere          ( 1e+6                  , Unit::Power                   );

constexpr Quantity Quantity::Volt                ( 1e+6                  , Unit::ElectricPotential       );
constexpr Quantity Quantity::MilliVolt           ( 1e+3                  , Unit::ElectricPotential       );
constexpr Quantity Quantity::KiloVolt            ( 1e+9                  , Unit::ElectricPotential       );

constexpr Quantity Quantity::MegaSiemens         ( 1.0                   , Unit::ElectricalConductance   );
constexpr Quantity Quantity::KiloSiemens         ( 1e-3                  , Unit::ElectricalConductance   );
constexpr Quantity Quantity::Siemens             ( 1e-6                  , Unit::ElectricalConductance   );
constexpr Quantity Quantity::MilliSiemens        ( 1e-9                  , Unit::ElectricalConductance   );
constexpr Quantity Quantity::MicroSiemens        ( 1e-12                 , Unit::ElectricalConductance   );

constexpr Quantity Quantity::Ohm                 ( 1e+6                  , Unit::ElectricalResistance    );
constexpr Quantity Quantity::KiloOhm             ( 1e+9                  , Unit::ElectricalResistance    );
constexpr Quantity Quantity::MegaOhm             ( 1e+12                 , Unit::ElectricalResistance    );

constexpr Quantity Quantity::Coulomb             ( 1.0                   , Unit::ElectricCharge          );

constexpr Quantity Quantity::Tesla               ( 1.0                   , Unit::MagneticFluxDensity     );
constexpr Quantity Quantity::MilliTesla          ( 1e-3                  , Unit::MagneticFluxDensity     );
constexpr Quantity Quantity::Gauss               ( 1e-4                  , Unit::MagneticFluxDensity     );

constexpr Quantity Quantity::Weber               ( 1e6                   , Unit::MagneticFlux            );

constexpr Quantity Quantity::PicoFarad           ( 1e-18                 , Unit::ElectricalCapacitance   );
constexpr Quantity Quantity::NanoFarad           ( 1e-15                 , Unit::ElectricalCapacitance   );
constexpr Quantity Quantity::MicroFarad          ( 1e-12                 , Unit::ElectricalCapacitance   );
constexpr Quantity Quantity::MilliFarad          ( 1e-9                  , Unit::ElectricalCapacitance   );
constexpr Quantity Quantity::Farad               ( 1e-6                  , Unit::ElectricalCapacitance   );

constexpr Quantity Quantity::NanoHenry           ( 1e-3                  , Unit::ElectricalInductance    );
constexpr Quantity Quantity::MicroHenry          ( 1.0                   , Unit::ElectricalInductance    );
constexpr Quantity Quantity::MilliHenry          ( 1e+3                  , Unit::ElectricalInductance    );
constexpr Quantity Quantity::Henry               ( 1e+6                  , Unit::ElectricalInductance    );

constexpr Quantity Quantity::Joule               ( 1e+6                  , Unit::Work                    );
constexpr Quantity Quantity::MilliJoule          ( 1e+3                  , Unit::Work                    );
constexpr Quantity Quantity::KiloJoule           ( 1e+9                  , Unit::Work                    );
constexpr Quantity Quantity::VoltAmpereSecond    ( 1e+6                  , Unit::Work                    );
constexpr Quantity Quantity::WattSecond          ( 1e+6                  , Unit::Work                    );
constexpr Quantity Quantity::KiloWattHour        ( 3.6e+12               , Unit::Work                    );
constexpr Quantity Quantity::ElectronVolt        ( 1.602176634e-13       , Unit::Work                    );
constexpr Quantity Quantity::KiloElectronVolt    ( 1.602176634e-10       , Unit::Work                    );
constexpr Quantity Quantity::MegaElectronVolt    ( 1.602176634e-7        , Unit::Work                    );
constexpr Quantity Quantity::Calorie             ( 4.1868e+6             , Unit::Work                    );
constexpr Quantity Quantity::KiloCalorie         ( 4.1868e+9             , Unit::Work                    );
constexpr Quantity Quantity::NewtonMeter         ( 1e+6                  , Unit::Moment                  );

constexpr Quantity Quantity::KMH                 ( 1e+6 / 3600           , Unit::Velocity                );
constexpr Quantity Quantity::MPH                 ( mi / 3600             , Unit::Velocity                );

constexpr Quantity Quantity::AngMinute           ( 1.0 / 60.0            , Unit::Angle                   );
constexpr Quantity Quantity::AngSecond           ( 1.0 / 3600.0          , Unit::Angle                   );
constexpr Quantity Quantity::Degree              ( 1.0                   , Unit::Angle                   );
constexpr Quantity Quantity::Radian              ( 180 / std::numbers::pi, Unit::Angle                   );
constexpr Quantity Quantity::Gon                 ( 360.0 / 400.0         , Unit::Angle                   );
// clang-format on

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
