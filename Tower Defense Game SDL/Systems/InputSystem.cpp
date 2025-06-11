#include "InputSystem.h"
#include "../Core/Game.h"
#include "../Managers/UIManager.h"
#include "../States/Menu.h"
#include "../Towers/Tower.h"
#include "../Towers/ArcherTower.h"
#include "../Towers/CannonTower.h"
#include "../Towers/LightningTower.h"
#include "../Managers/SoundManager.h"

InputSystem::InputSystem(Game* game) : m_game(game), previewTower(nullptr), isDraggingTower(false) {}

InputSystem::~InputSystem() {
	if (previewTower) delete previewTower;
}

void InputSystem::handleEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			m_game->setRunning(false);
			return;
		}
		if (m_game->isInWinScreen() || m_game->isInLoseScreen()) {
			continue;
		}
		if (m_game->isInMenu()) {
			handleMenuInput(event);
			continue;
		}
		handleGameInput(event);
	}
}

void InputSystem::handleMenuInput(const SDL_Event& event) {
	if (event.type == SDL_MOUSEBUTTONDOWN) {    
		m_game->getMenuSystem()->handleEvents(event);
		if (m_game->getMenuSystem()->getMenuState() == MenuState::PLAY) {
			m_game->startGame();
		}
		else if (m_game->getMenuSystem()->getMenuState() == MenuState::EXIT) {
			m_game->setRunning(false);
		}
	}
}

void InputSystem::handleGameInput(const SDL_Event& event) {
    switch (event.type) {
    case SDL_MOUSEMOTION:
        handleMouseMotion(event);
        break;
    case SDL_MOUSEBUTTONDOWN:
        handleMouseButtonDown(event);
        break;
    case SDL_MOUSEBUTTONUP:
        handleMouseButtonUp(event);
        break;
    case SDL_KEYDOWN:
        handleKeyDown(event);
        break;
    default:
        break;
    }
}

void InputSystem::handleMouseMotion(const SDL_Event& event) {
    int mouseX = event.motion.x;
    int mouseY = event.motion.y;
    // Update UI hover states when not in build mode
    UI* uiSystem = m_game->getUISystem();
    if (!m_game->isBuildTowerMode()) {
        uiSystem->archerTowerHovered = uiSystem->isArcherTowerClicked(mouseX, mouseY);
        uiSystem->cannonTowerHovered = uiSystem->isCannonTowerClicked(mouseX, mouseY);
        uiSystem->lightningTowerHovered = uiSystem->isLightningTowerClicked(mouseX, mouseY);
    }
    else if (previewTower) {
		previewTower->RenderRangeCircleWhileDraggingTower(mouseX, mouseY);
        previewTower->setX(mouseX - 16); 
        previewTower->setY(mouseY - 16);
    }
}

void InputSystem::handleMouseButtonUp(const SDL_Event& event) {}

void InputSystem::handleMouseButtonDown(const SDL_Event& event) {
    int mouseX = event.button.x;
    int mouseY = event.button.y;
    if (event.button.button == SDL_BUTTON_LEFT) {
        // Check UI clicks (if UI clicked, remain it is)
        if (handleUIClick(mouseX, mouseY)) {
            return;
        }
        // Handle tower placement mode
        if (m_game->isBuildTowerMode()) {
            handleTowerPlacement(mouseX, mouseY);
            if (previewTower) {
                delete previewTower; // Clean up previous tower preview
                previewTower = nullptr; // Reset preview tower
			}
			m_game->setBuildTowerMode(false);
			m_game->getUISystem()->resetSelectedTower(); 
            return;
        }
        // Handle tower selection and upgrade/delete
        handleTowerSelection(mouseX, mouseY);
    }
}

void InputSystem::handleKeyDown(const SDL_Event& event) {
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
        // Cancel tower selection or build mode
        if (m_game->getSelectedTower()) {
            m_game->getSelectedTower()->setSelected(false);
            m_game->setSelectedTower(nullptr);
        }
        if (m_game->isBuildTowerMode()) {
            m_game->setBuildTowerMode(false);
            m_game->getUISystem()->resetSelectedTower();
        }
        break;
    case SDLK_RETURN:  
        if (event.key.keysym.mod & KMOD_ALT) {  // Check if Alt is pressed
            SDL_Window* window = m_game->getWindow();
            if (window) {
                Uint32 flags = SDL_GetWindowFlags(window);
                if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
                    SDL_SetWindowFullscreen(window, 0);
                    std::cout << "Switching to windowed mode" << std::endl;
                } else {
                    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
                    std::cout << "Switching to fullscreen mode" << std::endl;
                }
            }
        }
        break;
    default:
        break;
    }
}

