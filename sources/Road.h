#pragma once

#include <W4Framework.h>

#include "Type.h"

class Road
{
private:
	class RoadChunk {
	public:
		RoadChunk(INDEX variation, w4::sptr<w4::render::Node> chunk_mesh);

		void Enable();

		void Disable();
	private:
		INDEX m_varation;
		w4::sptr<w4::render::Node> m_chunkMesh;
	};

public:

	static constexpr FLOAT cChunkSize = 8.0f;

	void LoadMeshes(std::string path, SIZE chunk_variety);

	void BuildMap(SIZE length, w4::sptr<w4::render::Node> root);

	void Update(FLOAT playhead_z_position);

private:
	SIZE m_chunkVariety;
	INDEX m_activeStart{ 0 }, m_activeFinish{ 0 };
	std::vector<w4::sptr<w4::render::Mesh>> m_samples{};
	std::vector<RoadChunk> m_chunks{};
};