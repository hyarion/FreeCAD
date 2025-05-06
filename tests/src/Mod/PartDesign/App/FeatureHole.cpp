// SPDX-License-Identifier: LGPL-2.1-or-later

#include <gtest/gtest.h>
#include "src/App/InitApplication.h"

#include <App/Application.h>
#include <App/Document.h>
#include <App/Origin.h>
#include "Mod/PartDesign/App/Body.h"
#include "Mod/PartDesign/App/FeatureHole.h"

using namespace PartDesign;

class HoleTest: public ::testing::Test
{
protected:
    static void SetUpTestSuite()
    {
        tests::initApplication();
    }

    void SetUp() override
    {
        _docName = App::GetApplication().getUniqueDocumentName("HoleTestDoc");
        _doc = App::GetApplication().newDocument(_docName.c_str(), "testUser");
        _body = _doc->addObject<PartDesign::Body>();
    }

    void TearDown() override
    {
        App::GetApplication().closeDocument(_docName.c_str());
    }

    App::Document* getDocument() const
    {
        return _doc;
    }
    PartDesign::Body* getBody() const
    {
        return _body;
    }

private:
    std::string _docName;
    App::Document* _doc = nullptr;
    PartDesign::Body* _body = nullptr;
};

TEST_F(HoleTest, FindClosestDesignationMatchesExpectedIndex)
{
    auto* hole = getDocument()->addObject<PartDesign::Hole>("Hole");
    ASSERT_TRUE(hole);
    getBody()->addObject(hole);

    // Set a valid thread type (e.g., 0 for ISO metric, depends on your data)
    const int threadTypeIndex = 0;
    hole->ThreadType.setValue(threadTypeIndex);

    // Fetch description set
    const auto& descList = Hole::threadDescription[threadTypeIndex];
    ASSERT_FALSE(descList.empty());

    // Pick a known entry in the thread description list
    const int knownIndex = 2;
    const auto& ref = descList[knownIndex];
    hole->ThreadDiameter.setValue(ref.diameter);
    hole->ThreadPitch.setValue(ref.pitch);

    // Set an intentionally wrong initial index
    hole->ThreadSize.setValue(0);

    // Act
    hole->findClosestDesignation();

    // Assert
    EXPECT_EQ(hole->ThreadSize.getValue(), knownIndex);
}
