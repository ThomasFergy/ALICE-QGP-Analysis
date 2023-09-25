import os

if __name__ == "__main__":
    # find all files withing the /lib directory
    Directories = os.listdir("./lib")
    cpp_files = []
    print("Searching for .cpp files:")
    for d in Directories:
        if d == "lib_compile.py":
            continue
        for f in os.listdir("./lib/" + d):
            if f.endswith(".cpp"):
                print("    Found: " + f)
                cpp_files.append(d + "/" + f)
    # for all .cpp files, compile them into .so files using ROOT
    print("Compiling .cpp files:")
    for f in cpp_files:
        print("    Compiling " + f + "...")
        command = "root -b -q -l 'lib/" + f + "+g'"
        print("    " + command)
        os.system(command)
