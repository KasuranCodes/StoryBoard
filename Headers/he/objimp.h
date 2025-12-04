#ifndef OBJIMP_H
#define OBJIMP_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "tri.h"
#include "vector.h"

#ifdef SOKOL_IMPL
vec4 defaultColors[] = {
    {1.0f, 0.0f, 0.0f, 1.0f},  // Red
    {0.0f, 1.0f, 0.0f, 1.0f},  // Green
    {0.0f, 0.0f, 1.0f, 1.0f},  // Blue
    {1.0f, 1.0f, 0.0f, 1.0f},  // Yellow
    {1.0f, 0.0f, 1.0f, 1.0f},  // Magenta
    {0.0f, 1.0f, 1.0f, 1.0f}   // Cyan
};

#else
vec4 defaultColors[] = {
    {1.0f, 0.0f, 0.0f, 1.0f},  // Red
    {0.0f, 1.0f, 0.0f, 1.0f},  // Green
    {0.0f, 0.0f, 1.0f, 1.0f},  // Blue
    {1.0f, 1.0f, 0.0f, 1.0f},  // Yellow
    {1.0f, 0.0f, 1.0f, 1.0f},  // Magenta
    {0.0f, 1.0f, 1.0f, 1.0f}   // Cyan
};
#endif 

typedef struct texture {
    char *filepath;

} Texture;

typedef struct material {
    char *name;
    
    int Ns;
    float Ni;
    float d;

} Material;

typedef struct object {
    char *name;
    char *filepath;
    int triNum;

    vec3 position;

    int vertexCount;
    tri3d *tris;
    vec3 *vertices;
    vec3 *normals;
    vec2 *texcoords;
    
    /*
    | f    | 1/1/1 |
    +------+-------+
    | face | v/t/n |
    */
   int orderSize;
   int *vertexOrder;
   int *texOrder;
   int *normalOrder;
   
   Texture texture;
   
   
} Object;


/*---Function Prototypes---*/
static int loadObject(char *filepath, Object *object);


/*

OBJ FORMATTING GUIDE

| Identifier | Meaning              | Example          |
| ---------- | -------------------- | ---------------- |
| `Ka`       | Ambient color (RGB)  | `Ka 1.0 1.0 1.0` |
| `Kd`       | Diffuse color (RGB)  | `Kd 1.0 0.0 0.0` |
| `Ks`       | Specular color (RGB) | `Ks 1.0 1.0 1.0` |
| `Ke`       | Emissive color (RGB) | `Ke 0.0 0.0 0.0` |

| Identifier | Meaning                                                  | Example    |
| ---------- | -------------------------------------------------------- | ---------- |
| `Ns`       | Specular exponent (shininess, range 0–1000)              | `Ns 200.0` |
| `Ni`       | Optical density (index of refraction)                    | `Ni 1.45`  |
| `d`        | Dissolve (transparency: 1.0 = opaque, 0.0 = transparent) | `d 0.9`    |
| `Tr`       | Transparency (deprecated; `Tr = 1 - d`)                  | `Tr 0.1`   |
| `illum`    | Illumination model                                       | `illum 2`  |

| Identifier           | Meaning              | Example               |
| -------------------- | -------------------- | --------------------- |
| `map_Ka`             | Ambient texture map  | `map_Ka ambient.jpg`  |
| `map_Kd`             | Diffuse texture map  | `map_Kd texture.jpg`  |
| `map_Ks`             | Specular texture map | `map_Ks specular.jpg` |
| `map_Ke`             | Emissive texture map | `map_Ke emissive.png` |
| `map_d`              | Alpha texture map    | `map_d alpha.png`     |
| `map_bump` or `bump` | Bump (normal) map    | `map_bump normal.png` |

*/




