/**
 * Assignment 1 - The Wacky Store
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
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 1024

typedef struct ItemNode ItemNode;
struct ItemNode {
    char name[MAX_NAME_LENGTH];
    int count;
    ItemNode* next;
};

typedef struct Customer Customer;
struct Customer {
    char name[MAX_NAME_LENGTH];
    ItemNode* cart;
};

typedef struct CheckoutLaneNode CheckoutLaneNode;
struct CheckoutLaneNode {
    Customer* customer;

    CheckoutLaneNode* front;
    CheckoutLaneNode* back;
};

typedef struct CheckoutLane CheckoutLane;
struct CheckoutLane {
    CheckoutLaneNode* first;
    CheckoutLaneNode* last;
};

/**
 * Function: new_item_node
 * -----------------------
 * Allocate a new ItemNode. Allocation must be done manually (with malloc or
 * calloc). Initialize all variables using the arguments provided. Assume that
 * count will always be greater than 0.
 */
ItemNode* new_item_node(char* name, int count) {
	ItemNode* item_pointer = calloc(1, sizeof(ItemNode));
	if (item_pointer == NULL) {
		return NULL;
	}
	strcpy(item_pointer->name, name);
	item_pointer->count = count;
	item_pointer->next = NULL;
    return item_pointer;
}

/**
 * Function: new_customer
 * ----------------------
 * Allocate a new Customer. Allocation must be done manually (with malloc or
 * calloc). Initialize all variables using the arguments provided.
 */
Customer* new_customer(char* name) {
	Customer* cust_pointer = calloc(1, sizeof(Customer));
	if (cust_pointer == NULL) {
		return NULL;
	}
	strcpy(cust_pointer->name, name);
	cust_pointer->cart = NULL;
    return cust_pointer;
}

/**
 * Function: free_customer
 * -----------------------
 * Release all memory associated with a Customer back to the system. This
 * includes any items they may have had in their cart.
 */
void free_customer(Customer* customer) {
    // TODO: Complete this function.
	ItemNode* node = customer->cart;
	while (node != NULL) {
		ItemNode* next_node = node->next;
		free(node);
		node = next_node;
	}
	free(customer);
	return;
}


/**
 * Function: open_new_checkout_line
 * --------------------------------
 * Allocate a new empty checkout lane. Allocation must be done manually (with
 * malloc or calloc).
 */
CheckoutLane* open_new_checkout_line() {
    CheckoutLane* lane_ptr = calloc(1, sizeof(CheckoutLane));
	if (lane_ptr == NULL) {
		return NULL;
	}
	lane_ptr->first = NULL;
	lane_ptr->last = NULL;
    return lane_ptr;
}

/**
 * Function: new_checkout_node
 * ---------------------------
 * Allocate a new CheckoutLaneNode. Allocation must be done manually (with
 * malloc or calloc). Initialize all variables using the arguments provided. Do
 * not allocate a new customer; instead copy the existing reference over.
 */
CheckoutLaneNode* new_checkout_node(Customer* customer) {
    CheckoutLaneNode* lane_ptr = calloc(1, sizeof(CheckoutLaneNode));
	if (lane_ptr == NULL) {
		return NULL;
	}
	lane_ptr->front = NULL;
	lane_ptr->back = NULL;
	lane_ptr->customer = customer;
    return lane_ptr;
}

/**
 * Function: add_item_to_cart
 * --------------------------
 * Add an item to a customer's cart, given its name and amount.
 *
 * If the given amount is 0 or less, do nothing.
 *
 * IMPORTANT: The items in a customer's cart should always be arranged in
 * lexicographically smallest order based on the item names. Consider the use of
 * the ASCII strcmp() function from <string.h>.
 *
 * No two ItemNodes in a customer's cart can have the same name.
 * If the customer already has an ItemNode with the same item name in their
 * cart, increase the node's count by the given amount instead.
 */
void add_item_to_cart(Customer* customer, char* item_name, int amount) {
    if (amount <= 0) {
		return;
	}
	ItemNode* node = customer->cart;
	if (node == NULL) {
		ItemNode* new_node = new_item_node(item_name, amount);
		customer->cart = new_node;
		return;
	}

	while (node != NULL) {
		ItemNode* next_node = node->next;
		if (strcmp(node->name, item_name) == 0) {
			node->count += amount; 
			return;
		}
		//Adding to end of the list
		if (next_node == NULL && strcmp(node->name, item_name) < 0) {
			ItemNode* cust = new_item_node(item_name, amount);
			if (cust == NULL) {
				return;
			}
			cust->next = next_node;
			node->next = cust;
			return;		
		}
		//Addint to the beggining of the list
		if (node == customer->cart && strcmp(node->name, item_name) > 0) {
			ItemNode* cust = new_item_node(item_name, amount);
			if (cust == NULL) {
				return;
			}
			cust->next = node;
			customer->cart = cust;
			return;	
		}
		if (strcmp(node->name, item_name) < 0 && strcmp(next_node->name, item_name) > 0) {
			ItemNode* cust = new_item_node(item_name, amount);
			if (cust == NULL) {
				return;
			}
			cust->next = next_node;
			node->next = cust;
			return;
		}
		node = next_node;
	}
	return;
}

/**
 * Function: remove_item_from_cart
 * -------------------------------
 * Attempt to reduce the quantity of an item in a customer's cart, given its
 * name and amount.
 *
 * If no ItemNode in the customer's cart match the given item name, or the
 * amount given is <= 0, do nothing.
 *
 * If the quantity is reduced to a value less than or equal to 0, remove the
 * ItemNode from the customer's cart. This means you will need to do memory
 * cleanup as well.
 */
