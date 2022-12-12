#include "main.h"

void Init()
{
	// Point light
	pl.pos = glm::vec3(-3.12f, 8.27f, -2.83f);
	pl.ambient = glm::vec3(0.1f);
	pl.diffuse = glm::vec3(1.0f);
	pl.specular = glm::vec3(1.0f);
	pl.atten = glm::vec3(0.2f);

	// Directional light
	dl.direction = glm::vec3(0.0f, -1.0f, 0.0f);
	dl.ambient = glm::vec3(0.25f);
	dl.diffuse = glm::vec3(0.25f);
	dl.specular = glm::vec3(0.25f);

	// Spot light
	sl.pos = glm::vec3(-5.0f, -8.37f, -5.0f);
	sl.direction = glm::vec3(1.0f);
	sl.ambient = glm::vec3(1.0f);
	sl.diffuse = glm::vec3(1.0f);
	sl.specular = glm::vec3(1.0f);
	sl.cutoff = 12.5f;
	sl.atten = glm::vec3(0.1f, 0.1f, 0.1f);

	// Material
	mat.ambient = glm::vec3(0.5f, 0.5f, 0.5f);
	mat.diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	mat.specular = glm::vec3(0.5f, 0.5f, 0.5f);
	mat.emission = glm::vec3(0.0f, 0.0f, 0.0f);
	mat.shininess = 1.0f;

	//Включаем проверку глубины
	glEnable(GL_DEPTH_TEST);
	// Инициализируем шейдеры
	InitShader();
	// Инициализируем вершинный буфер
	InitVBO();
	InitTextures();
}

int main()
{
	setlocale(LC_ALL, "Russian");
	sf::Window window(sf::VideoMode(1000, 1000), "Light & Magic", sf::Style::Default, sf::ContextSettings(24));
	SetIcon(window);
	window.setVerticalSyncEnabled(true); // Вертикальная синхронизация
	window.setActive(true); // Устанавливаем контекст OpenGL
	glewInit(); // Инициализируем GLEW
	Init(); // Инициализируем ресурсы
	bool paused_sun = false;
	bool paused_axis = false;
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) // Если пользователь закрыл окно
			{
				window.close(); // Закрываем окно
				goto EXIT_IS_RIGHT_HERE; // Выходим из цикла
			}
			else if (event.type == sf::Event::Resized) // Если пользователь изменил размер окна
			{
				glViewport(0, 0, event.size.width, event.size.height); // Устанавливаем область вывода
			}
			else if (event.type == sf::Event::KeyPressed) // Если пользователь нажал клавишу
			{
				// Rotation
				if (event.key.code == sf::Keyboard::Up)
				{
					cam.PitchPlus();
				}
				if (event.key.code == sf::Keyboard::Down)
				{
					cam.PitchMinus();
				}
				if (event.key.code == sf::Keyboard::Right)
				{
					cam.YawPlus();
				}
				if (event.key.code == sf::Keyboard::Left)
				{
					cam.YawMinus();
				}

				// Movement
				if (event.key.code == sf::Keyboard::W)
				{
					cam.W();
				}
				if (event.key.code == sf::Keyboard::S)
				{
					cam.S();
				}
				if (event.key.code == sf::Keyboard::A)
				{
					cam.A();
				}
				if (event.key.code == sf::Keyboard::D)
				{
					cam.D();
				}

				if (event.key.code == sf::Keyboard::F1)
				{
					cam.Perspective();
				}

				if (event.key.code == sf::Keyboard::F2)
				{
					cam.Ortho();
				}

				if (event.key.code == sf::Keyboard::Escape)
				{
					cam.Reset();
				}

				if (event.key.code == sf::Keyboard::Num1)
				{
					pl.Config();
				}
				if (event.key.code == sf::Keyboard::Num2)
				{
					dl.Config();
				}
				if (event.key.code == sf::Keyboard::Num3)
				{
					sl.Config();
				}
			}

		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очищаем буфер цвета и буфер глубины
		Draw(window); // Рисуем
		window.display(); // Выводим на экран
	}
EXIT_IS_RIGHT_HERE: // Метка выхода
	Release(); // Очищаем ресурсы
	return 0; // Выходим из программы
}

