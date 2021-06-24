#pragma once

#include <W4Framework.h>
#include "Type.h"

using namespace std::string_literals;

class Entity;

class Hub
{
public:
	Hub() = delete;
	Hub(const Hub & Other) = delete;
	Hub(Hub && Other) = default;
	Hub(w4::sptr<w4::render::Node> SceneRoot, FLOAT Time);

	w4::sptr<w4::render::Mesh> ResolveMesh(INDEX CallerId, const std::string & filename, const std::string & subname = ""s);
	w4::sptr<Entity> ResolveEntity(const std::string & fullname);
	w4::sptr<w4::resources::Texture> ResolveTexture(const std::string & filename);
	w4::sptr<w4::resources::MaterialInst> ResolveMaterial(const std::string & filename);

	void Update(FLOAT Time);

	FLOAT GetClock() const { return Clock; };

	w4::sptr<w4::render::Node> GetSceneRoot() const { return SceneRoot; };

private:
	friend class Entity;

	void Register(w4::sptr<Entity> Entity, const std::string & fullname);

	FLOAT Clock { 0.f };
	w4::sptr<w4::render::Node> SceneRoot {};

	std::unordered_map<std::string, w4::sptr<w4::resources::Material>> m_materials {};
	std::unordered_map<std::string, w4::sptr<Entity>> m_entities {};
};
