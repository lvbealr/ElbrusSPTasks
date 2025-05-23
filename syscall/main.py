#!/usr/bin/env python3

import os
import sys
import random
import stat
from pathlib import Path

EXTENSIONS      = ['.txt', '.bin', '.dat', '.log', '.tmp']
TEXT_EXTENSIONS = ['.txt', '.log']
PERMISSIONS     = [0o400, 0o200, 0o600, 0o444, 0o666, 0o700, 0o755, 0o777]

def generateFiles(numFiles, outputDir = "files"):
    Path(outputDir).mkdir(exist_ok = True)
    filePaths = []
    
    for i in range(numFiles):
        fileExtension = random.choice(EXTENSIONS)
        fileName = f"test_file_{i}{fileExtension}"
        filePath = os.path.join(outputDir, fileName)
        
        try:
            mode = 'w' if fileExtension in TEXT_EXTENSIONS else 'wb'

            with open(filePath, mode) as f:
                if fileExtension in TEXT_EXTENSIONS:
                    f.write("This is a test file with some content.\n" * random.randint(1, 10))
                else:
                    f.write(os.urandom(random.randint(10, 100)))
            
            permission = random.choice(PERMISSIONS)
            os.chmod(filePath, permission)

            filePaths.append(filePath)

        except Exception as err:
            print(f"Failed to create {filePath}: {err}")
    
    print(" ".join(filePaths))

def main():
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <number_of_files>")
        sys.exit(1)
    
    try:
        numFiles = int(sys.argv[1])

        if numFiles < 1:
            print("Number of files must be positive")
            sys.exit(1)
            
    except ValueError:
        print("Number of files must be an integer")
        sys.exit(1)
    
    generateFiles(numFiles)

if __name__ == "__main__":
    main()