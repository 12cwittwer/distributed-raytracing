#ifndef TEXTURE_H
#define TEXTURE_H

#include "rtw_stb_image.h"
#include "perlin.h"

enum TextureType {
  TEXTURE_SOLID,
  TEXTURE_CHECKER,
  TEXTURE_IMAGE,
  TEXTURE_NOSIE,
}

struct texture;

struct texture_solid;
struct texture_checker;
struct texture_image;
struct texture_noise;

struct texture {
  TextureType type;

  union {
    texture_solid* solid;
    texture_checker* checker;
    texture_image* image;
    texture_noise* noise;
  }

  __device__ __host__
  color value(double u, double v, const point3& p) const {
    switch (type) {
      case TEXTURE_SOLID: return solid->value(u, v, p)
      case TEXTURE_CHECKER: return solid->value(u, v, p)
      case TEXTURE_IMAGE: return solid->value(u, v, p)
      case TEXTURE_NOSIE: return solid->value(u, v, p)
    }
    return color(0, 1, 1);
  }
}

// class texture {
//   public:
//     virtual ~texture() = default;

//     virtual color value(double u, double v, const point3& p) const = 0;
// };

struct texture_solid {
  color albedo;

  __device__ __host__
  color value(double u, double v, const point3& p) const {
    return albedo;
  }
}

// class solid_color : public texture {
//   public:
//     solid_color(const color& albedo) : albedo(albedo) {}

//     solid_color(double red, double green, double blue) : solid_color(color(red,green,blue)) {}

//     color value(double u, double v, const point3& p) const override {
//         return albedo;
//     }

//   private:
//     color albedo;
// };

struct texture_checker {
  double inv_scale;
  texture* even;
  texture* odd;

  __device__ __host__
  color value(double u, double v, const point3& p) const {
    auto x = int(floor(inv_scale * p.x()));
    auto y = int(floor(inv_scale * p.y()));
    auto z = int(floor(inv_scale * p.z()));
    bool isEven = (x + y + z) % 2 == 0;
    return isEven ? even->value(u, v, p) : odd->value(u, v, p);
  }
}

// class checker_texture : public texture {
//   public:
//     checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
//       : inv_scale(1.0 / scale), even(even), odd(odd) {}

//     checker_texture(double scale, const color& c1, const color& c2)
//       : checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}

//     color value(double u, double v, const point3& p) const override {
//         auto xInteger = int(std::floor(inv_scale * p.x()));
//         auto yInteger = int(std::floor(inv_scale * p.y()));
//         auto zInteger = int(std::floor(inv_scale * p.z()));

//         bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;

//         return isEven ? even->value(u, v, p) : odd->value(u, v, p);
//     }

//   private:
//     double inv_scale;
//     shared_ptr<texture> even;
//     shared_ptr<texture> odd;
// };

struct texture_image {
  unsigned char* data;
  int width, height, bytes_per_pixel;

  __device__ __host__
  color value(double u, double v, const point3& p) const {
      if (!data) return color(0,1,1); // Debug cyan

      u = clamp(u, 0.0, 1.0);
      v = 1.0 - clamp(v, 0.0, 1.0);

      int i = int(u * width);
      int j = int(v * height);
      if (i >= width)  i = width - 1;
      if (j >= height) j = height - 1;

      int pixel_index = (j * width + i) * bytes_per_pixel;
      double color_scale = 1.0 / 255.0;

      return color(
          color_scale * data[pixel_index + 0],
          color_scale * data[pixel_index + 1],
          color_scale * data[pixel_index + 2]
      );
  }
};


// class image_texture : public texture {
//   public:
//     image_texture(const char* filename) : image(filename) {}

//     color value(double u, double v, const point3& p) const override {
//         // If we have no texture data, then return solid cyan as a debugging aid.
//         if (image.height() <= 0) return color(0,1,1);

//         // Clamp input texture coordinates to [0,1] x [1,0]
//         u = interval(0,1).clamp(u);
//         v = 1.0 - interval(0,1).clamp(v);  // Flip V to image coordinates

//         auto i = int(u * image.width());
//         auto j = int(v * image.height());
//         auto pixel = image.pixel_data(i,j);

//         auto color_scale = 1.0 / 255.0;
//         return color(color_scale*pixel[0], color_scale*pixel[1], color_scale*pixel[2]);
//     }

//   private:
//     rtw_image image;
// };

struct texture_noise {
  double scale;
  perlin noise;

  __device__ __host__
  color value(double u, double v, const point3& p) const {
    return color(0.5, 0.5, 0.5) * (1 + sin(scale * p.z() + 10 * noise.turb(p, 7)));
  }
}

// class noise_texture : public texture {
//   public:
//     noise_texture(double scale) : scale(scale) {}

//     color value(double u, double v, const point3& p) const override {
//         return color(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
//     }

//   private:
//     perlin noise;
//     double scale;
// };

#endif