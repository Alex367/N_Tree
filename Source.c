#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

struct AvlUzel
{
	int value;
	struct AvlUzel *left, *right;
};

struct AvlUzel *avlKoren = 0;

struct AvlUzel* avlRecursiveInsert(struct AvlUzel *akt, struct AvlUzel *n);
struct AvlUzel *avlBalanceStrom(struct AvlUzel *akt);
struct AvlUzel* avlFindRec(int key, struct AvlUzel *akt);
struct AvlUzel * avlRR(struct AvlUzel * rodic);
struct AvlUzel *avlLL(struct AvlUzel *rodic);
struct AvlUzel *avlLR(struct AvlUzel *rodic);
struct AvlUzel *avlRL(struct AvlUzel *rodic);
struct AvlUzel *avlDeleteRec(struct AvlUzel *akt, int key);

void avlDelete(int data);

void avlInsert(int data)
{
	struct AvlUzel *novyElem = (struct AvlUzel*)malloc(sizeof(struct AvlUzel));
	novyElem->left = 0;
	novyElem->right = 0;
	novyElem->value = data;

	if (avlKoren == 0)
		avlKoren = novyElem;
	else
		avlKoren = avlRecursiveInsert(avlKoren, novyElem); 
}

void avlDeleteAll()
{
	avlKoren = 0;
}

struct AvlUzel* avlRecursiveInsert(struct AvlUzel *akt, struct AvlUzel *n)
{
	if (akt == 0)
	{
		akt = n;
		return akt;
	}
	else if (n->value < akt->value)
	{
		akt->left = avlRecursiveInsert(akt->left, n);
		akt = avlBalanceStrom(akt);
	}
	else if (n->value > akt->value)
	{
		akt->right = avlRecursiveInsert(akt->right, n);
		akt = avlBalanceStrom(akt);
	}
	return akt;
}

int avlVyska(struct AvlUzel *akt)
{
	int l, r, m;

	int vyska = 0;
	if (akt != 0)
	{
		l = avlVyska(akt->left);
		r = avlVyska(akt->right);
		if (l >= r)
			m = l;
		else
			m = r;
		vyska = m + 1;
	}
	return vyska;
}

int avlBalanceFactor(struct AvlUzel *akt) 
{
	int l = avlVyska(akt->left);
	int r = avlVyska(akt->right);
	int b_factor = l - r; 
	return b_factor;
}

struct AvlUzel *avlBalanceStrom(struct AvlUzel *akt)
{
	int b_factor = avlBalanceFactor(akt);
	if (b_factor > 1)
	{
		if (avlBalanceFactor(akt->left) > 0)
		{
			akt = avlLL(akt);
		}
		else
		{
			akt = avlLR(akt);
		}
	}
	else if (b_factor < -1)
	{
		if (avlBalanceFactor(akt->right) > 0)
		{
			akt = avlRL(akt);
		}
		else
		{
			akt = avlRR(akt);
		}
	}
	return akt;
}

void avlDelete(int data)
{
	avlDeleteRec(avlKoren, data); 
}

struct AvlUzel *avlFind(int key) 
{
	return avlFindRec(key, avlKoren);
}

struct AvlUzel* avlFindRec(int key, struct AvlUzel *akt)
{
	if (key == akt->value)
		return akt;

	if (key < akt->value)
	{
		if (akt->left != 0)
			return avlFindRec(key, akt->left);
		else
			return 0;
	}
	else
	{
		if (akt->right != 0)
			return avlFindRec(key, akt->right);
		else
			return 0;
	}

}

struct AvlUzel * avlRR(struct AvlUzel * rodic)
{
	struct AvlUzel * pivot = rodic->right;
	rodic->right = pivot->left;
	pivot->left = rodic;
	return pivot;
}

struct AvlUzel *avlLL(struct AvlUzel *rodic)
{
	struct AvlUzel *pivot = rodic->left;
	rodic->left = pivot->right;
	pivot->right = rodic;
	return pivot;
}

struct AvlUzel *avlLR(struct AvlUzel *rodic)
{
	struct AvlUzel *pivot = rodic->left;
	rodic->left = avlRR(pivot);
	return avlLL(rodic);
}

struct AvlUzel *avlRL(struct AvlUzel *rodic)
{
	struct AvlUzel *pivot = rodic->right;
	rodic->right = avlLL(pivot);
	return avlRR(rodic);
}

