## 紫电引擎 
    出处《滕王阁序》
    腾蛟起凤,孟学士之词宗 
    紫电青霜,王将军之武库


### PurpleEle 跨平台渲染库
计划支持平台  
- [x] Windows 
- [x] linux(ubuntu) 
- [x] macos 
- [x] Android 
- [ ] iOS 
- [x] Raspiberry OS

环境: C++ 

底层渲染库 
    桌面端 OpenGL4.3   
    移动端 OpenGL-es 3.0 

### 运行
    Windows 
    采用mingw提供的编译器 及 构建工具 窗口创建glfw库 需要在运行前安装glfw 
    安装方式在platform_win INSTALL.md文件中有做说明

    cd platform_win
    make clean
    make run 

### 
    linux


### 组件介绍
##### RenderEngine 
    AssetManager 用于资源的管理 针对不同操作系统 文件访问方式提供统一方法   
    RenderEngine 组织渲染流程 为app提供渲染接口


### libjson
    json解析 c++实现的一个简易版的json解析库 主要用于项目中配置文件的读写 及 数据的持久化
    

    


