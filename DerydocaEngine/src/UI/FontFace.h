#pragma once
#include <string>
#include <map>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include "Rectangle.h"
#include "Utilities\TexturePackerImage.h"
#include "Rendering\Texture.h"

class Texture;

namespace DerydocaEngine::UI
{

	const int MISSING_CHAR_CODE = 95;

	class FontFace
	{
	public:
		FontFace();
		~FontFace();

		std::shared_ptr<Rendering::Texture> getTexture();
		Utilities::TexturePackerImage getCharData(int const& charCode) const {
			if (m_charImages.find(charCode) == m_charImages.end())
			{
				return m_charImages.at(MISSING_CHAR_CODE);
			}
			else
			{
				return m_charImages.at(charCode);
			}
		};
		float getLineHeight() const { return m_lineHeight; }
		void setFontSize(float const& fontSize) { m_fontSize = fontSize; }

		void loadFromFontFile(std::string const& filePath);
		void loadFromSerializedFile(std::string const& filePath);
		void saveToSerializedFile(std::string const& filePath);
	private:
		glm::ivec2 m_dotsPerInch;
		float m_fontSize;
		std::shared_ptr<Rendering::Texture> m_texture;
		std::map<int, Utilities::TexturePackerImage> m_charImages;
		unsigned char* m_imageBuffer;
		glm::ivec2 m_imageBufferSize;
		bool m_textureDirty;
		float m_lineHeight;
		std::string m_name;
		std::string m_style;
	};

}
