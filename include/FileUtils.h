#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <string>
#include <vector>

/// @brief Utility class for file operations
class FileUtils
{
public:
    /**
     * @brief Read the contents of a file
     * @param filePath -> The path to the file
     * @return -> The contents of the file
     */
    static std::string readFile(const std::string& filePath);

    /**
     * @brief Write data to a file
     * @param filePath -> The path to the file
     * @param data -> The data to write
     */
    static std::vector<std::string> getFilesInDirectory(const std::string& dirPath);

    struct FileData
    {
        std::string fileName;
        std::string fileContents;

        FileData(std::string fileName, std::string fileContents)
            : fileName(std::move(fileName)), fileContents(std::move(fileContents))
        {
        }
    };

    /**
     * @brief Read the contents of multiple files
     * @param fileData -> A vector of FileData objects
     */
    static std::vector<FileData> readFiles(const std::vector<std::string>& fileData);

    /**
     * @brief Write data to multiple files
     * @param fileData -> A vector of FileData objects
     */
    static void writeFiles(const std::vector<FileData>& fileData);
};

#endif // FILEUTILS_H
