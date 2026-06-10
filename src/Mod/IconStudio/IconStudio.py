# SPDX-License-Identifier: LGPL-2.1-or-later
"""
Icon Studio - dockable panel that renders the active document at the
axonometric23 angle and exports an SVG icon template with HLR linework,
checker bg, raster fill, and an optional guide overlay.

Conventions:
  - 1 mm world == 1 SVG unit == 1 pixel grid cell
  - Icon canvas is 24 mm x 24 mm centered at the world origin
  - SVG viewBox is 0 0 24 24 (Y-down; we flip from camera Y-up)
"""

import base64
import contextlib
import math
import os
import tempfile

import FreeCAD as App
import FreeCADGui as Gui
from PySide import QtCore, QtGui, QtWidgets

from PySide.QtSvg import QSvgRenderer


class PixelatedSvgPreview(QtWidgets.QWidget):
    """Three-layer preview: hand-painted checker bg, raster PNG with NN
    scaling, then a vector SVG overlay (HLR + grid + guides). The PNG is
    drawn outside of QSvgRenderer because Qt's SVG renderer ignores
    image-rendering CSS and always bilinear-scales <image> elements.

    Mouse interaction:
      - left-drag emits panRequested(dx_widget_px, dy_widget_px) so the
        panel can convert to camera-frame mm.
      - wheel emits zoomRequested(steps) where one notch = 1.0.
    """

    panRequested = QtCore.Signal(float, float)
    zoomRequested = QtCore.Signal(float)

    def __init__(self, parent=None):
        super().__init__(parent)
        self._renderer = QSvgRenderer(self)
        self._renderer.repaintNeeded.connect(self.update)
        self._png_image = None
        self._show_checker = True
        self._drag_last = None
        self.setCursor(QtCore.Qt.OpenHandCursor)
        self.setMouseTracking(False)

    def update_layers(self, svg_text, png_path, show_checker):
        if isinstance(svg_text, str):
            data = QtCore.QByteArray(svg_text.encode("utf-8"))
        elif isinstance(svg_text, bytes):
            data = QtCore.QByteArray(svg_text)
        else:
            data = svg_text
        self._renderer.load(data)

        if png_path and os.path.exists(png_path):
            img = QtGui.QImage(png_path)
            self._png_image = img if not img.isNull() else None
        else:
            self._png_image = None

        self._show_checker = bool(show_checker)
        self.update()

    def paintEvent(self, _event):
        side = min(self.width(), self.height())
        x = (self.width() - side) / 2
        y = (self.height() - side) / 2
        target = QtCore.QRectF(x, y, side, side)

        painter = QtGui.QPainter(self)
        painter.setRenderHint(QtGui.QPainter.Antialiasing, True)
        painter.setRenderHint(QtGui.QPainter.SmoothPixmapTransform, False)

        if self._show_checker:
            # 0.5 SVG units per checker square -> 48 squares per icon side.
            squares = int(ICON_SIZE_MM * 2)
            cell = side / squares
            light = QtGui.QColor("#ffffff")
            dark = QtGui.QColor("#cccccc")
            painter.fillRect(target, light)
            painter.setPen(QtCore.Qt.NoPen)
            for row in range(squares):
                for col in range(squares):
                    if (row + col) & 1:
                        cx = x + col * cell
                        cy = y + row * cell
                        # Slight overdraw so subpixel gaps don't show.
                        painter.fillRect(
                            QtCore.QRectF(cx, cy, cell + 0.5, cell + 0.5), dark
                        )

        if self._png_image is not None:
            painter.drawImage(target, self._png_image)

        if self._renderer.isValid():
            self._renderer.render(painter, target)

    # Mouse interaction --------------------------------------------------

    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            self._drag_last = event.pos()
            self.setCursor(QtCore.Qt.ClosedHandCursor)

    def mouseMoveEvent(self, event):
        if self._drag_last is None:
            return
        delta = event.pos() - self._drag_last
        self._drag_last = event.pos()
        if delta.x() or delta.y():
            self.panRequested.emit(float(delta.x()), float(delta.y()))

    def mouseReleaseEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            self._drag_last = None
            self.setCursor(QtCore.Qt.OpenHandCursor)

    def wheelEvent(self, event):
        steps = event.angleDelta().y() / 120.0  # one notch = 120
        if steps:
            self.zoomRequested.emit(steps)
        event.accept()


