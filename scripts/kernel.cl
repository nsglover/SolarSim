constant float G = 6.67428E-11f;

struct __attribute__ ((packed)) particle_struct
{
    float3 momentum;
    float3 position;
    float mass;
    float radius;
};

typedef struct particle_struct Particle;

inline void atomicAdd(volatile global float *source, const float operand) {
    union {
        unsigned int intVal;
        float floatVal;
    } newVal;
    union {
        unsigned int intVal;
        float floatVal;
    } prevVal;
    do {
        prevVal.floatVal = *source;
        newVal.floatVal = prevVal.floatVal + operand;
    } while (atomic_cmpxchg((volatile global unsigned int *) source, prevVal.intVal, newVal.intVal) != prevVal.intVal);
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

kernel void generateParticles(global Particle* particles) {
    int i = get_global_id(0);
    float t = (float) i;

    float8 rands = genRands(t, tan(t), 33.151f);

    int b = i < 512 ? 0 : 1;

    float x = rands[0 + b] - 0.5f;
    float y = rands[1 + b] - 0.5f;
    float z = rands[2 + b] - 0.5f;
    //float x = cos(t);
    //float y = sin(t);
    //float z = cos(t) * sin(t);

    particles[i].position.x = 2 * x + 5 * b;
    particles[i].position.y = 2 * y + 5 * b;
    particles[i].position.z = 2 * z + 5 * b;

    particles[i].mass = 1E9;
    particles[i].radius = 0.1;

    particles[i].momentum.x = particles[i].mass * y;
    particles[i].momentum.y = particles[i].mass * -x;
    particles[i].momentum.z = particles[i].mass * 0;
}

kernel void calculateForces(global Particle* particles, global float* forces) {
    int i = get_global_id(0);
    int j = get_global_id(1);

    if(i != j) {
        Particle p0 = particles[i];
        Particle p1 = particles[j];

        float dist = distance(p1.position, p0.position);
        float forceScalar = pow(dist, -2);
        
        float compressionFactor = 0.0f;

        if(dist < p0.radius + p1.radius) {
            forceScalar = -compressionFactor * G * p0.mass * p1.mass;
        }

        float3 force = G * p0.mass * p1.mass * forceScalar * normalize(p1.position - p0.position);

        atomicAdd(&forces[3 * i], force.x);
        atomicAdd(&forces[3 * i + 1], force.y);
        atomicAdd(&forces[3 * i + 2], force.z);
    }
}

kernel void updateStates(global Particle* particles, global float* forces, const float dt, global float* positions) {
    int i = get_global_id(0);

    Particle p = particles[i];
    float3 force = float3(0, 0, 0);
    force.x = forces[3 * i];
    force.y = forces[3 * i + 1];
    force.z = forces[3 * i + 2];

    float3 oldMomentum = p.momentum;
    float3 oldPosition = p.position;

    float3 newMomentum = oldMomentum + force * dt;
    float3 newPosition = oldPosition + 0.5f * (newMomentum + oldMomentum) / p.mass * dt;

    particles[i].momentum.x = newMomentum.x;
    particles[i].momentum.y = newMomentum.y;
    particles[i].momentum.z = newMomentum.z;

    particles[i].position.x = newPosition.x;
    particles[i].position.y = newPosition.y;
    particles[i].position.z = newPosition.z;

    particles[i].mass = p.mass;
    particles[i].radius = p.radius;

    positions[3 * i] = newPosition.x;
    positions[3 * i + 1] = newPosition.y;
    positions[3 * i + 2] = newPosition.z;
}
