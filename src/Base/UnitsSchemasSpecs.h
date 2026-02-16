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

#ifndef UNITSCHEMASPECS_H
#define UNITSCHEMASPECS_H

#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "UnitSystem.h"

namespace Base
{

struct QuantitySpec;

struct UnitTranslationSpec
{
    double threshold {0};
    const QuantitySpec* quantity {nullptr};
    std::string_view specialFunction {};
};

struct UnitsSchemaSpec
{
    std::size_t num;
    std::string name;
    std::string basicLengthUnitStr;
    bool isMultUnitLen {false};
    bool isMultUnitAngle {false};
    const char* description;
    bool isDefault {false};
    UnitSystem unitSystem {UnitSystem::Metric};

    /**
     * Applicable spec is the first with threshold > value under test
     * Special case: Threshold = 0 : default
     * Special case: quantity = nullptr : specialFunction names a custom formatter
     */
    std::map<std::string, std::vector<UnitTranslationSpec>> translationSpecs;
};

struct UnitsSchemasDataPack
{
    std::vector<UnitsSchemaSpec> specs;
    size_t defDecimals;
    size_t defDenominator;
};


}  // namespace Base
#endif  // UNITSCHEMASPECS_H