bool InputSystem::handleUIClick(int mouseX, int mouseY) {
    UI* ui = m_game->getUISystem();
    // Check tower type selection buttons
    if (ui->isArcherTowerClicked(mouseX, mouseY)) {
        ui->setSelectedTower(TowerSelection::ARCHER);
        if (previewTower) {
            delete previewTower; // Clean up previous tower preview
		}
		previewTower = new ArcherTower(m_game->renderer, nullptr, nullptr); // Create current preview tower
		previewTower->setPreviewModeWhileDragging(true); // Set preview circle range mode for the tower
        m_game->setBuildTowerMode(!m_game->isBuildTowerMode()); 
        if (m_game->isBuildTowerMode() && m_game->getSelectedTower()) {
            m_game->getSelectedTower()->setSelected(false);
            m_game->setSelectedTower(nullptr);
        }
        return true;
    }
    if (ui->isCannonTowerClicked(mouseX, mouseY)) {
        ui->setSelectedTower(TowerSelection::CANNON);
        if (previewTower) {
            delete previewTower; // Clean up previous tower preview
        }
		previewTower = new CannonTower(m_game->renderer, nullptr, nullptr); // Create current preview tower
		previewTower->setPreviewModeWhileDragging(true); // Set preview circle range mode for the tower
        m_game->setBuildTowerMode(!m_game->isBuildTowerMode()); 
        if (m_game->isBuildTowerMode() && m_game->getSelectedTower()) {
            m_game->getSelectedTower()->setSelected(false);
            m_game->setSelectedTower(nullptr);
        }
        return true;
    }
    if (ui->isLightningTowerClicked(mouseX, mouseY)) {
        ui->setSelectedTower(TowerSelection::LIGHTNING);
        if (previewTower) {
            delete previewTower; // Clean up previous tower preview
		}
		previewTower = new LightningTower(m_game->renderer, nullptr, nullptr); // Create current preview tower
		previewTower->setPreviewModeWhileDragging(true); // Set preview circle range mode for the tower
        m_game->setBuildTowerMode(!m_game->isBuildTowerMode());
        if (m_game->isBuildTowerMode() && m_game->getSelectedTower()) {
            m_game->getSelectedTower()->setSelected(false);
            m_game->setSelectedTower(nullptr);
        }
        return true;
    }
    return false;
}


bool InputSystem::isClickInUpgradeUI(int mouseX, int mouseY, Tower* tower) const {
    if (!tower || !tower->canUpgrade()) return false;
    int towerX = tower->getX();
    int towerY = tower->getY();
    SDL_Rect upgradeRect = { towerX - 16, towerY - 40, 32, 32 };
    return (mouseX >= upgradeRect.x && mouseX < upgradeRect.x + upgradeRect.w && mouseY >= upgradeRect.y && mouseY < upgradeRect.y + upgradeRect.h);
}

bool InputSystem::isClickInSellUI(int mouseX, int mouseY, Tower* tower) const {
    if (!tower) return false;
    int towerX = tower->getX();
    int towerY = tower->getY(); 
    SDL_Rect deleteRect = { towerX + 16, towerY - 40, 32, 32 };
    return (mouseX >= deleteRect.x && mouseX < deleteRect.x + deleteRect.w && mouseY >= deleteRect.y && mouseY < deleteRect.y + deleteRect.h);
}


void InputSystem::handleTowerPlacement(int mouseX, int mouseY) {
    m_game->placeTower(mouseX, mouseY);
    m_game->setBuildTowerMode(false);
    m_game->getUISystem()->resetSelectedTower();
}

void InputSystem::handleTowerSelection(int mouseX, int mouseY) {
    Tower* selectedTower = m_game->getSelectedTower();
    if (selectedTower) {
        if (isClickInUpgradeUI(mouseX, mouseY, selectedTower)) {  
            m_game->upgradeTower(selectedTower);
            return;
        }
        if (isClickInSellUI(mouseX, mouseY, selectedTower)) {  
            m_game->deleteTower(selectedTower);
            return;
        }
    }
    m_game->selectTowerAt(mouseX, mouseY);
}

void InputSystem::renderPreviewTowerWhenClicked() {
    if (m_game->isBuildTowerMode() && previewTower) {
        // Render the range circle first
        previewTower->RenderRangeCircleWhileDraggingTower(
            previewTower->getX() + 16,
            previewTower->getY() + 16
        );
        // Then render the tower preview
        previewTower->Render();
    }
}