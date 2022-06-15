#include "shader_s.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// ���ڴ�С
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
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

    // ����������ɫ������
    Shader ourShader("3.5.shader.vs", "3.5.shader.fs");

    // ��ʼ���������꼰��ɫ
    float vertices[] = {
        // ����         // ��ɫ
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // ����
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // ����
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // �� 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ��������
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // ��ɫ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ��Ⱦѭ��
    while (!glfwWindowShouldClose(window))
    {
        // ������
        processInput(window);

        // ����
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //������ɫ������
        ourShader.use();
        ourShader.setFloat("offset", 0.5f);
        //��VAO
        glBindVertexArray(VAO);
        // ����������
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //����������
        glfwSwapBuffers(window);
        //�����û�д����¼����������롢����ƶ��ȣ������´���״̬�������ö�Ӧ�Ļص�����
        glfwPollEvents();
    }

    //�ͷ���Դ
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
