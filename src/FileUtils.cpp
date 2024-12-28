#include "FileUtils.h"
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/Path.h>
#include <llvm/Support/raw_ostream.h>
#include <system_error>

std::string FileUtils::readFile(const std::string& filePath)
{
    auto bufferOrError = llvm::MemoryBuffer::getFile(filePath);
    if (!bufferOrError)
    {
        llvm::errs() << "Error reading file: " << filePath << "\n";
        return "";
    }
    return bufferOrError->get()->getBuffer().str();
}

std::vector<std::string> FileUtils::getFilesInDirectory(const std::string& dirPath)
{
    std::vector<std::string> files;
    std::error_code ec;

    for (llvm::sys::fs::recursive_directory_iterator it(dirPath, ec), end; it != end && !ec; it.increment(ec))
    {
        if (llvm::sys::fs::is_regular_file(it->path()))
        {
            files.push_back(it->path());
        }
    }

    if (ec)
    {
        llvm::errs() << "Error iterating directory: " << dirPath << " - " << ec.message() << "\n";
    }

    return files;
}

std::vector<FileUtils::FileData> FileUtils::readFiles(const std::vector<std::string>& fileData)
{
    std::vector<FileData> files;
    for (const auto& data : fileData)
    {
        auto bufferOrError = llvm::MemoryBuffer::getFile(data);
        if (!bufferOrError)
        {
            llvm::errs() << "Error reading file: " << data << "\n";
            continue;
        }
        files.emplace_back(data, bufferOrError->get()->getBuffer().str());
    }
    return files;
}

void FileUtils::writeFiles(const std::vector<FileData>& fileData)
{
    for (const auto& data : fileData)
    {
        std::error_code ec;
        llvm::raw_fd_ostream outFile(data.fileName, ec, llvm::sys::fs::OpenFlags::OF_None);
        if (!ec)
        {
            outFile << data.fileContents;
        }
        else
        {
            llvm::errs() << "Error writing to file: " << ec.message() << "\n";
        }
    }
}