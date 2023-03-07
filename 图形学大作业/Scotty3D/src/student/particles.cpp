
#include "../scene/particles.h"
#include "../rays/pathtracer.h"

bool Scene_Particles::Particle::update(const PT::Object& scene, float dt, float radius) {

    float tLeft = dt; // time left for collision looping
    float eps = 1e-3; // minimum time to continue loop
    while((tLeft - eps) > 0) {
        // TODO: ray from particle origin; velocity is always unit
        Ray ray = Ray(pos, velocity);
        // TODO: how far the particle will travel

        // TODO: hit something?
        PT::Trace trace = scene.hit(ray);
        if(trace.hit&&trace.distance<velocity.norm()*tLeft+radius) {
            // TODO: calculate new pos and velocity, and new simulation time.
            float tt = (trace.distance - radius) / velocity.norm();
            pos = trace.position - velocity.unit() * radius;
            Vec3 ori_v = velocity + acceleration * tt;
            Vec3 v_n = trace.normal * dot(ori_v, trace.normal);
            Vec3 v_orth = ori_v - v_n;
            velocity = v_orth - v_n;
            tLeft -= tt;

        } else {
            // use Forward Euler to calculate new pos and velocity and break loop
            pos += velocity * tLeft;
            velocity += acceleration * tLeft;
            break;
        }
    }
    age -= dt;
    return age > 0; // dead particle?
}
