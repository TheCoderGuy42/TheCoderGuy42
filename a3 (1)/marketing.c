/**
 * Assignment 3 - Wacky Marketing
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
 *
 * Starter code:  (Updated by) Charles Xu, 2024
 *             (Originally by) William Song, Mustafa Quraish, 2020
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3  // A small graph
#endif

typedef struct user_struct {
    char name[MAX_STR_LEN];
    struct friend_node_struct *friends;
    struct brand_node_struct *brands;
    bool visited;
} User;

typedef struct friend_node_struct {
    User *user;
    struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct {
    char brand_name[MAX_STR_LEN];
    struct brand_node_struct *next;
} BrandNode;

// Note: These are global variables.
FriendNode *allUsers = NULL;
int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head of a linked list of FriendNode, this function checks if
 * a given user's name exists in the list. Returns true if the user's name
 * is found, and false otherwise.
 *
 * @param head Pointer to the head of the linked list of FriendNode.
 * @param node Pointer to the user whose name is to be searched in the list.
 * @return true if the user's name exists in the list, false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node) {
    for (FriendNode *cur = head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->user->name, node->name) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Given the head of a linked list of BrandNode, this function checks if
 * a given brand's name exists in the list. Returns true if the brand's name
 * is found, and false otherwise.
 *
 * @param head Pointer to the head of the linked list of BrandNode.
 * @param name Pointer to the brand name to be searched in the list.
 * @return true if the brand's name exists in the list, false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name) {
    for (BrandNode *cur = head; cur != NULL; cur = cur->next) {
        if (strcmp(cur->brand_name, name) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * Given the head of a linked list of FriendNode, this function inserts
 * a given user into the linked list in alphabetical order of names. If
 * the user already exists, no modifications are made and the original
 * list is returned. Returns the head of the new linked list after insertion.
 *
 * @param head Pointer to the head of the linked list of FriendNode.
 * @param node Pointer to the user to be inserted into the list.
 * @return Pointer to the head of the new linked list after insertion.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node) {
    if (node == NULL) return head;

    if (in_friend_list(head, node)) {
        printf("User already in list\n");
        return head;
    }

    FriendNode *fn = calloc(1, sizeof(FriendNode));
    fn->user = node;
    fn->next = NULL;

    if (head == NULL) return fn;

    if (strcmp(head->user->name, node->name) > 0) {
        fn->next = head;
        return fn;
    }

    FriendNode *cur;
    for (cur = head;
         cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
         cur = cur->next)
        ;

    fn->next = cur->next;
    cur->next = fn;

    return head;
}

/**
 * Given the head of a linked list of BrandNode, this function inserts
 * a given brand into the linked list in alphabetical order of names. If
 * the brand already exists, no modifications are made and the original
 * list is returned. Returns the head of the new linked list after insertion.
 *
 * @param head Pointer to the head of the linked list of BrandNode.
 * @param node Pointer to the brand name to be inserted into the list.
 * @return Pointer to the head of the new linked list after insertion.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node) {
    if (node == NULL) return head;

    if (in_brand_list(head, node)) {
        printf("Brand already in list\n");
        return head;
    }

    BrandNode *fn = calloc(1, sizeof(BrandNode));
    strcpy(fn->brand_name, node);
    fn->next = NULL;

    if (head == NULL) return fn;

    if (strcmp(head->brand_name, node) > 0) {
        fn->next = head;
        return fn;
    }

    BrandNode *cur;
    for (cur = head;
         cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
         cur = cur->next)
        ;

    fn->next = cur->next;
    cur->next = fn;

    return head;
}

/**
 * Given the head of a linked list of FriendNode, this function removes
 * a given user from the linked list. If the user does not exist in the list,
 * no modifications are made and the original list is returned. Returns the
 * head of the new linked list after deletion.
 *
 * @param head Pointer to the head of the linked list of FriendNode.
 * @param node Pointer to the user to be removed from the list.
 * @return Pointer to the head of the new linked list after deletion.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node) {
    if (node == NULL) return head;

    if (!in_friend_list(head, node)) {
        printf("User not in list\n");
        return head;
    }

    if (strcmp(head->user->name, node->name) == 0) {
        FriendNode *temp = head->next;
        free(head);
        return temp;
    }

    FriendNode *cur;
    for (cur = head; cur->next->user != node; cur = cur->next)
        ;

    FriendNode *temp = cur->next;
    cur->next = temp->next;
    free(temp);

    return head;
}

/**
 * Given the head of a linked list of BrandNode, this function removes
 * a given brand from the linked list. If the brand does not exist in the list,
 * no modifications are made and the original list is returned. Returns the
 * head of the new linked list after deletion.
 *
 * @param head Pointer to the head of the linked list of BrandNode.
 * @param node Pointer to the brand name to be removed from the list.
 * @return Pointer to the head of the new linked list after deletion.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node) {
    if (node == NULL) return head;

    if (!in_brand_list(head, node)) {
        printf("Brand not in list\n");
        return head;
    }

    if (strcmp(head->brand_name, node) == 0) {
        BrandNode *temp = head->next;
        free(head);
        return temp;
    }

    BrandNode *cur;
    for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
        ;

    BrandNode *temp = cur->next;
    cur->next = temp->next;
    free(temp);

    return head;
}

/**
 * Given a user, this function prints their name, friends, and liked brands.
 *
 * @param user Pointer to the user whose data is to be printed.
 */
