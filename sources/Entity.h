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

	void SetMesh(std::string filename, std::string model);

	void SetTexture(std::string filename);

	void SetMaterial(std::string name);

	virtual void Update(FLOAT PlayheadPosition) {};

	w4::sptr<w4::render::Node> Transform();

	w4::sptr<w4::resources::MaterialInst> Material();

	void Parent(w4::sptr<w4::render::Node> parent);

protected:
	static w4::sptr<Hub> hub;
	INDEX Id { 0 };

	w4::sptr<w4::render::Mesh> m_mesh {};
	w4::sptr<w4::resources::Texture> m_texture {};
	w4::sptr<w4::resources::MaterialInst> m_material {};

private:
	static INDEX FreeId;
};
