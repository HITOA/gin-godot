#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <module.hpp>

#include "gingraph.hpp"
#include "meshbuilder.hpp"

#include <godot_cpp/classes/editor_plugin.hpp>
#include <gdextension_interface.h>

void initialize_module(godot::ModuleInitializationLevel p_level) {
    if (p_level == godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
        Initialize();
        godot::ClassDB::register_class<GinGraphPort>();
        godot::ClassDB::register_class<GinGraph>();
        godot::ClassDB::register_class<GinSerializedGraph>();
        godot::ClassDB::register_class<GinMeshBuilder>();
    }

    if (p_level == godot::MODULE_INITIALIZATION_LEVEL_EDITOR) {
        godot::ClassDB::register_class<GinGraphImporter>();
        godot::ClassDB::register_class<GinEditorPlugin>();

        godot::EditorPlugins::add_by_type<GinEditorPlugin>();
    }
}

void uninitialize_module(godot::ModuleInitializationLevel p_level) {
	if (p_level == godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
        Uninitialize();
	}
}

extern "C" {

    GDExtensionBool GDE_EXPORT gin_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                          GDExtensionClassLibraryPtr p_library, GDExtensionInitialization *r_initialization) {
        godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(initialize_module);
        init_obj.register_terminator(uninitialize_module);
        init_obj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }

}