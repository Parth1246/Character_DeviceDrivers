cmd_/home/parth/devicedriver/practice_driver/hello_world/mymodule.mod := printf '%s\n'   mymodule.o | awk '!x[$$0]++ { print("/home/parth/devicedriver/practice_driver/hello_world/"$$0) }' > /home/parth/devicedriver/practice_driver/hello_world/mymodule.mod