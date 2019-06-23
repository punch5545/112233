#include "framework.h"
#include "ResourceManager.h"
#include "../../Resource/Importer/TextureImporter.h"

ResourceManager::ResourceManager(Context * context)
    : ISubsystem(context)
{
}

ResourceManager::~ResourceManager()
{
    for (auto& resourceGroup : resourceGroups)
    {
        for (auto& resource : resourceGroup.second)
            SAFE_DELETE(resource);
    }

    resourceGroups.clear();
    resourceDirectories.clear();
}

const bool ResourceManager::Initialize()
{
    texture_importer = std::make_shared<TextureImporter>(context);

    RegisterResourceDirectory(ResourceType::Audio,      "../../_Assets/Audio/");
    RegisterResourceDirectory(ResourceType::Animation,  "../../_Assets/Animation/");
    RegisterResourceDirectory(ResourceType::Texture,    "../../_Assets/Texture/");
    RegisterResourceDirectory(ResourceType::Material,   "../../_Assets/Material/");
    RegisterResourceDirectory(ResourceType::Mesh,       "../../_Assets/Mesh/");

    return true;
}

void ResourceManager::RegisterResource(IResource * resource)
{
    resourceGroups[resource->GetResourceType()].emplace_back(resource);
}

void ResourceManager::RegisterResourceDirectory(const ResourceType & type, const std::string & directory)
{
    resourceDirectories[type] = directory;
}
