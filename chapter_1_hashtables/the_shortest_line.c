#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100

int shortest_line_index(int lines[], int numberOfLines) {
    int shortest = 0;
    for(int lineIndex = 1; lineIndex < numberOfLines; lineIndex++) {
        if(lines[lineIndex] < lines[shortest]) {
            shortest = lineIndex;
        }
    }
    return shortest;
}

void solve(int lines[], int numberOfLines, int numberOfJoining) {
    int shortest;
    for(int i = 0; i < numberOfJoining; i++) {
        shortest = shortest_line_index(lines, numberOfLines);
        printf("%d\n", lines[shortest]);
        lines[shortest]++;
    }
}

int main(void) {
    int lines[MAX_LINES];
    int numberOfLines, numberOfJoining;
    scanf("%d%d", &numberOfLines, &numberOfJoining);
    for(int line = 0; line < numberOfLines; line++) {
        scanf("%d", &lines[line]);
    }
    solve(lines, numberOfLines, numberOfJoining);
    return 0;
}