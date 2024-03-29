#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexCoord;

layout (location = 0) out vec3 outPosition;
layout (location = 1) out vec3 outNormal;
layout (location = 2) out vec2 texCoord;

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
} cameraData;

struct ObjectData
{
  mat4 model;
};

layout(std140, set = 2, binding = 0) readonly buffer ObjectBuffer
{
  ObjectData objects[];
} objectBuffer;

void main()
{
  vec4 position = objectBuffer.objects[gl_BaseInstance].model * vec4(vPosition, 1.0f);
  
  gl_Position = cameraData.viewproj * position;
  outPosition = vPosition;
  outNormal = vNormal;
  texCoord = vTexCoord;
}
