load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

# Common utilities library

cc_binary(
    name = "day1",
    srcs = ["day1.cc"],
    data = [":inputs"],
    deps = [
        "//utils:common",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/strings",
    ],
)
cc_binary(
    name = "day2",
    srcs = ["day2.cc"],
    data = [":inputs"],
    deps = [
        "//utils:common",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/strings",
    ],
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
