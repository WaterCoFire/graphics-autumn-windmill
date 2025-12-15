#include "model.h"
#include <iostream>
#include "assimp/postprocess.h"

// Loads a model from file and populates the meshes vector.
void Model::loadModel(std::string const &path) {
    // Read file via ASSIMP
    Assimp::Importer importer;
    // aiProcess_Triangulate: If the model is not (entirely) composed of triangles, it should transform all the model's primitive shapes to triangles.
    // aiProcess_FlipUVs: Flips the texture coordinates on the y-axis where necessary.
    // aiProcess_GenNormals: Creates normal vectors for each vertex if the model doesn't contain them.
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);

    // Check for errors
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }
    // Retrieve the directory path of the filepath
    directory = path.substr(0, path.find_last_of('/'));

    // Process ASSIMP's root node recursively
    processNode(scene->mRootNode, scene);
}

// Processes a node recursively.
void Model::processNode(const aiNode *node, const aiScene *scene) {
    // Process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // Then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

// Translates an aiMesh object to our Mesh object.
Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    const std::vector<Texture> textures;

    // Process vertex positions, normals and texture coordinates
    for(unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex{};
        // Positions
        vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        // Normals
        if (mesh->HasNormals()) {
            vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        } else {
            vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
        }
        // Texture coordinates
        if(mesh->mTextureCoords[0]) { // Does the mesh contain texture coordinates?
            // A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
            // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
            vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // Process indices
    for(unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        // Retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    return Mesh(vertices, indices, textures);
}