#include "Scene.h"

std::map<Key, bool> Scene::move_keys_ = {
	{Key::W, false}, 
	{Key::A, false}, 
	{Key::S, false}, 
	{Key::D, false}, 
	{Key::Space, false}
};
