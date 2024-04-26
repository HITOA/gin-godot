//
// Created by HITO on 24/04/24.
//

#include "meshbuilder.hpp"

#include <gin/mesh/indexedmesh.hpp>
#include <gin/mesh/surfacenet.hpp>
#include <godot_cpp/variant/utility_functions.hpp>
#include <string>
#include <cmath>

void GinMeshBuilder::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("build_volume", "volume_port", "scale", "bound"), &GinMeshBuilder::build_volume);
}

godot::Ref<godot::ArrayMesh>
GinMeshBuilder::build_volume(godot::Ref<GinGraphPort> volume_port, float scale, godot::AABB bound) {

    Gin::Mesh::MeshBuildData buildData{};

    Gin::Graph::GraphPort* vp = volume_port->get_port();
    if (vp->GetType().type != (Gin::Graph::PortType)((int)Gin::Graph::PortType::Scalar + (int)Gin::Graph::PortType::Field)) {
        WARN_PRINT("Can't build volume : volume port output is the wrong type, should be float.");
        return nullptr;
    }


    Gin::Field::Sampler<float>* volumeSampler = (Gin::Field::Sampler<float>*)vp->GetProperty();
    Gin::Mesh::IndexedMesh mesh{};

    buildData.mesh = &mesh;
    buildData.scale = scale;
    buildData.bounds = Gin::Math::Bounds{ Gin::Math::Vector3{
            bound.position.x, bound.position.y, bound.position.z
    }, Gin::Math::Vector3{
            bound.size.x, bound.size.y, bound.size.z
    } };
    buildData.volume = *volumeSampler;

    Gin::Mesh::SurfaceNetMeshBuilder builder{};
    builder.SetTriangleWindingOrder(Gin::Mesh::TriangleWindingOrder::CLOCK_WISE);
    builder.Build(buildData);

    godot::Ref<godot::ArrayMesh> arrayMesh{};
    arrayMesh.instantiate();

    godot::Array surfaceArray{};
    surfaceArray.resize(godot::Mesh::ArrayType::ARRAY_MAX);

    godot::PackedVector3Array vertices{};
    godot::PackedVector3Array normals{};
    godot::PackedColorArray colors{};
    godot::PackedInt32Array indices{};

    vertices.resize(mesh.GetVertices().size());
    normals.resize(mesh.GetNormals().size());
    colors.resize(mesh.GetColors().size());
    indices.resize(mesh.GetIndices().size());

    memcpy(vertices.ptrw(), mesh.GetVertices().data(), vertices.size() * sizeof(Gin::Math::Vector3)); //sizeof == 12
    memcpy(normals.ptrw(), mesh.GetNormals().data(), normals.size() * sizeof(Gin::Math::Vector3)); //sizeof == 12
    memcpy(colors.ptrw(), mesh.GetColors().data(), colors.size() * sizeof(Gin::Math::Vector4)); //sizeof == 16
    memcpy(indices.ptrw(), mesh.GetIndices().data(), indices.size() * sizeof(int));

    surfaceArray[godot::Mesh::ArrayType::ARRAY_VERTEX] = vertices;
    surfaceArray[godot::Mesh::ArrayType::ARRAY_NORMAL] = normals;
    surfaceArray[godot::Mesh::ArrayType::ARRAY_COLOR] = colors;
    surfaceArray[godot::Mesh::ArrayType::ARRAY_INDEX] = indices;

    arrayMesh->add_surface_from_arrays(godot::Mesh::PrimitiveType::PRIMITIVE_TRIANGLES, surfaceArray);
    return arrayMesh;
}