struct AvlUzel *avlDeleteRec(struct AvlUzel *akt, int key) 
{
	struct AvlUzel *rodic;
	if (akt == 0)
		return 0;
	else
	{
		if (key < akt->value) 	
		{
			akt->left = avlDeleteRec(akt->left, key);
			if (avlBalanceFactor(akt) == -2)
			{
				if (akt->left && avlBalanceFactor(akt->left) <= 0)
				{
					akt = avlRR(akt);
				}
				else
				{
					akt = avlRL(akt);
				}
			}
		}
		else if (key > akt->value) 		
		{
			akt->right = avlDeleteRec(akt->right, key);
			if (avlBalanceFactor(akt) == 2)
			{
				if (akt->right && avlBalanceFactor(akt->right) <= 0)
				{
					akt = avlLL(akt);
				}
				else
				{
					akt = avlLR(akt);
				}
			}
		}
		else 		
		{
			if (akt->right != 0)
			{
				rodic = akt->right; 				
				while (rodic->left != 0)
				{
					rodic = rodic->left;
				}
				akt->value = rodic->value;
				akt->right = avlDeleteRec(akt->right, rodic->value);
				if (avlBalanceFactor(akt) == 2)
				{
					if (avlBalanceFactor(akt->left) <= 0)
						akt = avlLL(akt);
					else
						akt = avlLR(akt);
				}
			}
			else
			{
				return akt->left;
			}
		}
	}
	return akt;
}


void StartAvl()
{
	int i, d;
	struct AvlUzel *uzel;

	for (i = 1; i <= 5; i++)
	{
		printf("Insert %d/5:", i);
		scanf_s("%d", &d);

		avlInsert(d);
	}

	for (i = 1; i <= 2; i++)
	{
		printf("Find %d/2:", i);
		scanf_s("%d", &d);

		uzel = avlFind(d);
		if (uzel == 0)
			printf("Not find\n");
		else
			printf("%d find\n", uzel->value);
	}

	printf("Delete:");
	scanf_s("%d", &d);

	avlDelete(d);

	printf("Find:");
	scanf_s("%d", &d);
	uzel = avlFind(d);
	if (uzel == 0)
		printf("Not find\n");
	else
		printf("%d find\n", uzel->value);
}

const int RED = 1;
const int BLACK = 2;

struct RedBlackUzel
{
	int data;
	int list;
	
	int color;
	struct RedBlackUzel *rodic;
	struct RedBlackUzel *right, *left;
};

struct RedBlackUzel *redBlackKoren;

void deleteFix(struct RedBlackUzel *x);
void deleteUzel(struct RedBlackUzel *z);

void redBlackDelete(int data)
{
	struct RedBlackUzel *n;

	n = redBlackKoren;

	while (n->list == 0)
	{
		if (n->data == data)
		{
			deleteUzel(n);
			return;
		}

		if (data < n->data)
			n = n->left;
		else if (data > n->data)
			n = n->right;
	}
}

struct RedBlackUzel *redBlackFind(int data)
{
	struct RedBlackUzel *n;

	n = redBlackKoren;

	while (n->list == 0)
	{
		if (n->data == data)
			return n;

		if (data < n->data)
			n = n->left;
		else if (data > n->data)
			n = n->right;
	}

	return 0;
}


void redBlackOtaceniLeft(struct RedBlackUzel *x)
{
	struct RedBlackUzel *y = x->right;

	x->right = y->left;
	if (y->left->list == 0)
		y->left->rodic = x;

	if (y->list == 0)
		y->rodic = x->rodic;
	if (x->rodic)
	{
		if (x == x->rodic->left)
			x->rodic->left = y;
		else
			x->rodic->right = y;
	}
	else
	{
		redBlackKoren = y;
	}

	y->left = x;
	if (x->list == 0)
		x->rodic = y;
}

void redBlackOtaceniRight(struct RedBlackUzel *x)
{
	struct RedBlackUzel *y = x->left;

	x->left = y->right;
	if (y->right->list == 0)
		y->right->rodic = x;

	if (y->list == 0)
		y->rodic = x->rodic;
	if (x->rodic)
	{
		if (x == x->rodic->right)
			x->rodic->right = y;
		else
			x->rodic->left = y;
	}
	else
	{
		redBlackKoren = y;
	}

	y->right = x;
	if (x->list == 0)
		x->rodic = y;
}

void insertFix(struct RedBlackUzel *x)
{
	while (x != redBlackKoren && x->rodic->color == RED)
	{
		if (x->rodic == x->rodic->rodic->left)
		{
			struct RedBlackUzel *y = x->rodic->rodic->right; 
			if (y->color == RED)
			{
				x->rodic->color = BLACK;
				y->color = BLACK;
				x->rodic->rodic->color = RED;
				x = x->rodic->rodic;
			}
			else
			{
				if (x == x->rodic->right)
				{
					x = x->rodic;
					redBlackOtaceniLeft(x);
				}

				x->rodic->color = BLACK;
				x->rodic->rodic->color = RED;
				redBlackOtaceniRight(x->rodic->rodic);
			}
		}
		else
		{
			struct RedBlackUzel *y = x->rodic->rodic->left;
			if (y->color == RED)
			{
				x->rodic->color = BLACK;
				y->color = BLACK;
				x->rodic->rodic->color = RED;
				x = x->rodic->rodic;
			}
			else
			{
				if (x == x->rodic->left)   
				{
					x = x->rodic;
					redBlackOtaceniRight(x);
				}
				x->rodic->color = BLACK;
				x->rodic->rodic->color = RED;
				redBlackOtaceniLeft(x->rodic->rodic);
			}
		}
	}
	redBlackKoren->color = BLACK;
}

