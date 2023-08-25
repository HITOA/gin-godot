#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include <module.hpp>

#include "graph.hpp"
#include "mesh.hpp"

void initialize_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	Initialize();

	godot::ClassDB::register_class<GinGraphContext>();
	godot::ClassDB::register_class<GinGraph>();
	godot::ClassDB::register_class<GinMeshBuilder>();
}

void uninitialize_module(godot::ModuleInitializationLevel p_level) {
	if (p_level != godot::MODULE_INITIALIZATION_LEVEL_SCENE) {
		return;
	}

	Uninitialize();
}

extern "C" {
	
	GDExtensionBool GDE_EXPORT GinGodotEntry(const GDExtensionInterface* p_interface, GDExtensionClassLibraryPtr p_library, GDExtensionInitialization* r_initialization) {
		godot::GDExtensionBinding::InitObject initObj(p_interface, p_library, r_initialization);

		initObj.register_initializer(initialize_module);
		initObj.register_terminator(uninitialize_module);
		initObj.set_minimum_library_initialization_level(godot::MODULE_INITIALIZATION_LEVEL_SCENE);

		return initObj.init();
	}

}