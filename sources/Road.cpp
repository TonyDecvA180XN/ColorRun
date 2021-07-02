#include "Road.h"
#include <algorithm>
#include "MathUtils.h"

Road::RoadChunk::RoadChunk(INDEX variation, w4::sptr<w4::render::Node> chunk_mesh) :
	m_variation(variation),
	m_chunkMesh(chunk_mesh)
{
	Disable();
}

void Road::RoadChunk::Enable()
{
	//m_chunkMesh->setEnabled(TRUE);
}

void Road::RoadChunk::Disable()
{
	m_chunkMesh->setEnabled(FALSE);
}

FLOAT Road::GetLength()
{
	return cChunkSize * m_chunks.size();
}

void Road::LoadMeshes(std::string path, SIZE chunk_variety)
{
	m_chunkVariety = chunk_variety;

	m_samples.clear();
	m_samples.reserve(chunk_variety);
	
	auto asset = w4::resources::Asset::get(path)->getFirstRoot();
	for (INDEX i = 0; i != chunk_variety; ++i)
	{
		// build format "00i"
		constexpr SIZE INDEX_LENGTH = 3;
		std::string index = std::to_string(i);
		index = std::string(INDEX_LENGTH - index.size(), '0').append(index);
		
		// load mesh from asset
		auto model = asset->getChild<w4::render::Mesh>(index);
		model->rotateWorld(w4::math::Rotator(0, 180_deg, 0));
		//static auto mat = w4::resources::Material::getDefaultBlinn()->createInstance();
		auto mat = w4::resources::Material::getDefaultLambert()->createInstance();
		auto tex = w4::resources::Texture::get("textures/road.png");
		tex->setFiltering(w4::resources::Filtering::Level1);
		mat->setTexture(w4::resources::TextureId::TEXTURE_0, tex);
		//mat->setParam("baseColor", w4::math::vec4::cref{ 1.f, 0.f, 0.f, 1.f });
		//mat->setParam("specColor", w4::math::vec4::cref{ 0.f, 0.f, 1.f, 1.f });
		model->traversal([=](w4::render::Node & node)
			{
				if (node.is<w4::render::Mesh>())
				{
					auto m = w4::resources::Material::getDefaultLambert()->createInstance();
					if (node.getName() == "000")
					{
						m->setTexture(w4::resources::TextureId::TEXTURE_0, tex);
					}
					if (node.getName() == "rock01")
					{
						m->setTexture(w4::resources::TextureId::TEXTURE_0, w4::resources::Texture::get("textures/rock.jpg"));
					}
					if (node.getName() == "rock02")
					{
						m->setTexture(w4::resources::TextureId::TEXTURE_0, w4::resources::Texture::get("textures/rock.jpg"));
					}
					if (node.getName() == "fence")
					{
						m->setTexture(w4::resources::TextureId::TEXTURE_0, w4::resources::Texture::predefined::white());
					}
					node.as<w4::render::Mesh>()->setMaterialInst(m);
				}
			});
		m_samples.push_back(model);
	}
}

void Road::BuildMap(SIZE length, w4::sptr<w4::render::Node> root)
{
	m_chunks.clear();
	m_chunks.reserve(length);

	SIZE previous = m_chunkVariety;
	SIZE current;

	for (INDEX i = 0; i != length; ++i)
	{
		current = w4::math::random<SIZE>(0, m_chunkVariety - 1);
		while ((m_chunkVariety > 1) && (current == previous))
		{
			current = w4::math::random<SIZE>(0, m_chunkVariety - 1);
		}
		previous = current;

		auto chunk = m_samples[current]->clone();
		chunk->translateWorld({ 0, 0, cChunkSize * i });
		root->addChild(chunk);

		m_chunks.emplace_back(i, chunk);
	}

	m_activeStart = m_activeFinish = m_chunkVariety;
}

void Road::Update(FLOAT playhead_z_position)
{
	static constexpr SIZE cChunkDrawAheadDistnace = 16;
	static constexpr SIZE cChunkDrawBehindDistnace = 1;


	INT64 current = playhead_z_position / cChunkSize;
	INT64 requiredStart = current - cChunkDrawBehindDistnace;
	INT64 requiredFinish = current + cChunkDrawAheadDistnace;
	requiredStart = std::clamp(requiredStart, 0LL, static_cast<INT64>(m_chunks.size()));
	requiredFinish = std::clamp(requiredFinish, 0LL, static_cast<INT64>(m_chunks.size()));

	// interval uninitialized
	if (m_activeStart == m_activeFinish && m_activeStart == m_chunkVariety)
	{
		m_activeStart = requiredStart;
		m_activeFinish = requiredFinish;
		for (INDEX i = m_activeStart; i != m_activeFinish; ++i)
		{
			m_chunks[i].Enable();
		}
		return;
	}
	
	// intervals match
	if (requiredStart == m_activeStart && requiredFinish == m_activeFinish)
	{
		return;
	}

	// active interval behind
	if (m_activeStart < requiredStart || m_activeFinish < requiredFinish)
	{
		while (m_activeStart < requiredStart)
		{
			m_chunks[m_activeStart].Disable();
			++m_activeStart;
		}
		while (m_activeFinish > 0 && m_activeFinish < requiredFinish)
		{
			m_chunks[m_activeFinish - 1].Enable();
			++m_activeFinish;
		}
		return;
	}

	W4_LOG_ERROR("REACHED UNREACHABLE POINT");
}
