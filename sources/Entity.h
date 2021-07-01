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
	Entity(Hub & InHub);
	Entity & operator=(const Entity & Other) = delete;
	Entity & operator=(Entity && Other) = delete;
	virtual ~Entity() = default;

	void SetMesh(std::string Filename, std::string Model);

	void SetMesh(w4::sptr<w4::render::Mesh> InMesh);

	void SetTexture(std::string Filename);

	void SetMaterial(std::string Name);

	virtual void Update(FLOAT PlayheadPosition) {};

	w4::sptr<w4::render::Node> GetNode();

	w4::sptr<w4::resources::MaterialInst> GetMaterial();

	void Parent(w4::sptr<w4::render::Node> Parent);

protected:
	static w4::sptr<Hub> LinkToHub;

	INDEX Id { 0 };
	w4::sptr<w4::render::Mesh> Mesh {};
	w4::sptr<w4::resources::Texture> Texture {};
	w4::sptr<w4::resources::MaterialInst> Material {};

private:
	static INDEX FreeId;
};
