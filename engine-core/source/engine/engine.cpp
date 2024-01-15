#include "engine.h"

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
    
    _bIsInitialized = true;
  }
  
  void Engine::cleanup()
  {
    if (!_bIsInitialized)
      return;
    
    _render.cleanup();
    _window.get()->cleanup();
    Window::delete_window(_window);
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
    _fileWatchers.tick();
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
    std::vector<Vertex> vertices(3);
    
    vertices[0].position = {  1.f,  1.f, 0.0f };
    vertices[1].position = { -1.f,  1.f, 0.0f };
    vertices[2].position = {  0.f, -1.f, 0.0f };
    
    vertices[0].color = { 0.f, 1.f, 0.0f };
    vertices[1].color = { 0.f, 0.f, 1.0f };
    vertices[2].color = { 1.f, 0.f, 0.0f };
    
    auto path = std::filesystem::current_path();

    _render._meshes.create("monkey", "assets/monkey_smooth.obj");
    _render._meshes.create("empire", "assets/lost_empire.obj");
    _render._meshes.create("triangle", vertices);
    
    std::vector<ShaderData> defaultData =
    {
      { "tri_mesh_ssbo", SHADER_VERTEX },
      { "default_lit", SHADER_FRAGMENT },
    };
    std::vector<ShaderData> texturedData =
    {
      { "tri_mesh_ssbo", SHADER_VERTEX },
      { "textured_lit", SHADER_FRAGMENT },
    };
    _render._pipelines.create("default", defaultData);
    _render._pipelines.create("textured", texturedData);
    
    _render._materials.create("defaultmesh", _render._pipelines["default"]);
    _render._materials.create("texturedmesh", _render._pipelines["textured"]);
    
    _render._textures.create("empire_diffuse", "assets/lost_empire-RGBA.png");
    _render._materials["texturedmesh"]->texture = _render._textures["empire_diffuse"];
    
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
    
    for (int32_t x = -20; x <= 20; x++) {
      for (int32_t y = -20; y <= 20; y++) {
        
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
  
}
