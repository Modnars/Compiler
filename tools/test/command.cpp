#include <cstdio>
#include <cstdlib>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int opt; 
    while ((opt = getopt(argc, argv, "a:il:n:s:r:")) != -1) {
        switch (opt) {
            case 'a':
                printf("%d a\n", opt);
                printf("%s\n", optarg);
                break;
            case 'l':
                printf("%d l\n", opt);
                printf("%s\n", optarg);
                break;
            case 'i':
                printf("%d i\n", opt);
                printf("%s\n", optarg);
                break;
            case 's':
                printf("%d s\n", opt);
                printf("%s\n", optarg);
                break;
            case 'n':
                printf("%d n\n", opt);
                printf("%s\n", optarg);
                break;
            default:
                printf("Default\n");
        }
    }
    return EXIT_SUCCESS;
}
