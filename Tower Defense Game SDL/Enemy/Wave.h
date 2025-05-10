#pragma once
#include <vector>
#include <string>
#include <map>
enum class EnemyType {
	GOBLIN,
	SKELETON,
	DEMON,
	DRAGON,
	RANDOM
};
class WaveSystem {
public:
	WaveSystem(bool easyMode = true);

	void update(float deltatime);
	bool aboutToSpawnEnemy();
	bool isWaveCompleted() const;
	void startNextWave();

	int getCurrentWave() const { return m_currentWave; }
	int getRemainingEnemies() const { return m_remainingEnemiesInWave; }
	float getWaveBreakTimer() const { return m_waveBreakTimer; }
	float getTimeBetweenWaves() const { return m_timeBetweenWaves; }
	EnemyType getNextEnemyType();
	int getTotalWaves() const { return static_cast<int>(m_waveDefinitions.size()); }
	bool isLastWave() const { return m_currentWave >= m_waveDefinitions.size(); }
	bool isWaveInProgress() const { return m_waveInProgress; }
private:
	void configureWaveDefinitions();

	int m_currentWave;
	int m_waveSize;
	int m_remainingEnemiesInWave;
	float m_spawnInterval;
	float m_currentSpawnTimer;
	float m_waveBreakTimer;
	float m_timeBetweenWaves;
	bool m_waveInProgress;
	bool m_easyMode;

	std::map<int, std::string> m_waveDefinitions;
	std::vector<EnemyType> m_currentWaveEnemies;
	int m_currentEnemyIndex;
};