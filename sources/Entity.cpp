#include "Entity.h"

INDEX Entity::FreeId = 1;
w4::sptr<Hub> Entity::LinkToHub = nullptr;

Entity::Entity(Hub & InHub, EMeshType InMeshType) :
	Id(FreeId++),
	MeshType(InMeshType)
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

void Entity::SetSkinnedMesh(std::string Filename, std::string Model)
{
	LinkToHub->Register(w4::sptr<Entity>(this), Filename + "#"s + Model + "#"s + std::to_string(Id));
	SkinnedMesh = LinkToHub->ResolveSkinnedMesh(Id, Filename, Model);
	SkinnedMesh->traversalTyped<w4::render::SkinnedMesh>([](w4::cref<w4::render::SkinnedMesh> Node)
														 {
															for (std::string & AnimationName : Node->getAvailableAnimations())
															{
																//W4_LOG_INFO(AnimationName.c_str());
																Node->getAnimator(AnimationName).setIsLooped(TRUE);
															}
														 });
	LinkToHub->GetSceneRoot()->addChild(SkinnedMesh);
}

void Entity::SetTexture(std::string Filename)
{
	Texture = LinkToHub->ResolveTexture(Filename);
	//Texture->setFiltering(w4::resources::Filtering::Level1);
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
	if (MeshType == EMeshType::Skinned)
	{
		SkinnedMesh->traversalTyped<w4::render::SkinnedMesh>([this](w4::cref<w4::render::SkinnedMesh> Node) { Node->setMaterialInst(Material); });
	}
	else if (MeshType == EMeshType::Static)
	{
		Mesh->traversalTyped<w4::render::Mesh>([this](w4::cref<w4::render::Mesh> Node) { Node->setMaterialInst(Material); });
	}
}

void Entity::SetColorMaterial(std::string Name)
{
	Material = LinkToHub->ResolveColor(Name);
	if (MeshType == EMeshType::Static)
	{
		Mesh->traversalTyped<w4::render::Mesh>([this](w4::cref<w4::render::Mesh> Node) { Node->setMaterialInst(Material); });
	}
}

void Entity::Play(std::string AnimationName)
{
	if (SkinnedMesh != nullptr)
	{
		//W4_LOG_INFO(("Playing " + AnimationName).c_str());
		SkinnedMesh->traversalTyped<w4::render::SkinnedMesh>([AnimationName](w4::cref<w4::render::SkinnedMesh> Node)
															 {
																 Node->stop();
																 Node->play(AnimationName);
															 });
	}
	else
	{
		W4_LOG_ERROR("NOT A SKINNED MESH!");
	}
}

w4::sptr<w4::render::Node> Entity::GetNode() const
{
	if (MeshType == EMeshType::Skinned)
	{
		return SkinnedMesh;
	}
	else
	{
		return Mesh;
	}
}

w4::sptr<w4::resources::MaterialInst> Entity::GetMaterial()
{
	return Material;
}

void Entity::Parent(w4::sptr<w4::render::Node> Parent)
{
	if (MeshType == EMeshType::Skinned)
	{
		Parent->addChild(SkinnedMesh);
	}
	else if (MeshType == EMeshType::Static)
	{
		Parent->addChild(Mesh);
	}
}
