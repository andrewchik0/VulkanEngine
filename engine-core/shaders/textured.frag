#version 460

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 texCoord;

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
  vec4 screenSize;
} cameraData;

layout(set = 2, binding = 0) uniform ObjectData
{
  mat4 modelMatrix;
} objectData;

layout(set = 3, binding = 0) uniform sampler2D tex1;

void main()
{
  vec3 sunlight = normalize(vec3(1, 1, 0.5));
  vec3 color = texture(tex1, texCoord).xyz;
  
  color *= max(dot(sunlight, inNormal), 0.4);
  
  outFragColor = vec4(color, 1.0f);
}
