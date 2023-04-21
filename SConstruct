import os
from glob import glob
from pathlib import Path
import platform

# Include Godot's C++ bindings SConstruct
env = SConscript("godot-cpp/SConstruct")

# Add source files.
env.Append(CPPPATH=["src/"])
sources = Glob("src/register_types.cpp")
sources += Glob("src/embed_api.cpp")

# Find gdextension path even if the directory or extension is renamed.
(extension_path,) = glob("*.gdextension")

# Find the addon path (e.g. project/addons/embed-external-editor).
addon_path = Path(extension_path).parent

# Find the extension name from the gdextension file (e.g. embed-external-editor).
extension_name = Path(extension_path).stem

# Set build configuration
debug_or_release = "release" if env["target"] == "template_release" else "debug"

# Create the library target (e.g. libembed-external-editor.linux.debug.x86_64.so).
library = env.SharedLibrary(
    "bin/lib{}.{}.{}.{}{}".format(
        extension_name,
        env["platform"],
        debug_or_release,
        env["arch"],
        env["SHLIBSUFFIX"],
    ),
    source=sources,
)


Default(library)
