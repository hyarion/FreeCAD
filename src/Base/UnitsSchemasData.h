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

#ifndef BASE_UNITSSCHEMASDATA_H
#define BASE_UNITSSCHEMASDATA_H

#include <map>
#include <vector>

#include <QtGlobal>

#include "fmt/format.h"
#include "fmt/ranges.h"

#include "QuantitySpecsData.h"
#include "UnitsConvData.h"
#include "UnitsSchemasSpecs.h"

/**
 * UnitSchemas raw data
 */

namespace Base::UnitsSchemasData
{

constexpr std::size_t defDecimals {2};
constexpr std::size_t defDenominator {8};

using namespace Base::UnitsConvData;

/// Look up a QuantitySpec by name at compile time. Fails to compile if not found.
consteval const QuantitySpec* q(std::string_view name)
{
    for (const auto& s : QuantitySpecsData::specs) {
        if (s.name == name) {
            return &s;
        }
    }
    throw "QuantitySpec not found in registry";
}

// NOLINTBEGIN
// clang-format off
inline const UnitsSchemaSpec s0
{ 6, "MmMin", "mm" , false, false , QT_TRANSLATE_NOOP("UnitsApi", "Metric small parts & CNC (mm, mm/min)"), false,
    {
        { "Length",   {{ 0 , q("MilliMetre")          }}},
        { "Angle",    {{ 0 , q("Degree")              }}},
        { "Velocity", {{ 0 , q("MilliMetrePerMinute") }}}
    }
};

inline const UnitsSchemaSpec s1
{ 9, "MeterDecimal", "m", false, false, QT_TRANSLATE_NOOP("UnitsApi", "Meter decimal (m, m², m³)"), false,
    {
        { "Length",             {{ 0 , q("Metre")            }}},
        { "Area",               {{ 0 , q("SquareMetre")      }}},
        { "Volume",             {{ 0 , q("CubicMetre")       }}},
        { "Power",              {{ 0 , q("Watt")             }}},
        { "ElectricPotential",  {{ 0 , q("Volt")             }}},
        { "HeatFlux",           {{ 0 , q("WattPerSqMetre")   }}},
        { "Velocity",           {{ 0 , q("MetrePerSecond")   }}}
    }
};

inline const UnitsSchemaSpec s2
{ 3, "ImperialDecimal", "in", false, false, QT_TRANSLATE_NOOP("UnitsApi", "Imperial decimal (in, lb)"), false,
    {
        { "Length",       {{ 0 , q("Inch")                    }}},
        { "Angle",        {{ 0 , q("Degree")                  }}},
        { "Area",         {{ 0 , q("SquareInch")              }}},
        { "Volume",       {{ 0 , q("CubicInch")               }}},
        { "Mass",         {{ 0 , q("Pound")                   }}},
        { "Pressure",                {{ 0 , q("PSI")              }}},
        { "Stiffness",               {{ 0 , q("PoundForcePerInch") }}},
        { "Velocity",     {{ 0 , q("InchPerMinute")           }}},
        { "Acceleration", {{ 0 , q("InchPerMinuteSquared")    }}}
    }
};

inline const UnitsSchemaSpec s3
{ 0, "Internal", "mm", false, false, QT_TRANSLATE_NOOP("UnitsApi", "Standard (mm, kg, s, °)"), true,
    {
        { "Length", {
            { 1e-6            , q("MilliMetre")              },
            { 1e-3            , q("NanoMetre")               },
            { 1e-1            , q("MicroMetre")              },
            { 1e4             , q("MilliMetre")              },
            { 1e7             , q("Metre")                   },
            { 1e10            , q("KiloMetre")               },
            { 0               , q("Metre")                   }}
        },
        { "Area", {
            { 1e2             , q("SquareMilliMetre")        },
            { 1e6             , q("SquareCentiMetre")        },
            { 1e12            , q("SquareMetre")             },
            { 0               , q("SquareKiloMetre")         }}
        },
        { "Volume", {
            { 1e3             , q("CubicMilliMetre")         },
            { 1e6             , q("MilliLiter")              },
            { 1e9             , q("Liter")                   },
            { 0               , q("CubicMetre")              }}
        },
        { "Angle", {
            { 0               , q("Degree")                  }}
        },
        { "Mass", {
            { 1e-6            , q("MicroGram")               },
            { 1e-3            , q("MilliGram")               },
            { 1.0             , q("Gram")                    },
            { 1e3             , q("KiloGram")                },
            { 0               , q("Ton")                     }}
        },
        { "Density", {
            { 1e-4            , q("KgPerCubicMetre")         },
            { 1.0             , q("KgPerCubicCentiMetre")    },
            { 0               , q("KgPerCubicMilliMetre")    }}
        },
        { "ThermalConductivity", {
            { 1e6             , q("WattPerMetreKelvin")      },
            { 0               , q("WattPerMmKelvin")         }}
        },
        { "ThermalExpansionCoefficient", {
            { 1e-3            , q("UmPerMPerKelvin")         },
            { 0               , q("MmPerMmPerKelvin")        }}
        },
        { "VolumetricThermalExpansionCoefficient", {
            { 1e-3            , q("CubicMmPerCubicMPerK")    },
            { 0               , q("CubicMPerCubicMPerK")     }}
        },
        { "SpecificHeat", {
            { 0               , q("JoulePerKgKelvin")        }}
        },
        { "ThermalTransferCoefficient", {
            { 0               , q("WattPerSqMetreKelvin")    }}
        },
        { "Pressure", {
            { 10.0            , q("Pascal")                  },
            { 1e4             , q("KiloPascal")              },
            { 1e7             , q("MegaPascal")              },
            { 1e10            , q("GigaPascal")              },
            { 0               , q("Pascal")                  }}
        },
        { "Stiffness", {
            { 1               , q("MilliNewtonPerMeter")     },
            { 1e3             , q("NewtonPerMeter")          },
            { 1e6             , q("KiloNewtonPerMeter")      },
            { 0               , q("MegaNewtonPerMeter")      }}
        },
        { "StiffnessDensity", {
            { 1e-3            , q("PaPerMetre")              },
            { 1               , q("KPaPerMetre")             },
            { 1e3             , q("MPaPerMetre")             },
            { 0               , q("GPaPerMetre")             }}
        },
        { "Force", {
            { 1e3             , q("MilliNewton")             },
            { 1e6             , q("Newton")                  },
            { 1e9             , q("KiloNewton")              },
            { 0               , q("MegaNewton")              }}
        },
        { "Power", {
            { 1e6             , q("MilliWatt")               },
            { 1e9             , q("Watt")                    },
            { 0               , q("KiloWatt")                }}
        },
        { "ElectricPotential", {
            { 1e6             , q("MilliVolt")               },
            { 1e9             , q("Volt")                    },
            { 1e12            , q("KiloVolt")                },
            { 0               , q("Volt")                    }}
        },
        { "Work", {
            { 1.602176634e-10 , q("ElectronVolt")            },
            { 1.602176634e-7  , q("KiloElectronVolt")        },
            { 1.602176634e-4  , q("MegaElectronVolt")        },
            { 1e6             , q("MilliJoule")              },
            { 1e9             , q("Joule")                   },
            { 1e12            , q("KiloJoule")               },
            { 3.6e+15         , q("KiloWattHour")            },
            { 0               , q("Joule")                   }}
        },
        { "Moment", {
            { 0               , q("NewtonMeter")             }}
        },
        { "SpecificEnergy", {
            { 0               , q("SqMetrePerSqSecond")      }}
        },
        { "HeatFlux", {
            { 0               , q("WattPerSqMetre")          }}
        },
        { "ElectricCharge", {
            { 0               , q("Coulomb")                 }}
        },
        { "SurfaceChargeDensity", {
            { 1e-2            , q("CoulombPerSqMetre")       },
            { 1.0             , q("CoulombPerSqCentiMetre")  },
            { 0               , q("CoulombPerSqMilliMetre")  }}
        },
        { "VolumeChargeDensity", {
            { 1e-3            , q("CoulombPerCubicMetre")    },
            { 1.0             , q("CoulombPerCubicCm")       },
            { 0               , q("CoulombPerCubicMm")       }}
        },
        { "CurrentDensity", {
            { 1e-2            , q("AmperePerSqMetre")        },
            { 1.0             , q("AmperePerSqCentiMetre")   },
            { 0               , q("AmperePerSqMilliMetre")   }}
        },
        { "MagneticFluxDensity", {
            { 1.0             , q("MilliTesla")              },
            { 0               , q("Tesla")                   }}
        },
        { "MagneticFieldStrength", {
            { 0               , q("AmperePerMetre")          }}
        },
        { "MagneticFlux", {
            { 0               , q("Weber")                   }}
        },
        { "Magnetization", {
            { 0               , q("AmperePerMetre")          }}
        },
        { "ElectromagneticPotential", {
            { 0               , q("WeberPerMetre")           }}
        },
        { "ElectricalConductance", {
            { 1e-9            , q("MicroSiemens")            },
            { 1e-6            , q("MilliSiemens")            },
            { 0               , q("Siemens")                 }}
        },
        { "ElectricalResistance", {
            { 1e9             , q("Ohm")                     },
            { 1e12            , q("KiloOhm")                 },
            { 0               , q("MegaOhm")                 }}
        },
        { "ElectricalConductivity", {
            { 1e-9            , q("MilliSiemensPerMetre")    },
            { 1e-6            , q("SiemensPerMetre")         },
            { 1e-3            , q("KiloSiemensPerMetre")     },
            { 0               , q("MegaSiemensPerMetre")     }}
        },
        { "ElectricalCapacitance", {
            { 1e-15           , q("PicoFarad")               },
            { 1e-12           , q("NanoFarad")               },
            { 1e-9            , q("MicroFarad")              },
            { 1e-6            , q("MilliFarad")              },
            { 0               , q("Farad")                   }}
        },
        { "ElectricalInductance", {
            { 1.0             , q("NanoHenry")               },
            { 1e3             , q("MicroHenry")              },
            { 1e6             , q("MilliHenry")              },
            { 0               , q("Henry")                   }}
        },
        { "VacuumPermittivity", {
            { 0               , q("FaradPerMetre")           }}
        },
        { "Frequency", {
            { 1e3             , q("Hertz")                   },
            { 1e6             , q("KiloHertz")               },
            { 1e9             , q("MegaHertz")               },
            { 1e12            , q("GigaHertz")               },
            { 0               , q("TeraHertz")               }}
        },
        { "Velocity", {
            { 0               , q("MilliMetrePerSecond")     }}
        },
        { "DynamicViscosity", {
            { 0               , q("PascalSecond")            }}
        },
        { "KinematicViscosity", {
            { 1e3             , q("SqMmPerSecond")           },
            { 0               , q("SqMetrePerSecond")        }}
        },
        { "VolumeFlowRate", {
            { 1e3             , q("CubicMmPerSecond")        },
            { 1e6             , q("MlPerSecond")             },
            { 1e9             , q("LitrePerSecond")          },
            { 0               , q("CubicMetrePerSecond")     }}
        },
        { "DissipationRate", {
            { 0               , q("WattPerKg")               }}
        },
        { "InverseLength", {
            { 1e-6            , q("PerMetre")                },
            { 1e-3            , q("PerKiloMetre")            },
            { 1.0             , q("PerMetre")                },
            { 1e3             , q("PerMilliMetre")           },
            { 1e6             , q("PerMicroMetre")           },
            { 1e9             , q("PerNanoMetre")            },
            { 0               , q("PerMetre")                }}
        },
        { "InverseArea", {
            { 1e-12           , q("PerSqMetre")              },
            { 1e-6            , q("PerSqKiloMetre")          },
            { 1.0             , q("PerSqMetre")              },
            { 1e2             , q("PerSqCentiMetre")         },
            { 0               , q("PerSqMilliMetre")         }}
        },
        { "InverseVolume", {
            { 1e-6            , q("PerCubicMetre")           },
            { 1e-3            , q("PerLitre")                },
            { 1.0             , q("PerMilliLitre")           },
            { 0               , q("PerCubicMilliMetre")      }}
        }
    }
};

inline const UnitsSchemaSpec s4
{ 1, "MKS", "m", false, false, QT_TRANSLATE_NOOP("UnitsApi", "MKS (m, kg, s, °)") , false,
    {
        { "Length", {
            { 1e-6            , q("MilliMetre")              },
            { 1e-3            , q("NanoMetre")               },
            { 0.1             , q("MicroMetre")              },
            { 1e4             , q("MilliMetre")              },
            { 1e7             , q("Metre")                   },
            { 1e10            , q("KiloMetre")               },
            { 0               , q("Metre")                   }}
        },
        { "Area", {
            { 100             , q("SquareMilliMetre")        },
            { 1e6             , q("SquareCentiMetre")        },
            { 1e12            , q("SquareMetre")             },
            { 0               , q("SquareKiloMetre")         }}
        },
        { "Volume", {
            { 1e3             , q("CubicMilliMetre")         },
            { 1e6             , q("MilliLiter")              },
            { 1e9             , q("Liter")                   },
            { 0               , q("CubicMetre")              }}
        },
        { "Mass", {
            { 1e-6            , q("MicroGram")               },
            { 1e-3            , q("MilliGram")               },
            { 1.0             , q("Gram")                    },
            { 1e3             , q("KiloGram")                },
            { 0               , q("Ton")                     }}
        },
        { "Density", {
            { 0.0001          , q("KgPerCubicMetre")         },
            { 1.0             , q("KgPerCubicCentiMetre")    },
            { 0               , q("KgPerCubicMilliMetre")    }}
        },
        { "Acceleration", {
            { 0               , q("MetrePerSecondSquared")   }}
        },
        { "Pressure", {
            { 10.0            , q("Pascal")                  },
            { 1e4             , q("KiloPascal")              },
            { 1e7             , q("MegaPascal")              },
            { 1e10            , q("GigaPascal")              },
            { 0               , q("Pascal")                  }}
        },
        { "Stiffness", {
            { 1               , q("MilliNewtonPerMeter")     },
            { 1e3             , q("NewtonPerMeter")          },
            { 1e6             , q("KiloNewtonPerMeter")      },
            { 0               , q("MegaNewtonPerMeter")      }}
        },
        { "StiffnessDensity", {
            { 1e-3            , q("PaPerMetre")              },
            { 1               , q("KPaPerMetre")             },
            { 1e3             , q("MPaPerMetre")             },
            { 0               , q("GPaPerMetre")             }}
        },
        { "ThermalConductivity", {
            { 1e6             , q("WattPerMetreKelvin")      },
            { 0               , q("WattPerMmKelvin")         }}
        },
        { "ThermalExpansionCoefficient", {
            { 0.001           , q("UmPerMPerKelvin")         },
            { 0               , q("MetrePerMetrePerKelvin")  }}
        },
        { "VolumetricThermalExpansionCoefficient", {
            { 0.001           , q("CubicMmPerCubicMPerK")    },
            { 0               , q("CubicMPerCubicMPerK")     }}
        },
        { "SpecificHeat", {
            { 0               , q("JoulePerKgKelvin")        }}
        },
        { "ThermalTransferCoefficient", {
            { 0               , q("WattPerSqMetreKelvin")    }}
        },
        { "Force", {
            { 1e3             , q("MilliNewton")             },
            { 1e6             , q("Newton")                  },
            { 1e9             , q("KiloNewton")              },
            { 0               , q("MegaNewton")              }}
        },
        { "Power", {
            { 1e6             , q("MilliWatt")               },
            { 1e9             , q("Watt")                    },
            { 0               , q("KiloWatt")                }}
        },
        { "ElectricPotential", {
            { 1e6             , q("MilliVolt")               },
            { 1e9             , q("Volt")                    },
            { 1e12            , q("KiloVolt")                },
            { 0               , q("Volt")                    }}
        },
        { "ElectricCharge", {
            { 0               , q("Coulomb")                 }}
        },
        { "SurfaceChargeDensity", {
            { 0               , q("CoulombPerSqMetre")       }}
        },
        { "VolumeChargeDensity", {
            { 0               , q("CoulombPerCubicMetre")    }}
        },
        { "CurrentDensity", {
            { 1.0             , q("AmperePerSqMetre")        },
            { 0               , q("AmperePerSqMilliMetre")   }}
        },
        { "MagneticFluxDensity", {
            { 1.0             , q("MilliTesla")              },
            { 0               , q("Tesla")                   }}
        },
        { "MagneticFieldStrength", {
            { 0               , q("AmperePerMetre")          }}
        },
        { "MagneticFlux", {
            { 0               , q("Weber")                   }}
        },
        { "Magnetization", {
            { 0               , q("AmperePerMetre")          }}
        },
        { "ElectromagneticPotential", {
            { 0               , q("WeberPerMetre")           }}
        },
        { "ElectricalConductance", {
            { 1e-9            , q("MicroSiemens")            },
            { 1e-6            , q("MilliSiemens")            },
            { 0               , q("Siemens")                 }}
        },
        { "ElectricalResistance", {
            { 1e9             , q("Ohm")                     },
            { 1e12            , q("KiloOhm")                 },
            { 0               , q("MegaOhm")                 }}
        },
        { "ElectricalConductivity", {
            { 1e-9            , q("MilliSiemensPerMetre")    },
            { 1e-6            , q("SiemensPerMetre")         },
            { 1e-3            , q("KiloSiemensPerMetre")     },
            { 0               , q("MegaSiemensPerMetre")     }}
        },
        { "ElectricalCapacitance", {
            { 1e-15           , q("PicoFarad")               },
            { 1e-12           , q("NanoFarad")               },
            { 1e-9            , q("MicroFarad")              },
            { 1e-6            , q("MilliFarad")              },
            { 0               , q("Farad")                   }}
        },
        { "ElectricalInductance", {
            { 1.0             , q("NanoHenry")               },
            { 1e3             , q("MicroHenry")              },
            { 1e6             , q("MilliHenry")              },
            { 0               , q("Henry")                   }}
        },
        { "VacuumPermittivity", {
            { 0               , q("FaradPerMetre")           }}
        },
        { "Work", {
            { 1.602176634e-10 , q("ElectronVolt")            },
            { 1.602176634e-7  , q("KiloElectronVolt")        },
            { 1.602176634e-4  , q("MegaElectronVolt")        },
            { 1e6             , q("MilliJoule")              },
            { 1e9             , q("Joule")                   },
            { 1e12            , q("KiloJoule")               },
            { 3.6e+15         , q("KiloWattHour")            },
            { 0               , q("Joule")                   }}
        },
        { "SpecificEnergy", {
            { 0               , q("SqMetrePerSqSecond")      }}
        },
        { "HeatFlux", {
            { 0               , q("WattPerSqMetre")          }}
        },
        { "Frequency", {
            { 1e3             , q("Hertz")                   },
            { 1e6             , q("KiloHertz")               },
            { 1e9             , q("MegaHertz")               },
            { 1e12            , q("GigaHertz")               },
            { 0               , q("TeraHertz")               }}
        },
        { "Velocity", {
            { 0               , q("MetrePerSecond")          }}
        },
        { "DynamicViscosity", {
            { 0               , q("PascalSecond")            }}
        },
        { "KinematicViscosity", {
            { 0               , q("SqMetrePerSecond")        }}
        },
        { "VolumeFlowRate", {
            { 1e-3            , q("CubicMetrePerSecond")     },
            { 1e3             , q("CubicMmPerSecond")        },
            { 1e6             , q("MlPerSecond")             },
            { 1e9             , q("LitrePerSecond")          },
            { 0               , q("CubicMetrePerSecond")     }}
        },
        { "DissipationRate", {
            { 0               , q("WattPerKg")               }}
        },
        { "InverseLength", {
            { 1e-6            , q("PerMetre")                },
            { 1e-3            , q("PerKiloMetre")            },
            { 1.0             , q("PerMetre")                },
            { 1e3             , q("PerMilliMetre")           },
            { 1e6             , q("PerMicroMetre")           },
            { 1e9             , q("PerNanoMetre")            },
            { 0               , q("PerMetre")                }}
        },
        { "InverseArea", {
            { 1e-12           , q("PerSqMetre")              },
            { 1e-6            , q("PerSqKiloMetre")          },
            { 1.0             , q("PerSqMetre")              },
            { 1e2             , q("PerSqCentiMetre")         },
            { 0               , q("PerSqMilliMetre")         }}
        },
        { "InverseVolume", {
            { 1e-6            , q("PerCubicMetre")           },
            { 1e-3            , q("PerLitre")                },
            { 1.0             , q("PerMilliLitre")           },
            { 0               , q("PerCubicMilliMetre")      }}
        }
    }
};

inline const UnitsSchemaSpec s5
{ 4, "Centimeter", "cm", false, false, QT_TRANSLATE_NOOP("UnitsApi", "Building Euro (cm, m², m³)") , false,
    {
        { "Length", {
            { 0              , q("CentiMetre")               }}
        },
        { "Area", {
            { 0              , q("SquareMetre")              }}
        },
        { "Volume", {
            { 0              , q("CubicMetre")               }}
        },
        { "Power", {
            { 0              , q("Watt")                     }}
        },
        { "ElectricPotential", {
            { 0              , q("Volt")                     }}
        },
        { "HeatFlux", {
            { 0              , q("WattPerSqMetre")           }}
        },
        { "Velocity", {
            { 0              , q("MilliMetrePerMinute")      }}
        }
    }
};

inline const UnitsSchemaSpec s6
{ 8, "FEM", "mm", false , false , QT_TRANSLATE_NOOP("UnitsApi", "FEM (mm, N, s)"), false,
    {
        { "Length", {
            { 0             , q("MilliMetre")                }}
        },
        { "Mass",   {
            { 0             , q("Ton")                       }}
        }
    }
};

inline const UnitsSchemaSpec s7
{ 2, "Imperial", "in", false, false, QT_TRANSLATE_NOOP("UnitsApi", "US customary (in, lb)"), false,
    {
        { "Length", {
            { 0.00000254      , q("Inch")                    },
            { 2.54            , q("Thou")                    },
            { 304.8           , q("InchMark")                },
            { 914.4           , q("FootMark")                },
            { 1'609'344.0     , q("Yard")                    },
            { 1'609'344'000.0 , q("Mile")                    },
            { 0               , q("Inch")                    }}
        },
        { "Angle", {
            { 0               , q("Degree")                  }}
        },
        { "Area", {
            { 0               , q("SquareInch")              }}
        },
        { "Volume", {
            { 0               , q("CubicInch")               }}
        },
        { "Mass", {
            { 0               , q("Pound")                   }}
        },
        { "Pressure", {
            { 1000 * psi      , q("PSI")                     },
            { 1000000 * psi   , q("KSI")                     },
            { 0               , q("PSI")                     }}
        },
        { "Stiffness", {
            { 0               , q("PoundForcePerInch")       }}
        },
        { "Velocity", {
            { 0               , q("InchPerMinute")           }}
        }
    }
};

inline const UnitsSchemaSpec s8
{ 5, "ImperialBuilding", "ft", true, false , QT_TRANSLATE_NOOP("UnitsApi", "Building US (ft-in, sqft, cft)"), false,
    {
        { "Length"   , {{ 0   , nullptr, "toFractional"          }}},
        { "Angle"    , {{ 0   , q("Degree")                      }}},
        { "Area"     , {{ 0   , q("SquareFoot")                  }}},
        { "Volume"   , {{ 0   , q("CubicFoot")                   }}},
        { "Velocity" , {{ 0   , q("InchPerMinute")               }}}
    }
};

inline const UnitsSchemaSpec s9
{ 7, "ImperialCivil", "ft", false, true, QT_TRANSLATE_NOOP("UnitsApi", "Imperial for Civil Eng (ft, lb, mph)"), false,
    {
        { "Length"   , {{ 0   , q("Foot")                        }}},
        { "Area"     , {{ 0   , q("SquareFoot2")                 }}},
        { "Volume"   , {{ 0   , q("CubicFoot2")                  }}},
        { "Mass"                     , {{ 0 , q("Pound")              }}},
        { "Pressure"                 , {{ 0 , q("PSI")               }}},
        { "Stiffness"               , {{ 0 , q("PoundForcePerInch")  }}},
        { "Velocity" , {{ 0   , q("MilePerHour")                 }}},
        { "Angle"    , {{ 0   , nullptr, "toDMS"                 }}}
    }
};

// clang-format on
// NOLINTEND
inline const std::vector schemaSpecs {s3, s4, s5, s6, s7, s8, s9, s0, s1, s2};

/**
 * Special functions
 *
 * A schema unit can have custom formatting via a special function
 * Such functions must be included here and also registered in special functions caller (below)
 */

/** utility function for toFractional */
inline std::size_t greatestCommonDenominator(const std::size_t a, const std::size_t b)
{
    return b == 0 ? a : greatestCommonDenominator(b, a % b);  // Euclid's algorithm
}

/**
 * double -> [feet'] [inches" [+ fraction]"], e.g.: 3' 4" + 3/8"
 */
inline std::string toFractional(const double value, std::size_t denominator)
{
    constexpr auto inchPerFoot {12};
    constexpr auto mmPerInch {25.4};

    auto numFractUnits = static_cast<std::size_t>(
        std::round(std::abs(value) / mmPerInch * denominator)
    );
    if (numFractUnits == 0) {
        return "0";
    }

    const auto feet = static_cast<std::size_t>(std::floor(numFractUnits / (inchPerFoot * denominator)));
    numFractUnits -= inchPerFoot * denominator * feet;

    const auto inches = static_cast<std::size_t>(std::floor(numFractUnits / denominator));
    std::size_t numerator = numFractUnits - (denominator * inches);

    const std::size_t common_denom = greatestCommonDenominator(numerator, denominator);
    numerator /= common_denom;
    denominator /= common_denom;

    bool addSpace {false};
    std::string result;

    if (value < 0) {
        result += "-";
    }

    if (feet > 0) {
        result += fmt::format("{}'", feet);
        addSpace = true;
    }

    if (inches > 0) {
        result += fmt::format("{}{}\"", addSpace ? " " : "", inches);
        addSpace = false;
    }

    if (numerator > 0) {
        if (inches > 0) {
            result += fmt::format(" {} ", value < 0 ? "-" : "+");
            addSpace = false;
        }
        result += fmt::format("{}{}/{}\"", addSpace ? " " : "", numerator, denominator);
    }

    return result;
}

/**
 * double -> degrees°[minutes′[seconds″]]
 */
inline std::string toDms(const double value)
{
    constexpr auto dmsRatio {60.0};

    auto calc = [&](const double total) -> std::pair<int, double> {
        const double whole = std::floor(total);
        return {static_cast<int>(whole), dmsRatio * (total - whole)};
    };

    auto [degrees, totalMinutes] = calc(value);
    std::string out = fmt::format("{}°", degrees);

    if (totalMinutes > 0) {
        auto [minutes, totalSeconds] = calc(totalMinutes);
        out += fmt::format("{}′", minutes);

        if (totalSeconds > 0) {
            out += fmt::format("{}″", std::round(totalSeconds));
        }
    }

    return out;
}


/**
 * Special functions caller
 */

// clang-format off
inline const std::map<std::string, std::function<std::string(double, std::size_t, std::size_t, double&, std::string&)>> specials
{
    {
        { "toDMS"        , [](const double val, [[maybe_unused]] const std::size_t precision, [[maybe_unused]] const std::size_t denominator,
                              double& factor, std::string& unitString) {
            factor = 1.0;
            unitString = "deg";
            return toDms(val);
        }},
        { "toFractional" , [](const double val, [[maybe_unused]] const std::size_t precision, const std::size_t denominator,
                              double& factor, std::string& unitString) {
            factor = 25.4;
            unitString = "in";
            return toFractional(val, denominator);
        }}
    }
};  // clang-format on

inline std::string runSpecial(
    std::string_view name,
    const double value,
    const std::size_t precision,
    const std::size_t denominator,
    double& factor,
    std::string& unitString
)
{
    const auto key = std::string(name);
    return specials.contains(key)
        ? specials.at(key)(value, precision, denominator, factor, unitString)
        : "";
}


/**
 * Build data pack
 */
inline const UnitsSchemasDataPack unitSchemasDataPack {schemaSpecs, defDecimals, defDenominator};


}  // namespace Base::UnitsSchemasData
#endif  // BASE_UNITSSCHEMASDATA_H
