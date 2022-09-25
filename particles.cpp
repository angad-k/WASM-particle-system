#include <iostream>
#include <emscripten.h> // note we added the emscripten header
#include <stdlib.h>
class particle_system
{
    int* colors;
    float* positions;
    int n;

    // base color
    int base_r = 40;
    int base_g = 40;
    int base_b = 100;

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
public:
    particle_system(int p_n, int* p_colors, float* p_positions)
    {
        n = p_n;
        colors = p_colors;
        positions = p_positions;
        set_random_colors();
        set_random_positions();
    }
};
particle_system* p_s;
extern "C" {
void initialize_particle_system(int p_n, int* p_colors, float* p_positions);
}
void initialize_particle_system(int p_n, int* p_colors, float* p_positions)
{
    p_s = new particle_system(p_n, p_colors, p_positions);
    EM_ASM(
        // here you can write inline javascript code!
        console.log("Particle system initiated");
        // end of javascript code
    );
}

int main(){
    std::cout << "Hello world!" << std::endl;
    return 0;
}
