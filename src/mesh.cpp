#include "mesh.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <gin/graph/serialization/graphserdes.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <gin/spatial/sampler.hpp>
#include <gin/mesh/marchingcube.hpp>

GinMesh::GinMesh()
{
	surfaceTool.instantiate();
	surfaceTool->begin(godot::Mesh::PrimitiveType::PRIMITIVE_TRIANGLES);
}

GinMesh::~GinMesh()
{
}

void GinMesh::AddVertexData(Gin::Mesh::VertexData& vertexData)
{
	/*surfaceTool->add_vertex(godot::Vector3{
			vertexData.position.x(),
			vertexData.position.y(),
			vertexData.position.z()
		});*/

	//We are doing this weird shit because Godot Winding Order is Clockwise and not Counter-Clockwise (Wtf)

	if (c < 2) {
		buff[c] = godot::Vector3{
			(real_t)vertexData.position.x(),
			(real_t)vertexData.position.y(),
			(real_t)vertexData.position.z()
		};
		++c;
	}
	else {
		surfaceTool->add_vertex(godot::Vector3{
			(real_t)vertexData.position.x(),
			(real_t)vertexData.position.y(),
			(real_t)vertexData.position.z()
			});
		surfaceTool->add_vertex(buff[1]);
		surfaceTool->add_vertex(buff[0]);

		c = 0;
	}
}

void GinMesh::ClearVertexData()
{
	surfaceTool->clear();
}

godot::Ref<godot::SurfaceTool> GinMesh::GetSurfaceTool()
{
	return surfaceTool;
}

void GinMeshBuilder::_bind_methods()
{
	godot::ClassDB::bind_static_method("GinMeshBuilder", godot::D_METHOD("build_volume", "graph", "ctx"), &GinMeshBuilder::BuildVolume);
}

godot::Ref<godot::SurfaceTool> GinMeshBuilder::BuildVolume(godot::Ref<GinGraph> graph, godot::Ref<GinGraphContext> ctx)
{
	GinMesh mesh{};

	if (graph.is_null()) {
		godot::UtilityFunctions::printerr("Graph can't be null.");
		return mesh.GetSurfaceTool();
	}

	if (ctx.is_null()) {
		godot::UtilityFunctions::printerr("Context can't be null.");
		return mesh.GetSurfaceTool();
	}

	size_t volumeIdx = graph->graph.HasOutput("_Volume");

	if (volumeIdx == std::numeric_limits<size_t>::max()) {
		godot::UtilityFunctions::printerr("Graph is not a Volume Graph");
		return mesh.GetSurfaceTool();
	}

	Gin::Graph::GraphPort& volumePort = graph->graph.GetOutputPort(volumeIdx);

	if (volumePort.GetType().type != (Gin::Graph::PortType)((int)Gin::Graph::PortType::Number + (int)Gin::Graph::PortType::Spatial)) {
		godot::UtilityFunctions::printerr("Can't build volume : a Spatial<Float> _Volume output is required.");
		return mesh.GetSurfaceTool();
	}

	graph->graph.Execute(ctx->ctx);

	Gin::Spatial::Spatial<float> spatial = *(Gin::Spatial::Spatial<float>*)volumePort.GetProperty();
	Gin::Spatial::Sampler<float> sampler{ spatial };

	sampler.SetScale(ctx->ctx.scale);
	sampler.SetBounds(ctx->ctx.bounds);

	Gin::Mesh::MarchingCubeMeshBuilder builder{};
	builder.bounds = ctx->ctx.bounds;
	builder.scale = ctx->ctx.scale;

	builder.Build(mesh, sampler);

	return mesh.GetSurfaceTool();
}
