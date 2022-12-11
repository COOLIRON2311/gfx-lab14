#include "main.h"

void Init()
{
	//// Point light
	//pl.pos = glm::vec3(0.0f, 15.0f, 15.0f);
	//pl.ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	//pl.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	//pl.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	//pl.atten = glm::vec3(1.0f, 0.0f, 0.0f);

	//// Directional light
	//dl.direction = glm::vec3(0.0f, -1.0f, 0.0f);
	//dl.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	//dl.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
	//dl.specular = glm::vec3(1.0f, 0.0f, 0.0f);
	//
	//// Spot light
	//sl.pos = glm::vec3(0.0f, 0.0f, 0.0f);
	//sl.direction = glm::vec3(1.0f, 1.0f, 1.0f);
	//sl.ambient = glm::vec3(1.0f, 0.22f, 0.2f);
	//sl.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	//sl.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	//sl.cutoff = 12.5f;
	//sl.atten = glm::vec3(0.1f, 0.1f, 0.1f);

	//// Material
	//mat.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	//mat.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	//mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	//mat.emission = glm::vec3(0.0f, 0.0f, 0.0f);
	//mat.shininess = 0.0f;
	
	// Point light
	pl.pos = glm::vec3(-3.12f, 8.2f, 0.0f);
	pl.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	pl.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	pl.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	pl.atten = glm::vec3(0.1f, 0.1f, 0.1f);
	
	// Directional light
	dl.direction = glm::vec3(0.0f, 0.0f, 0.0f);
	dl.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	dl.diffuse = glm::vec3(0.0f, 0.0f, 0.0f);
	dl.specular = glm::vec3(0.0f, 0.0f, 0.0f);
	
	// Spot light
	sl.pos = glm::vec3(0.0f, 0.0f, 8.37f);
	sl.direction = glm::vec3(0.0f, 0.0f, 0.0f);
	sl.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	sl.diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	sl.specular = glm::vec3(1.0f, 1.0f, 1.0f);
	sl.cutoff = 1.0f;
	sl.atten = glm::vec3(0.1f, 0.1f, 0.1f);
	
	// Material
	mat.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	mat.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	mat.emission = glm::vec3(0.0f, 0.0f, 0.0f);
	mat.shininess = 0.0f;
	
	//�������� �������� �������
	glEnable(GL_DEPTH_TEST);
	// �������������� �������
	InitShader();
	// �������������� ��������� �����
	InitVBO();
	InitTextures();
}

int main()
{
	setlocale(LC_ALL, "Russian");
	sf::Window window(sf::VideoMode(1000, 1000), "Light & Magic", sf::Style::Default, sf::ContextSettings(24));
	SetIcon(window);
	window.setVerticalSyncEnabled(true); // ������������ �������������
	window.setActive(true); // ������������� �������� OpenGL
	glewInit(); // �������������� GLEW
	Init(); // �������������� �������
	bool paused_sun = false;
	bool paused_axis = false;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // ���� ������������ ������ ����
			{
				window.close(); // ��������� ����
				goto EXIT_IS_RIGHT_HERE; // ������� �� �����
			}
			else if (event.type == sf::Event::Resized) // ���� ������������ ������� ������ ����
			{
				glViewport(0, 0, event.size.width, event.size.height); // ������������� ������� ������
			}
			else if (event.type == sf::Event::KeyPressed) // ���� ������������ ����� �������
			{
				// Rotation
				if (event.key.code == sf::Keyboard::Up)
				{
					cam.PitchPlus();
				}
				else if (event.key.code == sf::Keyboard::Down)
				{
					cam.PitchMinus();
				}
				else if (event.key.code == sf::Keyboard::Right)
				{
					cam.YawPlus();
				}
				else if (event.key.code == sf::Keyboard::Left)
				{
					cam.YawMinus();
				}

				// Movement
				else if (event.key.code == sf::Keyboard::W)
				{
					cam.W();
				}
				else if (event.key.code == sf::Keyboard::S)
				{
					cam.S();
				}
				else if (event.key.code == sf::Keyboard::A)
				{
					cam.A();
				}
				else if (event.key.code == sf::Keyboard::D)
				{
					cam.D();
				}

				else if (event.key.code == sf::Keyboard::F1)
				{
					cam.Perspective();
				}

				else if (event.key.code == sf::Keyboard::F2)
				{
					cam.Ortho();
				}

				else if (event.key.code == sf::Keyboard::Escape)
				{
					cam.Reset();
				}
			}

		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // ������� ����� ����� � ����� �������
		Draw(window); // ������
		window.display(); // ������� �� �����
	}
EXIT_IS_RIGHT_HERE: // ����� ������
	Release(); // ������� �������
	return 0; // ������� �� ���������
}