void remove_item_from_cart(Customer* customer, char* item_name, int amount) {
    if (amount <= 0) {
		return;
	}
	ItemNode* node = customer->cart;

	while (node != NULL) {
		ItemNode* next_node = node->next;
		// If you have to remove a 1 node list 
		if (next_node == NULL && strcmp(node->name, item_name) == 0) {
			if (((node->count)-amount) > 0) {
				node->count -= amount;
			}
			else {
				customer->cart = NULL;
				free(node);
			}
			return;
		}
		
		if (strcmp(next_node->name, item_name) == 0) {
			if (((next_node->count)-amount) > 0) {
				next_node->count -= amount;
			}
			else {
				node->next = next_node->next;
				free(next_node);
			}
			return;
		}
		node = next_node;
	}
	return;
}

/**
 * Function: total_number_of_items
 * -------------------------------
 * Count the total number of items in a customer's cart by summing all ItemNodes
 * and their associated quantities.
 */
int total_number_of_items(Customer* customer) {
    ItemNode* node = customer->cart;
	int count = 0;
	while (node != NULL) {
		ItemNode* next_node = node->next;
		count += node->count;
		node = next_node;
	}
    return count;
}

/**
 * Function: queue
 * ---------------
 * A customer has finished shopping and wishes to checkout. Add the given
 * customer to the end of the given checkout lane.
 */
void queue(Customer* customer, CheckoutLane* lane) {
    CheckoutLaneNode* node = new_checkout_node(customer);
	if (lane->last == NULL) {
		lane->last = node;
		lane->first = node;
		return;
	}
	lane->last->back = node;
	node->front = lane->last;
	lane->last = node;
	return;
}

/**
 * Function: process
 * -----------------
 * Serve the customer at the head of the checkout lane and return the
 * total_number_of_items() the customer had in their cart.
 *
 * The customer leaves the store after being processed. Therefore, you must also
 * free any memory associated with them.
 *
 * If this function is called on an empty lane, return 0.
 */
int process(CheckoutLane* lane) {
	if (lane == NULL || lane->first == NULL || lane->first->customer == NULL) {
		return 0;
	}
    Customer* head = lane->first->customer;
	int count = total_number_of_items(head);
	CheckoutLaneNode* node = lane->first;

	CheckoutLaneNode* behind = lane->first->back;
	if (behind != NULL) {
		lane->first = behind;
		lane->first->front = NULL;
		free_customer(head);
		free(node);
	}
	else {
		lane->first = NULL;
		lane->last = NULL;
		free_customer(head);
		free(node);
	}
	
	return count;
}

/**
 * Function: balance_lanes
 * -----------------------
 * Move a single customer from the end of the most busy checkout lane to the end
 * of the least busy checkout lane.
 *
 * Busyness is defined as the total number of customers in a checkout lane.
 *
 * If multiple lanes have the same busyness, select the lane that comes first in
 * the CheckoutLane* array.
 *
 * If the difference between the MAX and MIN checkout lanes is <= 1, do nothing.
 *
 * If there are less than 2 lanes, do nothing.
 *
 * Return true if and only if a customer was moved; otherwise false.
 */
 
 
int num_customer(CheckoutLane* lane) {
	int counter = 0;
	CheckoutLaneNode* node = lane->last;
	while (node != NULL) {
		CheckoutLaneNode* next_node = node->front;
		counter++;
		node = next_node;
	}
	return counter;
}
bool balance_lanes(CheckoutLane* lanes[], int number_of_lanes) {
	//Find out which checkout lane has the least customers (keep track of the max and min)
	//Find difference between max and mind
	//If less than 2 lanes do nothing
	if (number_of_lanes < 2) {
		return false;
	}
	int max = num_customer(lanes[1]);
	int min = num_customer(lanes[0]);
	int min_index = 0;
	int max_index = 1;
	for (int i = 0; i < number_of_lanes; i++){
		int num_cust = num_customer(lanes[i]);
		if (num_cust > max) {
			max = num_cust;
			max_index = i;
		}
		else if (num_cust < min) {
			min = num_cust;
			min_index = i;
		}
	}
	if ((max - min) > 1) {
		CheckoutLane* most_busy_lane = lanes[max_index];
		CheckoutLane* least_busy_lane = lanes[min_index];
		CheckoutLaneNode* last_node = most_busy_lane->last;
		most_busy_lane->last = most_busy_lane->last->front;
		most_busy_lane->last->back = NULL;
		last_node->front = least_busy_lane->last;
		least_busy_lane->last->back = last_node;
		least_busy_lane->last = last_node;
		return true;
	}
    return false;
}

/**
 * Function: process_all_lanes
 * ---------------------------
 * Given an array of CheckoutLane*, process() each CheckoutLane a single time
 * and return the the sum of the result.
 */
int process_all_lanes(CheckoutLane* lanes[], int number_of_lanes) {
    int count = 0; 
	for (int i = 0; i < number_of_lanes; i++){
		CheckoutLane* lane = lanes[i];
		count += process(lane);
	}
    return count;
}

/**
 * Function: close_store
 * ---------------------
 * It's closing time. Given an array of CheckoutLane*, free all memory
 * associated with them. Any customers still left in the queue is kicked out and
 * also freed from memory.
 */
void close_store(CheckoutLane* lanes[], int number_of_lanes) {
    for (int i = 0; i < number_of_lanes; i++) {
        CheckoutLane* lane = lanes[i];
        if (lane != NULL) {
            CheckoutLaneNode* node = lane->first;
            while (node != NULL) {
                CheckoutLaneNode* next_node = node->back;
                free_customer(node->customer);
                free(node);
                node = next_node;
            }
            free(lane);
			lanes[i] = NULL;
        }
    }
}

