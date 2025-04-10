#ifndef SERVER_STATE_H
#define SERVER_STATE_H

#include <string>

enum class ServerState {
    IDLE,
    MONITORING,
    ALERT,
    LOCKDOWN
};

class ServerStateManager {
private:
    ServerState currentState;
public:
    ServerStateManager();
    void setState(ServerState newState);
    ServerState getState() const;
    std::string getStateName() const;
};

#endif // SERVER_STATE_H
