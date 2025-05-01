#include "Texture.h"

#include <OpenImageIO/imageio.h>

#include "core/Base.h"

static void flipVertically(unsigned char *data, int width, int height, int channels)
{
	int rowSize = width * channels;
	std::vector<unsigned char> tempRow(rowSize);

	for (int y = 0; y < height / 2; ++y)
	{
		unsigned char *topRow = data + y * rowSize;
		unsigned char *bottomRow = data + (height - y - 1) * rowSize;

		// Swap the top row with the bottom row
		std::memcpy(tempRow.data(), topRow, rowSize);
		std::memcpy(topRow, bottomRow, rowSize);
		std::memcpy(bottomRow, tempRow.data(), rowSize);
	}
}

std::shared_ptr<Texture2D> Texture2D::Create()
{
	return std::make_shared<Texture2D>();
}

std::shared_ptr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
{
	return std::make_shared<Texture2D>(width, height);
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::string &path)
{
	return std::make_shared<Texture2D>(path);
}

std::shared_ptr<Texture2D> Texture2D::Create(const std::filesystem::path &path)
{
	return std::make_shared<Texture2D>(path);
}

std::shared_ptr<Texture2D> Texture2D::Create(const Texture2DSpecification &spec)
{
	return std::make_shared<Texture2D>(spec);
}

static GLenum TextureFormatToGL(const TextureFormat &format)
{
	switch (format)
	{
	case TextureFormat::RGBA8:
		return GL_RGBA8;
	case TextureFormat::RGBA16F:
		return GL_RGBA16F;
	case TextureFormat::RGBA32F:
		return GL_RGBA32F;
	case TextureFormat::RGB8:
		return GL_RGB8;
	case TextureFormat::RGB16F:
		return GL_RGB16F;
	case TextureFormat::RGB32F:
		return GL_RGB32F;
	case TextureFormat::RG8:
		return GL_RG8;
	case TextureFormat::RG16F:
		return GL_RG16F;
	case TextureFormat::RG32F:
		return GL_RG32F;
	case TextureFormat::R8:
		return GL_R8;
	case TextureFormat::R16F:
		return GL_R16F;
	case TextureFormat::R32F:
		return GL_R32F;
		// case TextureFormat::Depth: return GL_DEPTH_COMPONENT;
	}
	MY_ASSERT(false, "Unknown texture format!");
	return 0;
}

Texture2D::Texture2D(const Texture2DSpecification &spec)
	: m_Specification(spec)
{
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, TextureFormatToGL(spec.Format), spec.Width, spec.Height);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, spec.MinFilter);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, spec.MagFilter);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, spec.WrapS);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, spec.WrapT);
}

void Texture2D::Reload()
{
	if (m_RendererID)
		glDeleteTextures(1, &m_RendererID);
	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, TextureFormatToGL(m_Specification.Format), m_Specification.Width, m_Specification.Height);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, m_Specification.MinFilter);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, m_Specification.MagFilter);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, m_Specification.WrapS);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, m_Specification.WrapT);
}

Texture2D::Texture2D(const std::filesystem::path &path)
{
	std::string pathString = path.string();
	std::filesystem::path current_path = std::filesystem::current_path();
	// LOG_INFO("Current path: {0}", current_path.string());
	// LOG_INFO("Loading texture from path: {0}", pathString);
	auto inp = OIIO::ImageInput::open(pathString);
	if (!inp)
	{
		MY_ASSERT(inp, "Could not open image");
		LOG_ERROR("Could not open image");
	}
	else
	{
		LOG_INFO("Image opened successfully");
	}
	const OIIO::ImageSpec &spec = inp->spec();
	m_Width = spec.width;
	m_Height = spec.height;
	m_nChannels = spec.nchannels;
	auto pixels = std::make_unique<unsigned char[]>(m_Width * m_Height * m_nChannels);

	inp->read_image(OIIO::TypeDesc::UINT8, pixels.get());
	inp->close();
	// LOG_INFO("Number of channels: {0}", m_nChannels);
	// LOG_INFO("Image width: {0}, height: {1}", m_Width, m_Height);

	flipVertically(pixels.get(), m_Width, m_Height, m_nChannels);

	GLenum internalFormat = 0, dataFormat = 0;
	if (m_nChannels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (m_nChannels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	m_InternalFormat = internalFormat;
	m_DataFormat = dataFormat;

	MY_ASSERT(internalFormat & dataFormat, "Format not supported");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, pixels.get());
}

Texture2D::Texture2D(const std::string &path)
{
	std::filesystem::path current_path = std::filesystem::current_path();
	// LOG_INFO("Current path: {0}", current_path.string());
	// LOG_INFO("Loading texture from path: {0}", path);
	auto inp = OIIO::ImageInput::open(path);
	if (!inp)
	{
		MY_ASSERT(inp, "Could not open image");
		LOG_ERROR("Could not open image");
	}
	else
	{
		LOG_INFO("Image opened successfully");
	}
	const OIIO::ImageSpec &spec = inp->spec();
	m_Width = spec.width;
	m_Height = spec.height;
	m_nChannels = spec.nchannels;
	auto pixels = std::make_unique<unsigned char[]>(m_Width * m_Height * m_nChannels);

	inp->read_image(OIIO::TypeDesc::UINT8, pixels.get());
	inp->close();
	// LOG_INFO("Number of channels: {0}", m_nChannels);
	// LOG_INFO("Image width: {0}, height: {1}", m_Width, m_Height);

	flipVertically(pixels.get(), m_Width, m_Height, m_nChannels);

	GLenum internalFormat = 0, dataFormat = 0;
	if (m_nChannels == 4)
	{
		internalFormat = GL_RGBA8;
		dataFormat = GL_RGBA;
	}
	else if (m_nChannels == 3)
	{
		internalFormat = GL_RGB8;
		dataFormat = GL_RGB;
	}

	m_InternalFormat = internalFormat;
	m_DataFormat = dataFormat;

	MY_ASSERT(internalFormat & dataFormat, "Format not supported");

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, pixels.get());
}

Texture2D::Texture2D(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height)
{

	m_InternalFormat = GL_RGBA8;
	m_DataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
	glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

	glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void Texture2D::SetData(void *data, uint32_t size)
{
	uint32_t bpp = 4;
	MY_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture2D::Bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_RendererID);
}
