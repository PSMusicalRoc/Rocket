#pragma once

const char* text_shader_vert_src = ""
"#version 330 core"
"layout (location = 0) in vec4 vertex;" //vec2 pos, vec2 tex
"out vec2 TexCoords"
""
"void main() {"
"  gl_Position = vec4(vertex.xy, 0.0, 1.0);"
"  TexCoords = vertex.zw;"
"}";

const char* text_shader_frag_src = ""
"#version 330 core"
"in vec2 TexCoords;"
"out vec4 Color"
""
"uniform sampler2D text;"
""
"void main() {"
"  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"
"  color = vec4(1.0, 1.0, 1.0, 1.0) * sampled;"
"}";
