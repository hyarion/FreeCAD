// SPDX-License-Identifier: LGPL-2.1-or-later

/************************************************************************
 *                                                                      *
 *   This file is part of the FreeCAD CAx development system.           *
 *                                                                      *
 *   This library is free software; you can redistribute it and/or      *
 *   modify it under the terms of the GNU Library General Public        *
 *   License as published by the Free Software Foundation; either       *
 *   version 2 of the License, or (at your option) any later version.   *
 *                                                                      *
 *   This library  is distributed in the hope that it will be useful,   *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of     *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the      *
 *   GNU Library General Public License for more details.               *
 *                                                                      *
 *   You should have received a copy of the GNU Library General Public  *
 *   License along with this library; see the file COPYING.LIB. If not, *
 *   write to the Free Software Foundation, Inc., 59 Temple Place,      *
 *   Suite 330, Boston, MA  02111-1307, USA                             *
 *                                                                      *
 ************************************************************************/

#ifndef BASE_QUANTITYSPECSDATA_H
#define BASE_QUANTITYSPECSDATA_H

#include <algorithm>
#include <array>
#include <span>
#include <string_view>
#include <vector>

#include "Unit.h"
#include "UnitsConvData.h"

namespace Base
{

struct QuantitySpec
{
    std::string_view name;
    std::string_view symbol;
    double value;
    Unit unit;
};

namespace QuantitySpecsData
{
using namespace UnitsConvData;

// clang-format off
constexpr auto specs = std::to_array<QuantitySpec>({
    //  Name                     Symbol          Value                           Unit
    // --- Length ---
    { "NanoMetre"             , "nm"           , 1.0e-6                        , Unit::Length },
    { "MicroMetre"            , "\xC2\xB5m"    , 1.0e-3                        , Unit::Length },
    { "MilliMetre"            , "mm"           , 1.0                           , Unit::Length },
    { "CentiMetre"            , "cm"           , 10.0                          , Unit::Length },
    { "DeciMetre"             , "dm"           , 100.0                         , Unit::Length },
    { "Metre"                 , "m"            , 1.0e3                         , Unit::Length },
    { "KiloMetre"             , "km"           , 1.0e6                         , Unit::Length },
    { "Inch"                  , "in"           , in                            , Unit::Length },
    { "Foot"                  , "ft"           , ft                            , Unit::Length },
    { "Thou"                  , "thou"         , in / 1000                     , Unit::Length },
    { "Yard"                  , "yd"           , yd                            , Unit::Length },
    { "Mile"                  , "mi"           , mi                            , Unit::Length },

    // --- Area ---
    { "SquareMilliMetre"      , "mm^2"         , 1.0                           , Unit::Area },
    { "SquareCentiMetre"      , "cm^2"         , 1e2                           , Unit::Area },
    { "SquareMetre"           , "m^2"          , 1e6                           , Unit::Area },
    { "SquareKiloMetre"       , "km^2"         , 1e12                          , Unit::Area },
    { "SquareInch"            , "in^2"         , in * in                       , Unit::Area },
    { "SquareFoot"            , "sqft"         , ft * ft                       , Unit::Area },
    { "SquareFoot2"           , "ft^2"         , ft * ft                       , Unit::Area },

    // --- Volume ---
    { "CubicMilliMetre"       , "mm^3"         , 1.0                           , Unit::Volume },
    { "MilliLiter"            , "ml"           , 1000.0                        , Unit::Volume },
    { "Liter"                 , "l"            , 1.0e6                         , Unit::Volume },
    { "CubicMetre"            , "m^3"          , 1e9                           , Unit::Volume },
    { "CubicInch"             , "in^3"         , in * in * in                  , Unit::Volume },
    { "CubicFoot"             , "cft"          , ft * ft * ft                  , Unit::Volume },
    { "CubicFoot2"            , "ft^3"         , ft * ft * ft                  , Unit::Volume },

    // --- Frequency ---
    { "Hertz"                 , "Hz"           , 1.0                           , Unit::Frequency },
    { "KiloHertz"             , "kHz"          , 1.0e3                         , Unit::Frequency },
    { "MegaHertz"             , "MHz"          , 1.0e6                         , Unit::Frequency },
    { "GigaHertz"             , "GHz"          , 1.0e9                         , Unit::Frequency },
    { "TeraHertz"             , "THz"          , 1.0e12                        , Unit::Frequency },

    // --- Mass ---
    { "MicroGram"             , "\xC2\xB5g"    , 1.0e-9                        , Unit::Mass },
    { "MilliGram"             , "mg"           , 1.0e-6                        , Unit::Mass },
    { "Gram"                  , "g"            , 1.0e-3                        , Unit::Mass },
    { "KiloGram"              , "kg"           , 1.0                           , Unit::Mass },
    { "Ton"                   , "t"            , 1.0e3                         , Unit::Mass },
    { "Pound"                 , "lb"           , lb                            , Unit::Mass },
    { "Ounce"                 , "oz"           , lb / 16                       , Unit::Mass },
    { "Stone"                 , "st"           , lb * 14                       , Unit::Mass },
    { "Hundredweights"        , "cwt"          , lb * 112                      , Unit::Mass },

    // --- Time ---
    { "Second"                , "s"            , 1.0                           , Unit::TimeSpan },
    { "Minute"                , "min"          , 60.0                          , Unit::TimeSpan },
    { "Hour"                  , "h"            , 3600.0                        , Unit::TimeSpan },

    // --- Electric Current ---
    { "Ampere"                , "A"            , 1.0                           , Unit::ElectricCurrent },
    { "MilliAmpere"           , "mA"           , 0.001                         , Unit::ElectricCurrent },
    { "KiloAmpere"            , "kA"           , 1000.0                        , Unit::ElectricCurrent },
    { "MegaAmpere"            , "MA"           , 1.0e6                         , Unit::ElectricCurrent },

    // --- Temperature ---
    { "Kelvin"                , "K"            , 1.0                           , Unit::Temperature },
    { "MilliKelvin"           , "mK"           , 0.001                         , Unit::Temperature },
    { "MicroKelvin"           , "\xC2\xB5K"    , 0.000001                      , Unit::Temperature },

    // --- Amount of Substance ---
    { "MilliMole"             , "mmol"         , 0.001                         , Unit::AmountOfSubstance },
    { "Mole"                  , "mol"          , 1.0                           , Unit::AmountOfSubstance },

    // --- Luminous Intensity ---
    { "Candela"               , "cd"           , 1.0                           , Unit::LuminousIntensity },

    // --- Velocity ---
    { "MilliMetrePerSecond"   , "mm/s"         , 1.0                           , Unit::Velocity },
    { "MetrePerSecond"        , "m/s"          , 1e3                           , Unit::Velocity },
    { "MilePerHour"           , "mph"          , mi / 3600                     , Unit::Velocity },
    { "KMH"                   , "km/h"         , 1.0e6 / 3600                  , Unit::Velocity },
    { "MPH"                   , "mi/h"         , mi / 3600                     , Unit::Velocity },
    { "InchPerMinute"         , "in/min"       , in / 60                       , Unit::Velocity },
    { "MilliMetrePerMinute"   , "mm/min"       , 1.0 / 60                      , Unit::Velocity },

    // --- Acceleration ---
    { "MetrePerSecondSquared" , "m/s^2"        , 1e3                           , Unit::Acceleration },
    { "InchPerMinuteSquared"  , "in/min^2"     , in / 3600                     , Unit::Acceleration },

    // --- Force ---
    { "PoundForce"            , "lbf"          , 1000 * lbf                    , Unit::Force },
    { "Newton"                , "N"            , 1000.0                        , Unit::Force },
    { "MilliNewton"           , "mN"           , 1.0                           , Unit::Force },
    { "KiloNewton"            , "kN"           , 1e+6                          , Unit::Force },
    { "MegaNewton"            , "MN"           , 1e+9                          , Unit::Force },

    // --- Stiffness (Force/Length) ---
    { "NewtonPerMeter"        , "N/m"          , 1.00                          , Unit::Stiffness },
    { "MilliNewtonPerMeter"   , "mN/m"         , 1e-3                          , Unit::Stiffness },
    { "KiloNewtonPerMeter"    , "kN/m"         , 1e3                           , Unit::Stiffness },
    { "MegaNewtonPerMeter"    , "MN/m"         , 1e6                           , Unit::Stiffness },
    { "PoundForcePerInch"     , "lbf/in"       , lbf / in * 1000               , Unit::Stiffness },

    // --- Pressure ---
    { "Pascal"                , "Pa"           , 0.001                         , Unit::Pressure },
    { "KiloPascal"            , "kPa"          , 1.00                          , Unit::Pressure },
    { "MegaPascal"            , "MPa"          , 1000.0                        , Unit::Pressure },
    { "GigaPascal"            , "GPa"          , 1e+6                          , Unit::Pressure },
    { "Bar"                   , "bar"          , 100.0                         , Unit::Pressure },
    { "MilliBar"              , "mbar"         , 0.1                           , Unit::Pressure },
    { "Torr"                  , "Torr"         , 101.325 / 760.0               , Unit::Pressure },
    { "mTorr"                 , "mTorr"        , 101.325 / 760.0 / 1e3         , Unit::Pressure },
    { "yTorr"                 , "\xC2\xB5Torr" , 101.325 / 760.0 / 1e6         , Unit::Pressure },
    { "PSI"                   , "psi"          , psi                           , Unit::Pressure },
    { "KSI"                   , "ksi"          , psi * 1000                    , Unit::Pressure },
    { "MPSI"                  , "Mpsi"         , psi * 1000000                 , Unit::Pressure },

    // --- Density ---
    { "KgPerCubicMetre"       , "kg/m^3"       , 1e-9                          , Unit::Density },
    { "KgPerCubicCentiMetre"  , "kg/cm^3"      , 1e-3                          , Unit::Density },
    { "KgPerCubicMilliMetre"  , "kg/mm^3"      , 1.0                           , Unit::Density },

    // --- Power ---
    { "Watt"                  , "W"            , 1e+6                          , Unit::Power },
    { "MilliWatt"             , "mW"           , 1e+3                          , Unit::Power },
    { "KiloWatt"              , "kW"           , 1e+9                          , Unit::Power },
    { "VoltAmpere"            , "VA"           , 1e+6                          , Unit::Power },

    // --- Electric Potential ---
    { "Volt"                  , "V"            , 1e+6                          , Unit::ElectricPotential },
    { "MilliVolt"             , "mV"           , 1e+3                          , Unit::ElectricPotential },
    { "KiloVolt"              , "kV"           , 1e+9                          , Unit::ElectricPotential },

    // --- Electrical Conductance ---
    { "MegaSiemens"           , "MS"           , 1.0                           , Unit::ElectricalConductance },
    { "KiloSiemens"           , "kS"           , 1e-3                          , Unit::ElectricalConductance },
    { "Siemens"               , "S"            , 1e-6                          , Unit::ElectricalConductance },
    { "MilliSiemens"          , "mS"           , 1e-9                          , Unit::ElectricalConductance },
    { "MicroSiemens"          , "\xC2\xB5S"    , 1e-12                         , Unit::ElectricalConductance },

    // --- Electrical Resistance ---
    { "Ohm"                   , "Ohm"          , 1e+6                          , Unit::ElectricalResistance },
    { "KiloOhm"               , "kOhm"         , 1e+9                          , Unit::ElectricalResistance },
    { "MegaOhm"               , "MOhm"         , 1e+12                         , Unit::ElectricalResistance },

    // --- Electrical Conductivity ---
    { "MilliSiemensPerMetre"  , "mS/m"         , 1e-12                         , Unit::ElectricalConductivity },
    { "SiemensPerMetre"       , "S/m"          , 1e-9                          , Unit::ElectricalConductivity },
    { "KiloSiemensPerMetre"   , "kS/m"         , 1e-6                          , Unit::ElectricalConductivity },
    { "MegaSiemensPerMetre"   , "MS/m"         , 1e-3                          , Unit::ElectricalConductivity },

    // --- Electric Charge ---
    { "Coulomb"               , "C"            , 1.0                           , Unit::ElectricCharge },

    // --- Surface Charge Density ---
    { "CoulombPerSqMetre"     , "C/m^2"        , 1e-6                          , Unit::SurfaceChargeDensity },
    { "CoulombPerSqCentiMetre", "C/cm^2"       , 1e-2                          , Unit::SurfaceChargeDensity },
    { "CoulombPerSqMilliMetre", "C/mm^2"       , 1.0                           , Unit::SurfaceChargeDensity },

    // --- Volume Charge Density ---
    { "CoulombPerCubicMetre"  , "C/m^3"        , 1e-9                          , Unit::VolumeChargeDensity },
    { "CoulombPerCubicCm"     , "C/cm^3"       , 1e-3                          , Unit::VolumeChargeDensity },
    { "CoulombPerCubicMm"     , "C/mm^3"       , 1.0                           , Unit::VolumeChargeDensity },

    // --- Current Density ---
    { "AmperePerSqMetre"      , "A/m^2"        , 1e-6                          , Unit::CurrentDensity },
    { "AmperePerSqCentiMetre" , "A/cm^2"       , 1e-2                          , Unit::CurrentDensity },
    { "AmperePerSqMilliMetre" , "A/mm^2"       , 1.0                           , Unit::CurrentDensity },

    // --- Magnetic Flux Density ---
    { "Tesla"                 , "T"            , 1.0                           , Unit::MagneticFluxDensity },
    { "MilliTesla"            , "mT"           , 1e-3                          , Unit::MagneticFluxDensity },
    { "Gauss"                 , "G"            , 1e-4                          , Unit::MagneticFluxDensity },

    // --- Magnetic Field Strength ---
    { "AmperePerMetre"        , "A/m"          , 1e-3                          , Unit::MagneticFieldStrength },

    // --- Magnetic Flux ---
    { "Weber"                 , "Wb"           , 1e6                           , Unit::MagneticFlux },

    // --- Electrical Capacitance ---
    { "Farad"                 , "F"            , 1e-6                          , Unit::ElectricalCapacitance },
    { "MilliFarad"            , "mF"           , 1e-9                          , Unit::ElectricalCapacitance },
    { "MicroFarad"            , "\xC2\xB5""F"  , 1e-12                         , Unit::ElectricalCapacitance },
    { "NanoFarad"             , "nF"           , 1e-15                         , Unit::ElectricalCapacitance },
    { "PicoFarad"             , "pF"           , 1e-18                         , Unit::ElectricalCapacitance },

    // --- Vacuum Permittivity ---
    { "FaradPerMetre"         , "F/m"          , 1e-9                          , Unit::VacuumPermittivity },

    // --- Electrical Inductance ---
    { "Henry"                 , "H"            , 1e+6                          , Unit::ElectricalInductance },
    { "MilliHenry"            , "mH"           , 1e+3                          , Unit::ElectricalInductance },
    { "MicroHenry"            , "\xC2\xB5H"    , 1.0                           , Unit::ElectricalInductance },
    { "NanoHenry"             , "nH"           , 1e-3                          , Unit::ElectricalInductance },

    // --- Work/Energy ---
    { "Joule"                 , "J"            , 1e+6                          , Unit::Work },
    { "MilliJoule"            , "mJ"           , 1e+3                          , Unit::Work },
    { "KiloJoule"             , "kJ"           , 1e+9                          , Unit::Work },
    { "VoltAmpereSecond"      , "VAs"          , 1e+6                          , Unit::Work },
    { "WattSecond"            , "Ws"           , 1e+6                          , Unit::Work },
    { "KiloWattHour"          , "kWh"          , 3.6e+12                       , Unit::Work },
    { "ElectronVolt"          , "eV"           , 1.602176634e-13               , Unit::Work },
    { "KiloElectronVolt"      , "keV"          , 1.602176634e-10               , Unit::Work },
    { "MegaElectronVolt"      , "MeV"          , 1.602176634e-7                , Unit::Work },
    { "Calorie"               , "cal"          , 4.1868e+6                     , Unit::Work },
    { "KiloCalorie"           , "kcal"         , 4.1868e+9                     , Unit::Work },

    // --- Moment (same dimension as Work) ---
    { "NewtonMeter"           , "Nm"           , 1e+6                          , Unit::Moment },

    // --- Specific Energy ---
    { "SqMetrePerSqSecond"    , "m^2/s^2"      , 1e6                           , Unit::SpecificEnergy },

    // --- Heat Flux ---
    { "WattPerSqMetre"        , "W/m^2"        , 1.0                           , Unit::HeatFlux },

    // --- Thermal Conductivity ---
    { "WattPerMetreKelvin"    , "W/m/K"        , 1e3                           , Unit::ThermalConductivity },
    { "WattPerMmKelvin"       , "W/mm/K"       , 1.0                           , Unit::ThermalConductivity },

    // --- Thermal Expansion Coefficient ---
    { "UmPerMPerKelvin"       , "\xC2\xB5m/m/K", 1e-3                          , Unit::ThermalExpansionCoefficient },
    { "MmPerMmPerKelvin"      , "mm/mm/K"      , 1.0                           , Unit::ThermalExpansionCoefficient },

    // --- Volumetric Thermal Expansion Coefficient ---
    { "CubicMmPerCubicMPerK"  , "mm^3/m^3/K"   , 1e-9                          , Unit::VolumetricThermalExpansionCoefficient },
    { "CubicMPerCubicMPerK"   , "m^3/m^3/K"    , 1.0                           , Unit::VolumetricThermalExpansionCoefficient },

    // --- Specific Heat ---
    { "JoulePerKgKelvin"      , "J/kg/K"       , 1e6                           , Unit::SpecificHeat },

    // --- Thermal Transfer Coefficient ---
    { "WattPerSqMetreKelvin"  , "W/m^2/K"      , 1.0                           , Unit::ThermalTransferCoefficient },

    // --- Dynamic Viscosity ---
    { "PascalSecond"          , "Pa*s"         , 1e-3                          , Unit::DynamicViscosity },

    // --- Kinematic Viscosity ---
    { "SqMmPerSecond"         , "mm^2/s"       , 1.0                           , Unit::KinematicViscosity },
    { "SqMetrePerSecond"      , "m^2/s"        , 1e6                           , Unit::KinematicViscosity },

    // --- Volume Flow Rate ---
    { "CubicMmPerSecond"      , "mm^3/s"       , 1.0                           , Unit::VolumeFlowRate },
    { "MlPerSecond"           , "ml/s"         , 1e3                           , Unit::VolumeFlowRate },
    { "LitrePerSecond"        , "l/s"          , 1e6                           , Unit::VolumeFlowRate },
    { "CubicMetrePerSecond"   , "m^3/s"        , 1e9                           , Unit::VolumeFlowRate },

    // --- Dissipation Rate ---
    { "WattPerKg"             , "W/kg"         , 1e6                           , Unit::DissipationRate },

    // --- Inverse Length ---
    { "PerMetre"              , "1/m"          , 1e-3                          , Unit::InverseLength },
    { "PerKiloMetre"          , "1/km"         , 1e-6                          , Unit::InverseLength },
    { "PerMilliMetre"         , "1/mm"         , 1.0                           , Unit::InverseLength },
    { "PerMicroMetre"         , "1/\xC2\xB5m"  , 1e3                           , Unit::InverseLength },
    { "PerNanoMetre"          , "1/nm"         , 1e6                           , Unit::InverseLength },

    // --- Inverse Area ---
    { "PerSqMetre"            , "1/m^2"        , 1e-6                          , Unit::InverseArea },
    { "PerSqKiloMetre"        , "1/km^2"       , 1e-12                         , Unit::InverseArea },
    { "PerSqCentiMetre"       , "1/cm^2"       , 1e-2                          , Unit::InverseArea },
    { "PerSqMilliMetre"       , "1/mm^2"       , 1.0                           , Unit::InverseArea },

    // --- Inverse Volume ---
    { "PerCubicMetre"         , "1/m^3"        , 1e-9                          , Unit::InverseVolume },
    { "PerLitre"              , "1/l"          , 1e-6                          , Unit::InverseVolume },
    { "PerMilliLitre"         , "1/ml"         , 1e-3                          , Unit::InverseVolume },
    { "PerCubicMilliMetre"    , "1/mm^3"       , 1.0                           , Unit::InverseVolume },

    // --- Stiffness Density ---
    { "PaPerMetre"            , "Pa/m"         , 1e-6                          , Unit::StiffnessDensity },
    { "KPaPerMetre"           , "kPa/m"        , 1e-3                          , Unit::StiffnessDensity },
    { "MPaPerMetre"           , "MPa/m"        , 1.0                           , Unit::StiffnessDensity },
    { "GPaPerMetre"           , "GPa/m"        , 1e3                           , Unit::StiffnessDensity },

    // --- Angle ---
    { "AngMinute"             , "'"            , 1.0 / 60.0                    , Unit::Angle },
    { "AngSecond"             , "\""           , 1.0 / 3600.0                  , Unit::Angle },
    { "Degree"                , "\xC2\xB0"     , 1.0                           , Unit::Angle },
    { "Radian"                , "rad"          , 180.0 / 3.14159265358979323846, Unit::Angle },
    { "Gon"                   , "gon"          , 360.0 / 400.0                 , Unit::Angle },
});  // clang-format on

inline std::vector<QuantitySpec const*> findByUnit(const Unit& unit)
{
    std::vector<QuantitySpec const*> result;
    for (const auto& spec : specs) {
        if (spec.unit == unit) {
            result.push_back(&spec);
        }
    }
    return result;
}

inline std::span<const QuantitySpec> all()
{
    return specs;
}

inline const QuantitySpec* findByName(std::string_view name)
{
    auto it = std::ranges::find(specs, name, &QuantitySpec::name);
    return it != specs.end() ? &(*it) : nullptr;
}

}  // namespace QuantitySpecsData
}  // namespace Base

#endif  // BASE_QUANTITYSPECSDATA_H
