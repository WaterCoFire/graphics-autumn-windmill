#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

/*
 * Model Class
 * This class serves as a high-level interface for loading 3D models using the Assimp library.
 * It handles the loading of the model file, processing its nodes and meshes, and storing them
 * in a format that is ready for rendering.
 */
class Model {
public:
    // Model data
    std::vector<Mesh> meshes;
    std::string directory;

    // Constructor, expects a filepath to a 3D model.
    Model(std::string const &path) {
        loadModel(path);
    }

    // Draws the model, and thus all its meshes
    void Draw(GLuint shaderProgram) {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shaderProgram);
    }

private:
    // Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const &path);

    // Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode *node, const aiScene *scene);

    // Processes an aiMesh object and transforms it into our own Mesh object.
    Mesh processMesh(aiMesh *mesh, const aiScene *scene);

    // (Future implementation) Checks all material textures of a given type and loads the textures if they're not loaded yet.
    // The required info is returned as a Texture struct.
    // std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
};

#endif
