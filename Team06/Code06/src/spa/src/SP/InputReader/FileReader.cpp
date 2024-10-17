
#include <SP/InputReader/FileReader.h>


FileReader::FileReader() {}

std::string FileReader::process(std::string filepath) {
    FILE *file = fopen(filepath.c_str(), "r");

    if (file == NULL) {
        fputs("File not found.", stderr);
        exit(1);
    }

    fseek(file, 0, SEEK_END);
    size_t fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);
    std::string fileContents(fileSize, ' ');
    fread(fileContents.data(), 1, fileSize, file);

    return fileContents;
}
