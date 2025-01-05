#include <iostream>
#include <fstream>
#include <json/json.h>
#include <sstream>
#include <cstdlib> 

void clearScreen() {
    system("CLS");
}
void removeQuotes(std::string& path) {
    path.erase(std::remove(path.begin(), path.end(), '"'), path.end());
}
void message(std::string& input) {
    clearScreen();  // 清屏
    std::cout << "[1]读取文件 [2]加密文件" << std::endl;
    std::getline(std::cin, input);
    clearScreen();
}

void invertBits(std::string& data) {
    for (auto& byte : data) {
        byte = ~byte; // 按位取反
    }
}

bool readFileData_dec(const std::string filePath, Json::Value& jsonData,bool dec = true) {
    // 1. 读取文件
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "无法打开文件!" << std::endl;
        return false;  // 如果文件打开失败，返回 false
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    std::string jsonStr = buffer.str();

    if (dec) {
        invertBits(jsonStr);
    }

    // 创建一个 Json::CharReaderBuilder，用来解析 JSON
    Json::CharReaderBuilder readerBuilder;
    std::string errs;

    // 解析 JSON 字符串到 jsonData 对象
    std::istringstream s(jsonStr);
    if (Json::parseFromStream(readerBuilder, s, &jsonData, &errs)) {
    }
    return true;
}

bool writeFileData_enc(const std::string filePath, const Json::Value& jsonData, bool enc = true) {
    // 将JSON对象转换为字符串
    Json::StreamWriterBuilder writer;
    writer["emitUTF8"] = true;  // 输出 UTF-8 编码
    writer["enableYAMLCompatibility"] = true;  // 避免 Unicode 转义

    std::string jsonStr = Json::writeString(writer, jsonData);

    // 对数据进行位反转加密
    if (enc) {
        invertBits(jsonStr);
    }

    // 打开文件准备写入
    std::ofstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "无法创建文件!" << std::endl;
        return false;
    }
    // 写入加密后的数据
    file << jsonStr;
    file.close();

    return true;
}

int main() {
    Json::Value data;

    std::string choice;
    std::string path;
    message(choice);
    
    while (true) {
        if (choice != "1" && choice != "2") {
            message(choice);
            continue;  // 继续循环，重新选择操作   
        }     
        std::cout << "请输入文件路径(重选回车): ";
        std::getline(std::cin, path);
        removeQuotes(path);// 去掉路径中的双引号

        // 如果路径为空或用户输入0，重新选择操作
        if (path.empty()) {
            message(choice);
            continue;  // 继续循环，重新选择操作
        }

        else if (choice == "1") {
            readFileData_dec(path, data);
            std::cout << "读取文件内容: " << std::endl;
            std::cout << data << std::endl;

            std::string savefile;
            std::cout << "[1]保存文件" << std::endl;
            std::getline(std::cin, savefile);

            if (savefile == "1") {
                writeFileData_enc(path + ".dec", data, false);
                continue;
            }
        }

        else if (choice == "2") {
            clearScreen();  // 清屏
            readFileData_dec(path, data, false);  // 假设这是读取加密文件的方式
            writeFileData_enc(path + ".enc", data);
            std::cout << "加密完成，保存为: " << path + ".enc" << std::endl;
        }
    }

    return 0;
}