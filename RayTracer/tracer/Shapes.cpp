#include <vector>
#include <functional>
#include "Shapes.h"

std::optional<LightSourceData> calcLightSourceData(Shape* shape, float min_dist) {
  constexpr float MAX_HALO_DIST = 2.f;
  float halo_radius = MAX_HALO_DIST * shape->properties.intensity;
  if (min_dist >= halo_radius) {
    return std::nullopt;
  }
  float halo_mult_factor = 1.f - (min_dist / halo_radius);
  return std::make_optional(LightSourceData(shape->properties.color, halo_mult_factor ));
}

RayTraceDataPacket Shape::findNearestHitPoint(const Ray& incidentRay, const Shape* pIgnoreShape) {
  RayTraceDataPacket RayTraceData;
  if (this != pIgnoreShape) {
    auto hit = intersects_with(incidentRay);
    if (hit.has_value()) {
      RayTraceData.hitShape = this;
      RayTraceData.hitInfo = hit;
      if (auto lightSourceData = calcLightSourceData(this, 0.f); lightSourceData.has_value()) {
        RayTraceData.lightSources.push_back(*lightSourceData);
      }
    }
    else if (properties.intensity > 0.f) {
      auto min_dist = shortest_distance(incidentRay);
      if (auto lightSourceData = calcLightSourceData(this, min_dist); lightSourceData.has_value()) {
        RayTraceData.lightSources.push_back(*lightSourceData);
      }
    }
  }
  return RayTraceData;
}

//RayTraceDataPacket CompositeShape::findNearestHitPoint(const Ray& incidentRay, const Shape* pIgnoreShape) {
//  RayTraceDataPacket RayTraceData;
//  for (const auto& shape : shapes) {
//    auto lowDataPacket = shape->findNearestHitPoint(incidentRay, pIgnoreShape);
//    if (lowDataPacket.hitInfo.has_value()) {
//      if (!RayTraceData.hitInfo.has_value() || dist2(lowDataPacket.hitInfo->reflection.pos, incidentRay.pos) < dist2(RayTraceData.hitInfo->reflection.pos, incidentRay.pos)) {
//        RayTraceData.hitShape = lowDataPacket.hitShape;
//        RayTraceData.hitInfo = lowDataPacket.hitInfo;
//      }
//    }
//    for (auto& lightSource : lowDataPacket.lightSources) {
//      RayTraceData.lightSources.push_back(lightSource);
//    }
//  }
//  return RayTraceData;
//
//}

std::optional<HitData> Sphere::intersects_with(const Ray& incident_ray) const {
  auto v_ray2sph_center = pos - incident_ray.pos;
  auto v_r2s_proj_ray = v_ray2sph_center.dot(incident_ray.dir); // projection of ray_origin_to_sphere onto ray
  if (v_r2s_proj_ray + radius < 0.f) {  // if sphere is behind ray origin
    return std::nullopt;
  }
  auto v_ray2sph_bis = incident_ray.dir * v_r2s_proj_ray; // vector from ray origin to midway through the sphere along the ray
  auto v_min_dist = v_ray2sph_bis - v_ray2sph_center; // vector from sphere center to closest point on the ray 
  float d2 = v_min_dist.mag2();
  float r2 = radius*radius; 
  if (d2 > r2) { // if min distance between ray and sphere is greater than radius, then no intersection
    return std::nullopt;
  }
  float cd2 = r2 - d2; // ratio of v_ray2sph_bis that is inside the sphere... squared
  float ratio = 1.f - std::sqrt(cd2) / v_ray2sph_bis.mag(); // the remaining portion of v_ray2sph_bis leads to the surface of the sphere
  auto v_ray2sph_surface = v_ray2sph_bis * ratio;
  auto p_sphere_surface = incident_ray.pos + v_ray2sph_surface; 
  auto v_norm = p_sphere_surface - pos;
  v_norm.normalize();
  auto v_reflection = reflect(incident_ray.dir, v_norm);
  return std::make_optional(HitData(Ray(p_sphere_surface, v_reflection), v_norm, 0.f, 0.f));
}

