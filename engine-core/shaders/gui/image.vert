#version 460

layout (location = 0) in vec2 vPosition;
layout (location = 1) in vec2 vTexCoord;

layout (location = 0) out vec2 texCoord;

layout(set = 1, binding = 0) uniform CameraData
{
  mat4 view;
  mat4 proj;
  mat4 viewproj;
  vec4 pos;
  vec4 lookAt;
  vec4 up;
  vec4 screenSize;
} cameraData;

void main()
{
  
  gl_Position = vec4(vPosition / cameraData.screenSize.xy * 2.0f - 1.0f, 0.0, 1.0f);
  texCoord = vTexCoord;
}
