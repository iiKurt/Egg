#include "Resource.hpp"

#include "File.hpp"

Resource::Resource(const char* _path) {
	this->path = _path;
	io = SDL_RWFromFile(this->path, "r");

	if (io == NULL) {
		throw FileNotFoundError(this->path);
	}
}

#ifdef _WIN32
#include <windows.h>

// Windows embedded resource loading
Resource::Resource(const char* _path, int name, int type) {
	path = _path;

	HMODULE handle = ::GetModuleHandle(NULL);
	HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name), MAKEINTRESOURCE(type));
	HGLOBAL rcData = ::LoadResource(handle, rc);
	DWORD size = ::SizeofResource(handle, rc);
	void* data = ::LockResource(rcData);

	buffer = new char[size+1];
	::memcpy(buffer, data, size);
	buffer[size] = 0; // Null terminator

	io = SDL_RWFromMem(buffer, size + 1);
}
#endif

Resource::~Resource() {
	#ifdef _WIN32
	delete[] buffer;
	#endif
	// TODO: Fix segfault when closing RWops after using it in SDL_LoadBMP_EW (closing before is fine -- maybe an SDL bug??)
	//SDL_RWclose(io);
}
