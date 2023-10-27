#include "Roc_GL/FontManager.hpp"

#include <Roc_GL/Fonts/Noto-Sans.hpp>
#include "Roc_GL/CoordinateSystem.hpp"

#include "Roc_GL/Shader.hpp"


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
    FT_Face defaultface = LoadFontFromMemory(Noto_Sans_Font, Noto_Sans_len, "Noto Sans");
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
    face_index++;
    
    LoadedFonts.emplace(font_name, newface);
    return newface;
}

bool FontManager::DeleteFont(const std::string& font_name)
{
    if (LoadedFonts.find(font_name) == LoadedFonts.end())
    {
        LogWarn("Attempted to delete font " + font_name + " that doesn't exist.");
        return false;
    }

    FT_Error err = FT_Done_Face(LoadedFonts[font_name]);
    if (err)
    {
        LogError("FreeType error: " + std::string(FT_Error_String(err)));
        return false;
    }
    return LoadedFonts.erase(font_name);
}

bool FontManager::LoadGlyph(char c, const std::string& font_name, Character& char_object, int pixelsize)
{
    if (LoadedFonts.find(font_name) == LoadedFonts.end())
    {
        LogWarn("Attempted to load font " + font_name + " that doesn't exist");
        return false;
    }

    FT_Face font = LoadedFonts[font_name];
    FT_Set_Pixel_Sizes(font, 0, pixelsize);

    /* Taken from https://learnopengl.com/In-Practice/Text-Rendering */
    // load character glyph 
    if (FT_Load_Char(font, c, FT_LOAD_RENDER))
    {
        LogError(std::string("Failed to load Glyph ") + c);
        return false;
    }
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // generate texture
    glGenTextures(1, &char_object.TextureID);
    glBindTexture(GL_TEXTURE_2D, char_object.TextureID);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        font->glyph->bitmap.width,
        font->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        font->glyph->bitmap.buffer
    );
    // set texture options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // now store character for later use
    char_object.size_x_pixels = font->glyph->bitmap.width;
    char_object.size_y_pixels = font->glyph->bitmap.rows;
    char_object.bearing_x_pixels = font->glyph->bitmap_left;
    char_object.bearing_y_pixels = font->glyph->bitmap_top;
    char_object.advance_pixels = font->glyph->advance.x;
    char_object.font_key = font_name;

    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    return true;
}

double PixelToGLSizeX(int pixels)
{
    return 2 * ((double)pixels / (double)CoordinateSystem::screen_width);
}

double PixelToGLSizeY(int pixels)
{
    return 2 * ((double)pixels / (double)CoordinateSystem::screen_height);
}

void FontManager::RenderCharacter(Character& character, double& engine_x, double engine_y)
{
    FT_Face font = LoadedFonts[character.font_key];

    double xpos = E2GLX(engine_x) + PixelToGLSizeX(character.bearing_x_pixels);
    double ypos = E2GLY(engine_y) - PixelToGLSizeY(character.size_y_pixels - character.bearing_y_pixels);
    double w = PixelToGLSizeX(character.size_x_pixels);
    double h = PixelToGLSizeY(character.size_y_pixels);

    double vertices[] = {
        // x        y           z       tex_x       tex_y
        xpos,       ypos + h,   0.0,    0.0f,       0.0f ,
        xpos,       ypos,       0.0,    0.0f,       1.0f, 
        xpos + w,   ypos,       0.0,    1.0f,       1.0f,
        xpos,       ypos + h,   0.0,    0.0f,       0.0f,
        xpos + w,   ypos,       0.0,    1.0f,       1.0f,
        xpos + w,   ypos + h,   0.0,    1.0f,       0.0f  
    };

    // enable text shader
    Shader& text_shader = Shaders::ShaderMap["system-text-shader"];
    text_shader.Use();

    glBindVertexArray(VAO);
    glBindTexture(GL_TEXTURE_2D, character.TextureID);
    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // render quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
    engine_x += P2EX(character.advance_pixels >> 6); // bitshift by 6 to get value in pixels (2^6 = 64)

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void FontManager::RenderText(const std::string& text, const std::string& font_name, int pixelsize, double engine_x, double engine_y)
{
    double ex_ref = engine_x;
    for (char c : text)
    {
        Character character;
        fm->LoadGlyph(c, font_name, character, pixelsize);
        fm->RenderCharacter(character, ex_ref, engine_y);
    }
}