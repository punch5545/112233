#pragma once
#include "framework.h"

template <class T>
class Geometry
{
public:
    Geometry() = default;
    virtual ~Geometry();

    //------------------------------------------------------------
    //Vertex
    //------------------------------------------------------------
    auto GetVertexCount() const -> const uint { return vertices.size(); }
    auto GetVertexDataSize() const -> const uint { return vertices.size() * sizeof(T); }
    auto GetVertexData() -> T* { return vertices.data(); }
    auto GetVertices() const -> const std::vector<T>& { return vertices; }
    auto GetVertices
    (
        const uint& offset,
        const uint& count
    ) -> const std::vector<T>;

    void AddVertex(const T& vertex);
    void AddVertices(const std::vector<T>& vertices);
    void SetVertices(const std::vector<T>& vertices);

    //------------------------------------------------------------
    //Index
    //------------------------------------------------------------
    auto GetIndexCount() const -> const uint { return indices.size(); }
    auto GetIndexDataSize() const -> const uint { return indices.size() * sizeof(uint); }
    auto GetIndexData() -> uint* { return indices.data(); }
    auto GetIndices() const -> const std::vector<uint>& { return indices; }
    auto GetIndices
    (
        const uint& offset,
        const uint& count
    ) -> const std::vector<uint>;

    void AddIndex(const uint& index);
    void AddIndices(const std::vector<uint>& indices);
    void SetIndices(const std::vector<uint>& indices);


    void Clear();

protected:
    std::vector<T> vertices;
    std::vector<uint> indices;
};
#include "Geometry.inl"