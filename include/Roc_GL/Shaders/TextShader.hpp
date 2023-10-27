#pragma once

const char* text_shader_vert_src = "\n"
"#version 330 core\n"
"layout (location = 0) in vec3 vertex;\n"
"layout (location = 1) in vec2 texcoord;\n"
"out vec2 TexCoords;\n"
"\n"
"void main() {\n"
"  gl_Position = vec4(vertex.xyz, 1.0);\n"
"  TexCoords = texcoord;\n"
"}";

const char* text_shader_frag_src = "\n"
"#version 330 core\n"
"in vec2 TexCoords;\n"
"out vec4 color;\n"
"\n"
"uniform sampler2D text;\n"
"\n"
"void main() {\n"
"  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);\n"
"  color = vec4(1.0, 1.0, 1.0, 1.0) * sampled;\n"
"}";
