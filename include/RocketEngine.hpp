#pragma once

/**
 * @mainpage The Rocket Engine Documentation
 * 
 * @tableofcontents
 * 
 * @section welcome_sec Welcome!
 * 
 * Welcome to the Rocket Game Engine! This is an engine built from scratch
 * by Tim Bishop to facilitate learning and growth, as well as satiate a
 * primal urge to code just about anything.
 * 
 * The engine is built using an Entity-Component %System. For those not familiar,
 * here are the general rules the Engine follows:
 * <ul>
 *  <li>Any "thing" in the game world is an Entity</li>
 *  <li>Any data values a "thing" can have are a part of one or
 * more @ref Component "Components"</li>
 *  <li>Any functionality a "thing" can have is a System, which looks
 * for Entities with specific components.</li>
 * </ul>
 * 
 * For example, a basic Character could be defined like this (note, the language
 * used here is the Rscene language, another part of the Rocket Engine.)
 * 
 * ```
 * Entity "character" {
 *     Transform {
 *         double x = 5.0;
 *         double y = 5.0;
 *     }
 *     Sprite {
 *         string texture_key = "some_tex";
 *         string shader_key = "some_shader";
 *         double width = 5.0;
 *         double height = 5.0;
 *     }
 *     RectangleCollider {
 *         double width = 5.0;
 *         double height = 5.0;
 *     }
 *     CharacterMovement {
 *         key left = LEFT;
 *         key right = RIGHT;
 *         double runspeed = 15.0;
 *     }
 * }
 * ```
 * 
 * The Entity named "character" has 4 components attached to it: A Transform, a Sprite,
 * a RectangleCollider, and a CharacterMovement. Transform, Sprite, and RectangleCollider
 * are all examples of Components that ship with the engine, but CharacterMovement is a
 * custom created Component.
 * 
 * These hypothetical components could be wrapped together with a System. For more information
 * about Systems, check out this.
 * 
 * @todo Add System page
 * 
 * @section building_sec Building the Engine
 * 
 * Building the engine is fairly simple. It requires a few prerequisites, though:
 * - Git
 * - Premake (version 5 is the latest at the time of writing, and the one supported
 * by the engine)
 * - Doxygen (for automatically creating documentation)
 * - C++ compilation support (for your operating system, that could be very different)
 * - Makefile support (again, varies by operating system)
 * 
 * @subsection compiling_linux Compiling on Linux
 * 
 * - First, clone the repository:
 * ```
 * git clone https://github.com/Rocket-Game-Engine/Rocket.git --recursive
 * ```
 * - Run the following to create a Makefile for the project:
 * ```
 * premake5 gmake2
 * ```
 * - If you're building a Debug build:
 * ```
 * make
 * ```
 * - If you're building a Release build:
 * ```
 * make config=release
 * ```
 * 
 * These steps should build you the Engine, the example pong clone I'm making, and the documentation.
 * 
 * @section further_reading Further Reading
 * 
 * First, @ref application_layout "here is documentation for how an application should be laid out."
 * 
 * Additionally, @ref coordsystem "here is where one can learn about our Coordinate System."
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
