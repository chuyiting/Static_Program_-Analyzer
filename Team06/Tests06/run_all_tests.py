import subprocess
import os
from sys import platform
import shutil
import sys

test_root = os.getcwd()
test_root_github_action = "/Users/runner/work/_temp/build/src/autotester/Tests06"
out_path = os.path.join(test_root, "out")
SINGLE_SOURCE_NAME = "Success_source.txt"


def add_analysis_format():
    source = os.path.join(test_root, "analysis.xsl")
    destination = output_path = os.path.join(test_root, "out/analysis.xsl")
    if not os.path.exists(destination):
        shutil.copy2(source, destination)


def get_auto_tester_path(env="local"):
    if env != "local" and platform == "win32":
        return os.path.join(test_root_github_action, "../autotester.exe")
    elif env != "local":
        return os.path.join(test_root_github_action, "../autotester")

    if platform == "win32":
        return os.path.join(test_root, "../autotester.exe")
    else:
        return os.path.join(test_root, "../autotester")


# create the output directory if not exists
def create_output_dir():
    directory = "out"
    output_path = os.path.join(test_root, directory)
    if not os.path.exists(output_path):
        os.makedirs(output_path)


def list_immediate_subdir_path(path):
    return [f.path for f in os.scandir(path) if f.is_dir()]


def run_single_level_tests(env="local"):
    dirs = list_immediate_subdir_path(test_root)
    auto_tester_path = "/Users/runner/work/_temp/build/src/autotester/autotester"
    for directory in dirs:
        for file in os.listdir(directory):
            if file.endswith("queries.txt"):
                file_name = file[:-4]
                source_file_name = file.replace("queries.txt", "source.txt")
                source_file_path = os.path.join(directory, source_file_name)
                if not os.path.exists(source_file_path):
                    source_file_path = os.path.join(directory, SINGLE_SOURCE_NAME)
                query_file_path = os.path.join(directory, file)
                output_file_path = os.path.join(out_path, file_name + "_out.xml")
                print(sys.path)
                subprocess.run([auto_tester_path, source_file_path, query_file_path, output_file_path])


def run_double_level_tests(env="local"):
    dirs = list_immediate_subdir_path(test_root)
    auto_tester_path = "/Users/runner/work/_temp/build/src/autotester/autotester"
    for directory in dirs:
        for testDir in list_immediate_subdir_path(directory):
            for file in os.listdir(testDir):
                if file.endswith("queries.txt"):
                    file_name = file[:-4]
                    source_file_name = file.replace("queries.txt", "source.txt")
                    source_file_path = os.path.join(testDir, source_file_name)
                    query_file_path = os.path.join(testDir, file)
                    output_file_path = os.path.join(out_path, file_name + "_out.xml")
                    subprocess.run([auto_tester_path, source_file_path, query_file_path, output_file_path])


def main():
    if len(sys.argv) >= 3:
        env = sys.argv[2].lower()
    else:
        env = "local"

    create_output_dir()
    add_analysis_format()
    run_single_level_tests(env)
    run_double_level_tests(env)

    if env == "ci":
        return

    subprocess.run(["python", "-m", "http.server", "3000"])


main()

