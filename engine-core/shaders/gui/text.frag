#version 460

layout (location = 0) in vec2 texCoord;
layout (location = 0) out vec4 outFragColor;

layout(set = 3, binding = 0) uniform sampler2D tex;

void main()
{
  vec4 color = texture(tex, texCoord).xxxx;
  color.xyz = color.xyz * 1024;
  float gamma = 2.2;
  outFragColor = pow(color, vec4(1.0 / gamma));
}

