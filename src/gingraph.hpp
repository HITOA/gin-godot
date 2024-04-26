//
// Created by HITO on 22/04/24.
//

#ifndef GIN_GODOT_GINGRAPH_HPP
#define GIN_GODOT_GINGRAPH_HPP

#include <godot_cpp/core/binder_common.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/editor_import_plugin.hpp>
#include <godot_cpp/classes/editor_plugin.hpp>

#include <gin/graph/graph.hpp>
#include <gin/graph/serialization/graphserdes.hpp>

class GinGraphPort : public godot::RefCounted {
GDCLASS(GinGraphPort, godot::RefCounted);

protected:
    static void _bind_methods();

public:
    void set_graph_port(Gin::Graph::GraphPort* port);

    godot::String get_name();
    Gin::Graph::GraphPort* get_port();

    void set_float(float v);
    void set_vector2(godot::Vector2 v);
    void set_vector3(godot::Vector3 v);
    void set_vector4(godot::Vector4 v);

private:
    Gin::Graph::GraphPort* port{};
};

class GinGraph : public godot::RefCounted {
GDCLASS(GinGraph, godot::RefCounted);

protected:
    static void _bind_methods();

public:
    void load_graph(Gin::Graph::Serialization::SerializedGraph& serializedGraph);

    godot::Array get_inputs();
    godot::Array get_outputs();

    void execute(float scale, godot::AABB bound);

private:
    Gin::Graph::Graph graph{};
    godot::Array inputs{};
    godot::Array outputs{};
};

class GinSerializedGraph : public godot::Resource {
GDCLASS(GinSerializedGraph, godot::Resource);

protected:
    static void _bind_methods();

public:
    void set_serialized_graph(godot::String value);
    godot::String get_serialized_graph();

    godot::Ref<GinGraph> instantiate();

private:
    godot::String serializedGraph{};
};

class GinGraphImporter : public godot::EditorImportPlugin {
GDCLASS(GinGraphImporter, godot::EditorImportPlugin);

protected:
    static void _bind_methods();

public:
    virtual godot::String _get_importer_name() const;
    virtual godot::String _get_visible_name() const;
    virtual int32_t _get_preset_count() const;
    virtual godot::String _get_preset_name(int32_t preset_index) const;
    virtual godot::PackedStringArray _get_recognized_extensions() const;
    virtual godot::TypedArray<godot::Dictionary> _get_import_options(const godot::String& path, int32_t preset_index) const;
    virtual godot::String _get_save_extension() const;
    virtual godot::String _get_resource_type() const;
    virtual double _get_priority() const;
    virtual int32_t _get_import_order() const;
    virtual bool _get_option_visibility(const godot::String &path, const godot::StringName &option_name, const godot::Dictionary &options) const;
    virtual godot::Error _import(const godot::String &source_file, const godot::String &save_path, const godot::Dictionary &options, const godot::TypedArray<godot::String> &platform_variants, const godot::TypedArray<godot::String> &gen_files) const;

};

class GinEditorPlugin : public godot::EditorPlugin {
GDCLASS(GinEditorPlugin, godot::EditorPlugin);

protected:
    static void _bind_methods();

public:
    GinEditorPlugin();
    ~GinEditorPlugin();

    virtual void _notification(int p_what);

private:
    godot::Ref<GinGraphImporter> importer{};
};

#endif //GIN_GODOT_GINGRAPH_HPP
