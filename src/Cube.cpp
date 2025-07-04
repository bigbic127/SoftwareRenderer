#include "Cube.hpp"

Cube::Cube()
{
    int count = 0;
    for (float x = -1; x <= 1 ;  x += 0.25f)
    {
        for (float y = -1; y <= 1 ;  y += 0.25f)
        {
            for (float z = -1; z <= 1 ;  z += 0.25f)
            {
                Vector3D vec3 = {x, y, z};
                points[count++] = vec3;
            }
        }
    }
}