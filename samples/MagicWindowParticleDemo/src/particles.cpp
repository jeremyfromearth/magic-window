//
//  Particles.cpp
//  ParticlesBasic
//
//  Created by jeremy on 1/6/18.
//

#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "Particles.h"

using namespace ci;
using namespace ci::app;

void Particles::draw() {
  gl::clear();
  gl::ScopedBlendAdditive blend;
  gl::begin(GL_LINES);
  for(auto &particle : particles) {
    float r = 0.5f + particle.vel.x / (speed * 2);
    float g = 0.5f + particle.vel.y / (speed * 2);
    float b = 0.5f + particle.z * 0.5f;
    gl::color(r, g, b);
    gl::vertex(particle.prev_pos);
    gl::vertex(particle.pos);
  }
  gl::end();
}

void Particles::update() {
  counter += 1.0f;
  for(auto &particle : particles) {
    particle.prev_pos = particle.pos;
    vec3 deriv = perlin.dfBm(vec3(particle.pos.x, particle.pos.y, counter) * 0.0005f);
    particle.z = deriv.z;
    vec2 deriv2 = normalize(vec2(deriv.x, deriv.y));
    particle.vel += deriv2 * speed;
    particle.pos += particle.vel;
    particle.vel *= friction;
    if(!bounds.contains(particle.pos))
      particle.reset(vec2(Rand::randFloat(bounds.getWidth()), Rand::randFloat(bounds.getHeight())));
  }
}

void Particles::setup(Rectf particle_bounds) {
  bounds = particle_bounds;
  perlin.setOctaves(2);
  perlin.setSeed(clock());
  particles.reserve(NUM_PARTICLES);
  for(int s = 0; s < NUM_PARTICLES; ++s) {
    particles.push_back(
      Particle(vec2(
        Rand::randFloat(bounds.getWidth()),
        Rand::randFloat(bounds.getHeight())
      )
    ));
  }
  
  speed = 0.5f;
  counter = 0.0f;
  friction = 0.97f;
}
