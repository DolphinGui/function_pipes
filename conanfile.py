import os
from conans import ConanFile, tools

class function_pipes(ConanFile):
    name = "function_pipes"
    version = "1.1.0"
    url = "https://github.com/dolphingui/function_pipes"
    description = "A header-only library for composing functions in c++17"
    license = "Unlicense"
    topics = ("conan", "function_pipes")
    exports_sources = "include/*"
    no_copy_source = True
    # No settings/options are necessary, this is header only

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
