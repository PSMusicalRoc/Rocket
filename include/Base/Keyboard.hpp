/**
 * @file Keyboard.hpp
 * 
 * This file contains all keyboard definitions, including
 * which keys can be used, and which GLFW/strings they map
 * to.
 * 
 * This file also contains any and all functions relating
 * to getting input. Most useful is RocketKeyboard::IsKeyPressed(),
 * which can check, as its name suggest, whether a key is pressed
 * down or not.
 * 
 * @author Tim Bishop
*/

#pragma once

#include <GLFW/glfw3.h>

#include <boost/preprocessor.hpp>

#define ALL_KEYS    ENTER, A, B, C, D, E, F, G, H, I, J, K, L, M,\
                    N, O, P, Q, R, S, T, U, V, W, X, Y, Z, ESCAPE, BACKSPACE, SPACE,\
                    UP, DOWN, LEFT, RIGHT

#define NUM_KEYS    BOOST_PP_ADD(BOOST_PP_VARIADIC_SIZE(ALL_KEYS), 1)

#include <string>
#include <map>
#include <bitset>

enum class RocketKey {
    K_NULL = 0,
    ALL_KEYS
};

/**
 * @class RocketKeyboard
 * 
 * The main class to use for accessing the keyboard in any
 * part of the code. Checks whether keys are pressed or not
 * by utilizing the GLFW backend.
*/
struct RocketKeyboard {

    /**
     * Checks the key bitset for whether the selected
     * key is pressed down.
     * 
     * @param key The RocketKey to check the status of
     * 
     * @returns True if the key is pressed down, false otherwise.
    */
    static bool IsKeyPressed(RocketKey key);

    /**
     * Used in RScene loading, takes a string representation of
     * a key (ex, A) and maps it to its corresponding RocketKey
     * (ex, RocketKey::A)
     * 
     * @param str The string to parse the key from
     * 
     * @returns The key if it is found, or RocketKey::K_NULL if
     * the representation doesn't yield a correct key.
    */
    static RocketKey GetKeyFromStringRep(const std::string& str);

    /**
     * Callback specifically for GLFW - defined when an application is created.
     * Sets the bitset correctly depending on what happens in the window.
     * 
     * @param window The GLFWwindow* that should be queried for the inputs
     * @param key The GLFW keycode of the key
     * @param scancode ??
     * @param action The int representation of what the key actually did (ex.
     * GLFW_PRESS)
     * @param mods The mod keys that are pressed down
    */
    static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

private:

    /**
     * Conversions between GLFW and Rocket keycodes
    */
    static std::map<unsigned int, RocketKey> _conversion;

    /**
     * The internal bitset to use for checking key status
    */
    static std::bitset<NUM_KEYS> keys_pressed;

    /**
     * The map we use to get keys from string representations
    */
    static std::map<std::string, RocketKey> keys_from_strings;

    /**
     * Helper function for converting GLFW to RocketKey
     * 
     * @param glfwkey The GLFW keycode
     * 
     * @returns A valid RocketKey if the keycode is found, or
     * RocketKey::K_NULL if not.
    */
    static RocketKey ConvertToRocketKey(int glfwkey);
};
