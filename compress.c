#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<limits.h>
typedef struct iden {
	int freq;
	char *c;
}iden;

typedef struct idem {
	int prod;
	int size;
}idem;

typedef struct sort {
	int freq;
	int ASCII;
}sort;

//ADT STARTS HERE

typedef struct phode {
	int value;
	int ascii;
	char *cred;
	struct phode *upleft;
	struct phode *upright;
	struct phode *next;
	struct phode *prev;
}phode;

void p_init(phode **ph, sort *b, int count) {
	int i;
	phode *ptr1, *ptr, *p;
	ptr1 = (phode *)malloc(sizeof(phode));
	ptr1->value = b[0].freq;
	ptr1->ascii = b[0].ASCII;
	ptr1->next = NULL;
	ptr1->upleft = NULL;
	ptr1->upright = NULL;
	ptr1->prev = NULL;
	p = ptr1;
	*ph = p;
	ptr = ptr1;
	for(i = 1; i < count; i++) {
		ptr1 = (phode *)malloc(sizeof(phode));
		ptr1->value = b[i].freq;
		ptr1->ascii = b[i].ASCII;
		ptr1->next = NULL;
		ptr1->upleft = NULL;
		ptr1->upright = NULL;
		ptr1->prev = ptr;
		ptr->next = ptr1;
		ptr = ptr1;
	}
}

void print(phode *p) {
	phode *ptr, *ptr1;
	int sum = 0;
	ptr = p;
	while(ptr) {
		printf("ascii = %d\tvalue = %d\tcred = %s\n", ptr->ascii, ptr->value, ptr->cred);
		sum = sum + strlen(ptr->cred) * (ptr->value);
		ptr1 = ptr;
		ptr = ptr->next;
	}	
}

phode *getdata(phode **ph, int count) {//Second argument gives number of extra nodes 
	int i, sum = 0, min = INT_MAX, flag = 0, flag1 = 0;
	phode *ptr, *p, *head, *node, *dummyptr, *bottom, *ptrz, *ptr1;
	p = *ph;
	head = p;
	ptr = head;
	for(i = 0; i < count; i++) {
		while(1) {
			sum = ptr->value + (ptr->next)->value;
			if(sum < min)
				min = sum;
			else {
				ptr = dummyptr;
				dummyptr = ptr->prev;
				break;
			}	
			if(ptr->upleft == NULL) 
				break;
			if((ptr->next)->next == NULL) 
				break;
				
			dummyptr = ptr;
			ptr = ptr->next;	
		}
		node = (phode *)malloc(sizeof(phode));
		node->next = (ptr->next)->next;
		if((ptr->next)->next != NULL)
			((ptr->next)->next)->prev = node;
		node->upleft = ptr;
		node->upright = ptr->next;
		node->value = min;
		if(ptr != head) {	
			dummyptr->next = node;
			node->prev = dummyptr;
		}
		if(node->upleft == head)
			head = node;
		ptr = head;
		min = INT_MAX;
	}
	bottom = node;	
	return bottom;				
}

void assign(phode **ph, int flag, char *c, int level) {
	phode *bottom;
	bottom = *ph;
	if(bottom == NULL)
		return;
	if(flag == 1) {
		if(level >= 0)
			c[level] = '1';
	} 
	if(flag == 0) {
		if(level >= 0)
			c[level] = '0';
	}
	if(level >= 0) {
		bottom->cred = (char *)malloc(sizeof(char) * (level + 1));
		strcpy(bottom->cred, c);
	}
	level++;
	c = (char *)realloc(c, sizeof(char) * (level + 1));
	assign(&(bottom->upright), 1, c, level);
	assign(&(bottom->upleft), 0, c, level);
	
}	
		
//ADT ENDS HERE

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

