#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct Node 
{
	int num;
	long int spot;
	struct Node * next;
	struct Node * prev;
};
struct Node * newNode (int n, long int t, struct Node * n1, struct Node * n2) 
{

	struct Node * ptr = (struct Node *)malloc(sizeof(struct Node));
	if (!ptr)
	{
		return NULL;
	}
	ptr->num = n;
	ptr->spot = t;
	ptr->next = n1;
	ptr->prev = n2;

	return ptr;
}
void freeN(struct Node * p) 
{

	struct Node * prev = p;
	while(p!=NULL) 
	{
		prev = p;
		p=p->next;
	}
	p = prev;
	prev = p->prev;

	while (p!=NULL) 
	{
		free(p);
		p = prev;
		if (p!=NULL) 
		{
			prev = p->prev;
		}
	}

}
void freeC(struct Node ** cache, int size) 
{
	if (cache==NULL)
	{
		return;
	} 
	for (int i=0; i<size; i++) 
	{
		freeN(cache[i]);
	}
	free(cache);
}
int logbTw (int n) 
{
	int i=0;
	while (n>1) 
	{
		if (n%2==1) 
		{
			return -1;
		}
		n = n >> 1;
		i++;
	}
	return i;
}
int checker (struct Node ** cache, long int addr, int pol, int sizB, int sizA, int l, int pr) 
{
	if (cache == NULL) 
	{
		return 1;
	}
	int r=1;

	long int bOS = addr%sizB;
	long int c = (addr - bOS)%(sizB*sizA);
	long int i = c/sizB;
	long int spot = addr-bOS-(i*sizB*sizA);

	int n = 0;
	int x = 0;
	struct Node * s = cache[i];
	struct Node * p = cache[i];
	struct Node * prev = NULL;
	struct Node * g;


	while (p!=NULL) 
	{
		if (x!=1) 
		{
			if (p->spot == spot) 
			{
				if (pol==1) 
				{
					return 0;
				}
				else if (pr!=1) 
				{
					prev->next = p->next;
					if (p->next!=NULL) 
					{
						struct Node * x = p->next;
						x->prev = prev;
					}
					g = p;
					x=1;
					g->next = NULL;
				} 
				else 
				{
					return 0;
				}
			}
		}
		if (p->num>n) 
		{
			n=p->num;
		}
		prev = p;
		p = p->next;
	}
	if (x==1 && pr!=1) 
	{
		prev->next = g;
		g->prev = prev;
		g->next = NULL;
		g->num = n;
		return 0;
	}
	n++;

	struct Node * temp = newNode(n, spot, NULL, prev);
	prev->next = temp;

	if (n>l) 
	{
		g = s->next;
		p = g->next;
		s->next = p;
		p->prev = s;
		free(g);
	}
	return r;

}
int tS (char * str) 
{
	
	int r =0;
	int i;
	for ( i=0; str[i]!='\0'; i++) 
	{}
	for (int j=0; j<i; j++)
		{
			r = r * 10;
			r +=  (int)(str[j]) - 48;
		}
	return r;
}
int main (int argc, char ** argv) 
{
	if (argc!=6) 
	{
		printf("Error\n");
		exit(0);
	} 
	int sizC=0;
	int pol; 
	int sizB=0;
	int powB = 0;
	int nS;
	int l;

	int size;
	for (size=0; argv[5][size]!='\0'; size++) {
	}
	size++;

	char * filename = (char *)malloc(sizeof(char *) * size);
	strcpy(filename, argv[5]);

	FILE * fp = fopen(filename, "r");

	if (!fp)
	{
		return 1;
	}
	sizC = tS(argv[1]);
	int powC = logbTw(sizC);
	if (powC == -1)
	{
		 return 1;
	}
	sizB = tS(argv[4]);
	powB = logbTw(sizB);
	if (powB == -1) 
	{
		 return 1;
	}

	if (strcmp("fifo", argv[3])==0) 
	{
		pol = 1;
	} 
	else 
	{
		return 1;
	}
    int make;
	int i; 
	for (i=0; argv[2][i]!='\0'; i++) {}
	if (i==6) 
	{
		nS = sizC/sizB;
		make=0;
		l = 1;
	}
	else if (i==5) 
	{
		nS = 1;
		make=1;
		l = sizC/sizB;
	} 
	else 
	{
		int s = i-6;
		char * y = (char *)malloc(sizeof(char) * s+1);
		int j;
		for ( j=6; argv[2][j]!='\0'; j++) 
		{
			y[j-6] = argv[2][j];
		}
		y[j-6] = '\0';
		make = tS(y);
		free(y);
		if (logbTw(make)==-1)
		{
			return -1;
		}
		nS = sizC/(sizB * make);
		l = make;
	}
	struct Node ** reg = (struct Node **) malloc (sizeof(struct Node *) * nS);
	struct Node ** prefet = (struct Node **) malloc (sizeof(struct Node *) * nS);
	if (!prefet || !reg)
	{
		return 1;
	}
	for (i=0; i<nS; i++) 
	{

		struct Node * x = newNode(0, -1, NULL, NULL);
		struct Node * y = newNode(0, -1, NULL, NULL);
		reg[i] = x;
		prefet[i] = y;
	}
	int rHits=0; 
	int rMisses =0;
	int rReads =0;
	int rWrites = 0;
	int pHits = 0;
	int pMisses =0;
	int pReads = 0;
	int pWrites =0;
	char * g = (char *) malloc (sizeof(char)* 100);
	char instr = 'a';
	long int addr = 0;

	while (fscanf(fp, "%s ", g)==1) 
	{
		if (strcmp("#eof", g)==0)
		{
			break;
		} 
		if (strcmp("#end", g)==0)
		{
			break;
		}
		int iV = fscanf(fp, "%c %lx", &instr, &addr);
		if (iV!=2) 
		{
			return 1;
		} 
	
		if (checker(reg, addr, pol, sizB, nS, l, 0)==0) 
		{
			rHits ++;
			if (instr == 'W') 
			{
				rWrites ++;
			}
		}
		else 
		{
			rMisses ++;
			if (instr == 'R') 
			{
				rReads ++;
			}
			else
			{
				rReads ++;
				rWrites ++;
			}
		}
		if (checker(prefet, addr, pol, sizB, nS, l, 0)==0) 
		{
			pHits++;
			if (instr == 'W')
			{
				pWrites ++;
			}
		}
		else 
		{
			pMisses++;
			if (instr == 'R') 
			{
				pReads ++;
			}
			else 
			{
				pReads++;
				pWrites ++;
			}
			if (checker(prefet, addr+sizB, pol, sizB, nS, l, 1)==0) 
			{}
			else 
			{
				pReads ++;
			}
		}

	}

	printf("Prefetch 0\n");
	printf("Memory reads: %d\n", rReads);
	printf("Memory writes: %d\n", rWrites);
	printf("Cache hits: %d\n", rHits);
	printf("Cache misses: %d\n", rMisses);
	printf("Prefetch 1\n");
	printf("Memory reads: %d\n", pReads);
	printf("Memory writes: %d\n", pWrites);
	printf("Cache hits: %d\n", pHits);
	printf("Cache misses: %d\n", pMisses);
	

	
	freeC(reg, nS);
	freeC(prefet, nS);
	free(filename);
	free(g);
	return 0;
}