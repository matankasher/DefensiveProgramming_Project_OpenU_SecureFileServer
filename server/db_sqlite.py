import sqlite3
from enumFields import Generals
from datetime import datetime

def sql_client_connect():
    conn = sqlite3.connect(Generals.DBServer.value)
    c = conn.cursor()
    c.execute(''' SELECT count(name) FROM sqlite_master WHERE type='table' AND name='clients' ''')
    if c.fetchone()[0] != 1:  # check if the table already exits
        conn.execute(Generals.create_client_table.value)  # create table if the table doesn't exist
    conn.commit()
    conn.close()

def is_old_client(name):
    conn = sqlite3.connect(Generals.DBServer.value)
    cursor_conn = conn.cursor()
    cursor_conn = conn.execute("""SELECT PUBLICKEY FROM clients WHERE NAME =?""", [name])
    result = cursor_conn.fetchone()
    conn.commit()
    conn.close()
    if result:
        return result
    return "null"


def insert_client(id, name, public_key, aes):
    conn = sqlite3.connect(Generals.DBServer.value)
    cursor_conn = conn.cursor()
    rows = [id, name, public_key, datetime.now(), aes]
    try:
        cursor_conn.execute('insert into clients values (?,?,?,?,?)', rows)
    except OSError:
        print("error to save file in DB")
    conn.commit()
    conn.close()

def sql_files_connect():
    conn = sqlite3.connect(Generals.DBServer.value)
    c = conn.cursor()
    c.execute(''' SELECT count(name) FROM sqlite_master WHERE type='table' AND name='files' ''')
    if c.fetchone()[0] != 1:  # check if the table already exits
        conn.execute(Generals.create_file_table.value)  # create table if the table doesn't exist
    conn.commit()
    conn.close()


def insert_file(id, fileName, pathName, VERIFIED):
    conn = sqlite3.connect(Generals.DBServer.value)
    cursor_conn = conn.cursor()
    rows = [id, fileName, pathName, VERIFIED]
    try:
        cursor_conn.execute('insert into files values (?,?,?,?)', rows)
        print("The file is secure and saved!")

    except OSError:
        print("error to save file in DB")
    conn.commit()
    conn.close()