void LoadObject(int i, const char* path)
{
	glGenBuffers(1, &Objects[i]); // ���������� ��������� �����
	vector<Vertex> data;
	VERTICES[i] = load_obj(path, data);
	//cout << VERTICES << endl;
	glBindBuffer(GL_ARRAY_BUFFER, Objects[i]); // ����������� ��������� �����
	glBufferData(GL_ARRAY_BUFFER, VERTICES[i] * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // ���������� ��������� �����
	checkOpenGLerror();
}

void InitVBO()
{
	LoadObject(0, "models/plane.obj");
	LoadObject(1, "models/cube.obj");
	LoadObject(2, "models/torus.obj");
	LoadObject(3, "models/suzanne.obj");
	LoadObject(4, "models/cylinder.obj");
	LoadObject(5, "models/cone.obj");
}

void InitTextures()
{
	LoadTexture(GL_TEXTURE0, textures[0], "textures/wood.jpg");
	LoadTexture(GL_TEXTURE1, textures[1], "textures/marble.jpg");
	LoadTexture(GL_TEXTURE2, textures[2], "textures/dark_stone.jpg");
	LoadTexture(GL_TEXTURE3, textures[3], "textures/amber.jpg");
	LoadTexture(GL_TEXTURE4, textures[4], "textures/red_granite.jpg");
	LoadTexture(GL_TEXTURE5, textures[5], "textures/malachite.jpg");
}

void LoadAttrib(GLuint prog, GLint& attrib, const char* attr_name)
{
	attrib = glGetAttribLocation(prog, attr_name);
	if (attrib == -1)
	{
		std::cout << "could not bind attrib " << attr_name << std::endl;
		return;
	}
}

void LoadUniform(GLuint prog, GLint& attrib, const char* attr_name)
{
	attrib = glGetUniformLocation(prog, attr_name);
	if (attrib == -1)
	{
		std::cout << "could not bind uniform " << attr_name << std::endl;
		return;
	}
}

void LoadTexture(GLenum tex_enum, GLuint& tex, const char* path)
{
	glGenTextures(1, &tex); // ���������� ��������
	glActiveTexture(tex_enum);
	glBindTexture(GL_TEXTURE_2D, tex); // ����������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // ������������� ��������� ��������
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	sf::Image img;
	if (!img.loadFromFile(path))
	{
		std::cout << "could not load texture " << path << std::endl;
		return;
	}
	
	sf::Vector2u size = img.getSize();
	int width = size.x;
	int height = size.y;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr());
	glGenerateMipmap(GL_TEXTURE_2D);
}

void InitShader()
{
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vShader, 1, &VertexShaderSource, NULL);
	glCompileShader(vShader);
	std::cout << "vertex shader \n";
	ShaderLog(vShader);

	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &FragShaderSource, NULL);
	glCompileShader(fShader);
	std::cout << "fragment shader \n";
	ShaderLog(fShader);

	GLuint PhongVShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(PhongVShader, 1, &PhongVertexSource, NULL);
	glCompileShader(PhongVShader);
	std::cout << "phong vertex shader \n";
	ShaderLog(PhongVShader);
	
	GLuint PhongFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(PhongFShader, 1, &PhongFragSource, NULL);
	glCompileShader(PhongFShader);
	std::cout << "phong fragment shader \n";
	ShaderLog(PhongFShader);
	
	// ������� ��������� ���������
	Programs[0] = glCreateProgram(); // No lighting
	Programs[1] = glCreateProgram(); // Phong
	

	// ����������� ������� � ���������
	glAttachShader(Programs[0], vShader);
	glAttachShader(Programs[0], fShader);

	glAttachShader(Programs[1], PhongVShader);
	glAttachShader(Programs[1], PhongFShader);

	// ������� ��������� ���������
	glLinkProgram(Programs[0]);
	glLinkProgram(Programs[1]);

	int link1, link2;
	glGetProgramiv(Programs[0], GL_LINK_STATUS, &link1);
	glGetProgramiv(Programs[1], GL_LINK_STATUS, &link2);

	// ��������� �� ������
	if (!link1 || !link2)
	{
		std::cout << "could not link shader program" << std::endl;
		return;
	}
	 
	LoadAttrib(Programs[0], A1_coord, "coord");
	LoadAttrib(Programs[0], A1_texcoord, "texcoord");
	LoadUniform(Programs[0], U1_mvp, "mvp");

	LoadAttrib(Programs[1], A2_coord, "coord");
	LoadAttrib(Programs[1], A2_texcoord, "texcoord");
	LoadAttrib(Programs[1], A2_normal, "normal");
	LoadUniform(Programs[1], U2_mvp, "mvp");
	LoadUniform(Programs[1], U2_viewPos, "viewPos");
	checkOpenGLerror();
}

