#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <vector>
#include "Shapes.h"
#include "Color.h"
#include "Settings.h"

class RayTracer {
private:
  Ray primary_ray;
  Color background_color = TURQUOISE;
  float ambient_light_intensity = AMBIENT_LIGHT_INTENSITY;
  int max_depth = 5;

  Color trace(const std::vector<Sphere>& shapes, const Ray& incidentRay, int depth, const Sphere* curShape);

public:
  RayTracer(const Ray& t_primary) : primary_ray{ t_primary } {}
  Color trace(const std::vector<Sphere>& shapes);
};



#endif
