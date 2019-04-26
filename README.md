# Buxfer
A Simple implementation of Buxfer in C for a systems programming course. Checkout Buxfer at https://www.buxfer.com/

Buxfer is an application that allows people to track shared expenses.

It can be used to see how much an individual as paid for a certain expense.

Members of the group can look at the transaction histories and the member that owes the most.

This implementation of buxfer uses a linked list of `groups` that holds pointers to a linked list of `users` and `xcts`(transactions) for that group.

Structs of groups, xcts(transactions), and users.
```C
typedef struct group {
  char *name;
  struct user *users;
  struct xct *xcts;
  struct group *next;
} Group;

typedef struct user {
  char *name;
  double balance;
  struct user *next;
} User;

typedef struct xct {
  char *name;
  double amount;
  struct xct *next;
} Xct;
```

## Supported Methods
* `add_group <group name>` - Add a new group with name <group name>
* `list_groups` - List all groups
* `add_user <group name> <user name>` -Add a user of name <user name> to the group <group name>
* `remove_user <group name> <user name>` - Remove user of <user name> from the group <group name>
* `list_users <group name>` - List all users of the gruop
* `user_balance <group name> <user name>` - List balance of the user from the group <group name>
* `under_paid <group name>` - Lists the user that has paid the least in the group <group name>
* `add_xct <group name> <user name> <amount>` - Add a transaction to the group with the user that paid and the amount they paid
* `recent_xct <group name> <num xct>` - Lists the <num xct> most recent transactions. E.g. recent_xct friends 3 -> lists the 3 most recent transactions
* `quit` - to exit
  
## To Build
Call make in the `src` directory
```bash
make
```
To remove executables and object files call make clean in the `src` directory
```bash
make clean
```

## To Run
Buxfer can be run in two different modes.

1. When run with no arguments, users can start entering buxfer methods.
```bash
./buxfer
Welcome to Buxfer!
Please input command:
>add_group users
>list_groups
users
>quit
```
2. It can also be run with a `.txt` file that contains buxfer methods. Check out `batch_commands.txt` for all the buxfer commands.
```bash
./buxfer batch_commands.txt
Welcome to Buxfer!
Please input command:
>add_group instructors
>add_group friends
>add_group connections
>list_groups
instructors
friends
connections
```