void Draw(sf::Window& window)
{
	// 1
	GLuint tex_loc = glGetUniformLocation(Programs[1], "tex");
	glUseProgram(Programs[1]);
	glUniformMatrix4fv(U1_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(U2_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 0);
	glEnableVertexAttribArray(A2_coord);
	glEnableVertexAttribArray(A2_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[0]);
	glVertexAttribPointer(A2_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A2_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[0]);
	glDisableVertexAttribArray(A2_coord);
	glDisableVertexAttribArray(A2_texcoord);
	glUseProgram(0); // ��������� ��������� ���������

	// 2
	glUseProgram(Programs[1]);
	pl.Load(Programs[1]);
	dl.Load(Programs[1]);
	sl.Load(Programs[1]);
	mat.Load(Programs[1]);
	glUniformMatrix4fv(U2_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(U2_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 1);
	glEnableVertexAttribArray(A2_coord);
	glEnableVertexAttribArray(A2_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[1]);
	glVertexAttribPointer(A2_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A2_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[1]);
	glDisableVertexAttribArray(A2_coord);
	glDisableVertexAttribArray(A2_texcoord);
	glUseProgram(0); // ��������� ��������� ���������

	// 3
	glUseProgram(Programs[1]);
	pl.Load(Programs[1]);
	dl.Load(Programs[1]);
	sl.Load(Programs[1]);
	mat.Load(Programs[1]);
	glUniformMatrix4fv(U2_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(U2_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 2);
	glEnableVertexAttribArray(A2_coord);
	glEnableVertexAttribArray(A2_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[2]);
	glVertexAttribPointer(A2_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A2_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[2]);
	glDisableVertexAttribArray(A2_coord);
	glDisableVertexAttribArray(A2_texcoord);
	glUseProgram(0); // ��������� ��������� ���������

	// 4
	glUseProgram(Programs[1]);
	pl.Load(Programs[1]);
	dl.Load(Programs[1]);
	sl.Load(Programs[1]);
	mat.Load(Programs[1]);
	glUniformMatrix4fv(U2_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(U2_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 3);
	glEnableVertexAttribArray(A2_coord);
	glEnableVertexAttribArray(A2_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[3]);
	glVertexAttribPointer(A2_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A2_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[3]);
	glDisableVertexAttribArray(A2_coord);
	glDisableVertexAttribArray(A2_texcoord);
	glUseProgram(0); // ��������� ��������� ���������

	glUseProgram(Programs[1]);
	pl.Load(Programs[1]);
	dl.Load(Programs[1]);
	sl.Load(Programs[1]);
	mat.Load(Programs[1]);
	glUniformMatrix4fv(U2_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(U2_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 4);
	glEnableVertexAttribArray(A2_coord);
	glEnableVertexAttribArray(A2_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[4]);
	glVertexAttribPointer(A2_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A2_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[4]);
	glDisableVertexAttribArray(A2_coord);
	glDisableVertexAttribArray(A2_texcoord);
	glUseProgram(0); // ��������� ��������� ���������

	glUseProgram(Programs[1]);
	pl.Load(Programs[1]);
	dl.Load(Programs[1]);
	sl.Load(Programs[1]);
	mat.Load(Programs[1]);
	glUniformMatrix4fv(U2_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(U2_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 5);
	glEnableVertexAttribArray(A2_coord);
	glEnableVertexAttribArray(A2_texcoord);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[5]);
	glVertexAttribPointer(A2_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A2_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[5]);
	glDisableVertexAttribArray(A2_coord);
	glDisableVertexAttribArray(A2_texcoord);
	glUseProgram(0); // ��������� ��������� ���������

	
	checkOpenGLerror(); // ��������� �� ������
}

void Release()
{
	ReleaseShader(); // ������� �������
	ReleaseVBO(); // ������� �����
}

void ReleaseVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); // ���������� �����
	for (int i = 0; i < Objects.size(); i++)
	{
		glDeleteBuffers(1, &Objects[i]); // ������� �����
	}
}

void ReleaseShader()
{
	glUseProgram(0); // ��������� ��������� ���������
	for (int i = 0; i < Programs.size(); i++)
	{
		glDeleteProgram(Programs[i]); // ������� ��������� ���������
	}
}

void ShaderLog(unsigned int shader)
{
	int infologLen = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLen);
	if (infologLen > 1)
	{
		int charsWritten = 0;
		std::vector<char> infoLog(infologLen);
		glGetShaderInfoLog(shader, infologLen, &charsWritten, infoLog.data());
		std::cout << "InfoLog: " << infoLog.data() << std::endl;
		exit(1);
	}
}

void checkOpenGLerror()
{
	GLenum errCode;
	const GLubyte* errString;
	if ((errCode = glGetError()) != GL_NO_ERROR)
	{
		errString = gluErrorString(errCode);
		std::cout << "OpenGL error: (" << errCode << ") " << errString << std::endl;
	}
}

void SetIcon(sf::Window& wnd)
{
	sf::Image image;
	if (!image.loadFromFile("icon.png"))
	{
		std::cout << "error load icon \n";
		return;
	}

	wnd.setIcon(image.getSize().x, image.getSize().y, image.getPixelsPtr());
}
