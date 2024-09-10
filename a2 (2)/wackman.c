/**
 * Assignment 1 - Wackman Compression
 *
 * Please read the comments below carefully, they describe your task in detail.
 *
 * There are also additional notes and clarifications on Quercus.
 *
 * Any clarifications and corrections will be posted to Piazza so please keep an
 * eye on the forum!
 *
 * Unauthorized distribution or posting is strictly prohibited. You must seek
 * approval from course staff before uploading and sharing with others.
 */

// No additional imports are allowed. You can make helper functions if you wish.
#include "wackman.h"

/**
 * Given an array of integers and its length, this function calculates
 * the sum of all the elements in the array.
 *
 * @pre The array size (array_size) must be greater than or equal to 0.
 *
 * @param int_array Pointer to the array of integers.
 * @param array_size The number of elements in the array.
 * @return The sum of all integers in the array.
 */
int sum_array_elements(int int_array[], int array_size) {
   int sum = 0;
   for (int i = 0; i < array_size: i++) {
	   sum += int_array[i];  
   }
   return sum;
}

/**
 * Given an integer array of size ASCII_CHARACTER_SET_SIZE, this function
 * computes the number of occurrences of each ASCII character in the given
 * string. The results are stored in the corresponding indices of the
 * occurrence_array. The computation stops at and does not include the DELIMITER
 * character.
 *
 * @note The occurrence_array[0] should always be 0, representing the DELIMITER.
 * @note The ASCII decimal value of each character corresponds to its index in
 * the array. For example, 'F' with ASCII decimal value 70 will increment
 * occurrence_array[70].
 *
 * @param occurrence_array An array to store the occurrences of each ASCII
 * characters. The array size should be ASCII_CHARACTER_SET_SIZE.
 * @param string The input string to analyze for ASCII character occurrences.
 */
void compute_occurrence_array(int occurrence_array[ASCII_CHARACTER_SET_SIZE], char* string) {
	for (int i = 0; i < ASCII_CHARACTER_SET_SIZE; i++) {
	    occurrence_array[i] = 0;
	}
	for (int i = 0; i < strlen(string); i++) {
		int ascii = string[i];
		occurrence_array(ascii)++;
	}
}

/**
 * Given an integer array of size ASCII_CHARACTER_SET_SIZE, this function
 * computes and returns the number of characters that occur one or more times.
 *
 * @param occurrence_array An array representing the occurrences of ASCII
 * characters. The array size should be ASCII_CHARACTER_SET_SIZE.
 * @return The number of characters occurring one or more times in the ASCII
 * character set.
 */
int count_positive_occurrences(int occurrence_array[ASCII_CHARACTER_SET_SIZE]) {
	int count = 0;
	for (int i = 0; i < ASCII_CHARACTER_SET_SIZE; i++) {
	    if occurrence_array[i] > 0 {
			count++;
		}
	}
    return count;
}

/**
 * Given an integer array of size ASCII_CHARACTER_SET_SIZE, representing the
 * number of occurrences of ASCII characters, this function creates and returns
 * a sorted linked list of WackyLinkedNodes.
 *
 * Each node in the list stores a tree with a single leaf node. The node will
 * contain information about the probability of occurrence 'weight' and the
 * ASCII character itself 'val'.
 *
 * @note Exclude elements with a probability of 0 from the linked list.
 * @note Ensure that the linked list is sorted in ascending order from head to
 * tail, first by the probability of occurrence ('weight'), and in case of ties,
 * by the ASCII character ('val') in ascending order.
 * @note Compute the probability of occurrence for each ASCII character α as
 * occurrence_array[α] / SUM(occurrence_array).
 * @note The memory of any required data types must be allocated manually.
 *
 * @param occurrence_array An array representing the occurrences of ASCII
 * characters. The array size should be ASCII_CHARACTER_SET_SIZE.
 * @return A pointer to the head of the created linked list of WackyLinkedNodes.
 
 */
WackyLinkedNode* create_wacky_list(int occurrence_array[ASCII_CHARACTER_SET_SIZE]) {
	char val = 0;
	double weight = occurrence_array[0] / SUM(occurrence_array);
	WackyLinkedNode* head = new_linked_node(weight, val); 
    for int i = 0; i < ASCII_CHARACTER_SET_SIZE; i++) {
		val = i;
		weight = occurrence_array[i] / SUM(occurrence_array);
		WackyTreeNode* leaf = new_leaf_node(weight, val);
		WackyLinkedNode* node = insert_wacky_list(leaf);
		head = insert_wacky_list(head, node);
	}
    return head;
}

WackyLinkedNode* insert_wacky_list(WackyLinkedNode* head, WackyLinkedNode* node) {
		if (head = NULL) {
			return node;
		}
		current_node = head;
		while (current->next != NULL && (current->next->val->weight < node->val->weight || (current->next->val->weight == node->val->weight && current->next->val->val < node->val->val))) {
			current = current->next;
		}
		node->next = current->next;
		current->next = node;

		return head;
}
	
	
WackyTreeNode* new_leaf_node(double weight, char val) {
    WackyTreeNode* node = (WackyTreeNode*)malloc(sizeof(WackyTreeNode));
    node->weight = weight;
    node->val = val;
    node->left = NULL;
    node->right = NULL;
    return node;
}

WackyTreeNode* new_branch_node(WackyTreeNode* left, WackyTreeNode* right) {
    WackyTreeNode* node = (WackyTreeNode*)malloc(sizeof(WackyTreeNode));
    node->weight = left->weight + right->weight;
    node->val = '\0';
    node->left = left;
    node->right = right;
    return node;
}

