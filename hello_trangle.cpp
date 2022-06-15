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
	//初始化GLFW
	glfwInit();
	//配置GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//主版本号
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//次版本号
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//使用核心模式
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);//Mac OS X系统需要额外配置
#endif

	//创建窗口（宽、高、标题名）
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//窗口注册回调函数，窗口尺寸大小变化时触发回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//初始化 GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//生成shader程序
	//创建顶点着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//着色器源码附加到着色器对象上
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//编译着色器
	glCompileShader(vertexShader);
	//检查编译状态
	int success;//是否成功
	char infoLog[512];//存放错误日志
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//检查是否编译成功
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//创建并编译片元着色器
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	//创建着色器程序对象
	unsigned int shaderProgram = glCreateProgram();
	//将顶点着色器附加到程序对象上
	glAttachShader(shaderProgram, vertexShader);
	//将片元着色器附加到程序对象上
	glAttachShader(shaderProgram, fragmentShader);
	//链接着色器到程序上
	glLinkProgram(shaderProgram);
	//检查链接状态
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	//着色器对象链接到程序对象后，不再需要，可删除
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//初始化三角形顶点坐标
	/*
	float vertices[] = {
		-0.5f,-0.5f,0.0f,//左
		0.5f,-0.5f,0.0f,//右
		0.0f,0.5f,0.0f//上
	};
	*/
	//初始化矩形顶点坐标
	float vertices[] = {
		0.5f, 0.5f, 0.0f,   // 右上角
		0.5f, -0.5f, 0.0f,  // 右下角
		-0.5f, -0.5f, 0.0f, // 左下角
		-0.5f, 0.5f, 0.0f   // 左上角
	};
	
	//创建索引数据
	unsigned int indices[] = {
		// 注意索引从0开始! 
		// 此例的索引(0,1,2,3)就是顶点数组vertices的下标，
		// 这样可以由下标代表顶点组合成矩形

		0, 1, 3, // 第一个三角形
		1, 2, 3  // 第二个三角形
	};
	//顶点数组对象
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//顶点缓冲对象 
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//把数据复制到当前绑定的顶点缓冲对象（目标缓冲的类型、数据大小（单位字节）、实际数据、数据管理方式）
	//GL_STATIC_DRAW ：数据不会或几乎不会改变。
	//GL_DYNAMIC_DRAW：数据会被改变很多。
	//GL_STREAM_DRAW ：数据每次绘制时都会改变。
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//创建索引缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	//绑定索引缓冲对象
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//给索引缓冲对象设置数据
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//顶点数据解析
	//1.要配置的顶点属性：layout(location = 0)
	//2.顶点属性大小：vec3
	//3.数据类型
	//4.是否希望数据被标准化（true：所有数据都会被映射到0-1之间）
	//5.步长：数据紧密排列时可用0
	//6.偏移量
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//启用顶点属性
	glEnableVertexAttribArray(0);

	//解绑VBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//解绑VAO
	glBindVertexArray(0);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框绘制模式
	//渲染循环
	while (!glfwWindowShouldClose(window))//检查 GLFW 是否被要求退出
	{
		//输入检查
		processInput(window);
		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//激活着色器程序
		glUseProgram(shaderProgram);
		//绑定VAO
		glBindVertexArray(VAO);
		//绘制三角形
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);//交换缓冲区
		glfwPollEvents();//检查有没有触发事件（键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

	//释放资源
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	std::cout << "framebuffer_size_callback width=" << width << ", height =" << height << std::endl;
	glViewport(0, 0, width, height);//设置窗口大小（左下角x、左下角y、宽、高）
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)//检查用户是否按下了返回键
		glfwSetWindowShouldClose(window, true);
}