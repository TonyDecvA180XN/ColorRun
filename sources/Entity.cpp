#include "Entity.h"

Entity::Entity(const std::string asset, const std::string model, const std::string texture, const std::string material) :
	m_assetName(asset),
	m_modelName(model),
	m_textureName(texture),
	m_materialName(material)
{
}

void Entity::BindResources(Hub & hub)
{
	m_mesh = hub.ResolveMesh(m_assetName, m_modelName);
	m_material = hub.ResolveMaterial(m_materialName);

	if (m_textureName.size())
	{
		m_texture = hub.ResolveTexture(m_textureName);
		m_material->setTexture(w4::resources::TextureId::TEXTURE_0, m_texture);
	}
	m_mesh->setMaterialInst(m_material);
}

w4::render::Node & Entity::Transform()
{
	return *m_mesh;
}

w4::resources::MaterialInst & Entity::Material()
{
	return *m_material;
}

void Entity::Parent(w4::sptr<w4::render::Node> parent)
{
	parent->addChild(m_mesh);
}
