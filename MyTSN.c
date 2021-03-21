
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "MyTSN.h"

void printMenu()
{  printf("\n1: search for a user\n"
             "2: list a user's friends\n"
             "3: add a new friend to a user\n"
             "4: add a new user\n"
             "5: remove a friendship\n"
             "6: remove a user\n"
             "7: find a connection\n"
             "0: EXIT\n\n"
             "Enter your choice (0-7):");
}

USER *make_user(int a)
{ USER *this;
  this = NEW(USER);
  if (this == NULL) 
    return this;
  this->uid = a;
  // replaced by password generator
  this->number = 0;
  this->next = NULL;
  this->friend = NULL;

  return this;
}

ROOT *make_root()
{ ROOT *this;
  this = NEW(ROOT);
  if (this == NULL)
    return this;
  this->head = NULL;
  this->number = 0;

  return this;
}


NODE *make_node(int id)
{ // add your code here
  NODE *this;
  this = NEW(NODE);
  if(this == NULL)
    return this;
  this->uid = id;
  this->next = NULL;
  return this;
}


PATH *make_path(NODE *s, NODE *t)
{ // add your code here
  PATH *this;
  this = NEW(PATH);
  if(this == NULL)
    return this;
  this->head = s;
  this->tail = t;
  this->head->next = this->tail;
  this->length = 2;
  this->next = NULL;
  return this;
}

PATH_ROOT *keep_paths()
{ // add your code here
  PATH_ROOT *this;
  this = NEW(PATH_ROOT);
  if(this == NULL)
    return this;
  this->head = NULL;
  this->number = 0;
  return this;
}

/***********************
*** other functions   **
***********************/

void add_user (int id1, ROOT *r)
{ USER *Tom;

  Tom = make_user(id1);
  if (Tom == NULL)
    exit(0);
  Tom->next = r->head;
  r->head = Tom;
  r->number++;
  assignPassword(r);
  return;
}


USER *copy_user (USER *u)
{ USER *this;
  int i;

  this = NEW(USER);
  this->uid = u->uid;
  for (i=0; u->password[i]!='\0'; i++)
    this->password[i] = u->password[i];
  this->password[i] = '\0';
  this->number = u->number; 
  this->next = u->next;
  this->friend = u->friend;

  return this;
}

void add_friendship(int id1, int id2, ROOT * r)
{ USER *Tom, *Jerry, *u1, *u2;
  
  for (Tom = r->head; Tom != NULL; Tom = Tom->next)
  { if (Tom->uid == id1)
    { for (Jerry = r->head; Jerry != NULL; Jerry = Jerry->next)
      { if (Jerry->uid == id2)
          break;
      }
      break;
    }
  }

  u1 = copy_user(Tom);
  u2 = copy_user(Jerry);

  u2->friend = Tom->friend;
  Tom->friend = u2;
  Tom->number++;

  u1->friend = Jerry->friend;
  Jerry->friend = u1;
  Jerry->number++;
	
  return;
}


void assignPassword(ROOT *r)
{ // add your code here
  static char characters[26+26+10+8+1]; // upper + lower + digits + special + null
  int i;
  for(i=0; i < 26; i++) characters[i] = i + 65; // 'A' == 65
  for(i=26; i < 52; i++) characters[i] = (i-26) + 97; // 'a' == 97
  for(i=52; i < 62; i++) characters[i] = (i-52) + 48; // '0' == 48
  characters[62] = '~'; characters[63] = '!'; characters[64] = '@';
  characters[65] = '#'; characters[66] = '$'; characters[67] = '%';
  characters[68] = '&'; characters[69] = '*';

  USER *this;
  this = r->head;
  int length;
  int upper = 0, lower = 0, digit = 0, special = 0;
  char random;

  while(lower == 0 || upper == 0 || digit == 0 || special == 0)
  { upper = 0;
    lower = 0;
    digit = 0;
    special = 0;
    length = rand()%9 + 8;
    for(i=0; i < length; i++)
    { random = rand()%70;
      this->password[i] = characters[random];
      if(random >= 0 && random <= 26)
        upper = 1;
      else if(random >= 26 && random <= 51)
        lower = 1;
      else if(random >= 52 && random <= 61)
        digit = 1;
      else if(random >= 62 && random <= 69) 
        special = 1;
    }
    this->password[i] = '\0';
  }

  return;
}


void writeOutput(FILE *f, ROOT *r, PATH_ROOT *p)
{ // add your code here

  USER *u;
  USER *v;

  // write users data
  for(u = r->head; u; u = u->next)
  { fprintf(f, "%06d: %16s, %2d,", u->uid, u->password, u->number);
    for(v = u->friend; v; v = v->friend)
    { if(v->friend == NULL)
        fprintf(f, " %06d.", v->uid);
      else
        fprintf(f, " %06d,", v->uid);

    }
    fprintf(f, "\n");
  }

  // write connections
  fprintf(f, "Connections of pairs of users");
  PATH *this;
  NODE *curr;
  int i = 1;
  for(this = p->head; this; this = this->next)
  { fprintf(f, "\n%d.", i);
    if(this->length == 0)
      fprintf(f, " %06d and %06d cannot be connected.", this->head->uid, this->tail->uid);
    else
    { for(curr = this->head; curr; curr = curr->next)
      { if(curr->next == NULL)
          fprintf(f, " %06d.", curr->uid);
        else
          fprintf(f, " %06d,", curr->uid);
      }

    }
    i++;
  }

  return;
}


