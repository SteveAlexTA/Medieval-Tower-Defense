#pragma once
#include <vector>
#include<string>
enum class EnemyType {
	GOBLIN,
	SKELETON,
	RANDOM
};
class WaveSystem {
public:
	WaveSystem(int initialWaveSize = 5, float spawnInterval = 1.0f, bool easyMode = true);

	void update(float deltatime);
	bool aboutToSpawnEnemy();
	bool isWaveCompleted() const;
	void startNextWave();

	int getCurrentWave() const { return m_currentWave; }
	int getRemainingEnemies() const { return m_remainingEnemiesInWave; }
	float getTimeBetweenWaves() const { return m_timeBetweenWaves; }
	EnemyType getCurrentEnemyType() const { return m_currentEnemyType; }
	int getTotalWaves() const { return m_totalWaves; }
	bool isLastWave() const { return m_currentWave >= m_totalWaves; }
private:
	void configureEasyMode();
	void configureNormalMode();
	int m_currentWave;
	int m_waveSize;
	int m_remainingEnemiesInWave;
	float m_spawnInterval;
	float m_currentSpawnTimer;
	float m_waveBreakTimer;
	float m_timeBetweenWaves;
	bool m_waveInProgress;
	bool m_easyMode;
	int m_totalWaves;
	EnemyType m_currentEnemyType;
	std::vector<EnemyType> m_waveEnemyTypes;
};