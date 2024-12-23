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
