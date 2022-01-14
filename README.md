# LibConnTor
*A C++ library to easily connect to a Tor SOCKS5 proxy server*

## Compatibility
Windows only because of dependency on WinSock2

## Compile
This is compiled using g++ from MSYS on Windows
```
g++ -shared libconntor.cpp -o libconntor.dll -lws2_32
```

## Documentation

### Initialization
To initialize make an object of the libconntor class.
  It takes two arguments the WSA version and if output is enabled.
```
libconntor tor(MAKEWORD(2,2), true);
```

### Connect to Tor's proxy
To connect to Tor's proxy and send the first packet required by the SOCKS5 protocol, you need to call the ```torconnect``` function, it takes two arguments: IP address of SOCKS5 server and the port. It outputs ```0``` if successful and ```1``` if unsuccessful.
```
if(tor.torconnect("127.0.0.1", 9050) != 0) {
  // Connection failed
}
```

### Connect host and get the SOCKET
To connect to a host you need to call the ```hostconnect``` function. It takes two arguments: the domain name (Make sure only the domain name and no http prefix or slashes) and port. It outputs the SOCKET if successful and NULL if not.
```
SOCKET sock = tor.hostconnect("google.com", 80);
```