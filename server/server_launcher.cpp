#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./server_launcher --start|--stop|--status\n";
        return 1;
    }

    std::string cmd = argv[1];
    if (cmd == "--start") {
        std::cout << "✅ Starting SecureLink Server...\n";
        system("./server.out");
    } else if (cmd == "--stop") {
        std::cout << "🔴 Manual shutdown. (Implement signal if needed)\n";
    } else if (cmd == "--status") {
        std::cout << "📡 Status: Not connected (simulate this)\n";
    } else {
        std::cerr << "❌ Unknown command.\n";
    }

    return 0;
}