class IconSizePreview(QtWidgets.QWidget):
    """Renders the same icon at a specific pixel resolution (off-screen)
    and upscales it nearest-neighbour to a fixed display size — so the user
    can see how the strokes / fill / checker actually rasterize at, say,
    16 px or 32 px."""

    DISPLAY_PX = 128

    def __init__(self, render_size_px, parent=None):
        super().__init__(parent)
        self._renderer = QSvgRenderer(self)
        self._renderer.repaintNeeded.connect(self.update)
        self._render_size = render_size_px
        self._png_image = None
        self._show_checker = True
        self._bg_color = QtGui.QColor("#808080")
        self.setFixedSize(self.DISPLAY_PX, self.DISPLAY_PX)

    def set_background_color(self, color):
        if isinstance(color, str):
            color = QtGui.QColor(color)
        self._bg_color = color
        self.update()

    def update_layers(self, svg_text, png_path, show_checker):
        if isinstance(svg_text, str):
            data = QtCore.QByteArray(svg_text.encode("utf-8"))
        elif isinstance(svg_text, bytes):
            data = QtCore.QByteArray(svg_text)
        else:
            data = svg_text
        self._renderer.load(data)
        if png_path and os.path.exists(png_path):
            img = QtGui.QImage(png_path)
            self._png_image = img if not img.isNull() else None
        else:
            self._png_image = None
        self._show_checker = bool(show_checker)
        self.update()

    def paintEvent(self, _event):
        n = self._render_size
        small = QtGui.QImage(n, n, QtGui.QImage.Format_ARGB32_Premultiplied)
        small.fill(0)

        p1 = QtGui.QPainter(small)
        p1.setRenderHint(QtGui.QPainter.Antialiasing, True)
        p1.setRenderHint(QtGui.QPainter.SmoothPixmapTransform, False)
        rect_n = QtCore.QRectF(0, 0, n, n)

        if self._show_checker:
            squares = int(ICON_SIZE_MM * 2)
            cell = n / squares
            p1.fillRect(rect_n, QtGui.QColor("#ffffff"))
            dark = QtGui.QColor("#cccccc")
            for row in range(squares):
                for col in range(squares):
                    if (row + col) & 1:
                        p1.fillRect(
                            QtCore.QRectF(col * cell, row * cell,
                                          cell + 0.5, cell + 0.5),
                            dark,
                        )

        if self._png_image is not None:
            p1.drawImage(rect_n, self._png_image)

        if self._renderer.isValid():
            self._renderer.render(p1, rect_n)
        p1.end()

        p2 = QtGui.QPainter(self)
        p2.fillRect(self.rect(), self._bg_color)
        p2.setRenderHint(QtGui.QPainter.SmoothPixmapTransform, False)
        p2.drawImage(QtCore.QRectF(0, 0, self.DISPLAY_PX, self.DISPLAY_PX), small)


# Quaternion (qx, qy, qz, qw) matching Gui::Camera::axonometric23() in C++.
AXONOMETRIC23_Q = (0.512377, 0.182608, 0.281702, 0.790423)

# Camera looks along this direction in world space.
# Derived analytically: (-1/sqrt(3), 1/sqrt(2), -1/sqrt(6))
VIEW_DIR = App.Vector(
    -1.0 / math.sqrt(3.0),
    1.0 / math.sqrt(2.0),
    -1.0 / math.sqrt(6.0),
)

ICON_SIZE_MM = 24.0  # world extent of the icon canvas
PNG_MULTIPLIER = 16  # PNG fill rendered at 24 * PNG_MULTIPLIER px per side
GUIDES_PATH = os.path.join(os.path.dirname(__file__), "Resources", "guides.svg")


# ---------------------------------------------------------------------------
# Geometry pipeline
# ---------------------------------------------------------------------------

def _camera_rotation():
    return App.Rotation(*AXONOMETRIC23_Q)


def _collect_shapes():
    """All visible shape-bearing objects with at least one face. We ignore
    selection on purpose so the selection-highlight color doesn't bleed
    into the PNG render."""
    doc = App.ActiveDocument
    if doc is None:
        return []
    out = []
    for o in doc.Objects:
        if not hasattr(o, "Shape"):
            continue
        sh = o.Shape
        if sh is None or sh.isNull() or not sh.Faces:
            continue
        if not getattr(o.ViewObject, "Visibility", True):
            continue
        # Skip features inside a PartDesign::Body — the Body's Shape already
        # is the tip's final geometry, so including a Pad/Fillet/etc. on top
        # of the Body draws the same lines twice (often at offset positions
        # because the features and the Body have different Placements).
        get_parent = getattr(o, "getParentGeoFeatureGroup", None)
        parent = get_parent() if get_parent else None
        if parent is not None and parent.isDerivedFrom("PartDesign::Body"):
            continue
        out.append(o)
    return out


def _baked_world_shape(obj):
    """Return a copy of obj.Shape with its Placement baked into geometry,
    so further transforms compose cleanly without relying on metadata."""
    sh = obj.Shape.copy()
    matrix = sh.Placement.toMatrix()
    sh.Placement = App.Placement()  # reset before baking
    return sh.transformGeometry(matrix)


# Drawing order: interior kinds first, silhouette last so a 1-px silhouette
# overdraws boundary fragments of the 0.5-px interior kinds for free.
EDGE_KINDS = ("sharp", "smooth", "seam", "iso", "silhouette")


