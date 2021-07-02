#pragma once

#include <W4Framework.h>
#include "Hub.h"
#include "Type.h"

class Entity
{
public:
	Entity() = delete;
	Entity(const Entity & other) = delete;
	Entity(Entity && other) = default;
	Entity(Hub & InHub, EMeshType InMeshType = EMeshType::Static);
	Entity & operator=(const Entity & Other) = delete;
	Entity & operator=(Entity && Other) = delete;
	virtual ~Entity() = default;

	void SetMesh(std::string Filename, std::string Model);

	void SetMesh(w4::sptr<w4::render::Mesh> InMesh);

	void SetSkinnedMesh(std::string Filename, std::string Model);

	void SetTexture(std::string Filename);

	void SetMaterial(std::string Name);

	void SetColorMaterial(std::string Name);

	virtual void Update(FLOAT PlayheadPosition) {};

	void Play(std::string AnimationName);

	w4::sptr<w4::render::Node> GetNode() const;

	w4::sptr<w4::resources::MaterialInst> GetMaterial();

	void Parent(w4::sptr<w4::render::Node> Parent);

protected:
	static w4::sptr<Hub> LinkToHub;

	INDEX Id { 0 };
	EMeshType MeshType { EMeshType::Static };
	w4::sptr<w4::render::Node> Mesh {};
	w4::sptr<w4::render::SkinnedMesh> SkinnedMesh {};
	w4::sptr<w4::resources::Texture> Texture {};
	w4::sptr<w4::resources::MaterialInst> Material {};

private:
	static INDEX FreeId;
};
