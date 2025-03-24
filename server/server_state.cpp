#include "server_state.h"

ServerStateManager::ServerStateManager() {
    currentState = ServerState::IDLE;
}

void ServerStateManager::setState(ServerState newState) {
    currentState = newState;
}

ServerState ServerStateManager::getState() const {
    return currentState;
}

std::string ServerStateManager::getStateName() const {
    switch (static_cast<int>(currentState)) {
    case static_cast<int>(ServerState::IDLE): return "IDLE";
    case static_cast<int>(ServerState::MONITORING): return "MONITORING";
    case static_cast<int>(ServerState::ALERT): return "ALERT";
    case static_cast<int>(ServerState::LOCKDOWN): return "LOCKDOWN";
    default: return "UNKNOWN";
    }
}
