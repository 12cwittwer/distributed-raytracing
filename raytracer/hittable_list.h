#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include "rtweekend.h"
#include "aabb.h"

#include <vector>

struct hittable_list {
    hittable* objects;
    int size;
    aabb bbox;

    __device__ __host__
    hittable_list() : objects(nullptr), size(0), bbox() {}

    __device__ __host__
    hittable_list(hittable* objs, int n) : objects(objs), size(n) {
        compute_bounding_box();
    }

    __device__ __host__
    void compute_bounding_box() {
        if (size == 0) {
            bbox = aabb();
            return;
        }

        bbox = objects[0].bounding_box();
        for (int i = 1; i < size; i++) {
            bbox = aabb(bbox, objects[i].bounding_box());
        }
    }

    __device__ __host__
    bool hit(const ray& r, interval ray_t, hit_record& rec) const {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (int i = 0; i < size; ++i) {
            if (objects[i].hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    __device__ __host__
    aabb bounding_box() const {
        return bbox;
    }
}

class hittable_list : public hittable {
  public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) {
        objects.push_back(object);
        bbox = aabb(bbox, object->bounding_box());
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        hit_record temp_rec;
        bool hit_anything = false;
        auto closest_so_far = ray_t.max;

        for (const auto& object : objects) {
            if (object->hit(r, interval(ray_t.min, closest_so_far), temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }

        return hit_anything;
    }

    aabb bounding_box() const override { return bbox; }

  private:
    aabb bbox;
};

#endif