#include "SkyDome.h"

namespace GE {

    SkyDome::SkyDome() {

        float  radius = 1800;
        float pi =  3.14159;
        float resolution = 100;

        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;

        float azimuthStep = (2.0f * pi) / (float)(resolution - 1);
        float elevationStep = pi / (float)(resolution - 1);

        float x, y, z, u, v;
        float theta, phi;


        for (int latitude = 0; latitude < resolution + 1; latitude++) {

            for (int longitude = 0; longitude < resolution + 1; longitude++) {
                // Calculate inclination and azimuth angles.
                theta = pi / 2.0f - longitude * elevationStep;
                phi = latitude * azimuthStep;
                // Vertex position
                x = (radius * cosf(theta)) * cosf(phi);
                z = (radius * cosf(theta)) * sinf(phi);
                y = radius * sinf(theta);
                // UV cordinates
                u = (float)latitude / (float)resolution;
                v = (float)longitude / (float)resolution;
                vertices.emplace_back(x, y, z, u, v,0,0,0);
            }
        }

        unsigned int c, n;
        for (int latitude = 0; latitude < resolution-1  ; latitude++) {
            c = latitude * (resolution + 1);
            n = c + resolution + 1;
            for (int longitude = 0; longitude < resolution-1; longitude++) {
                if (longitude != 0) {
                    indices.push_back(c);
                    indices.push_back(n);
                    indices.push_back(c + 1);
                }
                if (longitude != resolution - 1) {
                    indices.push_back(c + 1);
                    indices.push_back(n);
                    indices.push_back(n + 1);
                }
                c++;
                n++;
            }
        }

        // creates buffers for the 
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        indexCount = indices.size();
        //SDL_FreeSurface(heightMap);

    }    
}