struct RedBlackUzel *redBlackInsert(int data)
{
	struct RedBlackUzel *akt, *rodic, *x;

	if (redBlackKoren == 0)
	{
		redBlackKoren = (struct RedBlackUzel *)malloc(sizeof(struct RedBlackUzel));
		redBlackKoren->color = BLACK;
		redBlackKoren->data = 0;
		redBlackKoren->rodic = 0;
		redBlackKoren->list = 1;
	}

	akt = redBlackKoren; 
	rodic = 0;
	while (akt->list == 0)
	{
		if (data == akt->data)
			return (akt);
		rodic = akt;
		akt = (data < akt->data) ? akt->left : akt->right;
	}

	x = (struct RedBlackUzel *)malloc(sizeof(struct RedBlackUzel));
	x->data = data;
	x->rodic = rodic;
	x->list = 0;
	x->left = (struct RedBlackUzel *)malloc(sizeof(struct RedBlackUzel));
	x->left->list = 1;
	x->left->left = 0;
	x->left->right = 0;
	x->right = (struct RedBlackUzel *)malloc(sizeof(struct RedBlackUzel));
	x->right->list = 1;
	x->right->left = 0;
	x->right->right = 0;
	x->color = RED;

	if (rodic)
	{
		if (data < rodic->data)
			rodic->left = x;
		else
			rodic->right = x;
	}
	else
	{
		redBlackKoren = x;
	}

	insertFix(x);
	return(x);
}

void deleteFix(struct RedBlackUzel *x)
{
	while (x != redBlackKoren && x->color == BLACK)
	{
		if (x == x->rodic->left)
		{
			struct RedBlackUzel *w = x->rodic->right;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->rodic->color = RED;
				redBlackOtaceniLeft(x->rodic);
				w = x->rodic->right;
			}
			if (w->left->color == BLACK && w->right->color == BLACK)
			{
				w->color = RED;
				x = x->rodic;
			}
			else
			{
				if (w->right->color == BLACK)
				{
					w->left->color = BLACK;
					w->color = RED;
					redBlackOtaceniRight(w);
					w = x->rodic->right;
				}
				w->color = x->rodic->color;
				x->rodic->color = BLACK;
				w->right->color = BLACK;
				redBlackOtaceniLeft(x->rodic);
				x = redBlackKoren;
			}
		}
		else
		{
			struct RedBlackUzel *w = x->rodic->left;
			if (w->color == RED)
			{
				w->color = BLACK;
				x->rodic->color = RED;
				redBlackOtaceniRight(x->rodic);
				w = x->rodic->left;
			}
			if (w->right->color == BLACK && w->left->color == BLACK)
			{
				w->color = RED;
				x = x->rodic;
			}
			else
			{
				if (w->left->color == BLACK)
				{
					w->right->color = BLACK;
					w->color = RED;
					redBlackOtaceniLeft(w);
					w = x->rodic->left;
				}
				w->color = x->rodic->color;
				x->rodic->color = BLACK;
				w->left->color = BLACK;
				redBlackOtaceniRight(x->rodic);
				x = redBlackKoren;
			}
		}
	}
	x->color = BLACK;
}

void deleteUzel(struct RedBlackUzel *z)
{
	struct RedBlackUzel *x, *y;

	if (!z || z->list == 1)
		return;


	if (z->left->list == 1 || z->right->list == 1)
	{
		y = z;
	}
	else
	{
		y = z->right;
		while (y->left->list == 0)
			y = y->left;
	}

	if (y->left->list == 0)
		x = y->left;
	else
		x = y->right;

	x->rodic = y->rodic;
	if (y->rodic)
		if (y == y->rodic->left)
			y->rodic->left = x;
		else
			y->rodic->right = x;
	else
		redBlackKoren = x;

	if (y != z) z->data = y->data;


	if (y->color == BLACK)
		deleteFix(x);

	free(y);
}

void StartRedBlack()
{
	int i, d;
	struct RedBlackUzel *uzel;

	for (i = 1; i <= 6; i++)
	{
		printf("Insert %d/6:", i);
		scanf_s("%i", &d);

		redBlackInsert(d);
	}

	for (i = 1; i <= 2; i++)
	{
		printf("Find %d/2:", i);
		scanf_s("%i", &d);

		uzel = redBlackFind(d);
		if (uzel == 0)
			printf("Not find\n");
		else
			printf("%d find\n", uzel->data);
	}

	printf("Delete:");
	scanf_s("%i", &d);

	redBlackDelete(d);

	printf("Find:");
	scanf_s("%i", &d);
	uzel = redBlackFind(d);
	if (uzel == 0)
		printf("Not find\n");
	else
		printf("%d find\n", uzel->data);
}

int main(int argc, char *argv[])
{
	char vybor;

	printf("Vyberte:\n1) AVL,\n2) Red-Black,\n");
	scanf_s("%c", &vybor);

	if (vybor == '1')
		StartAvl();
	else if (vybor == '2')
		StartRedBlack();

	printf(" ");
	_getch();
	return 0;
}