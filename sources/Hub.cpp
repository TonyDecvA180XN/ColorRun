#include "Hub.h"

Hub::Hub(w4::sptr<w4::render::Node> SceneRoot, FLOAT Time) :
	SceneRoot(SceneRoot), Clock(Time)
{
	m_materials["default"s] = w4::resources::Material::getDefault();
	m_materials["lambert"s] = w4::resources::Material::getDefaultLambert();
	m_materials["blinn"s] = w4::resources::Material::getDefaultBlinn();
}

w4::sptr<w4::render::Mesh> Hub::ResolveMesh(INDEX CallerId, const std::string & filename, const std::string & subname)
{
	w4::sptr<w4::resources::Asset> Asset = w4::resources::Asset::get(filename);
	w4::sptr<w4::render::Node> Mesh = Asset->getFirstRoot()->getChild<w4::render::Mesh>(subname)->clone();
	Mesh->setName(filename + "#"s + subname + "#"s + std::to_string(CallerId));
	return Mesh->as<w4::render::Mesh>();
}

w4::sptr<Entity> Hub::ResolveEntity(const std::string & fullname)
{
	return m_entities[fullname];
}

w4::sptr<w4::resources::Texture> Hub::ResolveTexture(const std::string & filename)
{
	return w4::resources::Texture::get(filename);
}

w4::sptr<w4::resources::MaterialInst> Hub::ResolveMaterial(const std::string & filename)
{
	if (!m_materials.count(filename))
	{
		W4_LOG_ERROR(("Unresolved material instance quiried: "s + filename).c_str());
	}
	return m_materials[filename]->createInstance();
}

void Hub::Update(FLOAT Time)
{
	Clock = Time;
}

void Hub::Register(w4::sptr<Entity> InEntity, const std::string & fullname)
{
	m_entities[fullname] = InEntity;
}
