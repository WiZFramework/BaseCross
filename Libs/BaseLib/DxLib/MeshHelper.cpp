/*!
@file MeshHelper.cpp
@brief プリミティブ作成関数等実体
@copyright Copyright (c) 2016 WiZ Tamura Hiroki,Yamanoi Yasushi.
*/
#include "stdafx.h"

namespace basecross {

	const float MeshUtill::SQRT2(1.41421356237309504880f);
	const float MeshUtill::SQRT3(1.73205080756887729352f);
	const float MeshUtill::SQRT6(2.44948974278317809820f);


	void MeshUtill::CreateCylinderCap(vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices,
		size_t tessellation, float height, float radius, bool isTop)
	{
		// indices.
		for (size_t i = 0; i < tessellation - 2; i++)
		{
			size_t i1 = (i + 1) % tessellation;
			size_t i2 = (i + 2) % tessellation;

			if (isTop)
			{
				std::swap(i1, i2);
			}

			size_t vbase = vertices.size();
			indices.push_back((uint16_t)vbase);
			indices.push_back((uint16_t)(vbase + i1));
			indices.push_back((uint16_t)(vbase + i2));
		}

		XMVECTOR normal = g_XMIdentityR1;
		XMVECTOR textureScale = g_XMNegativeOneHalf;

		if (!isTop)
		{
			normal = -normal;
			textureScale *= g_XMNegateX;
		}
		// vertices.
		for (size_t i = 0; i < tessellation; i++)
		{
			XMVECTOR circleVector = GetCircleVector(i, tessellation);

			XMVECTOR position = (circleVector * radius) + (normal * height);

			XMVECTOR textureCoordinate = XMVectorMultiplyAdd(XMVectorSwizzle<0, 2, 3, 3>(circleVector), textureScale, g_XMOneHalf);

			vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinate));
		}
	}

	void  MeshUtill::CreateSquare(float size,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			float HelfSize = size / 2.0f;
			//頂点配列
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 0.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 0.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(-HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(0.0f, 1.0f)));
			vertices.push_back(VertexPositionNormalTexture(XMFLOAT3(HelfSize, -HelfSize, 0), XMFLOAT3(0.0f, 0.0f, -1.0f), XMFLOAT2(1.0f, 1.0f)));
			//インデックスを作成するための配列
			indices.push_back((uint16_t)0);
			indices.push_back((uint16_t)1);
			indices.push_back((uint16_t)2);
			indices.push_back((uint16_t)1);
			indices.push_back((uint16_t)3);
			indices.push_back((uint16_t)2);
		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateCube(float size,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			//Face数は6
			const int FaceCount = 6;
			static const XMVECTORF32 faceNormals[FaceCount] =
			{
				{ 0,  0,  1 },
				{ 0,  0, -1 },
				{ 1,  0,  0 },
				{ -1,  0,  0 },
				{ 0,  1,  0 },
				{ 0, -1,  0 },
			};
			static const XMVECTORF32 textureCoordinates[4] =
			{
				{ 1, 0 },
				{ 1, 1 },
				{ 0, 1 },
				{ 0, 0 },
			};
			//１辺の長さをして位置にするため、半分にする
			size /= 2;
			for (int i = 0; i < FaceCount; i++)
			{
				//法線
				XMVECTOR normal = faceNormals[i];

				XMVECTOR basis = (i >= 4) ? g_XMIdentityR2 : g_XMIdentityR1;

				XMVECTOR side1 = XMVector3Cross(normal, basis);
				XMVECTOR side2 = XMVector3Cross(normal, side1);

				//インデックスの登録
				size_t vbase = vertices.size();
				indices.push_back((uint16_t)vbase + 0);
				indices.push_back((uint16_t)vbase + 1);
				indices.push_back((uint16_t)vbase + 2);

				indices.push_back((uint16_t)vbase + 0);
				indices.push_back((uint16_t)vbase + 2);
				indices.push_back((uint16_t)vbase + 3);
				//頂点の登録
				vertices.push_back(VertexPositionNormalTexture((normal - side1 - side2) * size, normal, textureCoordinates[0]));
				vertices.push_back(VertexPositionNormalTexture((normal - side1 + side2) * size, normal, textureCoordinates[1]));
				vertices.push_back(VertexPositionNormalTexture((normal + side1 + side2) * size, normal, textureCoordinates[2]));
				vertices.push_back(VertexPositionNormalTexture((normal + side1 - side2) * size, normal, textureCoordinates[3]));
			}
			//RHからLHに変更
			ReverseWinding(indices, vertices);
		}
		catch (...) {
			throw;
		}
	}


	void MeshUtill::CreateSphere(float diameter, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			if (tessellation < 3) {
				// 初期化失敗
				ThrowBaseException(
					L"分割数は3以上必要です",
					L"if (tessellation < 3)",
					L"VertexUtil::CreateSphere()"
				);
			}
			float radius = diameter / 2;
			size_t verticalSegments = tessellation;
			size_t horizontalSegments = tessellation * 2;
			for (size_t i = 0; i <= verticalSegments; i++)
			{
				float v = 1 - (float)i / verticalSegments;

				float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
				float dy, dxz;

				XMScalarSinCos(&dy, &dxz, latitude);

				// Create a single ring of vertices at this latitude.
				for (size_t j = 0; j <= horizontalSegments; j++)
				{
					float u = (float)j / horizontalSegments;

					float longitude = j * XM_2PI / horizontalSegments;
					float dx, dz;

					XMScalarSinCos(&dx, &dz, longitude);

					dx *= dxz;
					dz *= dxz;

					XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
					XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

					vertices.push_back(VertexPositionNormalTexture(normal * radius, normal, textureCoordinate));
				}
			}
			size_t stride = horizontalSegments + 1;
			for (size_t i = 0; i < verticalSegments; i++)
			{
				for (size_t j = 0; j <= horizontalSegments; j++)
				{
					size_t nextI = i + 1;
					size_t nextJ = (j + 1) % stride;

					indices.push_back((uint16_t)(i * stride + j));
					indices.push_back((uint16_t)(nextI * stride + j));
					indices.push_back((uint16_t)(i * stride + nextJ));

					indices.push_back((uint16_t)(i * stride + nextJ));
					indices.push_back((uint16_t)(nextI * stride + j));
					indices.push_back((uint16_t)(nextI * stride + nextJ));
				}
			}
			//RHからLHに変更
			ReverseWinding(indices, vertices);
		}
		catch (...) {
			throw;
		}
	}


	void MeshUtill::CreateCapsule(float diameter,
		const Vector3& PointA, const Vector3& PointB,
		size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			if (tessellation < 3) {
				// 初期化失敗
				ThrowBaseException(
					L"分割数は3以上必要です",
					L"if (tessellation < 3)",
					L"VertexUtil::CreateCapsule()"
				);
			}
			float radius = diameter / 2;
			size_t verticalSegments = tessellation;
			size_t horizontalSegments = tessellation * 2;
			for (size_t i = 0; i <= verticalSegments; i++)
			{


				float v = 1 - (float)i / verticalSegments;

				float latitude = (i * XM_PI / verticalSegments) - XM_PIDIV2;
				float dy, dxz;

				XMScalarSinCos(&dy, &dxz, latitude);

				Vector3 CenterPos = PointA;
				if (i >= (verticalSegments / 2)) {
					CenterPos = PointB;
				}

				float SphereTotalFront = diameter *  XM_PI / 2.0f;
				float SylinderTotalFront = abs(PointA.y - PointB.y);
				float SphereRate = SphereTotalFront / (SphereTotalFront + SylinderTotalFront) / 2.0f;

				if (v <= 0.5f) {
					v *= SphereRate;
				}
				else {
					v = 1.0f - ((1.0f - v) * SphereRate);
				}


				for (size_t j = 0; j <= horizontalSegments; j++)
				{
					float u = (float)j / horizontalSegments;

					float longitude = j * XM_2PI / horizontalSegments;
					float dx, dz;

					XMScalarSinCos(&dx, &dz, longitude);

					dx *= dxz;
					dz *= dxz;

					XMVECTOR normal = XMVectorSet(dx, dy, dz, 0);
					XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

					vertices.push_back(VertexPositionNormalTexture(normal * radius + (XMVECTOR)CenterPos, normal, textureCoordinate));
				}
			}
			size_t stride = horizontalSegments + 1;
			for (size_t i = 0; i < verticalSegments; i++)
			{
				for (size_t j = 0; j <= horizontalSegments; j++)
				{
					size_t nextI = i + 1;
					size_t nextJ = (j + 1) % stride;

					indices.push_back((uint16_t)(i * stride + j));
					indices.push_back((uint16_t)(nextI * stride + j));
					indices.push_back((uint16_t)(i * stride + nextJ));

					indices.push_back((uint16_t)(i * stride + nextJ));
					indices.push_back((uint16_t)(nextI * stride + j));
					indices.push_back((uint16_t)(nextI * stride + nextJ));
				}
			}
			//RHからLHに変更
			ReverseWinding(indices, vertices);
		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateCylinder(float height, float diameter, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices)
	{
		try {

			if (tessellation < 3) {
				// 初期化失敗
				ThrowBaseException(
					L"分割数は3以上必要です",
					L"if (tessellation < 3)",
					L"VertexUtil::CreateCylinder()"
				);
			}

			height /= 2;

			XMVECTOR topOffset = g_XMIdentityR1 * height;

			float radius = diameter / 2;
			size_t stride = tessellation + 1;

			for (size_t i = 0; i <= tessellation; i++)
			{
				XMVECTOR normal = GetCircleVector(i, tessellation);

				XMVECTOR sideOffset = normal * radius;

				float u = (float)i / tessellation;

				XMVECTOR textureCoordinate = XMLoadFloat(&u);

				vertices.push_back(VertexPositionNormalTexture(sideOffset + topOffset, normal, textureCoordinate));
				vertices.push_back(VertexPositionNormalTexture(sideOffset - topOffset, normal, textureCoordinate + g_XMIdentityR1));

				indices.push_back((uint16_t)(i * 2));
				indices.push_back((uint16_t)((i * 2 + 2) % (stride * 2)));
				indices.push_back((uint16_t)(i * 2 + 1));

				indices.push_back((uint16_t)(i * 2 + 1));
				indices.push_back((uint16_t)((i * 2 + 2) % (stride * 2)));
				indices.push_back((uint16_t)((i * 2 + 3) % (stride * 2)));
			}
			CreateCylinderCap(vertices, indices, tessellation, height, radius, true);
			CreateCylinderCap(vertices, indices, tessellation, height, radius, false);
			//RHからLHに変更
			ReverseWinding(indices, vertices);


		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateCone(float diameter, float height, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			if (tessellation < 3) {
				// 初期化失敗
				ThrowBaseException(
					L"分割数は3以上必要です",
					L"if (tessellation < 3)",
					L"VertexUtil::CreateCone()"
				);
			}

			height /= 2;

			XMVECTOR topOffset = g_XMIdentityR1 * height;

			float radius = diameter / 2;
			size_t stride = tessellation + 1;

			for (size_t i = 0; i <= tessellation; i++)
			{
				XMVECTOR circlevec = GetCircleVector(i, tessellation);

				XMVECTOR sideOffset = circlevec * radius;

				float u = (float)i / tessellation;

				XMVECTOR textureCoordinate = XMLoadFloat(&u);

				XMVECTOR pt = sideOffset - topOffset;

				XMVECTOR normal = XMVector3Cross(GetCircleTangent(i, tessellation), topOffset - pt);
				normal = XMVector3Normalize(normal);

				vertices.push_back(VertexPositionNormalTexture(topOffset, normal, g_XMZero));
				vertices.push_back(VertexPositionNormalTexture(pt, normal, textureCoordinate + g_XMIdentityR1));

				indices.push_back((uint16_t)(i * 2));
				indices.push_back((uint16_t)((i * 2 + 3) % (stride * 2)));
				indices.push_back((uint16_t)((i * 2 + 1) % (stride * 2)));
			}

			CreateCylinderCap(vertices, indices, tessellation, height, radius, false);
			//RHからLHに変更
			ReverseWinding(indices, vertices);


		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateTorus(float diameter, float thickness, size_t tessellation,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {
			if (tessellation < 3) {
				// 初期化失敗
				ThrowBaseException(
					L"分割数は3以上必要です",
					L"if (tessellation < 3)",
					L"VertexUtil::CreateTorus()"
				);
			}

			size_t stride = tessellation + 1;

			for (size_t i = 0; i <= tessellation; i++)
			{
				float u = (float)i / tessellation;

				float outerAngle = i * XM_2PI / tessellation - XM_PIDIV2;

				XMMATRIX transform = XMMatrixTranslation(diameter / 2, 0, 0) * XMMatrixRotationY(outerAngle);

				for (size_t j = 0; j <= tessellation; j++)
				{
					float v = 1 - (float)j / tessellation;

					float innerAngle = j * XM_2PI / tessellation + XM_PI;
					float dx, dy;

					XMScalarSinCos(&dy, &dx, innerAngle);

					XMVECTOR normal = XMVectorSet(dx, dy, 0, 0);
					XMVECTOR position = normal * thickness / 2;
					XMVECTOR textureCoordinate = XMVectorSet(u, v, 0, 0);

					position = XMVector3Transform(position, transform);
					normal = XMVector3TransformNormal(normal, transform);

					vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinate));

					size_t nextI = (i + 1) % stride;
					size_t nextJ = (j + 1) % stride;

					indices.push_back((uint16_t)(i * stride + j));
					indices.push_back((uint16_t)(i * stride + nextJ));
					indices.push_back((uint16_t)(nextI * stride + j));

					indices.push_back((uint16_t)(i * stride + nextJ));
					indices.push_back((uint16_t)(nextI * stride + nextJ));
					indices.push_back((uint16_t)(nextI * stride + j));
				}
			}

			//RHからLHに変更
			ReverseWinding(indices, vertices);


		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateTetrahedron(float size,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {

			static const XMVECTORF32 verts[4] =
			{
				{ 0.f,        0.f,      1.f },
				{ 2.f*SQRT2 / 3.f,        0.f, -1.f / 3.f },
				{ -SQRT2 / 3.f,  SQRT6 / 3.f, -1.f / 3.f },
				{ -SQRT2 / 3.f, -SQRT6 / 3.f, -1.f / 3.f }
			};

			static const uint32_t faces[4 * 3] =
			{
				0, 1, 2,
				0, 2, 3,
				0, 3, 1,
				1, 3, 2,
			};

			for (size_t j = 0; j < _countof(faces); j += 3)
			{
				uint32_t v0 = faces[j];
				uint32_t v1 = faces[j + 1];
				uint32_t v2 = faces[j + 2];

				XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
					verts[v2].v - verts[v0].v);
				normal = XMVector3Normalize(normal);

				size_t base = vertices.size();
				indices.push_back((uint16_t)base);
				indices.push_back((uint16_t)base + 1);
				indices.push_back((uint16_t)base + 2);

				// Duplicate vertices to use face normals
				XMVECTOR position = XMVectorScale(verts[v0], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMZero /* 0, 0 */));

				position = XMVectorScale(verts[v1], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMIdentityR0 /* 1, 0 */));

				position = XMVectorScale(verts[v2], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMIdentityR1 /* 0, 1 */));
			}

			if (vertices.size() != 4 * 3) {
				ThrowBaseException(
					L"頂点の数が合いません",
					L"if(vertices.size() != 4*3)",
					L"VertexUtil::CreateTetrahedron()"
				);
			}
			if (indices.size() != 4 * 3) {
				ThrowBaseException(
					L"インデックスの数が合いません",
					L"if(indices.size() != 4*3)",
					L"VertexUtil::CreateTetrahedron()"
				);
			}

		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateOctahedron(float size,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {

			static const XMVECTORF32 verts[6] =
			{
				{ 1,  0,  0 },
				{ -1,  0,  0 },
				{ 0,  1,  0 },
				{ 0, -1,  0 },
				{ 0,  0,  1 },
				{ 0,  0, -1 }
			};

			static const uint32_t faces[8 * 3] =
			{
				4, 0, 2,
				4, 2, 1,
				4, 1, 3,
				4, 3, 0,
				5, 2, 0,
				5, 1, 2,
				5, 3, 1,
				5, 0, 3
			};

			for (size_t j = 0; j < _countof(faces); j += 3)
			{
				uint32_t v0 = faces[j];
				uint32_t v1 = faces[j + 1];
				uint32_t v2 = faces[j + 2];

				XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
					verts[v2].v - verts[v0].v);
				normal = XMVector3Normalize(normal);

				size_t base = vertices.size();
				indices.push_back((uint16_t)base);
				indices.push_back((uint16_t)base + 1);
				indices.push_back((uint16_t)base + 2);

				XMVECTOR position = XMVectorScale(verts[v0], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMZero /* 0, 0 */));

				position = XMVectorScale(verts[v1], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMIdentityR0 /* 1, 0 */));

				position = XMVectorScale(verts[v2], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMIdentityR1 /* 0, 1*/));
			}

			if (vertices.size() != 8 * 3) {
				ThrowBaseException(
					L"頂点の数が合いません",
					L"if(vertices.size() != 8*3)",
					L"VertexUtil::CreateOctahedron()"
				);
			}
			if (indices.size() != 8 * 3) {
				ThrowBaseException(
					L"インデックスの数が合いません",
					L"if(indices.size() != 8*3)",
					L"VertexUtil::CreateOctahedron()"
				);
			}

		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateDodecahedron(float size,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {

			static const float a = 1.f / SQRT3;
			static const float b = 0.356822089773089931942f; // sqrt( ( 3 - sqrt(5) ) / 6 )
			static const float c = 0.934172358962715696451f; // sqrt( ( 3 + sqrt(5) ) / 6 );

			static const XMVECTORF32 verts[20] =
			{
				{ a,  a,  a },
				{ a,  a, -a },
				{ a, -a,  a },
				{ a, -a, -a },
				{ -a,  a,  a },
				{ -a,  a, -a },
				{ -a, -a,  a },
				{ -a, -a, -a },
				{ b,  c,  0 },
				{ -b,  c,  0 },
				{ b, -c,  0 },
				{ -b, -c,  0 },
				{ c,  0,  b },
				{ c,  0, -b },
				{ -c,  0,  b },
				{ -c,  0, -b },
				{ 0,  b,  c },
				{ 0, -b,  c },
				{ 0,  b, -c },
				{ 0, -b, -c }
			};

			static const uint32_t faces[12 * 5] =
			{
				0, 8, 9, 4, 16,
				0, 16, 17, 2, 12,
				12, 2, 10, 3, 13,
				9, 5, 15, 14, 4,
				3, 19, 18, 1, 13,
				7, 11, 6, 14, 15,
				0, 12, 13, 1, 8,
				8, 1, 18, 5, 9,
				16, 4, 14, 6, 17,
				6, 11, 10, 2, 17,
				7, 15, 5, 18, 19,
				7, 19, 3, 10, 11,
			};

			static const XMVECTORF32 textureCoordinates[5] =
			{
				{ 0.654508f, 0.0244717f },
				{ 0.0954915f,  0.206107f },
				{ 0.0954915f,  0.793893f },
				{ 0.654508f,  0.975528f },
				{ 1.f,       0.5f }
			};

			static const uint32_t textureIndex[12][5] =
			{
				{ 0, 1, 2, 3, 4 },
				{ 2, 3, 4, 0, 1 },
				{ 4, 0, 1, 2, 3 },
				{ 1, 2, 3, 4, 0 },
				{ 2, 3, 4, 0, 1 },
				{ 0, 1, 2, 3, 4 },
				{ 1, 2, 3, 4, 0 },
				{ 4, 0, 1, 2, 3 },
				{ 4, 0, 1, 2, 3 },
				{ 1, 2, 3, 4, 0 },
				{ 0, 1, 2, 3, 4 },
				{ 2, 3, 4, 0, 1 },
			};

			size_t t = 0;
			for (size_t j = 0; j < _countof(faces); j += 5, ++t)
			{
				uint32_t v0 = faces[j];
				uint32_t v1 = faces[j + 1];
				uint32_t v2 = faces[j + 2];
				uint32_t v3 = faces[j + 3];
				uint32_t v4 = faces[j + 4];

				XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
					verts[v2].v - verts[v0].v);
				normal = XMVector3Normalize(normal);

				size_t base = vertices.size();

				indices.push_back((uint16_t)base);
				indices.push_back((uint16_t)base + 1);
				indices.push_back((uint16_t)base + 2);

				indices.push_back((uint16_t)base);
				indices.push_back((uint16_t)base + 2);
				indices.push_back((uint16_t)base + 3);

				indices.push_back((uint16_t)base);
				indices.push_back((uint16_t)base + 3);
				indices.push_back((uint16_t)base + 4);

				// Duplicate vertices to use face normals
				XMVECTOR position = XMVectorScale(verts[v0], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinates[textureIndex[t][0]]));

				position = XMVectorScale(verts[v1], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinates[textureIndex[t][1]]));

				position = XMVectorScale(verts[v2], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinates[textureIndex[t][2]]));

				position = XMVectorScale(verts[v3], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinates[textureIndex[t][3]]));

				position = XMVectorScale(verts[v4], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, textureCoordinates[textureIndex[t][4]]));
			}

			if (vertices.size() != 12 * 5) {
				ThrowBaseException(
					L"頂点の数が合いません",
					L"if(vertices.size() != 12*5)",
					L"VertexUtil::CreateDodecahedron()"
				);
			}
			if (indices.size() != 12 * 3 * 3) {
				ThrowBaseException(
					L"インデックスの数が合いません",
					L"if(indices.size() != 12*3*3)",
					L"VertexUtil::CreateDodecahedron()"
				);
			}

		}
		catch (...) {
			throw;
		}
	}

	void MeshUtill::CreateIcosahedron(float size,
		vector<VertexPositionNormalTexture>& vertices, vector<uint16_t>& indices) {
		try {

			static const float  t = 1.618033988749894848205f; // (1 + sqrt(5)) / 2
			static const float t2 = 1.519544995837552493271f; // sqrt( 1 + sqr( (1 + sqrt(5)) / 2 ) )

			static const XMVECTORF32 verts[12] =
			{
				{ t / t2,  1.f / t2,       0 },
				{ -t / t2,  1.f / t2,       0 },
				{ t / t2, -1.f / t2,       0 },
				{ -t / t2, -1.f / t2,       0 },
				{ 1.f / t2,       0,    t / t2 },
				{ 1.f / t2,       0,   -t / t2 },
				{ -1.f / t2,       0,    t / t2 },
				{ -1.f / t2,       0,   -t / t2 },
				{ 0,    t / t2,  1.f / t2 },
				{ 0,   -t / t2,  1.f / t2 },
				{ 0,    t / t2, -1.f / t2 },
				{ 0,   -t / t2, -1.f / t2 }
			};

			static const uint32_t faces[20 * 3] =
			{
				0, 8, 4,
				0, 5, 10,
				2, 4, 9,
				2, 11, 5,
				1, 6, 8,
				1, 10, 7,
				3, 9, 6,
				3, 7, 11,
				0, 10, 8,
				1, 8, 10,
				2, 9, 11,
				3, 11, 9,
				4, 2, 0,
				5, 0, 2,
				6, 1, 3,
				7, 3, 1,
				8, 6, 4,
				9, 4, 6,
				10, 5, 7,
				11, 7, 5
			};

			for (size_t j = 0; j < _countof(faces); j += 3)
			{
				uint32_t v0 = faces[j];
				uint32_t v1 = faces[j + 1];
				uint32_t v2 = faces[j + 2];

				XMVECTOR normal = XMVector3Cross(verts[v1].v - verts[v0].v,
					verts[v2].v - verts[v0].v);
				normal = XMVector3Normalize(normal);

				size_t base = vertices.size();
				indices.push_back((uint16_t)base);
				indices.push_back((uint16_t)base + 1);
				indices.push_back((uint16_t)base + 2);

				// Duplicate vertices to use face normals
				XMVECTOR position = XMVectorScale(verts[v0], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMZero /* 0, 0 */));

				position = XMVectorScale(verts[v1], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMIdentityR0 /* 1, 0 */));

				position = XMVectorScale(verts[v2], size);
				vertices.push_back(VertexPositionNormalTexture(position, normal, g_XMIdentityR1 /* 0, 1 */));
			}

			if (vertices.size() != 20 * 3) {
				ThrowBaseException(
					L"頂点の数が合いません",
					L"if(vertices.size() != 20*3)",
					L"VertexUtil::CreateIcosahedron()"
				);
			}
			if (indices.size() != 20 * 3) {
				ThrowBaseException(
					L"インデックスの数が合いません",
					L"if(indices.size() != 20*3)",
					L"VertexUtil::CreateIcosahedron()"
				);
			}


		}
		catch (...) {
			throw;
		}
	}


}
//end basecross