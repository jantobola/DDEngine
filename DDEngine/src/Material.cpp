#include "Material.h"
#include "DDEUtils.h"
#include <algorithm>

const DirectX::XMINT4 DDEngine::Texture::DEFAULT_COORDS = DirectX::XMINT4(1, 2, 3, 4);

DDEngine::Texture::TextureType DDEngine::Texture::valueOf(std::string type)
{
	if (type == "DIFFUSE")
		return TextureType::DIFFUSE;
	else if (type == "NORMAL")
		return TextureType::NORMAL;
	else if (type == "SPECULAR")
		return TextureType::SPECULAR;
	else if (type == "AMBIENT")
		return TextureType::AMBIENT;
	else if (type == "EMISSIVE")
		return TextureType::EMISSIVE;
	else if (type == "SHININESS")
		return TextureType::SHININESS;
	else if (type == "OPACITY")
		return TextureType::OPACITY;
	else if (type == "DISPLACEMENT")
		return TextureType::DISPLACEMENT;
	else if (type == "LIGHTMAP")
		return TextureType::LIGHTMAP;
	else if (type == "REFLECTION")
		return TextureType::REFLECTION;
	else if (type == "HEIGHT")
		return TextureType::HEIGHT;
	else
		return TextureType::UNKNOWN;
}

DirectX::XMINT4 DDEngine::Texture::coordsOf(std::string coords)
{
	std::string s = coords;
	std::replace(s.begin(), s.end(), 'x', '1');
	std::replace(s.begin(), s.end(), 'y', '2');
	std::replace(s.begin(), s.end(), 'z', '3');
	std::replace(s.begin(), s.end(), 'w', '4');

	std::string t[4];

	int i = 0;
	for (std::string::size_type c = 0; c < s.size(); c++) {
		if (s[c] == '-') {
			t[i] = std::string(1, s[c]) + std::string(1, s[c + 1]);
			c++;
		} else {
			t[i] = s[c];
		}
		i++;
	}

	return DirectX::XMINT4(StringUtils::toInt(t[0]), StringUtils::toInt(t[1]), StringUtils::toInt(t[2]), StringUtils::toInt(t[3]));
}

void DDEngine::Material::sortTextures()
{
	std::sort(textures.begin(), textures.end());
}
