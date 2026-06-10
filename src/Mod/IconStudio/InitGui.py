# SPDX-License-Identifier: LGPL-2.1-or-later

import FreeCAD
import FreeCADGui as Gui


class IconStudioCommand:
    def GetResources(self):
        return {
            "MenuText": "Icon Studio…",
            "ToolTip": "Open the Icon Studio dock panel: render the active "
            "document at the axonometric23 angle and export an SVG icon "
            "template with HLR linework, raster fill, and guide overlay.",
        }

    def Activated(self):
        import IconStudio
        IconStudio.show_panel()

    def IsActive(self):
        return Gui.ActiveDocument is not None


Gui.addCommand("Std_IconStudio", IconStudioCommand())
