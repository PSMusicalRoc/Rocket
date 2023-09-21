#include "Roc_GL/Texture.hpp"

#include <string.h>
#include <iostream>

#include "RocLogger/RocLogger.hpp"

//TextureHashMap Textures;

std::map<std::string, TextureInfo> Textures::TextureMap;

size_t texture_hash(const std::string& key)
{
    size_t sum = 0;
    for (int i = 0; i < key.size(); i++)
    {
        sum += pow(10, i) * (key[i]);
    }

    return sum;
}

void ClearTexture(TextureInfo& info)
{
    if (info.tex_id != 0)
    {
        LogInfo("Freeing texture " + info.tex_key);
        glDeleteTextures(1, &info.tex_id);
        LogInfo("Freed texture " + info.tex_key);

        info.tex_id = 0;
        info.height = 0;
        info.width = 0;
        info.tex_key = "";
    }
}

std::ostream& operator<<(std::ostream& stream, const TextureInfo& info)
{
    stream << "Texture Info {" << std::endl;
    stream << "  GLuint ID: " << info.tex_id << std::endl;
    stream << "  int Height: " << info.height << std::endl;
    stream << "  int Width: " << info.width << std::endl;
    stream << "  string Key: " << info.tex_key << std::endl;
    stream << "}";

    return stream;
}

int TextureHashMap::get_next_mult_2(int inval)
{
    int i = 1;
    for (i; i < inval; i *= 2) {}

    return i * 2;
}

void TextureHashMap::regenerate_array(int newsize)
{
    size_t oldsize = _size;
    size_t oldnumelements = _num_elements;
    TextureInfo* temp = _arr;

    _size = get_next_mult_2(oldsize);
    _arr = new TextureInfo[_size];
    _num_elements = 0;

    for (int i = 0; i < oldsize; i++)
    {
        if ((*(temp + i)).tex_id != 0)
        {
            emplace(*(temp + i));
        }
    }

    delete[] temp;
}

TextureHashMap::TextureHashMap(size_t initial_size)
{
    if (initial_size > 0)
    {
        _size = initial_size;
    }
    else
    {
        _size = 4;
    }
    _arr = new TextureInfo[_size];
}

void TextureHashMap::DeleteHashMap()
{
    for (int i = 0; i < _size; i++)
    {
        if ((*(_arr + i)).tex_id != 0)
        {
            ClearTexture(_arr[i]);
        }
    }

    _size = 0;
    _num_elements = 0;
    delete[] _arr;
    _arr = nullptr;
}

TextureInfo TextureHashMap::at(const std::string& key)
{
    size_t start_loc = texture_hash(key) % _size;
    
    int i = start_loc;
    for (i; i < _size; i++)
    {
        if (key == _arr[i].tex_key)
        {
            return _arr[i];
        }
    }

    for (i = 0; i < start_loc; i++)
    {
        if (key == _arr[i].tex_key)
        {
            return _arr[i];
        }
    }

    TextureInfo out;
    return out;
}

void TextureHashMap::emplace(TextureInfo info)
{
    if ((double)(_num_elements + 1) / (double)_size > ratio)
    {
        regenerate_array(get_next_mult_2(_size));
    }

    size_t start_loc = texture_hash(info.tex_key) % _size;
    for (int i = start_loc; i < _size; i++)
    {
        if (_arr[i].tex_id == 0)
        {
            _arr[i] = info;
            _num_elements++;
            return;
        }
    }
    for (int i = 0; i < start_loc; i++)
    {
        if (_arr[i].tex_id == 0)
        {
            _arr[i] = info;
            _num_elements++;
            return;
        }
    }
}

void TextureHashMap::erase(const std::string& key)
{
    size_t start_loc = texture_hash(key) % _size;
    for (int i = start_loc; i < _size; i++)
    {
        if (_arr[i].tex_key == key)
        {
            _arr[i] = TextureInfo();
            _num_elements--;
            return;
        }
    }
    for (int i = 0; i < start_loc; i++)
    {
        if (_arr[i].tex_key == key)
        {
            _arr[i] = TextureInfo();
            _num_elements--;
            return;
        }
    }
}

void TextureHashMap::PrintHashMap()
{
#ifdef ROC_DEBUG
    std::cout << "Hashmap Printing" << std::endl;
    std::cout << "  Size: " << _size << std::endl;
    std::cout << "  Number of Elements: " << _num_elements << std::endl;
    std::cout << "---------------------------" << std::endl << std::endl;
    for (int i = 0; i < _size; i++)
    {
        std::cout << "Index " << i << std::endl;
        std::cout << _arr[i] << std::endl << std::endl;
    }
#endif
}

TextureInfo LoadTexture(const std::string& filename, const std::string& texture_key)
{
    unsigned char* data;
    TextureInfo info;
    int texture_channels;

    data = stbi_load(filename.c_str(), &info.width, &info.height, &texture_channels, 0);
    if (data == 0)
    {
        // failure, nothing read in
        LogError("Could not load image from filename " + filename + "!");
        if (stbi_failure_reason())
        {
            LogError(std::string("Reason: ") + stbi_failure_reason());
        }
        return TextureInfo();
    }

    LogInfo("Generating a texture for " + filename + "... (" + texture_key + ")");
    info.tex_id = 0;
    glGenTextures(1, &info.tex_id);
    glBindTexture(GL_TEXTURE_2D, info.tex_id);

    if (texture_channels == 3)
    {
        // RGB
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        LogInfo("Loaded texture " + texture_key + " as RGB");
    }
    else if (texture_channels == 4)
    {
        // RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, info.width, info.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        LogInfo("Loaded texture " + texture_key + " as RGBA");
    }
    
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    info.tex_key = texture_key;

    LogInfo("Successfully created texture " + texture_key);

    Textures::TextureMap.emplace(info.tex_key, info);

    return info;
}

void RenderTextureAsSprite(const std::string& key, double pos_x, double pox_y, double size_x, double size_y)
{
    
}
