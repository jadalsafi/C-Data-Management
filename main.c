/********* main.c ********
    Student Name   = Jad Al Safi
    Student Number = 101334255
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "a3_nodes.h"
#include "a3_functions.h"

int main(void)
{
    FILE *csv_file = fopen("user_details.csv", "r");
    if (csv_file == NULL)
    {
        perror("Error opening the CSV file");
        return 1;
    }

    // Parse CSV data and create users
    user_t *users = read_CSV_and_create_users(csv_file, 50);
    fclose(csv_file);

    printf("Welcome to Text-Based Facebook!\n");

    int choice = 0;

    while (1)
    {
        print_menu();
        if (scanf("%d", &choice) != 1)
        {
            // Clear invalid input
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        if (choice < 1 || choice > 6)
        {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        if (choice == 1)
        {
            // Register a new user
            char username[USERNAME_LEN];
            char password[PASSWORD_LEN];

            printf("Enter a username: ");
            scanf("%29s", username);

            // Check if user already exists
            if (find_user(users, username) != NULL)
            {
                printf("User already exists.\n");
            }
            else
            {
                int len = 0;
                do
                {
                    printf("Enter an up to 15 characters password: ");
                    scanf("%14s", password);
                    len = (int)strlen(password);
                    if (len < 8)
                    {
                        printf("Password must be at least 8 characters long.\n");
                    }
                } while (len < 8);

                users = add_user(users, username, password);
                printf("**** User Added! ****\n");
            }
        }
        else if (choice == 2)
        {
            // Manage a user's profile (change password)
            char username[USERNAME_LEN];
            char old_password[PASSWORD_LEN];
            char new_password[PASSWORD_LEN];

            printf("Enter a username to update their password: ");
            scanf("%29s", username);

            user_t *user = find_user(users, username);
            if (user == NULL)
            {
                printf("Error: user not found.\n");
            }
            else
            {
                printf("Enter the password: ");
                scanf("%14s", old_password);

                if (strcmp(user->password, old_password) != 0)
                {
                    printf("Error: incorrect password.\n");
                }
                else
                {
                    int len = 0;
                    do
                    {
                        printf("Enter a new password, up to 15 characters: ");
                        scanf("%14s", new_password);
                        len = (int)strlen(new_password);
                        if (len < 8)
                        {
                            printf("Password must be at least 8 characters long.\n");
                        }
                    } while (len < 8);

                    strncpy(user->password, new_password, PASSWORD_LEN - 1);
                    user->password[PASSWORD_LEN - 1] = '\0';

                    printf("**** Password changed! ****\n");
                }
            }
        }
        else if (choice == 3)
        {
            // Manage a user's posts (add/remove)
            char username[USERNAME_LEN];
            printf("Enter a username to manage their posts: ");
            scanf("%29s", username);

            user_t *user = find_user(users, username);
            if (user == NULL)
            {
                printf("Error: user not found.\n");
            }
            else
            {
                int post_choice = 0;
                do
                {
                    printf("-----------------------------------------------------------------------\n");
                    printf("Managing %s's posts\n", user->username);
                    printf("-----------------------------------------------------------------------\n");

                    if (user->posts == NULL)
                    {
                        printf("Note: No posts available for %s\n", user->username);
                    }
                    else
                    {
                        display_all_user_posts(user);
                    }

                    printf("1. Add a new post\n");
                    printf("2. Remove a post\n");
                    printf("3. Return to main menu\n");
                    printf("Enter your choice: ");

                    if (scanf("%d", &post_choice) != 1)
                    {
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF)
                            ;
                        printf("Invalid choice. Please try again.\n");
                        continue;
                    }

                    if (post_choice == 1)
                    {
                        char text[POST_LEN];
                        printf("Enter your post content: ");
                        // Read a full line of text (up to POST_LEN - 1 chars), including spaces
                        scanf(" %249[^\n]", text);
                        add_post(user, text);
                    }
                    else if (post_choice == 2)
                    {
                        if (delete_post(user))
                        {
                            printf("Latest post removed.\n");
                        }
                        else
                        {
                            printf("No posts to delete.\n");
                        }
                    }
                    else if (post_choice == 3)
                    {
                        // Return to main menu
                    }
                    else
                    {
                        printf("Invalid choice. Please try again.\n");
                    }

                } while (post_choice != 3);
            }
        }
        else if (choice == 4)
        {
            // Manage a user's friends (add/remove)
            char username[USERNAME_LEN];
            printf("Enter a username to update their friends: ");
            scanf("%29s", username);

            user_t *user = find_user(users, username);
            if (user == NULL)
            {
                printf("Error: user not found.\n");
            }
            else
            {
                int friend_choice = 0;
                do
                {
                    printf("-----------------------------------------------------------------------\n");
                    printf("Managing %s's friends\n", user->username);
                    printf("-----------------------------------------------------------------------\n");

                    printf("1. Add a new friend\n");
                    printf("2. Remove a friend\n");
                    printf("3. Return to main menu\n");
                    printf("Enter your choice: ");

                    if (scanf("%d", &friend_choice) != 1)
                    {
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF)
                            ;
                        printf("Invalid choice. Please try again.\n");
                        continue;
                    }

                    if (friend_choice == 1)
                    {
                        char friend_name[USERNAME_LEN];
                        printf("Enter a new friends' name: ");
                        scanf("%29s", friend_name);
                        add_friend(user, friend_name);
                        printf("Friend added to the list.\n");
                    }
                    else if (friend_choice == 2)
                    {
                        if (user->friends == NULL)
                        {
                            printf("No friends to delete.\n");
                        }
                        else
                        {
                            printf("List of %s's friends:\n", user->username);
                            display_user_friends(user);

                            char friend_name[USERNAME_LEN];
                            printf("Enter a friend's name to delete: ");
                            scanf("%29s", friend_name);

                            if (delete_friend(user, friend_name))
                            {
                                printf("Updated list of %s's friends\n", user->username);
                                display_user_friends(user);
                            }
                            else
                            {
                                printf("Error: friend not found.\n");
                            }
                        }
                    }
                    else if (friend_choice == 3)
                    {
                        // Return to main menu
                    }
                    else
                    {
                        printf("Invalid choice. Please try again.\n");
                    }

                } while (friend_choice != 3);
            }
        }
        else if (choice == 5)
        {
            // Display a user's posts (by chunks of 3)
            char username[USERNAME_LEN];
            printf("Enter a username to display their posts: ");
            scanf("%29s", username);

            user_t *user = find_user(users, username);
            if (user == NULL)
            {
                printf("Error: user not found.\n");
            }
            else
            {
                if (user->posts == NULL)
                {
                    printf("No posts available for %s\n", user->username);
                }
                else
                {
                    display_posts_by_n(user, 3);
                }
            }
        }
        else if (choice == 6)
        {
            // Exit
            printf("Goodbye!\n");
            teardown(users);
            return 0;
        }
    }

    // Should not reach here
    teardown(users);
    return 0;
}
