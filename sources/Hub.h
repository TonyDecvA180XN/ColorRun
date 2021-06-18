#pragma once

#include <W4Framework.h>
#include "Type.h"

using namespace std::string_literals;

class Hub
{
public:
	void Init();

	w4::sptr<w4::render::Mesh> ResolveMesh(const std::string & filename, const std::string & subname = ""s);
	w4::sptr<w4::resources::Texture> ResolveTexture(const std::string & filename);
	w4::sptr<w4::resources::MaterialInst> ResolveMaterial(const std::string & filename);

private:
	std::unordered_map<std::string, w4::sptr<w4::resources::Asset>> m_meshes{};
	std::unordered_map<std::string, w4::sptr<w4::resources::Texture>> m_textures{};
	std::unordered_map<std::string, w4::sptr<w4::resources::MaterialInst>> m_materials{};
};
