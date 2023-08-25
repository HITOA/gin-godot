#include "graph.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <gin/graph/serialization/graphserdes.hpp>
#include <godot_cpp/variant/utility_functions.hpp>


void GinGraph::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("get_name"), &GinGraph::GetName);
	godot::ClassDB::bind_method(godot::D_METHOD("compile"), &GinGraph::Compile);
	godot::ClassDB::bind_method(godot::D_METHOD("execute", "ctx"), &GinGraph::Execute);

	godot::ClassDB::bind_static_method("GinGraph", godot::D_METHOD("load_graph", "path"), &GinGraph::LoadGraph);
	godot::ClassDB::bind_static_method("GinGraph", godot::D_METHOD("load_graph_from_json", "json"), &GinGraph::LoadGraphFromJson);
}

GinGraph::GinGraph()
{
}

GinGraph::~GinGraph()
{
}

godot::String GinGraph::GetName() const
{
	return graphName;
}

void GinGraph::Compile()
{
	graph.Compile();
}

void GinGraph::Execute(godot::Ref<GinGraphContext> ctx)
{
	graph.Execute(ctx->ctx);
}

godot::Ref<GinGraph> GinGraph::LoadGraph(const godot::StringName& path)
{
	godot::Ref<GinGraph> graph{};
	graph.instantiate();

	auto& f = godot::FileAccess::open(path, godot::FileAccess::READ);
	godot::String content{ f->get_as_text() };
	f->close();

	nlohmann::json jsonGraph{nlohmann::json::parse(content.ascii().get_data())};
	
	Gin::Graph::Serialization::SerializedGraph serializedGraph{};
	Gin::Graph::Serialization::LoadSerializedGraphFromJson(serializedGraph, jsonGraph);
	Gin::Graph::Serialization::DeserializeGraph(graph->graph, serializedGraph);

	graph->graphName = serializedGraph.graphName.c_str();

	return graph;
}

godot::Ref<GinGraph> GinGraph::LoadGraphFromJson(const godot::Ref<godot::JSON> json)
{
	return godot::Ref<GinGraph>();
}

void GinGraphContext::_bind_methods()
{
	godot::ClassDB::bind_method(godot::D_METHOD("set_scale", "scale"), &GinGraphContext::SetScale);
	godot::ClassDB::bind_method(godot::D_METHOD("get_scale"), &GinGraphContext::GetScale);

	godot::ClassDB::bind_method(godot::D_METHOD("set_origin", "origin"), &GinGraphContext::SetOrigin);
	godot::ClassDB::bind_method(godot::D_METHOD("get_origin"), &GinGraphContext::GetOrigin);

	godot::ClassDB::bind_method(godot::D_METHOD("set_extent", "extent"), &GinGraphContext::SetExtent);
	godot::ClassDB::bind_method(godot::D_METHOD("get_extent"), &GinGraphContext::GetExtent);
}

void GinGraphContext::SetScale(float scale)
{
	ctx.scale = scale;
}

float GinGraphContext::GetScale()
{
	return ctx.scale;
}

void GinGraphContext::SetOrigin(godot::Vector3 origin)
{
	ctx.bounds.origin.x() = origin.x;
	ctx.bounds.origin.y() = origin.y;
	ctx.bounds.origin.z() = origin.z;
}

godot::Vector3 GinGraphContext::GetOrigin()
{
	return godot::Vector3{ (float)ctx.bounds.origin.x(), (float)ctx.bounds.origin.y(), (float)ctx.bounds.origin.z() };
}

void GinGraphContext::SetExtent(godot::Vector3 extent)
{
	ctx.bounds.extent.x() = extent.x;
	ctx.bounds.extent.y() = extent.y;
	ctx.bounds.extent.z() = extent.z;
}

godot::Vector3 GinGraphContext::GetExtent()
{
	return godot::Vector3{ (float)ctx.bounds.extent.x(), (float)ctx.bounds.extent.y(), (float)ctx.bounds.extent.z() };
}