def _empty_edges():
    return {k: {"visible": [], "hidden": []} for k in EDGE_KINDS}


def _hlr_edges(shape):
    """Run HLR via TechDraw and return a dict keyed by edge kind, each
    containing visible and hidden lists of Part.Edge objects in screen-mm
    (camera Y-up). Edge kinds:

      silhouette = full visible outline (via findShapeOutline / EdgeWalker)
      sharp      = sharp/main edges (V/H)
      smooth     = tangent/smooth edges, e.g. fillet boundaries (V1/H1)
      seam       = parameter-space seams on revolved/swept faces (VN/HN)
      iso        = iso-parametric helper lines (VI/HI)

    Note: there is no `silhouette` hidden bin — findShapeOutline only walks
    the visible outer wire.
    """
    import TechDraw

    # OCCT's HLR projector convention: the direction vector points from the
    # projection plane TOWARD THE EYE, not along the looking ray. After
    # rotating the shape into camera frame, the eye is at +Z.
    direction = App.Vector(0, 0, 1)
    V, V1, VN, _VO, VI, H, H1, HN, _HO, HI = TechDraw.projectEx(shape, direction)

    def edges_of(compound):
        return list(compound.Edges) if compound is not None else []

    silhouette_visible = []
    try:
        outer = TechDraw.findShapeOutline(shape, 1.0, direction)
        if outer is not None:
            silhouette_visible = edges_of(outer)
    except Exception as exc:
        App.Console.PrintWarning(
            f"IconStudio: silhouette extraction failed: {exc}\n"
        )

    return {
        "sharp":      {"visible": edges_of(V),    "hidden": edges_of(H)},
        "smooth":     {"visible": edges_of(V1),   "hidden": edges_of(H1)},
        "seam":       {"visible": edges_of(VN),   "hidden": edges_of(HN)},
        "iso":        {"visible": edges_of(VI),   "hidden": edges_of(HI)},
        "silhouette": {"visible": silhouette_visible, "hidden": []},
    }


def compute_hlr_for_active():
    """Returns the per-kind edge dict for the currently visible objects."""
    objs = _collect_shapes()
    if not objs:
        return _empty_edges()
    import Part

    rot_matrix = App.Placement(
        App.Vector(), _camera_rotation().inverted()
    ).toMatrix()

    rotated_shapes = []
    for o in objs:
        try:
            sh = _baked_world_shape(o)
            rotated_shapes.append(sh.transformGeometry(rot_matrix))
        except Exception as exc:
            App.Console.PrintWarning(
                f"IconStudio: skipping {o.Label}: {exc}\n"
            )

    if not rotated_shapes:
        return _empty_edges()

    compound = Part.makeCompound(rotated_shapes)
    return _hlr_edges(compound)


# ---------------------------------------------------------------------------
# Rendering and SVG composition
# ---------------------------------------------------------------------------

def setup_icon_camera(view, world_size_mm=ICON_SIZE_MM, zoom=1.0, pan=(0.0, 0.0)):
    """Configure the active 3D view as an orthographic camera at the
    axonometric23 angle. `zoom` shrinks the visible world extent (so 2.0 =
    twice the magnification). `pan` is in camera-frame mm and offsets the
    focal point from the world origin."""
    view.setCameraType("Orthographic")
    cam = view.getCameraNode()
    cam.orientation.setValue(*AXONOMETRIC23_Q)
    cam.height.setValue(world_size_mm / zoom)

    rot = _camera_rotation()
    cam_x_world = rot.multVec(App.Vector(1, 0, 0))
    cam_y_world = rot.multVec(App.Vector(0, 1, 0))
    focal = cam_x_world * pan[0] + cam_y_world * pan[1]

    distance = 1000.0
    back = App.Vector(-VIEW_DIR.x, -VIEW_DIR.y, -VIEW_DIR.z) * distance
    eye = focal + back
    cam.position.setValue(eye.x, eye.y, eye.z)
    cam.focalDistance.setValue(distance)
    cam.nearDistance.setValue(distance - world_size_mm * 4)
    cam.farDistance.setValue(distance + world_size_mm * 4)


@contextlib.contextmanager
def _viewport_overlays_hidden():
    """Temporarily hide on-screen overlays (NaviCube, axis cross, corner
    coordinate system) so they don't end up in the rendered PNG. The
    runtime listener is `View3DSettings`, attached to the
    `User parameter:BaseApp/Preferences/View` group."""
    hGrp = App.ParamGet("User parameter:BaseApp/Preferences/View")
    keys = {
        "ShowNaviCube": True,
        "ShowAxisCross": False,
        "CornerCoordSystem": True,
    }
    saved = {k: hGrp.GetBool(k, default) for k, default in keys.items()}
    try:
        for k in keys:
            if saved[k]:
                hGrp.SetBool(k, False)
        Gui.updateGui()
        yield
    finally:
        for k, v in saved.items():
            if v:
                hGrp.SetBool(k, True)


