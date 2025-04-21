#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include <sstream>
#include <memory>

/// \brief Интерфейс обработчика команд
class ICommandHandler {
public:
    virtual ~ICommandHandler() = default;
    virtual void handleCommands(const std::vector<std::string>& commands) = 0;
};

/// \brief Класс для вывода команд в консоль
class PrintCommandHandler : public ICommandHandler {
public:
    void handleCommands(const std::vector<std::string>& commands) override {
        for (size_t i = 0; i < commands.size(); ++i) {
            std::cout << commands[i];
            if (i != commands.size() - 1) std::cout << ", ";
        }
        std::cout << '\n';
    }
};

/// \brief Класс для сохранения команд в файл
class FileCommandHandler : public ICommandHandler {
public:
    void handleCommands(const std::vector<std::string>& commands) override {
        std::ostringstream filename;
//        const std::time_t timestamp;
//        std::time(nullptr)

        filename << "bulk" << std::time(nullptr) << ".log";

        std::ofstream outFile(filename.str());
        if (outFile) {
            for (const auto& command : commands) {
                outFile << command << ", ";
            }
            outFile << '\n';
            outFile.close();
        } else {
            std::cerr << "Failed to open file: " << filename.str() << "\n";
        }
    }
};

/// \brief Класс для обработки команд
class CommandProcessor {
public:
    CommandProcessor(std::vector<std::shared_ptr<ICommandHandler>> handlers, int blockSize)
        : handlers(std::move(handlers)), blockSize(blockSize) {}

    void processCommands() {
        std::vector<std::string> commands;
        std::string line;
        bool inDynamicBlock = false; // флаг для отслеживания динамического блока

        while (std::getline(std::cin, line)) {
            if (line == "eof") {
                break; // Конец ввода, игнорируем
            }

            // Проверка на начало и конец динамического блока
            if (line == "{") {
                if (!inDynamicBlock) {
                    inDynamicBlock = true; // Вошли в динамический блок
                    if (!commands.empty()) {
                        // Выводим предыдущие команды 
                        for (const auto& handler : handlers) {
                            handler->handleCommands(commands);
                        }
                        commands.clear();
                    }
                }
                continue; // Пропускаем обработку этой строки
            } 
            else if (line == "}") {
                if (inDynamicBlock) {
                    inDynamicBlock = false; // Вышли из динамического блока
                    continue; // Пропускаем обработку этой строки
                }
            }

            commands.push_back(line); // Добавляем команду

            // Проверка если достигли размера блока
            if (!inDynamicBlock && commands.size() == blockSize) {
                for (const auto& handler : handlers) {
                    handler->handleCommands(commands);
                }
                commands.clear(); // Очищаем список после обработки
            }
        }

        // Обработка оставшихся команд, если они есть
        if (!commands.empty() && !inDynamicBlock) {
            for (const auto& handler : handlers) {
                handler->handleCommands(commands);
            }
        }
    }

private:
    std::vector<std::shared_ptr<ICommandHandler>> handlers;
    int blockSize;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Use: " << argv[0] << " <N>\n";
        return 1;
    }

    int blockSize;
    try {
        blockSize = std::stoi(argv[1]);
        if (blockSize <= 0) {
            throw std::invalid_argument("Need N>0.");
        }
    } catch (const std::invalid_argument&) {
        std::cerr << "Uncorrect N.\n";
        return 1;
    }

    std::vector<std::shared_ptr<ICommandHandler>> handlers;
    handlers.push_back(std::make_shared<PrintCommandHandler>());
    handlers.push_back(std::make_shared<FileCommandHandler>());

    std::cout << "Enter commands (one command - one line). Use {} for make dynamic blocks:\n";
    CommandProcessor processor(handlers, blockSize);

    processor.processCommands();

    return 0;
}