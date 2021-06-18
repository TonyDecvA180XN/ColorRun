#pragma once

#include <W4Framework.h>
#include "Hub.h"

class Entity
{
public:
	Entity() = delete;
	Entity(const Entity & other) = delete;
	Entity(
		const std::string asset,
		const std::string model,
		const std::string texture,
		const std::string material
	);

	void BindResources(Hub & hub);

	w4::render::Node & Transform();

	w4::resources::MaterialInst & Material();

	void Parent(w4::sptr<w4::render::Node> parent);

	virtual ~Entity() {};

protected:
	const std::string m_assetName;
	const std::string m_modelName;
	const std::string m_textureName;
	const std::string m_materialName;

	w4::sptr<w4::render::Mesh> m_mesh;
	w4::sptr<w4::resources::Texture> m_texture;
	w4::sptr<w4::resources::MaterialInst> m_material;
};
