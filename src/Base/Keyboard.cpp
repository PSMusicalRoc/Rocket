#include "Base/Keyboard.hpp"

#include <boost/preprocessor/slot/counter.hpp>

#define INIT_KEY_PAIR(z, n, data) {BOOST_PP_CAT(GLFW_KEY_, BOOST_PP_SEQ_ELEM(n, data)), RocketKey::BOOST_PP_SEQ_ELEM(n, data)}\
BOOST_PP_COMMA_IF(BOOST_PP_SUB(BOOST_PP_SUB(NUM_KEYS, 1), n))

#define INIT_STRING_PAIR(z, n, data) {BOOST_PP_STRINGIZE(BOOST_PP_SEQ_ELEM(n, data)), RocketKey::BOOST_PP_SEQ_ELEM(n, data)}\
BOOST_PP_COMMA_IF(BOOST_PP_SUB(BOOST_PP_SUB(NUM_KEYS, 1), n))

#define INIT_KEY_MAP BOOST_PP_REPEAT(BOOST_PP_SUB(NUM_KEYS, 1), INIT_KEY_PAIR, BOOST_PP_VARIADIC_TO_SEQ(ALL_KEYS))

#define INIT_STRING_MAP BOOST_PP_REPEAT(BOOST_PP_SUB(NUM_KEYS, 1), INIT_STRING_PAIR, BOOST_PP_VARIADIC_TO_SEQ(ALL_KEYS))


/**
 * REAL CODE FROM HERE
*/


std::map<unsigned int, RocketKey> RocketKeyboard::_conversion = {INIT_KEY_MAP};

std::bitset<NUM_KEYS> RocketKeyboard::keys_pressed;

std::map<std::string, RocketKey> RocketKeyboard::keys_from_strings = {INIT_STRING_MAP};



bool RocketKeyboard::IsKeyPressed(RocketKey key)
{
    return keys_pressed[(unsigned int)key];
}

RocketKey RocketKeyboard::GetKeyFromStringRep(const std::string& str)
{
    if (keys_from_strings.find(str) == keys_from_strings.end())
        return RocketKey::K_NULL;
    return keys_from_strings[str];
}

void RocketKeyboard::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        RocketKey k = ConvertToRocketKey(key);
        if (k != RocketKey::K_NULL)
        {
            keys_pressed[(unsigned int)k] = true;
        }
    }
    if (action == GLFW_RELEASE)
    {
        RocketKey k = ConvertToRocketKey(key);
        if (k != RocketKey::K_NULL)
        {
            keys_pressed[(unsigned int)k] = false;
        }
    }
}

RocketKey RocketKeyboard::ConvertToRocketKey(int glfwkey)
{
    if (_conversion.find(glfwkey) == _conversion.end())
        return RocketKey::K_NULL;
    return _conversion[glfwkey];
}



#undef INIT_KEY_MAP
#undef INIT_KEY_PAIR
#undef INIT_STRING_MAP
#undef INIT_STRING_PAIR