void quicksort(sort *number,int first,int last){
	int i, j, pivot, temp, temp1;	
   	if(first < last) {
      		pivot = first;
      		i = first;
      		j = last;
      		while(i<j){
         		while(number[i].freq<=number[pivot].freq&&i<last)
         		   	i++;
         		while(number[j].freq>number[pivot].freq)
            			j--;
		 	if(i<j){
		    	temp=number[i].freq;
		    	temp1=number[i].ASCII;
		   	number[i].freq=number[j].freq;
		    	number[i].ASCII=number[j].ASCII;
		    	number[j].freq=temp;
		    	number[j].ASCII=temp1;
         		}
      		}
		temp=number[pivot].freq;
		temp1=number[pivot].ASCII;
		number[pivot].freq=number[j].freq;
		number[pivot].ASCII=number[j].ASCII;
		number[j].freq=temp;
		number[j].ASCII=temp1;
		quicksort(number,first,j-1);
		quicksort(number,j+1,last);
	}
}

void compress(int fd) {
	iden a[128];
	sort *b;
	phode *p, *ptr, *bottom;
	p = NULL;
	int i, num, count = 0, random = 2, level = -1, size = 7, n, c1 = 0, count1, max = 0, size1 = 0, j, n1, rem, fd1, g, size2, max1;
	fd1 = open("comp.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	char ch, *c, *d;
	d = (char *)malloc(sizeof(char) * size);
	c = (char *)malloc(sizeof(char));
	for(i = 0; i < 128; i++) 
		a[i].freq = 0;
	while(read(fd, &ch, 1)) {
		num = ch;
		a[num].freq++;
	}
	b = (sort *)malloc(sizeof(sort));
	for(i = 0; i < 128; i++) {
		if(a[i].freq > 0) {
			count++;
			b = (sort *)realloc(b, sizeof(sort) * count);
			b[count - 1].freq = a[i].freq;
			b[count - 1].ASCII = i;
			ch = i; 
		}
	}
	quicksort(b, 0, count - 1);
	p_init(&p, b, count);
	bottom = getdata(&p, count - 1);//This function will construct the tree and allot the characters to each phode
	assign(&bottom, random, c, level);
	ptr = p;
	while(ptr) {//Assigning the creds back to iden
		num = ptr->ascii;
		n = strlen(ptr->cred);
		if(n > max)
			max = n;
		a[num].c = (char *)malloc(sizeof(char) * n);
		strcpy(a[num].c, ptr->cred);
		ptr = ptr->next;
	}
	lseek(fd, 0, SEEK_SET);
	count1 = count;
	for(i = 0; i < 7; i++) {
		if(count % 2 == 1)
			d[6 - i] = '1';
		else
			d[6 - i] = '0';
		count = count / 2;
	}
	size = size + 5;
	d = (char *)realloc(d, sizeof(char) * size);
	max1 = max;
	while(max > 0) { //Evaluates the length needed to store the maximum size of cred
		size1++;
		max = max / 2;
	}
	size2 = size1;
	for(i = 0; i < 5; i++) {
		if(size2 % 2 == 1)
			d[11 - i] = '1';
		else
			d[11 - i] = '0';
		size2 = size2 / 2;
	}
	c1 = 12;
	size = size + 6;
	d = (char *)realloc(d, sizeof(char) * size);
	for(i = 0; i < 6; i++) {
		if(max1 % 2 == 1)
			d[17 - i] = '1';
		else
			d[17 - i] = '0';
		max1 = max1 / 2;
	}
	c1 = 18;
	for(i = 0; i < 128; i++) {
		if(a[i].freq != 0) {
			n1 = strlen(a[i].c);
			size = size + 7 + size1 + n1; //[ 7 : ASCII][size1 : storing length][n : length]
			d = (char *)realloc(d, sizeof(char) * size);
			n = i;
			for(j = c1 + 6; j >= c1; j--) {
				if(n % 2 == 0)
					d[j] = '0';
				else
					d[j] = '1';
				n = n / 2;
			}
			c1 = c1 + 7;
			n = n1;
			for(j = c1 + size1 - 1; j >= c1; j--) {
				if(n % 2 == 0)
					d[j] = '0';
				else
					d[j] = '1';
				n = n / 2;
			}
			c1 = c1 + size1;
			for(j = 0; j < n1; j++)
				d[c1++] = a[i].c[j];
		}
	}
	while(read(fd, &ch, 1)) {
		num = ch;
		n = strlen(a[num].c);
		size = size + n;
		d = (char *)realloc(d, sizeof(char) * size);
		for(i = 0; i < n; i++)
			d[c1++] = a[num].c[i];
	}						
	rem = c1 % 7;
	rem = 7 - rem;
	size = size + rem;
	c1 = c1 + rem;
	d = (char *)realloc(d, sizeof(char) * size); 
	for(i = 0; i < rem; i++)
		d[c1++] = '0';
	g = c1 / 7;
	c1 = 7;
	for(i = 0; i < g; i++) {
		ch = convtochar(d, &c1);
		write(fd1, &ch, 1);
		c1 = c1 + 7;	
	}
	close(fd);
	close(fd1);		
}
	
void decompress(int fd) {
	char *f, ch;
	int c = 7, num, i, size = 8, d, sum = 0, count = 0, ascii = 0, eff, sum1 = 0, n, flag = 0, j, max = 0, fd1, **a, l;
	fd1 = open("decomp.txt", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
	f = (char *)malloc(sizeof(char));
	while(read(fd, &ch, 1)) {
		f = (char *)realloc(f, sizeof(char) * size);
		num = ch;
		for(i = 0; i < 7; i++) {
			if(num % 2 == 1)
				f[c - 1 - i] = '1';
			else
				f[c - 1 - i] = '0';
			num = num / 2;
		}
		c = c + 7;
		size = size + 8;
	}
	c = 7;
	size = 0;
	for(i = 0; i < 7; i++) {
		if(f[i] == '1')
			size = size + pow(2, c - i - 1);
	}
	n = strlen(f);
	for(i = 7; i < 12; i++) {
		if(f[i] == '1')
			count = count + pow(2, 11 - i);
	}
	c = 12;
	for(i = c; i < 18; i++) {
		if(f[i] == '1')
			max = 2 * max + 1;
		else
			max = max * 2;
	}
	a = (int **)malloc(sizeof(int *) * max);
	for(i = 0; i <= max; i++) {
	l = pow(2, max - 1);
		a[i] = (int *)malloc(sizeof(int) * l);
		for(j = 0; j <= l; j++)
			a[i][j] = -1;
	}
	c = 18;
	for(i = 0; i < size; i++) {
		for(j = c; j < c + 7; j++) {
			if(f[j] == '1')
				ascii = ascii + pow(2, c + 6 - j);
		}
		c = c + 7;
		for(j = c; j < c + count; j++) {
			if(f[j] == '1')
				sum = sum + pow(2, c + count - 1 - j);
		}
		c = c + count;
		num = 0;
		for(j = 0; j < sum; j++) {
			if(f[c] == '1')
				sum1 = 2 * sum1 + 1;
			else
				sum1 = 2 * sum1;
			c++;
		}
		a[sum - 1][sum1] = ascii;
		ascii = 0;
		sum = 0;
		sum1 = 0;
	}
	num = 0;
	size = 0;
	while(c < n) {
		if(f[c++] == '1') {
			num = 2 * num + 1;
			size++;
		}
		else {
			num = num * 2;
			size++;
		}
		if((a[size - 1][num] >= 0)) {
			ascii = a[size - 1][num];
			ch = ascii;
			num = 0;
			size = 0;
			write(fd1, &ch, 1);
		}
	}
	close(fd);
	close(fd1);	
}

int main(int argv, char *argc[]) {
	if(argv < 3) {
		errno = EINVAL;
		perror("Please enter file name");
		return errno;
	}
	int fd;
	fd = open(argc[1], O_RDONLY);
	if(fd == -1) {
		errno = EINVAL;
		perror("Unable to open the file");
		return errno;
	}
	if(strcmp(argc[2], "c") == 0)
		compress(fd);
	else
		decompress(fd);
	return 0;
}
	

