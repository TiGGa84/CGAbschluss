#include "GamestateManager.h"

GamestateManager::GamestateManager() {
	gameState = 0;
}

void GamestateManager::setGameState(int state) {
	gameState = state;
}

int GamestateManager::getGameState() {
	return gameState;
}