void LoadObject(int i, const char* path)
{
	glGenBuffers(1, &Objects[i]); // Генерируем вершинный буфер
	vector<Vertex> data;
	VERTICES[i] = load_obj(path, data);
	//cout << VERTICES << endl;
	glBindBuffer(GL_ARRAY_BUFFER, Objects[i]); // Привязываем вершинный буфер
	glBufferData(GL_ARRAY_BUFFER, VERTICES[i] * sizeof(Vertex), data.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Отвязываем вершинный буфер
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
	glGenTextures(1, &tex); // Генерируем текстуру
	glActiveTexture(tex_enum);
	glBindTexture(GL_TEXTURE_2D, tex); // Привязываем текстуру
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // Устанавливаем параметры текстуры
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
	GLuint PhongVShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(PhongVShader, 1, load_shader("shaders/phong.vert"), NULL);
	glCompileShader(PhongVShader);
	std::cout << "phong vertex shader \n";
	ShaderLog(PhongVShader);

	GLuint PhongFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(PhongFShader, 1, load_shader("shaders/phong.frag"), NULL);
	glCompileShader(PhongFShader);
	std::cout << "phong fragment shader \n";
	ShaderLog(PhongFShader);

	GLuint ToonVShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ToonVShader, 1, load_shader("shaders/toon.vert"), NULL);
	glCompileShader(ToonVShader);
	std::cout << "toon vertex shader \n";
	ShaderLog(ToonVShader);

	GLuint ToonFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(ToonFShader, 1, load_shader("shaders/toon.frag"), NULL);
	glCompileShader(ToonFShader);
	std::cout << "toon fragment shader \n";
	ShaderLog(ToonFShader);

	GLuint BidirVShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(BidirVShader, 1, load_shader("shaders/bidir.vert"), NULL);
	glCompileShader(BidirVShader);
	std::cout << "bidir vertex shader \n";
	ShaderLog(BidirVShader);
	
	GLuint BidirFShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(BidirFShader, 1, load_shader("shaders/bidir.frag"), NULL);
	glCompileShader(BidirFShader);
	std::cout << "bidir fragment shader \n";
	ShaderLog(BidirFShader);

	// Создаем шейдерную программу
	Programs[0] = glCreateProgram(); // Phong
	Programs[1] = glCreateProgram(); // Toon
	Programs[2] = glCreateProgram(); // Bidir


	// Прикрепляем шейдеры к программе
	glAttachShader(Programs[0], PhongVShader);
	glAttachShader(Programs[0], PhongFShader);

	glAttachShader(Programs[1], ToonVShader);
	glAttachShader(Programs[1], ToonFShader);

	glAttachShader(Programs[2], BidirVShader);
	glAttachShader(Programs[2], BidirFShader);
	

	// Линкуем шейдерную программу
	glLinkProgram(Programs[0]);
	glLinkProgram(Programs[1]);
	glLinkProgram(Programs[2]);

	int link1, link2, link3;
	glGetProgramiv(Programs[0], GL_LINK_STATUS, &link1);
	glGetProgramiv(Programs[1], GL_LINK_STATUS, &link2);
	glGetProgramiv(Programs[2], GL_LINK_STATUS, &link3);

	// Проверяем на ошибки
	if (!link1 || !link2 || !link3)
	{
		std::cout << "could not link shader program" << std::endl;
		return;
	}

	LoadAttrib(Programs[0], Phong_coord, "coord");
	LoadAttrib(Programs[0], Phong_texcoord, "texcoord");
	LoadAttrib(Programs[0], Phong_normal, "normal");
	LoadUniform(Programs[0], Phong_mvp, "mvp");
	LoadUniform(Programs[0], Phong_viewPos, "viewPos");

	LoadAttrib(Programs[1], Toon_coord, "coord");
	LoadAttrib(Programs[1], Toon_texcoord, "texcoord");
	LoadAttrib(Programs[1], Toon_normal, "normal");
	LoadUniform(Programs[1], Toon_mvp, "mvp");
	LoadUniform(Programs[1], Toon_viewPos, "viewPos");

	LoadAttrib(Programs[2], Bidir_coord, "coord");
	LoadAttrib(Programs[2], Bidir_texcoord, "texcoord");
	LoadAttrib(Programs[2], Bidir_normal, "normal");
	LoadUniform(Programs[2], Bidir_mvp, "mvp");
	//LoadUniform(Programs[2], Bidir_viewPos, "viewPos");
	checkOpenGLerror();
}

