#include "ChangeRoughness.h"
#include "Shader.h"
#include "Utils.h"
#include "Common.h"
#include "Interface.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

CChangeRoughnessPass::CChangeRoughnessPass(const std::string& vPassName, int vExcutionOrder) : IRenderPass(vPassName, vExcutionOrder)
{
}

CChangeRoughnessPass::~CChangeRoughnessPass()
{
}

void CChangeRoughnessPass::initV()
{
	m_pShader = std::make_shared<CShader>("ChangeRoughness_VS.glsl", "ChangeRoughness_FS.glsl");
	m_Texture = loadTextureFromFile("Sponza_FlagPole_roughness.tga");

	m_pShader->activeShader();
	m_pShader->setTextureUniformValue("u_RoughnessTexture", m_Texture);
}

void CChangeRoughnessPass::updateV()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	drawQuad();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	captureScreen2Img("Result.tga");
}