#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
    char firstName[30];
    char lastName[30];
    char phoneNumber[10];
    struct node *leftChild, *rightChild, *duplicatePtr;
} Node, *nodePtr;

nodePtr create_node(void);
nodePtr delete_node(nodePtr);
void insert(nodePtr, nodePtr);
void inorder(nodePtr);
void printReturn(nodePtr);
void retrieve(nodePtr);
nodePtr findMinimumValue(nodePtr);
int retrieveFirstname(nodePtr);
nodePtr delete_node_firstname(nodePtr);
void inorderSave(nodePtr);

FILE *fp;


char lastNameTemp[30];
char firstNameTemp[30];
nodePtr subBinaryHead;

int main(void)
{
    int choice, ans = 1;
    nodePtr new_node, root;
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
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            do
            {
                new_node = create_node();
                printf("\nEnter last name: ");
                scanf("%s", new_node->lastName);

                printf("\nEnter first name: ");
                scanf("%s", new_node->firstName);

                printf("\nEnter phone number: ");
                scanf("%s", new_node->phoneNumber);
                if (root == NULL)
                    root = new_node;
                else
                    insert(root, new_node);
                printf("\nWant To enter More Elements?(1/0)");
                scanf("%d", &ans);
            } while (ans);
            break;
        case 2:
            if (root == NULL)
                printf("The tree is empty");
            else
            {
                printf("\nThe inorder display :");
				
                inorder(root);
			    
            }
            break;
        case 3:
            firstNameTemp[0] = 0;
            printf("\nEnter last name: ");
            scanf("%s", &lastNameTemp);
            retrieve(root);
            break;            
        case 4:
            printf("\nEnter last name to delete: ");
            scanf("%s", &lastNameTemp);
            root = delete_node(root);
            break;
		case 5:
			//save
			fp = fopen("phonebook.txt","w");
			inorderSave(root);
			fclose(fp);
			printf("\nData saved");
			break;
		case 6:
				//load
			fp = fopen("phonebook.txt","r");
			if(fp == NULL){
				fclose(fp);
				printf("\nFile does not exit");
				break;
				}
			while(!feof(fp)){
					new_node = create_node();
					fscanf(fp,"%s %s %s\n",new_node->lastName,new_node->firstName,new_node->phoneNumber);
					if(new_node->lastName == NULL)
						free(new_node);
 					else if (root == NULL)      
                   		root = new_node;             
               	    else
                    	insert(root, new_node);
				}
			fclose(fp);
			break;
        default:
            printf("Invalid choice");
            break;
        }

    } while (choice != 7);
    return 0;
}
void printReturn(nodePtr root)
{
    printf("\nLast Name: %s, First Name: %s, Phone: %s\t", root->lastName, root->firstName, root->phoneNumber);
}
int retrieveFirstname(nodePtr root){
	int res = strcmp(firstNameTemp, root->firstName);
	if(res == 0){
		printReturn(root);
		return 1;
	}
	else if(res < 0 && root->leftChild != NULL)
		retrieveFirstname(root->leftChild);
	else if(res > 0 && root->rightChild != NULL)
		retrieveFirstname(root->rightChild);
}
void retrieve(nodePtr root)
{
    if (root != NULL)
    {
        int res = strcmp(lastNameTemp, root->lastName);
        if (res == 0)
        {
			if (root->duplicatePtr == NULL){
				printReturn(root);
				return;
			}
			else{
				if (firstNameTemp[0] == 0)
                    {
                        printf("Found duplicate, please enter First Name:");
                        scanf("%s", &firstNameTemp);
						
                    }
				if(retrieveFirstname(root->duplicatePtr) == 1)
					return;
			}
        }         
        
        else if (res < 0)
        {
            retrieve(root->leftChild);
        }
        else if (res > 0)
        {
            retrieve(root->rightChild);
        }
    }
}
nodePtr create_node(void)
{
    nodePtr np;
    np = (nodePtr)malloc(sizeof(Node));
    np->leftChild = NULL;
    np->rightChild = NULL;
	np->duplicatePtr = NULL;

    return np;
}

void inorder(nodePtr temp)
{
    if (temp != NULL)
    {
        inorder(temp->leftChild);
		if(temp->duplicatePtr == NULL)
        	printReturn(temp);
		
		//else
		//	printf("\nI'm subhead: LastName: %s,FirstName: %s\t", temp->lastName, temp->firstName);
		inorder(temp->duplicatePtr);	
        inorder(temp->rightChild);
    }
}
void inorderSave(nodePtr temp)
{
    if (temp != NULL)
    {
        inorderSave(temp->leftChild);
		if(temp->duplicatePtr == NULL)
			fprintf(fp,"%s %s %s\n", temp->lastName, temp->firstName,temp->phoneNumber);	
		inorderSave(temp->duplicatePtr);	
        inorderSave(temp->rightChild);
    }
}

