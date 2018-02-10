//
//  Particles.h
//  ParticlesBasic
//
//  Created by jeremy on 1/6/18.
//

#pragma once

#include "cinder/Rand.h"
#include "cinder/Vector.h"
#include "cinder/Perlin.h"
#include "cinder/Rect.h"

#include "magicwindow.h"

class Particle {
public:
  Particle(const cinder::vec2 &position)
  : pos(position), prev_pos(position), vel(cinder::vec2(0)), z(0) {}
  
  void reset(const cinder::vec2 &position) {
    pos = prev_pos = position;
    vel = cinder::vec2(0);
    z = 0;
  }
  
  cinder::vec2 pos, prev_pos, vel;
  float z;
  
};

class Particles {
  
public:
  static const int  NUM_PARTICLES = 15000;
  
  Particles(magicwindow::context & ctx);
  
  float friction;
  float speed;
  float counter;
  cinder::Rectf bounds;
  
  ci::Perlin perlin;
  std::vector<Particle> particles;
  
  void draw();
  void update();
  void setup(cinder::Rectf particle_bounds);
  
  private:
    magicwindow::context & ctx;
};

