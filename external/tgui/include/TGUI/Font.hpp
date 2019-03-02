/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2019 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TGUI_FONT_HPP
#define TGUI_FONT_HPP

#include <TGUI/Config.hpp>
#include <SFML/Graphics/Font.hpp>
#include <cstdint>
#include <string>
#include <cstddef>
#include <memory>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    class TGUI_API Font
    {
    public:

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Default constructor which will set the font to nullptr
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(std::nullptr_t = nullptr);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to create the font from a string (filename by default)
        ///
        /// @param id  String to pass to the Deserializer class to load the font
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(const std::string& id);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to create the font from a string (filename by default)
        ///
        /// @param id  String to pass to the Deserializer class to load the font
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(const char* id);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to share the font directly
        ///
        /// @param font  Font to share
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(const std::shared_ptr<sf::Font>& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to set a copy of your font
        ///
        /// @param font  Font to copy
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(const sf::Font& font);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Constructor to create the font from a byte array
        /// @param data         Pointer to the file data in memory
        /// @param sizeInBytes  Size of the data to load, in bytes
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(const void* data, std::size_t sizeInBytes);


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Default copy constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(const Font& other) = default;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Default move constructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font(Font&& other) = default;

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Copy move assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font& operator=(const Font& other);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Copy move assignment operator
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        Font& operator=(Font&& other);

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        // @brief Destructor
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        ~Font();


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the id that was used to load the font
        ///
        /// @return Id of the font or an empty string when no id was passed when this object was created
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const std::string& getId() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Gets the underlying SFML font
        ///
        /// @return Font stored in this object
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        std::shared_ptr<sf::Font> getFont() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Gets the underlying SFML font
        ///
        /// @return Font stored in this object
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        operator std::shared_ptr<sf::Font>() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Converts the font to a boolean to check if it is a nullptr of not
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        operator bool() const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Compares the font with a nullptr
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool operator==(std::nullptr_t) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Compares the font with a nullptr
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool operator!=(std::nullptr_t) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Compares the font with another one
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool operator==(const Font& right) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Compares the font with another one
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        bool operator!=(const Font& right) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// \brief Retrieve a glyph of the font
        ///
        /// If the font is a bitmap font, not all character sizes might be available. If the glyph is not available at the
        /// requested size, an empty glyph is returned.
        ///
        /// Be aware that using a negative value for the outline thickness will cause distorted rendering.
        ///
        /// \param codePoint        Unicode code point of the character to get
        /// \param characterSize    Reference character size
        /// \param bold             Retrieve the bold version or the regular one?
        /// \param outlineThickness Thickness of outline (when != 0 the glyph will not be filled)
        ///
        /// \return The glyph corresponding to \a codePoint and \a characterSize
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        const sf::Glyph& getGlyph(std::uint32_t codePoint, unsigned int characterSize, bool bold, float outlineThickness = 0) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the kerning offset of two glyphs
        ///
        /// The kerning is an extra offset (negative) to apply between two glyphs when rendering them, to make the pair look
        /// more "natural". For example, the pair "AV" have a special kerning to make them closer than other characters.
        /// Most of the glyphs pairs have a kerning offset of zero, though.
        ///
        /// @param first         Unicode code point of the first character
        /// @param second        Unicode code point of the second character
        /// @param characterSize Size of the characters
        ///
        /// @return Kerning value for \a first and \a second, in pixels
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getKerning(std::uint32_t first, std::uint32_t second, unsigned int characterSize) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Returns the line spacing
        ///
        /// Line spacing is the vertical offset to apply between two consecutive lines of text.
        ///
        /// @param characterSize Size of the characters
        ///
        /// @return Line spacing, in pixels
        ///
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        float getLineSpacing(unsigned int characterSize) const;


        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    private:

        std::shared_ptr<sf::Font> m_font;
        std::string m_id;
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TGUI_TEXTURE_HPP
