#pragma once

#ifdef __APPLE__
	#define MAC_OS 1
	#define GL_SILENCE_DEPRECATION
#else
	#define MAC_OS 0 
#endif

#include <GLFW/glfw3.h>

#include <memory>

#include "Loader.h"
#include "Timer.h"
#include "renderers/MasterRenderer.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <string>

class Scene;
class FontType;

class Application {
private:
    bool initialised = false;
	std::unordered_map<std::string, std::shared_ptr<FontType>> fonts_;

    void loadFonts(const std::vector<std::string>& font_names);
    void initialiseWindow();
	bool update();

public:
	std::unique_ptr<Loader> loader = nullptr;
	std::unique_ptr<MasterRenderer> renderer = nullptr;
	std::unique_ptr<PostProcessor> post_processor = nullptr;

	std::weak_ptr<Scene> current_scene;
	std::unique_ptr<Timer> timer;

	GLFWwindow* window = nullptr;
	int window_width = 1280;
	int window_height = 720;
	int screen_width = 1280;
	int screen_height = 720;
	float aspect_ratio = 1280.f / 720.f;

    /**
     * Loads application with given textures and fonts
     *
     * @param gui_textures Textures to be used in GUIs,
     *        where a texture key (name) is mapped to its texture id
     */
    void loadGuiAssets(
        std::vector<string> font_names,
        std::unordered_map<string, int> gui_textures);

    /**
     * Creates a global application instance with window size
     * 1280 x 720. This can then be accessed with `app->...`
     */
    static void create();

	void changeScene(std::weak_ptr<Scene> scene);

    void appKeyCallback(int key, int scan_code, int action, int mods);
    void appMouseButtonCallback(int button, int action, int mods);
    void appCursorPosCallback(double x, double y);
    void appScrollCallBack(double x_offset, double y_offset);

	void resizeWindow(int width, int height);
	void resizeFramebuffer(int width, int height);
	void closeWindow();
	void enableCursor(bool enable);

	std::weak_ptr<FontType> getFont(const std::string& name);

    void run(std::weak_ptr<Scene> initial);
};

extern std::unique_ptr<Application> app;
