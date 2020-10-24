
#include <GL/glew.h>

#include "Texture.h"

#include <stb_image/stb_image.h>

/**
	Constructor
*/
Texture::Texture(const std::string& file_name) : m_id(0), m_file_name(file_name), 
	m_local_buffer(nullptr), m_width(0), m_height(0), m_bpp(0) {

	// Loading the image
	stbi_set_flip_vertically_on_load(1); // IF UPSIDE DOWN TEXTURE, CHANGE THIS
	m_local_buffer = stbi_load(file_name.c_str(), &m_width, &m_height, &m_bpp, 4);

	printf("w: %d, h: %d, bpp: %d\n", m_width, m_height, m_bpp);

	for (int i = 0; i < 100; i++) {
		printf("%u\n", *(m_local_buffer + i));
	}

	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_local_buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	unbind();

	if (m_local_buffer) { // if buffer contains data 
		stbi_image_free(m_local_buffer);	
	}

	printf("Loaded texture\n");
}

void Texture::bind(int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
