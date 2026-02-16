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
#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

#include "Unit.h"
#include "UnitSystem.h"
#include "UnitsConvData.h"

namespace Base
{

struct QuantitySpec
{
    std::string_view name;
    std::string_view symbol;
    double value;
    Unit unit;
    UnitSystem unitSystem;
};

namespace QuantitySpecsData
{
using namespace UnitsConvData;

// clang-format off
constexpr auto specs = std::to_array<QuantitySpec>({
    //  Name                     Symbol          Value                           Unit                                          UnitSystem
    // --- Length ---
    { "NanoMetre"             , "nm"           , 1.0e-6                        , Unit::Length                                 , UnitSystem::Metric   },
    { "MicroMetre"            , "\xC2\xB5m"    , 1.0e-3                        , Unit::Length                                 , UnitSystem::Metric   },
    { "MilliMetre"            , "mm"           , 1.0                           , Unit::Length                                 , UnitSystem::Metric   },
    { "CentiMetre"            , "cm"           , 10.0                          , Unit::Length                                 , UnitSystem::Metric   },
    { "DeciMetre"             , "dm"           , 100.0                         , Unit::Length                                 , UnitSystem::Metric   },
    { "Metre"                 , "m"            , 1.0e3                         , Unit::Length                                 , UnitSystem::Metric   },
    { "KiloMetre"             , "km"           , 1.0e6                         , Unit::Length                                 , UnitSystem::Metric   },
    { "Thou"                  , "thou"         , in / 1000                     , Unit::Length                                 , UnitSystem::Imperial },
    { "Inch"                  , "in"           , in                            , Unit::Length                                 , UnitSystem::Imperial },
    { "InchMark"              , "\""           , in                            , Unit::Length                                 , UnitSystem::Imperial },
    { "Foot"                  , "ft"           , ft                            , Unit::Length                                 , UnitSystem::Imperial },
    { "FootMark"              , "'"            , ft                            , Unit::Length                                 , UnitSystem::Imperial },
    { "Yard"                  , "yd"           , yd                            , Unit::Length                                 , UnitSystem::Imperial },
    { "Mile"                  , "mi"           , mi                            , Unit::Length                                 , UnitSystem::Imperial },

    // --- Area ---
    { "SquareMilliMetre"      , "mm^2"         , 1.0                           , Unit::Area                                  , UnitSystem::Metric   },
    { "SquareCentiMetre"      , "cm^2"         , 1e2                           , Unit::Area                                  , UnitSystem::Metric   },
    { "SquareMetre"           , "m^2"          , 1e6                           , Unit::Area                                  , UnitSystem::Metric   },
    { "SquareKiloMetre"       , "km^2"         , 1e12                          , Unit::Area                                  , UnitSystem::Metric   },
    { "SquareInch"            , "in^2"         , in * in                       , Unit::Area                                  , UnitSystem::Imperial },
    { "SquareFoot"            , "sqft"         , ft * ft                       , Unit::Area                                  , UnitSystem::Imperial },
    { "SquareFoot2"           , "ft^2"         , ft * ft                       , Unit::Area                                  , UnitSystem::Imperial },

    // --- Volume ---
    { "CubicMilliMetre"       , "mm^3"         , 1.0                           , Unit::Volume                                , UnitSystem::Metric   },
    { "MilliLiter"            , "ml"           , 1000.0                        , Unit::Volume                                , UnitSystem::Metric   },
    { "Liter"                 , "l"            , 1.0e6                         , Unit::Volume                                , UnitSystem::Metric   },
    { "CubicMetre"            , "m^3"          , 1e9                           , Unit::Volume                                , UnitSystem::Metric   },
    { "CubicInch"             , "in^3"         , in * in * in                  , Unit::Volume                                , UnitSystem::Imperial },
    { "CubicFoot"             , "cft"          , ft * ft * ft                  , Unit::Volume                                , UnitSystem::Imperial },
    { "CubicFoot2"            , "ft^3"         , ft * ft * ft                  , Unit::Volume                                , UnitSystem::Imperial },

    // --- Frequency ---
    { "Hertz"                 , "Hz"           , 1.0                           , Unit::Frequency                             , UnitSystem::Metric   },
    { "KiloHertz"             , "kHz"          , 1.0e3                         , Unit::Frequency                             , UnitSystem::Metric   },
    { "MegaHertz"             , "MHz"          , 1.0e6                         , Unit::Frequency                             , UnitSystem::Metric   },
    { "GigaHertz"             , "GHz"          , 1.0e9                         , Unit::Frequency                             , UnitSystem::Metric   },
    { "TeraHertz"             , "THz"          , 1.0e12                        , Unit::Frequency                             , UnitSystem::Metric   },

    // --- Mass ---
    { "MicroGram"             , "\xC2\xB5g"    , 1.0e-9                        , Unit::Mass                                  , UnitSystem::Metric   },
    { "MilliGram"             , "mg"           , 1.0e-6                        , Unit::Mass                                  , UnitSystem::Metric   },
    { "Gram"                  , "g"            , 1.0e-3                        , Unit::Mass                                  , UnitSystem::Metric   },
    { "KiloGram"              , "kg"           , 1.0                           , Unit::Mass                                  , UnitSystem::Metric   },
    { "Ton"                   , "t"            , 1.0e3                         , Unit::Mass                                  , UnitSystem::Metric   },
    { "Ounce"                 , "oz"           , lb / 16                       , Unit::Mass                                  , UnitSystem::Imperial },
    { "Pound"                 , "lb"           , lb                            , Unit::Mass                                  , UnitSystem::Imperial },
    { "Stone"                 , "st"           , lb * 14                       , Unit::Mass                                  , UnitSystem::Imperial },
    { "Hundredweights"        , "cwt"          , lb * 112                      , Unit::Mass                                  , UnitSystem::Imperial },

    // --- Time ---
    { "Second"                , "s"            , 1.0                           , Unit::TimeSpan                              , UnitSystem::Metric   },
    { "Minute"                , "min"          , 60.0                          , Unit::TimeSpan                              , UnitSystem::Metric   },
    { "Hour"                  , "h"            , 3600.0                        , Unit::TimeSpan                              , UnitSystem::Metric   },

    // --- Electric Current ---
    { "MilliAmpere"           , "mA"           , 0.001                         , Unit::ElectricCurrent                       , UnitSystem::Metric   },
    { "Ampere"                , "A"            , 1.0                           , Unit::ElectricCurrent                       , UnitSystem::Metric   },
    { "KiloAmpere"            , "kA"           , 1000.0                        , Unit::ElectricCurrent                       , UnitSystem::Metric   },
    { "MegaAmpere"            , "MA"           , 1.0e6                         , Unit::ElectricCurrent                       , UnitSystem::Metric   },

    // --- Temperature ---
    { "MicroKelvin"           , "\xC2\xB5K"    , 0.000001                      , Unit::Temperature                           , UnitSystem::Metric   },
    { "MilliKelvin"           , "mK"           , 0.001                         , Unit::Temperature                           , UnitSystem::Metric   },
    { "Kelvin"                , "K"            , 1.0                           , Unit::Temperature                           , UnitSystem::Metric   },

    // --- Amount of Substance ---
    { "MilliMole"             , "mmol"         , 0.001                         , Unit::AmountOfSubstance                     , UnitSystem::Metric   },
    { "Mole"                  , "mol"          , 1.0                           , Unit::AmountOfSubstance                     , UnitSystem::Metric   },

    // --- Luminous Intensity ---
    { "Candela"               , "cd"           , 1.0                           , Unit::LuminousIntensity                     , UnitSystem::Metric   },

    // --- Velocity ---
    { "MilliMetrePerMinute"   , "mm/min"       , 1.0 / 60                      , Unit::Velocity                              , UnitSystem::Metric   },
    { "MilliMetrePerSecond"   , "mm/s"         , 1.0                           , Unit::Velocity                              , UnitSystem::Metric   },
    { "KMH"                   , "km/h"         , 1.0e6 / 3600                  , Unit::Velocity                              , UnitSystem::Metric   },
    { "MetrePerSecond"        , "m/s"          , 1e3                           , Unit::Velocity                              , UnitSystem::Metric   },
    { "InchPerMinute"         , "in/min"       , in / 60                       , Unit::Velocity                              , UnitSystem::Imperial },
    { "MilePerHour"           , "mph"          , mi / 3600                     , Unit::Velocity                              , UnitSystem::Imperial },
    { "MPH"                   , "mi/h"         , mi / 3600                     , Unit::Velocity                              , UnitSystem::Imperial },

    // --- Acceleration ---
    { "MetrePerSecondSquared" , "m/s^2"        , 1e3                           , Unit::Acceleration                          , UnitSystem::Metric   },
    { "InchPerMinuteSquared"  , "in/min^2"     , in / 3600                     , Unit::Acceleration                          , UnitSystem::Imperial },

    // --- Force ---
    { "MilliNewton"           , "mN"           , 1.0                           , Unit::Force                                 , UnitSystem::Metric   },
    { "Newton"                , "N"            , 1000.0                        , Unit::Force                                 , UnitSystem::Metric   },
    { "KiloNewton"            , "kN"           , 1e+6                          , Unit::Force                                 , UnitSystem::Metric   },
    { "MegaNewton"            , "MN"           , 1e+9                          , Unit::Force                                 , UnitSystem::Metric   },
    { "PoundForce"            , "lbf"          , 1000 * lbf                    , Unit::Force                                 , UnitSystem::Imperial },

    // --- Stiffness (Force/Length) ---
    { "MilliNewtonPerMeter"   , "mN/m"         , 1e-3                          , Unit::Stiffness                             , UnitSystem::Metric   },
    { "NewtonPerMeter"        , "N/m"          , 1.00                          , Unit::Stiffness                             , UnitSystem::Metric   },
    { "KiloNewtonPerMeter"    , "kN/m"         , 1e3                           , Unit::Stiffness                             , UnitSystem::Metric   },
    { "MegaNewtonPerMeter"    , "MN/m"         , 1e6                           , Unit::Stiffness                             , UnitSystem::Metric   },
    { "PoundForcePerInch"     , "lbf/in"       , lbf / in * 1000               , Unit::Stiffness                             , UnitSystem::Imperial },

    // --- Pressure ---
    { "yTorr"                 , "\xC2\xB5Torr" , 101.325 / 760.0 / 1e6         , Unit::Pressure                              , UnitSystem::Metric   },
    { "mTorr"                 , "mTorr"        , 101.325 / 760.0 / 1e3         , Unit::Pressure                              , UnitSystem::Metric   },
    { "Pascal"                , "Pa"           , 0.001                         , Unit::Pressure                              , UnitSystem::Metric   },
    { "MilliBar"              , "mbar"         , 0.1                           , Unit::Pressure                              , UnitSystem::Metric   },
    { "Torr"                  , "Torr"         , 101.325 / 760.0               , Unit::Pressure                              , UnitSystem::Metric   },
    { "KiloPascal"            , "kPa"          , 1.00                          , Unit::Pressure                              , UnitSystem::Metric   },
    { "Bar"                   , "bar"          , 100.0                         , Unit::Pressure                              , UnitSystem::Metric   },
    { "MegaPascal"            , "MPa"          , 1000.0                        , Unit::Pressure                              , UnitSystem::Metric   },
    { "GigaPascal"            , "GPa"          , 1e+6                          , Unit::Pressure                              , UnitSystem::Metric   },
    { "PSI"                   , "psi"          , psi                           , Unit::Pressure                              , UnitSystem::Imperial },
    { "KSI"                   , "ksi"          , psi * 1000                    , Unit::Pressure                              , UnitSystem::Imperial },
    { "MPSI"                  , "Mpsi"         , psi * 1000000                 , Unit::Pressure                              , UnitSystem::Imperial },

    // --- Density ---
    { "KgPerCubicMetre"       , "kg/m^3"       , 1e-9                          , Unit::Density                               , UnitSystem::Metric   },
    { "KgPerCubicCentiMetre"  , "kg/cm^3"      , 1e-3                          , Unit::Density                               , UnitSystem::Metric   },
    { "KgPerCubicMilliMetre"  , "kg/mm^3"      , 1.0                           , Unit::Density                               , UnitSystem::Metric   },

    // --- Power ---
    { "MilliWatt"             , "mW"           , 1e+3                          , Unit::Power                                 , UnitSystem::Metric   },
    { "Watt"                  , "W"            , 1e+6                          , Unit::Power                                 , UnitSystem::Metric   },
    { "VoltAmpere"            , "VA"           , 1e+6                          , Unit::Power                                 , UnitSystem::Metric   },
    { "KiloWatt"              , "kW"           , 1e+9                          , Unit::Power                                 , UnitSystem::Metric   },

    // --- Electric Potential ---
    { "MilliVolt"             , "mV"           , 1e+3                          , Unit::ElectricPotential                     , UnitSystem::Metric   },
    { "Volt"                  , "V"            , 1e+6                          , Unit::ElectricPotential                     , UnitSystem::Metric   },
    { "KiloVolt"              , "kV"           , 1e+9                          , Unit::ElectricPotential                     , UnitSystem::Metric   },

    // --- Electrical Conductance ---
    { "MicroSiemens"          , "\xC2\xB5S"    , 1e-12                         , Unit::ElectricalConductance                 , UnitSystem::Metric   },
    { "MilliSiemens"          , "mS"           , 1e-9                          , Unit::ElectricalConductance                 , UnitSystem::Metric   },
    { "Siemens"               , "S"            , 1e-6                          , Unit::ElectricalConductance                 , UnitSystem::Metric   },
    { "KiloSiemens"           , "kS"           , 1e-3                          , Unit::ElectricalConductance                 , UnitSystem::Metric   },
    { "MegaSiemens"           , "MS"           , 1.0                           , Unit::ElectricalConductance                 , UnitSystem::Metric   },

    // --- Electrical Resistance ---
    { "Ohm"                   , "Ohm"          , 1e+6                          , Unit::ElectricalResistance                  , UnitSystem::Metric   },
    { "KiloOhm"               , "kOhm"         , 1e+9                          , Unit::ElectricalResistance                  , UnitSystem::Metric   },
    { "MegaOhm"               , "MOhm"         , 1e+12                         , Unit::ElectricalResistance                  , UnitSystem::Metric   },

    // --- Electrical Conductivity ---
    { "MilliSiemensPerMetre"  , "mS/m"         , 1e-12                         , Unit::ElectricalConductivity                , UnitSystem::Metric   },
    { "SiemensPerMetre"       , "S/m"          , 1e-9                          , Unit::ElectricalConductivity                , UnitSystem::Metric   },
    { "KiloSiemensPerMetre"   , "kS/m"         , 1e-6                          , Unit::ElectricalConductivity                , UnitSystem::Metric   },
    { "MegaSiemensPerMetre"   , "MS/m"         , 1e-3                          , Unit::ElectricalConductivity                , UnitSystem::Metric   },

    // --- Electric Charge ---
    { "Coulomb"               , "C"            , 1.0                           , Unit::ElectricCharge                        , UnitSystem::Metric   },

    // --- Surface Charge Density ---
    { "CoulombPerSqMetre"     , "C/m^2"        , 1e-6                          , Unit::SurfaceChargeDensity                  , UnitSystem::Metric   },
    { "CoulombPerSqCentiMetre", "C/cm^2"       , 1e-2                          , Unit::SurfaceChargeDensity                  , UnitSystem::Metric   },
    { "CoulombPerSqMilliMetre", "C/mm^2"       , 1.0                           , Unit::SurfaceChargeDensity                  , UnitSystem::Metric   },

    // --- Volume Charge Density ---
    { "CoulombPerCubicMetre"  , "C/m^3"        , 1e-9                          , Unit::VolumeChargeDensity                   , UnitSystem::Metric   },
    { "CoulombPerCubicCm"     , "C/cm^3"       , 1e-3                          , Unit::VolumeChargeDensity                   , UnitSystem::Metric   },
    { "CoulombPerCubicMm"     , "C/mm^3"       , 1.0                           , Unit::VolumeChargeDensity                   , UnitSystem::Metric   },

    // --- Current Density ---
    { "AmperePerSqMetre"      , "A/m^2"        , 1e-6                          , Unit::CurrentDensity                        , UnitSystem::Metric   },
    { "AmperePerSqCentiMetre" , "A/cm^2"       , 1e-2                          , Unit::CurrentDensity                        , UnitSystem::Metric   },
    { "AmperePerSqMilliMetre" , "A/mm^2"       , 1.0                           , Unit::CurrentDensity                        , UnitSystem::Metric   },

    // --- Magnetic Flux Density ---
    { "Gauss"                 , "G"            , 1e-4                          , Unit::MagneticFluxDensity                   , UnitSystem::Metric   },
    { "MilliTesla"            , "mT"           , 1e-3                          , Unit::MagneticFluxDensity                   , UnitSystem::Metric   },
    { "Tesla"                 , "T"            , 1.0                           , Unit::MagneticFluxDensity                   , UnitSystem::Metric   },

    // --- Magnetic Field Strength ---
    { "AmperePerMetre"        , "A/m"          , 1e-3                          , Unit::MagneticFieldStrength                 , UnitSystem::Metric   },

    // --- Magnetic Flux ---
    { "Weber"                 , "Wb"           , 1e6                           , Unit::MagneticFlux                          , UnitSystem::Metric   },

    // --- Electromagnetic Potential ---
    { "WeberPerMetre"         , "Wb/m"         , 1e3                           , Unit::ElectromagneticPotential              , UnitSystem::Metric   },

    // --- Electrical Capacitance ---
    { "PicoFarad"             , "pF"           , 1e-18                         , Unit::ElectricalCapacitance                 , UnitSystem::Metric   },
    { "NanoFarad"             , "nF"           , 1e-15                         , Unit::ElectricalCapacitance                 , UnitSystem::Metric   },
    { "MicroFarad"            , "\xC2\xB5""F"  , 1e-12                         , Unit::ElectricalCapacitance                 , UnitSystem::Metric   },
    { "MilliFarad"            , "mF"           , 1e-9                          , Unit::ElectricalCapacitance                 , UnitSystem::Metric   },
    { "Farad"                 , "F"            , 1e-6                          , Unit::ElectricalCapacitance                 , UnitSystem::Metric   },

    // --- Vacuum Permittivity ---
    { "FaradPerMetre"         , "F/m"          , 1e-9                          , Unit::VacuumPermittivity                    , UnitSystem::Metric   },

    // --- Electrical Inductance ---
    { "NanoHenry"             , "nH"           , 1e-3                          , Unit::ElectricalInductance                  , UnitSystem::Metric   },
    { "MicroHenry"            , "\xC2\xB5H"    , 1.0                           , Unit::ElectricalInductance                  , UnitSystem::Metric   },
    { "MilliHenry"            , "mH"           , 1e+3                          , Unit::ElectricalInductance                  , UnitSystem::Metric   },
    { "Henry"                 , "H"            , 1e+6                          , Unit::ElectricalInductance                  , UnitSystem::Metric   },

    // --- Work/Energy ---
    { "ElectronVolt"          , "eV"           , 1.602176634e-13               , Unit::Work                                  , UnitSystem::Metric   },
    { "KiloElectronVolt"      , "keV"          , 1.602176634e-10               , Unit::Work                                  , UnitSystem::Metric   },
    { "MegaElectronVolt"      , "MeV"          , 1.602176634e-7                , Unit::Work                                  , UnitSystem::Metric   },
    { "MilliJoule"            , "mJ"           , 1e+3                          , Unit::Work                                  , UnitSystem::Metric   },
    { "Joule"                 , "J"            , 1e+6                          , Unit::Work                                  , UnitSystem::Metric   },
    { "VoltAmpereSecond"      , "VAs"          , 1e+6                          , Unit::Work                                  , UnitSystem::Metric   },
    { "WattSecond"            , "Ws"           , 1e+6                          , Unit::Work                                  , UnitSystem::Metric   },
    { "Calorie"               , "cal"          , 4.1868e+6                     , Unit::Work                                  , UnitSystem::Metric   },
    { "KiloJoule"             , "kJ"           , 1e+9                          , Unit::Work                                  , UnitSystem::Metric   },
    { "KiloCalorie"           , "kcal"         , 4.1868e+9                     , Unit::Work                                  , UnitSystem::Metric   },
    { "KiloWattHour"          , "kWh"          , 3.6e+12                       , Unit::Work                                  , UnitSystem::Metric   },

    // --- Moment (same dimension as Work) ---
    { "NewtonMeter"           , "Nm"           , 1e+6                          , Unit::Moment                                , UnitSystem::Metric   },
    { "FootPound"             , "ft*lbf"       , ft * lbf * 1000               , Unit::Moment                                , UnitSystem::Imperial },

    // --- Specific Energy ---
    { "SqMetrePerSqSecond"    , "m^2/s^2"      , 1e6                           , Unit::SpecificEnergy                        , UnitSystem::Metric   },

    // --- Heat Flux ---
    { "WattPerSqMetre"        , "W/m^2"        , 1.0                           , Unit::HeatFlux                              , UnitSystem::Metric   },

    // --- Thermal Conductivity ---
    { "WattPerMetreKelvin"    , "W/m/K"        , 1e3                           , Unit::ThermalConductivity                   , UnitSystem::Metric   },
    { "WattPerMmKelvin"       , "W/mm/K"       , 1e6                           , Unit::ThermalConductivity                   , UnitSystem::Metric   },

    // --- Thermal Expansion Coefficient ---
    { "UmPerMPerKelvin"       , "\xC2\xB5m/m/K", 1e-6                          , Unit::ThermalExpansionCoefficient           , UnitSystem::Metric   },
    { "MmPerMmPerKelvin"      , "mm/mm/K"      , 1.0                           , Unit::ThermalExpansionCoefficient           , UnitSystem::Metric   },
    { "MetrePerMetrePerKelvin", "m/m/K"        , 1.0                           , Unit::ThermalExpansionCoefficient           , UnitSystem::Metric   },

    // --- Volumetric Thermal Expansion Coefficient ---
    { "CubicMmPerCubicMPerK"  , "mm^3/m^3/K"   , 1e-9                          , Unit::VolumetricThermalExpansionCoefficient , UnitSystem::Metric   },
    { "CubicMPerCubicMPerK"   , "m^3/m^3/K"    , 1.0                           , Unit::VolumetricThermalExpansionCoefficient , UnitSystem::Metric   },

    // --- Specific Heat ---
    { "JoulePerKgKelvin"      , "J/kg/K"       , 1e6                           , Unit::SpecificHeat                          , UnitSystem::Metric   },

    // --- Thermal Transfer Coefficient ---
    { "WattPerSqMetreKelvin"  , "W/m^2/K"      , 1.0                           , Unit::ThermalTransferCoefficient            , UnitSystem::Metric   },

    // --- Dynamic Viscosity ---
    { "PascalSecond"          , "Pa*s"         , 1e-3                          , Unit::DynamicViscosity                      , UnitSystem::Metric   },

    // --- Kinematic Viscosity ---
    { "SqMmPerSecond"         , "mm^2/s"       , 1.0                           , Unit::KinematicViscosity                    , UnitSystem::Metric   },
    { "SqMetrePerSecond"      , "m^2/s"        , 1e6                           , Unit::KinematicViscosity                    , UnitSystem::Metric   },

    // --- Volume Flow Rate ---
    { "CubicMmPerSecond"      , "mm^3/s"       , 1.0                           , Unit::VolumeFlowRate                        , UnitSystem::Metric   },
    { "MlPerSecond"           , "ml/s"         , 1e3                           , Unit::VolumeFlowRate                        , UnitSystem::Metric   },
    { "LitrePerSecond"        , "l/s"          , 1e6                           , Unit::VolumeFlowRate                        , UnitSystem::Metric   },
    { "CubicMetrePerSecond"   , "m^3/s"        , 1e9                           , Unit::VolumeFlowRate                        , UnitSystem::Metric   },

    // --- Dissipation Rate ---
    { "WattPerKg"             , "W/kg"         , 1e6                           , Unit::DissipationRate                       , UnitSystem::Metric   },

    // --- Inverse Length ---
    { "PerKiloMetre"          , "1/km"         , 1e-6                          , Unit::InverseLength                         , UnitSystem::Metric   },
    { "PerMetre"              , "1/m"          , 1e-3                          , Unit::InverseLength                         , UnitSystem::Metric   },
    { "PerMilliMetre"         , "1/mm"         , 1.0                           , Unit::InverseLength                         , UnitSystem::Metric   },
    { "PerMicroMetre"         , "1/\xC2\xB5m"  , 1e3                           , Unit::InverseLength                         , UnitSystem::Metric   },
    { "PerNanoMetre"          , "1/nm"         , 1e6                           , Unit::InverseLength                         , UnitSystem::Metric   },

    // --- Inverse Area ---
    { "PerSqKiloMetre"        , "1/km^2"       , 1e-12                         , Unit::InverseArea                           , UnitSystem::Metric   },
    { "PerSqMetre"            , "1/m^2"        , 1e-6                          , Unit::InverseArea                           , UnitSystem::Metric   },
    { "PerSqCentiMetre"       , "1/cm^2"       , 1e-2                          , Unit::InverseArea                           , UnitSystem::Metric   },
    { "PerSqMilliMetre"       , "1/mm^2"       , 1.0                           , Unit::InverseArea                           , UnitSystem::Metric   },

    // --- Inverse Volume ---
    { "PerCubicMetre"         , "1/m^3"        , 1e-9                          , Unit::InverseVolume                         , UnitSystem::Metric   },
    { "PerLitre"              , "1/l"          , 1e-6                          , Unit::InverseVolume                         , UnitSystem::Metric   },
    { "PerMilliLitre"         , "1/ml"         , 1e-3                          , Unit::InverseVolume                         , UnitSystem::Metric   },
    { "PerCubicMilliMetre"    , "1/mm^3"       , 1.0                           , Unit::InverseVolume                         , UnitSystem::Metric   },

    // --- Stiffness Density ---
    { "PaPerMetre"            , "Pa/m"         , 1e-6                          , Unit::StiffnessDensity                      , UnitSystem::Metric   },
    { "KPaPerMetre"           , "kPa/m"        , 1e-3                          , Unit::StiffnessDensity                      , UnitSystem::Metric   },
    { "MPaPerMetre"           , "MPa/m"        , 1.0                           , Unit::StiffnessDensity                      , UnitSystem::Metric   },
    { "GPaPerMetre"           , "GPa/m"        , 1e3                           , Unit::StiffnessDensity                      , UnitSystem::Metric   },

    // --- Angle ---
    { "AngSecond"             , "\""           , 1.0 / 3600.0                  , Unit::Angle                                 , UnitSystem::Metric   },
    { "AngMinute"             , "'"            , 1.0 / 60.0                    , Unit::Angle                                 , UnitSystem::Metric   },
    { "Gon"                   , "gon"          , 360.0 / 400.0                 , Unit::Angle                                 , UnitSystem::Metric   },
    { "Degree"                , "\xC2\xB0"     , 1.0                           , Unit::Angle                                 , UnitSystem::Metric   },
    { "Radian"                , "rad"          , 180.0 / 3.14159265358979323846, Unit::Angle                                 , UnitSystem::Metric   },
});  // clang-format on

/// Called during constexpr evaluation when a violation is found.
/// The compiler backtrace shows: "in call to 'specs_ordering_violation(entry, conflicts_with)"
/// giving you the 0-based indices of the two conflicting entries.
constexpr void specs_ordering_violation(bool violated, size_t /*entry*/, size_t /*conflicts_with*/)
{
    if (violated) {
        throw "QuantitySpecs: entry has smaller value than a preceding entry in the same "
              "(unit, unitSystem) group - see indices in the compiler backtrace above";
    }
}

template<size_t N>
constexpr bool specsOrderedByValue(const std::array<QuantitySpec, N>& arr)
{
    for (size_t i = 1; i < arr.size(); ++i) {
        for (size_t j = i; j-- > 0;) {
            if (arr[j].unit != arr[i].unit || arr[j].unit.name() != arr[i].unit.name()
                || arr[j].unitSystem != arr[i].unitSystem) {
                continue;
            }
            if (arr[i].value < arr[j].value) {
                specs_ordering_violation(true, i, j);
            }
            break;
        }
    }
    return true;
}

static_assert(specsOrderedByValue(specs));

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
