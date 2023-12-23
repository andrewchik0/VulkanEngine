#version 460

layout (location = 0) in vec3 inColor;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 inNormal;
layout (location = 3) in vec3 inPosition;

layout (location = 0) out vec4 outFragColor;

layout(set = 0, binding = 0) uniform SceneData
{
  vec4 fogColorExponent;
  vec4 fogDistancesMinMax;
  vec4 ambientColor;
  vec4 sunlightDirectionPower;
  vec4 sunlightColor;
} sceneData;

layout(set = 1, binding = 0) uniform CameraData
{
  mat4 view;
  mat4 proj;
  mat4 viewproj;
  vec4 pos;
  vec4 lookAt;
  vec4 up;
} cameraData;


void main()
{
  vec3 sunlight = normalize(vec3(1, 1, 0.5));
  vec3 color = inColor;
    
  color *= max(dot(sunlight, inNormal), 0.1);
  
  outFragColor = vec4(color, 1.0f);
}
