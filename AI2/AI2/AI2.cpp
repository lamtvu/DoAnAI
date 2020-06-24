#include <iostream>
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<random>
#include<time.h>

using namespace std;
int lengthInput;

//bat buoc
enum Actions
{
	AC,
	AB,
	BB,
	Ex,
	Strart
};
typedef struct NodeString
{
	char letter[100];
	Actions action;
	int value;
	int depth;
	NodeString* parent;
	NodeString* nextNode;
} Node;

typedef struct NodeList {
	unsigned int nodeCount;
	Node* head;
	Node* tail;
};

#pragma region functions
char* NhapChuoi();
NodeString* DFS_search(NodeList* queueNode);
void Add(NodeList* list, Node* node);
void Init_list(NodeList* list);
NodeList* GetSuccessor(NodeString* current);
bool IsGoal(NodeString* node);
char* ChangeLetter(char str[], int i, Actions action);
char* RemoveAtString(int at, char str[]);
int isLegalAction(int i, NodeString* current);
void Add(NodeList* list, Node* node);
Node* FIFO_pop(NodeList* list);
void LIFO_add(NodeList* list, Node* node);
Node* LIFO_pop(NodeList* list);
NodeString* BFS_search(NodeList* queueNode);
NodeString* BFS(NodeString* current);
#pragma endregion

