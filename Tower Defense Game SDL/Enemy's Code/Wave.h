#pragma once
#include <vector>
class WaveSystem {
public:
	WaveSystem(int initialWaveSize = 5, float spawnInterval = 1.0f);

	void update(float deltatime);
	bool aboutToSpawnEnemy();
	bool isWaveCompleted() const;
	void startNextWave();

	int getCurrentWave() const { return m_currentWave; }
	int getRemainingEnemies() const { return m_remainingEnemiesInWave; }
	float getTimeBetweenWaves() const { return m_timeBetweenWaves; }
private:
	int m_currentWave;
	int m_waveSize;
	int m_remainingEnemiesInWave;
	float m_spawnInterval;
	float m_currentSpawnTimer;
	float m_waveBreakTimer;
	float m_timeBetweenWaves;
	bool m_waveInProgress;
};