# SFML Particle System

Project of creating a complete particle system using SFML with the follow architecture:

`ParticleSystem -> Emitters -> Particles`

## ParticleEffects

The idea is, currently the system requires creating all the emitters manually, which potentially can be
tedious. I'd like to have some "recipes" that automatically create standard particle effects. This can
be potentially achieved using the Builder pattern.

- ExplosionEffect
  - FireEmitter
  - SmokeEmitter
  - BlastEmitter
  - DebrisEmitter
- ThrusterEffect
  - FireEmitter (blue flame)
  - FireEmitter (red flame)
  - SmokeEmitter

## To Do

- [ ] Improve the randomization; it's currently uniform, using Gaussian/normal and/or Perlin noise
- [ ] Allow effects like color transitions, size/scale, alpha fading
- [ ] Add texture support, currently the ParticleSystem only supports points
- [ ] Currently, the Emitter only emits in a circle/ring pattern, add grid/line, path-following, mesh surface
- [ ] Allow modifiers to be attached to the particle systems:
  - [ ] Gravity (basic)
  - [ ] Wind zones (basic)
  - [ ] Collision surfaces (basic)
  - [ ] Attractors/repellers (extended)
  - [ ] Vortex effects (extended)
- [ ] Create some particle effect factories (Explosion, Thruster, Fire, Water, Sand)
- [ ] Allow the ParticleEmitter to be toggled on/off instead of using a duration, probably using a map to keep track of
  the instance
- [ ] Integrate ImGui for more profiling metrics (particle count, emission rate, update time vs render time)
- [ ] Add temporal control like Pause/Resume/TimeWarp/Slowdown
- [ ] Particle chaining, anyone?
- [ ] Implement particle interpolation for smoother visuals
- [ ] Consider more optimizations like pool allocations, multi-threading, pre-allocation
- [x] The code in the ParticleEmitter::Emit() function doesn't yet respect the emission rate
- [x] Create a ParticleEmitter
- [x] Make the ParticleSystem accept multiple ParticleEmitters
- [x] Trigger the effect on mouse click
