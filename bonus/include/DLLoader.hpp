/*
** EPITECH PROJECT, 2026
** G-OOP-400-LIL-4-1-arcade-12
** File description:
** DLLoader
*/

#pragma once
#include <dlfcn.h>
#include <stdexcept>
#include <string>
#include <map>
#include "Item.hpp"

template <typename T>
class DLLoader {
    private:
    void *_handle;
    T* (*_entryPoint)(std::map<std::string, Arc::Item>&);

    public:
    DLLoader(const std::string &libPath, const std::string& entryPoint)
        : _handle(dlopen(libPath.c_str(), RTLD_LAZY)),
        _entryPoint(reinterpret_cast<T* (*)(std::map<std::string, Arc::Item>&)>(dlsym(_handle, entryPoint.c_str()))) {
        if (!_handle || !_entryPoint)
            throw std::runtime_error(dlerror());
    }

    ~DLLoader() {
        if (_handle)
            dlclose(_handle);
    }

    T *getInstance(std::map<std::string, Arc::Item>& items) {
        return _entryPoint(items);
    }
};
