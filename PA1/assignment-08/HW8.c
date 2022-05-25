#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>


#define MAX_BRANCHES             3
#define DECORATION_NONE          0
#define DECORATION_CANDLE        1
#define DECORATION_SPARKLER      2


typedef struct TNode
{
  struct TNode   * m_Parent;
  struct TNode   * m_Branches[MAX_BRANCHES];
  int              m_Decoration;
} TNODE;
#endif /* __PROGTEST__ */

TNODE* newNode (void) {
  TNODE* node = (TNODE*) malloc(sizeof(TNODE));
  for (int i = 0; i < 3; i++) {
    node->m_Branches[i] = NULL;
  }
  node->m_Parent = NULL;
  node->m_Decoration = DECORATION_NONE;
  return node;
}

int fire (TNODE* root) {
    if (root->m_Parent) {
      switch (root->m_Decoration) {
      case DECORATION_SPARKLER:
        if (root->m_Parent->m_Decoration == DECORATION_CANDLE)
          return 1;
        for (int i = 0; i < 3; i++) {
          if (root->m_Parent->m_Branches[i]) {
            if (root->m_Parent->m_Branches[i]->m_Decoration == DECORATION_CANDLE) {
              return 1;
            }
          }
        }  
        break;
      case DECORATION_CANDLE:
        if (root->m_Parent->m_Decoration == DECORATION_SPARKLER)
          return 1;
        for (int i = 0; i < 3; i++) {
          if (root->m_Parent->m_Branches[i]) {
            if (root->m_Parent->m_Branches[i]->m_Decoration == DECORATION_SPARKLER) {
              return 1;
            }
          }
        }  
      break;
      default:
        return 0;
    }
  }
  return 0;
}

int isLeaf (TNODE* node) {
  if (node->m_Branches[0] == NULL &&
      node->m_Branches[1] == NULL &&
      node->m_Branches[2] == NULL)
    return 1;
  return 0;
}

void               destroyTree        ( TNODE           * root )
{
  /* TODO */
  if (root == NULL) {
    return;
  }

  if (!isLeaf(root)) {
    for (int i = 0; i < 3; i++) {
      if (root->m_Branches[i] != NULL ) {
        destroyTree(root->m_Branches[i]);
        root->m_Branches[i] = NULL;
      }
    }
  }

  root->m_Parent = NULL;
  free(root);
}

int                easyToCatchFire    ( TNODE           * root )
{
  if (!(root)) {
    return 0;
  }
  

  int static fire1 = 0;
  fire1 = fire(root);
  if (root) {
    for (int i = 0; i < 3; i++) {
        if (root->m_Branches[i]) {
            easyToCatchFire(root->m_Branches[i]);
          }
        }
    }        
  return fire1;
}

int                cutBranch          ( TNODE          ** root,
                                        const char      * path )
{
  /* TODO */
  int static invalid = 1;
  
  if (!(*root)) {
    invalid = 0;
    return 0;
  }

  printf("%c\n", *path);

  switch (*path)
  {
  case '0':
    cutBranch (&(*root)->m_Branches[0], path + 1);
    break;
  case '1':
    cutBranch (&(*root)->m_Branches[1], path + 1);
    break;
  case '2':
    cutBranch (&(*root)->m_Branches[2], path + 1);    
    break;
  case '\0':
    destroyTree(*root);
    (*root) = NULL;
    invalid = 1;
    break;
  default:
    invalid = 0;
    break;
  }

  return invalid;
}