void Draw(sf::Window& window)
{
	GLuint tex_loc;
	// Plane
	glUseProgram(Programs[0]);
	tex_loc = glGetUniformLocation(Programs[0], "tex");
	pl.Load(Programs[0]);
	dl.Load(Programs[0]);
	sl.Load(Programs[0]);
	mat.Load(Programs[0]);
	glUniformMatrix4fv(Phong_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(Phong_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 0);
	glEnableVertexAttribArray(Phong_coord);
	glEnableVertexAttribArray(Phong_texcoord);
	glEnableVertexAttribArray(Phong_normal);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[0]);
	glVertexAttribPointer(Phong_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(Phong_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(Phong_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[0]);
	glDisableVertexAttribArray(Phong_coord);
	glDisableVertexAttribArray(Phong_texcoord);
	glDisableVertexAttribArray(Phong_normal);
	glUseProgram(0); // Отключаем шейдерную программу

	// Cube
	glUseProgram(Programs[0]);
	tex_loc = glGetUniformLocation(Programs[0], "tex");
	pl.Load(Programs[0]);
	dl.Load(Programs[0]);
	sl.Load(Programs[0]);
	mat.Load(Programs[0]);
	glUniformMatrix4fv(Phong_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(Phong_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 1);
	glEnableVertexAttribArray(Phong_coord);
	glEnableVertexAttribArray(Phong_texcoord);
	glEnableVertexAttribArray(Phong_normal);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[1]);
	glVertexAttribPointer(Phong_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(Phong_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(Phong_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[1]);
	glDisableVertexAttribArray(Phong_coord);
	glDisableVertexAttribArray(Phong_texcoord);
	glDisableVertexAttribArray(Phong_normal);
	glUseProgram(0); // Отключаем шейдерную программу

	// Torus
	glUseProgram(Programs[0]);
	tex_loc = glGetUniformLocation(Programs[0], "tex");
	pl.Load(Programs[0]);
	dl.Load(Programs[0]);
	sl.Load(Programs[0]);
	mat.Load(Programs[0]);
	glUniformMatrix4fv(Phong_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(Phong_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 2);
	glEnableVertexAttribArray(Phong_coord);
	glEnableVertexAttribArray(Phong_texcoord);
	glEnableVertexAttribArray(Phong_normal);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[2]);
	glVertexAttribPointer(Phong_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(Phong_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(Phong_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[2]);
	glDisableVertexAttribArray(Phong_coord);
	glDisableVertexAttribArray(Phong_texcoord);
	glDisableVertexAttribArray(Phong_normal);
	glUseProgram(0); // Отключаем шейдерную программу

	// Suzanne
	glUseProgram(Programs[2]);
	tex_loc = glGetUniformLocation(Programs[2], "tex");
	pl.Load(Programs[2]);
	dl.Load(Programs[2]);
	sl.Load(Programs[2]);
	mat.Load(Programs[2]);
	glUniformMatrix4fv(Bidir_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	//glUniform3fv(Phong_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 3);
	glEnableVertexAttribArray(Bidir_coord);
	glEnableVertexAttribArray(Bidir_texcoord);
	glEnableVertexAttribArray(Bidir_normal);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[3]);
	glVertexAttribPointer(Bidir_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(Bidir_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(Bidir_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[3]);
	glDisableVertexAttribArray(Bidir_coord);
	glDisableVertexAttribArray(Bidir_texcoord);
	glDisableVertexAttribArray(Bidir_normal);
	glUseProgram(0); // Отключаем шейдерную программу

	// Cylinder
	glUseProgram(Programs[0]);
	tex_loc = glGetUniformLocation(Programs[0], "tex");
	pl.Load(Programs[0]);
	dl.Load(Programs[0]);
	sl.Load(Programs[0]);
	mat.Load(Programs[0]);
	glUniformMatrix4fv(Phong_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(Phong_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 4);
	glEnableVertexAttribArray(Phong_coord);
	glEnableVertexAttribArray(Phong_texcoord);
	glEnableVertexAttribArray(Phong_normal);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[4]);
	glVertexAttribPointer(Phong_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(Phong_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(Phong_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[4]);
	glDisableVertexAttribArray(Phong_coord);
	glDisableVertexAttribArray(Phong_texcoord);
	glDisableVertexAttribArray(Phong_normal);
	glUseProgram(0); // Отключаем шейдерную программу

	// Cone
	glUseProgram(Programs[1]);
	tex_loc = glGetUniformLocation(Programs[1], "tex");
	pl.Load(Programs[1]);
	dl.Load(Programs[1]);
	sl.Load(Programs[1]);
	mat.Load(Programs[1]);
	glUniformMatrix4fv(Phong_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform3fv(Phong_viewPos, 1, glm::value_ptr(cam.Pos));
	glUniform1i(tex_loc, 5);
	glEnableVertexAttribArray(Toon_coord);
	glEnableVertexAttribArray(Toon_texcoord);
	glEnableVertexAttribArray(Toon_normal);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[5]);
	glVertexAttribPointer(Toon_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(Toon_texcoord, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glVertexAttribPointer(Toon_normal, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[5]);
	glDisableVertexAttribArray(Toon_coord);
	glDisableVertexAttribArray(Toon_texcoord);
	glDisableVertexAttribArray(Toon_normal);
	glUseProgram(0); // Отключаем шейдерную программу


	checkOpenGLerror(); // Проверяем на ошибки
}

void Release()
{
	ReleaseShader(); // Очищаем шейдеры
	ReleaseVBO(); // Очищаем буфер
}

void ReleaseVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Отвязываем буфер
	for (int i = 0; i < Objects.size(); i++)
	{
		glDeleteBuffers(1, &Objects[i]); // Удаляем буфер
	}
}

void ReleaseShader()
{
	glUseProgram(0); // Отключаем шейдерную программу
	for (int i = 0; i < Programs.size(); i++)
	{
		glDeleteProgram(Programs[i]); // Удаляем шейдерную программу
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