void print_user_data(User *user) {
    printf("User name: %s\n", user->name);

    printf("Friends:\n");
    for (FriendNode *f = user->friends; f != NULL; f = f->next) {
        printf("   %s\n", f->user->name);
    }

    printf("Brands:\n");
    for (BrandNode *b = user->brands; b != NULL; b = b->next) {
        printf("   %s\n", b->brand_name);
    }
}

/**
 * Given a brand name, this function returns the index of the brand inside
 * the brand_names array. If the brand doesn't exist in the array, it
 * returns -1.
 *
 * @param name Pointer to the brand name.
 * @return The index of the brand in the brand_names array, or -1
 * if it doesn't exist.
 */
int get_brand_index(char *name) {
    for (int i = 0; i < MAT_SIZE; i++) {
        if (strcmp(brand_names[i], name) == 0) {
            return i;
        }
    }

    printf("Brand '%s' not found\n", name);
    return -1;  // Not found
}

/**
 * Given a brand name, this function prints its name, index inside the
 * brand_names array, and the names of other similar brands.
 *
 * @param brand_name Pointer to the brand name.
 */
void print_brand_data(char *brand_name) {
    int idx = get_brand_index(brand_name);
    if (idx < 0) {
        printf("Brand '%s' not in the list.\n", brand_name);
        return;
    }

    printf("Brand name: %s\n", brand_name);
    printf("Brand idx: %d\n", idx);

    printf("Similar brands:\n");
    for (int i = 0; i < MAT_SIZE; i++) {
        if (brand_adjacency_matrix[idx][i] == 1 &&
            strcmp(brand_names[i], "") != 0) {
            printf("   %s\n", brand_names[i]);
        }
    }
}

/**
 * Given a file name, this function reads from the file and populates the brand
 * list and brand matrix.
 *
 * @param file_name Pointer to the name of the file to be read.
 */
void populate_brand_matrix(char *file_name) {
    char buff[MAX_STR_LEN];
    FILE *f = fopen(file_name, "r");
    fscanf(f, "%s", buff);
    char *line = buff;
    for (int i = 0; i < MAT_SIZE; i++) {
        if (i == MAT_SIZE - 1) {
            strcpy(brand_names[i], line);
            break;
        }
        int index = strchr(line, ',') - line;
        strncpy(brand_names[i], line, index);
        line = strchr(line, ',') + sizeof(char);
    }
    for (int x = 0; x < MAT_SIZE; x++) {
        fscanf(f, "%s", buff);
        for (int y = 0; y < MAT_SIZE; y++) {
            int value = (int)buff[y * 2];
            if (value == 48) {
                value = 0;
            } else {
                value = 1;
            }
            brand_adjacency_matrix[x][y] = value;
        }
    }
}

/**
 * Given a name, this function creates a new user on the platform. The user is
 * then inserted into the allUsers linked list. If a user already existed with
 * the same name, no action is taken, and NULL is returned instead.
 *
 * @param name Pointer to the name of the user to be created.
 * @return Pointer to the created user, or NULL if a user with
 * the same name already exists.
 */
