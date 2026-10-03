#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1024

typedef struct LineNode {
    char *data;
    struct LineNode *prev;
    struct LineNode *next;
} LineNode;

typedef struct {
    LineNode *head;
    LineNode *tail;
    LineNode *cursor;
    int cursor_idx;
} LineEditor;

LineEditor* create_editor() {
    LineEditor *ed = (LineEditor*)malloc(sizeof(LineEditor));
    ed->head = NULL;
    ed->tail = NULL;
    ed->cursor = NULL;
    ed->cursor_idx = 0;
    return ed;
}

void insert_after(LineEditor *ed, const char *text) {
    LineNode *node = (LineNode*)malloc(sizeof(LineNode));
    node->data = strdup(text);
    node->prev = NULL;
    node->next = NULL;

    if (ed->head == NULL) {
        ed->head = node;
        ed->tail = node;
        ed->cursor = node;
        ed->cursor_idx = 1;
    } else {
        node->next = ed->cursor->next;
        node->prev = ed->cursor;
        if (ed->cursor->next != NULL) {
            ed->cursor->next->prev = node;
        } else {
            ed->tail = node;
        }
        ed->cursor->next = node;
        ed->cursor = node;
        ed->cursor_idx++;
    }
}

void delete_line(LineEditor *ed) {
    if (ed->cursor == NULL) {
        printf("Error: No lines to delete.\n");
        return;
    }

    LineNode *target = ed->cursor;

    if (target->prev != NULL) {
        target->prev->next = target->next;
    } else {
        ed->head = target->next;
    }

    if (target->next != NULL) {
        target->next->prev = target->prev;
        ed->cursor = target->next;
    } else {
        ed->tail = target->prev;
        ed->cursor = target->prev;
        if (ed->cursor_idx > 1) ed->cursor_idx--;
    }

    free(target->data);
    free(target);

    if (ed->head == NULL) {
        ed->cursor_idx = 0;
    }
}

void print_buffer(LineEditor *ed) {
    if (ed->head == NULL) {
        printf("--- Buffer Empty ---\n");
        return;
    }
    LineNode *curr = ed->head;
    int idx = 1;
    while (curr != NULL) {
        if (curr == ed->cursor) {
            printf("> %3d: %s\n", idx, curr->data);
        } else {
            printf("  %3d: %s\n", idx, curr->data);
        }
        curr = curr->next;
        idx++;
    }
}

void free_editor(LineEditor *ed) {
    LineNode *curr = ed->head;
    while (curr != NULL) {
        LineNode *next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;
    }
    free(ed);
}

int main() {
    LineEditor *ed = create_editor();
    char input[MAX_LINE_LEN];

    printf("=========================================\n");
    printf("     Simple C Line Editor Initialized     \n");
    printf("     Type 'H' for list of commands.      \n");
    printf("=========================================\n\n");

    while (1) {
        printf("editor> ");
        if (!fgets(input, sizeof(input), stdin)) break;

        input[strcspn(input, "\r\n")] = 0;
        if (strlen(input) == 0) continue;

        char cmd = input[0];

        if (cmd == 'Q' || cmd == 'q') {
            printf("Exiting editor.\n");
            break;
        } else if (cmd == 'I' || cmd == 'i') {
            printf("Enter line text: ");
            char text[MAX_LINE_LEN];
            if (fgets(text, sizeof(text), stdin)) {
                text[strcspn(text, "\r\n")] = 0;
                insert_after(ed, text);
            }
        } else if (cmd == 'D' || cmd == 'd') {
            delete_line(ed);
        } else if (cmd == 'P' || cmd == 'p') {
            print_buffer(ed);
        } else if (cmd == 'N' || cmd == 'n') {
            if (ed->cursor && ed->cursor->next) {
                ed->cursor = ed->cursor->next;
                ed->cursor_idx++;
            } else {
                printf("Already at bottom of file.\n");
            }
        } else if (cmd == 'B' || cmd == 'b') {
            if (ed->cursor && ed->cursor->prev) {
                ed->cursor = ed->cursor->prev;
                ed->cursor_idx--;
            } else {
                printf("Already at top of file.\n");
            }
        } else if (cmd == 'H' || cmd == 'h') {
            printf("\nCommands:\n");
            printf("  I - Insert line after cursor\n");
            printf("  D - Delete current line\n");
            printf("  P - Print entire buffer\n");
            printf("  N - Move cursor down (Next line)\n");
            printf("  B - Move cursor up (Back/Previous line)\n");
            printf("  Q - Quit\n\n");
        } else {
            printf("Unknown command. Type 'H' for help.\n");
        }
    }

    free_editor(ed);
    return 0;
}