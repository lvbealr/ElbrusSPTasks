# ElbrusSPTasks

## Installation:

```sh
git submodule update --init --remote --recursive

cmake -DCURRENT_CONFIG=Release -S . -B build && $(cd build && make)
build/fileInfo/fileInfo <file name>
```

---

```sh
build/fileInfo/fileInfo CMakeLists.txt

# File: CMakeLists.txt
# st_dev (device): 66311
# st_ino (inode): 1966806
# st_mode (mode): 100644
# st_nlink (number of links): 1
# st_uid (user ID): 1000
# st_gid (group ID): 1000
# st_rdev (device type): 0
# st_size (size in bytes): 1856
# st_blksize (block size): 4096
# st_blocks (number of blocks): 8
# st_atime (last access time): 1748236645
# st_mtime (last modification time): 1748236645
# st_ctime (last status change time): 1748236645
```

```sh
build/fileInfo/fileInfo fileInfo

# File: fileInfo
# st_dev (device): 66311
# st_ino (inode): 1992404
# st_mode (mode): 40755
# st_nlink (number of links): 4
# st_uid (user ID): 1000
# st_gid (group ID): 1000
# st_rdev (device type): 0
# st_size (size in bytes): 4096
# st_blksize (block size): 4096
# st_blocks (number of blocks): 8
# st_atime (last access time): 1748236645
# st_mtime (last modification time): 1748236645
# st_ctime (last status change time): 1748236645
```

```sh
build/fileInfo/fileInfo /dev/tty1

# File: /dev/tty1
# st_dev (device): 6
# st_ino (inode): 20
# st_mode (mode): 20600
# st_nlink (number of links): 1
# st_uid (user ID): 1000
# st_gid (group ID): 5
# st_rdev (device type): 1025
# st_size (size in bytes): 0
# st_blksize (block size): 4096
# st_blocks (number of blocks): 0
# st_atime (last access time): 1748200464
# st_mtime (last modification time): 1748233253
# st_ctime (last status change time): 1748200467
```

```sh
build/fileInfo/fileInfo /dev/loop0

# File: /dev/loop0
# st_dev (device): 6
# st_ino (inode): 987
# st_mode (mode): 60660
# st_nlink (number of links): 1
# st_uid (user ID): 0
# st_gid (group ID): 995
# st_rdev (device type): 1792
# st_size (size in bytes): 0
# st_blksize (block size): 512
# st_blocks (number of blocks): 0
# st_atime (last access time): 1748200458
# st_mtime (last modification time): 1748200458
# st_ctime (last status change time): 1748200458
```