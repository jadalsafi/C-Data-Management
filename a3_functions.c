/*****************
    Student Name   = Jad Al Safi
    Student Number = 101334255
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>
#include <ctype.h>
#include "a3_nodes.h"
#include "a3_functions.h"

// Helper: convert a string to lowercase (in place)
void to_lowercase(char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        str[i] = (char)tolower((unsigned char)str[i]);
    }
}

/*
   Function that creates a new user and adds it to a sorted (ascending order) linked list at
   the proper sorted location. Return the head of the list.
*/
user_t *add_user(user_t *users, const char *username, const char *password)
{
    if (username == NULL || password == NULL)
    {
        return users;
    }

    user_t *new_user = (user_t *)malloc(sizeof(user_t));
    if (new_user == NULL)
    {
        // Allocation failed; return original list
        return users;
    }

    // Copy username and password safely
    strncpy(new_user->username, username, USERNAME_LEN - 1);
    new_user->username[USERNAME_LEN - 1] = '\0';
    to_lowercase(new_user->username);

    strncpy(new_user->password, password, PASSWORD_LEN - 1);
    new_user->password[PASSWORD_LEN - 1] = '\0';

    new_user->friends = NULL;
    new_user->posts = NULL;
    new_user->next = NULL;

    // Insert into sorted position by username (ascending)
    if (users == NULL || strcmp(new_user->username, users->username) < 0)
    {
        new_user->next = users;
        return new_user;
    }

    user_t *prev = users;
    user_t *curr = users->next;

    while (curr != NULL && strcmp(new_user->username, curr->username) > 0)
    {
        prev = curr;
        curr = curr->next;
    }

    // Insert between prev and curr
    prev->next = new_user;
    new_user->next = curr;

    return users;
}

/*
   Function that searches if the user is available in the database 
   Return a pointer to the user if found and NULL if not found.
*/
user_t *find_user(user_t *users, const char *username)
{
    if (username == NULL)
    {
        return NULL;
    }

    char lowered[USERNAME_LEN];
    strncpy(lowered, username, USERNAME_LEN - 1);
    lowered[USERNAME_LEN - 1] = '\0';
    to_lowercase(lowered);

    for (user_t *u = users; u != NULL; u = u->next)
    {
        if (strcmp(u->username, lowered) == 0)
        {
            return u;
        }
    }

    return NULL;
}

/*
   Function that creates a new friend's node.
   Return the newly created node.
*/
friend_t *create_friend(const char *username)
{
    if (username == NULL)
    {
        return NULL;
    }

    friend_t *new_friend = (friend_t *)malloc(sizeof(friend_t));
    if (new_friend == NULL)
    {
        return NULL;
    }

    strncpy(new_friend->username, username, USERNAME_LEN - 1);
    new_friend->username[USERNAME_LEN - 1] = '\0';
    to_lowercase(new_friend->username);
    new_friend->next = NULL;

    return new_friend;
}

/*
   Function that links a friend to a user. The friend's name should be added into
   a sorted (ascending order) linked list.
*/
void add_friend(user_t *user, const char *friend)
{
    if (user == NULL || friend == NULL)
    {
        return;
    }

    friend_t *new_friend = create_friend(friend);
    if (new_friend == NULL)
    {
        return;
    }

    // Insert into sorted friend list
    if (user->friends == NULL ||
        strcmp(new_friend->username, user->friends->username) < 0)
    {
        new_friend->next = user->friends;
        user->friends = new_friend;
        return;
    }

    friend_t *prev = user->friends;
    friend_t *curr = user->friends->next;

    while (curr != NULL &&
           strcmp(new_friend->username, curr->username) > 0)
    {
        prev = curr;
        curr = curr->next;
    }

    prev->next = new_friend;
    new_friend->next = curr;
}

/*
   Function that removes a friend from a user's friend list.
   Return true if the friend was deleted and false otherwise.
*/
_Bool delete_friend(user_t *user, char *friend_name)
{
    if (user == NULL || friend_name == NULL || user->friends == NULL)
    {
        return false;
    }

    char lowered[USERNAME_LEN];
    strncpy(lowered, friend_name, USERNAME_LEN - 1);
    lowered[USERNAME_LEN - 1] = '\0';
    to_lowercase(lowered);

    friend_t *curr = user->friends;
    friend_t *prev = NULL;

    while (curr != NULL && strcmp(curr->username, lowered) != 0)
    {
        prev = curr;
        curr = curr->next;
    }

    if (curr == NULL)
    {
        return false; // Not found
    }

    if (prev == NULL)
    {
        // Removing head
        user->friends = curr->next;
    }
    else
    {
        prev->next = curr->next;
    }

    free(curr);
    return true;
}

/*
   Function that creates a new user's post.
   Return the newly created post.
*/
post_t *create_post(const char *text)
{
    if (text == NULL)
    {
        return NULL;
    }

    post_t *new_post = (post_t *)malloc(sizeof(post_t));
    if (new_post == NULL)
    {
        return NULL;
    }

    strncpy(new_post->content, text, POST_LEN - 1);
    new_post->content[POST_LEN - 1] = '\0';
    new_post->next = NULL;

    return new_post;
}

/*
   Function that adds a post to a user's timeline following a stack.
*/
void add_post(user_t *user, const char *text)
{
    if (user == NULL || text == NULL)
    {
        return;
    }

    post_t *new_post = create_post(text);
    if (new_post == NULL)
    {
        return;
    }

    // Stack: push on top (head)
    new_post->next = user->posts;
    user->posts = new_post;
}

