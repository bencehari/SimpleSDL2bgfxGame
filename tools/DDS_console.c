#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

#include "DDS.h"

// AC stands for ANSI_COLOR
#define AC_RED     "\x1b[31m"
#define AC_GREEN   "\x1b[32m"
#define AC_YELLOW  "\x1b[33m"
#define AC_BLUE    "\x1b[34m"
#define AC_MAGENTA "\x1b[35m"
#define AC_CYAN    "\x1b[36m"
#define AC_RESET   "\x1b[0m"

static void print_usage(const char* _argv0);

static bool check_file_extension(const char* _filePath);

static int print_dds_file_info_local(const char* _filePath);
static int print_dds_file_infos_in_directory(const char* _dirPath);
static int save_dds_file_to_targa_local(const char* _filePath);

int main(int argc, char* argv[]) {
	int exitCode = EXIT_SUCCESS;
	
	if (argc < 2) {
		puts(AC_RED "[ERR]" AC_RESET " No target file or directory provided.");
		print_usage(argv[0]);
		return exitCode;
	}
	
	if (argc == 2) {
		if (!check_file_extension(argv[1])) return EXIT_FAILURE;
		exitCode = save_dds_file_to_targa_local(argv[1]);
	}
	else if (argc == 3 && strcmp(argv[1], "-i") == 0) {
		if (!check_file_extension(argv[2])) return EXIT_FAILURE;
		exitCode = print_dds_file_info_local(argv[2]);
	}
	else if (argc == 3 && strcmp(argv[1], "-d") == 0) {
		// TODO: Create targa(s) from all dds files in folder
		puts(AC_MAGENTA "Not implemented yet." AC_RESET);
	}
	else if (argc == 4 && strcmp(argv[1], "-i") == 0 && strcmp(argv[2], "-d") == 0) {
		print_dds_file_infos_in_directory(argv[3]);
	}
	else {
		print_usage(argv[0]);
	}
	
	return exitCode;
}

static int get_file_name_index(const char* _filePath) {
	int len = strlen(_filePath);
	int i = len - 1;
	for (; i >= 0; i--) {
		if (_filePath[i] == '\\' || _filePath[i] == '/') break;
	}
	if (i != 0) i++;
	return i;
}

static void print_usage(const char* _argv0) {
	const char* fileName = _argv0 + get_file_name_index(_argv0);
	printf(
		"Usage:\n"
		"------\n"
		"%s <path-of-dds-image>.dds           Create targa from image.\n"
		"%s -i <path-of-dds-image>.dds        Prints info about the image.\n"
		"%s -d <path-of-parent-directory>     Create targa(s) from all dds files in folder. Must ends with '\\' or '/'.\n"
		"%s -i -d <path-of-parent-directory>  Prints info about all dds files in folder. Must ends with '\\' or '/'.\n",
		fileName, fileName, fileName, fileName);
}

static bool check_file_extension(const char* _filePath) {
	size_t filePathLen = strlen(_filePath);
	const char* ext = _filePath + filePathLen - 4;
	if (strcmp(ext, ".dds") != 0 && strcmp(ext, ".DDS") != 0) {
		puts(AC_RED "[ERR]" AC_RESET " Extension mismatch (not .dds or .DDS).");
		return false;
	}
	return true;
}

static int print_dds_file_info_local(const char* _filePath) {
	FILE* file = fopen(_filePath, "rb");
	if (file == NULL) {
		printf(AC_RED "[ERR]" AC_RESET " Failed to open file: \"%s\"", _filePath);
		fclose(file);
		return EXIT_FAILURE;
	}
	
	int exitCode = print_dds_file_info(file);

	fclose(file);
	
	return exitCode;
}

static int print_dds_file_infos_in_directory(const char* _dirPath) {
	DIR* dir = opendir(_dirPath);
	if (dir == NULL) {
		printf(AC_RED "[ERR]" AC_RESET " Folder '%s' is invalid.", _dirPath);
		return EXIT_FAILURE;
	}

	size_t dirLen = strlen(_dirPath);
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL) {
		size_t fileNameLen = strlen(entry->d_name);
		if (fileNameLen < 5) continue;
		char* ext = entry->d_name + fileNameLen - 4;
		if (strcmp(ext, ".dds") != 0 && strcmp(ext, ".DDS") != 0) continue;
		
		char* filePath = malloc(sizeof(char) * (dirLen + fileNameLen + 1));
		memcpy(filePath, _dirPath, dirLen);
		memcpy(filePath + dirLen, entry->d_name, fileNameLen);
		filePath[dirLen + fileNameLen] = '\0';
		
		printf(AC_YELLOW "%s\n" AC_RESET, filePath);
		FILE* file = fopen(filePath, "rb");
		if (file == NULL) {
			printf(AC_RED "[ERR]" AC_RESET " Failed to open file '%s'.", filePath);
			continue;
		}
		
		print_dds_file_info(file);
		
		fclose(file);
	}
	
	closedir(dir);
	
	return EXIT_SUCCESS;
}

static int save_dds_file_to_targa_local(const char* _filePath) {
	FILE* file = fopen(_filePath, "rb");
	if (file == NULL) {
		printf(AC_RED "[ERR]" AC_RESET " Failed to open file: \"%s\"", _filePath);
		fclose(file);
		return EXIT_FAILURE;
	}
	
	const char* fileName = _filePath + get_file_name_index(_filePath);
	size_t len = strlen(fileName);
	
	char withoutExtension[len - 3];
	memcpy(withoutExtension, fileName, len - 4);
	withoutExtension[len - 4] = '\0';
	
	int exitCode = save_dds_file_to_targa(file, withoutExtension);

	fclose(file);
	
	return exitCode;
}
