#include "Entity.h"

INDEX Entity::FreeId = 1;
w4::sptr<Hub> Entity::LinkToHub = nullptr;

Entity::Entity(Hub & InHub) :
	Id(FreeId++)
{
	if (LinkToHub == nullptr)
	{
		LinkToHub = w4::sptr<Hub>(&InHub);
	}
}

void Entity::SetMesh(std::string Filename, std::string Model)
{
	LinkToHub->Register(w4::sptr<Entity>(this), Filename + "#"s + Model + "#"s + std::to_string(Id));
	Mesh = LinkToHub->ResolveMesh(Id, Filename, Model);
	LinkToHub->GetSceneRoot()->addChild(Mesh);
}

void Entity::SetMesh(w4::sptr<w4::render::Mesh> InMesh)
{
	Mesh = InMesh;
	LinkToHub->GetSceneRoot()->addChild(Mesh);
}

void Entity::SetTexture(std::string Filename)
{
	Texture = LinkToHub->ResolveTexture(Filename);
	if (Material == nullptr)
	{
		W4_LOG_ERROR("TEXTURE CAN BE ASSIGNED ONLY AFTER MATERIAL!");
		return;
	}
	Material->setTexture(w4::resources::TextureId::TEXTURE_0, Texture);
}

void Entity::SetMaterial(std::string Name)
{
	Material = LinkToHub->ResolveMaterial(Name);
	Mesh->setMaterialInst(Material);
}

w4::sptr<w4::render::Node> Entity::GetNode()
{
	return Mesh;
}

w4::sptr<w4::resources::MaterialInst> Entity::GetMaterial()
{
	return Material;
}

void Entity::Parent(w4::sptr<w4::render::Node> Parent)
{
	Parent->addChild(Mesh);
}
