#version 460

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec3 vColor;
layout (location = 3) in vec2 vTexCoord;

layout (location = 0) out vec3 outColor;
layout (location = 1) out vec2 texCoord;
layout (location = 2) out vec3 outNormal;
layout (location = 3) out vec3 outPosition;

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
  
  float scale = 1.0;//(gl_BaseInstance + 1);
  position = vec4(position.x * scale, position.y * scale, position.z * scale, 1.0);
  
  gl_Position = cameraData.viewproj * position;
  outColor = vColor;
  texCoord = vTexCoord;
  outNormal = vNormal;
  outPosition = vPosition;
}
