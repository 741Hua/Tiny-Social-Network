/* the main() program for project 3.
   01: read in input file, not yet store user information yet.
   02: add the user choice menu and read in user choice.
	Do nothing. Enter 0 as choice to terminate the code.
   03: move user choice menu printing to a function in MyTSN.c and test
	separate compilation and linking. 
       gcc -c MyTSN.c -o MyTSN.o
       gcc p3_03.c MyTSN.o
   04: create the user database and add new users into it from input file
	Note: new users are addes at the head, so first come last out.
	The program prints out user IDs to show that users have been added
	to the linked list of all users successfully.
   05: add new user by function make_user()
   06: add function copy_user which will be used in friend list. 
   07: add friendship to both users in each friendship pair
	user->head CHANGED TO user->friend FOR BETTER READABILITY
	MyTSN.h and MyTSN.c are changed, previous versions of p3.c are not.
   08: use functions add_user() and  add_friendship() to build the database.
	add functions make_root() for AllUser and keep_paths() for AllPath.
	add new structures in MyTSN.h for the shortest path between two users
		NODE, PATH, PATH_ROOT
 	and the associated functions to initalize these varilables

   p3.c: this version, a working version with the all the user options 
	implemented. You need to complete the functions in the MyTSN.c and 
	then do the separate compilation and link to make it work. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "MyTSN.h"

#define NEW(x) (x*)malloc(sizeof(x))

int main(int argc, char **argv) 
{   srand(time(NULL));
	
  if(argc != 3) {
    printf("Requires two filenames. Try:  a.out user_database.txt update.txt\n");
    exit(0);
  }

  FILE *fi, *fo;

  fi = fopen(argv[1], "r");
  if(fi == NULL) {
	printf("Error opening file: %s\n", argv[1]);
	exit(0);
  }
  fo = fopen(argv[2], "w");
  if(fo == NULL) {
	printf("Error opening file: %s\n", argv[2]);
	exit(0);
  }

  int choice, ctr=0;		// choice: user option (0-7)
				// ctr: counter for incorrect user input
  int id1, id2;			// temp variables for UIDs from input file
  int i, j, k;
  char ch;
  USER *Tom, *Jerry, *u1, *u2;	// pointers to temp users
  ROOT *AllUser;		// root of the linked list of all users
  PATH_ROOT *AllPath;		// root of all the paths that will be queried 

  AllUser = make_root();
  AllPath = keep_paths();
	
  //Read input file
  while(fscanf(fi, "%d%c", &id1, &ch) != EOF)
  { if (ch == '#')
    { add_user(id1, AllUser);
				// add new user to AllUser at head
    }
    else 
    { fscanf(fi, "%d", &id2);
      add_friendship(id1, id2, AllUser);
				// add the new friendship to the database
    }
  } // end of while, reach EOF of input file

				// this is to test the database, can be removed.
  for (Tom = AllUser->head; Tom != NULL; Tom = Tom->next)
  { printf("%06d\t%s\n", Tom->uid, Tom->password);
    printf("%d Friends: ", Tom->number);
    for (Jerry = Tom->friend; Jerry != NULL; Jerry = Jerry->friend)
      printf("%06d ", Jerry->uid);
    printf("\n");
  } 
 
//  assignPassword(AllUser);

  printf("Welcome to MyTSN!\n");
    do
    { printMenu();
        scanf("%d", &choice);

        switch(choice)
        {   case 0:
                printf("\n\nThank you for using MyTSN!\n\n");
                writeOutput(fo, AllUser, AllPath);
                break;
            case 1:
                printf("Enter the user ID: "); 
                scanf("%d", &id1);
                Tom = search_user(AllUser, id1);	
                if (Tom == NULL) 
                    printf("User %06d not found.\n", id1);
                else 
                    printf("User %06d found.\n", id1);
                break;
            case 2: 
                i = 0; 			// 3 attempts to enter an existing uid
                Tom = NULL;
                do 
                {   printf("Enter the user ID: ");
                    scanf("%d", &id1);
                    Tom = search_user(AllUser, id1);
      		        if (Tom == NULL)
                        printf("User %06d does not exit.\n", id1);
                    i++;
                } while (Tom == NULL && i < 3);
    	  	    if (Tom != NULL)
                    print_Friends(Tom);
                break;
            case 3: 
                printf("Enter the user IDs of the two friends: ");
                scanf("%d %d", &id1, &id2);
                add_a_friend(AllUser, id1, id2);
                break;
            case 4: 
                i = 0;                    // 3 attempts to enter a valid uid
                j = 0;			// change to 1 when a valid uid is entered
                do
                {   printf("Enter the user ID: ");
                    scanf("%d", &id1);
                    if (id1 < 0 || id1 > 999999)
                    {   printf("%d can only have up to 6 digits.\n", id1);
                        i++;
                        continue;
                    }
                    Tom = search_user(AllUser, id1);
                    if (Tom != NULL) 
                    {   printf("%06d already exists.\n", id1);
                        i++;
                    }
                    else 	
                    {   j = 1;		// a valid uid is obtained
                        break;
                    }
                } while(j==0 && i<3);

                add_user(id1, AllUser);
                break;
            case 5: 
                printf("Enter the user IDs of the two friends: ");
                scanf("%d %d", &id1, &id2);
                i = remove_friendship(AllUser, id1, id2);
                if (i==0)
                printf("Users %06d and %06d are not friends.\n", id1, id2);
                if (i==1)
                printf("Users %06d and %06d are no longer friends.\n", id1, id2);
                break;
            case 6:
                printf("Enter the user ID: ");
                scanf("%d", &id1);
                remove_user(AllUser, id1);
                break;
            case 7:
                printf("Enter the user IDs of the two users to be connected: ");
                scanf("%d %d", &id1, &id2);
                find_path(AllUser, id1, id2, AllPath);
                break;
            default:
                printf("Invalid choice!\n"); 
        }
      } while(choice != 0);

  return 0;
}
