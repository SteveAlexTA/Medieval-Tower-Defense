#include "Wave.h"
#include <algorithm>

WaveSystem::WaveSystem(int initialWaveSize, float spawnInterval) : m_currentWave(0), m_waveSize(initialWaveSize), m_remainingEnemiesInWave(0), m_spawnInterval(spawnInterval), m_currentSpawnTimer(0.0f), m_waveBreakTimer(0.0f), m_timeBetweenWaves(5.0f), m_waveInProgress(false) {}

void WaveSystem::update(float deltaTime) {
	if (m_waveInProgress) {
		if (m_remainingEnemiesInWave > 0) {
			m_currentSpawnTimer += deltaTime;
		}
	}
	else {
		m_waveBreakTimer += deltaTime;
		if (m_waveBreakTimer >= m_timeBetweenWaves) {
			startNextWave();
		}
	}
}
bool WaveSystem::aboutToSpawnEnemy() {
	if (m_waveInProgress && m_remainingEnemiesInWave > 0 && m_currentSpawnTimer >= m_spawnInterval) {
		m_currentSpawnTimer = 0.0f;
		m_remainingEnemiesInWave--;
		return true;
	}
	return false;
}
bool WaveSystem::isWaveCompleted() const {
	return m_waveInProgress && m_remainingEnemiesInWave <= 0;
}
void WaveSystem::startNextWave() {
	m_currentWave++;
	m_waveSize = m_currentWave * 5;
	m_remainingEnemiesInWave = m_waveSize;
	m_waveBreakTimer = 0.0f;
	m_waveInProgress = true;
	m_spawnInterval = std::max(0.5f, m_spawnInterval * 0.9f); 
}