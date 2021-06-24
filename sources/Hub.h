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

	w4::sptr<w4::render::Mesh> ResolveMesh(INDEX CallerId, const std::string & Filename, const std::string & Model = ""s);
	w4::sptr<Entity> ResolveEntity(const std::string & Name);
	w4::sptr<w4::resources::Texture> ResolveTexture(const std::string & Filename);
	w4::sptr<w4::resources::MaterialInst> ResolveMaterial(const std::string & Name);

	void Update(FLOAT Time);

	FLOAT GetClock() const { return Clock; };

	w4::sptr<w4::render::Node> GetSceneRoot() const { return SceneRoot; };

private:
	friend class Entity;

	void Register(w4::sptr<Entity> Entity, const std::string & Name);

	FLOAT Clock { 0.f };
	w4::sptr<w4::render::Node> SceneRoot {};

	std::unordered_map<std::string, w4::sptr<w4::resources::Material>> Materials {};
	std::unordered_map<std::string, w4::sptr<Entity>> Entities {};
};