USER *search_user(ROOT *r, int uid)
{ USER *this;
  // add your code here

  for (this = r->head; this != NULL; this = this->next)
  { if (this->uid == uid)
      break;
  }

  return this;
}


void print_Friends(USER *Tom)
{ // add your code here
  USER *this;
  if(Tom->friend == NULL)
  { printf("User %06d has no friends\n", Tom->uid);
    return;
  }

  printf("User %06d's friends, %d:", Tom->uid, Tom->number);
  for(this = Tom->friend; this != NULL; this = this->friend)
  { if(this->friend == NULL)
      printf(" %06d.", this->uid);
    else
      printf(" %06d,", this->uid);
  }
  printf("\n");
  return;
}


void add_a_friend(ROOT *r, int id1, int id2)
{ // add your code here
  USER *this;
  //check if friendship exists
  for(this = r->head; this && this->uid != id1 && this->uid != id2; this = this->next);
  for(this = this->friend; this && this->uid != id1 && this->uid != id2; this = this->friend);
  if(this != NULL)
  { printf("%06d and %06d are already friends.\n", id1, id2);
    return;
  }

  add_friendship(id1, id2, r);
  printf("%06d and %06d are now friends.\n", id1, id2);
  return;
}


int remove_friendship(ROOT *r, int id1, int id2)
{ // add your code here

  USER *Tom, *Jerry, *u, *prev;
  
  for (Tom = r->head; Tom != NULL; Tom = Tom->next)
  { if (Tom->uid == id1)
    { for (Jerry = r->head; Jerry != NULL; Jerry = Jerry->next)
      { if (Jerry->uid == id2)
          break;
      }
      break;
    }
  }

  for(u = Tom->friend, prev = Tom; u; prev = u, u = u->friend)
  { if(u->uid == id2)
    { //remove 
      prev->friend = u->friend;
      //free(u);
      break;
    }
  }
  if(u == NULL) return 0;
  for(u = Jerry->friend, prev = Jerry; u; prev = u, u = u->friend)
  { if(u->uid == id1)
    { //remove 
      prev->friend = u->friend;
      //free(u);
      break;
    }
  }
  return 1;
}


void remove_user(ROOT *r, int id1)
{ // add your code here
  USER *Tom, *prev, *v;
  for(Tom = r->head, prev = r->head; Tom; prev = Tom, Tom = Tom->next)
  { if(Tom->uid == id1)
    { for(v = Tom->friend; v; v = v->friend)
      { remove_friendship(r, id1, v->uid);
      }
      prev->next = Tom->next;
      free(Tom);
      break;
    }
  }
  printf("User %06d has been removed from MyTSN.\n", id1);
}


PATH *find_path(ROOT *r, int id1, int id2, PATH_ROOT *p)
{
  // breadth first search to find id2 distance from id1.

  USER *Tom = search_user(r, id1);
  USER *Jerry = search_user(r, id2);

  USER *tmp;
  USER *u;
  USER *v;

  for(tmp = r->head; tmp; tmp = tmp->next)
    tmp->number = -1;
  Tom->number = 0;
  tmp = Tom;
  int lvl = 0;
  int no_path = 0; // when all users assigned a number and Jerry hasn't been found, then no_path will remain as 1

  while(tmp->uid != Jerry->uid)
  { if(no_path == 1)
      break;
    no_path = 1;
    for(u = r->head; u; u = u->next)
    { // only check the current level
      if(u->number == lvl)
      { no_path = 0;
        for(v = u->friend; v; v = v->friend)
        {
          tmp = search_user(r, v->uid);
          if(tmp->number == -1) //protects from changing number of already discovered users
            tmp->number = u->number + 1;

          if(tmp->uid == Jerry->uid)
            break;
        }
        if(tmp->uid == Jerry->uid) break;
      }
    }
    lvl++;
  }

  NODE *s = make_node(Tom->uid);
  NODE *t = make_node(Jerry->uid);
  PATH *this = make_path(s, t);
  NODE *copy;

  if(no_path == 1)
  { printf("%06d and %06d cannot be connected\n", id1, id2);
    this->length = 0;
  }
  else
  { printf("PATH FOUND!\n");
    // Get Path -> go backwards using recorded numbers
    lvl = Jerry->number;
    this->length = lvl;
    u = Jerry;
    for(v = u->friend; v && u->uid != Tom->uid; v = v->friend)
    { tmp = search_user(r, v->uid);
      if(tmp->number == lvl-1)
        { // insert after head, first in last out
        if(tmp->uid != Tom->uid) // avoid duplicate tom in path
        { copy = make_node(tmp->uid);
          copy->next = this->head->next;
          this->head->next = copy;
        }
        u = tmp;
        v = u->friend; //restart loop with new user, depth first search.
      }
      lvl--;
    }
  }

  // add path to PATH_ROOT at tail
  PATH *last;
  if(p->head == NULL)
  { p->head = this;
  }
  else
  { for(last = p->head; last->next; last = last->next);
    last->next = this;
  }

  p->number++;
  // print shortest path
  /*printf("shortest path:");
  for(copy = this->head; copy; copy = copy->next)
  { if(copy->next == NULL)
      printf(" %06d.", copy->uid);
    else 
      printf(" %06d,", copy->uid);

  }*/

  friend_count(r); // restore user->number
  return this;
}

void friend_count(ROOT *r)
{ USER *u;
  USER *v;
  for(u = r->head; u; u = u->next)
  { u->number = 0;
    for(v = u->friend; v; v = v->friend)
      u->number++;
  }
  return;
}




