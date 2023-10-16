#pragma once



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
