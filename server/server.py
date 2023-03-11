# © All right reserved to Matan Kasher 2023A
import socket
from db_sqlite import sql_client_connect, sql_files_connect
from responeHandler import first_connect_request_handler
DEFAULT_PORT = 1234
PORT_FILE = "port.info.txt"
READ = "r"

def getport():
    try:
        portFile = open(PORT_FILE, READ)
    except OSError:
        print("couldn't open the file: port.info, using port 1234")
        return int(DEFAULT_PORT)
    port = portFile.readline()
    portFile.close()
    return int(port)



if __name__ == "__main__":
    print("Welcome to Host server to save files (version 3)")
# Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to a port
    server_address = ('localhost', getport())
    print(f'Starting up on {server_address[0]} port {server_address[1]}')
    sock.bind(server_address)

# Listen for incoming connections
    sock.listen(1)
    sql_client_connect()
    sql_files_connect()
    while True:
    # Wait for a connection
        print('Waiting for a connection...')
        try:
            connection, client_address = sock.accept()
            print('Connection from', client_address , '\n')
            first_connect_request_handler(connection)
        finally:
            connection.close()


