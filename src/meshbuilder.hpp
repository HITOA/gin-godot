//
// Created by HITO on 24/04/24.
//

#ifndef GIN_GODOT_MESHBUILDER_HPP
#define GIN_GODOT_MESHBUILDER_HPP

#include <godot_cpp/classes/array_mesh.hpp>
#include "gingraph.hpp"

class GinMeshBuilder : public godot::RefCounted {
GDCLASS(GinMeshBuilder, godot::RefCounted);

protected:
    static void _bind_methods();

public:
    godot::Ref<godot::ArrayMesh> build_volume(godot::Ref<GinGraphPort> volume_port, float scale, godot::AABB bound);
};

#endif //GIN_GODOT_MESHBUILDER_HPP