def render_png(out_path, multiplier, zoom=1.0, pan=(0.0, 0.0)):
    if Gui.ActiveDocument is None or Gui.ActiveDocument.ActiveView is None:
        raise RuntimeError("No active 3D view to render from.")
    view = Gui.ActiveDocument.ActiveView
    if os.path.exists(out_path):
        try:
            os.remove(out_path)
        except OSError:
            pass
    Gui.Selection.clearSelection()
    setup_icon_camera(view, zoom=zoom, pan=pan)
    Gui.updateGui()
    pixels = int(round(ICON_SIZE_MM * multiplier))
    with _viewport_overlays_hidden():
        view.saveImage(out_path, pixels, pixels, "Transparent")
    return pixels


def _png_data_uri(path):
    with open(path, "rb") as fh:
        data = fh.read()
    return "data:image/png;base64," + base64.b64encode(data).decode("ascii")


def _to_svg_xy(x, y):
    """Camera (x, y) in mm with (0,0) at origin and Y-up
    -> SVG (x, y) with (0,0) at top-left, Y-down, in 24-unit space."""
    return (ICON_SIZE_MM / 2 + x, ICON_SIZE_MM / 2 - y)


def _discretized_path(edge, xfrm, deflection=0.05):
    """Fallback emitter: chord-tolerance polyline for any edge geometry."""
    try:
        pts = edge.discretize(Deflection=deflection)
    except Exception:
        pts = []
    if len(pts) < 2:
        try:
            pts = [edge.valueAt(edge.FirstParameter), edge.valueAt(edge.LastParameter)]
        except Exception:
            return ""
    sx, sy = xfrm(pts[0].x, pts[0].y)
    parts = [f"M{sx:.4f},{sy:.4f}"]
    for p in pts[1:]:
        x, y = xfrm(p.x, p.y)
        parts.append(f"L{x:.4f},{y:.4f}")
    return "".join(parts)


def _edge_to_svg_path(edge, xfrm):
    """Build the SVG path-data 'd' attribute for one OCCT edge, applying
    `xfrm(x_mm, y_mm) -> (svg_x, svg_y)`. Picks the appropriate primitive
    per OCCT curve type so straight edges stay 2 points and circles/arcs
    map onto exact SVG arc commands. Falls back to a discretized polyline
    if anything in the curve-specific path raises."""
    try:
        return _edge_to_svg_path_impl(edge, xfrm)
    except Exception as exc:
        App.Console.PrintLog(
            f"IconStudio: edge emit fell back to discretize: {exc}\n"
        )
        return _discretized_path(edge, xfrm)


def _edge_to_svg_path_impl(edge, xfrm):
    import Part

    curve = edge.Curve

    try:
        first, last = edge.FirstParameter, edge.LastParameter
        p_start = edge.valueAt(first)
        p_end = edge.valueAt(last)
        sx, sy = xfrm(p_start.x, p_start.y)
        ex, ey = xfrm(p_end.x, p_end.y)
    except Exception:
        return _discretized_path(edge, xfrm)

    # Straight line — exact, 2 points.
    if isinstance(curve, Part.Line):
        return f"M{sx:.4f},{sy:.4f}L{ex:.4f},{ey:.4f}"

    # Everything else (Ellipse, ArcOf*, BSplineCurve, Bezier, …) goes
    # through adaptive discretize. Reasons:
    #   - Projected conics come back as rational BSplines (cubic Bezier
    #     can't represent them) or ArcOfEllipse with rotation, where the
    #     SVG A-command flags are fiddly.
    #   - Sub-arcs from HLR splits can have a tiny chord (start ≈ end)
    #     even when the arc is partial; any "is this a full circle?"
    #     heuristic risks turning the partial arc into a full one.
    # Discretize respects the edge's First/Last parameter range, so HLR
    # splits stay correct, and at 0.05 mm deflection the chord error is
    # well under a pixel at icon scale.
    return _discretized_path(edge, xfrm)


def _read_guides_svg(path):
    if not path or not os.path.exists(path):
        return ""
    try:
        with open(path, "r", encoding="utf-8") as fh:
            text = fh.read()
        start = text.find("<svg")
        if start < 0:
            return ""
        gt = text.find(">", start)
        end = text.rfind("</svg>")
        if gt < 0 or end < 0:
            return ""
        return text[gt + 1:end]
    except Exception as exc:
        App.Console.PrintWarning(f"IconStudio: could not load guides SVG: {exc}\n")
        return ""


