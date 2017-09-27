// Omkar Kulkarni, 09-27-2017, Period 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char symbol;
    int freq;
    struct Node* left;
    struct Node* right;
} TreeNode;

void build_encoding(TreeNode* root, char path[], char map[256][256]){
    if(root -> symbol != '*'){
        strcpy(map[(int)(root -> symbol)], path);
        return;
    }
    char newstr[1234] = "";
    sprintf(newstr, "%s0", path);
    if(root -> left != NULL)
        build_encoding(root -> left, newstr, map);
    sprintf(newstr, "%s1", path);
    if(root -> right != NULL)
        build_encoding(root -> right, newstr, map);
}

int main(int argc, char* argv[]){
    FILE* fin = fopen("encode.txt", "r");
    TreeNode* tree[12345] = {NULL};
    int freqs[256] = {0};
    int numbytes;
    int numchars = 0;
    int numnodes = 0;
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
            numnodes++;
        }
    }
    int one = 0;
    int two = 0;
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
    while(tree[one] -> freq != numchars){
        while(tree[one] == NULL || tree[two] == NULL){
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
        }
        TreeNode* comb = (TreeNode*)malloc(sizeof(TreeNode));
        comb -> freq = (tree[one] -> freq + tree[two] -> freq);
        comb -> symbol = '*';
        comb -> left = tree[one];
        comb -> right = tree[two];
        tree[two] = NULL;
        tree[one] = comb;
        two = 0;
        one = 0;
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
    }
    TreeNode* root = tree[one];
    char map[256][256];
    build_encoding(root, "", map);
    printf("%d\n", numnodes);
    fin = fopen("encode.txt", "r");
    for(int i = 0; i < 256; i++){
        if(map[i][0] != 0){
            printf("%c%s\n", i, map[i]);
        }
    }
    while(1){
        numbytes = fread( &ch, sizeof(char), 1, fin);
        if(numbytes == 0) break;
        printf("%s", map[(int)ch]);
    }
    printf("\n");
}

/**************************************************
17
 000000000
H000000001
K00000001
O0000001
a1111
d0000010
e0000011
f000010
i000011
k0001
l001
m010
n0110
o110
r10
u0111
w1110
00000000100000110010011100000000001110110100010000010000000000000010101100100000000000000001010000111111000000000000000001011100100011111100110000011
**************************/
