#include "Utils.h"

auto Utils::logToDebug(std::string output) -> void {
    auto roaming = std::string(Utils::getAppDataDir()) + std::string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState");
    auto fPath = std::string(roaming + "\\SwapChain_Output.txt");

    CloseHandle(CreateFileA(fPath.c_str(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL));

    std::ofstream file;
    file.open(fPath, std::ios::app);
    
    file << output << std::endl;
    file.close();
};

auto Utils::getAppDataDir() -> std::string {
    char* path = NULL;
    size_t length;

    _dupenv_s(&path, &length, "appdata");

    return std::string(path);
};