# AES256_implem
Implementation of AES256 CBC with BearSSL

## Encryption Time/data table (binary execution time)

| Size | Time count (in tick) | milliseconds |
| ----------- | ----------- | ----------- |
| 1ko | 157118 | 15 |
| 10Mo | 1390344 | 139 |
| 1Go | 103523776 | ~10000 |
| 2Go | 207048598 | ~20000 |

### Warning: this is a very tiny project from 2021, you can't use this on a file > 4294967295 Bytes (sizeof unsigned long)

To continue the project one day it would be necessary to:
1. Use GMP library
2. Use POSIX environement and use the syscall STAT
