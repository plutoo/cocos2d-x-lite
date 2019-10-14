cmake_minimum_required(VERSION 3.8)


project(google-breakpad CXX)

add_library(google-breakpad 
    src/client/linux/crash_generation/crash_generation_client.cc
    src/client/linux/dump_writer_common/thread_info.cc
    src/client/linux/dump_writer_common/ucontext_reader.cc
    src/client/linux/handler/exception_handler.cc
    src/client/linux/handler/minidump_descriptor.cc
    src/client/linux/log/log.cc
    src/client/linux/microdump_writer/microdump_writer.cc
    src/client/linux/minidump_writer/linux_dumper.cc
    src/client/linux/minidump_writer/linux_ptrace_dumper.cc
    src/client/linux/minidump_writer/minidump_writer.cc
    src/client/minidump_file_writer.cc
    src/common/android/breakpad_getcontext.S
    src/common/md5.cc
    src/common/string_conversion.cc
    src/common/linux/elfutils.cc
    src/common/linux/file_id.cc
    src/common/linux/guid_creator.cc
    src/common/linux/linux_libc_support.cc
    src/common/linux/memory_mapped_file.cc
    src/common/linux/safe_readlink.cc
)

target_include_directories(google-breakpad PRIVATE 
    src
    src/common/android/include
)