## What is in the repo
A program that checks Spamhaus blacklist whether IP address is a spam or can have negative impact.

## Description
The program is using Winsock and Ws2tcpip libraries and therefore with DNS request checks whether a host is a spam.
For example, if we have the ip address 220.100.170.12, the program will convert that address to host 12.170.100.220.zen.spamhaus.org and after a check will return codes for Spamhaus public IP zone which are described [here](https://www.spamhaus.org/faq/section/DNSBL%2520Usage#200)

## How to run
- Compile using gcc or any other way. 
- Run the *.exe  file produced by the compiler followed by atleast one ip address like this:
  - ./name.exe 220.100.170.12 114.231.105.37
