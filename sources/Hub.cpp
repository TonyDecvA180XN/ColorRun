#include "Hub.h"

Hub::Hub(w4::sptr<w4::render::Node> SceneRoot, FLOAT Time) :
	SceneRoot(SceneRoot), Clock(Time)
{
	Materials["default"s] = w4::resources::Material::getDefault();
	Materials["lambert"s] = w4::resources::Material::getDefaultLambert();
	Materials["blinn"s] = w4::resources::Material::getDefaultBlinn();

	PredefinedMaterials["green"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	PredefinedMaterials["brown"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	PredefinedMaterials["grey"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	PredefinedMaterials["yellow"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	PredefinedMaterials["darkgrey"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	PredefinedMaterials["darkgreen"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	PredefinedMaterials["blue"s] = w4::resources::Material::getDefaultLambert()->createInstance();
	PredefinedMaterials["terrain"s] = w4::resources::Material::getDefaultLambert()->createInstance();

	auto TexId = w4::resources::TextureId::TEXTURE_0;
	PredefinedMaterials["green"s]->setTexture(TexId, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, w4::math::color(0x2AD84FFF))));
	PredefinedMaterials["brown"s]->setTexture(TexId, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, w4::math::color(0x966F5FFF))));
	PredefinedMaterials["grey"s]->setTexture(TexId, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, w4::math::color(0x8C8781FF))));
	PredefinedMaterials["yellow"s]->setTexture(TexId, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, w4::math::color(0xFFE642FF))));
	PredefinedMaterials["darkgrey"s]->setTexture(TexId, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, w4::math::color(0x2D2D30FF))));
	PredefinedMaterials["darkgreen"s]->setTexture(TexId, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, w4::math::color(0x0D9E3DFF))));
	PredefinedMaterials["blue"s]->setTexture(TexId, w4::resources::Texture::get(w4::resources::ResourceGenerator(w4::resources::ColorTextureGenerator, w4::math::color(0x007ACCFF))));
}

w4::sptr<w4::render::Node> Hub::ResolveMesh(INDEX CallerId, const std::string & Filename, const std::string & Model)
{
	w4::sptr<w4::render::Node> AssetRoot = w4::resources::Asset::get(Filename)->getFirstRoot();
	w4::sptr<w4::render::Node> Mesh;
	if (Model != "")
		Mesh = AssetRoot->getChild<w4::render::Mesh>(Model)->clone();
	else
		Mesh = AssetRoot->clone();
	Mesh->setName(Filename + "#"s + Model + "#"s + std::to_string(CallerId));
	//Mesh->setWorldScale({ 1.f, 1.f, 1.f });
	//Mesh->setWorldRotation({ 0.f, 0.f, 0.f });
	return Mesh;
}

w4::sptr<w4::render::SkinnedMesh> Hub::ResolveSkinnedMesh(INDEX CallerId, const std::string & Filename, const std::string & Model)
{
	w4::sptr<w4::resources::Asset> Asset = w4::resources::Asset::get(Filename);
	w4::sptr<w4::render::Node> Node = Asset->getFirstRoot();
	//for (auto s : Node->getAllChildren())
	//{
	//	W4_LOG_INFO(s->getName().c_str());
	//}
	Node = Node->getChild<w4::render::SkinnedMesh>(Model)->clone();
	Node->setName(Filename + "#"s + Model + "#"s + std::to_string(CallerId));
	Node->setWorldScale({ 1.f, 1.f, 1.f });
	Node->setWorldRotation({ 0.f, 0.f, 0.f });
	return Node->as<w4::render::SkinnedMesh>();
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

w4::sptr<w4::resources::MaterialInst> Hub::ResolveColor(const std::string & Name)
{
	return PredefinedMaterials[Name];
}

void Hub::Update(FLOAT Time)
{
	Clock = Time;
}

void Hub::Register(w4::sptr<Entity> InEntity, const std::string & Name)
{
	Entities[Name] = InEntity;
}
