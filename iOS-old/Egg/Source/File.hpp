#pragma once

#include <exception>

class FileError : public std::exception // Is an abstract class
{
public:
	const char* path;
	FileError(const char* _path);
	// Pure virtual function (= 0), is const will be terminated if throws an exception
	virtual const char* what() const noexcept = 0;
};

class FileNotFoundError : public FileError {
public:
	FileNotFoundError(const char* _path);
	virtual const char* what() const noexcept;
};
