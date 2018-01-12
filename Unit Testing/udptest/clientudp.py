import socket
addr=('10.18.31.173',8888)
s=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
while True:
    msg=input("udpsent:")
    s.sendto(msg.encode('utf-8'),addr)
    if msg=='bye':
        break
s.close()