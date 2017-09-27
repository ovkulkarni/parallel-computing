#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    char symbol;
    int freq;
    struct Node* left;
    struct Node* right;
} TreeNode;

int main(int argc, char* argv[]){
    FILE* fin = fopen("encode.txt", "r");
    TreeNode* tree[12345] = {NULL};
    int freqs[256] = {0};
    int numbytes;
    int numchars = 0;
    char ch;
    while(1){
        numbytes = fread( &ch, sizeof(char), 1, fin);
        if(numbytes == 0) break;
        freqs[(int)ch] += 1;
        numchars++;
    }
    int curr = 0;
    for(int i = 0; i < 256; i++){
        if(freqs[i] != 0){
            TreeNode* t = (TreeNode*)malloc( sizeof(TreeNode) );
            t -> symbol = ((char)i);
            t -> freq = freqs[i];
            t -> right = NULL;
            t -> left = NULL;
            tree[i] = t;
        }
    }
    int one = 0;
    int two = 0;
    while(tree[one] != NULL && tree[one] -> freq != numchars){
        for(int i = 0; i < 256; i++){
            if(tree[i] != NULL && ((tree[one] == NULL || tree[two] == NULL) || tree[i] -> freq < tree[two] -> freq)){
                if(tree[one] == NULL){
                    one = i;
                }
                else if(tree[two] == NULL){
                    two = i;
                }
                else if(tree[i] -> freq < tree[one] -> freq){
                    one = i;
                }
                else if(tree[i] -> freq < tree[two] -> freq){
                    two = i;
                }
            }
        }
        TreeNode* comb = (TreeNode*)malloc(sizeof(TreeNode));
        comb -> freq = (tree[one] -> freq + tree[two] -> freq);
        comb -> symbol = '*';
        comb -> left = tree[one];
        comb -> right = tree[two];
        printf("%c -> %d", comb -> symbol, comb -> freq);
        tree[two] = NULL;
        tree[one] = comb;
    }
    printf("%c -> %d\n", tree[one] -> symbol, tree[one] -> freq);
    printf("%c -> %d\n", tree[two] -> symbol, tree[two] -> freq);
}
