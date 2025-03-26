#ifndef CAMERA_H
#define CAMERA_H

#include <mpi.h>
#include <vector>
#include <chrono>
#include "hittable.h"
#include "material.h"
#include "PPM.h"

const int TAG_REQUEST = 1, TAG_WORK = 2, TAG_RESULT = 3, TAG_STOP = 4;

class camera {
  public:
    double aspect_ratio         = 1.0;  // Ratio of image width over height
    int    image_width          = 100;  // Rendered image width in pixel count
    int    samples_per_pixel    = 10;   // Count of random samples for each pixel
    int    max_depth            = 10;   // Maximum number of ray bounces into scene
    color  background;                  // Scene background color

    double vfov = 90;  // Vertical view angle (field of view)

    point3 lookfrom = point3(0,0,0);   // Point camera is looking from
    point3 lookat   = point3(0,0,-1);  // Point camera is looking at
    vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus

    
    void render(const hittable& world) {
        initialize();
    
        int rank, num_procs;
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
    
        if (rank == 0) { // Master
            // Begin Timer
            auto start = std::chrono::high_resolution_clock::now();

            // Create image where results are stored
            PPM image = PPM(image_height, image_width);

            if (num_procs == 1) { // Single-threaded rendering
                for (int j = 0; j < image_height; j++) {
                    std::cout << "\rScanlines remaining: " << (image_height - j) << ' ' << std::endl;
                    for (int i = 0; i < image_width; i++) {
                        color pixel_color(0, 0, 0);
                        for (int sample = 0; sample < samples_per_pixel; sample++) {
                            ray r = get_ray(i, j);
                            pixel_color += ray_color(r, max_depth, world);
                        }
                        image.setPixel(j, i, pixel_samples_scale * pixel_color);
                    }
                }
    
                image.writeImage();
            } else {
                int next_row = 0, active_workers = num_procs - 1;
                MPI_Status status;
        
                // Assign initial work
                for (int i = 1; i < num_procs; i++) {
                    if (next_row < image_height) {
                        MPI_Send(&next_row, 1, MPI_INT, i, TAG_WORK, MPI_COMM_WORLD);
                        next_row++;
                    }
                }
        
                while (active_workers > 0) {
                    std::vector<float> buffer(image_width * 3);
                    int row_index;
                    int worker_rank;
        
                    // Receive computed row
                    MPI_Recv(buffer.data(), image_width * 3, MPI_FLOAT, MPI_ANY_SOURCE, TAG_RESULT, MPI_COMM_WORLD, &status);
                    worker_rank = status.MPI_SOURCE;
                    MPI_Recv(&row_index, 1, MPI_INT, worker_rank, TAG_RESULT, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
                    // ✅ Fix: Correctly map row_index back to image
                    for (int i = 0; i < image_width; i++) {
                        color pixel_color(buffer[i * 3], buffer[i * 3 + 1], buffer[i * 3 + 2]);
                        image.setPixel(row_index, i, pixel_color); // ✅ Use row_index from worker
                    }
        
                    // Assign new work if available
                    if (next_row < image_height) {
                        MPI_Send(&next_row, 1, MPI_INT, worker_rank, TAG_WORK, MPI_COMM_WORLD);
                        next_row++;
                        std::cout << "\rScanlines remaining: " << (image_height - next_row) << ' ' << std::endl;
                    } else {
                        MPI_Send(nullptr, 0, MPI_INT, worker_rank, TAG_STOP, MPI_COMM_WORLD);
                        active_workers--;
                    }
                }
            }

            image.writeImage();

            // Stop timer and print duration
            auto end = std::chrono::high_resolution_clock::now();
            // Calculate elapsed time in milliseconds
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            std::cout << "Execution Time: " << duration.count() / 1000 << " s\n";

            std::cout << "Rendering completed!\n";
    
        } else { // Workers
            while (true) {
                int row;
                MPI_Status status;
    
                MPI_Recv(&row, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                if (status.MPI_TAG == TAG_STOP) break;
    
                // ✅ Compute row colors correctly
                std::vector<float> results(image_width * 3);
                for (int i = 0; i < image_width; i++) {
                    color pixel_color(0, 0, 0);
                    for (int sample = 0; sample < samples_per_pixel; sample++) {
                        ray r = get_ray(i, row);
                        pixel_color += ray_color(r, max_depth, world);
                    }
                    color result = pixel_samples_scale * pixel_color;
                    results[i * 3] = result.x();
                    results[i * 3 + 1] = result.y();
                    results[i * 3 + 2] = result.z();
                }
    
                // Send row data
                MPI_Send(results.data(), image_width * 3, MPI_FLOAT, 0, TAG_RESULT, MPI_COMM_WORLD);
                MPI_Send(&row, 1, MPI_INT, 0, TAG_RESULT, MPI_COMM_WORLD);
            }
        }
    
        MPI_Finalize();
    }

  private:
    int    image_height;   // Rendered image height
    double pixel_samples_scale;  // Color scale factor for a sum of pixel samples
    point3 center;         // Camera center
    point3 pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below
    vec3   u, v, w;        // Camera frame basis vectors
    vec3   defocus_disk_u;       // Defocus disk horizontal radius
    vec3   defocus_disk_v;       // Defocus disk vertical radius

    void initialize() {
        image_height = int(image_width / aspect_ratio);
        image_height = (image_height < 1) ? 1 : image_height;

        pixel_samples_scale = 1.0 / samples_per_pixel;

        center = lookfrom;

        // Determine viewport dimensions.
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(image_width)/image_height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / image_width;
        pixel_delta_v = viewport_v / image_height;

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors.
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    ray get_ray(int i, int j) const {
        // Construct a camera ray originating from the defocus disk and directed at a randomly
        // sampled point around the pixel location i, j.

        auto offset = sample_square();
        auto pixel_sample = pixel00_loc
                          + ((i + offset.x()) * pixel_delta_u)
                          + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        return ray(ray_origin, ray_direction, ray_time);
    }

    vec3 sample_square() const {
        // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
        return vec3(random_double() - 0.5, random_double() - 0.5, 0);
    }

    point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    color ray_color(const ray& r, int depth, const hittable& world) const {
        // If we've exceeded the ray bounce limit, no more light is gathered.
        if (depth <= 0)
            return color(0,0,0);

        hit_record rec;

        // If the ray hits nothing, return the background color.
        if (!world.hit(r, interval(0.001, infinity), rec))
            return background;

        ray scattered;
        color attenuation;
        color color_from_emission = rec.mat->emitted(rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, attenuation, scattered))
            return color_from_emission;

        color color_from_scatter = attenuation * ray_color(scattered, depth-1, world);

        return color_from_emission + color_from_scatter;
    }
};

#endif