User *create_user(char *name) {
    for (FriendNode *cur = allUsers; cur != NULL; cur = cur->next) {
        if (strcmp(cur->user->name, name) == 0) {
            return NULL; // User already exists
        }
    }

    User *new_user = calloc(1, sizeof(User));
    strcpy(new_user->name, name);
    new_user->friends = NULL;
    new_user->brands = NULL;
    new_user->visited = false;

    FriendNode *new_node = calloc(1, sizeof(FriendNode));
    new_node->user = new_user;

    if (allUsers == NULL || strcmp(allUsers->user->name, name) > 0) {
        new_node->next = allUsers;
        allUsers = new_node;
    } else {
        FriendNode *current = allUsers;
        while (current->next != NULL && strcmp(current->next->user->name, name) < 0) {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    return new_user;
}



/**
 * Removes a given user from the platform. The user is removed from the allUsers
 * linked list and the friend lists of any users that they belong to. Returns 0
 * if the user was successfully removed. If the user does not exist, returns -1
 * instead.
 *
 * @param user Pointer to the user to be removed.
 * @return 0 if the user was successfully removed,
 * -1 if the user does not exist.
 */
int delete_user(User *user) {
    if (user == NULL) return -1;

    FriendNode **cur = &allUsers;
    while (*cur != NULL) {
        if ((*cur)->user == user) {
            FriendNode *temp = *cur;
            *cur = (*cur)->next;
            free(temp);
            return 0; // User found and deleted
        }
        cur = &((*cur)->next);
    }

    return -1; // User not found
}


/**
 * Given a pair of valid users, this function creates a friendship between them.
 * The user's friends list must remain in alphabetical order. Returns 0 if the
 * friendship was successfully created. Returns -1 if the pair were already
 * friends.
 *
 * @param user Pointer to the user initiating the friendship.
 * @param friend Pointer to the user to be added as a friend.
 * @return 0 if the friendship was successfully created,
 * -1 if the pair were already friends.
 */
int add_friend(User *user, User *friend) {
    if (user == NULL || friend == NULL) return -1;
    if (strcmp(user->name, friend->name) == 0) return -1; // Cannot be friends with oneself

    if (in_friend_list(user->friends, friend)) return -1; // Already friends

    user->friends = insert_into_friend_list(user->friends, friend);
    friend->friends = insert_into_friend_list(friend->friends, user); // Make the friendship mutual

    return 0;
}


/**
 * Given a pair of valid users, this function removes their friendship. The
 * user's friends list must remain in alphabetical order. Returns 0 if the pair
 * are no longer friends. Returns -1 if the pair were not friends to begin with.
 *
 * @param user Pointer to the user initiating the removal of friendship.
 * @param friend Pointer to the user to be removed as a friend.
 * @return 0 if the friendship was successfully removed,
 * -1 if the pair were not friends.
 */
int remove_friend(User *user, User *friend) {
    if (user == NULL || friend == NULL) return -1;

    if (!in_friend_list(user->friends, friend)) return -1; // Not friends

    user->friends = delete_from_friend_list(user->friends, friend);
    friend->friends = delete_from_friend_list(friend->friends, user); // Remove the friendship mutually

    return 0;
}


/**
 * Given a valid user and the name of a brand, this function creates a link
 * between the user and the brand. The user's brands list must remain in
 * alphabetical order. Returns 0 if the link was successfully created. Returns
 * -1 if the link already existed or if the brand name is invalid.
 *
 * @param user Pointer to the user.
 * @param brand_name Pointer to the name of the brand to follow.
 * @return 0 if the link was successfully created,
 * -1 if the link already existed or the brand name is invalid.
 */
int follow_brand(User *user, char *brand_name) {
    if (user == NULL || brand_name == NULL) return -1;
    if (in_brand_list(user->brands, brand_name)) return -1; 

    user->brands = insert_into_brand_list(user->brands, brand_name);
    return 0;
}



/**
 * Given a valid user and the name of a brand, this function removes the link
 * between the user and the brand. The user's brands list must remain in
 * alphabetical order. Returns 0 if the link was successfully removed. Returns
 * -1 if the link did not previously exist or if the brand name is invalid.
 *
 * @param user Pointer to the user.
 * @param brand_name Pointer to the name of the brand to unfollow.
 * @return 0 if the link was successfully removed, -1 if the link did not
 * previously exist or the brand name is invalid.
 */

int unfollow_brand(User *user, char *brand_name) {
    if (user == NULL || brand_name == NULL) return -1;
    if (!in_brand_list(user->brands, brand_name)) return -1;

    user->brands = delete_from_brand_list(user->brands, brand_name);
    return 0;
}

/**
 * Given a pair of valid users, this function returns the number of mutual
 * friends between them. A mutual friend is a user that exists in the friends
 * list of both User a and User b.
 *
 * @param a Pointer to the first user.
 * @param b Pointer to the second user.
 * @return The number of mutual friends between the two users.
 */
int get_mutual_friends(User *a, User *b) {
    int mutualCount = 0;
    for (FriendNode *curA = a->friends; curA != NULL; curA = curA->next) {
        if (in_friend_list(b->friends, curA->user)) {
            mutualCount++;
        }
    }
    return mutualCount;
}



/**
 * Given a pair of valid users, this function returns the degree of connection
 * between both users. The degree of connection is the shortest number of steps
 * it takes to get from one user to the other. If a connection cannot be formed,
 * returns -1.
 *
 * @param a Pointer to the first user.
 * @param b Pointer to the second user.
 * @return The degree of connection between the two users, or -1 if no
 * connection can be formed.
 */
int get_degrees_of_connection(User *a, User *b) {
    if (a == NULL || b == NULL) return -1;
    if (strcmp(a->name, b->name) == 0) return 0; // Same user

    for (FriendNode *cur = allUsers; cur != NULL; cur = cur->next) {
        cur->user->visited = false;
    }

    FriendNode *queue = NULL;
    a->visited = true;
    queue = insert_into_friend_list(queue, a);
    int degree = 0;

    while (queue != NULL) {
        int levelSize = 0;
        for (FriendNode *temp = queue; temp != NULL; temp = temp->next) {
            levelSize++;
        }
        
        while (levelSize--) {
            User *s = queue->user;
            queue = delete_from_friend_list(queue, s);

            if (strcmp(s->name, b->name) == 0) {
                while (queue != NULL) {
                    queue = delete_from_friend_list(queue, queue->user);
                }
                return degree;
            }

            for (FriendNode *f = s->friends; f != NULL; f = f->next) {
                if (!f->user->visited) {
                    f->user->visited = true;
                    queue = insert_into_friend_list(queue, f->user);
                }
            }
        }
        degree++;
    }

    return -1;
}


/**
 * Given two brand names, this function marks the two brands as similar in the
 * brand_adjacency_matrix variable. If either brand name is invalid, no action
 * is taken.
 *
 * @param brandNameA Pointer to the first brand name.
 * @param brandNameB Pointer to the second brand name.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB) {
    int indexA = get_brand_index(brandNameA);
    int indexB = get_brand_index(brandNameB);
    if (indexA >= 0 && indexB >= 0) {
        brand_adjacency_matrix[indexA][indexB] = 1;
        brand_adjacency_matrix[indexB][indexA] = 1; 
    }
}




/**
 * Given two brand names, this function marks the two brands as different (not
 * similar) in the brand_adjacency_matrix variable. If either brand name is
 * invalid, no action is taken.
 *
 * @param brandNameA Pointer to the first brand name.
 * @param brandNameB Pointer to the second brand name.
 */
void remove_similar_brands(char *brandNameA, char *brandNameB) {
    int indexA = get_brand_index(brandNameA);
    int indexB = get_brand_index(brandNameB);
    if (indexA >= 0 && indexB >= 0) {
        brand_adjacency_matrix[indexA][indexB] = 0;
        brand_adjacency_matrix[indexB][indexA] = 0; 
    }
}

/**
 * Given a user, suggest a new friend for them.
 *
 * To find the best match, the new suggested friend should have the highest
 * number of mutually liked brands amongst all other valid candidates.
 *
 * If a tie needs to be broken, select the user with the name that comes first
 * in reverse-alphanumerical order.
 *
 * The suggested friend must be a valid user, cannot be the user themself, nor
 * cannot be someone that they're already friends with.
 *
 * If the user is already friends with everyone on the platform, returns NULL.
 *
 * At times, a similarity rating of 0 is the best option.
 *
 * @param user Pointer to the user for whom a friend is being suggested.
 * @return Pointer to the suggested friend, or NULL if no suitable friend is
 * found.
 */
User *get_suggested_friend(User *user) {
    int maxBrandsInCommon = -1;
    User *suggestedFriend = NULL;

    for (FriendNode *cur = allUsers; cur != NULL; cur = cur->next) {
        User *potentialFriend = cur->user;
        if (strcmp(potentialFriend->name, user->name) == 0 || in_friend_list(user->friends, potentialFriend)) {
            continue;
        }

        int brandsInCommon = 0;
        for (BrandNode *b = user->brands; b != NULL; b = b->next) {
            if (in_brand_list(potentialFriend->brands, b->brand_name)) {
                brandsInCommon++;
            }
        }

        if (brandsInCommon > maxBrandsInCommon ||
            (brandsInCommon == maxBrandsInCommon && suggestedFriend && strcmp(potentialFriend->name, suggestedFriend->name) > 0)) {
            maxBrandsInCommon = brandsInCommon;
            suggestedFriend = potentialFriend;
        }
    }

    return suggestedFriend;
}


/**
 * Given a user and a positive integer n, this function adds n suggested friends
 * using the get_suggested_friend() function. There might not be enough users on
 * the platform to satisfy n completely, so it returns the amount of friends
 * successfully added.
 *
 * @param user Pointer to the user for whom suggested friends are being added.
 * @param n The number of suggested friends to add.
 * @return The number of friends successfully added.
 */
int add_suggested_friends(User *user, int n) {
    int friendsAdded = 0;
    while (friendsAdded < n) {
        User *suggestedFriend = get_suggested_friend(user);
        if (suggestedFriend == NULL) break; // No more suitable suggestions

        if (add_friend(user, suggestedFriend) == 0) {
            friendsAdded++;
        } else {
            break;
        }
    }
    return friendsAdded;
}


/**
 * Given a user and a positive integer n, this function suggests and follows n
 * new brands for them.
 *
 * To find the best matches, suggested brands have the most similarities with
 * the brands that the user already follows.
 *
 * If a tie needs to be broken, the brands with the names that comes first in
 * reverse-alphanumerical order is selected.
 *
 * The suggested brand must be a valid brand and cannot be a brand that the user
 * already follows.
 *
 * There might not be enough brands on the platform to satisfy n completely, so
 * the function returns the amount of brands successfully followed.
 *
 * At times, a similarity rating of 0 is the best option.
 *
 * @param user Pointer to the user for whom brands are being suggested.
 * @param n The number of suggested brands to follow.
 * @return The number of brands successfully followed.
 */
int follow_suggested_brands(User *user, int n) {
    int brandsFollowed = 0;
    int similarityScores[MAT_SIZE] = {0};
    for (BrandNode *b = user->brands; b != NULL; b = b->next) {
        int brandIndex = get_brand_index(b->brand_name);
        if (brandIndex == -1) continue; // Skip if brand is not found
        
        for (int i = 0; i < MAT_SIZE; i++) {
            if (brand_adjacency_matrix[brandIndex][i] == 1 && !in_brand_list(user->brands, brand_names[i])) {
                similarityScores[i]++;
            }
        }
    }

    for (int i = 0; i < n && brandsFollowed < n; i++) {
        int maxScore = -1;
        int brandToFollow = -1;
        for (int j = 0; j < MAT_SIZE; j++) {
            if (similarityScores[j] > maxScore || (similarityScores[j] == maxScore && brandToFollow != -1 && strcmp(brand_names[j], brand_names[brandToFollow]) > 0)) {
                maxScore = similarityScores[j];
                brandToFollow = j;
            }
        }

        if (brandToFollow != -1) {
            follow_brand(user, brand_names[brandToFollow]);
            brandsFollowed++;
            similarityScores[brandToFollow] = -1;
        } else {
            break;
        }
    }

    return brandsFollowed;
}

