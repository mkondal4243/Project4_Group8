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
    switch (currentState) {
        case ServerState::IDLE: return "IDLE";
        case ServerState::MONITORING: return "MONITORING";
        case ServerState::ALERT: return "ALERT";
        case ServerState::LOCKDOWN: return "LOCKDOWN";
        default: return "UNKNOWN";
    }
}
