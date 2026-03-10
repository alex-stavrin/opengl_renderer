#pragma once

class GLFWwindow;

class UI
{
    public:
        static UI& GetInstance()
        {
            static UI instance;
            return instance;
        }

        // Delete copy and move operations
        UI(const UI&) = delete;
        void operator=(const UI&) = delete;
        UI(UI&&) = delete;
        void operator=(UI&&) = delete;

        static void Init(GLFWwindow* window);
        static void BeginFrame();
        static void DrawFrame();

        static void ConfigureDebugWindow();

    private:
        UI() = default;
        ~UI() = default;
        
};