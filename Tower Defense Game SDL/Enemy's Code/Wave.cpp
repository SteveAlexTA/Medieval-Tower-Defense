#include "Wave.h"
#include <iostream>
#include <algorithm>

WaveSystem::WaveSystem(float spawnInterval, bool easyMode)
    : m_currentWave(0)
    , m_remainingEnemiesInWave(0)
    , m_spawnInterval(spawnInterval)
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
    // g = goblin, s = skeleton
    if (m_easyMode) {
        m_waveDefinitions[1] = "g,g,g,g,s";                      
        m_waveDefinitions[2] = "g,g,g,s,g,s";                    
        m_waveDefinitions[3] = "g,s,g,s,g,s,s";                  
        m_waveDefinitions[4] = "s,g,s,g,s,g,s,g";                
        m_waveDefinitions[5] = "s,s,g,s,g,s,s,s";                
        m_waveDefinitions[6] = "s,g,s,s,g,s,s,s,s";              
        m_waveDefinitions[7] = "s,s,g,s,s,g,s,s,s,s";            
        m_waveDefinitions[8] = "s,s,s,g,s,s,g,s,s,s,s";          
        m_waveDefinitions[9] = "s,s,s,g,s,s,s,g,s,s,s,s";      
        m_waveDefinitions[10] = "s,s,s,s,g,s,s,s,s,g,s,s,s,s";
    }
    else {
        m_waveDefinitions[1] = "g,g,s,s,g";
        m_waveDefinitions[2] = "s,s,g,g,s,g";
        m_waveDefinitions[3] = "s,s,s,g,s,s,g";
        m_waveDefinitions[4] = "s,s,s,s,s,s,g";
        m_waveDefinitions[5] = "s,s,s,s,s,s,s,s";
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
        // Create a harder wave if it's beyond defined waves
        std::string hardWave = "s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s,s";
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
    }

    m_remainingEnemiesInWave = m_currentWaveEnemies.size();
    m_currentEnemyIndex = 0;
    m_waveBreakTimer = 0.0f;
    m_waveInProgress = true;

    // Make spawn interval slightly shorter each wave (faster spawning)
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