load("@rules_cc//cc:cc_binary.bzl", "cc_binary")

# Common utilities library

filegroup(
    name = "inputs",
    srcs = glob(["input*.txt"]),
)

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

cc_binary(
    name = "day3",
    srcs = ["day3.cc"],
    data = [":inputs"],
    deps = [
        "//utils:common",
        "@abseil-cpp//absl/status",
        "@abseil-cpp//absl/strings",
    ],
)

