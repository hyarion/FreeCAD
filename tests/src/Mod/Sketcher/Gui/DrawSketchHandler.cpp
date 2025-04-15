// SPDX-License-Identifier: LGPL-2.1-or-later

#include <gtest/gtest.h>

#define protected public
#include <Mod/Sketcher/Gui/DrawSketchHandler.h>
#undef protected

using namespace Sketcher;
using namespace SketcherGui;


// Tests that no constraint is returned when no geometry is preselected
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test, NoPreselection_ReturnsNoConstraint)
{
    // When no geometry is preselected (geoId is GeoUndef), the function should
    // return no constraint, regardless of the other parameters. This ensures
    // that no automatic constraints are created when nothing is selected.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = GeoEnum::GeoUndef;

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(1.0, 0.0);

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::VERTEX);
    EXPECT_FALSE(result.has_value());
}

// Tests that a Coincident constraint is returned when selecting a vertex point
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test, VertexPoint_ReturnsCoincidentConstraint)
{
    // When a vertex is selected (posId is not none) and the target type is VERTEX,
    // the function should return a Coincident constraint to make the new point
    // coincident with the selected vertex. This is used when drawing a new point
    // that should be connected to an existing vertex.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::start;

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(1.0, 0.0);

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::VERTEX);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->Type, Sketcher::Coincident);
    EXPECT_EQ(result->GeoId, 1);
    EXPECT_EQ(result->PosId, PointPos::start);
}

// Tests that a PointOnObject constraint is returned when selecting a line
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test,
     LineSelection_ReturnsPointOnObjectConstraint)
{
    // When a line is selected (posId is none) and the target type is VERTEX,
    // the function should return a PointOnObject constraint to make the new point
    // lie on the selected line. This is used when drawing a new point that should
    // be constrained to lie on an existing line.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::none;

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(1.0, 0.0);

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::VERTEX);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->Type, Sketcher::PointOnObject);
    EXPECT_EQ(result->GeoId, 1);
    EXPECT_EQ(result->PosId, PointPos::none);
}

// Tests that a Tangent constraint is returned when drawing perpendicular to a line
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test,
     PerpendicularLine_ReturnsTangentConstraint)
{
    // When a line is selected (isLine is true) and the cursor direction is perpendicular
    // to the line direction, the function should return a Tangent constraint.
    // This is because two lines are tangent when they intersect at exactly one point,
    // which occurs when they are perpendicular. The tangent constraint ensures
    // the new line will touch the existing line at a single point.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::none;
    preSel.isLine = true;
    preSel.hitShapeDir = Base::Vector3d(1.0, 0.0, 0.0);

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(0.0, 1.0);  // Perpendicular to line

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::CURVE);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->Type, Sketcher::Tangent);
    EXPECT_EQ(result->GeoId, 1);
    EXPECT_EQ(result->PosId, PointPos::none);
}

// Tests that no constraint is returned when drawing parallel to a line
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test, ParallelLine_ReturnsNoConstraint)
{
    // When a line is selected but the cursor direction is parallel to the line
    // direction, the function should not return a constraint. This is because
    // parallel lines either never intersect (if they are distinct) or coincide
    // completely (if they are the same line), neither of which satisfies the
    // geometric definition of tangency (intersecting at exactly one point).
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::none;
    preSel.isLine = true;
    preSel.hitShapeDir = Base::Vector3d(1.0, 0.0, 0.0);

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(1.0, 0.0);  // Parallel to line

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::CURVE);
    EXPECT_FALSE(result.has_value());
}

// Tests that no constraint is returned when direction vector is zero
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test, ZeroDirection_ReturnsNoConstraint)
{
    // When the direction vector is zero, the function should not return a constraint
    // because it cannot determine the angle between the directions. This prevents
    // creating invalid tangent constraints when the direction is undefined.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::none;
    preSel.isLine = true;
    preSel.hitShapeDir = Base::Vector3d(1.0, 0.0, 0.0);

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(0.0, 0.0);  // Zero direction

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::CURVE);
    EXPECT_FALSE(result.has_value());
}

// Tests that VERTEX_NO_TANGENCY behaves the same as VERTEX
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test, VertexNoTangency_ReturnsSameAsVertex)
{
    // The VERTEX_NO_TANGENCY case should behave the same as VERTEX, returning a
    // Coincident constraint when a vertex is selected. This test verifies that
    // the behavior is consistent between the two cases, ensuring that the
    // NO_TANGENCY flag doesn't affect vertex selection behavior.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::start;

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(1.0, 0.0);

    auto result =
        DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                          nullptr,
                                                          pos,
                                                          dir,
                                                          AutoConstraint::VERTEX_NO_TANGENCY);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->Type, Sketcher::Coincident);
    EXPECT_EQ(result->GeoId, 1);
    EXPECT_EQ(result->PosId, PointPos::start);
}

// Tests that a PointOnObject constraint is returned when selecting a point on a curve
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test,
     CurvePoint_ReturnsPointOnObjectConstraint)
{
    // When a point on a curve is selected (posId is not none) and the target type
    // is CURVE, the function should return a PointOnObject constraint. This is used
    // when drawing a curve that should pass through a specific point on another curve,
    // such as when connecting curves at their endpoints.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::start;
    preSel.isLine = true;
    preSel.hitShapeDir = Base::Vector3d(1.0, 0.0, 0.0);

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(1.0, 0.0);

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::CURVE);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->Type, Sketcher::PointOnObject);
    EXPECT_EQ(result->GeoId, 1);
    EXPECT_EQ(result->PosId, PointPos::start);
}

// Tests that a Tangent constraint is returned for non-line curves without angle check
TEST(DrawSketchHandler_findPreselectionAutoConstraint_Test, NonLineCurve_ReturnsTangentConstraint)
{
    // When a non-line curve is selected (isLine is false), the function should
    // return a Tangent constraint without checking the angle condition. This is
    // used when drawing a curve that should be tangent to another curve, and
    // the exact angle of tangency will be determined by the curve's shape.
    DrawSketchHandler::PreselectionData preSel;
    preSel.geoId = 1;
    preSel.posId = PointPos::none;
    preSel.isLine = false;
    preSel.hitShapeDir = Base::Vector3d(1.0, 0.0, 0.0);

    Base::Vector2d pos(0.0, 0.0);
    Base::Vector2d dir(1.0, 0.0);

    auto result = DrawSketchHandler::findPreselectionAutoConstraint(preSel,
                                                                    nullptr,
                                                                    pos,
                                                                    dir,
                                                                    AutoConstraint::CURVE);
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result->Type, Sketcher::Tangent);
    EXPECT_EQ(result->GeoId, 1);
    EXPECT_EQ(result->PosId, PointPos::none);
}
