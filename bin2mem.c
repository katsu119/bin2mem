#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printBinary(unsigned char c) {
    for (int i = 7; i >= 0; i--) {
        printf("%d", (c >> i) & 1);
    }
}
void writeBinary(FILE *file,unsigned char c) {
    for (int i = 7; i >= 0; i--) {
        fprintf(file,"%d", (c >> i) & 1);
    }
}
void printHex(unsigned char c) {
    short c_l,c_h;
	c_l = c & 0x0F;
	c_h = (c & 0xF0)>>4;
        printf("%x%x",c_h,c_l);
}

void writeHex(FILE *file,unsigned char c) {
    short c_l,c_h;
	c_l = c & 0x0F;
	c_h = (c & 0xF0)>>4;
        fprintf(file,"%x%x",c_h,c_l);
}


void print_help(const char *program_name) {
    printf("Usage: %s [options]\n", program_name);
    printf("\n");
    printf("Options:\n");
    printf("  --help          Display this help message and exit.\n");
    printf("  -b,-h           Print the output in binary or hexadecimal format.\n");
    printf("  -n <number>\n");
    printf("                  Set the number of bytes per line (default is 16).\n");
    printf("  -o <filename>\n");
    printf("                  Specify the output file name. If not specified,\n");
    printf("                  output is sent to the standard output.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  bin2mem main.bin -h -n 4 -o main.mem\n");
    printf(" 00000033\n 00000033\n 00108093\n 0000a103\n 00100073\n");
    printf("\n");
    printf("Please report bugs to <katsu119@foxmail.com>.\n");
    exit(EXIT_SUCCESS);
}


// config_format: 
// b -- binary formart
// h -- hex format
char config_format = 'b';

short config_bytes_perline = 1;

int main(int argc, char *argv[]) {
    if (argc < 1) {
        fprintf(stderr, "Usage: %s <input_file> -o <output_file>\n", argv[0]);
        return EXIT_FAILURE;
    }
    
    const char *output_filename = "a.mem";
    const char *in_filename = argv[1];
    for(int i = 1; i < argc; ++i) {
        if(!strcmp("-o", argv[i])){
            if(i+1 < argc){
                output_filename = argv[i+1];
                ++i;
            }
        } else if(!strcmp("--help", argv[i])){
            print_help(argv[0]);
        } else if(!strcmp("-b", argv[i])){
            config_format = 'b';
        } else if(!strcmp("-h", argv[i])){
            config_format = 'h';
        } else if(!strcmp("-n", argv[i])){
            if(i+1 < argc) {
                config_bytes_perline = atoi(argv[i+1]);
                ++i;
            }
        } else {
            in_filename = argv[i];
        }


    }
    FILE *in_file = fopen(in_filename, "rb");  // 打开文件用于读取二进制数据
    if (in_file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    FILE *out_file = fopen(output_filename, "w");
    if(out_file == NULL) {
        perror("Error: can't open output file");
        return EXIT_FAILURE;
    }


    char ch;
    short bytes_counter = 0;
    char * in_buffer = (char *)malloc(config_bytes_perline);
    if(in_buffer == NULL){
        perror("Memory insufficient");
        return 1;
    }

    while (!feof(in_file)) {
        memset(in_buffer, 0, config_bytes_perline);
        size_t bytes_read = fread(in_buffer, 1, config_bytes_perline, in_file);
        if(bytes_read == 0) {
            if(!feof(in_file)) {
                perror("Error: read input file failed");
                return 1;
            }
        }
        for(int i = config_bytes_perline-1; i >= 0; --i){
            ch = in_buffer[i];
            if(config_format == 'b') {
                writeBinary(out_file, ch);
            } else{
                writeHex(out_file, ch); 
            }
        }

        fputc('\n',out_file);
    }

    fclose(in_file);  // 关闭文件
    fclose(out_file);  // 关闭文件
    free(in_buffer);
    return EXIT_SUCCESS;
}
