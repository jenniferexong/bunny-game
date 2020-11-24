﻿#include "TextShader.h"

#include "../Location.h"

const std::string TextShader::vertex_file = "res/shaders/text-vert.glsl";
const std::string TextShader::fragment_file = "res/shaders/text-frag.glsl";

void TextShader::getAllUniformLocations()
{
	locations_.insert({ UniformVariable::Translation, getUniformLocation("uTranslation") });
	locations_.insert({ UniformVariable::Color, getUniformLocation("uColor") });
}

void TextShader::bindAttributes()
{
	bindAttribute(TextAttributeLocation::Position, "aPosition");
	bindAttribute(TextAttributeLocation::Texture, "aTextureCoords");
}

void TextShader::setUp()
{
	Shader::setUp(vertex_file, fragment_file);
}


void TextShader::loadUniforms(const GuiText& text)
{
	// translation
	loadVector(locations_.at(UniformVariable::Translation), text.getPosition());

	// colour
	loadVector(locations_.at(UniformVariable::Color), text.getColor());
}

