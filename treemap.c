#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    //new->lower_than = lower_than;
    return NULL;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode* aux = tree->root;
  
  if (tree->root == NULL){
    aux = createTreeNode(key, value);
    tree->root = aux;
    tree->current = aux;
  }
  
  while(aux->left != NULL && aux->right != NULL){
    
    if (tree->lower_than(key, aux->pair->key) == 1){
      aux = aux->left;
      
    }else{
      if(tree->lower_than(aux->pair->key, key) == 1){
        aux = aux->right;
        
      }else{
        return;
      }
      tree->current = aux;
    }

}
    tree->current = aux;
    
    if(aux->left == NULL || aux->right == NULL){
      aux = createTreeNode(key, value);
      aux->parent = tree->current->parent;
      
      if(tree->lower_than(key, aux->parent->pair->key) == 1){
        aux->parent->left = aux;
        
      }else{
        aux->parent->right = aux;
      }
      
      tree->current = aux;
      return;
    }
}

///////////////////////////////////////////////////////////////
TreeNode * minimum(TreeNode * x){
  while(x->left != NULL){
    x = x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  if (tree == NULL || tree->root == NULL){
    return;
  }

  if (node ==  tree->root){
    if (node->left != NULL){
      tree->root = node->left;
      node = NULL;
    }else
      if(node->right != NULL){
        tree->root = node->right;

        node = node->left;
        node = NULL;
      }else{
        if (node->right != NULL){
          node = node->right;
          node = NULL;
        }else{
          tree->root = NULL;
          node = NULL;
        }
      }
      
  }else{
    TreeNode* hijo = node->right;
    hijo = minimum(hijo);
    node->left->parent = hijo;
    node->right->parent = hijo;
    hijo->left = node->left;
    hijo->right = node->right;
    eraseTreeMap(tree, hijo->pair->key);
    tree->root = hijo;
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {

    TreeNode* aux = tree->root;
    while(aux != NULL){
      if(tree->lower_than (key, aux-> pair->key) == 1){
        aux = aux->left;
        }else{
          if (tree->lower_than (aux-> pair->key, key) == 1){
            aux =aux->right;
          }else{
              tree->current = aux;
              return aux->pair;
            }
          }
      }
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode* aux = tree->root;
    TreeNode* ub_node = tree->root;
    while(aux != NULL){
      //Si es el que se se busca es menor que el aux, cambia el valor del ub_node por el hijo derecho de aux
      if(tree->lower_than (key, aux-> pair->key) == 1){
        aux = aux->left;
        if (ub_node == tree->root){
          aux = aux->left;
          continue;
          }
        
        }else{ //si no, pregunta si es el que se busca es mayor al aux
          if (tree->lower_than (aux->pair->key, key) == 1){ //Si lo es avanza el aux, sin cambiar el ub_node
            
            aux = aux->right;
            
            if (ub_node == tree->root){
              ub_node = aux;
              continue;
            }
            if (tree->lower_than (aux->pair->key, ub_node->pair->key) == 1){
              ub_node = aux;
            }else{
              continue;
            }
          }
          else{
            return aux->pair;
          }
        }
    }
  aux = ub_node;
  return aux->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode* node = tree->root;
    while(node->left != NULL){
      node = node->left;
      tree->current = node;
    }
    return tree->current->pair;
}

////////////////////////////////////////
Pair * nextTreeMap(TreeMap * tree) {
    if(tree->current->right != NULL){
      tree->current = tree->current->right;
      return tree->current->pair;
      
    }else{
      
      while(1){
        
        if (tree->lower_than(tree->current->pair->key, tree->current->parent->pair->key) == 1){
          
          tree->current = tree->current->parent;
          return tree->current->pair;
        }else{
          tree->current = tree->current->parent;
          tree->current->parent = tree->current->parent->parent; 
        }
      }
    }
    return NULL;
}