int                setDecoration      ( TNODE          ** root,
                                        const char      * path,
                                        int               decor )
{
  /* TODO */
  int static invalid = 1;
  int len = strlen(path);
  int start = 1;

  if (start) {
    for (int i = 0; i < len; i++) {
      printf("%c\n", path[i]);
      if (path[i] != '0' && path[i] != '1' && path[i] != '2') {
        return 0;
      }  
    }
    start = 0;
  }

  if (!(*root)) {
    *root = newNode();
  }

  printf("%c\n", *path);

  switch (*path)
  {
  case '0':
    setDecoration (&(*root)->m_Branches[0], path + 1, decor);
    (*root)->m_Branches[0]->m_Parent = (*root);
    break;
  case '1':
    setDecoration (&(*root)->m_Branches[1], path + 1, decor);
    (*root)->m_Branches[1]->m_Parent = (*root);
    break;
  case '2':
    setDecoration (&(*root)->m_Branches[2], path + 1, decor);    
    (*root)->m_Branches[2]->m_Parent = (*root);
    break;
  case '\0':
    (*root)->m_Decoration = decor;
    break;
  default:
    invalid = 0;
    break;
  }

  return invalid;
}

#ifndef __PROGTEST__
int                main               ( void )
{
  TNODE * r;

  r = NULL;
  assert ( setDecoration ( &r, "000", DECORATION_SPARKLER ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "001", DECORATION_SPARKLER ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "002", DECORATION_SPARKLER ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "1", DECORATION_CANDLE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] != NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
           && r -> m_Branches[1] -> m_Parent == r
           && r -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[1] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "01", DECORATION_NONE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] != NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[1] -> m_Parent == r
           && r -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[1] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "", DECORATION_CANDLE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] != NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[1] -> m_Parent == r
           && r -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[1] -> m_Branches[2] == NULL );
 
  assert ( easyToCatchFire ( r ) == 0 );
 destroyTree ( r ); 

  r = NULL;
  assert ( setDecoration ( &r, "000", DECORATION_SPARKLER ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "002", DECORATION_CANDLE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "2", DECORATION_CANDLE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL
           && r -> m_Branches[2] -> m_Parent == r
           && r -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[2] -> m_Branches[2] == NULL );
  assert ( easyToCatchFire ( r ) == 0 );
  destroyTree ( r ); 

  r = NULL;
  assert ( setDecoration ( &r, "0001", DECORATION_SPARKLER ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "000", DECORATION_CANDLE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
  assert ( easyToCatchFire ( r ) == 1 );
  destroyTree ( r ); 

  r = NULL;
  assert ( setDecoration ( &r, "012001", DECORATION_SPARKLER ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "012002", DECORATION_CANDLE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] -> m_Branches[0] -> m_Branches[2] -> m_Branches[2] == NULL );
  assert ( easyToCatchFire ( r ) == 1 );
  assert ( cutBranch ( &r, "0120" ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL );
  assert ( easyToCatchFire ( r ) == 0 );
  destroyTree ( r ); 

  r = NULL;
  assert ( setDecoration ( &r, "0123", DECORATION_SPARKLER ) == 0 );
  assert ( r == NULL );
  assert ( cutBranch ( &r, "012" ) == 0 );
  assert ( r == NULL );
  assert ( easyToCatchFire ( r ) == 0 );
  destroyTree ( r ); 

  r = NULL;
  assert ( setDecoration ( &r, "012", DECORATION_SPARKLER ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL );
  assert ( setDecoration ( &r, "011", DECORATION_CANDLE ) == 1 );
  assert ( r != NULL
           && r -> m_Parent == NULL
           && r -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] != NULL
           && r -> m_Branches[1] == NULL
           && r -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Parent == r
           && r -> m_Branches[0] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Parent == r -> m_Branches[0]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Decoration == DECORATION_NONE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] != NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Decoration == DECORATION_CANDLE
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[1] -> m_Branches[2] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Parent == r -> m_Branches[0] -> m_Branches[1]
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Decoration == DECORATION_SPARKLER
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[0] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[1] == NULL
           && r -> m_Branches[0] -> m_Branches[1] -> m_Branches[2] -> m_Branches[2] == NULL );
  assert ( easyToCatchFire ( r ) == 1 );
  assert ( cutBranch ( &r, "" ) == 1 );
  assert ( r == NULL );
  assert ( easyToCatchFire ( r ) == 0 );
  assert ( cutBranch ( &r, "" ) == 0 );
  assert ( r == NULL );
  destroyTree ( r ); 

  return 0;
}
#endif /* __PROGTEST__ */