WackyLinkedNode* new_linked_node(WackyTreeNode* val) {
    WackyLinkedNode* node = (WackyLinkedNode*)malloc(sizeof(WackyLinkedNode));
    node->val = val;
    node->next = NULL;
    return node;
}

int count(WackyTreeNode* head) {
	int count = 0;
	WackyLinkedNode* node = head; 
	while(node->next != NULL) {
		count++;
		node = node->next;
	}
	return count;
}
/**
 * Given a sorted linked list of WackyTreeNodes, where each node (initially)
 * contains a tree with a single leaf node, this function generates a tree based
 * on the following algorithm:
 * - If the size of the linked list is 2 or more:
 *    - Remove 2 WackyLinkedNodes from the head.
 *    - Create a new WackyTreeNode that joins the tree nodes inside the removed
 *      linked nodes. The first node ('head') goes to the LEFT, and the
 *      following node ('head->next') goes to the RIGHT.
 *    - Create a new WackyLinkedNode and add the newly created WackyTreeNode
 *      back into the linked list. The linked list must remain in sorted order
 *      by the probability of occurrence 'weight'. If the newly created node has
 *      the same weight as another node already in the list, add it in front of
 *      all existing similarily weighted nodes.
 *    - Repeat this algorithm until the size of the linked list is 1.
 * - If the size of the linked list is 1:
 *    - Return the address of the tree held at the head of the linked list.
 * - Otherwise:
 *    - Return NULL.
 *
 * @note The memory of any WackyLinkedNodes must be freed by the end of this
 * function.
 *
 * @param linked_list The sorted linked list of WackyTreeNodes.
 * @return A pointer to the root of the generated tree or NULL if the list is
 * empty.
 */
WackyTreeNode* merge_wacky_list(WackyLinkedNode* linked_list) {
    int number_of_nodes = count(linked_list);
	if (number_of_nodes == 1) {
		return linked_list->val;
	}
	if (number_of_nodes == 0) {
		return NULL;
	}
	
	
    //Remove 2 WackyLinkedNodes from the head
	WackyLinkedNode* head = linked_list->next->next;
	linked_list->next->next = NULL;
	linked_list->next = NULL;
	WackyTreeNode* tree_node = new_branch_node(linked_list->val, linked_list->next->val);
	WackyLinkedNode* list_node = new_linked_node(tree_node);
	merge_wacky_list(insert_wacky_list(head, list_node));
	
}

/**
 * Given a tree, this function calculates and returns the height of the tree.
 *
 * @note An empty tree (NULL) has a height of 0.
 *
 * @param tree Pointer to the root of the tree.
 * @return The height of the tree.
 */
int get_height(WackyTreeNode* tree) {
    if (tree == NULL) {
		return 0;
	}
	left_height = get_height(tree->left);
	right_height = get_height(tree->right);
    if (left_height > right_height) {
		return left_height + 1
	}
	if (right_height > left_height) {
		return right_height + 1
	}
}
BST_Node *BST_search(BST_Node *root)
{
	if (root == NULL) {
		return NULL;
	}
	root->left = BST_search(root->left);
	
	printf(name);
	
	root->right = BST_search(root->right);
	printf(root->name);
	return NULL;

)

BST_Node *BST_del(BST_Node *root, char name[1024])
{
	if (root == NULL) {
		return NULL;
	}

	root->name,name < 0
		root->left = BST_del(root->left)
	root->name,name > 0 
		root->right BST_del(root->right)
		

)
/**
 * Given a WackyTree and a specific character, this function computes the
 * traversal of the character based on its position in the tree. Movement to the
 * LEFT is FALSE, and movement to the RIGHT is TRUE. The steps are written
 * inside boolean_array, and the total number of steps is stored in array_size.
 *
 * @note the size of boolean_array is greater than or equal to get_height(tree)
 *
 * @param tree Pointer to the root of the WackyTree.
 * @param character The specific ASCII character to compute the encoding for.
 * @param boolean_array An array to store the traversal steps.
 * @param array_size Pointer to the variable holding the total number of steps.
 * If the character is not found, -1 is written to array_size instead.
 */
void get_wacky_code(WackyTreeNode* tree, char character, bool boolean_array[], int* array_size) {
    // TODO: Complete this function.
}

/**
 * Given the root of a WackyTree, a boolean array, and the size of the array,
 * this function traverses the tree. FALSE indicates a movement to the left,
 * and TRUE indicates a movement to the right. The function returns the
 * character at the node reached after all the steps have been taken. If the
 * node is not a leaf node (LEFT and RIGHT are NOT NULL), it returns the
 * DELIMITER ('\0') instead.
 *
 * @param tree Pointer to the root of the WackyTree.
 * @param boolean_array An array representing the traversal steps.
 * @param array_size The size of the boolean array.
 * @return The character at the reached node or the DELIMITER ('\0') if the node
 * is not a leaf node.
 */
char get_character(WackyTreeNode* tree, bool boolean_array[], int array_size) {
    // TODO: Complete this function.
    return '\0';
}

/**
 * Given a binary tree, this function frees the memory associated with the
 * entire tree.
 *
 * @param tree Pointer to the root of the binary tree to be freed.
 */
void free_tree(WackyTreeNode* tree) {
    if (tree == NULL) {
		return;
	}
	free_tree(tree->left);
	free_tree(tree->right);
	free(tree);
}
