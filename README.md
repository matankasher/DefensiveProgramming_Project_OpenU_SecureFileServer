# DefensiveProgramming-Project-OpenU

### Secure files storage server
### Semester 2023a - Final Project - Grade 100. 

### Description
Server System to Save files at the server side via secure communication  
Client side at c++ , get client information , file to transfer , and port 
connect to server-side at python , send request to new-connect/re-connect.
the server create AES key, encrpyt it with public keyy of the client, send to client-side,
the client side decrypt the AES with his own private key.
with the AES key, the client encrypt the file he want to transfer.
the server side decrypt the file , verify with CRC, and save it.
### Server-Side
- Python
### Client-Side
- C + +
### Input
- port file 
- client file
- file to save
### Methods
- C ++
- Python
- TCP/IP communication
- Symmetric and asymmetric encryption
- RSA keys
- AES keys
- SQLite
- header and payload packs
- CRC checksum
### Screenshots

- Test file (input)
<img src="/screenshots/run file.png" alt="main page"/>
- Code after deployment marco
 <img src="/screenshots/after macro file.png" alt="main page"/>
- 16 base code
<img src="/screenshots/32 base code file.png" alt="main page"/>
- External label
 <img src="/screenshots/extern file.png" alt="main page"/>
- Data label file 
<img src="/screenshots/data label file.png" alt="main page"/>

