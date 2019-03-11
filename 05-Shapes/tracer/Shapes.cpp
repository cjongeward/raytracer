#include "Shapes.h"

std::optional<ReflectionData> intersects(const Ray & incident_ray, const Shape& other)
{
  return other.intersects_with(incident_ray);
}

std::optional<ReflectionData> intersects(const Sphere& sphere, const Ray& incident_ray) {
  auto v_ray2sph_center = sphere.pos - incident_ray.pos;
  auto v_r2s_proj_ray = v_ray2sph_center.dot(incident_ray.dir); // projection of ray_origin_to_sphere onto ray
  if (v_r2s_proj_ray < 0.f) {  // if sphere is behind ray origin
    return std::nullopt;
  }
  auto v_ray2sph_bis = incident_ray.dir * v_r2s_proj_ray; // vector from ray origin to midway through the sphere along the ray
  auto v_min_dist = v_ray2sph_bis - v_ray2sph_center; // vector from sphere center to closest point on the ray 
  float d2 = v_min_dist.mag2();
  float r2 = sphere.radius*sphere.radius; 
  if (d2 > r2) { // if min distance between ray and sphere is greater than radius, then no intersection
    return std::nullopt;
  }
  float cd2 = r2 - d2; // ratio of v_ray2sph_bis that is inside the sphere... squared
  float ratio = 1.f - std::sqrtf(cd2) / v_ray2sph_bis.mag(); // the remaining portion of v_ray2sph_bis leads to the surface of the sphere
  auto v_ray2sph_surface = v_ray2sph_bis * ratio;
  auto p_sphere_surface = incident_ray.pos + v_ray2sph_surface; 
  auto v_norm = p_sphere_surface - sphere.pos;
  v_norm.normalize();
  auto v_reflection = reflect(incident_ray.dir, v_norm);
  return std::make_optional(ReflectionData(Ray(p_sphere_surface, v_reflection), v_norm));
}

std::optional<ReflectionData> intersects(const Triangle & triangle, const Ray & incident_ray)
{
  const auto AmB = triangle.pos - triangle.pos2;
  const auto AmC = triangle.pos - triangle.pos3;
  const auto dir = incident_ray.dir;
  const auto AmP = triangle.pos - incident_ray.pos;
  const float detA = AmB.x * (AmC.y*dir.z - dir.y*AmC.z) + AmC.x * (dir.y*AmB.z - AmB.y*dir.z) + dir.x * (AmB.y*AmC.z - AmC.y*AmB.z);
  const float tDet = AmB.x * (AmC.y*AmP.z - AmP.y*AmC.z) + AmC.x * (AmP.y*AmB.z - AmB.y*AmP.z) + AmP.x * (AmB.y*AmC.z - AmC.y*AmB.z);
  const float gamDet = AmB.x * (AmP.y*dir.z - dir.y*AmP.z) + AmP.x * (dir.y*AmB.z - AmB.y*dir.z) + dir.x * (AmB.y*AmP.z - AmP.y*AmB.z);
  const float betDet = AmP.x * (AmC.y*dir.z - dir.y*AmC.z) + AmC.x * (dir.y*AmP.z - AmP.y*dir.z) + dir.x * (AmP.y*AmC.z - AmC.y*AmP.z);
  const float beta = betDet / detA;
  const float gamma = gamDet / detA;
  const float t = tDet / detA;
  if (beta < 0.f || gamma < 0.f || beta + gamma > 1.f || t < 0.f) {
    return std::nullopt;
  }

  const auto hitPoint = incident_ray.pos + incident_ray.dir * t;
  auto norm = AmB.cross(AmC);
  norm.normalize();
  auto reflection = reflect(incident_ray.dir, norm);
  return std::make_optional(ReflectionData(Ray(hitPoint, reflection), norm));
}

std::optional<ReflectionData> Sphere::intersects_with(const Ray & ray) const
{
  return intersects(*this, ray); 
}

std::optional<ReflectionData> Triangle::intersects_with(const Ray & ray) const
{
  return intersects(*this, ray); 
}
