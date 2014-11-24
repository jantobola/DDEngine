#pragma once

#include "D3DUtils.h"
#include <string>
#include <vector>

namespace DDEngine
{
	struct Texture
	{

		enum TextureType
		{
			DIFFUSE = 0,
			NORMAL = 1,
			SPECULAR = 2,
			AMBIENT = 3,
			EMISSIVE = 4,
			HEIGHT = 5,
			SHININESS = 6,
			OPACITY = 7,
			DISPLACEMENT = 8,
			LIGHTMAP = 9,
			REFLECTION = 10,
			UNKNOWN = 11
		};

		Texture() { }

		Texture(std::string path, ShaderResourceView* texture, TextureType type)
			: path(path), texture(texture), type(type) { }

		Texture(std::string path, ShaderResourceView* texture, TextureType type, std::string coords)
			: path(path), texture(texture), type(type)
		{
			this->coords = Texture::coordsOf(coords);
		}

		std::string path;
		ShaderResourceView* texture = nullptr;
		TextureType type = DIFFUSE;

		/*
			Local texture coordinates, e.g. for Normal Map.

			1 = x; -1 = -x
			2 = y; -2 = -y
			3 = z; -3 = -z
		*/
		DirectX::XMINT4 coords = DEFAULT_COORDS;

		bool operator < (const Texture& texture) const
		{
			return (type < texture.type);
		}
		
		static const int NUM_TYPES = 11;
		static const DirectX::XMINT4 DEFAULT_COORDS;

		static Texture::TextureType valueOf(std::string type);
		static DirectX::XMINT4 coordsOf(std::string coords);
	};

	struct Material
	{
		Material()
		{ 
			textureArray.resize(Texture::NUM_TYPES);
			coordsArray.resize(Texture::NUM_TYPES);

			for (size_t i = 0; i < Texture::NUM_TYPES; i++) {
				textureArray[i] = NULL;
				coordsArray[i] = Texture::DEFAULT_COORDS;
			}
		}
		
		Material(unsigned int index, std::vector<Texture> textures) :
			index(index), textures(textures) 
		{ 
			textureArray.resize(Texture::NUM_TYPES);
			coordsArray.resize(Texture::NUM_TYPES);

			for (size_t i = 0; i < Texture::NUM_TYPES; i++) {
				textureArray[i] = NULL;
				coordsArray[i] = Texture::DEFAULT_COORDS;
			}
		}

		unsigned int index;
		std::vector<Texture> textures;
		std::vector<ShaderResourceView*> textureArray;
		std::vector<DirectX::XMINT4> coordsArray;

		DirectX::XMFLOAT4 ambient = DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
		DirectX::XMFLOAT4 diffuse = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		DirectX::XMFLOAT4 specular = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		float shininess = 20;

		void sortTextures();
	};

}