import os
import sys
import glob

if __name__ == "__main__":
    filename = sys.argv[1]
    file_dir = os.path.dirname(filename)
    # remove all .so files, .d files, and .pcm files
    files = glob.glob(file_dir + os.sep + "*.pcm")
    for f in files:
        os.remove(f)
    files = glob.glob(file_dir + os.sep + "*.d")
    for f in files:
        os.remove(f)
    files = glob.glob(file_dir + os.sep + "*.so")
    for f in files:
        os.remove(f)
