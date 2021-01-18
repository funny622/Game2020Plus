#include "GameScene.h"
using namespace std;

GameScene::GameScene(int id, LPCWSTR filePath, int startupSectionId) : SceneMain(id, filePath)
{
	CurrentSectionId = startupSectionId;
}
