#pragma once

#include <gin/mesh/mesh.hpp>
#include <godot_cpp/classes/mesh.hpp>
#include <godot_cpp/classes/surface_tool.hpp>
#include <godot_cpp/classes/array_mesh.hpp>

#include "graph.hpp"

class GinMesh : public Gin::Mesh::Mesh {
public:
	GinMesh();
	~GinMesh();

	void AddVertexData(Gin::Mesh::VertexData& vertexData) final;
	void ClearVertexData() final;

	godot::Ref<godot::SurfaceTool> GetSurfaceTool();

private:
	godot::Ref<godot::SurfaceTool> surfaceTool{};
	godot::Vector3 buff[2]{};
	size_t c{ 0 };
};

class GinMeshBuilder : public godot::Object {
	GDCLASS(GinMeshBuilder, godot::Object);

protected:
	static void _bind_methods();

public:
	GinMeshBuilder() {};
	~GinMeshBuilder() {};

public:
	static godot::Ref<godot::SurfaceTool> BuildVolume(godot::Ref<GinGraph> graph, godot::Ref<GinGraphContext> ctx);
};