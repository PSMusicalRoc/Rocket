/**
 * @file LoadRscene.hpp
 * 
 * This is the file responsible for loading scenes into the engine.
 * It works entirely dynamically, assuming that any components
 * <ol>
 *  <li>Are initialized before the call</li>
 *  <li>Are defined with public setters in the lambda map _setters</li>
 *  <li>Have a static member function `name()` that return the name of the component</li>
 * </ol>
 * 
 * This is all achieved through the macros defined in
 * @link Component.h @endlink
 * 
 * @todo Come up with a better name than RScene... Current frontrunners are
 * "Liftoff" and "Arsene"
 * 
 * @todo Create a custom doxygen page detailing the Rscene syntax.
 * 
 * @author Tim Bishop
*/

#pragma once

#include <ECS/Roc_ECS_Additions.hpp>
#include <Roc_GL/CoordinateSystem.hpp>
#include <Roc_GL/Shader.hpp>
#include <Roc_GL/Texture.hpp>

/**
 * Call this function to load a scene into the game engine. Ensure
 * that the format of the scene file is correct, or there will be errors.
 * 
 * @todo Add more thorough error checking for syntax
 * 
 * @param filepath The relative path to the scene file to load
*/
void LoadScene(const std::string& filepath);