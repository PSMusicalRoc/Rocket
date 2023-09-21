#ifndef _ROC_SHADER_GL_HPP_
#define _ROC_SHADER_GL_HPP_

#include <iostream>
#include <iomanip>

#include <map>

#include "Logger/RocLogger.hpp"
#include <glad/gl.h>
#include <string>

class Shader
{
    friend class ShaderHashMap;

protected:
    GLuint m_program;

    GLuint LoadShader(const std::string& vertshader_file, const std::string& fragshader_file);

    std::string _shader_name;

public:
    Shader();
    Shader(const Shader& shad) { *this = shad; }
    Shader(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name);

    void operator=(const Shader& shad);

    void DeleteShader();

    void Use() { glUseProgram(m_program); }
    bool SetUniformFloat(const std::string& uniform, float val);
    bool SetUniformInt(const std::string& uniform, int val);
    bool SetUniformBool(const std::string& uniform, bool val);
    bool SetUniformVec2(const std::string& uniform, float v1, float v2);
    bool SetUniformVec3(const std::string& uniform, float v1, float v2, float v3);
    bool SetUniformVec4(const std::string& uniform, float v1, float v2, float v3, float v4);
};

class Shaders
{
public:
    static std::map<std::string, Shader> ShaderMap;
    static void clear()
    {
        for (auto& p : ShaderMap)
        {
            p.second.DeleteShader();
        }
        ShaderMap.clear();
    }
};

class ShaderHashMap
{
protected:
    typedef long unsigned int size_t;

    Shader** _arr = nullptr;
    size_t _size = 0;
    size_t _num_elements = 0;

    double ratio = 0.7;

    int get_next_mult_2(int inval);
    void regenerate_array(int newsize);

public:
    ShaderHashMap(size_t initial_size = 0);
    void DeleteHashMap();
    ~ShaderHashMap() { DeleteHashMap(); }

    Shader* at(const std::string& key);
    Shader* operator[](const std::string& key) { return at(key); }
    size_t hash(const std::string& key);

    void emplace(const Shader& shader);
    void emplace(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name);
    void erase(const std::string& key);


    // These are debug functions, do not use much
    //void PrintHashMap();
};

//static ShaderHashMap Shaders;

Shader& LoadShader(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name);

#endif