nodePtr delete_node_firstname(nodePtr root){
	
	if (root == NULL)
    {
        return root;
    }
	int res = strcmp(firstNameTemp,root->firstName);
	if(res<0)
	{
		root->leftChild = delete_node_firstname(root->leftChild);
	}
	else if(res>0){
		root->rightChild = delete_node(root->rightChild); 
	}
	//Found the firstname
	else{
		//Case 1: No child
		if (root->leftChild == NULL && root->rightChild == NULL)
        {	
			if(strcmp(root->firstName,subBinaryHead->firstName) ==0 &&
					subBinaryHead ->duplicatePtr->rightChild == NULL &&
					subBinaryHead ->duplicatePtr->leftChild == NULL )

					subBinaryHead ->duplicatePtr= NULL;
			
			free(root);
			root = NULL;		
		}	
			   
        //case2: One child
        else if (root->leftChild == NULL)
        {		
			nodePtr temp = root;
			if(strcmp(root->firstName, subBinaryHead->firstName) == 0){
				root = root->rightChild;
				strcpy(subBinaryHead->lastName, root->lastName);
            	strcpy(subBinaryHead->firstName, root->firstName);
            	strcpy(subBinaryHead->phoneNumber,root->phoneNumber);	
				subBinaryHead->duplicatePtr =root;
			}
			else
				root = root->rightChild;				
			free(temp);
		}
        else if (root->rightChild == NULL)
        {		
				nodePtr temp = root;
				if(strcmp(root->firstName, subBinaryHead->firstName) == 0){
					root = root->leftChild;
					strcpy(subBinaryHead->lastName, root->lastName);
            		strcpy(subBinaryHead->firstName, root->firstName);
            		strcpy(subBinaryHead->phoneNumber,root->phoneNumber);
					subBinaryHead->duplicatePtr =root;				
				}
				else
					root = root->leftChild;			
			
				free(temp);	

        }
		//case3: 2 children
        else
        {
            nodePtr temp = findMinimumValue(root->rightChild);
			if(strcmp(root->firstName, subBinaryHead->firstName) == 0){
			strcpy(subBinaryHead->lastName, temp->lastName);
            strcpy(subBinaryHead->firstName, temp->firstName);
            strcpy(subBinaryHead->phoneNumber,temp->phoneNumber);	
			}
            strcpy(root->lastName, temp->lastName);
            strcpy(root->firstName, temp->firstName);
            strcpy(root->phoneNumber,temp->phoneNumber);	
			strcpy(firstNameTemp,temp->firstName);		
            root->rightChild = delete_node_firstname(root->rightChild);
        }
	}
	return root;
}

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
    else
    {
		if(root->duplicatePtr == NULL){
        // found the thing
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
            strcpy(root->phoneNumber,temp->phoneNumber);
			strcpy(lastNameTemp,temp->lastName);
            root->rightChild = delete_node(root->rightChild);
        }
    }else{
			
			if(root->duplicatePtr->leftChild ==NULL && root->duplicatePtr->rightChild==NULL){
				root->duplicatePtr = NULL;
				root = delete_node(root);
			}
			else{
			subBinaryHead = root;	
			printf("Found duplicate, enter first name to delete: ");
			scanf("%s", firstNameTemp);
			
			delete_node_firstname(root->duplicatePtr);
			if(subBinaryHead->duplicatePtr == NULL)
				root = delete_node(subBinaryHead);	
			}				
		}
	}
    return root;
}

nodePtr findMinimumValue(nodePtr root)
{
    if (root->leftChild == NULL)
        return root;
    else if (root->leftChild != NULL)
        findMinimumValue(root->leftChild);
}

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
		printf("Cannot insert duplicate first and last name");
	}
}

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
    else if (strcmp(new_node->lastName, root->lastName) == 0)
    {
		if(strcmp(new_node->lastName, root->lastName) == 0 && strcmp(new_node->firstName, root->firstName) == 0)
		{
			printf("Cannot insert duplicate first and last name");
		}
		else if(root->duplicatePtr == NULL){
			root->duplicatePtr = create_node();
			strcpy(root->duplicatePtr->firstName,root->firstName);
			strcpy(root->duplicatePtr->lastName,root->lastName);
			strcpy(root->duplicatePtr->phoneNumber,root->phoneNumber);
			insert(root,new_node);
		}
		else
			insertFirstname(root->duplicatePtr,new_node);
    }
}
