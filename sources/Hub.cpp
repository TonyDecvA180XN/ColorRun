#include "Hub.h"

Hub::Hub(w4::sptr<w4::render::Node> SceneRoot, FLOAT Time) :
	SceneRoot(SceneRoot), Clock(Time)
{
	Materials["default"s] = w4::resources::Material::getDefault();
	Materials["lambert"s] = w4::resources::Material::getDefaultLambert();
	Materials["blinn"s] = w4::resources::Material::getDefaultBlinn();
}

w4::sptr<w4::render::Mesh> Hub::ResolveMesh(INDEX CallerId, const std::string & Filename, const std::string & Model)
{
	w4::sptr<w4::resources::Asset> Asset = w4::resources::Asset::get(Filename);
	w4::sptr<w4::render::Node> Mesh = Asset->getFirstRoot()->getChild<w4::render::Mesh>(Model)->clone();
	Mesh->setName(Filename + "#"s + Model + "#"s + std::to_string(CallerId));
	return Mesh->as<w4::render::Mesh>();
}

w4::sptr<Entity> Hub::ResolveEntity(const std::string & Name)
{
	return Entities[Name];
}

w4::sptr<w4::resources::Texture> Hub::ResolveTexture(const std::string & Filename)
{
	return w4::resources::Texture::get(Filename);
}

w4::sptr<w4::resources::MaterialInst> Hub::ResolveMaterial(const std::string & Name)
{
	if (!Materials.count(Name))
	{
		W4_LOG_ERROR(("Unresolved material instance quiried: "s + Name).c_str());
	}
	return Materials[Name]->createInstance();
}

void Hub::Update(FLOAT Time)
{
	Clock = Time;
}

void Hub::Register(w4::sptr<Entity> InEntity, const std::string & Name)
{
	Entities[Name] = InEntity;
}
