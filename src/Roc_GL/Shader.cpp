#include "Roc_GL/Shader.hpp"
#include <fstream>
#include <iostream>
#include <cmath>

//ShaderHashMap Shaders;
std::map<std::string, Shader> Shaders::ShaderMap;

GLuint Shader::LoadShader(const std::string& vertshader_file, const std::string& fragshader_file)
{
    std::ifstream vsh_file(vertshader_file);
    if (!vsh_file.is_open())
    {
        LogError("Failed to load shader file " + vertshader_file + "!");
        return 0;
    }

    std::string inLine, vshad_source;
    while (std::getline(vsh_file, inLine))
    {
        vshad_source += inLine + "\n";
    }
    vsh_file.close();

    std::ifstream fsh_file(fragshader_file);
    if (!fsh_file.is_open())
    {
        LogError("Failed to load shader file " + fragshader_file + "!");
        return 0;
    }

    std::string fshad_source;
    while (std::getline(fsh_file, inLine))
    {
        fshad_source += inLine + "\n";
    }
    fsh_file.close();

    const char* vsh_cstr = vshad_source.c_str();

    GLuint vertshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertshader, 1, &vsh_cstr, NULL);
    glCompileShader(vertshader);

    GLint success;
    int totalfails = 0;
    char infoLog[512];

    glGetShaderiv(vertshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertshader, 512, NULL, infoLog);
        totalfails++;
        LogError(std::string("Could not compile vertex shader: ") + infoLog);
    }

    const char* fsh_cstr = fshad_source.c_str();

    GLuint fragshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragshader, 1, &fsh_cstr, NULL);
    glCompileShader(fragshader);

    glGetShaderiv(fragshader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragshader, 512, NULL, infoLog);
        totalfails++;
        LogError(std::string("Could not compile fragment shader: ") + infoLog);
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertshader);
    glAttachShader(program, fragshader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        totalfails++;
        LogError(std::string("Could not link shader program: ") + infoLog);
    }

    if (totalfails)
    {
        LogWarn("Errors found in loading shader " + _shader_name);
    }
    else
    {
        LogInfo("Successfully loaded shader " + _shader_name);
    }
    
    glDeleteShader(vertshader);
    glDeleteShader(fragshader);

    return program;
}


Shader::Shader() :m_program(0), _shader_name("") {}


Shader::Shader(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name)
    :_shader_name(shader_name)
{
    m_program = LoadShader(vertshaderfile, fragshaderfile);
}

void Shader::operator=(const Shader& shad)
{
    this->m_program = shad.m_program;
    this->_shader_name = shad._shader_name;
}

void Shader::DeleteShader()
{
    if (m_program)
    {
        glDeleteProgram(m_program);
        m_program = 0;
        LogInfo("Deleted Shader " + _shader_name);
        _shader_name = "";
    }
}

bool Shader::SetUniformFloat(const std::string& uniform, float val)
{
    int uniformloc = glGetUniformLocation(m_program, uniform.c_str());
    glUniform1f(uniformloc, val);
    return true;
}

bool Shader::SetUniformInt(const std::string& uniform, int val)
{
    int uniformloc = glGetUniformLocation(m_program, uniform.c_str());
    glUniform1i(uniformloc, val);
    return true;
}

bool Shader::SetUniformBool(const std::string& uniform, bool val)
{
    int uniformloc = glGetUniformLocation(m_program, uniform.c_str());
    glUniform1i(uniformloc, (int)val);
    return true;
}

bool Shader::SetUniformVec2(const std::string& uniform, float v1, float v2)
{
    int uniformloc = glGetUniformLocation(m_program, uniform.c_str());
    glUniform2f(uniformloc, v1, v2);
    return true;
}

bool Shader::SetUniformVec3(const std::string& uniform, float v1, float v2, float v3)
{
    int uniformloc = glGetUniformLocation(m_program, uniform.c_str());
    glUniform3f(uniformloc, v1, v2, v3);
    return true;
}

bool Shader::SetUniformVec4(const std::string& uniform, float v1, float v2, float v3, float v4)
{
    int uniformloc = glGetUniformLocation(m_program, uniform.c_str());
    glUniform4f(uniformloc, v1, v2, v3, v4);
    return true;
}




int ShaderHashMap::get_next_mult_2(int inval)
{
    int i = 1;
    for (i; i < inval; i *= 2) {}

    return i * 2;
}

