#!../../../tools/confply/confply.py
from confply.cpp_compiler.config import *

confply_tool = "clang"
source_files = [
    "../../main.cpp",
    "rc_sample.cpp",
    "../../input/inSimple/inSimple.cpp",
    "../../networking/connection/http_server.cpp",
    "../../networking/connection/address.cpp",
    "../../networking/connection/socket.cpp",
    "../../networking/packet/packet.cpp",
    "../../networking/utils/encode.cpp",
    "../../networking/utils/encrypt.cpp"
]
warings = "all"
debug_info = True
link_libraries = ["stdc++", "pthread"]
standard = "c++17"
output_file = "rc_sample.bin"