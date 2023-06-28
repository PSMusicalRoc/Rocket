#ifndef _ROC_SHADER_GL_HPP_
#define _ROC_SHADER_GL_HPP_

#include <glad/gl.h>
#include <string>

class Shader
{
protected:
    GLuint m_program;

    GLuint LoadShader(const std::string& vertshader_file, const std::string& fragshader_file);

public:
    Shader(const std::string& vertshaderfile, const std::string& fragshaderfile);
    ~Shader();

    void DeleteShader() { delete this; }

    void Use() { glUseProgram(m_program); }
    bool SetUniformFloat(const std::string& uniform, float val);
    bool SetUniformInt(const std::string& uniform, int val);
    bool SetUniformBool(const std::string& uniform, bool val);
    bool SetUniformVec2(const std::string& uniform, float v1, float v2);
    bool SetUniformVec3(const std::string& uniform, float v1, float v2, float v3);
    bool SetUniformVec4(const std::string& uniform, float v1, float v2, float v3, float v4);
};

#endif