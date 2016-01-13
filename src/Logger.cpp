#include "MagicWindow/Logger.h"

using namespace boost::posix_time;
using namespace ci::app;
using namespace ci::fs;
using namespace magicwindow;

Logger::Logger(Signals & signals) {
    maxLines = 100;
    signals.logError.connect([&](std::string message, bool printToConsole) {
        log("errors.txt", message, printToConsole);
    });

    signals.logMessage.connect([&](std::string message, bool printToConsole) {
        log("log.txt", message, printToConsole);
    });

    signals.clearErrorLog.connect([&]() {
        clearFile("errors.txt");
    });

    signals.clearMessageLog.connect([&](){
        clearFile("log.txt");
    });
}

void Logger::log(std::string filename, std::string message, bool printToConsole) {
    std::string line;
    std::string text = "";
    path filepath;
    
    if(!path(getAssetPath(filename)).has_filename()) {
        std::ofstream file((getAssetPath("") / filename).c_str());
        file.close();
    }

    filepath = getAssetPath(filename);
    std::ifstream read(filepath.c_str());
    int numLines = std::count(std::istreambuf_iterator<char>(read), std::istreambuf_iterator<char>(), '\n');
    int diff = numLines - maxLines;
    read.seekg(0);

    if (read.is_open()) {
        while (getline(read, line)) {
            if (diff < 0) {
                text += line + "\n";
            }
            diff--;
        }
        read.close();
    }

    std::ofstream write(filepath.c_str());
    if (write.is_open()) {
        ptime timestamp = microsec_clock::universal_time();
        write << text;
        write << "[";
        write << timestamp;
        write << "] : ";
        write << message;
        write.close();
        if (printToConsole) console() << timestamp << " : " << message << std::endl;
    }
}

void Logger::clearFile(std::string filename) {
    int maxLinesTemp = maxLines;
    maxLines = 0;
    log(filename, "File Cleared", false);
    maxLines = maxLinesTemp;
}
