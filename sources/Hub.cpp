#include "Hub.h"

Hub::Hub(w4::sptr<w4::render::Node> SceneRoot) :
	SceneRoot(SceneRoot)
{
	m_materials["default"s] = w4::resources::Material::getDefault()->createInstance();
	m_materials["lambert"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	m_materials["blinn"s] = w4::resources::Material::getDefaultBlinn()->createInstance();
}

w4::sptr<w4::render::Mesh> Hub::ResolveMesh(const std::string & filename, const std::string & subname)
{
	if (!m_meshes.count(filename))
	{
		m_meshes[filename] = w4::resources::Asset::get(filename);
	}
	return m_meshes[filename]->getFirstRoot()->getChild<w4::render::Mesh>(subname)->clone()->as<w4::render::Mesh>();
}

w4::sptr<w4::resources::Texture> Hub::ResolveTexture(const std::string & filename)
{
	if (m_textures.count(filename))
	{
		m_textures[filename] = w4::resources::Texture::get(filename);
	}
	return m_textures[filename];
}

w4::sptr<w4::resources::MaterialInst> Hub::ResolveMaterial(const std::string & filename)
{
	if (!m_materials.count(filename))
	{
		W4_LOG_ERROR(("Unresolved material instance quiried: "s + filename).c_str());
	}
	return m_materials[filename]->getMaterial()->createInstance();
}