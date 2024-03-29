#include "engine/engine.h"

namespace VKEngine {
  
  Engine* Engine::s_instance = nullptr;
  
  void Engine::init(ApplicationSpecs specs)
  {
    s_instance = this;
    
    _window = Window::create_window();
    _window.get()->init(specs, [&](){ draw(); }, [&](){ recreate(); });
    _camera.init(this, _window.get()->width(), _window.get()->height());
    _render.init(&_window.get()->width(), &_window.get()->height());
    
    init_scene();
    
    {
      using namespace std::chrono_literals;
      _fileWatchers.set_delay(200ms);
    }
    _fileWatchers.run_on_separate_thread();
    
    _bIsInitialized = true;
  }
  
  void Engine::cleanup()
  {
    if (!_bIsInitialized)
      return;
    
    _render.cleanup();
    _window.get()->cleanup();
    Window::delete_window(_window);
    _fileWatchers.terminate();
  }
  
  void Engine::recreate()
  {
    if (!_bIsInitialized)
      return;

    _render.recreate();
    _camera.init(_window.get()->width(), _window.get()->height());
  }
  
  void Engine::draw()
  {
    _camera.handle();
    
    _render.draw();
    
    _timeHandler.tick();
  }
  
  void Engine::run()
  {
    _window.get()->run();
  }
  
  void Engine::init_scene()
  {
    _render._fontHandler.load_font("assets/Consolas.ttf", 30);
    _render._fontHandler.set_current_font("assets/ARIAL.TTF", 18);
    
    RawTexture tex = RawTexture::plain(0x88000000);
    GUIImageCreateInfo info;
    info.pos.y1 = 20;
    info.pos.x2 = 20;
    info.width = 600;
    info.height = 600;
    info.textureBuf = &tex;
    _render._gui.image(info);
    
    GUITextCreateInfo textInfo;
    textInfo.pos.y1 = 50;
    textInfo.pos.x2 = 570;
    textInfo.text = "AVI\nThis is a font testing\nHello123 ! ^&()&^";
    _render._gui.text(textInfo);
    
    _render._frames.set_clear_value({ 0.4, 0.5, 1, 1 });
    Vertices vertices;
    
    vertices.positions3f =
    {
      {  1.f,  1.f, 0.0f },
      { -1.f,  1.f, 0.0f },
      {  0.f, -1.f, 0.0f },
    };
    
    vertices.colors =
    {
      { 0.f, 1.f, 0.0f },
      { 0.f, 0.f, 1.0f },
      { 1.f, 0.f, 0.0f },
    };
    
    vertices.normals =
    {
      { 1.f, 1.f, 1.0f },
      { 0.f, 0.f, 1.0f },
      { 1.f, 0.f, 0.0f },
    };
    
    VertexInputDescription vertexTexturedDesc = VertexInputDescriptionBuilder()
      .float3()
      .float3()
      .float2()
      .build();
    
    std::vector<ShaderInfo> defaultInfo =
    {
      { "default", SHADER_VERTEX, vertices.build_vertex_desc() },
      { "default", SHADER_FRAGMENT },
    };
    std::vector<ShaderInfo> texturedInfo =
    {
      { "textured", SHADER_VERTEX, vertexTexturedDesc },
      { "textured", SHADER_FRAGMENT },
    };
    _render._pipelines.create("default", defaultInfo, 0);
    _render._pipelines.create("textured", texturedInfo, 0);
    
    _render._materials.create("defaultmesh", _render._pipelines["default"]);
    _render._materials.create("texturedmesh", _render._pipelines["textured"]);
    
    _render._textures.create_from_file("empire_diffuse", "assets/lost_empire-RGBA.png");
    _render._materials["texturedmesh"]->texture = _render._textures["empire_diffuse"];
    
    _render._meshes.create("monkey", "assets/monkey_smooth.obj");
    _render._meshes.create("empire", "assets/lost_empire.obj", true);
    _render._meshes.create("triangle", vertices);
    
    RenderObject monkey;
    monkey.mesh = _render._meshes["monkey"];
    monkey.material = _render._materials["defaultmesh"];
    monkey.transformMatrix = glm::scale(glm::vec3(2.0));
    
    _render.add_object(monkey);
    
    RenderObject map;
    map.mesh = _render._meshes["empire"];
    map.material = _render._materials["texturedmesh"];
    map.transformMatrix = glm::translate(glm::vec3{ 5, -10, 0 });
    map.transformMatrix = glm::transpose(glm::scale(glm::vec3(1.0)));
    
    _render.add_object(map);
    
    for (int32_t x = -20; x <= 20; x++)
      for (int32_t y = -20; y <= 20; y++)
      {
        RenderObject tri;
        tri.mesh = _render._meshes["triangle"];
        tri.material = _render._materials["defaultmesh"];
        glm::mat4 rotate = glm::rotate(abs(y) / 20.0f + abs(x) / 20.0f, glm::vec3(x / 20.0, 0, y / 20.0));
        glm::mat4 translation = glm::translate(glm::mat4{ 1.0 }, glm::vec3(x / 5.0, 0, y / 5.0));
        glm::mat4 scale = glm::scale(glm::mat4{ 1.0 }, glm::vec3(0.2, 0.2, 0.2));
        tri.transformMatrix = translation * scale * rotate;
        
        _render.add_object(tri);
      }
  }
  
}
