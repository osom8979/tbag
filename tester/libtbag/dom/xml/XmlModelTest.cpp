/**
 * @file   XmlModelTest.cpp
 * @brief  XmlModel class tester.
 * @author zer0
 * @date   2017-04-21
 */

#include <gtest/gtest.h>
#include <tester/DemoAsset.hpp>
#include <libtbag/dom/xml/XmlModel.hpp>
#include <libtbag/dom/xml/node/ResourceNodeInterface.hpp>

#include <string>

using namespace libtbag;
using namespace libtbag::dom;
using namespace libtbag::dom::xml;

TBAG_CONSTEXPR static char const * const FIRST_NODE_NAME   = "Node";
TBAG_CONSTEXPR static char const * const INTEGER_ATTR_NAME = "Integer";

TBAG_CONSTEXPR static char const * const TEXT_VALUE = "TEST STRING";
TBAG_CONSTEXPR static int const INTEGER_ATTR_VALUE = 100;

struct ModelNode : public node::ResourceNodeInterface
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

TEST(XmlModelTest, Resource)
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

    node->setRootDirectory(tttDirGet().toString());
    ASSERT_FALSE(node->getDynamicAsset().empty());

    std::string const TAG1 = "tag1";
    std::string const TAG2 = "tag2";

    std::string const KEY1 = "key1";
    std::string const KEY2 = "key2";

    std::string const VAL1 = "val1";
    std::string const VAL2 = "val2";
    std::string const VAL3 = "val3";
    std::string const VAL4 = "val4";

    ASSERT_TRUE(node->saveValue(TAG1, KEY1, VAL1));
    ASSERT_EQ(VAL1, node->readValue<std::string>(TAG1, KEY1));

    ASSERT_TRUE(node->saveValue(TAG1, KEY2, VAL2));
    ASSERT_EQ(VAL2, node->readValue<std::string>(TAG1, KEY2));

    ASSERT_TRUE(node->readValue(TAG2, KEY1, std::string()).empty());
    ASSERT_TRUE(node->readValue(TAG2, KEY2, std::string()).empty());

    ASSERT_TRUE(node->saveValue(TAG2, KEY1, VAL3));
    ASSERT_TRUE(node->saveValue(TAG2, KEY2, VAL4));

    ASSERT_EQ(VAL1, node->readValue<std::string>(TAG1, KEY1));
    ASSERT_EQ(VAL2, node->readValue<std::string>(TAG1, KEY2));
    ASSERT_EQ(VAL3, node->readValue<std::string>(TAG2, KEY1));
    ASSERT_EQ(VAL4, node->readValue<std::string>(TAG2, KEY2));

    ASSERT_TRUE(node->saveValue(TAG2, KEY1, 100));
    ASSERT_TRUE(node->saveValue(TAG2, KEY2, 200));
    ASSERT_EQ(100, node->readValue(TAG2, KEY1, 0));
    ASSERT_EQ(200, node->readValue(TAG2, KEY2, 0));
}