void ShaderHashMap::regenerate_array(int newsize)
{
    size_t oldsize = _size;
    size_t oldnumelements = _num_elements;
    Shader** temp = _arr;

    _size = get_next_mult_2(oldsize);
    _arr = new Shader*[_size];
    for (int i = 0; i < _size; i++)
    {
        _arr[i] = new Shader();
    }
    _num_elements = 0;

    for (int i = 0; i < oldsize; i++)
    {
        if ((**(temp + i)).m_program != 0)
        {
            emplace(**(temp + i));
        }
    }

    delete[] temp;
}

ShaderHashMap::ShaderHashMap(size_t initial_size)
{
    if (initial_size > 0)
    {
        _size = initial_size;
    }
    else
    {
        _size = 4;
    }
    _arr = new Shader*[_size];
    for (int i = 0; i < _size; i++)
    {
        _arr[i] = new Shader();
    }
}

void ShaderHashMap::DeleteHashMap()
{
    for (int i = 0; i < _size; i++)
    {
        if (_arr[i]->m_program != 0)
        {
            _arr[i]->DeleteShader();
            delete _arr[i];
        }
    }
    _size = 0;
    _num_elements = 0;
    delete[] _arr;
    _arr = nullptr;
}

Shader* ShaderHashMap::at(const std::string& key)
{
    size_t start_loc = hash(key) % _size;
    
    int i = start_loc;
    for (i; i < _size; i++)
    {
        if (key == _arr[i]->_shader_name)
        {
            return _arr[i];
        }
    }

    for (i = 0; i < start_loc; i++)
    {
        if (key == _arr[i]->_shader_name)
        {
            return _arr[i];
        }
    }

    return nullptr;
}

size_t ShaderHashMap::hash(const std::string& key)
{
    size_t sum = 0;
    for (int i = 0; i < key.size(); i++)
    {
        sum += pow(10, i) * (key[i]);
    }

    return sum;
}

void ShaderHashMap::emplace(const Shader& info)
{
    if ((double)(_num_elements + 1) / (double)_size > ratio)
    {
        regenerate_array(get_next_mult_2(_size));
    }

    size_t start_loc = hash(info._shader_name) % _size;
    for (int i = start_loc; i < _size; i++)
    {
        if (_arr[i]->m_program == 0)
        {
            _arr[i]->m_program = info.m_program;
            _arr[i]->_shader_name = info._shader_name;
            _num_elements++;
            return;
        }
    }
    for (int i = 0; i < start_loc; i++)
    {
        if (_arr[i]->m_program == 0)
        {
            *(_arr[i]) = info;
            _num_elements++;
            return;
        }
    }
}

void ShaderHashMap::emplace(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name)
{
    if ((double)(_num_elements + 1) / (double)_size > ratio)
    {
        regenerate_array(get_next_mult_2(_size));
    }

    size_t start_loc = hash(shader_name) % _size;
    for (int i = start_loc; i < _size; i++)
    {
        if (_arr[i]->m_program == 0)
        {
            delete _arr[i];
            _arr[i] = new Shader(vertshaderfile, fragshaderfile, shader_name);
            _num_elements++;
            return;
        }
    }
    for (int i = 0; i < start_loc; i++)
    {
        if (_arr[i]->m_program == 0)
        {
            _arr[i]->LoadShader(vertshaderfile, fragshaderfile);
            _num_elements++;
            return;
        }
    }
}

void ShaderHashMap::erase(const std::string& key)
{
    size_t start_loc = hash(key) % _size;
    for (int i = start_loc; i < _size; i++)
    {
        if (_arr[i]->_shader_name == key)
        {
            _arr[i]->DeleteShader();
            delete _arr[i];
            _arr[i] = new Shader();
            _num_elements--;
            return;
        }
    }
    for (int i = 0; i < start_loc; i++)
    {
        if (_arr[i]->_shader_name == key)
        {
            _arr[i]->DeleteShader();
            delete _arr[i];
            _arr[i] = new Shader();
            _num_elements--;
            return;
        }
    }
}

Shader& LoadShader(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name)
{
    Shader shad(vertshaderfile, fragshaderfile, shader_name);
    Shaders::ShaderMap.emplace(shader_name, shad);
    return Shaders::ShaderMap[shader_name];
}
