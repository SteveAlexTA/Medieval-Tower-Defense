#include "Wave.h"
#include <iostream>
#include <algorithm>

WaveSystem::WaveSystem(bool easyMode)
    : m_currentWave(0)
    , m_remainingEnemiesInWave(0)
    , m_spawnInterval(2.0f)
    , m_currentSpawnTimer(0.0f)
    , m_waveBreakTimer(0.0f)
    , m_timeBetweenWaves(5.0f)
    , m_waveInProgress(false)
    , m_easyMode(easyMode)
    , m_currentEnemyIndex(0)
{
    configureWaveDefinitions();
}

void WaveSystem::configureWaveDefinitions() {
	// g = goblin, s = skeleton, d = demon, r = dragon
    if (m_easyMode) {
        m_waveDefinitions[1] = "g,g,g,g,g";
        m_waveDefinitions[2] = "g,g,g,g,g,g,g";
        m_waveDefinitions[3] = "g,g,g,g,g,g,g,g";
        m_waveDefinitions[4] = "g,g,g,g,g,g,g,g,g,g";
        m_waveDefinitions[5] = "s,s,s,s,s,s,s,s";
        m_waveDefinitions[6] = "g,s,g,g,g,s,s,g,s,s,g,s,g,g,s";
        m_waveDefinitions[7] = "s,s,s,s,g,s,s,s,s,g,s,s,s,s,s";
        m_waveDefinitions[8] = "s,s,s,s,s,s,s,s,s,s,s,s,s,s,s";
        m_waveDefinitions[9] = "s,g,s,g,s,s,s,g,g,g,g,s,s,d,d";
        m_waveDefinitions[10] = "g,g,g,g,d,g,g,s,g,s,s,g,d,s,g,g,g,s,d,g";
        m_waveDefinitions[11] = "g,s,g,g,s,s,g,s,s,g,s,s,s,g,s,s,g,s,g,d";
        m_waveDefinitions[12] = "d,r,d,r,d,r,d,r";
        m_waveDefinitions[13] = "g,s,s,d,s,g,s,s,g,s,s,s,g,s,d,g,g,s,g,g,d";
        m_waveDefinitions[14] = "g,s,s,s,g,g,g,s,g,s,g,s,g,d,g,d,d,d,g,g,d";
        m_waveDefinitions[15] = "g,s,s,s,g,g,g,s,g,s,g,s,d,s,s,d,d,s,s,d,d";
        m_waveDefinitions[16] = "s,g,g,g,s,s,d,d,s,d,d,s,g,g,g,g,g,s,s,s,d";
        m_waveDefinitions[17] = "r,s,g,s,d,g,g,g,g,g,s,g,g,g,s,s,s,s,s,d,d,d";
        m_waveDefinitions[18] = "d,r,r,d,s,s,d,s,s,r,r,d,d,d,r,d,r,d,r,d,r,d";
        m_waveDefinitions[19] = "r,d,r,d,r,d,s,d,r,s,d,s,d,s,d,s,d,r,d,r,s,d";
        m_waveDefinitions[20] = "r,d,r,d,r,d,r,d,r,d,r,d,r,d,r,d,r,d,r";

    }
    else {
        m_waveDefinitions[1] = "";
    }
}

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
        m_currentEnemyIndex++;
        return true;
    }
    return false;
}

bool WaveSystem::isWaveCompleted() const {
    return m_waveInProgress && m_remainingEnemiesInWave <= 0;
}

void WaveSystem::startNextWave() {
    m_currentWave++;

    if (m_waveDefinitions.find(m_currentWave) == m_waveDefinitions.end()) {
        // Create a super hard wave if it's beyond defined waves
        std::string hardWave = "r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r,r";
        m_waveDefinitions[m_currentWave] = hardWave;
    }
    // Parse the wave definition string into enemy types
    std::string waveDefinition = m_waveDefinitions[m_currentWave];
    m_currentWaveEnemies.clear();

    for (char c : waveDefinition) {
        if (c == 'g') {
            m_currentWaveEnemies.push_back(EnemyType::GOBLIN);
        }
        else if (c == 's') {
            m_currentWaveEnemies.push_back(EnemyType::SKELETON);
		}
		else if (c == 'd') {
			m_currentWaveEnemies.push_back(EnemyType::DEMON);
		}
        else if (c == 'r') {
            m_currentWaveEnemies.push_back(EnemyType::DRAGON);
        }
    }

    m_remainingEnemiesInWave = m_currentWaveEnemies.size();
    m_currentEnemyIndex = 0;
    m_waveBreakTimer = 0.0f;
    m_waveInProgress = true;
	//Make interval shorter for harder waves (increasing difficulty each wave)
    if (m_currentWave > 1) {
        m_spawnInterval *= 0.9f;
        if (m_spawnInterval < 0.5f) m_spawnInterval = 0.5f;
    }
    std::cout << "Starting Wave " << m_currentWave << " with " << m_remainingEnemiesInWave << " enemies!" << std::endl;
}

EnemyType WaveSystem::getNextEnemyType() {
    if (m_currentEnemyIndex < m_currentWaveEnemies.size()) {
        return m_currentWaveEnemies[m_currentEnemyIndex];
    }
    return EnemyType::GOBLIN; // Default if index is out of bounds
}