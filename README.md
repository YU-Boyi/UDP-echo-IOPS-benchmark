# Introduce to *UDP echo IOPS benchmark*
It is a tool program to **test UDP delay**.
## Definition of UDP echo IOPS
We define a *UDP echo* procedure:
1. The client sends a fixed size empty UDP packet to the server.
2. The sever sends a same size empty UDP packet back to the client as long as client packet reached.

Just like:
1. Client says 'hello'
2. Sever says 'hello too'

We define IOPS(IO per second): average completion times of *UDP echo* in a second.

## UDP delay
We calculate UDP delay by formula:
> UDP-delay = 1000 / UDP-echo-IOPS / 2 (unit in milliseconds)

## Compilation
You should compile two programs with different settings.
```cpp
bool server_or_client = true;        // true: server     false: client
auto server_ip = "127.0.0.1";        // set to your real IP
auto server_port = 10024;
auto client_port = 10025;
//
constexpr auto msg_length = 8;  // packet length
auto test_time_sec = 10;             // test time
```
*Client* should set
> bool server_or_client = false;

*Sever* should set
> bool server_or_client = true;

and keep other codes the same.

This program only uses ASIO network library and it is included in
> ./UDP_echo_IOPS_benchmark/include

so you do NOT need to install any other dependencies.
## Environment
*UDP_echo_IOPS_benchmark.sln* is configured by Visual Studio Community 2022 in Windows 10.

If you are not Visual Studio Community 2022, use *main.cpp* seperatly.
For Linux:
> apt install libasio-dev

For Windows & Visual studio:
Add 
> ./include

to: Debug - Debug Properties - VC++ Directories - Include Directories
## Some experiment results
|  Hardware Configuration | UDP echo IOPS | UDP delay |
| ------------ | ------------ | ------------ |
|  Normal PC with onboard 1000M  | 4500 | 0.11 ms |
| Network server with onboard 1000M  | 9600 | 0.052 ms |
|  AMD 7H12 with 200GG Mellanox ConnectX-6  | 15000 | 0.033 ms |
|  Intel Xeon 6248R with 25G Mellanox ConnectX-5  | 29000 | 0.017 ms |

## Thanks
Thanks to my friend *Helixzzz* provides test machines.
