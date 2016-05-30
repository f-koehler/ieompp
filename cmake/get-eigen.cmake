function(get_eigen GET_EIGEN_SOURCE_PATH)
    include(ExternalProject)
    ExternalProject_Add(
        External_Eigen
        SOURCE_DIR ${GET_EIGEN_SOURCE_PATH}
        URL https://bitbucket.org/eigen/eigen/get/3.2.8.tar.gz
        URL_HASH SHA512=be52b81de726938e81ad4d2ef6d65e8d3ee9dc70038f22a4fd0b5df98bd2d4a4d1bc5f78e6580e81abaff389b8003f9239b0e1476f93ee9236c946ae215896b7
        CONFIGURE_COMMAND ""
        CMAKE_COMMAND     ""
        BUILD_COMMAND     ""
        INSTALL_COMMAND   ""
    )
endfunction()
