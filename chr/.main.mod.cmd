cmd_/home/parth/devicedriver/chr/main.mod := printf '%s\n'   main.o | awk '!x[$$0]++ { print("/home/parth/devicedriver/chr/"$$0) }' > /home/parth/devicedriver/chr/main.mod
