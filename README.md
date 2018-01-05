# SmallWebServerContainer

## Achieved
* Serve the static pages and files
* use multithread method to handle massive clients (C++11 thread)
* alternatively use event driven method to hanld massive clients (UNIX kqueue)

## TODO
* Implement the event driven mode in main thread, and handle the request in other thread
* coress paltfrom the project (especially the kqueue)