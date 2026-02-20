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

#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#include <FCGlobal.h>

#include "Exception.h"

namespace Base
{

constexpr auto unitSymbols = std::to_array<std::string_view>(
    {"mm", "kg", "s", "A", "K", "mol", "cd", "deg"}
);

constexpr auto unitNumExponents {unitSymbols.size()};
using UnitExponents = std::array<int8_t, unitNumExponents>;

constexpr auto unitExponentLimit {8};

struct UnitSpec
{
    std::string_view name;
    UnitExponents exps;
};

constexpr auto unitSpecs = std::to_array<UnitSpec>({
    // clang-format off
    //                                             Length
    //                                             .   Mass
    //                                             .   .   Time
    //                                             .   .   .   ElectricCurrent
    //                                             .   .   .   .   ThermodynamicTemperature
    //                                             .   .   .   .   .   AmountOfSubstance
    //                                             .   .   .   .   .   .   LuminousIntensity
    //                                             .   .   .   .   .   .   .   Angle
    { "1"                                     , {  0,  0,  0,  0,  0,  0,  0,  0 } },
    { "Length"                                , {  1                             } },
    { "Mass"                                  , {  0,  1                         } },
    { "TimeSpan"                              , {  0,  0,  1                     } },
    { "ElectricCurrent"                       , {  0,  0,  0,  1                 } },
    { "Temperature"                           , {  0,  0,  0,  0,  1             } },
    { "AmountOfSubstance"                     , {  0,  0,  0,  0,  0,  1         } },
    { "LuminousIntensity"                     , {  0,  0,  0,  0,  0,  0,  1     } },
    { "Angle"                                 , {  0,  0,  0,  0,  0,  0,  0,  1 } },
    { "Acceleration"                          , {  1,  0, -2                     } },
    { "AngleOfFriction"                       , {  0,  0,  0,  0,  0,  0,  0,  1 } },
    { "Area"                                  , {  2                             } },
    { "CurrentDensity"                        , { -2,  0,  0,  1                 } },
    { "Density"                               , { -3,  1                         } },
    { "DissipationRate"                       , {  2,  0, -3                     } },
    { "DynamicViscosity"                      , { -1,  1, -1                     } },
    { "ElectricalCapacitance"                 , { -2, -1,  4,  2                 } },
    { "ElectricalConductance"                 , { -2, -1,  3,  2                 } },
    { "ElectricalConductivity"                , { -3, -1,  3,  2                 } },
    { "ElectricalInductance"                  , {  2,  1, -2, -2                 } },
    { "ElectricalResistance"                  , {  2,  1, -3, -2                 } },
    { "ElectricCharge"                        , {  0,  0,  1,  1                 } },
    { "ElectricPotential"                     , {  2,  1, -3, -1                 } },
    { "ElectromagneticPotential"              , {  1,  1, -2, -1                 } },
    { "Force"                                 , {  1,  1, -2                     } },
    { "Frequency"                             , {  0,  0, -1                     } },
    { "HeatFlux"                              , {  0,  1, -3                     } },
    { "InverseArea"                           , { -2                             } },
    { "InverseLength"                         , { -1                             } },
    { "InverseVolume"                         , { -3                             } },
    { "KinematicViscosity"                    , {  2,  0, -1                     } },
    { "MagneticFieldStrength"                 , { -1,  0,  0,  1                 } },
    { "MagneticFlux"                          , {  2,  1, -2, -1                 } },
    { "MagneticFluxDensity"                   , {  0,  1, -2, -1                 } },
    { "Magnetization"                         , { -1,  0,  0,  1                 } },
    { "Moment"                                , {  2,  1, -2                     } },
    { "Pressure"                              , { -1,  1, -2                     } },
    { "Power"                                 , {  2,  1, -3                     } },
    { "ShearModulus"                          , { -1,  1, -2                     } },
    { "SpecificEnergy"                        , {  2,  0, -2                     } },
    { "SpecificHeat"                          , {  2,  0, -2,  0, -1             } },
    { "Stiffness"                             , {  0,  1, -2                     } },
    { "StiffnessDensity"                      , { -2,  1, -2                     } },
    { "Stress"                                , { -1,  1, -2                     } },
    { "SurfaceChargeDensity"                  , { -2,  0,  1,  1                 } },
    { "ThermalConductivity"                   , {  1,  1, -3,  0, -1             } },
    { "ThermalExpansionCoefficient"           , {  0,  0,  0,  0, -1             } },
    { "ThermalTransferCoefficient"            , {  0,  1, -3,  0, -1             } },
    { "UltimateTensileStrength"               , { -1,  1, -2                     } },
    { "VacuumPermittivity"                    , { -3, -1,  4,  2                 } },
    { "Velocity"                              , {  1,  0, -1                     } },
    { "Volume"                                , {  3                             } },
    { "VolumeChargeDensity"                   , { -3,  0,  1,  1                 } },
    { "VolumeFlowRate"                        , {  3,  0, -1                     } },
    { "VolumetricThermalExpansionCoefficient" , {  0,  0,  0,  0, -1             } },
    { "Work"                                  , {  2,  1, -2                     } },
    { "YieldStrength"                         , { -1,  1, -2                     } },
    { "YoungsModulus"                         , { -1,  1, -2                     } },
});  // clang-format on

class BaseExport Unit final
{
public:
    Unit() = default;

