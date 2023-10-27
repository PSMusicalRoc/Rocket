#ifndef _ROC_SHADER_GL_HPP_
#define _ROC_SHADER_GL_HPP_

/**
 * @file Shader.hpp
 * 
 * This file details a wrapper class for the OpenGL
 * Shader. It can set uniform variables, run on command,
 * and be loaded in at runtime through the RScene
 * language.
 * 
 * @todo Maybe load in a default shader (currently "sprite_shader")
 * for users that don't want/need any custom shaders.
 *  
 * @author Tim Bishop
*/

#include <iostream>
#include <iomanip>

#include "RocLogger/RocLogger.hpp"
#ifndef __gl_h_

#include <glad/gl.h>
#endif
#include <string>

/**
 * @class Shader
 * 
 * This is the wrapper to the OpenGL Shader class. As
 * any OpenGL shader does, it requires a vertex and
 * fragment shader, which are files that can be loaded
 * in at runtime. Once loaded in by LoadShader(), they
 * are kept until their deletion in the Shaders holder
 * class.
 * 
 * Each shader is uniquely identified by a string key
 * inside the Shaders hash map. Additionally, at the end
 * of runtime, Shaders::clear() should be called to
 * avoid memory leaks.
 * 
 * @note No destructor is defined here so that we have
 * better control over when a Shader's contents become
 * void. If we had defined a destructor that instantly
 * unloaded the contained program, any accidental copy
 * of the Shader object that goes out of scope would
 * instantly nullify whatever OpenGL %Shader we wanted
 * to load.
 * 
 * @author Tim Bishop
*/
class Shader
{
    friend class ShaderHashMap;
    friend Shader LoadShaderFromMemory();

protected:
    /**
     * The program ID that represents the Shader in OpenGL.
    */
    GLuint m_program;

    /**
     * The helper function that loads a shader in, and logs
     * any errors it may encounter in doing so. It does NOT,
     * however, add the shader to the Shaders holder class,
     * instead operating exclusively on the Shader it is
     * called from. To correctly load a Shader into Shaders,
     * call ::LoadShader().
     * 
     * @param vertshader_file A string path relative to where
     * the binary is called from that represents where the
     * vertex shader source file is located.
     * 
     * @param fragshader_file A string path relative to where
     * the binary is called from that represents where the
     * fragment shader source file is located.
     * 
     * @return An unsigned integer representing the program
     * number of the given shader. If the return value is 0,
     * there were errors.
    */
    GLuint LoadShader(const std::string& vertshader_file, const std::string& fragshader_file);

    GLuint LoadShaderFromMemory(const std::string& vshad_source, const std::string& fshad_source);

    /**
     * The unique identifier of the Shader.
    */
    std::string _shader_name;

public:
    /**
     * Default constructor, needed for hash map support.
    */
    Shader();

    /**
     * Copy constructor, needed for hash map support.
     * 
     * @param shad The shader to copy parameters from.
    */
    Shader(const Shader& shad) { *this = shad; }

    /**
     * Constructor, loading the shader into memory, but
     * not loading it directly into Shaders.
     * 
     * @param vertshaderfile A string path relative to where
     * the binary is called from that represents where the
     * vertex shader source file is located.
     * 
     * @param fragshaderfile A string path relative to where
     * the binary is called from that represents where the
     * fragment shader source file is located.
     * 
     * @param shader_name The identifier to give the newly
     * created Shader.
    */
    Shader(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name, bool from_memory = false);


    /**
     * Overload for the assignment operator, needed for
     * hash map support
     * 
     * @param shad The shader to copy parameters from.
    */
    void operator=(const Shader& shad);

    /**
     * Our "Destructor" of sorts. When called on a Shader
     * object, the program inside it is removed from OpenGL
     * memory, and thus any copy of the Shader ceases to
     * function as well. This effectively destroys the
     * Shader.
    */
    void DeleteShader();

    /**
     * Call this function to set the active shader program
     * as the one defined inside the Shader object.
    */
    void Use() { glUseProgram(m_program); }

    /**
     * Used to set a uniform float value inside the Shader.
     * 
     * @param uniform The name of the uniform to assign a
     * value to.
     * 
     * @param val The value to assign.
    */
    bool SetUniformFloat(const std::string& uniform, float val);

    /**
     * Used to set a uniform integer value inside the Shader.
     * 
     * @param uniform The name of the uniform to assign a
     * value to.
     * 
     * @param val The value to assign.
    */
    bool SetUniformInt(const std::string& uniform, int val);

    /**
     * Used to set a uniform boolean value inside the Shader.
     * 
     * @param uniform The name of the uniform to assign a
     * value to.
     * 
     * @param val The value to assign.
    */
    bool SetUniformBool(const std::string& uniform, bool val);

    /**
     * Used to set a vector value inside the Shader. Note that
     * vectors are of floats and not integers, booleans, etc.
     * 
     * @param uniform The name of the uniform to assign a
     * value to.
     *
     * @param v1 The first value in the vector.
     * @param v2 The second value in the vector.
    */
    bool SetUniformVec2(const std::string& uniform, float v1, float v2);

    /**
     * Used to set a vector value inside the Shader. Note that
     * vectors are of floats and not integers, booleans, etc.
     * 
     * @param uniform The name of the uniform to assign a
     * value to.
     *
     * @param v1 The first value in the vector.
     * @param v2 The second value in the vector.
     * @param v3 The third value in the vector.
    */
    bool SetUniformVec3(const std::string& uniform, float v1, float v2, float v3);

