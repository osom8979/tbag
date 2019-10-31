/**
 * @file   XmlUtilsTest.cpp
 * @brief  XmlUtils class tester.
 * @author zer0
 * @date   2019-10-31
 */

#include <gtest/gtest.h>
#include <libtbag/dom/xml/XmlUtils.hpp>

using namespace libtbag;
using namespace libtbag::dom;
using namespace libtbag::dom::xml;

TBAG_CONSTEXPR static char const * const TEST_XML = R"(
<root>
    <sub1>
        <sub2>text</sub2>
    </sub1>
</root>
)";

TEST(XmlUtilsTest, WriteElementToXmlText)
{
    Document doc;
    ASSERT_EQ(E_SUCCESS, readFromXmlText(doc, TEST_XML));

    auto * root = doc.FirstChild();
    ASSERT_NE(nullptr, root);
    ASSERT_STREQ("root", libtbag::dom::xml::name(*root->ToElement()).c_str());
    auto * sub1 = root->FirstChild();
    ASSERT_NE(nullptr, sub1);
    ASSERT_STREQ("sub1", libtbag::dom::xml::name(*sub1->ToElement()).c_str());
    auto * sub2 = sub1->FirstChild();
    ASSERT_NE(nullptr, sub2);
    ASSERT_STREQ("sub2", libtbag::dom::xml::name(*sub2->ToElement()).c_str());

    std::string sub1_xml;
    ASSERT_EQ(E_SUCCESS, writeElementToXmlText(*sub1->ToElement(), sub1_xml));
    ASSERT_FALSE(sub1_xml.empty());
    // std::cout << sub1_xml << std::endl;

    Document doc2;
    ASSERT_EQ(E_SUCCESS, readFromXmlText(doc2, sub1_xml));

    auto * doc2_sub1 = doc2.FirstChild();
    ASSERT_NE(nullptr, doc2_sub1);
    ASSERT_STREQ("sub1", libtbag::dom::xml::name(*doc2_sub1->ToElement()).c_str());
    auto * doc2_sub2 = doc2_sub1->FirstChild();
    ASSERT_NE(nullptr, doc2_sub2);
    ASSERT_STREQ("sub2", libtbag::dom::xml::name(*doc2_sub2->ToElement()).c_str());
}

