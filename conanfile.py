from conans import ConanFile

class TTVGConan(ConanFile):
    settings = "os", "compiler", "build_type", "arch"

    requires = (
        "boost/1.71.0@conan/stable",
        "sfml/2.5.1@bincrafters/stable",
        "spdlog/1.4.2@bincrafters/stable",
        "fmt/5.3.0@bincrafters/stable",
        "jsonformoderncpp/3.6.0@vthiery/stable",
        "luadist/5.2.3@owl/stable",
    )

    generators = "cmake"

    default_options = {
        "boost:shared": False,
        "boost:without_test": False,
        "boost:without_program_options": False,
        "boost:without_filesystem": False,
        "boost:without_thread": True,
        "boost:without_system": True,
        "boost:without_stacktrace": True,
        "boost:without_math": True,
        "boost:without_wave": True,
        "boost:without_container": True,
        "boost:without_contract": True,
        "boost:without_exception": True,
        "boost:without_graph": True,
        "boost:without_iostreams": True,
        "boost:without_locale": True,
        "boost:without_log": True,
        "boost:without_random": True,
        "boost:without_regex": True,
        "boost:without_mpi": True,
        "boost:without_serialization": True,
        "boost:without_coroutine": True,
        "boost:without_fiber": True,
        "boost:without_context": True,
        "boost:without_timer": True,
        "boost:without_chrono": True,
        "boost:without_date_time": True,
        "boost:without_atomic": True,
        "boost:without_graph_parallel": True,
        "boost:without_python": True,
        "boost:without_type_erasure": True
    }
