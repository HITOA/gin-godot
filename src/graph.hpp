#pragma once

#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/classes/viewport.hpp>

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/classes/json.hpp>

#include <gin/gin.hpp>

class GinGraphContext : public godot::RefCounted {
	GDCLASS(GinGraphContext, godot::RefCounted);

protected:
	static void _bind_methods();

public:
	void SetScale(float scale);
	float GetScale();
	void SetOrigin(godot::Vector3 origin);
	godot::Vector3 GetOrigin();
	void SetExtent(godot::Vector3 extent);
	godot::Vector3 GetExtent();

public:
	Gin::Graph::GraphContext ctx{};
};

class GinGraph : public godot::RefCounted {
	GDCLASS(GinGraph, godot::RefCounted);

protected:
	static void _bind_methods();

public:
	GinGraph();
	~GinGraph();

	godot::String GetName() const;
	void Compile();
	void Execute(godot::Ref<GinGraphContext> ctx);

public:
	static godot::Ref<GinGraph> LoadGraph(const godot::StringName& path);
	/**
	 * Not Implemented.
	 */
	static godot::Ref<GinGraph> LoadGraphFromJson(const godot::Ref<godot::JSON> json);

public:
	Gin::Graph::Graph graph{};
	godot::String graphName{ "<new graph>" };
};