# SmallWebServerContainer

## Achieved
* serve the static pages and files
* use multithread method to handle massive clients (C++11 thread)
* alternatively use event-driven method to handle massive clients (UNIX kqueue)  
* maintain the connection to support not only http1.0 but also http1.1

## TODO
* implement the event-driven mode in main thread, and handle the request in other thread (not sure if this will be faster or slower)
* cross-platform the project (especially the kqueue and thread libraries)  
* implement CGI to support different web backends

## Test
Using Webbech, sending the http1.1 requests with 500 clients. Obviously, the event-driven method has much larger throughout but failed more times. Maybe because event-driven has only one thread so the response time is not guaranteed. (if the response time is too large, webbench may treat it as a fail)
### Multi-Thread Method  
Running info: 500 clients, running 30 sec.  
Speed=33594 pages/min, 1121 bytes/sec.  
Requests: 16548 susceed, 249 failed.  
### Event driven with kqueue  
Runing info: 500 clients, running 30 sec.  
Speed=35256 pages/min, 82002 bytes/sec.  
Requests: 16762 susceed, 866 failed.  
