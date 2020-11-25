#include "GameScene.h"

#include <memory>
#include <ostream>
#include <fstream>
#include <iostream>

#include <glm/gtc/random.hpp>

#include "../Location.h"

using std::shared_ptr;
using std::make_shared;

GameScene::GameScene(shared_ptr<MasterRenderer> renderer, shared_ptr<GLFWwindow*> window, shared_ptr<Loader> loader)
{
	renderer_ = std::move(renderer);
	window_ = std::move(window);
	loader_ = std::move(loader);

	setup();
	//makeGame();
	makeTest();
}

void GameScene::render()
{
	renderer_->updateWindowSize();

	renderer_->renderWaterReflection(*this, &GameScene::renderScene);
	renderer_->renderWaterRefraction(*this, &GameScene::renderScene);

	renderScene(glm::vec4(0), true);

	renderer_->renderWater(environment_);

	renderer_->renderGui(guis_);
	renderer_->renderText(text_master_);
}

void GameScene::renderScene(glm::vec4 clipping_plane, bool progress_time)
{
	renderer_->prepare(getProjectionMatrix());
	renderer_->renderEntities(environment_, clipping_plane, progress_time);
	renderer_->renderTerrain(environment_, clipping_plane, progress_time);
	renderer_->renderSkybox(environment_, progress_time);
}

void GameScene::update()
{
	// only render lights that are within a distance from the player
	close_lights_.clear();
	close_lights_.push_back(sun_);
	for (const auto& l : lights_) {
		if (glm::distance(player_->getPosition(), l->getPosition()) <= 250.f)
			close_lights_.push_back(l);
	}
	environment_.setLights(close_lights_);

	player_->updatePosition(environment_, compass_, move_keys_);
	camera_->updateView(terrain_1_, environment_.getWater());
	mouse_picker.update(getProjectionMatrix(), *camera_); // must update after camera is moved

	selected_ = mouse_picker.selectEntity(environment_);
	if (selected_ != nullptr) 
		selected_->highlight();

	frame_rate_->updateText("FPS: " + std::to_string((int)Application::fps));
}

void GameScene::postRenderUpdate()
{
	if (selected_ != nullptr)
		selected_->unhighlight();
	selected_ = nullptr;
}

using glm::vec2;

void GameScene::setup()
{
	//TODO: Make files that you can read material properties from, and position, scale, rotation...
	int width, height;
	glfwGetWindowSize(*window_, &width, &height);

	// GUI
	const float padding = 100;
	const float size = 150;
	compass_ = std::make_shared<GuiTexture>(loader_->loadTexture("compass"), vec2(padding, height - padding), vec2(size));

	const float cross_hair_size = 60;
	const vec2 center = vec2((float)width / 2, (float)height / 2);
	auto cross_hair = make_shared<GuiTexture>(loader_->loadTexture("cross-hair"), center, vec2(cross_hair_size));

	guis_.push_back(cross_hair);
	guis_.push_back(compass_);

	// Terrains
	auto texture_pack = Application::makeTexturePack("green", "green", "light-green", "rocks");
	Texture blend_map = Texture(loader_->loadTexture("terrain1"));
	Texture normal_map = Texture(loader_->loadTexture("rocks-normal"));
	TerrainTexture ground_texture = TerrainTexture(texture_pack, blend_map, normal_map);
	terrain_1_ = Terrain(0, -1, ground_texture, "heightmap");
	environment_.addTerrain(terrain_1_);

	// Player
	Material player_material = Material();
	auto player_model = Application::makeModel("nibbles", "nibbles", player_material);

	// Bunny player
	float player_x = 328.411f;
	float player_z = -19.45f;
	float player_y = terrain_1_.getHeightOfTerrain(player_x, player_z);
	player_ = make_shared<Player>(player_model, vec3(player_x, player_y, player_z), vec3(0.f, 0, 0), 0.7f);
	player_->setRotationOffset(180.f, 0, 0);
	environment_.addEntity(player_);

	camera_ = make_shared<Camera>(player_);
	environment_.setCamera(camera_);
	environment_.setSun(sun_);

	// skybox
	Skybox sky = Skybox("skybox-textures-day", "skybox-textures-night");
	environment_.setSkybox(sky);

	auto font = std::make_shared<FontType>("maiandra");
	frame_rate_ = std::make_shared<GuiText>("", 1.5f, font, glm::vec2(0.94f, 0.025), 1.f, false);
	frame_rate_->setColor(vec3(1));
	text_master_.addText(frame_rate_);
}

