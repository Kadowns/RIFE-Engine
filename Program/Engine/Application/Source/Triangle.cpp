#include <Triangle.h>

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
	//Solicitamos a placa de v�deo um novo id de shader
//	GLuint shader = glCreateShader(shaderType);

	//Informamos a OpenGL qual � o c�digo fonte do shader a ser compilado (vari�vel code)
	
//	glShaderSource(shader, 1, &code, NULL);

	//Solicitamos que a OpenGL fa�a a compila��o.
//	glCompileShader(shader);

	//Testamos se n�o houve erro de compila��o
//	GLint isCompiled = 0;
//	glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
	//if (isCompiled == GL_FALSE) {
	//	int length;
	//	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

	//	auto errorLog = static_cast<char*>(alloca(length * sizeof(char)));

	//	glGetShaderInfoLog(shader, length, &length, errorLog);
	//	throw new std::exception(errorLog);
	//}

	//Caso n�o haja, retornamos o id do shader
	//return shader;
}
