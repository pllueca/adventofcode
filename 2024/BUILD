# Common utilities library
cc_library(
    name = "utils",
    srcs = ["utils.cc"],
    hdrs = ["utils.h"],
    visibility = ["//2024:__subpackages__"],
)

# Day 1 binary
cc_binary(
    name = "day1",
    srcs = ["day1.cc"],
    deps = [":utils"],
    data = [":inputs"],
)

filegroup(
    name = "inputs",
    srcs = [
        "input1.txt",
        "input1_full.txt",
    ]
)

# Add more binaries for additional days below:
# cc_binary(
#     name = "day2",
#     srcs = ["day2.cc"],
#     deps = [":utils"],
# )
#
# cc_binary(
#     name = "day3",
#     srcs = ["day3.cc"],
#     deps = [":utils"],
# )
