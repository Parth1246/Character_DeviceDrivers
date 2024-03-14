cmd_/home/parth/devicedriver/hello/main.mod := printf '%s\n'   main.o | awk '!x[$$0]++ { print("/home/parth/devicedriver/hello/"$$0) }' > /home/parth/devicedriver/hello/main.mod
