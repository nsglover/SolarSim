constant float G = 6.67428E-11f;
constant float PI = 3.141592654f;


struct __attribute__ ((packed)) particle_struct
{
    float8 data;
};

typedef struct particle_struct Particle;

constant int PX = 0;
constant int PY = 1;
constant int PZ = 2;
constant int MX = 3;
constant int MY = 4;
constant int MZ = 5;
constant int M = 6;
constant int R = 7;

inline void atomicAdd(volatile global float *addr, float val)
{
union {
unsigned int u32;
float f32;
} next, expected, current;
current.f32 = *addr;
do {
expected.f32 = current.f32;
next.f32 = expected.f32 + val;
current.u32 = atomic_cmpxchg( (volatile __global unsigned int *)addr,
expected.u32, next.u32);
} while( current.u32 != expected.u32 );
}

inline float noise(float x, float y, float z) {
    float ptr = 0.0f;
    return fract(sin(x*112.9898f + y*179.233f + z*237.212f) * 43758.5453f, &ptr);
}

inline float8 genRands(float x, float y, float z) {
    float8 output;
    output[0] = noise(x, y, z);
    output[1] = noise(y, z, output[0]);
    output[2] = noise(z, output[0], output[1]);
    output[3] = noise(output[0], output[1], output[2]);
    output[4] = noise(output[1], output[2], output[3]);
    output[5] = noise(output[2], output[3], output[4]);
    output[6] = noise(output[3], output[4], output[5]);
    output[7] = noise(output[4], output[5], output[6]);
    return output;
}

inline float3 getPosition(Particle particle) {
    float3 pos;
    pos.x = particle.data[PX];
    pos.y = particle.data[PY];
    pos.z = particle.data[PZ];
    return pos;
}

inline float3 getMomentum(Particle particle) {
    float3 momentum;
    momentum.x = particle.data[MX];
    momentum.y = particle.data[MY];
    momentum.z = particle.data[MZ];
    return momentum;
}

inline float getMass(Particle particle) {
    return particle.data[M];
}

inline float getRadius(Particle particle) {
    return particle.data[R];
}

kernel void generateParticles(global Particle* particles) {
    int i = get_global_id(0);
    int n = get_global_size(0);
    float t = (float) i;

    float8 rands = genRands(t, tan(t), 33.151f);

    int b = i < 0.50f * n ? 0 : 0;

    float x = rands[0 + 3 * b] - 0.5f;
    float y = rands[1 + 3 * b] - 0.5f;
    float z = rands[2 + 3 * b] - 0.5f;
    //float x = cos(t);
    //float y = sin(t);
    //float z = cos(t) * sin(t);

    particles[i].data[PX] = 100.0f * (x + b);
    particles[i].data[PY] = 100.0f * (y + b);
    particles[i].data[PZ] = 100.0f * (z + b);

    float mass = 1e11;

    particles[i].data[MX] = 15.0f * mass * y;
    particles[i].data[MY] = 15.0f * mass * -x;
    particles[i].data[MZ] = 15.0f * mass * 0;

    particles[i].data[M] = mass;
    particles[i].data[R] = 0.5f;
}

kernel void calculateForces(global Particle* particles, global float* forces) {
    int i = get_global_id(0);
    int j = get_global_id(1);

    if(i != j) {
        Particle p0 = particles[i];
        Particle p1 = particles[j];

        float p0Mass = getMass(p0);
        float p1Mass = getMass(p1);

        float3 p0Pos = getPosition(p0);
        float3 p1Pos = getPosition(p1);

        float dist = distance(p0Pos, p1Pos);
        float forceScalar = pow(dist, -2);

        float minDist = getRadius(p0) + getRadius(p1);

        if(dist < minDist) {
            int k = (i < j) ? i : j;
            int k0 = (i < j) ? j : i;

            particles[k].data[MX] = getMomentum(p0).x + getMomentum(p1).x;
            particles[k].data[MY] = getMomentum(p0).y + getMomentum(p1).y;
            particles[k].data[MZ] = getMomentum(p0).z + getMomentum(p1).z;

            particles[k].data[PX] = (p0Pos.x * p0Mass + p1Pos.x * p1Mass) / (p0Mass + p1Mass);
            particles[k].data[PY] = (p0Pos.y * p0Mass + p1Pos.y * p1Mass) / (p0Mass + p1Mass);
            particles[k].data[PZ] = (p0Pos.z * p0Mass + p1Pos.z * p1Mass) / (p0Mass + p1Mass);

            particles[k].data[M] = p0Mass + p1Mass;
            particles[k].data[R] = cbrt(pow(getRadius(p0), 3) + pow(getRadius(p1), 3));

            particles[k0].data[MX] = 0;
            particles[k0].data[MY] = 0;
            particles[k0].data[MZ] = 0;

            particles[k0].data[PX] = 0;
            particles[k0].data[PY] = 0;
            particles[k0].data[PZ] = 0;

            particles[k0].data[M] = 0;
            particles[k0].data[R] = 0;
        }

        float3 direction = normalize(getPosition(particles[j]) - getPosition(particles[i]));
        float3 force = forceScalar * G * getMass(particles[i]) * getMass(particles[j]) * direction;

        if(length(force) > 0.01f) {

            atomicAdd(&forces[3 * i], force.x);
            atomicAdd(&forces[3 * i + 1], force.y);
            atomicAdd(&forces[3 * i + 2], force.z);
        }
    }
}

kernel void updateStates(global Particle* particles, global float* forces, const float dt) {
    int i = get_global_id(0);

    Particle p = particles[i];
    float3 force = float3(0, 0, 0);
    force.x = forces[3 * i];
    force.y = forces[3 * i + 1];
    force.z = forces[3 * i + 2];

    float3 oldMomentum = getMomentum(p);
    float3 oldPosition = getPosition(p);

    float3 newMomentum = oldMomentum + force * dt;
    float3 newPosition = oldPosition + 0.5f * (newMomentum + oldMomentum) / getMass(p) * dt;

    particles[i].data[MX] = newMomentum.x;
    particles[i].data[MY] = newMomentum.y;
    particles[i].data[MZ] = newMomentum.z;

    particles[i].data[PX] = newPosition.x;
    particles[i].data[PY] = newPosition.y;
    particles[i].data[PZ] = newPosition.z;

    particles[i].data[M] = getMass(p);
    particles[i].data[R] = getRadius(p);
}
