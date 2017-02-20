#pragma once

class GamestateManager {
public:
	GamestateManager();
	void setGameState(int state);
	int getGameState();
protected:
	int gameState;
};