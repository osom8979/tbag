/**
 * @file   XmlModelTest.cpp
 * @brief  XmlModel class tester.
 * @author zer0
 * @date   2017-04-21
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/dom/XmlModel.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::dom;

TBAG_CONSTEXPR static char const * const FIRST_NODE_NAME   = "Node";
TBAG_CONSTEXPR static char const * const INTEGER_ATTR_NAME = "Integer";

TBAG_CONSTEXPR static char const * const TEXT_VALUE = "TEST STRING";
TBAG_CONSTEXPR static int const INTEGER_ATTR_VALUE = 100;

struct ModelNode : public XmlModel::NodeInterface
{
    std::string root;

    int call_setup;
    int call_teardown;
    int call_load;
    mutable int call_save;

    int integer;
    std::string text;

    ModelNode(std::string const & r) : root(r) , call_setup(0), call_teardown(0), call_load(0), call_save(0)
    { /* EMPTY. */ }

    ~ModelNode()
    { /* EMPTY. */ }

    virtual std::string name() const override
    {
        return root;
    }

    virtual void setup() override
    {
        call_setup++;
        integer = INTEGER_ATTR_VALUE;
        text = TEXT_VALUE;
    }

    virtual void teardown() override
    {
        call_teardown++;
        integer = 0;
        text.clear();
    }

    virtual void load(Element const & element) override
    {
        call_load++;
        auto * node = element.FirstChildElement(FIRST_NODE_NAME);
        node->QueryAttribute(INTEGER_ATTR_NAME, &integer);
        text = node->GetText();
    }

    virtual void save(Element & element) const override
    {
        call_save++;
        auto * node = element.GetDocument()->NewElement(FIRST_NODE_NAME);
        node->SetAttribute(INTEGER_ATTR_NAME, integer);
        node->SetText(text.c_str());
        element.InsertEndChild(node);
    }
};

TEST(XmlModelTest, Default)
{
    tttDir(true, true);

    std::string const CASE_NAME = test_info_->test_case_name();
    XmlModel model;
    ASSERT_TRUE(model.add(XmlModel::SharedNode(new ModelNode(CASE_NAME))));

    auto weak = model.get(CASE_NAME);
    ASSERT_FALSE(weak.expired());
    auto shared = weak.lock();
    auto * node = static_cast<ModelNode*>(shared.get());
    ASSERT_NE(nullptr, node);

    auto const CONFIG_PATH = tttDirGet() / model.getFileName();
    namespace fs = ::libtbag::filesystem::details;

    ASSERT_EQ(0, node->call_setup);
    ASSERT_EQ(0, node->call_teardown);
    ASSERT_EQ(0, node->call_load);
    ASSERT_EQ(0, node->call_save);

    ASSERT_FALSE(CONFIG_PATH.exists());
    ASSERT_TRUE(model.loadOrDefaultSave(CONFIG_PATH));
    ASSERT_TRUE(CONFIG_PATH.exists());

    ASSERT_EQ(1, node->call_setup);
    ASSERT_EQ(1, node->call_teardown);
    ASSERT_EQ(0, node->call_load);
    ASSERT_EQ(1, node->call_save);

    ASSERT_EQ(TEXT_VALUE, node->text);
    ASSERT_EQ(INTEGER_ATTR_VALUE, node->integer);
    model.teardown();

    ASSERT_TRUE(model.load(CONFIG_PATH));
    ASSERT_EQ(1, node->call_setup);
    ASSERT_EQ(2, node->call_teardown);
    ASSERT_EQ(1, node->call_load);
    ASSERT_EQ(1, node->call_save);

    ASSERT_EQ(TEXT_VALUE, node->text);
    ASSERT_EQ(INTEGER_ATTR_VALUE, node->integer);
}

