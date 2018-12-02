/*
Abdul Tesqie(991327045) and Rushen Naidu(991435389)
Dual Binary Search Tree
12/2/2018

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//Structure to hold all the phonebook values with 3 pointers to form a binary search tree
typedef struct node
{
    char firstName[30];
    char lastName[30];
    char phoneNumber[10];
    struct node *leftChild, *rightChild, *duplicatePtr;
} Node, *nodePtr;

//All the method decleration
nodePtr create_node(void);
nodePtr delete_node(nodePtr);
void insert(nodePtr, nodePtr);
void inorder(nodePtr);
void printReturn(nodePtr);
void retrieve(nodePtr);
nodePtr findMinimumValue(nodePtr);
int retrieveFirstname(nodePtr);
int checkNames(char[]);
int checkPhoneN(char[]);
nodePtr delete_node_firstname(nodePtr);
void inorderSave(nodePtr);

//File pointer
FILE *fp;

char lastNameTemp[30];
char firstNameTemp[30];
//The subBinaryHead is the middle pointer to store duplicates
nodePtr subBinaryHead;

int main(void)
{
    char choice[1], ans;
    nodePtr new_node, root;
	//used for name and number validations
    char checkFName[30];
    char checkLName[30];
    char checkPhone[10];
    int num;
    root = NULL;
    printf("Phone book app");
    do
    {
        printf("\n1. Add");
        printf("\n2. Traversals");
        printf("\n3. Retrieve");
        printf("\n4. Delete");
        printf("\n5. Save");
        printf("\n6. Load");
        printf("\n7. Exit");
        printf("\nPlease select one of the following options: ");
        scanf("%s", choice);
        num = atoi(choice);
        switch (num)
        {
        //Case 1 is adding a new value to the tree
        case 1:
            do
            {
                new_node = create_node();

                printf("\nEnter last name: ");
                scanf("%s", checkLName);

                while (checkNames(checkLName) != 1)
                {
                    printf("\nEnter last name: ");
                    scanf("%s", checkLName);
                }
                strcpy(new_node->lastName, checkLName);

                printf("\nEnter first name: ");
                scanf("%s", checkFName);

                while (checkNames(checkFName) != 1)
                {
                    printf("\nEnter first name: ");
                    scanf("%s", checkFName);
                }
                strcpy(new_node->firstName, checkFName);

                printf("\nEnter phone number: ");
                scanf("%s", checkPhone);

                while (checkPhoneN(checkPhone) != 1)
                {
                    printf("Please enter a valid phone number");
                    printf("\nEnter phone number: ");
                    scanf("%s", checkPhone);
                }

                strcpy(new_node->phoneNumber, checkPhone);

                if (root == NULL)
                    root = new_node;
                else
                    insert(root, new_node);
                printf("\nWant To enter More Elements?(1/0)");
                scanf("%d", &ans);
            } while (ans);
            break;
        //Case 2 is used to traverse the tree
        case 2:
            if (root == NULL)
                printf("\nThe tree is empty\n");
            else
            {
                printf("\nThe inorder display :");

                inorder(root);
            }
            break;
        //case 3 Used to retrieve a specific user
        case 3:
            firstNameTemp[0] = 0;
            printf("\nEnter last name: ");
            scanf("%s", lastNameTemp);
            retrieve(root);
            break;
		//case 4 is used to delete a node
        case 4:
            printf("\nEnter last name to delete: ");
            scanf("%s", lastNameTemp);
            root = delete_node(root);
            break;
		//case 5 is ued to save
        case 5:
            fp = fopen("phonebook.txt", "w");
            inorderSave(root);
            fclose(fp);
            printf("\nData saved");
            break;
		//case 6 is used to load
        case 6:         
            fp = fopen("phonebook.txt", "r");
            if (fp == NULL)
            {
                fclose(fp);
                printf("\nFile does not exit");
                break;
            }
            while (!feof(fp))
            {
                new_node = create_node();
                fscanf(fp, "%s %s %s\n", new_node->lastName, new_node->firstName, new_node->phoneNumber);
                if (new_node->lastName == NULL)
                    free(new_node);
                else if (root == NULL)
                    root = new_node;
                else
                    insert(root, new_node);
            }
            fclose(fp);
            break;
		//Default case prints invalid choice or goodbye
        default:
            if (num == 7)
                printf("\nGoodbye\n");
            else
                printf("\nInvalid choice\n");
            break;
        }
    } while (num != 7);
    return 0;
}
//Used to print each ndoe
void printReturn(nodePtr root)
{
    printf("\nLast Name: %s, First Name: %s, Phone: %s\t", root->lastName, root->firstName, root->phoneNumber);
}

//Retrieve using firstname if duplicates exist
int retrieveFirstname(nodePtr root)
{
    int res = strcmp(firstNameTemp, root->firstName);
    if (res == 0)
    {
        printReturn(root);
        return 1;
    }
    else if (res < 0 && root->leftChild != NULL)
        retrieveFirstname(root->leftChild);
    else if (res > 0 && root->rightChild != NULL)
        retrieveFirstname(root->rightChild);
}
//retrieve using last name
void retrieve(nodePtr root)
{
    if (root != NULL)
    {
        int res = strcmp(lastNameTemp, root->lastName);
		//If res == 0 we found a match
        if (res == 0)
        {
			//if this condition is true, there is no duplicates and we print
            if (root->duplicatePtr == NULL)
            {
                printReturn(root);
                return;
            }
			//else there is a duplicate and we must traverse the duplicate tree
            else
            {
                if (firstNameTemp[0] == 0)
                {
                    printf("Found duplicate, please enter First Name:");
                    scanf("%s", firstNameTemp);
                }
                if (retrieveFirstname(root->duplicatePtr) == 1)
                    return;
            }
        }
		//Checks to see if we should go left or right in the binary tree
        else if (res < 0)
        {
            retrieve(root->leftChild);
        }
		//Checks to see if we should go left or right in the binary tree
        else if (res > 0)
        {
            retrieve(root->rightChild);
        }
    }
}
//Method used to allocate memory to a new node
nodePtr create_node(void)
{
    nodePtr np;
    np = (nodePtr)malloc(sizeof(Node));
    np->leftChild = NULL;
    np->rightChild = NULL;
    np->duplicatePtr = NULL;

    return np;
}

//traverse the tree inorder
void inorder(nodePtr temp)
{
    if (temp != NULL)
    {
        inorder(temp->leftChild);
        if (temp->duplicatePtr == NULL)
            printReturn(temp);

        inorder(temp->duplicatePtr);
        inorder(temp->rightChild);
    }
}
//traverse the tree in order and print to file
void inorderSave(nodePtr temp)
{
    if (temp != NULL)
    {
        inorderSave(temp->leftChild);
        if (temp->duplicatePtr == NULL)
            fprintf(fp, "%s %s %s\n", temp->lastName, temp->firstName, temp->phoneNumber);
        inorderSave(temp->duplicatePtr);
        inorderSave(temp->rightChild);
    }
}

//Delete a node using first name, only used if there is duplicates
nodePtr delete_node_firstname(nodePtr root)
{

    if (root == NULL)
    {
        return root;
    }
	//Comparing strings using strcmp
    int res = strcmp(firstNameTemp, root->firstName);
    if (res < 0)
    {
        root->leftChild = delete_node_firstname(root->leftChild);
    }
    else if (res > 0)
    {
        root->rightChild = delete_node(root->rightChild);
    }
    //Found the firstname
    else
    {
        //Case 1: No child
        if (root->leftChild == NULL && root->rightChild == NULL)
        {
            if (strcmp(root->firstName, subBinaryHead->firstName) == 0 &&
                subBinaryHead->duplicatePtr->rightChild == NULL &&
                subBinaryHead->duplicatePtr->leftChild == NULL)

                subBinaryHead->duplicatePtr = NULL;

            free(root);
            root = NULL;
        }

        //case2: One child
        else if (root->leftChild == NULL)
        {
            nodePtr temp = root;
            if (strcmp(root->firstName, subBinaryHead->firstName) == 0)
            {
                root = root->rightChild;
                strcpy(subBinaryHead->lastName, root->lastName);
                strcpy(subBinaryHead->firstName, root->firstName);
                strcpy(subBinaryHead->phoneNumber, root->phoneNumber);
                subBinaryHead->duplicatePtr = root;
            }
            else
                root = root->rightChild;
            free(temp);
        }
        else if (root->rightChild == NULL)
        {
            nodePtr temp = root;
            if (strcmp(root->firstName, subBinaryHead->firstName) == 0)
            {
                root = root->leftChild;
                strcpy(subBinaryHead->lastName, root->lastName);
                strcpy(subBinaryHead->firstName, root->firstName);
                strcpy(subBinaryHead->phoneNumber, root->phoneNumber);
                subBinaryHead->duplicatePtr = root;
            }
            else
                root = root->leftChild;

            free(temp);
        }
        //case3: 2 children
        else
        {
            nodePtr temp = findMinimumValue(root->rightChild);
            if (strcmp(root->firstName, subBinaryHead->firstName) == 0)
            {
                strcpy(subBinaryHead->lastName, temp->lastName);
                strcpy(subBinaryHead->firstName, temp->firstName);
                strcpy(subBinaryHead->phoneNumber, temp->phoneNumber);
            }
            strcpy(root->lastName, temp->lastName);
            strcpy(root->firstName, temp->firstName);
            strcpy(root->phoneNumber, temp->phoneNumber);
            strcpy(firstNameTemp, temp->firstName);
            root->rightChild = delete_node_firstname(root->rightChild);
        }
    }
    return root;
}
//Deletes the node using lastname
nodePtr delete_node(nodePtr root)
{
    if (root == NULL)
    {
        return root;
    }
    int res = strcmp(lastNameTemp, root->lastName);
    if (res < 0)
        root->leftChild = delete_node(root->leftChild);
    else if (res > 0)
        root->rightChild = delete_node(root->rightChild);
	// if res == 0 then we found the last name
    else
    {	
		//we must check if any duplicates exist
        if (root->duplicatePtr == NULL)
        {
            
            if (root->leftChild == NULL && root->rightChild == NULL)
            {
                free(root);
                root = NULL;
            }
            else if (root->leftChild == NULL)
            {
                nodePtr temp = root;
                root = root->rightChild;
                free(temp);
            }
            else if (root->rightChild == NULL)
            {
                nodePtr temp = root;
                root = root->leftChild;
                free(temp);
            }
            else
            {
                nodePtr temp = findMinimumValue(root->rightChild);
                strcpy(root->lastName, temp->lastName);
                strcpy(root->firstName, temp->firstName);
                strcpy(root->phoneNumber, temp->phoneNumber);
                strcpy(lastNameTemp, temp->lastName);
                root->rightChild = delete_node(root->rightChild);
            }
        }
		//else there are duplicates and we must check serveral cases 
        else
        {
			//we delete the duplicate if there is only one duplicate
            if (root->duplicatePtr->leftChild == NULL && root->duplicatePtr->rightChild == NULL)
            {
                root->duplicatePtr = NULL;
                root = delete_node(root);
            }
			//else we ask the user to enter the first name to be deleted and then we traverse the tree using first name
            else
            {
                subBinaryHead = root;
                printf("Found duplicate, enter first name to delete: ");
                scanf("%s", firstNameTemp);

                delete_node_firstname(root->duplicatePtr);
                if (subBinaryHead->duplicatePtr == NULL)
                    root = delete_node(subBinaryHead);
            }
        }
    }
    return root;
}

//recursvice call to find the minimum value in the right subtree
nodePtr findMinimumValue(nodePtr root)
{
    if (root->leftChild == NULL)
        return root;
    else if (root->leftChild != NULL)
        findMinimumValue(root->leftChild);
}

//Method to isnert node into tree with duplicate lastname
void insertFirstname(nodePtr root, nodePtr new_node)
{
    if (strcmp(new_node->firstName, root->firstName) < 0)
    {
        if (root->leftChild == NULL)
            root->leftChild = new_node;
        else
            insertFirstname(root->leftChild, new_node);
    }
    if (strcmp(new_node->firstName, root->firstName) > 0)
    {
        if (root->rightChild == NULL)
            root->rightChild = new_node;
        else
            insertFirstname(root->rightChild, new_node);
    }
    else if (strcmp(new_node->firstName, root->firstName) == 0)
    {
        printf("Cannot insert duplicate first and last name\n");
    }
}

//Inserting lastname into tree
void insert(nodePtr root, nodePtr new_node)
{
    if (strcmp(new_node->lastName, root->lastName) < 0)
    {
        if (root->leftChild == NULL)
            root->leftChild = new_node;
        else
            insert(root->leftChild, new_node);
    }
    if (strcmp(new_node->lastName, root->lastName) > 0)
    {
        if (root->rightChild == NULL)
            root->rightChild = new_node;
        else
            insert(root->rightChild, new_node);
    }
	//If the lastname exists then we must copy the value and create a duplicate
	//Then we order the binary search tree based off first name
	//Please see powerpoint for detailed explanation
    else if (strcmp(new_node->lastName, root->lastName) == 0)
    {
        if (strcmp(new_node->lastName, root->lastName) == 0 && strcmp(new_node->firstName, root->firstName) == 0)
        {
            printf("Cannot insert duplicate first and last name\n");
        }
        else if (root->duplicatePtr == NULL)
        {
            root->duplicatePtr = create_node();
            strcpy(root->duplicatePtr->firstName, root->firstName);
            strcpy(root->duplicatePtr->lastName, root->lastName);
            strcpy(root->duplicatePtr->phoneNumber, root->phoneNumber);
            insert(root, new_node);
        }
        else
            insertFirstname(root->duplicatePtr, new_node);
    }
}
//function to validate names  by checking if it only contains letters
int checkNames(char letters[])
{ 
    int val = 0;
    if (strlen(letters) > 30)
    {
        printf("The maximum length of a name is 30");
        val = 0;
    }
    else
    {
        for (int i = 0; i < strlen(letters); i++)
        {
            if (isalpha(letters[i]))
            {
                val = 1;
            }
            else
            {
                val = 0;
                printf("Name can only contain letters");
                break;
            }
        }
    }
    return val;
}
//function to validate phone number  by checking if it only contains numbers
int checkPhoneN(char numbers[])
{ 
    int val = 0;
    if (strlen(numbers) > 10)
    {
        val = 0;
    }
    else
    {
        for (int i = 0; i < strlen(numbers); i++)
        {
            if (isdigit(numbers[i]))
            {
                val = 1;
            }
            else
            {
                val = 0;
                break;
            }
        }
    }
    return val;
}