/*
   Function that removes a post from a user's list of posts.
   Return true if the post was deleted and false otherwise.
*/
_Bool delete_post(user_t *user)
{
    if (user == NULL || user->posts == NULL)
    {
        return false;
    }

    post_t *to_delete = user->posts;
    user->posts = to_delete->next;
    free(to_delete);

    return true;
}

/*
   Function that  displays a specific user's posts (All posts)
*/
void display_all_user_posts(user_t *user)
{
    if (user == NULL)
    {
        return;
    }

    if (user->posts == NULL)
    {
        printf("No posts available for %s\n", user->username);
        return;
    }

    printf("-----------------------------------------------------------------------\n");
    printf("%s's posts\n", user->username);
    int i = 1;
    for (post_t *p = user->posts; p != NULL; p = p->next)
    {
        printf("%d - %s\n", i, p->content);
        i++;
    }
    printf("-----------------------------------------------------------------------\n");
}

/*
   Function that displays a specific user's friends (All friends)
*/
void display_user_friends(user_t *user)
{
    if (user == NULL)
    {
        return;
    }

    if (user->friends == NULL)
    {
        printf("%s has no friends.\n", user->username);
        return;
    }

    printf("List of %s's friends:\n", user->username);
    int i = 1;
    for (friend_t *f = user->friends; f != NULL; f = f->next)
    {
        printf("%d- %s\n", i, f->username);
        i++;
    }
}

/*
   Function that displays "number" of posts for a given user.
   After displaying "number" posts, it prompts if you want to display
   more posts.
   If there are no more post or the user types “n” or “N”, the function returns. 
*/
void display_posts_by_n(user_t *user, int number)
{
    if (user == NULL || number <= 0)
    {
        return;
    }

    // Count posts
    int count = 0;
    for (post_t *p = user->posts; p != NULL; p = p->next)
    {
        count++;
    }

    if (count == 0)
    {
        printf("No posts available for %s\n", user->username);
        return;
    }

    // Store pointers to posts in an array so we can display in chunks
    post_t **posts_array = (post_t **)malloc(sizeof(post_t *) * count);
    if (posts_array == NULL)
    {
        return;
    }

    int idx = 0;
    for (post_t *p = user->posts; p != NULL; p = p->next)
    {
        posts_array[idx++] = p;
    }

    int current = 0;
    char answer;

    while (current < count)
    {
        printf("-----------------------------------------------------------------------\n");
        printf("%s's posts\n", user->username);

        int printed = 0;
        while (printed < number && current < count)
        {
            printf("%d - %s\n", current + 1, posts_array[current]->content);
            current++;
            printed++;
        }

        printf("-----------------------------------------------------------------------\n");

        if (current >= count)
        {
            printf("All posts have been displayed.\n");
            break;
        }

        printf("Do you want to display more posts? (Y/N): ");
        scanf(" %c", &answer);

        if (answer != 'Y' && answer != 'y')
        {
            break;
        }
    }

    free(posts_array);
}

/*
   Function that free all users from the database before quitting the application.
*/
void teardown(user_t *users)
{
    while (users != NULL)
    {
        user_t *current_user = users;
        users = users->next;

        // Free friends list
        friend_t *f = current_user->friends;
        while (f != NULL)
        {
            friend_t *next_f = f->next;
            free(f);
            f = next_f;
        }

        // Free posts list
        post_t *p = current_user->posts;
        while (p != NULL)
        {
            post_t *next_p = p->next;
            free(p);
            p = next_p;
        }

        free(current_user);
    }
}

/*
   Function that prints the main menu with a list of options for the user to choose from
*/
void print_menu()
{
    printf("***********************************************\n");
    printf("MAIN MENU:\n");
    printf("***********************************************\n");
    printf("1. Register a new user\n");
    printf("2. Manage a user's profile (change password)\n");
    printf("3. Manage a user's posts (add/remove)\n");
    printf("4. Manage a user's friends (add/remove)\n");
    printf("5. Display a user's posts\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

/*
   ******** DONT MODIFY THIS FUNCTION ********
   Function that reads users from the text file.
   IMPORTANT: This function shouldn't be modified and should be used as is
   ******** DONT MODIFY THIS FUNCTION ********
*/
user_t *read_CSV_and_create_users(FILE *file, int num_users)
{
    srand(time(NULL));
    user_t *users = NULL;
    char buffer[500];
    fgets(buffer, sizeof(buffer), file); // Read and discard the header line
    int count = 0;
    for (int i = 0; i < num_users; i++)
    {
        if (fgets(buffer, sizeof(buffer), file) == NULL)
        {
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0; // Remove newline characters

        char *token = strtok(buffer, ",");
        if (token == NULL)
        {
            continue;
        }

        char *token2 = strtok(NULL, ",");
        if (token2 == NULL)
        {
            continue;
        }

        users = add_user(users, token, token2);
        char *username = token;

        token = strtok(NULL, ",");

        user_t *current_user = users;
        for (; current_user != NULL && strcmp(current_user->username, username) != 0; current_user = current_user->next)
            ;

        while (token != NULL && strcmp(token, ",") != 0 && count < 3)
        {
            if (strcmp(token, " ") != 0)
            {
                add_friend(current_user, token);
            }
            token = strtok(NULL, ",");
            count++;
        }
        count = 0;

        while (token != NULL && strcmp(token, ",") != 0)
        {
            add_post(current_user, token);
            token = strtok(NULL, ",");
        }
    }
    return users;
}
