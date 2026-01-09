#ifndef A3_NODES_H
#define A3_NODES_H

/********* nodes.h ********
    Student Name    = Jad Al Safi
    Student Number  = 101334255
*/

/* Constants for username, password, and post content lengths */
#define USERNAME_LEN 30
#define PASSWORD_LEN 15
#define POST_LEN 250

/********** DON'T MODIFY **********/
// Structure to represent a linked list of users
typedef struct user
{
    char username[30];
    char password[15];
    struct friend *friends;
    struct post *posts;
    struct user *next;
} user_t;

// Structure to represent a linked list of a user's friends
typedef struct friend
{
    char username[30];
    struct friend *next;
}
friend_t;

// Structure to represent a linked list of a user's posts
typedef struct post
{
    char content[250];
    struct post *next;
} post_t;

#endif /* A3_NODES_H */
