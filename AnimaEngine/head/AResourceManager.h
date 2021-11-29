#ifndef ARESOURCEMANAGER_H
#define ARESOURCEMANAGER_H

#include <vector>
#include <string>
#include "AResource.h"
#include "AMeshResource.h"
#include "AMeshLODResource.h"
#include "ATextureResource.h"
#include "AShaderResource.h"
#include "AMaterialResource.h"
#include "AAnimationResource.h"

class AResourceManager
{
    public:
        AResourceManager();
        virtual ~AResourceManager();
        AMeshResource* getMeshResource(std::string);
        AMeshLODResource* getMeshLODResource(std::string);
        AShaderResource* getShaderResource(std::string);
        ATextureResource* getTextureResource(std::string);
        AMaterialResource* getMaterialResource(std::string);
        AAnimationResource* getAnimationResource(std::string, int);
        void clearResourceManager();

        //free resource from memory
        void eraseMeshResource(AMeshResource*);
        void eraseTextureResource(ATextureResource*);
        void eraseMaterialResource(AMaterialResource*);
    private:
    	std::vector<AMeshResource*> meshes;
        std::vector<AMeshLODResource*> meshesLOD;
        std::vector<AShaderResource*> shaders;
    	std::vector<ATextureResource*> textures;
        std::vector<AMaterialResource*> materials;
    	std::vector<AAnimationResource*> animations;
};

#endif // ARESOURCEMANAGER_H