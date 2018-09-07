#include "Triangle.h"

Triangle::Triangle(){

}


Triangle::~Triangle() {

}

void Triangle::init() {


	/*static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         0.0f,  1.0f, 0.0f,
	};*/
}

void Triangle::update(float secs) {
}

void Triangle::draw() {

}

void Triangle::deinit() {

}

int Triangle::compileShader(int shaderType, const char* code) {
	return 0;
	//Solicitamos a placa de vídeo um novo id de shader
//	GLuint shader = glCreateShader(shaderType);

	//Informamos a OpenGL qual é o código fonte do shader a ser compilado (variável code)
	
//	glShaderSource(shader, 1, &code, NULL);

	//Solicitamos que a OpenGL faça a compilação.
//	glCompileShader(shader);

	//Testamos se não houve erro de compilação
//	GLint isCompiled = 0;
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	//if (isCompiled == GL_FALSE) {
	//	int length;
	//	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	//	auto errorLog = static_cast<char*>(alloca(length * sizeof(char)));

	//	glGetShaderInfoLog(shader, length, &length, errorLog);
	//	throw new std::exception(errorLog);
	//}

	//Caso não haja, retornamos o id do shader
	//return shader;
}
