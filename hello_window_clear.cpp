#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 窗口大小
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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

	//渲染循环
	while (!glfwWindowShouldClose(window))//检查 GLFW 是否被要求退出
	{
		//输入检查
		processInput(window);
		//渲染
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(window);//交换缓冲区
		glfwPollEvents();//检查有没有触发事件（键盘输入、鼠标移动等）、更新窗口状态，并调用对应的回调函数
	}

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