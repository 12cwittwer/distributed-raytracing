#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "texture.h"

enum MaterialType {
  MATERIAL_LAMBERTIAN,
  MATERIAL_METAL,
  MATERIAL_DIELECTRIC,
  MATERIAL_DIFFUSE_LIGHT,
  MATERIAL_ISOTROPIC
}

struct material;

struct material_lambertian;
struct material_metal;
struct material_dielectric;
struct material_diffuse_light;
struct material_isotropic;

struct material {
  MaterialType type;

  union {
    material_lambertian* lambertian;
    material_metal* metal;
    material_dielectric* dielectric;
    material_diffuse_light* dffuse_light;
    material_isotropic* isotropic;
  }

  __host__ __device__
  bool scatter (const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    switch (type) {
      case MATERIAL_LAMBERTIAN: return lambertian->scatter(r_in, rec, attenuation, scattered);
      case MATERAIL_METAL: return metal->scatter(r_in, rec, attenuation, scattered);
      case MATERIAL_DIELECTRIC: return dielectric->scatter(r_in, rec, attenuation, scattered);
      case MATERIAL_DIFFUSE_LIGHT: return diffuse_light->scatter(r_in, rec, attenuation, scattered);
      case MATERIAL_ISOTROPIC: return isotropic->scatter(r_in, rec, attenuation, scatterd);
    }
    return false;
  }

  __host__ __device__
  bool emitted(double u, double v, const point3& p) const {
    switch (type) {
      case MATERIAL_DIFFUSE_LIGHT: return diffuse_light->emitted(u, v, p);
      default: return color(0, 0, 0);
    }
  }
}

// class material {
//   public:
//     virtual ~material() = default;

//     virtual color emitted(double u, double v, const point3& p) const {
//       return color(0,0,0);
//     }

//     virtual bool scatter(
//         const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
//     ) const {
//         return false;
//     }
// };

struct material_lambertian {
  color albedo;

  __device__ __host__
  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    auto scatter_direction = rec.normal + random_unit_vector();

    if (scatter_direction.near_zero())
      scatter_direction = rec.normal;

    scattered = ray(rec.p, scatter_direction, r_in.time());
    attenuation = albedo;
    return true;
  }
}

// class lambertian : public material {
//   public:
//     lambertian(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
//     lambertian(shared_ptr<texture> tex) : tex(tex) {}

//     bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
//     const override {
//         auto scatter_direction = rec.normal + random_unit_vector();

//         // Catch degenerate scatter direction
//         if (scatter_direction.near_zero())
//             scatter_direction = rec.normal;

//         scattered = ray(rec.p, scatter_direction, r_in.time());
//         attenuation = tex->value(rec.u, rec.v, rec.p);
//         return true;
//     }

//   private:
//     shared_ptr<texture> tex;
// };

struct material_metal {
  color albedo;
  double fuzz;

  __device__ __host__
  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) {
    vec3 reflected = reflect(r_in.direction(), rec.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = ray(rec.p, reflected, r_in.time());
    attenuation = albedo;
    return (dot(scattered.direction(), rec.normal) > 0);
  }
}

// class metal : public material {
//   public:
//     metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

//     bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
//     const override {
//         vec3 reflected = reflect(r_in.direction(), rec.normal);
//         reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
//         scattered = ray(rec.p, reflected, r_in.time());
//         attenuation = albedo;
//         return (dot(scattered.direction(), rec.normal) > 0);
//     }

//   private:
//     color albedo;
//     double fuzz;
// };

struct material_dielectric {
  double refractive_index;

  __device__ __host__
  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) {
    attenuation = color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta*cos_theta);;

    bool cannot refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cost_theta, ri) > random_double()) {
      direction = reflect(unit_direction, rec.normal);
    } else {
      direction = refract(unit_direction, rec.normal, ri);
    }

    scattered = ray(rec.p, direction, r_in.time());
    return true;
  }

  __device__ __host__
  static double reflectance(double cosine, double refraction_index) {
    auto r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0*r0;
    return r0 + (1 - r0)*pow((1-cosine), 5);
  }
}

// class dielectric : public material {
//   public:
//     dielectric(double refraction_index) : refraction_index(refraction_index) {}

//     bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
//     const override {
//         attenuation = color(1.0, 1.0, 1.0);
//         double ri = rec.front_face ? (1.0/refraction_index) : refraction_index;

//         vec3 unit_direction = unit_vector(r_in.direction());
//         double cos_theta = std::fmin(dot(-unit_direction, rec.normal), 1.0);
//         double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);

//         bool cannot_refract = ri * sin_theta > 1.0;
//         vec3 direction;

//         if (cannot_refract || reflectance(cos_theta, ri) > random_double())
//             direction = reflect(unit_direction, rec.normal);
//         else
//             direction = refract(unit_direction, rec.normal, ri);

//         scattered = ray(rec.p, direction, r_in.time());
//         return true;
//     }

//   private:
//     // Refractive index in vacuum or air, or the ratio of the material's refractive index over
//     // the refractive index of the enclosing media
//     double refraction_index;

//     static double reflectance(double cosine, double refraction_index) {
//         // Use Schlick's approximation for reflectance.
//         auto r0 = (1 - refraction_index) / (1 + refraction_index);
//         r0 = r0*r0;
//         return r0 + (1-r0)*std::pow((1 - cosine),5);
//     }
// };

struct material_diffuse_light {
  color emit;

  __device__ __host__
  color emitted(double u, double v, const point3& p) const {
    return emit;
  }
}

// class diffuse_light : public material {
//   public:
//     diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
//     diffuse_light(const color& emit) : tex(make_shared<solid_color>(emit)) {}

//     color emitted(double u, double v, const point3& p) const override {
//         return tex->value(u, v, p);
//     }

//   private:
//     shared_ptr<texture> tex;
// };

struct material_isotropic {
  color albedo;

  __device__ __host__
  bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const {
    scattered = ray(rec.p, random_unit_vector(), r_in.time());
    attenuation = albedo;
    return true;
  }
}

// class isotropic : public material {
//   public:
//     isotropic(const color& albedo) : tex(make_shared<solid_color>(albedo)) {}
//     isotropic(shared_ptr<texture> tex) : tex(tex) {}

//     bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered)
//     const override {
//         scattered = ray(rec.p, random_unit_vector(), r_in.time());
//         attenuation = tex->value(rec.u, rec.v, rec.p);
//         return true;
//     }

//   private:
//     shared_ptr<texture> tex;
// };

#endif