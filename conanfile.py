from conans import  ConanFile, CMake

class IeomppConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    requires = "Boost/1.60.0@lasote/stable"
    generators = "cmake"

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake "%s" %s' % (self.conanfile_directory, cmake.command_line))
        self.run('cmake --build . %s' % cmake.build_config)
