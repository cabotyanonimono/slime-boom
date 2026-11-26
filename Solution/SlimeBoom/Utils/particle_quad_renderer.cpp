#include "pch.h"
#include "particle_quad_renderer.h"

namespace SlimeBoom::ParticleQuadRenderer
{
std::shared_ptr<engine::Mesh> Setup(const engine::AssetPtr<engine::MeshRenderer> &renderer, const uint32_t particle_count)
{
    if (renderer.CastedLock() == nullptr)
        return nullptr;

    const auto mesh = engine::Object::Instantiate<engine::Mesh>("ParticleQuad");
    mesh->vertices = {
        Vector3(-1.0f, -1.0f, 0.0f), Vector3(-1.0f, 1.0f, 0.0f), Vector3(1.0f, 1.0f, 0.0f), Vector3(1.0f, -1.0f, 0.0f)
    };
    mesh->normals.assign(4, Vector3(0.0f, 0.0f, -1.0f));
    mesh->uvs[0] = {Vector2(0.0f, 1.0f), Vector2(0.0f, 0.0f), Vector2(1.0f, 0.0f), Vector2(1.0f, 1.0f)};
    mesh->indices = {0, 1, 2, 0, 2, 3};
    mesh->ReconstructMeshesBuffer();

    renderer->SetSharedMesh(engine::AssetPtr<engine::Mesh>::FromInstance(mesh));
    renderer->SetInstanceCount(particle_count);
    return mesh;
}
}
