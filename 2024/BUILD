load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

# Common utilities library

# Day 1 binary
cc_binary(
    name = "day1",
    srcs = ["day1.cc"],
    data = [":inputs"],
    deps = ["//utils:common"],
)

cc_binary(
    name = "day2",
    srcs = ["day2.cc"],
    data = [":inputs"],
    deps = ["//utils:common"],
)

filegroup(
    name = "inputs",
    srcs = [
        "input1.txt",
        "input1_full.txt",
        "input2.txt",
        "input2_full.txt",
    ],
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
