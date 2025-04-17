# SFML Particle System

Project of creating a complete particle system using SFML with the follow architecture:

`ParticleSystem -> Emitters -> Particles`

ParticleEffects (Factory):

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

- [ ] Create a ParticleEmitter virtual class
- [ ] Make the ParticleSystem accept multiple ParticleEmitters
- [ ] Create some particle effect factories (Explosion)
- [ ] Trigger the effect on mouse click
