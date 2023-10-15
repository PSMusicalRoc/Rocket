#pragma once

/**
 * @mainpage The Rocket Engine Documentation
 * 
 * @tableofcontents
 * 
 * @section welcome_sec Welcome!
 * 
 * @section building_sec Building the Engine
 * 
 * @section further_reading Further Reading
 * 
 * First, @ref application_layout "here is documentation for how an application should be laid out."
 * 
 * Additionally, @ref coordsystem "here is where one can learn about our Coordinate %System."
*/

/**
 * @page application_layout How a Rocket Application is laid out
 * 
 * @section test_sec Under construction...
*/

// Needed backends
#ifndef __gl_h_

#include <glad/gl.h>
#endif

#include <GLFW/glfw3.h>

// Base features

#include "Base/Application.hpp"
#include "Base/Keyboard.hpp"

// ECS

#include "Roc_ECS.h"

// OpenGL Additions

#include "Roc_GL/CoordinateSystem.hpp"
#include "Roc_GL/Shader.hpp"
#include "Roc_GL/Texture.hpp"

// RScene Language

#include "RScene/LoadRscene.hpp"

// Logger

#include "RocLogger/RocLogger.hpp"
