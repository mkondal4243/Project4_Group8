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

    std::cout << "\n\033[1;36mSecureLink CLI Dashboard\033[0m\n\n";
}

void showMenu() {
    std::cout << "[1] Start SecureLink Server (Background)\n";
    std::cout << "[2] Open SecureLink GUI (Client)\n";
    std::cout << "[3] Run Login Test\n";
    std::cout << "[4] Run Logs Test\n";
    std::cout << "[5] Exit\n";
    std::cout << "\nEnter choice: ";
}

int main() {
    while (true) {
        printBanner();
        showMenu();

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1:
                std::cout << "[INFO] Launching server in background...\n";
                system("./build/server &");
                sleep(2);
                break;
            case 2:
                std::cout << "[INFO] Launching SecureLink Qt GUI...\n";
                system("./build/SecureLinkUI/SecureLinkUI.app/Contents/MacOS/SecureLinkUI &");
                sleep(2);
                break;
            case 3:
                std::cout << "[INFO] Running login test...\n";
                system("./tests/test_login");
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 4:
                std::cout << "[INFO] Running logs test...\n";
                system("./tests/test_logs");
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore();
                std::cin.get();
                break;
            case 5:
                std::cout << "[INFO] Exiting CLI. Stay secure.\n";
                return 0;
            default:
                std::cout << "[WARN] Invalid input. Try again.\n";
                sleep(1);
        }
    }

    return 0;
}
