
#include "GLShaderManager.h"
/*
 `#include<GLShaderManager.h>` 移入了GLTool 着色器管理器（shader Mananger）类。没有着色器，我们就不能在OpenGL（核心框架）进行着色。着色器管理器不仅允许我们创建并管理着色器，还提供一组“存储着色器”，他们能够进行一些初步䄦基本的渲染操作。
 */

#include "GLTools.h"
/*
 `#include<GLTools.h>`  GLTool.h头文件包含了大部分GLTool中类似C语言的独立函数
*/

 
#include <GLUT/GLUT.h>
/*
 在Mac 系统下，`#include<glut/glut.h>`
 在Windows 和 Linux上，我们使用freeglut的静态库版本并且需要添加一个宏
*/


// 定义一个着色管理器
GLShaderManager shaderManager;

// 批次容器，是GLTools的一个简单的容器类
GLBatch triangleBatch;

/*
 当窗口大小改变时，系统会调用此函数
 调用 glutReshapeFunc 注册此回调函数
 */
void changeSize(int w, int h) {
    printf("func:%s w: %d, h: %d \n", __func__, w, h);
    // 设置视口， 原点为视口的左下角
    glViewport(0, 0, w, h);
}


void renderScene() {
    
    printf("%s \n", __func__);
    
    // 1.清除一个或者一组特定的缓存区
    /*
     缓冲区是一块存在图像信息的储存空间，红色、绿色、蓝色和alpha分量通常一起分量通常一起作为颜色缓存区或像素缓存区引用。
     OpenGL 中不止一种缓冲区（颜色缓存区、深度缓存区和模板缓存区）
      清除缓存区对数值进行预置
     参数：指定将要清除的缓存的
     GL_COLOR_BUFFER_BIT :指示当前激活的用来进行颜色写入缓冲区
     GL_DEPTH_BUFFER_BIT :指示深度缓存区
     GL_STENCIL_BUFFER_BIT:指示模板缓冲区
     */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    // 2. 设置一组浮点数来表示红色
    GLfloat vRed[] = {1.0f, 0.0f, 0.0f, 1.0f};
    
    // 3. 传递到存储着色器，即GLT_SHADER_IDENTITY着色器，这个着色器只是使用指定颜色以默认笛卡尔坐标第在屏幕上渲染几何图形
    shaderManager.UseStockShader(GLT_SHADER_IDENTITY, vRed);
    
    // 4. 提交着色器
    triangleBatch.Draw();
    
    /* 5.
     在开始的设置openGL 窗口的时候，我们指定要一个双缓冲区的渲染环境。这就意味着将在后台缓冲区进行渲染，渲染结束后交换给前台。这种方式可以防止观察者看到可能伴随着动画帧与动画帧之间的闪烁的渲染过程。缓冲区交换平台将以平台特定的方式进行。
     将后台缓冲区进行渲染，然后结束后交换给前台
     */
    glutSwapBuffers();
}


void setupRC() {
    
    // 设置清屏颜色（背景颜色）
    glClearColor(.53f, .60f, .43f, 1);
    
    // 初始化固定渲染管线
    shaderManager.InitializeStockShaders();
    
    // 指定顶点
    // 在OpenGL中
    GLfloat vVerts[] = {
        -0.5f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f
    };
    
    triangleBatch.Begin(GL_TRIANGLES, 3);
    triangleBatch.CopyVertexData3f(vVerts);
    triangleBatch.End();
}


int main(int argc, char *argv[]) {
    
    // 初始化GLUT库, 传入命令参数并初始化glut库
    glutInit(&argc, argv);
    
    /*
     初始化双缓冲窗口，其中GLUT_DOUBLE、GLUT_RGBA、GLUT_DEPTH、GLUT_STENCIL分别代表
     双缓冲窗口、RGBA颜色模式、深度测试、模板缓冲区
     --GLUT_DOUBLE`：双缓存窗口，是指绘图命令实际上是离屏缓存区执行的，然后迅速转换成窗口视图，这种方式，经常用来生成动画效果；
     --GLUT_DEPTH`：标志将一个深度缓存区分配为显示的一部分，因此我们能够执行深度测试；
     --GLUT_STENCIL`：确保我们也会有一个可用的模板缓存区。
     */
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
    
    // 初始化窗口大小，窗口标题
    glutInitWindowSize(800, 600);
    glutCreateWindow("Triangle");
    
    /*
    GLUT 内部运行一个本地消息循环，拦截适当的消息。然后调用我们不同时间注册的回调函数。我们一共注册2个回调函数：
    1）为窗口改变大小而设置的一个回调函数
    2）包含OpenGL 渲染的回调函数
    */
    // 注册重塑函数
    glutReshapeFunc(changeSize);
    // 注册显示函数
    glutDisplayFunc(renderScene);
    
    /*
    初始化一个GLEW库,确保OpenGL API对程序完全可用。
    在试图做任何渲染之前，要检查确定驱动程序的初始化过程中没有任何问题
    */
    GLenum status = glewInit();
    if (GLEW_OK != status) {
        printf("GLEW Error:%s\n",glewGetErrorString(status));
        return 1;
    }
    
    // 设置渲染环境
    setupRC();
    
    glutMainLoop();
    
}
