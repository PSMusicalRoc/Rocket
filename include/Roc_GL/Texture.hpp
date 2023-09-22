#pragma once

#include <glad/gl.h>
#include <string>
#include <map>
#include <cmath>
#include "RocLogger/RocLogger.hpp"

#include <stb_image.h>

struct TextureInfo
{
    GLuint tex_id = 0;
    int width = 0, height = 0;
    std::string tex_key = "";
};

void ClearTexture(TextureInfo& info);

class TexInfoHash
{
public:
    size_t operator()(const std::string& s) const
    {
        size_t sum = 0;
        for (unsigned int i = 0; i < s.size(); i++)
        {
            sum += pow(10, i) * (s[i]);
        }

        return sum;
    }
};

class TextureHashMap
{
protected:
    typedef long unsigned int size_t;

    TextureInfo* _arr = nullptr;
    size_t _size = 0;
    size_t _num_elements = 0;

    double ratio = 0.7;

    int get_next_mult_2(int inval);
    void regenerate_array();

public:
    TextureHashMap(size_t initial_size = 0);
    void DeleteHashMap();
    ~TextureHashMap() { DeleteHashMap(); }

    TextureInfo at(const std::string& key);
    TextureInfo operator[](const std::string& key) { return at(key); }

    void emplace(TextureInfo info);
    void erase(const std::string& key);


    // These are debug functions, do not use much
    void PrintHashMap();
};

class Textures
{
public:
    static TextureHashMap TextureMap;
    static void clear() { TextureMap.DeleteHashMap(); }
};

std::ostream& operator<<(std::ostream&, const TextureInfo& info);

TextureInfo LoadTexture(const std::string& filename, const std::string& texture_key);

void ClearTexture(TextureInfo& info);
