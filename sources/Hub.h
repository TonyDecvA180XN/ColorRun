#pragma once

#include <W4Framework.h>

using namespace std::string_literals;

class Hub
{
public:
	Hub() = delete;
	Hub(const Hub & Other) = delete;
	Hub(Hub && Other) = default;
	Hub(w4::sptr<w4::render::Node> SceneRoot);

	w4::sptr<w4::render::Mesh> ResolveMesh(const std::string & filename, const std::string & subname = ""s);
	w4::sptr<w4::resources::Texture> ResolveTexture(const std::string & filename);
	w4::sptr<w4::resources::MaterialInst> ResolveMaterial(const std::string & filename);

	w4::sptr<w4::render::Node> GetSceneRoot() const { return SceneRoot; };

private:
	w4::sptr<w4::render::Node> SceneRoot {};

	std::unordered_map<std::string, w4::sptr<w4::resources::Asset>> m_meshes{};
	std::unordered_map<std::string, w4::sptr<w4::resources::Texture>> m_textures{};
	std::unordered_map<std::string, w4::sptr<w4::resources::MaterialInst>> m_materials{};
};
