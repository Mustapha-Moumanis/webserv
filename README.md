# Non-blocking HTTP Server (webserv):  42 School Project

[![Watch the video](./pic/webserv.gif)](https://www.youtube.com/watch?v=zwq--iPUd0k)

## About

This project was completed in collaboration with [Souad Hilal](https://github.com/SoTizme).

We developed a non-blocking HTTP server using **C++98**, implementing the **HTTP/1.1 methods**: GET, POST, and DELETE. The server efficiently handles client-server communication using a single `poll()` for all I/O operations.

## Key Features

- **Static file serving**: Serve files from a specified root directory.
- **File uploads**: Handle user-uploaded files seamlessly.
- **CGI handling**: Process dynamic content with Common Gateway Interface.
- **Customizable configuration**:
  - Specify ports, routes, and error pages.
- **High-performance design**: Stress-tested for resilience and accuracy under high load conditions.

This project enhanced my skills in **network programming**, **system design**, and **HTTP protocols**. It demonstrates robust performance and efficient resource utilization in a non-blocking server architecture.