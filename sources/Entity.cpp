#include "Entity.h"

Entity::Entity(Hub & hub) :
	hub(hub)
{
}

void Entity::SetMesh(std::string filename, std::string model)
{
	m_mesh = hub.ResolveMesh(filename, model);
	hub.GetSceneRoot()->addChild(m_mesh);
}

void Entity::SetTexture(std::string filename)
{
	m_texture = hub.ResolveTexture(filename);
}

void Entity::SetMaterial(std::string name)
{
	m_material = hub.ResolveMaterial(name);
	m_mesh->setMaterialInst(m_material);
}

w4::render::Node & Entity::Transform()
{
	return *m_mesh;
}

w4::sptr<w4::resources::MaterialInst> Entity::Material()
{
	return m_material;
}

void Entity::Parent(w4::sptr<w4::render::Node> parent)
{
	parent->addChild(m_mesh);
}