int heuristic(Node* node)// ưu tiên hành động
{
	switch (node->action)
	{
	case Ex:
		return 0;
	case AC:
		return 1;
	case BB:
		return 2;
	case AB:
		return 3;
	default:
		break;
	}
}
int heuristic2(Node* node)// ưu tiên chuỗi chứa nhiều phần tử biến đổi thành E được 
{
	int u = 0;
	for (int i = 0; i < strlen(node->letter); i++)
	{
		if (isLegalAction(i, node) == 0 || isLegalAction(i, node) == 3 || isLegalAction(i, node) == 2)
		{
			u++;
		}
	}
	return strlen(node->letter) - u;
	return 0;
}
int heuristic3(Node* node)// ưu tiên chuỗi ngắn
{
	return strlen(node->letter) * 4;
}
Node* MinCost(NodeList* list)
{
	Node* temp = list->head;
	Node* min = temp;
	while (temp->nextNode != NULL)
	{
		if (min->value > temp->value)
		{
			min = temp;
		}
		temp = temp->nextNode;
	}

	if (min == list->head)
	{
		list->head = list->head->nextNode;
		return min;
	}
	temp = list->head;
	while (temp->nextNode != NULL)
	{
		if (temp->nextNode == min)
		{
			temp->nextNode = min->nextNode;
			return min;
		}
		temp = temp->nextNode;
	}

}
int f(Node* node)
{
	return node->depth;
}
NodeList* GetSuccessor(NodeString* current)
{
	NodeList* list = new NodeList();
	Init_list(list);
	int n = strlen(current->letter);
	for (int i = 0; i < n - 1; i++)
	{
		if (isLegalAction(i, current) != -1)
		{
			NodeString* node = new NodeString();
			node->parent = current;

			node->action = Actions(isLegalAction(i, current));
			char strTemp[255];
			strcpy_s(strTemp, current->letter);
			strcpy_s(node->letter, ChangeLetter(strTemp, i, node->action));
			node->nextNode = NULL;
			node->depth = current->depth + 1;
			node->value = heuristic(node) + heuristic2(node) + heuristic3(node);
			//node->value= heuristic(node) + heuristic4(node);
			Add(list, node);
		}
	}
	return list;
}
bool IsGoal(NodeString* node)
{
	if (strlen(node->letter) == 1 && node->letter[0] == 'E')
	{
		return true;
	}
	return false;
}
char* ChangeLetter(char str[], int i, Actions action)
{
	char* strTemp = str;
	switch (action)
	{
	case AC:
		strTemp[i] = 'E';
		strTemp = RemoveAtString(i + 1, strTemp);
		return strTemp;
	case AB:
		strTemp[i] = 'B';
		strTemp[i + 1] = 'C';
		return strTemp;
	case BB:
		strTemp[i] = 'E';
		strTemp = RemoveAtString(i + 1, strTemp);
		return strTemp;
	case Ex:
		strTemp = RemoveAtString(i, strTemp);
		return strTemp;
	default:
		break;
	}
	return strTemp;
}
char* RemoveAtString(int at, char str[])
{

	char* strTemp = str;
	int n = strlen(str);
	for (int i = at; i < n; i++)
	{
		strTemp[i] = strTemp[i + 1];
	}
	return strTemp;
}
int isLegalAction(int i, NodeString* current)
{
	switch (current->letter[i])
	{
	case'A':
		if (current->letter[i + 1] == 'C')
		{

			return 0;
		}
		if (current->letter[i + 1] == 'B')
		{
			return 1;
		}
		break;
	case 'B':
		if (current->letter[i + 1] == 'B')
		{
			return 2;
		}
		break;
	case 'E':
		return 3;
		break;
	default:
		break;
	}
	return -1;
}
void Add(NodeList* list, Node* node) {
	node->nextNode = NULL;

	if (list->head == NULL)
	{
		list->head = node;
	}
	else
	{
		Node* nodeTemp = list->head;
		while (nodeTemp->nextNode != NULL)
		{
			nodeTemp = nodeTemp->nextNode;
		}
		nodeTemp->nextNode = node;

	}
}
Node* FIFO_pop(NodeList* list) {


	Node* nodeTemp = list->head;
	list->head = list->head->nextNode;
	return nodeTemp;
}
void LIFO_add(NodeList* list, Node* node)
{
	node->nextNode = NULL;

	if (list->head == NULL)
		list->head = node;
	else
	{
		node->nextNode = list->head;
		list->head = node;
	}
}
Node* LIFO_pop(NodeList* list)
{
	Node* nodeTemp = list->head;
	list->head = list->head->nextNode;
	return nodeTemp;
}
bool checkExist(NodeList* list, Node* node)
{
	Node* nodeTemp = list->head;

	while (nodeTemp != NULL)
	{
		if (strcmp(nodeTemp->letter, node->letter) == 0)
			return false;
		nodeTemp = nodeTemp->nextNode;
	}
	return false;
}
void Solution(Node* node) {
	//NodeList* stack = (NodeList*)malloc(sizeof(NodeList));
	NodeList* stack = new NodeList();
	Init_list(stack);
	if (node == NULL)
	{
		cout << "no solution" << endl;
		return;
	}
	cout << "actions = " << node->depth << endl;
	while (node != NULL)
	{
		LIFO_add(stack, node);
		node = node->parent;
	}
	cout << "init letter: ";
	while (stack->head != NULL)
	{
		Node* nodeTemp = LIFO_pop(stack);
		switch (nodeTemp->action)
		{
		case AB:
			cout << "---------" << endl;
			printf("Action: AB->BC\n");
			cout << "new letter:";
			break;
		case AC:
			cout << "---------" << endl;
			printf("Action: AC->E\n");
			cout << "new letter:";
			break;
		case BB:
			cout << "---------" << endl;
			printf("Action: BB->E\n");
			cout << "new letter:";
			break;
		case Ex:
			cout << "---------" << endl;
			printf("Action: Ex->x\n");
			cout << "new letter:";
			break;
		default:
			break;
		}
		cout << nodeTemp->letter << endl;
	}

	return;
}
void solution1(Node* node, double DFS, double BFS, double A) {
	int n = 51 - strlen(node->letter);
	cout << "| " << node->letter;
	for (int i = 0; i < n - 1; i++)
	{
		cout << " ";
	}
	if (DFS == -1)
	{
		cout << "| no solution ";
	}
	else
	{
		printf_s("| %.9f ", DFS);

	}
	if (BFS == -1)
	{
		cout << "| no solution ";
	}
	else
	{
		printf_s("| %.9f ", BFS);


	}
	if (A == -1)
	{
		cout << "| no solution |";
	}
	else
	{
		printf_s("| %.9f |", A);

	}
	cout << endl;
	cout << "-----------------------------------------------------------------------------------------------\n";


}
void Init_list(NodeList* list)
{
	list->head = NULL;
}
int random(int min, int max)
{
	random_device device;
	mt19937 generator(device());
	uniform_real_distribution<float> randomColl(min, max);
	int rd = randomColl(generator);
	return rd;
}

