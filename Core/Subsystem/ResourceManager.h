#pragma once
#include "ISubsystem.h"
#include "../../Resource/IResource.h"

class ResourceManager final : public ISubsystem
{
public:
    ResourceManager(class Context* context);
    ~ResourceManager();

    const bool Initialize() override;

	auto GetTextureImporter() const -> class TextureImporter* { return this->texture_importer.get(); }

    template <typename T> auto Load(const std::string& path) -> T*;
    template <typename T> auto GetResourceFromName(const std::string& name) -> T*;
    template <typename T> auto GetResourceFromPath(const std::string& path) -> T*;
    template <typename T> auto GetResourceDirectory() -> const std::string;

    void RegisterResource(IResource* resource);
    void RegisterResourceDirectory(const ResourceType& type, const std::string& directory);

private:
	std::shared_ptr<class TextureImporter> texture_importer;

    typedef std::vector<IResource*> ResourceGroup;
    std::map<ResourceType, ResourceGroup> resourceGroups;
    std::map<ResourceType, std::string> resourceDirectories;
};

template<typename T>
inline auto ResourceManager::Load(const std::string & path) -> T *
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");

    auto resource = GetResourceFromPath<T>(path);
    if (!resource)
    {
        auto type = IResource::DeduceResourceType<T>();
        auto directory = resourceDirectories[type];

        resource = new T(context);
        resource->SetResourceType(type);
        resource->SetResourceName(FileSystem::GetIntactFileNameFromPath(path));
        resource->SetResourcePath(directory + path);
        resource->LoadFromFile(resource->GetResourcePath());

        RegisterResource(resource);
    }
    return resource;
}

template<typename T>
inline auto ResourceManager::GetResourceFromName(const std::string & name) -> T *
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");
    for (auto resource : resourceGroups[IResource::DeduceResourceType<T>()])
    {
        if (resource->GetResourceName() == name)
            return static_cast<T*>(resource);
    }
    return nullptr;
}

template<typename T>
inline auto ResourceManager::GetResourceFromPath(const std::string & path) -> T *
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");
    for (auto resource : resourceGroups[IResource::DeduceResourceType<T>()])
    {
        if (resource->GetResourcePath() == path)
            return static_cast<T*>(resource);
    }
    return nullptr;
}

template<typename T>
inline auto ResourceManager::GetResourceDirectory() -> const std::string
{
    static_assert(std::is_base_of<IResource, T>::value, "Provided type does not implement IResource");
    return resourceDirectories[IResource::DeduceResourceType<T>()];
}
