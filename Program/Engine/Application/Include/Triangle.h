#pragma once

#include <Application.h>
#include <Scene.h>

class Triangle : public Scene {
private:
	const char* VERTEX_SHADER_CODE = 
		"#version 330\n"
        "in vec2 aPosition;\n"
        "void main(){\n"
        "    gl_Position = vec4(aPosition, 0.0, 1.0);\n"
		"}";
	const char* FRAGMENT_SHADER_CODE =
		"#version 330\n"
		"out vec4 out_color;\n"
		"void main(){\n"
		"    out_color = vec4(1.0, 1.0, 0.0, 1.0);\n"
		"}";

public:
	Triangle();
	~Triangle();

	void init();
	void update(float secs);
	void draw();
	void deinit();

	int compileShader(int shaderType, const char *code);
};

