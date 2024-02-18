#version 460

layout (location = 0) in vec2 texCoord;
layout (location = 0) out vec4 outFragColor;

layout(set = 3, binding = 0) uniform sampler2D tex;

void main()
{
  outFragColor = texture(tex, texCoord).xyzw;
}