def compose_svg(
    edges,
    png_path=None,
    guides_path=GUIDES_PATH,
    show_grid=True,
    show_checker=True,
    show_guides=True,
    show_hidden=False,
    edge_visibility=None,
    for_preview=False,
    xfrm=None,
):
    """Build the final SVG document as a string. `edges` is a per-kind dict
    {kind: {"visible": [Part.Edge], "hidden": [Part.Edge]}} in camera-space
    mm (Y-up). `xfrm(x,y)->(svg_x,svg_y)` carries pan/zoom and the Y-flip;
    if omitted, defaults to the static centered Y-flip."""

    if edge_visibility is None:
        edge_visibility = {k: True for k in EDGE_KINDS}
    if xfrm is None:
        xfrm = _to_svg_xy

    png_uri = _png_data_uri(png_path) if png_path and os.path.exists(png_path) else ""
    guides_inner = _read_guides_svg(guides_path) if show_guides else ""

    grid_lines = []
    if show_grid:
        for i in range(1, int(ICON_SIZE_MM)):
            grid_lines.append(
                f'<line x1="{i}" y1="0" x2="{i}" y2="{ICON_SIZE_MM}"/>'
            )
            grid_lines.append(
                f'<line x1="0" y1="{i}" x2="{ICON_SIZE_MM}" y2="{i}"/>'
            )

    image_tag = "" if for_preview else (
        (
            f'<image href="{png_uri}" x="0" y="0" '
            f'width="{ICON_SIZE_MM}" height="{ICON_SIZE_MM}" '
            f'style="image-rendering:crisp-edges;image-rendering:pixelated"/>'
        ) if png_uri else ""
    )

    bg_block = "" if for_preview else (
        f'<rect width="{ICON_SIZE_MM}" height="{ICON_SIZE_MM}" fill="url(#iconstudio-checker)"/>'
        if show_checker else ""
    )

    def edge_group(group_id, edge_list, kind, mode):
        # Silhouette is the visible boundary of the shape (sharp + smooth
        # combined via EdgeWalker); render it thicker than interior edges.
        width = 1.0 if kind == "silhouette" else 0.5
        if mode == "hidden":
            extra = f' stroke-dasharray="{width},{width}"'
        else:
            extra = ""
        on = edge_visibility.get(kind, True) and (mode == "visible" or show_hidden)
        display = "" if on else ' display="none"'
        path_strs = []
        for e in edge_list:
            d = _edge_to_svg_path(e, xfrm)
            if d:
                path_strs.append(f'<path d="{d}"/>')
        paths = "\n    ".join(path_strs)
        return (
            f'<g id="{group_id}" fill="none" stroke="#000000" '
            f'stroke-width="{width}" stroke-linecap="round" '
            f'stroke-linejoin="round"{extra}{display}>\n    '
            f'{paths}\n  </g>'
        )

    visible_groups = "\n  ".join(
        edge_group(f"hlr-{k}", edges.get(k, {}).get("visible", []), k, "visible")
        for k in EDGE_KINDS
    )
    hidden_groups = "\n  ".join(
        edge_group(f"hidden-{k}", edges.get(k, {}).get("hidden", []), k, "hidden")
        for k in EDGE_KINDS
    )

    return f'''<?xml version="1.0" encoding="UTF-8"?>
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink"
     viewBox="0 0 {ICON_SIZE_MM} {ICON_SIZE_MM}" width="{ICON_SIZE_MM}" height="{ICON_SIZE_MM}">
  <defs>
    <pattern id="iconstudio-checker" x="0" y="0" width="1" height="1" patternUnits="userSpaceOnUse">
      <rect width="0.5" height="0.5" fill="#cccccc"/>
      <rect x="0.5" y="0.5" width="0.5" height="0.5" fill="#cccccc"/>
      <rect x="0.5" y="0" width="0.5" height="0.5" fill="#ffffff"/>
      <rect x="0" y="0.5" width="0.5" height="0.5" fill="#ffffff"/>
    </pattern>
  </defs>
  <g id="background">
    {bg_block}
  </g>
  <g id="fill">
    {image_tag}
  </g>
  <g id="hlr">
  {visible_groups}
  </g>
  <g id="hidden">
  {hidden_groups}
  </g>
  <g id="grid" stroke="#888888" stroke-width="0.02" fill="none">
    {chr(10).join(grid_lines)}
  </g>
  <g id="guides">
    {guides_inner}
  </g>
</svg>
'''


# ---------------------------------------------------------------------------
# Dockable panel with live preview
# ---------------------------------------------------------------------------

