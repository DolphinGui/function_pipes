from conans import ConanFile
import os

class function_pipes(ConanFile):
    name = "function_pipes"
    version = "1.0.0"
    url = "https://github.com/dolphingui/function_pipes"
    description = "A header-only library for composing functions in c++17"
    license = "Unlicense"
    # No settings/options are necessary, this is header only
    exports_sources = ["include/*"]
    no_copy_source = True

    def package(self):
        self.copy("*.hpp", dst="include", src="public")

    def package_info(self):
        self.info.header_only()
