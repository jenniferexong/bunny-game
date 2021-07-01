#include "Game.h"

#include <memory>

#include <string>
#include <unordered_map>
#include <vector>

#include "../../engine/Application.h"
#include "../game-scene/GameScene.h"

using namespace std;
Game::Game()
{
    Log::init("Game", false);
    Application::create();

    vector<string> font_names = { "maiandra" };

    unordered_map<string, int> textures = {
        {"blur-bg", app->post_processor->getBlurTexture()},
        {"compass", app->loader->loadTexture("compass.png")},
        {"cross-hair", app->loader->loadTexture("cross-hair.png")}
    };

    app->loadGuiAssets(font_names, textures);

    shared_ptr<GameScene> game_scene = make_shared<GameScene>();
    shared_ptr<PauseScene> pause_scene = make_shared<PauseScene>();
    game_scene->pause_scene = pause_scene;
    pause_scene->game_scene = game_scene;

    app->run(game_scene);
}