class IconStudioPanel(QtWidgets.QDockWidget):
    """Dockable panel: 3D-source controls on top, live SVG preview below."""

    OBJECT_NAME = "IconStudioDock"

    def __init__(self, parent=None):
        super().__init__("Icon Studio", parent)
        self.setObjectName(self.OBJECT_NAME)

        # Cached HLR + PNG so cheap recomposes don't redo expensive work.
        self._cached_edges = _empty_edges()
        self._cached_png_path = None

        # Pan in camera-frame mm; zoom is a multiplicative factor (1.0 =
        # 24mm fits the canvas, 2.0 = 12mm fits, etc.).
        self._pan = [0.0, 0.0]
        self._zoom = 1.0

        self._compose_timer = QtCore.QTimer(self)
        self._compose_timer.setSingleShot(True)
        self._compose_timer.setInterval(120)
        self._compose_timer.timeout.connect(self._update_preview)

        # Re-render the PNG only after the user stops interacting, since
        # saveImage is the slow step.
        self._png_timer = QtCore.QTimer(self)
        self._png_timer.setSingleShot(True)
        self._png_timer.setInterval(250)
        self._png_timer.timeout.connect(self._refresh_png_only)

        self._build_ui()

    # -- UI ---------------------------------------------------------------

    def _build_ui(self):
        root = QtWidgets.QWidget(self)
        layout = QtWidgets.QVBoxLayout(root)
        layout.setContentsMargins(6, 6, 6, 6)
        layout.setSpacing(6)

        # Refresh / save bar
        bar = QtWidgets.QHBoxLayout()
        self.refresh_btn = QtWidgets.QPushButton("Refresh from 3D")
        self.refresh_btn.setToolTip(
            "Re-run HLR on the current selection (or visible objects) "
            "and re-render the PNG fill from the active 3D view."
        )
        self.refresh_btn.clicked.connect(self._refresh_from_3d)
        self.reset_btn = QtWidgets.QPushButton("Reset view")
        self.reset_btn.setToolTip("Reset pan and zoom (drag preview to pan, scroll to zoom).")
        self.reset_btn.clicked.connect(self._reset_view)
        self.save_btn = QtWidgets.QPushButton("Save SVG…")
        self.save_btn.clicked.connect(self._save)
        bar.addWidget(self.refresh_btn)
        bar.addWidget(self.reset_btn)
        bar.addWidget(self.save_btn)
        bar.addStretch()
        layout.addLayout(bar)

        # Form controls
        form = QtWidgets.QFormLayout()
        form.setLabelAlignment(QtCore.Qt.AlignRight)

        self.guides_edit = QtWidgets.QLineEdit(GUIDES_PATH)
        guides_browse = QtWidgets.QPushButton("…")
        guides_browse.setFixedWidth(28)
        guides_browse.clicked.connect(self._pick_guides)
        form.addRow("Guides SVG:", self._row(self.guides_edit, guides_browse))

        # Per-kind edge layer toggles. Silhouette renders at 1 px on top of
        # the 0.5 px interior kinds. Defaults: silhouette + sharp + smooth +
        # seam on (smooth catches fillets, seam catches swept/revolved
        # parts); iso off (noisy isoparametric helper lines).
        self.edge_checks = {}
        edge_defaults = {
            "silhouette": True,
            "sharp": True,
            "smooth": True,
            "seam": True,
            "iso": False,
        }
        edge_row = QtWidgets.QHBoxLayout()
        # UI order (silhouette first); EDGE_KINDS controls SVG draw order.
        for kind in edge_defaults:
            cb = QtWidgets.QCheckBox(kind.capitalize())
            cb.setChecked(edge_defaults[kind])
            self.edge_checks[kind] = cb
            edge_row.addWidget(cb)
        edge_row.addStretch()
        form.addRow("Edges:", self._wrap(edge_row))

        toggles = QtWidgets.QHBoxLayout()
        self.grid_check = QtWidgets.QCheckBox("Grid")
        self.grid_check.setChecked(True)
        self.checker_check = QtWidgets.QCheckBox("Checker")
        self.checker_check.setChecked(True)
        self.guides_check = QtWidgets.QCheckBox("Guides")
        self.guides_check.setChecked(True)
        self.hidden_check = QtWidgets.QCheckBox("Show hidden lines")
        toggles.addWidget(self.grid_check)
        toggles.addWidget(self.checker_check)
        toggles.addWidget(self.guides_check)
        toggles.addWidget(self.hidden_check)
        toggles.addStretch()
        form.addRow("Other:", self._wrap(toggles))

        layout.addLayout(form)

        # Background color for size previews — lets the user check the
        # icon against the actual surfaces it will sit on.
        bg_row = QtWidgets.QHBoxLayout()
        bg_row.addWidget(QtWidgets.QLabel("Preview bg:"))
        self.bg_group = QtWidgets.QButtonGroup(self)
        self.bg_light_rb = QtWidgets.QRadioButton("Light")
        self.bg_gray_rb = QtWidgets.QRadioButton("Gray")
        self.bg_dark_rb = QtWidgets.QRadioButton("Dark")
        self.bg_gray_rb.setChecked(True)
        for idx, btn in enumerate(
            (self.bg_light_rb, self.bg_gray_rb, self.bg_dark_rb)
        ):
            self.bg_group.addButton(btn, idx)
            bg_row.addWidget(btn)
        bg_row.addStretch()
        layout.addLayout(bg_row)
        self.bg_group.buttonClicked.connect(self._apply_preview_bg)

        # Row of size previews: shows how the icon rasterizes at common
        # render sizes, each upscaled nearest-neighbour to a fixed display
        # size for easy comparison.
        self.size_previews = []
        sizes_row = QtWidgets.QHBoxLayout()
        for size in (16, 24, 32, 48, 64):
            col = QtWidgets.QVBoxLayout()
            col.setSpacing(2)
            label = QtWidgets.QLabel(f"{size}px")
            label.setAlignment(QtCore.Qt.AlignCenter)
            sp = IconSizePreview(size, root)
            col.addWidget(label)
            col.addWidget(sp, 0, QtCore.Qt.AlignCenter)
            sizes_row.addLayout(col)
            self.size_previews.append(sp)
        sizes_row.addStretch()
        layout.addLayout(sizes_row)
        self._apply_preview_bg()  # set the initial bg

        # Main preview
        self.preview = PixelatedSvgPreview(root)
        self.preview.setMinimumSize(QtCore.QSize(240, 240))
        self.preview.setSizePolicy(
            QtWidgets.QSizePolicy.Expanding, QtWidgets.QSizePolicy.Expanding
        )
        self.preview.setStyleSheet("background: #2a2a2a; border: 1px solid #444;")
        self.preview.panRequested.connect(self._on_pan)
        self.preview.zoomRequested.connect(self._on_zoom)
        layout.addWidget(self.preview, 1)

        self.setWidget(root)

        # Live updates: any input change schedules a (cheap) re-compose.
        toggle_widgets = [
            self.grid_check, self.checker_check,
            self.guides_check, self.hidden_check,
        ] + list(self.edge_checks.values())
        for w in toggle_widgets:
            w.toggled.connect(self._schedule)
        self.guides_edit.textChanged.connect(self._schedule)

        # First paint with whatever's currently active.
        QtCore.QTimer.singleShot(0, self._refresh_from_3d)

    @staticmethod
    def _row(*widgets):
        w = QtWidgets.QWidget()
        h = QtWidgets.QHBoxLayout(w)
        h.setContentsMargins(0, 0, 0, 0)
        for x in widgets:
            h.addWidget(x)
        return w

    @staticmethod
    def _wrap(layout):
        w = QtWidgets.QWidget()
        w.setLayout(layout)
        return w

    def _pick_guides(self):
        start = self.guides_edit.text() or GUIDES_PATH
        path, _ = QtWidgets.QFileDialog.getOpenFileName(
            self, "Select Guides SVG", start, "SVG files (*.svg)"
        )
        if path:
            self.guides_edit.setText(path)

    # -- Pipeline -------------------------------------------------------

    def _refresh_from_3d(self):
        """Re-run the expensive steps: HLR + PNG. Then recompose."""
        QtWidgets.QApplication.setOverrideCursor(QtCore.Qt.WaitCursor)
        try:
            try:
                self._cached_edges = compute_hlr_for_active()
                counts = ", ".join(
                    f"{k}={len(self._cached_edges[k]['visible'])}/"
                    f"{len(self._cached_edges[k]['hidden'])}"
                    for k in EDGE_KINDS
                )
                App.Console.PrintMessage(
                    f"IconStudio HLR (visible/hidden): {counts}\n"
                )
            except Exception as exc:
                App.Console.PrintError(f"IconStudio: HLR failed: {exc}\n")
                self._cached_edges = _empty_edges()

            self._refresh_png_only(update_preview=False)
        finally:
            QtWidgets.QApplication.restoreOverrideCursor()

        self._update_preview()

    def _refresh_png_only(self, update_preview=True):
        """Re-render only the PNG (fast path used after pan/zoom interactions)."""
        png = os.path.join(tempfile.gettempdir(), "iconstudio_render.png")
        try:
            render_png(png, PNG_MULTIPLIER, zoom=self._zoom, pan=tuple(self._pan))
            self._cached_png_path = png
        except Exception as exc:
            App.Console.PrintWarning(f"IconStudio: PNG render failed: {exc}\n")
            self._cached_png_path = None
        if update_preview:
            self._update_preview()

    def _on_pan(self, dx_widget_px, dy_widget_px):
        """Drag delta in widget px → camera-frame mm. SVG-y is flipped vs
        camera-y, so dragging downward shifts the focal point upward in
        camera frame. Drag-to-pan: world content moves with the cursor."""
        side = min(self.preview.width(), self.preview.height())
        if side <= 0:
            return
        # Each widget px corresponds to (24 / side) SVG units, and 1 SVG
        # unit = (1/zoom) world mm at the current zoom level.
        mm_per_px = ICON_SIZE_MM / (side * self._zoom)
        self._pan[0] -= dx_widget_px * mm_per_px
        self._pan[1] += dy_widget_px * mm_per_px
        self._update_preview()
        self._png_timer.start()

    def _on_zoom(self, steps):
        old = self._zoom
        self._zoom = max(0.1, min(50.0, old * (1.15 ** steps)))
        if self._zoom != old:
            self._update_preview()
            self._png_timer.start()

    def _reset_view(self):
        self._pan = [0.0, 0.0]
        self._zoom = 1.0
        self._refresh_png_only()

    def _apply_preview_bg(self, *_):
        """Push the chosen radio bg colour onto each size preview."""
        colors = {0: "#f0f0f0", 1: "#808080", 2: "#1a1a1a"}
        color = colors.get(self.bg_group.checkedId(), "#808080")
        for sp in self.size_previews:
            sp.set_background_color(color)

    def _schedule(self, *_):
        self._compose_timer.start()

    def _xfrm(self):
        """Build the camera-mm → SVG-unit transform for the current pan/zoom.
        Y-flip is folded in here; SVG's Y axis points down, camera's Y up."""
        zoom = self._zoom
        px, py = self._pan
        half = ICON_SIZE_MM / 2

        def xfrm(x, y):
            return (half + (x - px) * zoom, half - (y - py) * zoom)
        return xfrm

    def _make_svg(self, for_preview, force_hide_guides=False, force_hide_grid=False):
        edge_visibility = {k: cb.isChecked() for k, cb in self.edge_checks.items()}
        return compose_svg(
            self._cached_edges,
            png_path=self._cached_png_path,
            guides_path=self.guides_edit.text().strip() or None,
            show_grid=self.grid_check.isChecked() and not force_hide_grid,
            show_checker=self.checker_check.isChecked(),
            show_guides=self.guides_check.isChecked() and not force_hide_guides,
            show_hidden=self.hidden_check.isChecked(),
            edge_visibility=edge_visibility,
            for_preview=for_preview,
            xfrm=self._xfrm(),
        )

    def _update_preview(self):
        try:
            svg_main = self._make_svg(for_preview=True)
            # Size previews show how the icon would actually rasterize, so
            # drop design-time overlays (guides + grid) for those.
            svg_size = self._make_svg(
                for_preview=True, force_hide_guides=True, force_hide_grid=True
            )
            png = self._cached_png_path
            checker = self.checker_check.isChecked()
            self.preview.update_layers(svg_main, png, checker)
            # Size previews mirror what the rendered icon would look like
            # against a transparent background, so checker is forced off.
            for sp in self.size_previews:
                sp.update_layers(svg_size, png, False)
        except Exception as exc:
            App.Console.PrintError(f"IconStudio: preview failed: {exc}\n")

    def _save(self):
        path, _ = QtWidgets.QFileDialog.getSaveFileName(
            self, "Save Icon SVG",
            os.path.expanduser("~/icon.svg"),
            "SVG files (*.svg)",
        )
        if not path:
            return
        try:
            with open(path, "w", encoding="utf-8") as fh:
                fh.write(self._make_svg(for_preview=False))
            App.Console.PrintMessage(f"IconStudio: wrote {path}\n")
        except Exception as exc:
            QtWidgets.QMessageBox.critical(self, "Icon Studio", str(exc))


