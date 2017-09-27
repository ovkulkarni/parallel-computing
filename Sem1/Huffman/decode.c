#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[]){
    FILE* fin = fopen("encoded_assignment.txt", "r");
    char tree[12345] = {0};
    int freqs[256] = {0};
    char ch;
    char act;
    int numlines = 17;
    int currline = 0;
    int numbytes;
    int index = 1;
    fread( &ch, sizeof(char), 1, fin);
    fread( &ch, sizeof(char), 1, fin);
    fread( &ch, sizeof(char), 1, fin);
    while(1){
        numbytes = fread( &ch, sizeof(char), 1, fin);
        if(numbytes == 0) break;
        if(ch == '0') index = 2*index;
        else if (ch == '1') index = ((2*index)+1);
        else if(ch != '\n'){
            act = ch;
        }
        else if(ch == '\n'){
            currline++;
            // printf("Index: %d, Char: %c\n", index, act);
            tree[index] = act;
            index = 1;
        }
        if(currline == numlines) break;
    }
    index = 1;
    int enc_bits = 0;
    int dec_bits = 0;
    while(1){
        numbytes = fread( &ch, sizeof(char), 1, fin);
        if(numbytes == 0) break;
        enc_bits += 1;
        if(ch == '\n') break;
        if(ch == '0') index = (2*index);
        else if(ch == '1') index = ((2*index)+1);
        if(tree[index] != 0){
            printf("%c", tree[index]);
            freqs[(int)tree[index]] += 1;
            dec_bits += 8;
            index = 1;
        }
    }
    printf("\n");
    fclose(fin);
    printf("Compression Ratio: %f\n", (dec_bits*1.0 - enc_bits*1.0)/(dec_bits*1.0));
    int numchars = 0;
    for(int i = 1; i < 128; i++){
        if(freqs[i] != 0){
            // printf("Frequency of %c: %d\n", i, freqs[i]);
            numchars += freqs[i];
        }
    }
    float sbits = 0.0;
    for(int i = 1; i < 128; i++){
        if(freqs[i] != 0){
            sbits += (freqs[i] * -1.0 * (log((freqs[i]*1.0/numchars))/log(2)));
        }
    }
    printf("Shannon Bits: %f\n", sbits);
}
