#include "Scene.h"
#include "Shapes.h"

std::vector<std::unique_ptr<Shape>> Scene::getFrame()
{
  // Assembles a room with a bunch of crap in it
  Sphere sphere{ vec{-4.f, 0.f, -4.f}, MIRRORISH, 1.f };
  Sphere sphere2{ vec{2.5f, -2.f, -15.f}, DULL, 0.75f };
  Sphere sphere3{ vec{2.5f, -2.f, -10.f}, BRIGHT, 0.75f };
  Sphere sphere4{ vec{2.5f, -2.f, -5.f}, BRIGHT, 0.75f };
  Sphere sphere5{ vec{-2.5f, -2.f, -2.f}, BRIGHT, 1.f };
  Sphere light_bulb2{ vec{-2.f, 3.75f, -3.f}, LIGHT, 0.25f };
  Rect rect{ vec{1.f, 2.f, -6.f}, vec{1.5f, 2.5f, -6.5f}, vec{0.5f, 2.5f, -6.5f}, BRIGHT };
  Rect rect2{ vec{1.f, 2.f, -6.f}, vec{0.5f, 2.5f, -6.5f}, vec{1.f, 1.f, -6.f}, BRIGHT };
  Rect rect3{ vec{1.f, 2.f, -6.f}, vec{1.f, 1.f, -6.f}, vec{1.5f, 2.5f, -6.5f}, BRIGHT };
  Rect rect4{ vec{1.f, 1.f, -6.f}, vec{0.5f, 1.5f, -6.5f}, vec{1.5f, 1.5f, -6.5f}, BRIGHT };
  Rect rect5{ vec{1.f, 2.f, -7.f}, vec{0.5f, 1.5f, -6.5f}, vec{1.f, 3.f, -7.f}, BRIGHT };
  Rect rect6{ vec{1.f, 2.f, -7.f}, vec{1.f, 3.f, -7.f}, vec{1.5f, 1.5f, -6.5f}, BRIGHT };
  Rect floor{ vec{-10.f, -4.f, -20.f}, vec{-10.f, -4.f, 0.1f}, vec{10.f, -4.f, -20.f}, GRAYISH };
  Rect wall1{ vec{-10.f, -4.f, -20.f}, vec{-10.f, 4.f, -20.f}, vec{-10.f, -4.f, 0.f}, YELLOWISH };
  Rect wall2{ vec{10.f, -4.f, 0.1f}, vec{10.f, 4.f, 0.1f}, vec{10.f, -4.f, -20.f}, YELLOWISH };
  Rect wall3{ vec{10.f, -4.f, -20.f}, vec{10.f, 4.f, -20.f}, vec{-10.f, -4.f, -20.f}, YELLOWISH };
  Rect wall4{ vec{-10.f, -4.f, 0.1f}, vec{-10.f, 4.f, 0.1f}, vec{10.f, -4.f, 0.1f}, YELLOWISH };
  Rect cieling{ vec{-10.f, 4.f, -20.f}, vec{10.f, 4.f, -20.f}, vec{-10.f, 4.f, 0.1f}, WHITEISH };
  Rect mirror{ vec{5.f, -3.f, -19.9f}, vec{5.f, 3.f, -19.9f}, vec{-5.f, -3.f, -19.9f}, MIRRORISH };
  std::vector<std::unique_ptr<Shape>> shapes;
  shapes.push_back(std::make_unique<Sphere>(sphere));
  shapes.push_back(std::make_unique<Sphere>(sphere2));
  shapes.push_back(std::make_unique<Sphere>(sphere3));
  shapes.push_back(std::make_unique<Sphere>(sphere4));
  shapes.push_back(std::make_unique<Sphere>(sphere5));
  shapes.push_back(std::make_unique<Sphere>(light_bulb2));
  shapes.push_back(std::make_unique<Rect>(rect));
  shapes.push_back(std::make_unique<Rect>(rect2));
  shapes.push_back(std::make_unique<Rect>(rect3));
  shapes.push_back(std::make_unique<Rect>(rect4));
  shapes.push_back(std::make_unique<Rect>(rect5));
  shapes.push_back(std::make_unique<Rect>(rect6));
  shapes.push_back(std::make_unique<Rect>(floor));
  shapes.push_back(std::make_unique<Rect>(wall1));
  shapes.push_back(std::make_unique<Rect>(wall2));
  shapes.push_back(std::make_unique<Rect>(wall3));
  shapes.push_back(std::make_unique<Rect>(wall4));
  shapes.push_back(std::make_unique<Rect>(cieling));
  shapes.push_back(std::make_unique<Rect>(mirror));
  return shapes;
}