# ---------------------------------------------------------------------------
# Entry points
# ---------------------------------------------------------------------------

_panel = None


def show_panel():
    """Create the dock if needed, then show + raise it."""
    global _panel
    mw = Gui.getMainWindow()
    if mw is None:
        return None

    if _panel is None:
        _panel = IconStudioPanel(mw)
        mw.addDockWidget(QtCore.Qt.RightDockWidgetArea, _panel)
    _panel.show()
    _panel.raise_()
    return _panel


# Backwards-compat shim for older invocations.
def show_dialog():
    return show_panel()


def generate(output_path, **kwargs):
    """One-shot SVG generation without UI. Kept for scripting use."""
    edges = compute_hlr_for_active()
    png = os.path.join(tempfile.gettempdir(), "iconstudio_render.png")
    try:
        render_png(png, kwargs.get("multiplier", PNG_MULTIPLIER))
    except Exception:
        png = None
    svg = compose_svg(
        edges,
        png_path=png,
        guides_path=kwargs.get("guides_path", GUIDES_PATH),
        show_grid=kwargs.get("show_grid", True),
        show_checker=kwargs.get("show_checker", True),
        show_guides=kwargs.get("show_guides", True),
        show_hidden=kwargs.get("show_hidden", False),
        edge_visibility=kwargs.get("edge_visibility"),
    )
    with open(output_path, "w", encoding="utf-8") as fh:
        fh.write(svg)
    App.Console.PrintMessage(f"IconStudio: wrote {output_path}\n")
