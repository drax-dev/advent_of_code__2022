import sys
import shutil
from datetime import datetime


include_list = [r"      <LanguageStandard>stdcpplatest</LanguageStandard>" "\n",
                 r"      <AdditionalIncludeDirectories>$(SolutionDir)\Utils\;%(AdditionalIncludeDirectories)</AdditionalIncludeDirectories>" "\n"]

library_list = [r"      <AdditionalDependencies>Utils.lib;%(AdditionalDependencies)</AdditionalDependencies>" "\n",
                r"      <AdditionalLibraryDirectories>$(OutputPath);%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>" "\n"]


def usage():
    print("\nThis script configure vcxproj file for AoC_2022 solution\n")
    print('Usage: ' + sys.argv[0] + ' <vcxproj_path>')


def create_file_backup(src_file_path):
    now = datetime.now()
    date_format = now.strftime("_%d_%b_%Y_%H_%M_%S")
    dst_dir = vcxproj_path + date_format + ".bak"
    shutil.copy2(src_file_path, dst_dir)


if __name__ == '__main__':
    if len(sys.argv) != 2:
        usage()
        exit(1)

    vcxproj_path = sys.argv[1]
    if not vcxproj_path.endswith(".vcxproj"):
        print(
            f"Wrong input argument: {vcxproj_path}\nArgument should be a path to vcxproj file eg. c:\\test\\test.vcxproj")
        exit(2)

    create_file_backup(vcxproj_path)
    with open(vcxproj_path, mode="r") as read_handle:
        data = read_handle.readlines()

    data_copy = data.copy()
    offset = 0
    for index in range(len(data)):
        if "<ConformanceMode>true</ConformanceMode>" in data[index]:
            data_copy = data_copy[0:index + offset] + include_list + data_copy[index + offset:]
            offset = offset + 2
        elif "<GenerateDebugInformation>true</GenerateDebugInformation>" in data[index]:
            data_copy = data_copy[0:index + offset] + library_list + data_copy[index + offset:]
            offset = offset + 2

    with open(vcxproj_path, 'w') as write_handle:
        write_handle.writelines(data_copy)
    print("DONE")
