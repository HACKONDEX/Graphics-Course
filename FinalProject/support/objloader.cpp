#include <vector>
#include <stdio.h>
#include <string>
#include <cstring>

#include <glm/glm.hpp>

#include "objloader.hpp"

namespace support {

bool LoadObj(const char* path, std::vector<glm::vec3>& out_vertices,
             std::vector<glm::vec2>& out_uvs,
             std::vector<glm::vec3>& out_normals) {
  printf("Loading OBJ file %s...\n", path);

  std::vector<unsigned int> vertex_indices, uv_indices, normal_indices;
  std::vector<glm::vec3> temp_vertices;
  std::vector<glm::vec2> temp_uvs;
  std::vector<glm::vec3> temp_normals;

  FILE* file = fopen(path, "r");
  if (file == nullptr) {
    printf(
        "Impossible to open the file ! Are you in the right path ? See "
        "Tutorial 1 for details\n");
    getchar();
    return false;
  }

  while (1) {
    char line_header[128];
    // read the first word of the line
    int res = fscanf(file, "%s", line_header);
    if (res == EOF) {
      break;
    }

    if (strcmp(line_header, "v") == 0) {
      glm::vec3 vertex;
      fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
      temp_vertices.push_back(vertex);
    } else if (strcmp(line_header, "vt") == 0) {
      glm::vec2 uv;
      fscanf(file, "%f %f\n", &uv.x, &uv.y);
      uv.y = -uv.y;  // Invert V coordinate since we will only use DDS texture,
      // which are inverted. Remove if you want to use TGA or BMP
      // loaders.
      temp_uvs.push_back(uv);
    } else if (strcmp(line_header, "vn") == 0) {
      glm::vec3 normal;
      fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
      temp_normals.push_back(normal);
    } else if (strcmp(line_header, "f") == 0) {
      std::string vertex1, vertex2, vertex3;
      unsigned int vertex_index[3], uv_index[3], normal_index[3];
      int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                           &vertex_index[0], &uv_index[0], &normal_index[0],
                           &vertex_index[1], &uv_index[1], &normal_index[1],
                           &vertex_index[2], &uv_index[2], &normal_index[2]);
      if (matches != 9) {
        printf(
            "File can't be read by our simple parser :-( Try exporting with "
            "other options\n");
        return false;
      }
      vertex_indices.push_back(vertex_index[0]);
      vertex_indices.push_back(vertex_index[1]);
      vertex_indices.push_back(vertex_index[2]);
      uv_indices.push_back(uv_index[0]);
      uv_indices.push_back(uv_index[1]);
      uv_indices.push_back(uv_index[2]);
      normal_indices.push_back(normal_index[0]);
      normal_indices.push_back(normal_index[1]);
      normal_indices.push_back(normal_index[2]);
    } else {
      // Probably a comment, eat up the rest of the line
      char stupid_buffer[1000];
      fgets(stupid_buffer, 1000, file);
    }
  }

  // For each vertex of each triangle
  for (unsigned int i = 0; i < vertex_indices.size(); i++) {
    // Get the indices of its attributes
    unsigned int vertex_index = vertex_indices[i];
    unsigned int uv_index = uv_indices[i];
    unsigned int normal_index = normal_indices[i];

    // Get the attributes thanks to the index
    glm::vec3 vertex = temp_vertices[vertex_index - 1];
    glm::vec2 uv = temp_uvs[uv_index - 1];
    glm::vec3 normal = temp_normals[normal_index - 1];

    // Put the attributes in buffers
    out_vertices.push_back(vertex);
    out_uvs.push_back(uv);
    out_normals.push_back(normal);
  }

  return true;
}
}  // namespace support
