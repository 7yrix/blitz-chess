#include <filesystem>
#include <unistd.h>

std::filesystem::path getAbsoluteExecutableDirectory() {
	#if defined(_MSC_VER)
			wchar_t path[FILENAME_MAX] = { 0 };
			GetModuleFileNameW(nullptr, path, FILENAME_MAX);
			return std::filesystem::path(path).parent_path().string();
	#else
			char path[FILENAME_MAX];
			ssize_t count = readlink("/proc/self/exe", path, FILENAME_MAX);
			return std::filesystem::path(std::string(path, (count > 0) ? count: 0)).parent_path().string();
	#endif
}