#include "Roc_GL/FontManager.hpp"

#include <RocLogger/RocLogger.hpp>
#include <Roc_GL/Fonts/Noto-Sans.hpp>


FontManager* FontManager::fm = nullptr;
unsigned int FontManager::face_index = 0;


bool FontManager::InitFreeType()
{
    FT_Error error = FT_Init_FreeType(&library);
    if (error)
    {
        LogError(std::string("Error loading Freetype: ") + FT_Error_String(error));
        return false;
    }

    // Load Engine Default font (Noto Sans)
    FT_Face defaultface = LoadFontFromMemory(Noto_Sans_Font, Noto_Sans_len * sizeof(char), "Noto Sans");
    if (defaultface == NULL) { return false; }

    return true;
}

FT_Face FontManager::LoadFontFromMemory(const unsigned char* font, unsigned int font_len, const std::string& font_name)
{
    FT_Face newface;
    FT_Error err = FT_New_Memory_Face(library, font, font_len, face_index, &newface);
    if (err)
    {
        LogError(std::string("Error loading Freetype: ") + FT_Error_String(err));
        return NULL;
    }
    newface++;
    
    LoadedFonts.emplace(font_name, newface);
    return newface;
}