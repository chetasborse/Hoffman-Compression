#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
typedef struct iden {
	int freq;
	char *val;
}iden;
int getsum(char *value, int size) {
	int sum = 0, i;
		for(i = 0; i < size; i++) {
			if(value[i] == '1')
				sum = sum + pow(2, size - i - 1);		
		}
	return sum;
}
char convtochar(char *b, int *p) {
	int i, num = 0, c;
	char ch;
	c = *p;
	for(i = c - 1; i >= c - 8; i--)
	//for(i = c - 1; i >= c - 7; i--)
		if(b[i] == '1')
			num = num + pow(2, c - i- 1);
	ch = num;
	return ch;
	
}
void compress(iden *a, int size, int fd, int lets, int count) {
	int size1 = size, char1, j, c = 3, fd1, i, h, size2 = size, val, l, ball = 0, g = 0, rem;
	char ch, *b, *an;
	an = (char *)malloc(sizeof(char) * 8);
	printf("size = %d\ncount = %d\n", size, count);
	fd1 = open("comp.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	lseek(fd, 0, SEEK_SET);
	size = 4;
	printf("\nIn compression\n");
	b = (char *)malloc(sizeof(char) * size);
	while(size2 > 0) {
		i = size2 % 2;
		g++;
		size2 = size2 / 2;
		if(i == 1)
			b[c--] = '1';
		else
			b[c--] = '0';
	}
	while(c >= 0) {
		b[c--] = '0';
		g++;
	}
	printf("\nAfter First stage: count = %d", g);
	size = size + 8;
	//size = size + 7;
	c = 11;
	//c = 10;
	b = (char *)realloc(b, sizeof(char) * size);
	while(count > 0) {
		i = count % 2;
		g++;
		count = count / 2;
		if(i == 1)
			b[c--] = '1';
		else
			b[c--] = '0';
	}
	while(c >= 4) {
		b[c--] = '0';
		g++;
	}
	printf("\nAfter stage 2: count = %d", g);
	c = 19;
	//c = 17;
	size = size + 8 + size1;
	//size = size + 7 + size1;
	//printf("\nIn Compress\n");
	for(i = 0; i < 256; i++) {
		if(a[i].freq != 0) {
			//printf("%d\n", i);
			l = c - 7;
			//l = c - 6;
			b = (char *)realloc(b, sizeof(char) * size);
			val = i;
			while(val > 0) {
				g++;
				if(val % 2 == 1) 
					b[c--] = '1';
				else
					b[c--] = '0';
				val = val / 2;
			}
			
			while(c >= l) {
				b[c--] = '0';
				g++;
			}
			//c = c + 8;
			c = c + 9;
			for(j = 0; j < size1; j++) {
				b[c++] = a[i].val[j];
				g++;
			}
			//size = size + 8 + size1;
			size = size + 8 + size1;
			c = c + 7;	
		}
	}
	printf("\nAfter third stage: count = %d", g);
	c = c - 7; //Last element
	//printf("%s", b);
	printf("length1 = %d\n", strlen(b));
	//size = size - 8;
	size = size - 7;
	while(read(fd, &ch, 1)) {
		ball++;
		char1 = ch;
		//printf("%d\n", char1);
		for(j = 0; j < size1; j++) {
			b[c++] = a[char1].val[j];
			g++;
		}
		size = size + size1;
		b = (char *)realloc(b, sizeof(char) * size);
	}
	printf("\n%s\n", b);
	printf("\nAfter final stage: count = %d\n", g);
	printf("\nSize of string = %ld", strlen(b));
	printf("\nNo. of letters during compression = %d\n", ball);
	//h = (lets * 5) % 8;
	//h = (lets * 5) % 7;
	rem = g % 8;
	rem = 8 - rem;
	g = g + rem;
	size = size + rem;
	b = (char *)realloc(b, sizeof(char) * size);
	printf("size = %d\n", size);
	for(i = 0; i < rem; i++)
		b[c++] = '0';
	//c = 8;
	c = 8;
	//lets = (lets * 5) / 8;
	//lets = (lets * 5) / 7;
	g = g / 8;
	printf("g = %d\nAfter\n", g);
	
	for(i = 0; i < g; i++) {
		ch = convtochar(b, &c);
		rem = ch;
		for(j = 0; j < 8; j++) {	
			if(rem % 2 == 1)
				an[7 - j] = '1';
			else
				an[7 - j] = '0';
			rem = rem / 2;
			
		}	
		printf("%s", an);
		write(fd1, &ch, 1);
		c = c + 8;
		//c = c + 7;	
	}
	close(fd);
	close(fd1);	
}
void decompress() {
	iden a[128];
	int fd, i, num, c = 8, d, /*size = 8*/size = 8, k = 7, fd1, count = 0, sum, j, m, a1[128], p, limit, ball = 0;
	char ch, *f, *g;
	g = (char *)malloc(sizeof(char) * size);
	f = (char *)malloc(sizeof(char) * size);
	fd = open("comp.txt", O_RDONLY);
	fd1 = open("comp2.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	while(read(fd, &ch, 1)) {
		num = ch;
		ball++;
		//for(i = c - 1; i >= c - 8; i--) {
		for(i = c - 1; i >= c - 8; i--) {
			d = num % 2;
			num = num / 2;
			if(d == 1)
				f[i] = '1';
			else
				f[i] = '0';
		}
		size = size + 8;
		//size = size + 7;
		c = c + 8;
		//c = c + 7;
		f = (char *)realloc(f, sizeof(char) * size);
	}
	printf("\nIn Decompress\n%s", f); 
	printf("\nsize of string in decompression is = %d\n", strlen(f));
	//printf("\nNo. of letters = %d\n", ball);
	limit = c - 8;
	printf("\nlimit1 = %d\n", limit);
	c = 0;
	size = 0;
	for(i = 0; i < 4; i++) {
		if(f[i] == '1') 
			size = size + pow(2, 3 - i);
	}
	for(i = 4; i < 12; i++) {
		if(f[i] == '1') 
			count = count + pow(2, 11 - i);
	}
	printf("\ncount = %d\n", count);
	c = 12;
	sum = 0;
	printf("\nAfter decompress\n");
	for(i = 0; i < count; i++) {
		for(j = c; j < c + 8; j++) {
			if(f[j] == '1')
				sum = sum + pow(2, 7 + c - j);
		}
		a[sum].freq = 786;
		//printf("%d\n", sum);
		c = c + 8;
		k = 0;
		a[sum].val = (char *)malloc(sizeof(char) * size);
		while(k < size) {
			a[sum].val[k] = f[c];
			//printf("%c", f[c]);
			c++;
			k++;
		}
		p = getsum(a[sum].val, size);
		a1[p] = sum;
		sum = 0;
	}
	printf("\nHere begins the text");
	while(c <= limit) {
		for(i = 0; i < size; i++)
			g[i] = f[c++];
		num = getsum(g, size);
		ch = a1[num];
		//printf("%c", ch);
		write(fd1, &ch, 1);
	}
	/*while(k < size) {
		ch = convtochar(f, &k);
		write(fd1, &ch, 1);
		//printf("%c", ch);
		//k = k + 8;
		k = k + 7;
	}*/
	close(fd);
	close(fd1);	
}
char *inc(char c[], int n) {
	int i = 0, num, l;
	for(i = n - 1; i >= 0; i--)
		if(c[i] == '1')
			num = num + pow(2, n - i -1);
	num++;
	c = (char *)malloc(sizeof(char) * n);
	for(i = 0; i < n; i++) {
		l = num % 2;
		num = num / 2;
		if(l == 1)
			c[n - 1 - i] = '1';
		else
			c[n - 1 - i] = '0';
	}
	return c;
}
int main() {
	if(argv < 2) {
		errno = EINVAL;
		perror("Please enter file name");
		return errno;
	}
	iden a[256];
	int fd, b, i, count = 0, size = 0, count1, e = 1, j = 0, lets = 0, ball = 0;
	char c, *d, *h[2];
	for(i = 0; i < 256; i++) {
		a[i].freq = 0;
	}
	fd = open(argc[1], O_RDONLY);
	if(fd == -1) {
		errno = EINVAL;
		perror("Unable to open the file");
		return errno;
	}
	while(read(fd, &c, 1)){
		b = c;
		ball++;
		lets++;
		a[b].freq++;
	}
	printf("\nNo. of letters in main = %d\n", ball);
	for(i = 0; i < 256; i++) {
		if(a[i].freq != 0) 
			count++;
	}
	count1 = count;
	count1--;
	while(count1 > 0) {
		count1 = count1 / 2;
		size++;
	}
	d = (char *)malloc(sizeof(char) * size);
	for(i = 0; i < size; i++) {
		d[i] = '0';
	}
	for(i = 0; i < 256; i++) {
		if(a[i].freq != 0) {
			//printf("%d\n", i);
			a[i].val =  (char *)malloc(sizeof(char) * size);
			if(e != 1)
				d = inc(d, size);
			else
				e--;
			strcpy(a[i].val, d);
		}
	}
	compress(a, size, fd, lets, count);
	decompress();		
	return 0;
}  
/*
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
typedef struct iden {
	int freq;
	char *val;
}iden;
int getsum(char *value, int size) {
	int sum = 0, i;
		for(i = 0; i < size; i++) {
			if(value[i] == '1')
				sum = sum + pow(2, size - i - 1);		
		}
	return sum;
}
char convtochar(char *b, int *p) {
	int i, num = 0, c;
	char ch;
	c = *p;
	for(i = c - 1; i >= c - 7; i--)
		if(b[i] == '1')
			num = num + pow(2, c - i- 1);
	ch = num;
	return ch;	
}
void compress(iden *a, int size, int fd, int lets, int count) {
	int size1 = size, char1, j, c = 3, fd1, i, h, size2 = size, val, l, ball = 0, g = 0, rem;
	char ch, *b;
	printf("size = %d\ncount = %d\n", size, count);
	fd1 = open("comp.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	lseek(fd, 0, SEEK_SET);
	size = 4;
	printf("\nIn compression\n");
	b = (char *)malloc(sizeof(char) * size);
	while(size2 > 0) {
		i = size2 % 2;
		g++;
		size2 = size2 / 2;
		if(i == 1)
			b[c--] = '1';
		else
			b[c--] = '0';
	}
	while(c >= 0) {
		b[c--] = '0';
		g++;
	}
	size = size + 7;
	c = 10;
	b = (char *)realloc(b, sizeof(char) * size);
	while(count > 0) {
		i = count % 2;
		g++;
		count = count / 2;
		if(i == 1)
			b[c--] = '1';
		else
			b[c--] = '0';
	}
	while(c >= 4) {
		b[c--] = '0';
		g++;
	}
	c = 17;
	size = size + 7 + size1;
	for(i = 0; i < 256; i++) {
		if(a[i].freq != 0) {
			l = c - 6;
			b = (char *)realloc(b, sizeof(char) * size);
			val = i;
			while(val > 0) {
				g++;
				if(val % 2 == 1) 
					b[c--] = '1';
				else
					b[c--] = '0';
				val = val / 2;
			}
			
			while(c >= l) {
				b[c--] = '0';
				g++;
			}
			c = c + 8;
			for(j = 0; j < size1; j++) {
				b[c++] = a[i].val[j];
				g++;
			}
			size = size + 8 + size1;
			c = c + 6;	
		}
	}
	c = c - 6; //Last element
	size = size - 7;
	while(read(fd, &ch, 1)) {
		ball++;
		char1 = ch;
		if(char1 >= 0) {
			for(j = 0; j < size1; j++) {
				b[c++] = a[char1].val[j];
				g++;
			}
			size = size + size1;
			b = (char *)realloc(b, sizeof(char) * size);
		}
	}
	rem = g % 7;
	rem = 7 - rem;
	g = g + rem;
	size = size + rem;
	b = (char *)realloc(b, sizeof(char) * size);
	printf("size = %d\n", size);
	for(i = 0; i < rem; i++)
		b[c++] = '0';
	c = 7;
	g = g / 7;
	printf("g = %d", g);
	for(i = 0; i < g; i++) {
		ch = convtochar(b, &c);
		write(fd1, &ch, 1);
		c = c + 7;	
	}
	close(fd);
	close(fd1);	
}
void decompress(int fd) {
	iden a[128];
	int i, num, c = 7, d, size = 8, k = 7, fd1, count = 0, sum, j, m, a1[128], p, limit, ball = 0;
	char ch, *f, *g;
	g = (char *)malloc(sizeof(char) * size);
	f = (char *)malloc(sizeof(char) * size);
	fd1 = open("decomp.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	while(read(fd, &ch, 1)) {
		num = ch;
		ball++;
		for(i = c - 1; i >= c - 7; i--) {
			d = num % 2;
			num = num / 2;
			if(d == 1)
				f[i] = '1';
			else
				f[i] = '0';
		}
		size = size + 7;
		c = c + 7;
		f = (char *)realloc(f, sizeof(char) * size);
	}
	limit = c - 8;
	c = 0;
	size = 0;
	for(i = 0; i < 4; i++) {
		if(f[i] == '1') 
			size = size + pow(2, 3 - i);
	}
	for(i = 4; i < 11; i++) {
		if(f[i] == '1') 
			count = count + pow(2, 10 - i);
	}
	c = 11;
	sum = 0;
	for(i = 0; i < count; i++) {
		for(j = c; j < c + 7; j++) {
			if(f[j] == '1')
				sum = sum + pow(2, 6 + c - j);
		}
		a[sum].freq = 786;
		c = c + 7;
		k = 0;
		a[sum].val = (char *)malloc(sizeof(char) * size);
		while(k < size) {
			a[sum].val[k] = f[c];
			c++;
			k++;
		}
		p = getsum(a[sum].val, size);
		a1[p] = sum;
		sum = 0;
	}
	while(c <= limit) {
		for(i = 0; i < size; i++)
			g[i] = f[c++];
		num = getsum(g, size);
		ch = a1[num];
		write(fd1, &ch, 1);
	}
	close(fd);
	close(fd1);	
}
char *inc(char c[], int n) {
	int i = 0, num, l;
	char *b;
	b = (char *)malloc(sizeof(char) * n);
	strcpy(b, c);
	for(i = n - 1; i >= 0; i--)
		if(c[i] == '1')
			num = num + pow(2, n - i -1);
	num++;
	c = (char *)malloc(sizeof(char) * n);
	for(i = 0; i < n; i++) {
		l = num % 2;
		num = num / 2;
		if(l == 1)
			c[n - 1 - i] = '1';
		else
			c[n - 1 - i] = '0';
	}	
	return c;
}
int main(int argv, char *argc[]) {
	if(argv < 3) {
		errno = EINVAL;
		perror("Please enter file name");
		return errno;
	}
	iden a[256];
	int fd, b, i, count = 0, size = 0, count1, e = 1, j = 0, lets = 0, ball = 0, flag = 0;
	char c, *d, *h[2], *f;
	fd = open(argc[1], O_RDONLY);
	if(fd == -1) {
		errno = EINVAL;
		perror("Unable to open the file");
		return errno;
	}
	if(strcmp(argc[2], "c") == 0) {
		for(i = 0; i < 256; i++) {
			a[i].freq = 0;
		}
		while(read(fd, &c, 1)){
			b = c;
			ball++;
			lets++;
			a[b].freq++;
		}
		printf("\nNo. of letters in main = %d\n", ball);
		for(i = 0; i < 256; i++) {
			if(a[i].freq != 0) 
				count++;
		}
		count1 = count;
		count1--;
		while(count1 > 0) {
			count1 = count1 / 2;
			size++;
		}
		d = (char *)malloc(sizeof(char) * size);
		f = (char *)malloc(sizeof(char) * size);
		for(i = 0; i < size; i++) {
			d[i] = '0';
			f[i] = '0';
		}
		for(i = 0; i < 256; i++) {
			if(a[i].freq != 0) {
				a[i].val =  (char *)malloc(sizeof(char) * size);
				if(e != 1) {
					d = inc(d, size);
					if(strcmp(d, f) == 0 && flag == 0) {
						printf("\nFile was not compressed. Please compress again\n");
						close(fd);
						return 0;
					}
					flag = 1;
				}
				else
					e--;
				strcpy(a[i].val, d);
				printf("%d\t%s\n", a[i].freq, a[i].val);
			}
		}
		compress(a, size, fd, lets, count);
	}	
	else if(strcmp(argc[2], "d") == 0)
		decompress(fd);	
	return 0;
}  
	
	
*/	
	
