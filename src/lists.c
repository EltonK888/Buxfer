#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    Group *new_group;
    new_group = malloc(sizeof(Group));
    if (new_group == NULL) { // exit if malloc fails
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    new_group->name = malloc((strlen(group_name)+1)*sizeof(char)); // initialize the new group with its name
    if (new_group->name == NULL) {
        perror("malloc");
        free(new_group);
        exit(EXIT_FAILURE);
    }
    strncpy(new_group->name, group_name, strlen(group_name)+1);
    new_group->next = NULL;
    new_group->xcts = NULL;
    new_group->users = NULL;
    Group *cur = *group_list_ptr; // get the address of the head by dereferencing the double pointer arg
    if (cur == NULL) { // if no groups in the group_list_ptr then make the new_group the head
        *group_list_ptr = new_group;
        return 0;
    }
    while(cur->next != NULL) { // find if a group with same name already exists
        if (strcmp(cur->name, group_name) == 0) { // if a group with the same name already exists, free the group we created and return -1
            free(new_group->name);
            free(new_group);
            return -1;
        }
        else { // keep searching until end of list
            cur = cur->next;
        }
    }
    // if we get here then no group of group_name was found
    cur->next = new_group; // add the group to the end of the list and return 0
    return 0;
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {
    Group *head = group_list;
    if (head == NULL) { // if there are no groups
        return;
    }
    while(head != NULL) { // print the group names until end of the list
        char *cur_name = malloc((strlen(head->name)+1)*sizeof(char));
        if (cur_name == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(cur_name, head->name, strlen(head->name)+1); // copy the name of the group
        printf("%s\n", cur_name);
        head = head->next;
        free(cur_name);
    }
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    Group *head = group_list;
    while( head != NULL ) { // search until end of list for the group_name
        if (strcmp(group_name, head->name) == 0) { // if we find the group_name, return the Group
            return head;
        }
        else {
            head = head->next;
        }
        
    }
    return NULL; // if searched entire list and can't find group_name, return NULL
}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
    char *uname = malloc((strlen(user_name)+1)*sizeof(char));
    if (uname == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    strncpy(uname, user_name, strlen(user_name)+1);
    if (group == NULL) { // if the group is empty/uninitialized
        printf("Group is does not exist, cannot add user\n");
        return -1;
    }
    User *prev = find_prev_user(group, user_name); // check to see if a user with the user_name exists in the group
    if (prev != NULL) { // found a user with the same name that exists in the group
        free(uname);
        return -1;
    }
    else { // no user with the name exists, add it to the front of the user list with balance of 0.0
        User *new_user;
        new_user = malloc(sizeof(User));
        if (new_user == NULL) {
            perror("malloc");
            free(uname);
            exit(EXIT_FAILURE);
        }
        new_user->name = uname;
        new_user->balance = 0.0;
        User *temp = group->users;
        group->users = new_user;
        new_user->next = temp;
        return 0;
    }

}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    User *prev = find_prev_user(group, user_name);
    if (prev == NULL) { //the user doesn't exist in the group
       return -1;
    }
    else {
        if (strcmp(prev->name, user_name) == 0) { //if the user we want to remove is the first in the user list
            group->users = prev->next; // set the new head of the user list
            remove_xct(group, user_name);
            free(prev->name);
            free(prev);
            return 0;
        }
        else { // if the user is not at beginning of the list
            User *to_remove = prev->next; // make the prev node point to next, next node
            prev->next = to_remove->next;
            remove_xct(group, user_name);
            free(to_remove->name);
            free(to_remove);
            return 0;
        }
    }
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
    if (group == NULL) { // if the group is uninitialized
       return; 
    } 
    User *cur_usr = group->users;
    if (cur_usr == NULL) { // if the user list is empty
        return;
    }
    while (cur_usr != NULL) { // print the user name until the end
        char *uname = malloc(strlen(cur_usr->name)+1 * sizeof(char));
        if (uname == NULL) {
            perror("malloc");
            exit(EXIT_FAILURE);
        }
        strncpy(uname, cur_usr->name, strlen(cur_usr->name)+1);
        printf("%s %.2f\n", uname, cur_usr->balance);
        free(uname);
        cur_usr = cur_usr->next;
    }

}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {
    User *prev = find_prev_user(group, user_name);
    if (prev == NULL) { // if the user doesn't exist in the group
        return -1;
    }
    else {
        if (strcmp(prev->name, user_name) == 0) { // user that we want to print is the first in the group
            printf("%.2f\n", prev->balance);
        }
        else { // the user is prev->next, so print their balance
            User *user_to_print = prev->next;
            printf("%.2f\n", user_to_print->balance);
        }
        return 0;
    }
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
    User *cur_user = group->users;
    if (cur_user == NULL) { // the user list is empty so return -1
        return -1;
    }
    double min_bal = cur_user->balance; // the first user has the minimum balance so store it
    while (cur_user != NULL && cur_user->balance == min_bal) { // find all the users that have only paid the min balance
        printf("%s\n", cur_user->name);
        cur_user = cur_user->next;
    }
    return 0;
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {
    Group *current;
    current = group;
    if (current == NULL || current->users == NULL) { // if there user list or the group is empty
        return NULL;
    }
    User *cur_user;
    cur_user = current->users;
    if (strcmp(cur_user->name, user_name) == 0){ // if the first user is the user we're looking for
        return cur_user;
    }
    else { // else then look through the rest of the list
        while (cur_user->next != NULL) {
            if (strcmp(cur_user->next->name, user_name) == 0){ // if the next user's name is user_name
                return cur_user; // then return the current user
            }
            else { // if not then keep looking through the list
                cur_user = cur_user->next;
            }
        }
    }
    return NULL; // if we reach the end of the list
}

void sort_user(Group *group, User *user) {
    /* helper function that will take a user and place it into the right order in the user list.
    this function is to be used as a call in add_xct. We assume the list is already sorted except
    for this new user who has had their balance updated. */
    User *prev = find_prev_user(group, user->name);
    if (!(strcmp(prev->name, user->name) == 0)) { // if the user is not at the head, we need to remove it from the user list order
        prev->next = user->next;
    }
    else {
        group->users = user->next; // user is at the head, remove it from the user linked list
    }
    User *cur_user = group->users;
    if (cur_user == NULL) { // only way cur_user would be NULL is if user is the only user in the list so don't have to change anything
        group->users = user;
        return;
    }
    if (cur_user->balance > user->balance) { // if the head of the user list has a balance greater than the user we're sorting, then make the new user the head
        group->users = user;
        user->next = cur_user;
        return;
    }
    while (cur_user->next != NULL && cur_user->next->balance < user->balance) { // find where to insert the user
        cur_user = cur_user->next;
    }
    if (cur_user->next == NULL) { // case where we insert the user to the back of the list
        cur_user->next = user;
        user->next = NULL;
    }
    else { // else, we're inserting somewhere in the middle of the user list
        User *temp = cur_user->next; // insert the user and make it point to the right next user
        cur_user->next = user;
        user->next = temp;
    }
    return;

}
/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
    User *prev = find_prev_user(group, user_name);
    if (prev == NULL) { // if the user doesn't exit in the group then return -1
        return -1;
    }
    Xct *new_xct = malloc(sizeof(Xct));
    if (new_xct == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    // make a new xct and insert it at the front of the xct list
    // xct are sorted by time in which transaction was added
    char *uname = malloc((strlen(user_name)+1)*sizeof(char));
    if (uname == NULL) {
        perror("malloc");
        free(new_xct);
        exit(EXIT_FAILURE);
    }
    strncpy(uname, user_name, strlen(user_name)+1); 
    new_xct->name = uname;
    new_xct->amount = amount;
    Xct *temp = group->xcts;
    new_xct->next = temp;
    group->xcts = new_xct;
    if (strcmp(prev->name, user_name) == 0) { // if the user is at the head of the user list
        prev->balance += amount; // update their balance
        sort_user(group, prev); // sort the user list to reflect balance changes
    }
    else { // user is not at head of the list, update their balance
        prev = prev->next;
        prev->balance += amount;
        sort_user(group, prev); // sort the user list to reflect balance changes
    }
    return 0;
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
    Xct *cur_xct = group->xcts;
    long counter = 0;
    // loop until the end of the list or until we've printed num_xct xcts. which ever comes first
    while (cur_xct != NULL && counter<nu_xct) { 
        printf("%s %.2f\n", cur_xct->name, cur_xct->amount);
        // increment the counter and move to the next xct
        cur_xct = cur_xct->next;
        ++counter;
    }
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/
void remove_xct(Group *group, const char *user_name) {
    Xct *prev = group->xcts;
    if (prev == NULL) { // if no xcts, then do nothing
        return;
    }
    Xct *cur_xct = prev->next;
    if (cur_xct == NULL) { // this means there's only 1 xct in the xct list
        if (strcmp(prev->name, user_name) == 0) { // the only xct in the list is the one to remove
            free(prev->name);
            free(prev);
            group->xcts = NULL;
        }
    }
    else { // if we get here, there is more than 2 xct in the xct list
        if (group->xcts == prev) { // we're at the head of the xct
            /* if the xct we want to remove is at the head of the list,
            remove until we see a different user or until the end */
            while (prev != NULL && (strcmp(prev->name, user_name) == 0)) {
                cur_xct = prev;
                prev = prev->next;
                free(cur_xct->name);
                free(cur_xct);
                group->xcts = prev;
            }
            if (prev == NULL) { // if prev is NULL, we got to the end of the xct_list so return from the function
                return;
            }
            else { // else, we still have to check the remaining xcts, so set cur_xct to the next xct
                cur_xct = prev->next;
            }
        }
        while (cur_xct != NULL) { // check all xcts until the end of the list
                if (strcmp(cur_xct->name, user_name) == 0) { // if the current xct is the one we want to remove
                    // set the next node accordingly and free the current xct
                    prev->next = cur_xct->next;
                    free(cur_xct->name);
                    free(cur_xct);
                    cur_xct = prev->next;
                }
                else { // else, move both pointers
                    prev = cur_xct;
                    cur_xct = cur_xct->next;
                }
        }
    }
}
