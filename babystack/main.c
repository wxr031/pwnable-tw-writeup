int initialize()
{
	signal(14, (__sighandler_t)handler);
	alarm(0x708u);
	addr_ptr = mmap(0LL, 0x1000uLL, 3, 34, -1, 0LL);
	if ( addr_ptr == (const void *)-1LL )
	{
		puts("mmap error");
		exit(0);
	}
	setvbuf(stdin, 0LL, 2, 0LL);
	return setvbuf(stdout, 0LL, 2, 0LL);
}

int __fastcall password(const char *dst)
{
	size_t len; // rax
	char s[128]; // [rsp+10h] [rbp-80h]

	printf("Your passowrd :");
	nread(s, 127u);
	len = strlen(s);
	if ( strncmp(s, dst, len) )
		return puts("Failed !");
	auth = 1;
	return puts("Login Success !");
}

int __fastcall magic_copy(char *dst) {
	char src[128]; // [rsp+10h] [rbp-80h]
	printf("Copy :");
	nread(src, 63u);
	strcpy(dst, src);
	return puts("It is magic copy !");
}
char *__fastcall nread(char *buf, unsigned int num) {
	int nr; // [rsp+1Ch] [rbp-4h]
	nr = read(0, buf, num);
	if ( nr <= 0 )
	{
		puts("read error");
		exit(1);
	}
	if ( buf[nr - 1] == '\n' )
	{
		buf[nr - 1] = '\0';
	}
	return last;
}

int main(__int64 a1, char **a2, char **a3) {
	_QWORD *addr_ptr_tmp; // rcx
	__int64 v4; // rdx
	char cp_buff[64]; // [rsp+0h] [rbp-60h]
	char buff[16]; // [rsp+40h] [rbp-20h]
	char choice; // [rsp+50h] [rbp-10h]

	initialize();
	fd[0] = open("/dev/urandom", 0);
	read(fd[0], buff, 16uLL);
	addr_ptr[0] = buff[0];
	addr_ptr[1] = buff[1];
	close(fd[0]);
	while ( 1 )
	{
		write(1, ">> ", 3uLL);
		_read_chk(0LL, &choice, 16LL, 16LL);
		if ( choice == '2' )
			break;
		if ( choice == '3' )
		{
			if ( auth )
				magic_copy(cp_buff);
			else
				puts("Invalid choice");
		}
		else if ( choice == '1' )
		{
			if ( auth )
				auth = 0;
			else
				password(buff);
		}
		else
		{
			puts("Invalid choice");
		}
	}
	if ( !auth )
		exit(0);
	if ( memcmp(buff, addr_ptr, 16uLL) )
		__stack_chk_fail();
	return 0LL;
}
