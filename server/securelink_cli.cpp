#include <iostream>
#include <cstdlib>
#include <unistd.h>

void printBanner() {
    system("clear");
    std::cout << "\033[1;33m";
    std::cout << R"( 
███████╗███████╗ ██████╗ ██████╗ ██╗   ██╗██████╗ ██╗     ██╗███╗   ██╗██╗  ██╗
╚══███╔╝██╔════╝██╔════╝██╔═══██╗██║   ██║██╔══██╗██║     ██║████╗  ██║██║ ██╔╝
  ███╔╝ █████╗  ██║     ██║   ██║██║   ██║██████╔╝██║     ██║██╔██╗ ██║█████╔╝ 
 ███╔╝  ██╔══╝  ██║     ██║   ██║██║   ██║██╔═══╝ ██║     ██║██║╚██╗██║██╔═██╗ 
███████╗███████╗╚██████╗╚██████╔╝╚██████╔╝██║     ███████╗██║██║ ╚████║██║  ██╗
╚══════╝╚══════╝ ╚═════╝ ╚═════╝  ╚═════╝ ╚═╝     ╚══════╝╚═╝╚═╝  ╚═══╝╚═╝  ╚═╝
    )" << '\n';
    std::cout << "\033[0m";

    std::cout << "\n\033[1;36m🔥 SecureLink Smart Home CLI Interface 🔐\033[0m\n\n";
}

void showMenu() {
    std::cout << "[1] Start SecureLink Server\n";
    std::cout << "[2] Stop Server (Not implemented yet)\n";
    std::cout << "[3] Show Server Status (Simulated)\n";
    std::cout << "[4] Exit\n";
    std::cout << "\nSelect an option: ";
}

int main() {
    while (true) {
        printBanner();
        showMenu();

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "\n✅ Launching SecureLink server...\n";
                system("./server.out"); // this opens the actual server
                break;
            case 2:
                std::cout << "\n❌ Stop function not implemented yet.\n";
                sleep(2);
                break;
            case 3:
                std::cout << "\n📡 Status: Running on 127.0.0.1:9090\n";
                sleep(2);
                break;
            case 4:
                std::cout << "\n👋 Exiting SecureLink CLI. Stay safe!\n";
                exit(0);
            default:
                std::cout << "\n⚠️ Invalid option. Try again.\n";
                sleep(2);
        }
    }

    return 0;
}