static int loadObject(char *filepath, Object *obj) {
    FILE *file = fopen(filepath, "r");
    if (file == NULL) {
        return 0;
    }
    
    // Initialize all fields properly
    obj->vertices = NULL;
    obj->vertexCount = 0;
    obj->vertexOrder = NULL;
    obj->orderSize = 0;
    obj->triNum = 0;
    
    char line[256];  // Increased buffer size
    
    while (fgets(line, sizeof(line), file)) {
        // Parse vertex lines: "v x y z"
        if (line[0] == 'v' && line[1] == ' ') {
            float x, y, z;
            if (sscanf(line, "v %f %f %f", &x, &y, &z) == 3) {
                // Reallocate memory for new vertex
                obj->vertices = realloc(obj->vertices, sizeof(vec3) * (obj->vertexCount + 1));
                if (obj->vertices == NULL) {
                    fclose(file);
                    return 0;
                }
                
                obj->vertices[obj->vertexCount].x = x;
                obj->vertices[obj->vertexCount].y = y;
                obj->vertices[obj->vertexCount].z = z;
                obj->vertexCount++;
                
                printf("Loaded vertex %d: (%.3f, %.3f, %.3f)\n", obj->vertexCount, x, y, z);
            }
        }
        // Parse face lines: "f v1 v2 v3" (simple triangular faces)
        else if (line[0] == 'f' && line[1] == ' ') {
            int v1, v2, v3;
            if (sscanf(line, "f %d %d %d", &v1, &v2, &v3) == 3) {
                printf("Face: %d, %d, %d\n", v1, v2, v3);
                
                // Reallocate memory for 3 new indices
                obj->vertexOrder = realloc(obj->vertexOrder, sizeof(int) * (obj->orderSize + 3));
                if (obj->vertexOrder == NULL) {
                    fclose(file);
                    return 0;
                }
                
                obj->vertexOrder[obj->orderSize++] = v1 - 1;  // OBJ indices are 1-based, convert to 0-based
                obj->vertexOrder[obj->orderSize++] = v2 - 1;
                obj->vertexOrder[obj->orderSize++] = v3 - 1;
                obj->triNum++;
            }
            // Handle complex face format: "f v1/t1/n1 v2/t2/n2 v3/t3/n3"
            else {
                int v1, v2, v3, t1, t2, t3, n1, n2, n3;
                if (sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", 
                          &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3) == 9) {
                    printf("Complex face: %d/%d/%d %d/%d/%d %d/%d/%d\n", 
                           v1, t1, n1, v2, t2, n2, v3, t3, n3);
                    
                    // Reallocate memory for 3 new indices
                    obj->vertexOrder = realloc(obj->vertexOrder, sizeof(int) * (obj->orderSize + 3));
                    if (obj->vertexOrder == NULL) {
                        fclose(file);
                        return 0;
                    }
                    
                    obj->vertexOrder[obj->orderSize++] = v1 - 1;  // Convert to 0-based
                    obj->vertexOrder[obj->orderSize++] = v2 - 1;
                    obj->vertexOrder[obj->orderSize++] = v3 - 1;
                    obj->triNum++;
                }
            }
        }
        // You can add more parsing for normals (vn) and texture coordinates (vt) here
    }
    
    fclose(file);


    if (obj->triNum > 0) {
        obj->tris = malloc(sizeof(tri3d) * obj->triNum);
        if (obj->tris == NULL) {
            return 0;
        }
        
        // Build triangles from vertices and assign colors
        for (int i = 0; i < obj->triNum; i++) {
            int idx = i * 3;  // Each triangle uses 3 vertices
            
            // Assign vertices to triangle
            obj->tris[i].v[0] = obj->vertices[obj->vertexOrder[idx + 0]];
            obj->tris[i].v[1] = obj->vertices[obj->vertexOrder[idx + 1]];
            obj->tris[i].v[2] = obj->vertices[obj->vertexOrder[idx + 2]];
            
            // Cycle through colors using modulo
            #ifdef SOKOL_IMPL
            obj->tris[i].color = defaultColors[i % 6];
            
            printf("Triangle %d: Color = (%.1f, %.1f, %.1f, %.1f)\n", 
                   i, 
                   obj->tris[i].color.x,
                   obj->tris[i].color.y,
                   obj->tris[i].color.z,
                   obj->tris[i].color.w);
            #else
            obj->tris[i].color = defaultColors[i % 6];
            
            printf("Triangle %d: Color = (%.1f, %.1f, %.1f)\n", 
                   i, 
                   obj->tris[i].color.x,
                   obj->tris[i].color.y,
                   obj->tris[i].color.z);
            #endif 
        }
    }


    printf("Loaded %d vertices, %d faces (%d indices)\n", 
           obj->vertexCount, obj->triNum, obj->orderSize);
    
    return 1;
}

#endif