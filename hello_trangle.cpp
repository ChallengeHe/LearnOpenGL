#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";


int	main()
{
	//��ʼ��GLFW
	glfwInit();
	//����GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//���汾��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//�ΰ汾��
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//ʹ�ú���ģʽ
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//Mac OS Xϵͳ��Ҫ��������
#endif

	//�������ڣ����ߡ���������
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//����ע��ص����������ڳߴ��С�仯ʱ�����ص�
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//��ʼ�� GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//����shader����
	//����������ɫ��
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//��ɫ��Դ�븽�ӵ���ɫ��������
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//������ɫ��
	glCompileShader(vertexShader);
	//������״̬
	int success;//�Ƿ�ɹ�
	char infoLog[512];//��Ŵ�����־
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//����Ƿ����ɹ�
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//����������ƬԪ��ɫ��
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//������ɫ���������
	unsigned int shaderProgram = glCreateProgram();
	//��������ɫ�����ӵ����������
	glAttachShader(shaderProgram, vertexShader);
	//��ƬԪ��ɫ�����ӵ����������
	glAttachShader(shaderProgram, fragmentShader);
	//������ɫ����������
	glLinkProgram(shaderProgram);
	//�������״̬
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//��ɫ���������ӵ��������󣬲�����Ҫ����ɾ��
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//��ʼ�������ζ�������
	/*
	float vertices[] = {
		-0.5f,-0.5f,0.0f,//��
		0.5f,-0.5f,0.0f,//��
		0.0f,0.5f,0.0f//��
	};
	*/
	//��ʼ�����ζ�������
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // ���Ͻ�
		0.5f, -0.5f, 0.0f,  // ���½�
		-0.5f, -0.5f, 0.0f, // ���½�
		-0.5f, 0.5f, 0.0f   // ���Ͻ�
	};
	
	//������������
	unsigned int indices[] = {
		// ע��������0��ʼ! 
		// ����������(0,1,2,3)���Ƕ�������vertices���±꣬
		// �����������±��������ϳɾ���

		0, 1, 3, // ��һ��������
		1, 2, 3  // �ڶ���������
	};
	//�����������
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//���㻺����� 
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//�����ݸ��Ƶ���ǰ�󶨵Ķ��㻺�����Ŀ�껺������͡����ݴ�С����λ�ֽڣ���ʵ�����ݡ����ݹ���ʽ��
	//GL_STATIC_DRAW �����ݲ���򼸺�����ı䡣
	//GL_DYNAMIC_DRAW�����ݻᱻ�ı�ܶࡣ
	//GL_STREAM_DRAW ������ÿ�λ���ʱ����ı䡣
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//���������������
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//�������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//���������������������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//�������ݽ���
	//1.Ҫ���õĶ������ԣ�layout(location = 0)
	//2.�������Դ�С��vec3
	//3.��������
	//4.�Ƿ�ϣ�����ݱ���׼����true���������ݶ��ᱻӳ�䵽0-1֮�䣩
	//5.���������ݽ�������ʱ����0
	//6.ƫ����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//���ö�������
	glEnableVertexAttribArray(0);

	//���VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//���VAO
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿����ģʽ
	//��Ⱦѭ��
	while (!glfwWindowShouldClose(window))//��� GLFW �Ƿ�Ҫ���˳�
	{
		//������
		processInput(window);
		//��Ⱦ
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//������ɫ������
		glUseProgram(shaderProgram);
		//��VAO
		glBindVertexArray(VAO);
		//����������
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//����������
		glfwPollEvents();//�����û�д����¼����������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	//�ͷ���Դ
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "framebuffer_size_callback width=" << width << ", height =" << height << std::endl;
	glViewport(0, 0, width, height);//���ô��ڴ�С�����½�x�����½�y�����ߣ�
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//����û��Ƿ����˷��ؼ�
		glfwSetWindowShouldClose(window, true);
}