    explicit constexpr Unit(const UnitExponents exps, const std::string_view name = "")
        : _exps {exps}
        , _name {name}
    {
        checkRange();
    }

    /// helper constructor to ease Unit construction from Python
    explicit Unit(
        const int length,
        const int mass = 0,
        const int time = 0,
        const int electricCurrent = 0,
        const int thermodynamicTemperature = 0,
        const int amountOfSubstance = 0,
        const int luminousIntensity = 0,
        const int angle = 0
    );

    bool operator==(const Unit&) const;
    bool operator!=(const Unit& that) const;
    Unit& operator*=(const Unit& that);
    Unit& operator/=(const Unit& that);
    Unit operator*(const Unit&) const;
    Unit operator/(const Unit&) const;

    [[nodiscard]] Unit pow(const double exp) const;
    [[nodiscard]] Unit root(const uint8_t num) const;

    [[nodiscard]] UnitExponents exponents() const;
    [[nodiscard]] int length() const;

    [[nodiscard]] std::string getString() const;       // E.g. kg, mm^2, mm*kg/s^2
    [[nodiscard]] std::string getTypeString() const;   // E.g. "Area", "Length", "Pressure"
    [[nodiscard]] std::string representation() const;  // E.g. "Unit: mm (1,0,0,0,0,0,0,0) [Length]"

    Unit sqrt() const
    {
        return root(2);
    }
    Unit cbrt() const
    {
        return root(3);
    }

private:
    UnitExponents _exps {};
    std::string_view _name;

    constexpr void checkRange()
    {
        for (const auto exp : _exps) {
            if (exp >= unitExponentLimit) {
                throw OverflowError("Unit exponent overflow");
            }
            if (exp < -unitExponentLimit) {
                throw UnderflowError("Unit exponent underflow");
            }
        }
    }

    /** Returns posIndexes, negIndexes*/
    std::pair<std::vector<size_t>, std::vector<size_t>> nonZeroValsIndexes() const;