float Sphere::shortest_distance(const Ray& incident_ray) const {
  auto v_ray2sph_center = pos - incident_ray.pos;
  auto v_r2s_proj_ray = v_ray2sph_center.dot(incident_ray.dir); // projection of ray_origin_to_sphere onto ray
  if (v_r2s_proj_ray < 0.f) {  // if sphere is behind ray origin
    return v_ray2sph_center.mag();
  }
  auto v_ray2sph_bis = incident_ray.dir * v_r2s_proj_ray; // vector from ray origin to midway through the sphere along the ray
  auto v_min_dist = v_ray2sph_bis - v_ray2sph_center; // vector from sphere center to closest point on the ray 
  float dist = v_min_dist.mag();
  return dist;
}

template <class FUNC>
std::optional<HitData> barycentric_intersects(const vec & pos1, const vec& p2mp1, const vec& p3mp1, const vec& norm, const Ray & incident_ray, FUNC func) {
  // use Cramers rule to solve for barycentric coordinates (alpha and gamma) and t in this equation
  // incident_ray.pos + incident_ray.dir * t = pos1 + alpha * (pos2 - pos1) + gamma * (pos3 - pos1);
  const auto& dir = incident_ray.dir;
  const float incident_dot_norm = -dir.dot(norm);
  // early abort if surface is facing away from the ray
  if (incident_dot_norm < 0.f) {
    return std::nullopt;
  }
  const auto AmP = pos1 - incident_ray.pos;
  // Determinates...  I really need to use a matrix class that has a det method`
  const float detA = p3mp1.x * (p2mp1.y*dir.z - dir.y*p2mp1.z) + p2mp1.x * (dir.y*p3mp1.z - p3mp1.y*dir.z) + dir.x * (p3mp1.y*p2mp1.z - p2mp1.y*p3mp1.z);
  const float tDet = p3mp1.x * (p2mp1.y*AmP.z - AmP.y*p2mp1.z) + p2mp1.x * (AmP.y*p3mp1.z - p3mp1.y*AmP.z) + AmP.x * (p3mp1.y*p2mp1.z - p2mp1.y*p3mp1.z);
  const float gamDet = p3mp1.x * (AmP.y*dir.z - dir.y*AmP.z) + AmP.x * (dir.y*p3mp1.z - p3mp1.y*dir.z) + dir.x * (p3mp1.y*AmP.z - AmP.y*p3mp1.z);
  const float betDet = AmP.x * (p2mp1.y*dir.z - dir.y*p2mp1.z) + p2mp1.x * (dir.y*AmP.z - AmP.y*dir.z) + dir.x * (AmP.y*p2mp1.z - p2mp1.y*AmP.z);
  const float overDetA = 1.0f / detA;
  const float beta = betDet * overDetA;
  const float gamma = gamDet * overDetA;
  const float t = tDet * overDetA;
  // intersection if barycentric coordinates between 0 and 1, and t is positive
  if(t < 0.f || !func(beta, gamma)) {
    return std::nullopt;
  }
  const auto hitPoint = incident_ray.pos + incident_ray.dir * t;
  auto reflection = reflect(incident_ray.dir, norm);
  return std::make_optional(HitData(Ray(hitPoint, reflection), norm, beta, gamma));
}

std::optional<HitData> Triangle::intersects_with(const Ray & incident_ray) const {
  return barycentric_intersects(pos, p2mp1, p3mp1, norm, incident_ray, [](float beta, float gamma) {
    return beta >= 0.f && gamma >= 0.f && beta + gamma <= 1.f;
  });
}

std::optional<HitData> Rect::intersects_with(const Ray & incident_ray) const {
  return barycentric_intersects(pos, p2mp1, p3mp1, norm, incident_ray, [](float beta, float gamma) {
    return beta >= 0.f && gamma >= 0.f && beta <= 1.f && gamma <= 1.f;
  });
}

std::optional<HitData> Box::intersects_with(const Ray & incident_ray) const {
  auto r0 = rect0.intersects_with(incident_ray);
  auto r1 = rect1.intersects_with(incident_ray);
  auto r2 = rect2.intersects_with(incident_ray);
  auto r3 = rect3.intersects_with(incident_ray);
  auto r4 = rect4.intersects_with(incident_ray);
  auto r5 = rect5.intersects_with(incident_ray);
  if(r0 != std::nullopt) return r0;
  if(r1 != std::nullopt) return r1;
  if(r2 != std::nullopt) return r2;
  if(r3 != std::nullopt) return r3;
  if(r4 != std::nullopt) return r4;
  return r5;
}
