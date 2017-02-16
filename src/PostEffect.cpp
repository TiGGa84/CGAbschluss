#include "PostEffect.h"

GLuint PostEffect::QuadVAOID = 0;
GLuint PostEffect::QuadVBOID = 0;

PostEffect::PostEffect() {}

PostEffect::~PostEffect() {}

// Viereck über den ganzen Viewport malen
void PostEffect::drawQuad() const
{
	// Daten nur ein mal erstellen
	if (QuadVAOID == 0) {
		// Vertices für 2 dreiecke
		GLfloat quadVertices[] = {
			-1.0f, 1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			1.0f, 1.0f, 0.0f,
			1.0f, -1.0f, 0.0f,
		};
		// Setup VAO
		glGenVertexArrays(1, &QuadVAOID);
		// Setup VBO
		glGenBuffers(1, &QuadVBOID);
		glBindVertexArray(QuadVAOID);
		// Nur Position übergeben, UVs werden im Vertex-shader berechnet
		glBindBuffer(GL_ARRAY_BUFFER, QuadVBOID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	}
	glBindVertexArray(QuadVAOID);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