#pragma region Search
NodeString* A_search(NodeList* queueNode)
{
	NodeList* daDi = new NodeList();
	Init_list(daDi);
	int step = 0;
	while (queueNode->head != NULL && step < 10000)
	{
		step++;
		NodeString* nodeTemp = MinCost(queueNode);
		Node* temp = new Node();
		strcpy_s(temp->letter, nodeTemp->letter);
		temp->parent = nodeTemp->parent;
		temp->nextNode = NULL;
		temp->action = nodeTemp->action;
		temp->depth = nodeTemp->depth;
		temp->value = nodeTemp->value;
		Add(daDi, temp);
		if (IsGoal(nodeTemp))
		{
			return nodeTemp;
		}
		NodeList* successor = GetSuccessor(nodeTemp);//con cua nodetemp
		Node* node = successor->head;
		while (node != NULL)
		{
			if (IsGoal(successor->head))
			{
				return node;
			}
			if (checkExist(queueNode, node) == false && checkExist(daDi, node) == false)
			{
				Node* nodeNew = new Node();
				strcpy_s(nodeNew->letter, node->letter);
				nodeNew->parent = node->parent;
				nodeNew->nextNode = NULL;
				nodeNew->action = node->action;
				nodeNew->depth = node->depth;
				nodeNew->value = node->value;
				Add(queueNode, nodeNew);
			}
			node = node->nextNode;
		}
	}
	return NULL;
}
NodeString* ASearch(NodeString* current)
{

	NodeList* queue = new NodeList();
	Init_list(queue);
	Node* temp = new Node();
	strcpy_s(temp->letter, current->letter);
	temp->parent = current->parent;
	temp->nextNode = NULL;
	temp->action = current->action;
	temp->depth = current->depth;
	temp->value = current->value;
	Add(queue, temp);
	return A_search(queue);
}
NodeString* BFS(NodeString* current)
{

	NodeList* queue = new NodeList();
	Init_list(queue);
	Node* temp = new Node();
	strcpy_s(temp->letter, current->letter);
	temp->parent = current->parent;
	temp->nextNode = NULL;
	temp->action = current->action;
	temp->depth = current->depth;
	temp->value = current->value;
	Add(queue, temp);
	return BFS_search(queue);
}
NodeString* BFS_search(NodeList* queueNode)
{
	NodeList* daDi = new NodeList();
	Init_list(daDi);
	int step = 0;
	while (queueNode->head != NULL && step < 10000)
	{
		step++;
		NodeString* nodeTemp = FIFO_pop(queueNode);
		Node* temp = new Node();
		strcpy_s(temp->letter, nodeTemp->letter);
		temp->parent = nodeTemp->parent;
		temp->nextNode = NULL;
		temp->action = nodeTemp->action;
		temp->depth = nodeTemp->depth;
		temp->value = nodeTemp->value;
		Add(daDi, temp);

		if (IsGoal(nodeTemp))
		{
			return nodeTemp;
		}
		NodeList* successor = GetSuccessor(nodeTemp);
		Node* node = successor->head;
		while (node != NULL)
		{
			if (IsGoal(successor->head))
			{
				return node;
			}
			if (checkExist(queueNode, node) == false && checkExist(daDi, node) == false)
			{
				Node* nodeNew = new Node();
				strcpy_s(nodeNew->letter, node->letter);
				nodeNew->parent = node->parent;
				nodeNew->nextNode = NULL;
				nodeNew->action = node->action;
				nodeNew->depth = node->depth;
				nodeNew->value = node->value;
				Add(queueNode, nodeNew);
			}
			node = node->nextNode;


		}
	}
	return NULL;
}
NodeString* DFS(NodeString* current)
{

	NodeList* queue = new NodeList();
	Init_list(queue);
	Node* temp = new Node();
	strcpy_s(temp->letter, current->letter);
	temp->parent = current->parent;
	temp->nextNode = NULL;
	temp->action = current->action;
	temp->depth = current->depth;
	temp->value = current->value;
	Add(queue, temp);
	return DFS_search(queue);
}
NodeString* DFS_search(NodeList* queueNode)
{
	NodeList* daDi = new NodeList();
	Init_list(daDi);
	int step = 0;
	while (queueNode->head != NULL && step < 10000)
	{
		step++;
		NodeString* nodeTemp = LIFO_pop(queueNode);
		Node* temp = new Node();
		strcpy_s(temp->letter, nodeTemp->letter);
		temp->parent = nodeTemp->parent;
		temp->nextNode = NULL;
		temp->action = nodeTemp->action;
		temp->depth = nodeTemp->depth;
		temp->value = nodeTemp->value;
		Add(daDi, temp);

		if (IsGoal(nodeTemp))
		{
			return nodeTemp;
		}
		NodeList* successor = GetSuccessor(nodeTemp);
		Node* node = successor->head;
		while (node != NULL)
		{
			if (IsGoal(successor->head))
			{
				return node;
			}
			if (checkExist(queueNode, node) == false && checkExist(daDi, node) == false)
			{
				Node* nodeNew = new Node();
				strcpy_s(nodeNew->letter, node->letter);
				nodeNew->parent = node->parent;
				nodeNew->nextNode = NULL;
				nodeNew->action = node->action;
				nodeNew->depth = node->depth;
				nodeNew->value = node->value;
				LIFO_add(queueNode, nodeNew);
			}
			node = node->nextNode;


		}
	}
	return NULL;
}
#pragma endregion
void Menu()
{
	cout << "            ******************************************************" << endl;
	cout << "            ***************** BAI TOAN BIEN DOI CHUOI ************" << endl;
	cout << "            ******************************************************" << endl;
	cout << "            *                                                    *" << endl;
	cout << "            * Menu:                                              *" << endl;
	cout << "            *                                                    *" << endl;
	cout << "            * <0> Exit                                           *" << endl;
	cout << "            *                                                    *" << endl;
	cout << "            * <1> Tim kiem bang thuat toan DFS                   *" << endl;
	cout << "            *                                                    *" << endl;
	cout << "            * <2> Tim kiem bang thuat toan BFS                   *" << endl;
	cout << "            *                                                    *" << endl;
	cout << "            * <3> Tim kiem bang thuat toan A*                    *" << endl;
	cout << "            *                                                    *" << endl;
	cout << "            * <4> So sanh thuat toan bang cac chuoi mac dinh     *" << endl;
	cout << "            *                                                    *" << endl;
	cout << "            * <5> So sanh thuat toan bang chuoi nhap vao         *" << endl;
	cout << "            ******************************************************\n" << endl;

}
NodeString* NhapNode()
{
	NodeString* node = new NodeString();
	char* temp = NhapChuoi();
	int n = strlen(temp);
	for (int i = 0; i < n; i++)
	{
		node->letter[i] = temp[i];
	}
	node->action = Actions::Strart;
	node->depth = 0;
	return node;
}
int KiemTraChuoi(char s[])
{
	if (strlen(s) == 0 || strlen(s) > 100)
	{
		return -1;
	}
	for (int i = 0; i < strlen(s); i++)
	{
		if (s[i] != 'A' && s[i] != 'B' && s[i] != 'C' && s[i] != 'E')
		{
			return -2;
		}
	}
	return 0;
}
char* NhapChuoi()
{
	char s[100];

	do
	{
		cout << "Nhap chuoi: ";
		gets_s(s);
		//system("cls");
		if (KiemTraChuoi(s) == -1)
		{
			cout << "Error: do dai chuoi phai lon hon 0 va nho hon 100" << endl;
		}
		if (KiemTraChuoi(s) == -2)
		{
			cout << "Error: chuoi chi chua cac phan tu {'A','B','C','E'}" << endl;
		}

	} while (KiemTraChuoi(s) != 0);
	return s;
}
int SelectMenu()
{
	int i = -1;
	do
	{
		cout << endl;
		cout << "Chon muc: ";
		scanf_s("%d", &i);
		cin.get();

		if (i != 0 && i != 1 && i != 2 && i != 3 && i != 4 && i != 5)
		{
			cout << "i phai thuoc { 0 , 1 , 2 , 3 , 4 , 5}";
		}
	} while (i != 0 && i != 1 && i != 2 && i != 3 && i != 4 && i != 5);
	//cin.get();
	return i;
}
Node* NodeRandom(int n)
{

	Node* node = new Node();
	int l = random(n, n + 1);
	for (int i = 0; i < l; i++)
	{
		char temp;
		do
		{
			temp = random(65, 70);
		} while (temp == 68);
		node->letter[i] = temp;
	}
	node->action = Actions::Strart;
	node->depth = 0;
	return node;
}
Node* NewNode(char s[])
{
	Node* node = new Node();
	strcpy_s(node->letter, s);
	node->action = Actions::Strart;
	node->depth = 0;
	return node;
}
int main()
{
	char s[100];
	NodeList* listInput = new NodeList();
	Init_list(listInput);
#pragma region initListInput
	strcpy_s(s, "AABABBAABABBEBCEBC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABACAABABBEBCACACAEEACC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "AEABABEABBAABABBEBCEEBBECC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABABABBBEABAABABBEBCBEC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABAABABBEBCAABABBEBC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABAABABBAABABBEBCAABABBEBCEBCAC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ACBAEAABABBEBCCBAC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABACACE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ACACABAABABEBACACABABEE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABABABAABABECACABABACACABABEEACACACEACE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "AABABABEABBAABABBEBCEBC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABACAABABBEBCACACAEEACC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "AEABAABABEBEABBAABABBEBCEEBBECC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABABABABEABBBEABAABABBEBCBEC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABAABABABABEBEBCAABABBEBC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABAABABBAABABBEBCAABABBEBCEBCAC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ACBABABEAEAABABBEBCCBAC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABAABABECACE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ACACABABACAABABECABABEE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABABABACACABABACACABABEEACACACEACE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "AABABBAABABBEBCEBC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABACAABABBEBCACACAEEACABABEC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "AEABABEABBAABABBEBCEEBBECABABEC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABABABBBEABAABABBEBCBEC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABABABABEACABEE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABAABABBEBCABABEAABABBEBC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABAABABBAABABBEBABABECAABABBEBCEBCAC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ACBAEAABABBEBCCBAABABEC");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ABABACACE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "ACACABABACACABABEABABEE");
	LIFO_add(listInput, NewNode(s));
	strcpy_s(s, "EACACABABABABACACEACACEABABACACABABEABABEE");
	LIFO_add(listInput, NewNode(s));
#pragma endregion


	Menu();

	NodeString* t;
	clock_t begin, end;
	Node* temp;
	double timeDFS, timeBFS, timeA, totalDFS = 0, totalBFS = 0, totalA = 0, avgDFS, avgBFS, avgA;

	do
	{
		switch (SelectMenu())
		{
		case 0:
			exit(0);
			break;
		case 1:
			cout << "Tim kiem bang DFS:" << endl;
			t = DFS(NhapNode());
			system("cls");
			Menu();
			Solution(t);
			break;
		case 2:
			cout << "Tim kiem bang BFS:" << endl;
			t = BFS(NhapNode());
			system("cls");
			Menu();
			Solution(t);
			break;
		case 3:
			cout << "Tim kiem bang A*:" << endl;
			t = ASearch(NhapNode());
			system("cls");
			Menu();
			Solution(t);
			break;
		case 4:
			system("cls");
			Menu();

			cout << " ---------------------------------------------------------------------------------------------\n";
			cout << "| letter                                            |     DFS     |    BFS      |     A*      |\n";
			cout << " ---------------------------------------------------------------------------------------------\n";
			temp = listInput->head;
			while (temp != NULL)
			{


				begin = clock();
				t = DFS(temp);
				end = clock();
				totalDFS += (double)(end - begin) / CLOCKS_PER_SEC;
				if (t == NULL)
				{
					timeDFS = -1;
				}
				else
				{
					timeDFS = (double)(end - begin) / CLOCKS_PER_SEC;
				}
				begin = clock();
				t = BFS(temp);
				end = clock();
				totalBFS += (double)(end - begin) / CLOCKS_PER_SEC;
				if (t == NULL)
				{
					timeBFS = -1;
				}
				else
				{
					timeBFS = (double)(end - begin) / CLOCKS_PER_SEC;

				}
				begin = clock();
				t = ASearch(temp);
				end = clock();
				totalA += (double)(end - begin) / CLOCKS_PER_SEC;
				if (t == NULL)
				{
					timeA = -1;
				}
				else
				{
					timeA = (double)(end - begin) / CLOCKS_PER_SEC;
				}
				solution1(temp, timeDFS, timeBFS, timeA);
				temp = temp->nextNode;
			}
			printf_s("| Total                                             | %.9f | %.9f | %.9f |\n", totalDFS, totalBFS, totalA);
			cout << " ---------------------------------------------------------------------------------------------\n";
			break;
		case 5:
			int n;
			totalDFS = 0;
			totalBFS = 0;
			totalA = 0;
			cout << "nhap do dai: ";
			cin >> n;
			begin = clock();
			for (int i = 0; i < 100; i++)
			{
				temp = NodeRandom(n);
				t = DFS(temp);
				end = clock();
				totalDFS += (double)(end - begin) / (CLOCKS_PER_SEC);
				begin = clock();
				t = BFS(temp);
				end = clock();
				totalBFS = (double)(end - begin) / (CLOCKS_PER_SEC);
				begin = clock();
				t = ASearch(temp);
				end = clock();
				totalA = (double)(end - begin) / (CLOCKS_PER_SEC);
			}
			avgDFS = totalDFS / 100;
			avgBFS = totalBFS / 100;
			avgA = totalA / 100;
			cout << " ------------------------------------------\n";
			cout << "|     DFS     |     BFS     |     A*      |\n";
			cout << " ------------------------------------------\n";
			printf_s("| %.9f | %.9f | %.9f |\n", avgDFS,avgBFS, avgA);
			cout << " ------------------------------------------\n";

		default:
			break;
		}

	} while (true);
}






