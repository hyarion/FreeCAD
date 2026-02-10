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

#include <array>
#include <string_view>

#include "Unit.h"
#include "UnitsConvData.h"

namespace Base
{

struct QuantitySpec
{
    std::string_view name;
    std::string_view symbol;
    double value;
    UnitExponents exps;
};

namespace QuantitySpecsData
{
using namespace UnitsConvData;

// UnitExponents order: Length, Mass, Time, ElectricCurrent, Temperature,
//                      AmountOfSubstance, LuminousIntensity, Angle

// clang-format off
constexpr auto specs = std::to_array<QuantitySpec>({
    //  Name                     Symbol    Value                           Exponents (L,M,T,I,Th,N,J,A)
    // --- Length ---
    { "NanoMetre"             , "nm"    , 1.0e-6                        , { 1 } },
    { "MicroMetre"            , "um"    , 1.0e-3                        , { 1 } },
    { "MilliMetre"            , "mm"    , 1.0                           , { 1 } },
    { "CentiMetre"            , "cm"    , 10.0                          , { 1 } },
    { "DeciMetre"             , "dm"    , 100.0                         , { 1 } },
    { "Metre"                 , "m"     , 1.0e3                         , { 1 } },
    { "KiloMetre"             , "km"    , 1.0e6                         , { 1 } },
    { "Inch"                  , "in"    , in                            , { 1 } },
    { "Foot"                  , "ft"    , ft                            , { 1 } },
    { "Thou"                  , "thou"  , in / 1000                     , { 1 } },
    { "Yard"                  , "yd"    , yd                            , { 1 } },
    { "Mile"                  , "mi"    , mi                            , { 1 } },

    // --- Volume ---
    { "MilliLiter"            , "ml"    , 1000.0                        , { 3 } },
    { "Liter"                 , "l"     , 1.0e6                         , { 3 } },

    // --- Frequency ---
    { "Hertz"                 , "Hz"    , 1.0                           , { 0, 0, -1 } },
    { "KiloHertz"             , "kHz"   , 1.0e3                         , { 0, 0, -1 } },
    { "MegaHertz"             , "MHz"   , 1.0e6                         , { 0, 0, -1 } },
    { "GigaHertz"             , "GHz"   , 1.0e9                         , { 0, 0, -1 } },
    { "TeraHertz"             , "THz"   , 1.0e12                        , { 0, 0, -1 } },

    // --- Mass ---
    { "MicroGram"             , "ug"    , 1.0e-9                        , { 0, 1 } },
    { "MilliGram"             , "mg"    , 1.0e-6                        , { 0, 1 } },
    { "Gram"                  , "g"     , 1.0e-3                        , { 0, 1 } },
    { "KiloGram"              , "kg"    , 1.0                           , { 0, 1 } },
    { "Ton"                   , "t"     , 1.0e3                         , { 0, 1 } },
    { "Pound"                 , "lb"    , lb                            , { 0, 1 } },
    { "Ounce"                 , "oz"    , lb / 16                       , { 0, 1 } },
    { "Stone"                 , "st"    , lb * 14                       , { 0, 1 } },
    { "Hundredweights"        , "cwt"   , lb * 112                      , { 0, 1 } },

    // --- Time ---
    { "Second"                , "s"     , 1.0                           , { 0, 0, 1 } },
    { "Minute"                , "min"   , 60.0                          , { 0, 0, 1 } },
    { "Hour"                  , "h"     , 3600.0                        , { 0, 0, 1 } },

    // --- Electric Current ---
    { "Ampere"                , "A"     , 1.0                           , { 0, 0, 0, 1 } },
    { "MilliAmpere"           , "mA"    , 0.001                         , { 0, 0, 0, 1 } },
    { "KiloAmpere"            , "kA"    , 1000.0                        , { 0, 0, 0, 1 } },
    { "MegaAmpere"            , "MA"    , 1.0e6                         , { 0, 0, 0, 1 } },

    // --- Temperature ---
    { "Kelvin"                , "K"     , 1.0                           , { 0, 0, 0, 0, 1 } },
    { "MilliKelvin"           , "mK"    , 0.001                         , { 0, 0, 0, 0, 1 } },
    { "MicroKelvin"           , "uK"    , 0.000001                      , { 0, 0, 0, 0, 1 } },

    // --- Amount of Substance ---
    { "MilliMole"             , "mmol"  , 0.001                         , { 0, 0, 0, 0, 0, 1 } },
    { "Mole"                  , "mol"   , 1.0                           , { 0, 0, 0, 0, 0, 1 } },

    // --- Luminous Intensity ---
    { "Candela"               , "cd"    , 1.0                           , { 0, 0, 0, 0, 0, 0, 1 } },

    // --- Area ---
    { "SquareFoot"            , "sqft"  , ft * ft                       , { 2 } },

    // --- Volume (imperial) ---
    { "CubicFoot"             , "cft"   , ft * ft * ft                  , { 3 } },

    // --- Velocity ---
    { "MilePerHour"           , "mph"   , mi / 3600                     , { 1, 0, -1 } },
    { "KMH"                   , "km/h"  , 1.0e6 / 3600                  , { 1, 0, -1 } },
    { "MPH"                   , "mi/h"  , mi / 3600                     , { 1, 0, -1 } },

    // --- Force ---
    { "PoundForce"            , "lbf"   , 1000 * lbf                    , { 1, 1, -2 } },
    { "Newton"                , "N"     , 1000.0                        , { 1, 1, -2 } },
    { "MilliNewton"           , "mN"    , 1.0                           , { 1, 1, -2 } },
    { "KiloNewton"            , "kN"    , 1e+6                          , { 1, 1, -2 } },
    { "MegaNewton"            , "MN"    , 1e+9                          , { 1, 1, -2 } },

    // --- Stiffness (Force/Length) ---
    { "NewtonPerMeter"        , "N/m"   , 1.00                          , { 0, 1, -2 } },
    { "MilliNewtonPerMeter"   , "mN/m"  , 1e-3                          , { 0, 1, -2 } },
    { "KiloNewtonPerMeter"    , "kN/m"  , 1e3                           , { 0, 1, -2 } },
    { "MegaNewtonPerMeter"    , "MN/m"  , 1e6                           , { 0, 1, -2 } },

    // --- Pressure ---
    { "Pascal"                , "Pa"    , 0.001                         , { -1, 1, -2 } },
    { "KiloPascal"            , "kPa"   , 1.00                          , { -1, 1, -2 } },
    { "MegaPascal"            , "MPa"   , 1000.0                        , { -1, 1, -2 } },
    { "GigaPascal"            , "GPa"   , 1e+6                          , { -1, 1, -2 } },
    { "Bar"                   , "bar"   , 100.0                         , { -1, 1, -2 } },
    { "MilliBar"              , "mbar"  , 0.1                           , { -1, 1, -2 } },
    { "Torr"                  , "Torr"  , 101.325 / 760.0               , { -1, 1, -2 } },
    { "mTorr"                 , "mTorr" , 101.325 / 760.0 / 1e3         , { -1, 1, -2 } },
    { "yTorr"                 , "uTorr" , 101.325 / 760.0 / 1e6         , { -1, 1, -2 } },
    { "PSI"                   , "psi"   , psi                           , { -1, 1, -2 } },
    { "KSI"                   , "ksi"   , psi * 1000                    , { -1, 1, -2 } },
    { "MPSI"                  , "Mpsi"  , psi * 1000000                 , { -1, 1, -2 } },

    // --- Power ---
    { "Watt"                  , "W"     , 1e+6                          , { 2, 1, -3 } },
    { "MilliWatt"             , "mW"    , 1e+3                          , { 2, 1, -3 } },
    { "KiloWatt"              , "kW"    , 1e+9                          , { 2, 1, -3 } },
    { "VoltAmpere"            , "VA"    , 1e+6                          , { 2, 1, -3 } },

    // --- Electric Potential ---
    { "Volt"                  , "V"     , 1e+6                          , { 2, 1, -3, -1 } },
    { "MilliVolt"             , "mV"    , 1e+3                          , { 2, 1, -3, -1 } },
    { "KiloVolt"              , "kV"    , 1e+9                          , { 2, 1, -3, -1 } },

    // --- Electrical Conductance ---
    { "MegaSiemens"           , "MS"    , 1.0                           , { -2, -1, 3, 2 } },
    { "KiloSiemens"           , "kS"    , 1e-3                          , { -2, -1, 3, 2 } },
    { "Siemens"               , "S"     , 1e-6                          , { -2, -1, 3, 2 } },
    { "MilliSiemens"          , "mS"    , 1e-9                          , { -2, -1, 3, 2 } },
    { "MicroSiemens"          , "uS"    , 1e-12                         , { -2, -1, 3, 2 } },

    // --- Electrical Resistance ---
    { "Ohm"                   , "Ohm"   , 1e+6                          , { 2, 1, -3, -2 } },
    { "KiloOhm"               , "kOhm"  , 1e+9                          , { 2, 1, -3, -2 } },
    { "MegaOhm"               , "MOhm"  , 1e+12                         , { 2, 1, -3, -2 } },

    // --- Electric Charge ---
    { "Coulomb"               , "C"     , 1.0                           , { 0, 0, 1, 1 } },

    // --- Magnetic Flux Density ---
    { "Tesla"                 , "T"     , 1.0                           , { 0, 1, -2, -1 } },
    { "MilliTesla"            , "mT"    , 1e-3                          , { 0, 1, -2, -1 } },
    { "Gauss"                 , "G"     , 1e-4                          , { 0, 1, -2, -1 } },

    // --- Magnetic Flux ---
    { "Weber"                 , "Wb"    , 1e6                           , { 2, 1, -2, -1 } },

    // --- Electrical Capacitance ---
    { "Farad"                 , "F"     , 1e-6                          , { -2, -1, 4, 2 } },
    { "MilliFarad"            , "mF"    , 1e-9                          , { -2, -1, 4, 2 } },
    { "MicroFarad"            , "uF"    , 1e-12                         , { -2, -1, 4, 2 } },
    { "NanoFarad"             , "nF"    , 1e-15                         , { -2, -1, 4, 2 } },
    { "PicoFarad"             , "pF"    , 1e-18                         , { -2, -1, 4, 2 } },

    // --- Electrical Inductance ---
    { "Henry"                 , "H"     , 1e+6                          , { 2, 1, -2, -2 } },
    { "MilliHenry"            , "mH"    , 1e+3                          , { 2, 1, -2, -2 } },
    { "MicroHenry"            , "uH"    , 1.0                           , { 2, 1, -2, -2 } },
    { "NanoHenry"             , "nH"    , 1e-3                          , { 2, 1, -2, -2 } },

    // --- Work/Energy ---
    { "Joule"                 , "J"     , 1e+6                          , { 2, 1, -2 } },
    { "MilliJoule"            , "mJ"    , 1e+3                          , { 2, 1, -2 } },
    { "KiloJoule"             , "kJ"    , 1e+9                          , { 2, 1, -2 } },
    { "VoltAmpereSecond"      , "VAs"   , 1e+6                          , { 2, 1, -2 } },
    { "WattSecond"            , "Ws"    , 1e+6                          , { 2, 1, -2 } },
    { "KiloWattHour"          , "kWh"   , 3.6e+12                       , { 2, 1, -2 } },
    { "ElectronVolt"          , "eV"    , 1.602176634e-13               , { 2, 1, -2 } },
    { "KiloElectronVolt"      , "keV"   , 1.602176634e-10               , { 2, 1, -2 } },
    { "MegaElectronVolt"      , "MeV"   , 1.602176634e-7                , { 2, 1, -2 } },
    { "Calorie"               , "cal"   , 4.1868e+6                     , { 2, 1, -2 } },
    { "KiloCalorie"           , "kcal"  , 4.1868e+9                     , { 2, 1, -2 } },

    // --- Moment (same dimension as Work but different Unit type) ---
    { "NewtonMeter"           , "Nm"    , 1e+6                          , { 2, 1, -2 } },

    // --- Angle ---
    { "AngMinute"             , "'"     , 1.0 / 60.0                    , { 0, 0, 0, 0, 0, 0, 0, 1 } },
    { "AngSecond"             , "\""    , 1.0 / 3600.0                  , { 0, 0, 0, 0, 0, 0, 0, 1 } },
    { "Degree"                , "deg"   , 1.0                           , { 0, 0, 0, 0, 0, 0, 0, 1 } },
    { "Radian"                , "rad"   , 180.0 / 3.14159265358979323846, { 0, 0, 0, 0, 0, 0, 0, 1 } },
    { "Gon"                   , "gon"   , 360.0 / 400.0                 , { 0, 0, 0, 0, 0, 0, 0, 1 } },
});  // clang-format on

}  // namespace QuantitySpecsData
}  // namespace Base

#endif  // BASE_QUANTITYSPECSDATA_H
