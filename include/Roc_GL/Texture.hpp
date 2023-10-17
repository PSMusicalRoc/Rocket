#pragma once

/**
 * @file Texture.hpp
 * 
 * This file contains all the classes and functions revolving around
 * textures. The end user mostly won't work with textures except
 * through loading them via Rscene and accessing the TextureInfo
 * of textures. Outside of that, the Sprite component usually
 * takes care of the rendering automatically.
 * 
 * @author Tim Bishop
*/

#ifndef __gl_h_

#include <glad/gl.h>
#endif
#include <string>
#include <map>
#include <cmath>
#include "RocLogger/RocLogger.hpp"

#include <stb_image.h>

/**
 * @struct TextureInfo
 * 
 * This is a basic structure containing all the data
 * values a texture would need to access to render
 * itself. These should be created via ::LoadTexture().\
*/
struct TextureInfo
{
    /** The OpenGL index of the loaded texture */
    GLuint tex_id = 0;
    /** The width of the loaded texture */
    int width = 0;
    /** The height of the loaded texture */
    int height = 0;
    /** The unique string key that identifies the texture to the engine */
    std::string tex_key = "";
};

/**
 * Takes a reference to a TextureInfo struct,
 * unloads the attached OpenGL texture, and reverts
 * all of its data members back to their defaults,
 * effectively clearing the texture from the engine.
 * 
 * @param info The TextureInfo object to nullify
*/
void ClearTexture(TextureInfo& info);


/**
 * @class TexInfoHash
 * 
 * @todo Remove this class, it's never used
 * and now serves no purpose.
*/
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

/**
 * @class TextureHashMap
 * 
 * This is a custom hash map created explicitly to hold
 * TextureInfo objects for future use.
*/
class TextureHashMap
{
protected:
    typedef long unsigned int size_t;

    /** The TextureInfo objects */
    TextureInfo* _arr = nullptr;
    /** The size (not the number of objects) of the array */
    size_t _size = 0;
    /** The number of non-null TextureInfo objects in the map */
    size_t _num_elements = 0;

    /** The ratio of fullness the map should never be more than. */
    double ratio = 0.7;

    /**
     * A helper function that returns the next highest
     * power of 2 above the given value.
     * 
     * @param inval The value to compare to
     * 
     * @returns The next highest power of 2
    */
    int get_next_mult_2(int inval);

    /**
     * The method that recreates the underlying array.
     * This is done by creating a new array, setting it
     * as _arr, and emplacing every object from the old
     * array into the new one.
    */
    void regenerate_array();

public:
    /**
     * Default constructor for TextureHashMap,
     * if initial_size < 1, the size will be
     * set to 4.
     * 
     * @param initial_size The size of the array we
     * should allocate.
    */
    TextureHashMap(size_t initial_size = 0);

    /**
     * The function that clears the hash map and
     * invalidates it.
    */
    void DeleteHashMap();
    
    /**
     * Destructor for TextureHashMap
    */
    ~TextureHashMap() { DeleteHashMap(); }


    /**
     * Returns the TextureInfo object found
     * at the key index of the map. If the key
     * does not match the object's tex_key, then
     * we continue onto the next one until wey
     * run out of objects to check.
     * 
     * @param key The index of the TextureInfo object
     * to retrieve
     * 
     * @returns The TextureInfo object, or a default
     * TextureInfo object should we not find the
     * object with the correct key.
    */
    TextureInfo at(const std::string& key);

    /**
     * Returns the TextureInfo object found
     * at the key index of the map. If the key
     * does not match the object's tex_key, then
     * we continue onto the next one until wey
     * run out of objects to check.
     * 
     * @param key The index of the TextureInfo object
     * to retrieve
     * 
     * @returns The TextureInfo object, or a default
     * TextureInfo object should we not find the
     * object with the correct key.
    */
    TextureInfo operator[](const std::string& key) { return at(key); }


    /**
     * Places the TextureInfo object into the hashmap.
     * Since the key is a part of the struct, asking
     * for it as a parameter is unneeded.
     * 
     * @param info The TextureInfo struct to place inside
     * the map
    */
    void emplace(TextureInfo info);

    /**
     * This function erases the TextureInfo object
     * at the key via ::ClearTexture().
     * 
     * @param key The key of the TextureInfo object
     * to delete.
    */
    void erase(const std::string& key);


    /**
     * Debug Function, do not use
     * 
     * @todo remove this, or lock it behind a define
    */
    void PrintHashMap();
};


/**
 * @class Textures
 * 
 * This is a wrapper class for a static TextureHashMap. This
 * is where any textures loaded will end up being stored
 * for further use.
 * 
 * @todo Add a way to directly access the TextureInfo objects
 * inside TextureMap so we don't have to constantly write out
 * "Textures::TextureMap".
*/
class Textures
{
public:
    /** The TextureHashMap where all textures will be stored. */
    static TextureHashMap TextureMap;
    /**
     * Clears the contents of the internal hashmap,
     * effectively unloading every texture known
     * to the engine.
    */
    static void clear() { TextureMap.DeleteHashMap(); }
};

std::ostream& operator<<(std::ostream&, const TextureInfo& info);


/**
 * This is how the user is intended to load a texture into
 * Rocket. It will be loaded from the filename (a path
 * relative to the location the app was called from) and
 * store the generated TextureInfo object into
 * Textures::TextureMap. Then, it will return the TextureInfo
 * object.
 * 
 * @param filename A relative path detailing where to find the
 * texture the engine should load.
 * @param texture_key The unique identifier of the texture to
 * load.
 * 
 * @returns The newly generated TextureInfo object
*/
TextureInfo LoadTexture(const std::string& filename, const std::string& texture_key);


/** @todo why is this redefinition here?? */
void ClearTexture(TextureInfo& info);