    /**
     * Used to set a vector value inside the Shader. Note that
     * vectors are of floats and not integers, booleans, etc.
     * 
     * @param uniform The name of the uniform to assign a
     * value to.
     *
     * @param v1 The first value in the vector.
     * @param v2 The second value in the vector.
     * @param v3 The third value in the vector.
     * @param v4 The fourth value in the vector.
    */
    bool SetUniformVec4(const std::string& uniform, float v1, float v2, float v3, float v4);
};


/**
 * @class ShaderHashMap
 * 
 * A custom hash map intended to optimally store and
 * access Shader objects stored within. It will never
 * be over 70% full, and only increases in size to
 * powers of 2. Additionally, the hash map does not
 * use linked lists, and if two hashes collide, the
 * newer of the two will be pushed to the closest
 * available space <i>ahead</i> of where it would usually
 * have been.
 * 
 * The actual hash map relies on the has function defined,
 * which can be read about in the documentation of
 * ShaderHashMap::hash().
*/
class ShaderHashMap
{
protected:
    typedef long unsigned int size_t;

    /** The actual array in which Shaders are stored. */
    Shader* _arr = nullptr;
    /** The current size (not the number of elements stored) of the map. */
    size_t _size = 0;
    /** The current number of elements stored in the map. */
    size_t _num_elements = 0;

    /** The percentage to which the hash map cannot be more full than. */
    double ratio = 0.7;

    /**
     * A helper function that returns the next power of 2
     * higher than the input value.
     * 
     * @param inval The number to compare against.
     * 
     * @return The next highest power of 2 above inval.
    */
    int get_next_mult_2(int inval);

    /**
     * The function that actually regenerates the array.
     * It first calculates the next size higher than the
     * current size with get_next_mult_2(). Then, it moves
     * the current array pointer to a temporary value, and
     * sets the internal array pointer to a new array of the
     * new size. Finally, we copy any Shaders in the old
     * array that aren't null to the new array via
     * ShaderHashMap::emplace(), then destroying the old
     * array via the delete keyword.
    */
    void regenerate_array();

public:

    /**
     * Constructor for ShaderHashMap
     * 
     * @param initial_size An initial size for the map. If
     * not provided or provided size is <1, the default size
     * is 4.
    */
    ShaderHashMap(size_t initial_size = 0);
    
    /**
     * Function responsible for deleting the hash map.
    */
    void DeleteHashMap();

    /**
     * Destructor for ShaderHashMap - Calls ShaderHashMap::DeleteHashMap().
    */
    ~ShaderHashMap() { DeleteHashMap(); }


    /**
     * Returns a reference to the shader at the provided key.
     * 
     * @param key The key to search the hash map for
     * 
     * @returns A reference to the given Shader object.
    */
    Shader& at(const std::string& key);

    /**
     * Returns a reference to the shader at the provided key.
     * 
     * @param key The key to search the hash map for
     * 
     * @returns A reference to the given Shader object.
    */
    Shader& operator[](const std::string& key) { return at(key); }

    /**
     * The hash function for the map. The algorithm is
     * fairly simple: Loop through every character in the
     * key, keeping a counter `i` of what character we're on.
     * Then add (10^i) * int(character) to the total.
     * 
     * @param key The key to hash.
     * 
     * @returns The hash value, which should be modulo'ed
     * by the size of the hash map.
    */
    size_t hash(const std::string& key);


    /**
     * Adds an already created shader to the Hash map. The
     * key is stored inside the shader already, so asking for
     * it is redundant.
     * 
     * @param shader The shader to emplace inside the map.
    */
    void emplace(const Shader& shader);

    /**
     * Adds a new Shader (one not already created) to the
     * hash map. Because the Shader is being created in place,
     * the key is needed so we know where to put it.
     * 
     * @param vertshaderfile A string path relative to where
     * the binary is called from that represents where the
     * vertex shader source file is located.
     * 
     * @param fragshaderfile A string path relative to where
     * the binary is called from that represents where the
     * fragment shader source file is located.
     * 
     * @param shader_name The identifier to give the newly
     * created Shader.
    */
    void emplace(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name);

    /**
     * Erases the Shader at `key`, provided it exists.
     * 
     * @param key The key to search for.
    */
    void erase(const std::string& key);
};


/**
 * @class Shaders
 * 
 * A holder class for the static ShaderHashMap.
 * 
 * @todo Add direct accessor to the class so we
 * don't have to write Shaders::ShaderMap[key].
 * 
 * @author Tim Bishop
*/
class Shaders
{
public:
    /** The static ShaderHashMap object. */
    static ShaderHashMap ShaderMap;

    /**
     * Clears the entire map, unloading every currently
     * loaded Shader.
    */
    static void clear() { ShaderMap.DeleteHashMap(); }
};

/**
 * This is how one should create a Shader object.
 * It loads the Shader, emplaces it inside the wrapper
 * object Shaders, then returns it in case the user needs
 * it immediately.
 * 
 * @param vertshaderfile A string path relative to where
 * the binary is called from that represents where the
 * vertex shader source file is located.
 * 
 * @param fragshaderfile A string path relative to where
 * the binary is called from that represents where the
 * fragment shader source file is located.
 * 
 * @param shader_name The identifier to give the newly
 * created Shader.
 * 
 * @returns A reference to the created Shader object.
 * If there are errors in usage, check the logs, where
 * any errors will show.
 */
Shader& LoadShader(const std::string& vertshaderfile, const std::string& fragshaderfile, const std::string& shader_name);

Shader& LoadShaderFromMemory(const std::string& vertshader, const std::string& fragshader, const std::string& shader_name);

#endif