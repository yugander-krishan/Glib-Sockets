# Glib-Sockets

The code is about simple server-client communication through sockets in C. We have made use of Glib, GIO libraries functionalities.


The motivation for this code is from [here](https://www.programmersought.com/article/76748398/#_120)

It's only drawback is that it's **synchronous** but today is all about async so I made some changes to it so that it works **asynchronously**.



**Compile server-async.c using:**
gcc -g  server-async.c -o server-async $(pkg-config --cflags --libs glib-2.0 gobject-2.0 gio-2.0)

**Compile client-async.c using:**
gcc -g  client-async.c -o client-async $(pkg-config --cflags --libs glib-2.0 gobject-2.0 gio-2.0)

**Steps to run:**
- Start the server by running server executable
- After that start the client by running client executable
