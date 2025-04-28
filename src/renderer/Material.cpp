#include "Material.h"

#include "core/Base.h"

void Material::Use() const
{
	MY_ASSERT(m_Shader, "Material shader is not set!");
	m_Shader->Bind();
}