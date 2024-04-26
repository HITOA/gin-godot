//
// Created by HITO on 22/04/24.
//

#include "gingraph.hpp"

#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/resource_saver.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

void GinGraphPort::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("get_name"), &GinGraphPort::get_name);
    godot::ClassDB::bind_method(godot::D_METHOD("set_float", "value"), &GinGraphPort::set_float);
    godot::ClassDB::bind_method(godot::D_METHOD("set_vector2", "value"), &GinGraphPort::set_vector2);
    godot::ClassDB::bind_method(godot::D_METHOD("set_vector3", "value"), &GinGraphPort::set_vector3);
    godot::ClassDB::bind_method(godot::D_METHOD("set_vector4", "value"), &GinGraphPort::set_vector4);
}

void GinGraphPort::set_graph_port(Gin::Graph::GraphPort *port) {
    this->port = port;
}

godot::String GinGraphPort::get_name() {
    return godot::String{ port->GetName().c_str() };
}

Gin::Graph::GraphPort *GinGraphPort::get_port() {
    return port;
}

void GinGraphPort::set_float(float v) {
    port->SetValue(Gin::Field::Sampler<Gin::Math::Scalar>{ v });
}

void GinGraphPort::set_vector2(godot::Vector2 v) {
    port->SetValue(Gin::Field::Sampler<Gin::Math::Vector2>{ Gin::Math::Vector2{ v.x, v.y } });
}

void GinGraphPort::set_vector3(godot::Vector3 v) {
    port->SetValue(Gin::Field::Sampler<Gin::Math::Vector3>{ Gin::Math::Vector3{ v.x, v.y, v.z } });
}

void GinGraphPort::set_vector4(godot::Vector4 v) {
    port->SetValue(Gin::Field::Sampler<Gin::Math::Vector4>{ Gin::Math::Vector4{ v.x, v.y, v.z, v.w } });
}

void GinGraph::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("get_inputs"), &GinGraph::get_inputs);
    godot::ClassDB::bind_method(godot::D_METHOD("get_outputs"), &GinGraph::get_outputs);
    godot::ClassDB::bind_method(godot::D_METHOD("execute", "scale", "bound"), &GinGraph::execute);
}

void GinGraph::load_graph(Gin::Graph::Serialization::SerializedGraph& serializedGraph) {
    Gin::Graph::Serialization::DeserializeGraph(graph, serializedGraph);

    inputs.resize(graph.GetInputsCount());
    outputs.resize(graph.GetOutputsCount());

    for (int i = 0; i < graph.GetInputsCount(); ++i) {
        godot::Ref<GinGraphPort> port{};
        port.instantiate();
        port->set_graph_port(&graph.GetInputPort(i));
        inputs[i] = port;
    }

    for (int i = 0; i < graph.GetOutputsCount(); ++i) {
        godot::Ref<GinGraphPort> port{};
        port.instantiate();
        port->set_graph_port(&graph.GetOutputPort(i));
        outputs[i] = port;
    }
}

godot::Array GinGraph::get_inputs() {
    return inputs;
}

godot::Array GinGraph::get_outputs() {
    return outputs;
}

void GinGraph::execute(float scale, godot::AABB bound) {

    Gin::Graph::GraphContext ctx{};
    ctx.scale = scale;
    ctx.bounds = Gin::Math::Bounds{ Gin::Math::Vector3{
            bound.position.x, bound.position.y, bound.position.z
    }, Gin::Math::Vector3{
            bound.size.x, bound.size.y, bound.size.z
    } };

    graph.Compile();
    graph.Execute(ctx);


    Gin::Field::Sampler<float>* volumeSampler = (Gin::Field::Sampler<float>*)graph.GetOutputPort(0).GetProperty();

    for (int z = 0; z < 128; ++z) {
        for (int y = 0; y < 128; ++y) {
            for (int x = 0; x < 128; ++x) {
                float v = volumeSampler->GetScalar(x, y, z);
                if (std::isnan(v)) {
                    godot::UtilityFunctions::print(v);
                }
            }
        }
    }
}

void GinSerializedGraph::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("set_serialized_graph", "value"), &GinSerializedGraph::set_serialized_graph);
    godot::ClassDB::bind_method(godot::D_METHOD("get_serialized_graph"), &GinSerializedGraph::get_serialized_graph);
    godot::ClassDB::bind_method(godot::D_METHOD("instantiate"), &GinSerializedGraph::instantiate);

    ADD_PROPERTY(godot::PropertyInfo( godot::Variant::STRING, "serialized_graph" ), "set_serialized_graph", "get_serialized_graph");
}

void GinSerializedGraph::set_serialized_graph(godot::String value) {
    serializedGraph = value;
}

godot::String GinSerializedGraph::get_serialized_graph() {
    return serializedGraph;
}

godot::Ref<GinGraph> GinSerializedGraph::instantiate() {
    godot::Ref<GinGraph> gg{};
    gg.instantiate();

    Gin::Graph::Serialization::SerializedGraph sg{};
    auto asciistr = serializedGraph.ascii();
    nlohmann::json jsonGraph = nlohmann::json::parse(asciistr.ptr());

    Gin::Graph::Serialization::LoadSerializedGraphFromJson(sg, jsonGraph);

    gg->load_graph(sg);
    return gg;
}

void GinGraphImporter::_bind_methods() {}

godot::String GinGraphImporter::_get_importer_name() const {
    return "GinGraphImporter";
}

godot::String GinGraphImporter::_get_visible_name() const {
    return "GinGraph";
}

int32_t GinGraphImporter::_get_preset_count() const {
    return 0;
}

godot::String GinGraphImporter::_get_preset_name(int32_t preset_index) const {
    return godot::String{};
}

godot::PackedStringArray GinGraphImporter::_get_recognized_extensions() const {
    godot::PackedStringArray exts{};
    exts.push_back("gg");
    return exts;
}

godot::TypedArray<godot::Dictionary>
GinGraphImporter::_get_import_options(const godot::String &path, int32_t preset_index) const {
    return godot::TypedArray<godot::Dictionary>{};
}

godot::String GinGraphImporter::_get_save_extension() const {
    return "res";
}

godot::String GinGraphImporter::_get_resource_type() const {
    return "Resource";
}

double GinGraphImporter::_get_priority() const {
    return 1.0;
}

int32_t GinGraphImporter::_get_import_order() const {
    return 0;
}

bool GinGraphImporter::_get_option_visibility(const godot::String &path, const godot::StringName &option_name,
                                              const godot::Dictionary &options) const {
    return true;
}

godot::Error GinGraphImporter::_import(const godot::String &source_file, const godot::String &save_path,
                                       const godot::Dictionary &options,
                                       const godot::TypedArray <godot::String> &platform_variants,
                                       const godot::TypedArray <godot::String> &gen_files) const {

    godot::String file_content = godot::FileAccess::get_file_as_string(source_file);

    godot::Ref<GinSerializedGraph> graph{};
    graph.instantiate();
    graph->set_serialized_graph(file_content);

    return godot::ResourceSaver::get_singleton()->save(graph, save_path + godot::String{ ".res" });
}

GinEditorPlugin::GinEditorPlugin() {
}

GinEditorPlugin::~GinEditorPlugin() {
}

void GinEditorPlugin::_bind_methods() {

}

void GinEditorPlugin::_notification(int p_what) {
    if (p_what == NOTIFICATION_ENTER_TREE)
    {
        importer.instantiate();
        add_import_plugin(importer, false);
    }
    else if (p_what == NOTIFICATION_EXIT_TREE)
    {
        remove_import_plugin(importer);
    }
}