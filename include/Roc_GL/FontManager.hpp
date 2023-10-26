#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>

#include "RocLogger/RocLogger.hpp"

#ifndef __gl_h_
#include <glad/gl.h>
#endif

struct Character
{
    unsigned int    TextureID;
    unsigned int    size_x_pixels;
    unsigned int    size_y_pixels;
    int             bearing_x_pixels;
    int             bearing_y_pixels;
    unsigned int    advance_pixels;

    ~Character()
    {
        if (TextureID)
        {
            glDeleteTextures(1, &TextureID);
            LogTrace("Deleted Glyph Texture");
        }
    }
};

class FontManager
{
private:
    FT_Library library;
    std::map<std::string, FT_Face> LoadedFonts;
    static FontManager* fm;
    static unsigned int face_index;

    FontManager() {}

public:

    /**
     * Returns a pointer to the singleton FontManager.
     * 
     * @returns A pointer to the FontManager object
    */
    static FontManager* Get()
    {
        if (fm == nullptr)
        {
            fm = new FontManager();
        }
        return fm;
    }

    static void DestroyFontManager()
    {
        if (fm != nullptr)
        {
            FT_Done_FreeType(fm->library);
            delete fm;
        }
        fm = nullptr;
    }

    /**
     * Initializes the FreeType library for the engine,
     * allowing for font rendering.
     * 
     * @returns True if Freetype initialized correctly,
     * false if there was an error.
    */
    bool InitFreeType();

    /**
     * Creates a new font from a file, then adds it to the font
     * map.
     * 
     * @param filepath The path to the font you'd like to load
     * @param font_name The key that the font should be stored
     * in the font map under.
     * 
     * @returns The loaded FT_Face, or NULL if there was an error.
    */
    FT_Face LoadFontFromFile(const std::string filepath, const std::string font_name) { return NULL; }

    FT_Face LoadFontFromMemory(const unsigned char* font, unsigned int font_len, const std::string& font_name);

    bool DeleteFont(const std::string& font_name);

    /**
     * Loads a character from a font as a texture so that it can
     * be used to render text. Ensure that a size has been set
     * before using this function.
     * 
     * @param c The character to render to a texture
     * @param font_name The font to use to render the glyph
     * @param char_object The character object to store details
     * about the text into.
     * 
     * @returns True if loading was successful, false if
     * there was any error.
    */
    bool LoadGlyph(char c, const std::string& font_name, Character& char_object);
};