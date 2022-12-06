#include "main.h"

void Init()
{
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
	sf::Window window(sf::VideoMode(1000, 1000), "Objects", sf::Style::Default, sf::ContextSettings(24));
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
	
	// Создаем шейдерную программу
	Programs[0] = glCreateProgram();

	// Прикрепляем шейдеры к программе
	glAttachShader(Programs[0], vShader);
	glAttachShader(Programs[0], fShader);

	// Линкуем шейдерную программу
	glLinkProgram(Programs[0]);

	int link;
	glGetProgramiv(Programs[0], GL_LINK_STATUS, &link);

	// Проверяем на ошибки
	if (!link)
	{
		std::cout << "error attach shaders \n";
		return;
	}
	LoadAttrib(Programs[0], A1_coord, "coord");
	LoadAttrib(Programs[0], A1_uv, "uv");
	LoadUniform(Programs[0], U1_mvp, "mvp");
	checkOpenGLerror();
}

void Draw(sf::Window& window)
{
	// 1
	GLuint tex_loc = glGetUniformLocation(Programs[0], "tex");
	glUseProgram(Programs[0]);
	glUniformMatrix4fv(U1_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform1i(tex_loc, 0);
	glEnableVertexAttribArray(A1_coord);
	glEnableVertexAttribArray(A1_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[0]);
	glVertexAttribPointer(A1_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A1_uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[0]);
	glDisableVertexAttribArray(A1_coord);
	glDisableVertexAttribArray(A1_uv);
	glUseProgram(0); // Отключаем шейдерную программу

	// 2
	glUseProgram(Programs[0]);
	glUniformMatrix4fv(U1_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform1i(tex_loc, 1);
	glEnableVertexAttribArray(A1_coord);
	glEnableVertexAttribArray(A1_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[1]);
	glVertexAttribPointer(A1_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A1_uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[1]);
	glDisableVertexAttribArray(A1_coord);
	glDisableVertexAttribArray(A1_uv);
	glUseProgram(0); // Отключаем шейдерную программу

	// 3
	glUseProgram(Programs[0]);
	glUniformMatrix4fv(U1_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform1i(tex_loc, 2);
	glEnableVertexAttribArray(A1_coord);
	glEnableVertexAttribArray(A1_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[2]);
	glVertexAttribPointer(A1_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A1_uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[2]);
	glDisableVertexAttribArray(A1_coord);
	glDisableVertexAttribArray(A1_uv);
	glUseProgram(0); // Отключаем шейдерную программу

	// 4
	glUseProgram(Programs[0]);
	glUniformMatrix4fv(U1_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform1i(tex_loc, 3);
	glEnableVertexAttribArray(A1_coord);
	glEnableVertexAttribArray(A1_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[3]);
	glVertexAttribPointer(A1_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A1_uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[3]);
	glDisableVertexAttribArray(A1_coord);
	glDisableVertexAttribArray(A1_uv);
	glUseProgram(0); // Отключаем шейдерную программу

	glUseProgram(Programs[0]);
	glUniformMatrix4fv(U1_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform1i(tex_loc, 4);
	glEnableVertexAttribArray(A1_coord);
	glEnableVertexAttribArray(A1_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[4]);
	glVertexAttribPointer(A1_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A1_uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[4]);
	glDisableVertexAttribArray(A1_coord);
	glDisableVertexAttribArray(A1_uv);
	glUseProgram(0); // Отключаем шейдерную программу

	glUseProgram(Programs[0]);
	glUniformMatrix4fv(U1_mvp, 1, GL_FALSE, glm::value_ptr(cam.MVP()));
	glUniform1i(tex_loc, 5);
	glEnableVertexAttribArray(A1_coord);
	glEnableVertexAttribArray(A1_uv);
	glBindBuffer(GL_ARRAY_BUFFER, Objects[5]);
	glVertexAttribPointer(A1_coord, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glVertexAttribPointer(A1_uv, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDrawArrays(GL_TRIANGLES, 0, VERTICES[5]);
	glDisableVertexAttribArray(A1_coord);
	glDisableVertexAttribArray(A1_uv);
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
		std::cout << "OpenGL error: " << errString << std::endl;
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