    /// Creates a Unit by looking up its name in unitSpecs
    static constexpr Unit makeUnit(const std::string_view name);

public:
    static const Unit Acceleration;
    static const Unit AmountOfSubstance;
    static const Unit Angle;
    static const Unit AngleOfFriction;
    static const Unit Area;
    static const Unit CompressiveStrength;
    static const Unit CurrentDensity;
    static const Unit Density;
    static const Unit DissipationRate;
    static const Unit DynamicViscosity;
    static const Unit ElectricalCapacitance;
    static const Unit ElectricalConductance;
    static const Unit ElectricalConductivity;
    static const Unit ElectricalInductance;
    static const Unit ElectricalResistance;
    static const Unit ElectricCharge;
    static const Unit ElectricCurrent;
    static const Unit ElectricPotential;
    static const Unit ElectromagneticPotential;
    static const Unit Force;
    static const Unit Frequency;
    static const Unit HeatFlux;
    static const Unit InverseArea;
    static const Unit InverseLength;
    static const Unit InverseVolume;
    static const Unit KinematicViscosity;
    static const Unit Length;
    static const Unit LuminousIntensity;
    static const Unit MagneticFieldStrength;
    static const Unit MagneticFlux;
    static const Unit MagneticFluxDensity;
    static const Unit Magnetization;
    static const Unit Mass;
    static const Unit Moment;
    static const Unit One;
    static const Unit Pressure;
    static const Unit Power;
    static const Unit ShearModulus;
    static const Unit SpecificEnergy;
    static const Unit SpecificHeat;
    static const Unit Stiffness;
    static const Unit StiffnessDensity;
    static const Unit Stress;
    static const Unit SurfaceChargeDensity;
    static const Unit Temperature;
    static const Unit TimeSpan;
    static const Unit ThermalConductivity;
    static const Unit ThermalExpansionCoefficient;
    static const Unit ThermalTransferCoefficient;
    static const Unit UltimateTensileStrength;
    static const Unit VacuumPermittivity;
    static const Unit Velocity;
    static const Unit Volume;
    static const Unit VolumeChargeDensity;
    static const Unit VolumeFlowRate;
    static const Unit VolumetricThermalExpansionCoefficient;
    static const Unit Work;
    static const Unit YieldStrength;
    static const Unit YoungsModulus;
};

constexpr Unit Unit::makeUnit(const std::string_view name)
{
    if (const auto spec = std::ranges::find(unitSpecs, name, &UnitSpec::name);
        spec != unitSpecs.end()) {
        return Unit {spec->exps, spec->name};
    }
    throw NameError("Invalid unit name");
}

// Stringification macro to avoid typos when member name matches spec name
#define UNIT_BY_NAME(name) Unit::name = Unit::makeUnit(#name)

// clang-format off
inline constexpr Unit UNIT_BY_NAME(Acceleration);
inline constexpr Unit UNIT_BY_NAME(AmountOfSubstance);
inline constexpr Unit UNIT_BY_NAME(Angle);
inline constexpr Unit Unit::AngleOfFriction                       = Unit::makeUnit("Angle"                       );
inline constexpr Unit UNIT_BY_NAME(Area);
inline constexpr Unit Unit::CompressiveStrength                   = Unit::makeUnit("Pressure"                    );
inline constexpr Unit UNIT_BY_NAME(CurrentDensity);
inline constexpr Unit UNIT_BY_NAME(Density);
inline constexpr Unit UNIT_BY_NAME(DissipationRate);
inline constexpr Unit UNIT_BY_NAME(DynamicViscosity);
inline constexpr Unit UNIT_BY_NAME(ElectricalCapacitance);
inline constexpr Unit UNIT_BY_NAME(ElectricalConductance);
inline constexpr Unit UNIT_BY_NAME(ElectricalConductivity);
inline constexpr Unit UNIT_BY_NAME(ElectricalInductance);
inline constexpr Unit UNIT_BY_NAME(ElectricalResistance);
inline constexpr Unit UNIT_BY_NAME(ElectricCharge);
inline constexpr Unit UNIT_BY_NAME(ElectricCurrent);
inline constexpr Unit UNIT_BY_NAME(ElectricPotential);
inline constexpr Unit UNIT_BY_NAME(ElectromagneticPotential);
inline constexpr Unit UNIT_BY_NAME(Force);
inline constexpr Unit UNIT_BY_NAME(Frequency);
inline constexpr Unit UNIT_BY_NAME(HeatFlux);
inline constexpr Unit UNIT_BY_NAME(InverseArea);
inline constexpr Unit UNIT_BY_NAME(InverseLength);
inline constexpr Unit UNIT_BY_NAME(InverseVolume);
inline constexpr Unit UNIT_BY_NAME(KinematicViscosity);
inline constexpr Unit UNIT_BY_NAME(Length);
inline constexpr Unit UNIT_BY_NAME(LuminousIntensity);
inline constexpr Unit UNIT_BY_NAME(MagneticFieldStrength);
inline constexpr Unit UNIT_BY_NAME(MagneticFlux);
inline constexpr Unit UNIT_BY_NAME(MagneticFluxDensity);
inline constexpr Unit UNIT_BY_NAME(Magnetization);
inline constexpr Unit UNIT_BY_NAME(Mass);
inline constexpr Unit UNIT_BY_NAME(Moment);
inline constexpr Unit Unit::One                                   = Unit::makeUnit("1"                           );
inline constexpr Unit UNIT_BY_NAME(Pressure);
inline constexpr Unit UNIT_BY_NAME(Power);
inline constexpr Unit Unit::ShearModulus                          = Unit::makeUnit("Pressure"                    );
inline constexpr Unit UNIT_BY_NAME(SpecificEnergy);
inline constexpr Unit UNIT_BY_NAME(SpecificHeat);
inline constexpr Unit UNIT_BY_NAME(Stiffness);
inline constexpr Unit UNIT_BY_NAME(StiffnessDensity);
inline constexpr Unit Unit::Stress                                = Unit::makeUnit("Pressure"                    );
inline constexpr Unit UNIT_BY_NAME(SurfaceChargeDensity);
inline constexpr Unit UNIT_BY_NAME(Temperature);
inline constexpr Unit UNIT_BY_NAME(TimeSpan);
inline constexpr Unit UNIT_BY_NAME(ThermalConductivity);
inline constexpr Unit UNIT_BY_NAME(ThermalExpansionCoefficient);
inline constexpr Unit UNIT_BY_NAME(ThermalTransferCoefficient);
inline constexpr Unit Unit::UltimateTensileStrength               = Unit::makeUnit("Pressure"                    );
inline constexpr Unit UNIT_BY_NAME(VacuumPermittivity);
inline constexpr Unit UNIT_BY_NAME(Velocity);
inline constexpr Unit UNIT_BY_NAME(Volume);
inline constexpr Unit UNIT_BY_NAME(VolumeChargeDensity);
inline constexpr Unit UNIT_BY_NAME(VolumeFlowRate);
inline constexpr Unit Unit::VolumetricThermalExpansionCoefficient = Unit::makeUnit("ThermalExpansionCoefficient" );
inline constexpr Unit UNIT_BY_NAME(Work);
inline constexpr Unit Unit::YieldStrength                         = Unit::makeUnit("Pressure"                    );
inline constexpr Unit Unit::YoungsModulus                         = Unit::makeUnit("Pressure"                    );
// clang-format on

#undef UNIT_BY_NAME

}  // namespace Base
