/*
 *  Copyright (c) 2011-2012 Maroš Kasinec
 *  Licensed under the GNU General Public License v3.
 *  See COPYING for more information.
 */

#include "plugin_manager.h"
#include "log.h"

#ifndef USE_PRECOMPILED_HEADERS
#ifdef WIN32
#include <direct.h>
#include <windows.h>
#else
#include <sys/types.h>
#include <dlfcn.h>
#endif
#endif

#include <dirent.h>
#include <stdio.h>
#include <iostream>
#include <sstream>

using std::vector;
using std::string;

namespace sdc {

PluginManager::PluginManager(Core* core)
    : AbstractManager(core),
      connectionconfigmodel_provider_(this) {}
PluginManager::~PluginManager() {}

void PluginManager::LoadPlugins() {
  LOG(INFO) << "Loading plugins...";

  /**
   * Each plugin MUST be in plugins dir relative to sdc executable.
   */
  const string plugin_dir = "./plugins";
  vector<string> libnames;
  DIR *d;
  struct dirent *dir;
  d = opendir(plugin_dir.c_str());
  if (d) {
    while ((dir = readdir(d)) != NULL)
    {
      char* name = dir->d_name;
      int i = 0;
      while ( name[i] != '\0' ) {
        ++i;
      }
      if (i > 3 && name[i-3] == '.' && name[i-2] == 's' && name[i-1] == 'o') {
        libnames.push_back(name);
      }
    }
    closedir(d);
  }

  vector<string>::iterator it;
  for (it = libnames.begin(); it != libnames.end(); ++it) {
    string path = plugin_dir + '/' + *it;
    LOG(TRACE) << "Trying to load library: " << path;
    LIB_HANDLE_TYPE lib_handle = LoadLibrary(path);
    registerLibrary* fn_register = (registerLibrary*) LoadLibrarySymbol(lib_handle, "registerLibrary");
    Registration* registration = fn_register();
    LOG(INFO) << registration->name_ << " plugin loaded.";
    LibSignature lib_sig = GetLibrarySignature(*it);
    lib_handles_[lib_sig] = lib_handle;
    for(std::vector<ClassDataRegistration>::iterator it = registration->new_plugins_.begin();
        it != registration->new_plugins_.end(); ++it) {
      it->lib_signature = lib_sig;
      class_data_[GetClassSignature(*it)] = (*it); // @anchor regFriend
    }
    delete registration;
  }
}

void PluginManager::UnloadPlugins() {
  LOG(INFO) << "Releasing plugins...";
  for(std::map<std::string, void*>::iterator i = lib_handles_.begin(); i != lib_handles_.end(); ++i) {
#ifdef WIN32
  FreeLibrary(i->second);
#else
  dlclose(i->second);
#endif
  }
}

LIB_HANDLE_TYPE PluginManager::LoadLibrary(const std::string & filepath) {
  // reset errors
  dlerror();
  LIB_HANDLE_TYPE lib_handle;
#ifdef WIN32
  lib_handle = LoadLibrary(TEXT(filepath.c_str()));
  if (!lib_handle) {
      std::cerr << "Cannot load library: " << TEXT(filepath.c_str()) << std::endl;
  }
#else
  lib_handle = dlopen(filepath.c_str(), RTLD_NOW);
  if (!lib_handle) {
      std::cerr << "Cannot load library: " << dlerror() << std::endl;
  }
#endif
  return lib_handle;
}

LIB_HANDLE_TYPE PluginManager::GetLibraryHandle(const PluginSignature & class_signature) {
  return lib_handles_[class_data_[class_signature].lib_signature];
}

void* PluginManager::LoadLibrarySymbol(LIB_HANDLE_TYPE lib_handle, const std::string & symbol) {
  // reset errors
  dlerror();
  // load symbol
  void* fn_handle = dlsym(lib_handle, symbol.c_str());
  const char* dlsym_error = dlerror();
  if (dlsym_error) {
    std::cerr << "Cannot load symbol: " << dlsym_error << std::endl;
  }
  return fn_handle;
}

void* PluginManager::LoadLibrarySymbol(const PluginSignature & class_signature) {
  ClassDataRegistration* class_data = &class_data_[class_signature];
  return LoadLibrarySymbol(lib_handles_[class_data->lib_signature], class_data->class_symbol);
}

LibSignature PluginManager::GetLibrarySignature(const std::string & lib_filename) const {
  return lib_filename;
}

PluginSignature PluginManager::GetClassSignature(const ClassDataRegistration & class_data) const {
  std::ostringstream signature;
  signature << class_data.lib_signature << class_data.type << class_data.class_name;
  return signature.str();
}

std::vector<ClassDataRegistration> PluginManager::GetClassData(const PluginType & plugin_type) const {
  std::vector<ClassDataRegistration> filtered;
  for(std::map<PluginSignature, ClassDataRegistration>::const_iterator it = class_data_.begin();
      it != class_data_.end(); ++it) {
    if(it->second.type == plugin_type)
      filtered.push_back(it->second);
  }
  return filtered;
}

} // namespace sdc
