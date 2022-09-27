#include <iostream>
#include <emscripten.h> // note we added the emscripten header
#include <stdlib.h>
#include <cmath>
class particle_system
{
    float* colors;
    float* positions;
    float* masses;
    float* speeds;
    int n;
    

    // base color
    int base_r = 109;
    int base_g = 227;
    int base_b = 18;

    int target_r = 227;
    int target_g = 18;
    int target_b = 178;

    //constants
    float force_multiplier = 20;
    float max_speed = 0.5; 

    void set_random_colors()
    {
        for(int i = 0; i < n; i++)
        {
            int r = 3*i;
            int g = 3*i + 1;
            int b = 3*i + 2;
            colors[r] = base_r;
            colors[g] = base_g;
            colors[b] = base_b;
        }
    }

    void set_masses()
    {
        masses = (float*)malloc(n*sizeof(int));
        for(int i = 0; i < n; i++)
        {
            masses[i] = rand()%100;
        }
    }

    void set_speeds()
    {
        speeds = (float*)malloc(n*sizeof(int)*2);
        for(int i = 0; i < 2*n; i++)
        {
            speeds[i] = 0;
        }
    }

    void set_random_positions()
    {
        for(int i = 0; i < n; i++)
        {
            int j = 2*i;
            int k = 2*i + 1;
            positions[j] = rand() % 100;
            positions[k] = rand() % 100;
        }
    }

    float sigmoid(float x)
    {
        float e_x = std::expf(-x);
        e_x += 1;
        return 1/e_x;
    }

    float map_inf_to_1(float x)
    {
        return ((2 * sigmoid(x)) - 1); 
    }

    float smooth_step(float from, float to, float x)
    {
        return (from * (1 - x)) + (to * x);
    }

public:
    particle_system(int p_n, float* p_colors, float* p_positions)
    {
        n = p_n;
        colors = p_colors;
        positions = p_positions;
        set_random_colors();
        set_random_positions();
        set_masses();
        set_speeds();
    }
    void update_particle_system(int target_x, int target_y, float delta)
    {
        delta = delta/1000;
        for(int i = 0; i < n; i++)
        {
            float x = positions[i*2];
            float y = positions[i*2 + 1];
            // std::cout << "positions " << positions[i*2] << " " << positions[i*2 + 1] << "\n";
            float dx = target_x-x;
            float dy = target_y-y;
            // std::cout << "dx dy " << dx << " " << dy << "\n";
            float distance = std::sqrt(dx*dx + dy*dy);
            
            float mapped_d = map_inf_to_1(distance);
            colors[i*3] = smooth_step(base_r, target_r, mapped_d);
            colors[i*3 + 1] = smooth_step(base_g, target_g, mapped_d);
            colors[i*3 + 2] = smooth_step(base_b, target_b, mapped_d);

            if(distance < 5)
            {
                distance = 5;
            }
            // std::cout << "distance " << distance << "\n";
            float force = force_multiplier/distance;
            // std::cout << "force " << force << "\n";
            float acceleration = force/masses[i];
            // std::cout << "acceleration " << acceleration << "\n";
            // std::pair<float, float> normalized = normalize(dx, dy);
            float l = std::sqrt(dx*dx + dy*dy);
            float ndx = float(dx)/l;
            float ndy = float(dy)/l;
            // std::cout << "ndx ndy " << ndx << " " << ndy << "\n";
            float acc_x = acceleration * ndx;
            float acc_y = acceleration * ndy;
            // std::cout << "acc_x acc_y " << acc_x << " " << acc_y << "\n";
            positions[i*2] = positions[i*2] + speeds[i*2]*delta;
            positions[i*2 + 1] = positions[i*2 + 1] + speeds[i*2 + 1]*delta;
            // std::cout << "positions " << positions[i*2] << " " << positions[i*2 + 1] << "\n";
            speeds[i*2] = speeds[i*2] + acc_x*delta;
            speeds[i*2 + 1] = speeds[i*2 + 1] + acc_y*delta;
            // std::cout << "speeds " << speeds[i*2] << " " << speeds[i*2 + 1] << "\n";
            float length_speed = std::sqrt(speeds[i*2]*speeds[i*2] + speeds[i*2 + 1]*speeds[i*2 + 1]);
            if(length_speed > max_speed)
            {
                speeds[i*2] = (speeds[i*2] / length_speed) * max_speed;
                speeds[i*2 + 1] = (speeds[i*2 + 1] / length_speed) * max_speed;
            }
        }
    }
};
particle_system* p_s;
extern "C" {
void initialize_particle_system(int p_n, float* p_colors, float* p_positions);
void update_particle_system(int target_x, int target_y, float delta);
}
void initialize_particle_system(int p_n, float* p_colors, float* p_positions)
{
    p_s = new particle_system(p_n, p_colors, p_positions);
    EM_ASM(
        // here you can write inline javascript code!
        console.log("Particle system initiated");
        // end of javascript code
    );
}
void update_particle_system(int target_x, int target_y, float delta)
{
    p_s->update_particle_system(target_x, target_y, delta);
}

int main(){
    std::cout << "Hello world!" << std::endl;
    return 0;
}