void GameScene::makeGame()
{
	Material flower_material = Material(false, false);
	Material carrot_material = Material(10.f, 20.f);

	auto flower_model = Application::makeModel("flower", "flower", flower_material);
	auto carrot_model = Application::makeModel("carrot", "carrot", carrot_material);

	auto flower_set = make_shared<set<shared_ptr<Entity>>>();

	Application::loadPositionsFromFile(terrain_1_, environment_, carrot_model, "carrot", vec3(0), 0.02f);
	Application::loadPositionsFromFileToSet(terrain_1_, flower_set, flower_model, "flower", vec3(0, -90.f, 0), 0.15f);
	environment_.addEntitySet(flower_set);

	// Position the lights above flowers
	vec3 color = vec3(1.f, 1, 1);
	for (const auto& flower: *flower_set) {
		vec3 flower_pos = flower->getPosition();
		vec3 terrain_normal = terrain_1_.getNormalOfTerrain(flower_pos.x, flower_pos.z);
		vec3 light_pos = flower_pos + (15.f * terrain_normal);
		lights_.emplace_back(make_shared<Light>(light_pos, color, Light::point_light_attenuation)); // cyan
	}
}

void GameScene::makeTest()
{
	Material material = Material();
	auto model = Application::makeModel("flower", "flower", material);

	float x = 283.491f;
	float z = -195.017f;
	player_->setPosition(x, terrain_1_.getHeightOfTerrain(x, z), z);

	auto test = make_shared<Entity>(model);
	test->setPosition(100, terrain_1_.getHeightOfTerrain(100, -120) + 10, -120);
	test->setRotation(0, -90.f, 0);
	test->setScale(0.2f);
	environment_.addEntity(test);

	auto flower_model = Application::makeModel("flower", "flower", Material());
	Application::loadPositionsFromFile(terrain_1_, environment_, flower_model, "test-flowers", vec3(0, -90.f, 0), 0.15f);

	Application::loadWaterFromFile(environment_, -15.f);
}

glm::mat4 GameScene::getProjectionMatrix() // why is this in game scene
{
	// Setting the projection matrix
	float aspect_ratio = (float) MasterRenderer::window_width / (float) MasterRenderer::window_height;
	return glm::perspective(MasterRenderer::fov, aspect_ratio, MasterRenderer::near_plane, MasterRenderer::far_plane);
}


void GameScene::keyCallback(int key, int scan_code, int action, int mods)
{
	switch (key) {
	case GLFW_KEY_W: 
		if (action == GLFW_PRESS) {
			move_keys_[Key::W] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys_[Key::W] = false;
		}
		break;
	case GLFW_KEY_A: 
		if (action == GLFW_PRESS) {
			move_keys_[Key::A] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys_[Key::A] = false;
		}
		break;
	case GLFW_KEY_S: 
		if (action == GLFW_PRESS) {
			move_keys_[Key::S] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys_[Key::S] = false;
		}
		break;
	case GLFW_KEY_D: 
		if (action == GLFW_PRESS) {
			move_keys_[Key::D] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys_[Key::D] = false;
		}
		break;
	case GLFW_KEY_L: 
		if (action == GLFW_RELEASE) {
			string entity = "water";
			// Create and open a text file
			std::ofstream positions(FilePath::data_path + entity + "-positions.txt", std::ios::app);

			vec3 position = player_->getPosition();
			// Write to the file
			positions << position.x << " " << position.z << std::endl;
		}
		break;
	case GLFW_KEY_SPACE: 
		if (action == GLFW_PRESS) {
			move_keys_[Key::Space] = true;
		} else if(action == GLFW_RELEASE) {
			move_keys_[Key::Space] = false;
		}
		break;
	case GLFW_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	default:
		break;
	}
}

void GameScene::cursorPosCallback(double x, double y)
{
	if (first_mouse_movement_) {
		first_mouse_movement_ = false;
		previous_mouse_x_ = x;
		previous_mouse_y_ = y;
		return;
	}

	const float sensitivity = 0.07f;
	float x_offset = float(x - previous_mouse_x_) * sensitivity;
	float y_offset = float(y - previous_mouse_y_) * sensitivity;
	player_->changeDirection(x_offset);
	camera_->changePitch(y_offset);
	previous_mouse_x_ = x;
	previous_mouse_y_ = y;
}

void GameScene::mouseButtonCallback(int button, int action, int mods)
{
	switch (button) {
	case GLFW_MOUSE_BUTTON_LEFT:
		if (action == GLFW_PRESS) 
			mouse_buttons_[MouseButton::Left] = true;
		else if (action == GLFW_RELEASE)
			mouse_buttons_[MouseButton::Left] = false;
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		if (action == GLFW_PRESS) 
			mouse_buttons_[MouseButton::Right] = true;
		else if (action == GLFW_RELEASE)
			mouse_buttons_[MouseButton::Right] = false;
		break;
	default:
		break;
	}
}

void GameScene::scrollCallBack(double x_offset, double y_offset)
{
	camera_->zoom(float(y_offset), environment_.getWater());
}



